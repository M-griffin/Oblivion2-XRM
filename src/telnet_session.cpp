#include "telnet_session.hpp"

#include <string>
#include <fstream>
#include <unordered_set>

#include "telnet.hpp"
#include "session.hpp"
#include "logging.hpp"

TelnetSession::TelnetSession(Session &session)
    : m_log(Logging::getInstance())
      , m_session(session)
      , m_nawsRow(24)
      , m_nawsCol(80)
      , m_termType("undetected")
      , m_isBinary(false)
      , m_isEcho(false)
      , m_isSga(false)
      , m_isLinemode(false)
      , m_isNawsDetected(false)
      , m_isUseAnsi(false)
      , m_teloptStage(0)
      , m_teloptCommand(0)
      , m_currentOption(0)
      , m_subnegoOption(0) {
    m_replySequence.clear();
}

TelnetSession::~TelnetSession() {
    m_log.log(Logging::LogLevel::Console, "~TelnetSession()");
    m_replySequence.clear();
}

void TelnetSession::sendIACSequences(Byte command, Byte option) {
    // Don't send if we've already negotiated this option
    if (checkReply(option)) {
        return;
    }

    ByteBuffer buf = {IAC, command, option};
    m_session.send(buf); // Set to Bytes

    addReply(option);
}

bool TelnetSession::checkReply(Byte option) {
    return m_replySequence.find(option) != m_replySequence.end();
}

void TelnetSession::addReply(Byte option) {
    m_replySequence.insert(option);
}

int TelnetSession::getTermRows() const {
    return m_nawsRow;
}

int TelnetSession::getTermCols() const {
    return m_nawsCol;
}

void TelnetSession::setTermRows(const int value) {
    m_nawsRow = value;
}

void TelnetSession::setTermCols(const int value) {
    m_nawsCol = value;
}

std::string TelnetSession::getTermType() const {
    return m_termType;
}

void TelnetSession::setUseAnsi(bool value) {
    m_isUseAnsi = value;
}

bool TelnetSession::getUseAnsi() const {
    return m_isUseAnsi;
}

Byte TelnetSession::telnetOptionAcknowledge(Byte command) {
    switch (command) {
        case DO: return WILL;
        case WILL: return DO;
        case DONT: return WONT;
        case WONT: return DONT;
        default: break;
    }
    return 0;
}

Byte TelnetSession::telnetOptionDeny(Byte command) {
    switch (command) {
        case DO: return WONT;
        case WILL: return DONT;
        case DONT: return WONT;
        case WONT: return DONT;
        default: break;
    }
    return 0;
}

void TelnetSession::decodeBuffer() {
    m_log.log(Logging::LogLevel::Debug, "decodeBuffer 240 - SE received");

    switch (m_subnegoOption) {
        case TELOPT_NAWS:
            if (m_dataSequence.size() >= 4) {
                m_nawsCol = (static_cast<uint16_t>(m_dataSequence[0]) << 8)
                            | static_cast<uint16_t>(m_dataSequence[1]);

                m_nawsRow = (static_cast<uint16_t>(m_dataSequence[2]) << 8)
                            | static_cast<uint16_t>(m_dataSequence[3]);

                m_log.log(Logging::LogLevel::Debug, "TELOPT_NAWS option", m_nawsCol, "x", m_nawsRow);
                m_isNawsDetected = true;
            }
            break;

        case TELOPT_TTYPE: {
            std::string termType(
                reinterpret_cast<const char *>(m_dataSequence.data()),
                m_dataSequence.size()
            );
            m_termType = termType;
            m_log.log(Logging::LogLevel::Debug, "TELOPT_TTYPE option", m_termType);
            break;
        }

        case TELOPT_NEW_ENVIRON:
            m_log.log(Logging::LogLevel::Debug, "TELOPT_NEW_ENVIRON data", m_dataSequence);
            break;

        case TELOPT_LINEMODE:
            m_log.log(Logging::LogLevel::Debug, "TELOPT_LINEMODE data", m_dataSequence);
            break;

        default:
            m_log.log(Logging::LogLevel::Debug, "Invalid option:", static_cast<int>(m_subnegoOption), m_subnegoOption);
            break;
    }

    m_teloptStage = 0;
    m_subnegoOption = 0;
    m_dataSequence.clear();
}

bool TelnetSession::isValidCommand(const unsigned char command) {
    return command == DO || command == DONT || command == WILL || command == WONT || command == SB;
}

void TelnetSession::handleDoDont(const unsigned char command, const unsigned char option) {
    if (checkReply(option)) {
        return;
    }

    switch (option) {
        case TELOPT_ECHO:
            m_isEcho = (command == DO);
            sendIACSequences(telnetOptionAcknowledge(command), option);
            break;
        case TELOPT_BINARY:
            m_isBinary = (command == DO);
            sendIACSequences(telnetOptionAcknowledge(command), option);
            break;
        case TELOPT_SGA:
            m_isSga = (command == DO);
            sendIACSequences(telnetOptionAcknowledge(command), option);
            break;
        case TELOPT_LINEMODE:
            m_isLinemode = (command == DO);
            sendIACSequences(telnetOptionAcknowledge(command), option);
            break;
        default:
            sendIACSequences(telnetOptionDeny(command), option);
            break;
    }
}

void TelnetSession::handleWillWont(const unsigned char command, const unsigned char option) {
    if (checkReply(option)) {
        return;
    }

    switch (option) {
        case TELOPT_ECHO:
            m_isEcho = (command == WILL);
            sendIACSequences(telnetOptionAcknowledge(command), option);
            break;
        case TELOPT_BINARY:
            m_isBinary = (command == WILL);
            sendIACSequences(telnetOptionAcknowledge(command), option);
            break;
        case TELOPT_SGA:
            m_isSga = (command == WILL);
            sendIACSequences(telnetOptionAcknowledge(command), option);
            break;
        case TELOPT_LINEMODE:
            m_isLinemode = (command == WILL);
            sendIACSequences(telnetOptionAcknowledge(command), option);
            break;
        case TELOPT_TTYPE:
            sendTTYPERequest();
            break;
        default:
            sendIACSequences(telnetOptionDeny(command), option);
            break;
    }
}

void TelnetSession::sendTTYPERequest() {
    if (checkReply(TELOPT_TTYPE)) {
        return;
    }

    ByteBuffer buf = {IAC, SB, TELOPT_TTYPE, TELQUAL_SEND, IAC, SE};
    m_session.send(buf);

    addReply(TELOPT_TTYPE);
}

void TelnetSession::sendENVRequest() {
    if (checkReply(TELOPT_NEW_ENVIRON)) {
        return;
    }

    std::stringstream stm;
    std::vector<std::string> vars = {
        "USER", "TERM", "SHELL", "COLUMNS", "LINES",
        "C_CTYPE", "XTERM_LOCALE", "DISPLAY", "SSH_CLIENT",
        "SSH_CONNECTION", "SSH_TTY", "HOME", "HOSTNAME",
        "PWD", "MAIL", "LANG", "PWD", "UID", "USER_ID",
        "EDITOR", "LOGNAME", "SYSTEMTYPE"
    };

    stm << static_cast<uint8_t>(IAC)
            << static_cast<uint8_t>(SB)
            << static_cast<uint8_t>(TELOPT_NEW_ENVIRON)
            << static_cast<uint8_t>(TELQUAL_SEND);

    for (auto &v: vars) {
        stm << static_cast<uint8_t>(NEW_ENV_VAR) << v.data();
    }

    stm << static_cast<uint8_t>(IAC) << static_cast<uint8_t>(SE);
    const std::string buf = stm.str();
    stm.clear();
    vars.clear();

    m_session.send(stm.str());

    addReply(TELOPT_NEW_ENVIRON);
}

ByteBuffer TelnetSession::telnetOptionParse(Byte byte) {
    ByteBuffer output;

    switch (m_teloptStage) {
        case 0:
            if (byte != IAC) {
                output.push_back(byte);
                return output;
            }
            m_teloptStage++;
            break;

        case 1:
            if (byte == IAC && m_isBinary) {
                m_log.log(Logging::LogLevel::Debug, "Got double IAC BINARY");
                m_teloptStage = 0;
                output.push_back(byte);
                return output;
            } else if (byte == IAC) {
                m_log.log(Logging::LogLevel::Debug, "Got double IAC");
                break;
            }

            if (!isValidCommand(byte)) {
                m_log.log(Logging::LogLevel::Debug, "Invalid command:", static_cast<int>(byte));
                m_teloptStage = 0;
                break;
            }

            m_teloptCommand = byte;
            m_teloptStage++;
            break;

        case 2:
            m_log.log(Logging::LogLevel::Debug, "[IAC]", static_cast<int>(byte), "STAGE 2");

            switch (m_teloptCommand) {
                case DO: handleDoDont(DO, byte);
                    break;
                case DONT: handleDoDont(DONT, byte);
                    break;
                case WILL: handleWillWont(WILL, byte);
                    break;
                case WONT: handleWillWont(WONT, byte);
                    break;
                case SB:
                    m_subnegoOption = byte;
                    if (byte == TELOPT_TTYPE || byte == TELOPT_NEW_ENVIRON) {
                        m_currentOption = byte;
                        m_teloptStage = 3;
                    } else if (byte == TELOPT_NAWS) {
                        m_currentOption = byte;
                        m_teloptStage = 5;
                    } else if (byte == TELOPT_LINEMODE) {
                        m_currentOption = byte;
                        m_teloptStage = 7;
                    } else {
                        m_teloptStage = 0;
                    }
                    break;
                default:
                    m_teloptStage = 0;
                    break;
            }
            break;

        case 3:
            m_log.log(Logging::LogLevel::Debug, "--> STAGE 3", static_cast<int>(byte));

            switch (m_currentOption) {
                case TELOPT_TTYPE:
                    if (byte == TELQUAL_IS) {
                        m_log.log(Logging::LogLevel::Debug, "[IAC] TELQUAL_IS", static_cast<int>(m_currentOption),
                                                        static_cast<int>(byte));
                        m_teloptStage = 4;
                    } else {
                        m_teloptStage = 0;
                    }
                    break;

                case TELOPT_NEW_ENVIRON:
                    if (byte == TELQUAL_IS) {
                        m_log.log(Logging::LogLevel::Debug, "[IAC] TELQUAL_IS", static_cast<int>(m_currentOption),
                                                        static_cast<int>(byte));
                        m_teloptStage = 6;
                    } else {
                        m_teloptStage = 0;
                    }
                    break;

                default:
                    if (byte == SE) {
                        m_log.log(Logging::LogLevel::Debug, "[IAC] SB END", static_cast<int>(m_currentOption),
                                                        static_cast<int>(byte));
                        m_teloptStage = 0;
                    } else {
                        m_teloptStage = 0;
                    }
                    break;
            }
            break;

        case 4:
            m_log.log(Logging::LogLevel::Debug, "--> STAGE 4 TTYPE", static_cast<int>(byte));

            if (byte != IAC && byte != SE) {
                if (byte == '\x00')
                    m_dataSequence.push_back('\0');
                else
                    m_dataSequence.push_back(byte);

                if (m_dataSequence.size() >= SB_MAXLEN) {
                    m_dataSequence.clear();
                }
            }

            if (byte == IAC) {
                m_teloptStage = 1;
            } else if (byte == SE) {
                m_teloptStage = 0;
                decodeBuffer();
            }
            break;

        case 5:
            m_log.log(Logging::LogLevel::Debug, "--> STAGE 5 NAWS", static_cast<int>(byte));

            if (byte != IAC && byte != SE) {
                if (byte == '\x00')
                    m_dataSequence.push_back('\0');
                else
                    m_dataSequence.push_back(byte);

                if (m_dataSequence.size() >= SB_MAXLEN) {
                    m_dataSequence.clear();
                }
            }

            if (byte == IAC) {
                m_teloptStage = 1;
            } else if (byte == SE) {
                m_teloptStage = 0;
                decodeBuffer();
            }
            break;

        case 6:
            m_log.log(Logging::LogLevel::Debug, "--> STAGE 6 TELOPT_NEW_ENVIRON", static_cast<int>(byte));

            if (byte != IAC && byte != SE) {
                if (byte == '\x00')
                    m_dataSequence.push_back(' ');
                else
                    m_dataSequence.push_back(byte);

                if (m_dataSequence.size() >= SB_MAXLEN) {
                    m_dataSequence.clear();
                    m_teloptStage = 0;
                }
            }

            if (byte == IAC) {
                m_teloptStage = 1;
            } else if (byte == SE) {
                m_teloptStage = 0;
                decodeBuffer();
            }
            break;

        case 7:
            m_log.log(Logging::LogLevel::Debug, "--> STAGE 7 TELOPT_LINEMODE", static_cast<int>(byte));

            if (byte != IAC && byte != SE) {
                m_dataSequence.push_back(byte);

                if (m_dataSequence.size() >= SB_MAXLEN) {
                    m_dataSequence.clear();
                }
            }

            if (byte == IAC) {
                m_teloptStage = 1;
            } else if (byte == SE) {
                m_teloptStage = 0;
                m_dataSequence.clear();
                sendIACSequences(telnetOptionDeny(WONT), TELOPT_LINEMODE);
            }
            break;

        default:
            break;
    }

    return {};
}
