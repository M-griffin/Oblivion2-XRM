#include "telnet_session.hpp"
#include "telnet.hpp"
#include "session.hpp"
#include "logging.hpp"

#include <sstream>
#include <cstring>

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
      , m_isUtf8(false)
      , m_isCP437(true)
      , m_teloptStage(DATA)
      , m_teloptCommand(0)
      , m_currentOption(0)
      , m_subnegoOption(0) {
    m_log.log(Logging::LogLevel::Console, "TelnetSession()");
    m_replySequence.clear();
    m_dataSequence.clear();
}

TelnetSession::~TelnetSession() {
    m_log.log(Logging::LogLevel::Console, "~TelnetSession()");
    m_replySequence.clear();
    m_dataSequence.clear();
}

void TelnetSession::sendIACSequences(Byte command, Byte option) {
    if (checkReply(option)) return;
    ByteBuffer buf = {IAC, command, option};
    m_session.send(buf);
    addReply(option);
}

// Check if we already responded to this option
bool TelnetSession::checkReply(Byte option) {
    return m_replySequence.find(option) != m_replySequence.end();
}

void TelnetSession::addReply(Byte option) {
    m_replySequence.insert(option);
}

int TelnetSession::getTermRows() const { return m_nawsRow; }
int TelnetSession::getTermCols() const { return m_nawsCol; }
void TelnetSession::setTermRows(int value) { m_nawsRow = value; }
void TelnetSession::setTermCols(int value) { m_nawsCol = value; }

std::string TelnetSession::getTermType() const { return m_termType; }
void TelnetSession::setUseAnsi(bool value) { m_isUseAnsi = value; }
bool TelnetSession::getUseAnsi() const { return m_isUseAnsi; }

Byte TelnetSession::telnetOptionAcknowledge(Byte command) {
    switch (command) {
        case DO: return WILL;
        case DONT: return WONT;
        case WILL: return DO;
        case WONT: return DONT;
        default: return 0;
    }
}

Byte TelnetSession::telnetOptionDeny(Byte command) {
    switch (command) {
        case DO: return WONT;
        case DONT: return WONT;
        case WILL: return DONT;
        case WONT: return DONT;
        default: return 0;
    }
}

// ====================== Decode Subnegotiation ======================
void TelnetSession::decodeBuffer() {
    switch (m_subnegoOption) {
        case TELOPT_NAWS:
            if (m_dataSequence.size() >= 4) {
                m_nawsCol = (uint16_t(m_dataSequence[0]) << 8) | uint16_t(m_dataSequence[1]);
                m_nawsRow = (uint16_t(m_dataSequence[2]) << 8) | uint16_t(m_dataSequence[3]);
                m_isNawsDetected = true;

                m_log.log(Logging::LogLevel::Info,
                          "NAWS detected: cols=%d, rows=%d", m_nawsCol, m_nawsRow);
            } else {
                m_log.log(Logging::LogLevel::Warn,
                          "NAWS subnegotiation too short, length=%zu", m_dataSequence.size());
            }
            break;

        case TELOPT_TTYPE:
            if (!m_dataSequence.empty()) {
                m_termType.assign(reinterpret_cast<const char *>(m_dataSequence.data()), m_dataSequence.size());
                m_log.log(Logging::LogLevel::Info,
                          "Terminal type received: %s", m_termType.c_str());

                // Detect ANSI support
                m_isUseAnsi = (m_termType.find("ansi") != std::string::npos ||
                               m_termType.find("xterm") != std::string::npos ||
                               m_termType.find("vt") != std::string::npos);

                // Detect UTF-8 vs CP437
                m_isUtf8 = (m_termType.find("utf") != std::string::npos ||
                            m_termType.find("UTF") != std::string::npos);
                m_isCP437 = !m_isUtf8;
            } else {
                m_log.log(Logging::LogLevel::Warn,
                          "TTYPE subnegotiation empty");
            }
            break;

        case TELOPT_NEW_ENVIRON:
            m_log.log(Logging::LogLevel::Info,
                      "New ENVIRON subnegotiation received, length=%zu", m_dataSequence.size());
            break;

        default:
            m_log.log(Logging::LogLevel::Info,
                      "Unhandled subnegotiation: option=%d, length=%zu", m_subnegoOption, m_dataSequence.size());
            break;
    }

    m_dataSequence.clear();
    m_subnegoOption = 0;
}

// ====================== Telnet Option Handlers ======================
bool TelnetSession::isValidCommand(Byte command) {
    return command == DO || command == DONT || command == WILL || command == WONT || command == SB;
}

void TelnetSession::handleDoDont(Byte command, Byte option) {
    if (command == DO) {
        m_log.log(Logging::LogLevel::Info, "DO received for option %d", option);
        handleWillWont(WILL, option); // server WILL to comply
    } else if (command == DONT) {
        m_log.log(Logging::LogLevel::Info, "DONT received for option %d", option);
        handleWillWont(WONT, option); // server WONT to comply
    } else if (command == WILL) {
        m_log.log(Logging::LogLevel::Info, "WILL received for option %d", option);
        handleWillWont(WILL, option);
    } else if (command == WONT) {
        m_log.log(Logging::LogLevel::Info, "WONT received for option %d", option);
        handleWillWont(WONT, option);
    }
}

void TelnetSession::handleWillWont(Byte command, Byte option) {
    switch (option) {
        case TELOPT_NAWS:
            if (command == WILL) {
                m_isNawsDetected = true;
                m_log.log(Logging::LogLevel::Info,
                          "NAWS detected from client, accepting window size.");
                sendIACSequences(DO, TELOPT_NAWS);
                addReply(TELOPT_NAWS);
            } else if (command == WONT) {
                m_isNawsDetected = false;
                m_log.log(Logging::LogLevel::Info, "Client WONT NAWS");
            }
            break;

        case TELOPT_TTYPE:
            if (command == WILL) {
                m_log.log(Logging::LogLevel::Info, "TTYPE WILL received, sending request");
                sendTTYPERequest();
            } else if (command == WONT) {
                m_log.log(Logging::LogLevel::Info, "Client WONT TTYPE");
            }
            break;

        case TELOPT_ECHO:
        case TELOPT_SGA:
        case TELOPT_BINARY:
            // Accept these by default
            if (command == WILL) {
                sendIACSequences(DO, option);
                addReply(option);
                m_log.log(Logging::LogLevel::Info, "Accepted WILL for option %d", option);
            } else if (command == WONT) {
                sendIACSequences(DONT, option);
                addReply(option);
                m_log.log(Logging::LogLevel::Info, "Client WONT option %d", option);
            }
            break;

        default:
            // Reject unknown options
            if (command == WILL) {
                sendIACSequences(DONT, option);
                m_log.log(Logging::LogLevel::Info, "Rejected WILL option %d", option);
            } else if (command == WONT) {
                sendIACSequences(DO, option);
                m_log.log(Logging::LogLevel::Info, "Rejected WONT option %d", option);
            }
            break;
    }
}

void TelnetSession::handleSubnegotiation(Byte option, const ByteBuffer &data) {
    switch (option) {
        case TELOPT_NAWS:
            if (data.size() >= 4) {
                int cols = (data[0] << 8) | data[1];
                int rows = (data[2] << 8) | data[3];
                setTermCols(cols);
                setTermRows(rows);
                m_log.log(Logging::LogLevel::Info,
                          "NAWS received: cols=%d, rows=%d", cols, rows);
            } else {
                m_log.log(Logging::LogLevel::Warn, "NAWS subnegotiation too short");
            }
            break;

        case TELOPT_TTYPE:
            if (!data.empty() && data[0] == TELQUAL_IS) {
                std::string ttype(data.begin() + 1, data.end());
                m_termType = ttype;
                m_log.log(Logging::LogLevel::Info,
                          "TTYPE received: %s", ttype.c_str());
            }
            break;

        default:
            m_log.log(Logging::LogLevel::Info,
                      "Unhandled subnegotiation for option %d, length %zu",
                      option, data.size());
            break;
    }
}

// ====================== Requests ======================
void TelnetSession::sendTTYPERequest() {
    ByteBuffer buf = {IAC, SB, TELOPT_TTYPE, TELQUAL_SEND, IAC, SE};
    m_session.send(std::string(buf.begin(), buf.end()));
    m_log.log(Logging::LogLevel::Info, "Sent TTYPE request to client");
}

void TelnetSession::sendENVRequest() {
    if (checkReply(TELOPT_NEW_ENVIRON)) return;

    std::stringstream stm;
    std::vector<std::string> vars = {
        "USER", "TERM", "SHELL", "COLUMNS", "LINES", "C_CTYPE", "XTERM_LOCALE",
        "DISPLAY", "SSH_CLIENT", "SSH_CONNECTION", "SSH_TTY", "HOME", "HOSTNAME",
        "PWD", "MAIL", "LANG", "PWD", "UID", "USER_ID", "EDITOR", "LOGNAME", "SYSTEMTYPE"
    };

    stm << static_cast<uint8_t>(IAC)
            << static_cast<uint8_t>(SB)
            << static_cast<uint8_t>(TELOPT_NEW_ENVIRON)
            << static_cast<uint8_t>(TELQUAL_SEND);

    for (auto &v: vars)
        stm << static_cast<uint8_t>(NEW_ENV_VAR) << v.data();

    stm << static_cast<uint8_t>(IAC) << static_cast<uint8_t>(SE);
    std::string buf = stm.str();
    m_session.send(buf);
    addReply(TELOPT_NEW_ENVIRON);

    m_log.log(Logging::LogLevel::Info, "Sent NEW_ENVIRON request");
}

// ====================== Main Parser ======================
ByteBuffer TelnetSession::telnetOptionParse(Byte byte) {
    ByteBuffer appBytes;

    switch (m_teloptStage) {
        case DATA:
            if (byte == IAC) {
                m_teloptStage = IAC_SEEN;
            } else {
                appBytes.push_back(byte); // normal data
            }
            break;

        case IAC_SEEN:
            switch (byte) {
                case IAC:
                    appBytes.push_back(IAC); // escaped IAC
                    m_teloptStage = DATA;
                    break;
                case DO:
                case DONT:
                case WILL:
                case WONT:
                    m_teloptCommand = byte;
                    m_teloptStage = COMMAND;
                    break;
                case SB:
                    m_teloptStage = SB_OPTION;
                    break;
                case SE:
                    // Unexpected SE
                    m_log.log(Logging::LogLevel::Warn, "Unexpected IAC SE");
                    m_teloptStage = DATA;
                    break;
                default:
                    m_log.log(Logging::LogLevel::Info,
                              "Received IAC unknown command: %d", byte);
                    m_teloptStage = DATA;
                    break;
            }
            break;

        case COMMAND:
            handleDoDont(m_teloptCommand, byte);
            m_teloptStage = DATA;
            break;

        case SB_OPTION:
            m_currentOption = byte;
            m_dataSequence.clear();
            m_teloptStage = SB_DATA;
            m_log.log(Logging::LogLevel::Info,
                      "Subnegotiation started for option %d", m_currentOption);
            break;

        case SB_DATA:
            if (byte == IAC) {
                m_teloptStage = SB_IAC;
            } else {
                m_dataSequence.push_back(byte);
            }
            break;

        case SB_IAC:
            if (byte == SE) {
                // End of subnegotiation
                handleSubnegotiation(m_currentOption, m_dataSequence);
                m_teloptStage = DATA;
            } else if (byte == IAC) {
                m_dataSequence.push_back(IAC); // escaped IAC
                m_teloptStage = SB_DATA;
            } else {
                m_log.log(Logging::LogLevel::Warn,
                          "Unexpected byte after IAC in SB: %d", byte);
                m_teloptStage = DATA;
            }
            break;
    }

    return appBytes;
}
