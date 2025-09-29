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
    std::stringstream stm;

    stm << static_cast<char>(IAC);
    stm << command;
    stm << static_cast<char>(option);
    const std::string buf = stm.str();
    stm.clear();
    m_session.send(buf);

    if (!checkReply(option)) {
        addReply(option);
    }
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
                m_nawsCol = (static_cast<unsigned char>(m_dataSequence[0]) << 8) | static_cast<unsigned char>(m_dataSequence[
                              1]);
                m_nawsRow = (static_cast<unsigned char>(m_dataSequence[2]) << 8) | static_cast<unsigned char>(m_dataSequence[
                              3]);
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
}

// Helper method to validate Telnet commands
bool TelnetSession::isValidCommand(const unsigned char command) {
    return command == DO || command == DONT || command == WILL || command == WONT || command == SB;
}

// Helper method to handle DO/DONT commands
void TelnetSession::handleDoDont(const unsigned char command, const unsigned char option) {
    switch (option) {
        case TELOPT_ECHO: m_isEcho = (command == DO);
            break;
        case TELOPT_BINARY: m_isBinary = (command == DO);
            break;
        case TELOPT_SGA: m_isSga = (command == DO);
            break;
        case TELOPT_LINEMODE: m_isLinemode = (command == DO);
            break;
        default:
            sendIACSequences(telnetOptionDeny(command), option);
            break;
    }
}

// Helper method to handle WILL/WONT commands
void TelnetSession::handleWillWont(const unsigned char command, const unsigned char option) {
    switch (option) {
        case TELOPT_ECHO: m_isEcho = (command == WILL);
            break;
        case TELOPT_BINARY: m_isBinary = (command == WILL);
            break;
        case TELOPT_SGA: m_isSga = (command == WILL);
            break;
        case TELOPT_LINEMODE: m_isLinemode = (command == WILL);
            break;
        case TELOPT_TTYPE: sendTTYPERequest();
            break;
        default:
            sendIACSequences(telnetOptionDeny(command), option);
            break;
    }
}

// Main Telnet option parsing method
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

        // Handle subnegotiation stages (unchanged)
        case 3:
            m_log.write<Logging::DEBUG_LOG>("--> STAGE 3", static_cast<int>(c));

            //Options will be 1 After SB
            switch (m_currentOption) {
                // IAC SB TTYPE TELQUAL_IS
                case TELOPT_TTYPE:
                    if (c == TELQUAL_IS) {
                        m_log.write<Logging::DEBUG_LOG>("[IAC] TELQUAL_IS", static_cast<int>(m_currentOption), static_cast<int>(c));
                        m_teloptStage = 4;
                    } else
                        m_teloptStage = 0;

                    break;

                case TELOPT_NEW_ENVIRON:
                    if (c == TELQUAL_IS) {
                        m_log.write<Logging::DEBUG_LOG>("[IAC] TELQUAL_IS", static_cast<int>(m_currentOption), static_cast<int>(c));
                        m_teloptStage = 6;
                    } else
                        m_teloptStage = 0;

                    break;

                default:

                    //printf("\r\n [Stage 3 - unregistered stuff it] - %i, %i \r\n",opt, c);
                    if (c == SE) {
                        m_log.write<Logging::DEBUG_LOG>("[IAC] SB END", static_cast<int>(m_currentOption), static_cast<int>(c));
                        m_teloptStage = 0;
                    } else {
                        // reset
                        m_teloptStage = 0;
                    }

                    break;
            }

            break;

        // Only Gets here on TTYPE Sub-Negotiation.
        case 4:
            m_log.write<Logging::DEBUG_LOG>("--> STAGE 4 TTYPE", static_cast<int>(c));

            if (c != IAC && c != SE) {
                if (c == '\x00')
                    m_dataSequence += '\0';
                else
                    m_dataSequence += static_cast<char>(c);

                if (m_dataSequence.size() >= SB_MAXLEN) {
                    // Invalid Sequences,, just clear.
                    m_dataSequence.clear();
                }
            }

            // check for End of Sequence IAC SE.
            if (c == IAC) {
                // IAC then 240 to close sequence for TTYPE
                m_teloptStage = 1;
            }

            /*
            else if (c == SE)
            {
                //data_sequence.clear();
                m_teloptStage = 0;
            }*/
            break;

        case 5:
            m_log.write<Logging::DEBUG_LOG>("--> STAGE 5 NAWS", static_cast<int>(c));

            if (c != IAC && c != SE) {
                if (c == '\x00')
                    m_dataSequence += '\0';
                else
                    m_dataSequence += static_cast<char>(c);

                if (m_dataSequence.size() >= SB_MAXLEN) {
                    // Invalid Sequences
                    m_dataSequence.clear();
                }
            }

            // check for End of Sequence IAC SE.
            if (c == IAC) {
                // IAC then 240 to close sequence for NAWS
                m_teloptStage = 1;
            }

            /*
            else if (c == SE)
            {
                //data_sequence.clear();
                m_teloptStage = 0;
            }*/
            break;

        case 6:
            m_log.write<Logging::DEBUG_LOG>("--> STAGE 5 TELOPT_NEW_ENVIRON", static_cast<int>(c));

            if (c != IAC && c != SE) {
                if (c == '\x00')
                    m_dataSequence += ' ';
                else
                    m_dataSequence += static_cast<char>(c);

                if (m_dataSequence.size() >= SB_MAXLEN) {
                    // Invalid Sequences
                    m_dataSequence.clear();
                    m_teloptStage = 0;
                }
            }

            // check for End of Sequence IAC SE.
            if (c == IAC) {
                // IAC then 240 to close sequence for NAWS
            } else if (c == SE) {
                //data_sequence.clear();
                m_teloptStage = 0;
            }

            break;

        case 7:
            /*
            * Right now were just parsing out the data received
            * so we can reply back WON'T so it will toggle LINEMODE off!
            * LINUX/BSD console telnet's default to LINEMODE ON, and ignore
            * DONT and WONT requests until we initiate the discussion with DO.
            * Then we we received this Sub Options, which we need to parse out.
            *
            * Notes: Add some Extra Variables.  If Binary, We need to parse
            * and ignore double IAC!  they are Unicode chars.
            * then only look for IAC / 240 for SE to exit.
            */
            m_log.write<Logging::DEBUG_LOG>("--> STAGE 5 TELOPT_LINEMODE", static_cast<int>(c));

            if (c != IAC && c != SE) {
                m_dataSequence += c;

                if (m_dataSequence.size() >= SB_MAXLEN) {
                    // Invalid Sequences, just clear.
                    m_dataSequence.clear();
                }
            }

            // check for End of Sequence IAC SE.
            if (c == IAC) {
                // IAC then 240 to close sequence for NAWS
            } else if (c == SE) {
                m_teloptStage = 0;
                m_dataSequence.erase();
                // All Done, Now it's ok to turn off LINEMODE!
                sendIACSequences(telnetOptionDeny(WONT), TELOPT_LINEMODE);
            }

            break;
        default: ;
    }

    return '\0';
}

void TelnetSession::sendTTYPERequest() {
    std::stringstream stm;
    stm << static_cast<char>(IAC) << static_cast<char>(SB)
            << static_cast<char>(TELOPT_TTYPE) << static_cast<char>(TELQUAL_SEND)
            << static_cast<char>(IAC) << static_cast<char>(SE);
    const std::string buf = stm.str();
    stm.clear();
    m_session.send(buf);
}

void TelnetSession::sendENVRequest() {
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
}
