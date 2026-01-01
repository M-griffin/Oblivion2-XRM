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
    m_log.write<Logging::DEBUG_LOG>("~TelnetDecoder()");
    m_replySequence.clear();
}

void TelnetSession::sendIACSequences(const unsigned char command, const int option) {
    // Don't send if we've already negotiated this option
    if (checkReply(option)) {
        return;
    }

    std::stringstream stm;
    stm << static_cast<char>(IAC);
    stm << command;
    stm << static_cast<char>(option);
    const std::string buf = stm.str();
    stm.clear();
    m_session.send(buf);

    addReply(option);
}

bool TelnetSession::checkReply(const unsigned char &option) const {
    return m_replySequence.find(option) != m_replySequence.end();
}

void TelnetSession::addReply(const unsigned char &option) {
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

unsigned char TelnetSession::telnetOptionAcknowledge(const unsigned char &command) {
    switch (command) {
        case DO: return WILL;
        case WILL: return DO;
        case DONT: return WONT;
        case WONT: return DONT;
        default: break;
    }
    return 0;
}

unsigned char TelnetSession::telnetOptionDeny(const unsigned char &command) {
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
    m_log.write<Logging::DEBUG_LOG>("decodeBuffer 240 - SE received");

    switch (m_subnegoOption) {
        case TELOPT_NAWS:
            if (m_dataSequence.size() >= 4) {
                m_nawsCol = (static_cast<unsigned char>(m_dataSequence[0]) << 8) | static_cast<unsigned char>(
                                m_dataSequence[1]);
                m_nawsRow = (static_cast<unsigned char>(m_dataSequence[2]) << 8) | static_cast<unsigned char>(
                                m_dataSequence[3]);
                m_log.write<Logging::DEBUG_LOG>("TELOPT_NAWS option", m_nawsCol, "x", m_nawsRow);
                m_isNawsDetected = true;
            }
            break;

        case TELOPT_TTYPE:
            m_termType = m_dataSequence;
            m_log.write<Logging::DEBUG_LOG>("TELOPT_TTYPE option", m_termType);
            break;

        case TELOPT_NEW_ENVIRON:
            m_log.write<Logging::DEBUG_LOG>("TELOPT_NEW_ENVIRON data", m_dataSequence);
            break;

        case TELOPT_LINEMODE:
            m_log.write<Logging::DEBUG_LOG>("TELOPT_LINEMODE data", m_dataSequence);
            break;

        default:
            m_log.write<Logging::DEBUG_LOG>("Invalid option:", static_cast<int>(m_subnegoOption), m_subnegoOption);
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

    std::stringstream stm;
    stm << static_cast<char>(IAC) << static_cast<char>(SB)
            << static_cast<char>(TELOPT_TTYPE) << static_cast<char>(TELQUAL_SEND)
            << static_cast<char>(IAC) << static_cast<char>(SE);
    const std::string buf = stm.str();
    stm.clear();
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

    stm << static_cast<char>(IAC)
            << static_cast<char>(SB)
            << static_cast<char>(TELOPT_NEW_ENVIRON)
            << static_cast<char>(TELQUAL_SEND);

    for (auto &v: vars) {
        stm << static_cast<char>(NEW_ENV_VAR) << v.data();
    }

    stm << static_cast<char>(IAC) << static_cast<char>(SE);
    const std::string buf = stm.str();
    stm.clear();
    vars.clear();
    m_session.send(buf);
    addReply(TELOPT_NEW_ENVIRON);
}

unsigned char TelnetSession::telnetOptionParse(const unsigned char &c) {
    switch (m_teloptStage) {
        case 0:
            if (c != IAC) {
                return c;
            }
            m_teloptStage++;
            break;

        case 1:
            if (c == IAC && m_isBinary) {
                m_log.write<Logging::DEBUG_LOG>("Got double IAC BINARY");
                m_teloptStage = 0;
                return IAC;
            } else if (c == IAC) {
                m_log.write<Logging::DEBUG_LOG>("Got double IAC");
                break;
            }

            if (!isValidCommand(c)) {
                m_log.write<Logging::DEBUG_LOG>("Invalid command:", static_cast<int>(c));
                m_teloptStage = 0;
                break;
            }

            m_teloptCommand = c;
            m_teloptStage++;
            break;

        case 2:
            m_log.write<Logging::DEBUG_LOG>("[IAC]", static_cast<int>(c), "STAGE 2");

            switch (m_teloptCommand) {
                case DO: handleDoDont(DO, c);
                    break;
                case DONT: handleDoDont(DONT, c);
                    break;
                case WILL: handleWillWont(WILL, c);
                    break;
                case WONT: handleWillWont(WONT, c);
                    break;
                case SB:
                    m_subnegoOption = c;
                    if (c == TELOPT_TTYPE || c == TELOPT_NEW_ENVIRON) {
                        m_currentOption = c;
                        m_teloptStage = 3;
                    } else if (c == TELOPT_NAWS) {
                        m_currentOption = c;
                        m_teloptStage = 5;
                    } else if (c == TELOPT_LINEMODE) {
                        m_currentOption = c;
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
            m_log.write<Logging::DEBUG_LOG>("--> STAGE 3", static_cast<int>(c));

            switch (m_currentOption) {
                case TELOPT_TTYPE:
                    if (c == TELQUAL_IS) {
                        m_log.write<Logging::DEBUG_LOG>("[IAC] TELQUAL_IS", static_cast<int>(m_currentOption),
                                                        static_cast<int>(c));
                        m_teloptStage = 4;
                    } else {
                        m_teloptStage = 0;
                    }
                    break;

                case TELOPT_NEW_ENVIRON:
                    if (c == TELQUAL_IS) {
                        m_log.write<Logging::DEBUG_LOG>("[IAC] TELQUAL_IS", static_cast<int>(m_currentOption),
                                                        static_cast<int>(c));
                        m_teloptStage = 6;
                    } else {
                        m_teloptStage = 0;
                    }
                    break;

                default:
                    if (c == SE) {
                        m_log.write<Logging::DEBUG_LOG>("[IAC] SB END", static_cast<int>(m_currentOption),
                                                        static_cast<int>(c));
                        m_teloptStage = 0;
                    } else {
                        m_teloptStage = 0;
                    }
                    break;
            }
            break;

        case 4:
            m_log.write<Logging::DEBUG_LOG>("--> STAGE 4 TTYPE", static_cast<int>(c));

            if (c != IAC && c != SE) {
                if (c == '\x00')
                    m_dataSequence += '\0';
                else
                    m_dataSequence += static_cast<char>(c);

                if (m_dataSequence.size() >= SB_MAXLEN) {
                    m_dataSequence.clear();
                }
            }

            if (c == IAC) {
                m_teloptStage = 1;
            } else if (c == SE) {
                m_teloptStage = 0;
                decodeBuffer();
            }
            break;

        case 5:
            m_log.write<Logging::DEBUG_LOG>("--> STAGE 5 NAWS", static_cast<int>(c));

            if (c != IAC && c != SE) {
                if (c == '\x00')
                    m_dataSequence += '\0';
                else
                    m_dataSequence += static_cast<char>(c);

                if (m_dataSequence.size() >= SB_MAXLEN) {
                    m_dataSequence.clear();
                }
            }

            if (c == IAC) {
                m_teloptStage = 1;
            } else if (c == SE) {
                m_teloptStage = 0;
                decodeBuffer();
            }
            break;

        case 6:
            m_log.write<Logging::DEBUG_LOG>("--> STAGE 6 TELOPT_NEW_ENVIRON", static_cast<int>(c));

            if (c != IAC && c != SE) {
                if (c == '\x00')
                    m_dataSequence += ' ';
                else
                    m_dataSequence += static_cast<char>(c);

                if (m_dataSequence.size() >= SB_MAXLEN) {
                    m_dataSequence.clear();
                    m_teloptStage = 0;
                }
            }

            if (c == IAC) {
                m_teloptStage = 1;
            } else if (c == SE) {
                m_teloptStage = 0;
                decodeBuffer();
            }
            break;

        case 7:
            m_log.write<Logging::DEBUG_LOG>("--> STAGE 7 TELOPT_LINEMODE", static_cast<int>(c));

            if (c != IAC && c != SE) {
                m_dataSequence += c;

                if (m_dataSequence.size() >= SB_MAXLEN) {
                    m_dataSequence.clear();
                }
            }

            if (c == IAC) {
                m_teloptStage = 1;
            } else if (c == SE) {
                m_teloptStage = 0;
                m_dataSequence.erase();
                sendIACSequences(telnetOptionDeny(WONT), TELOPT_LINEMODE);
            }
            break;

        default:
            break;
    }

    return '\0';
}
