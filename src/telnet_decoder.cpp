#include "telnet_decoder.hpp"

#include <string>
#include <fstream>
#include <unordered_set>

#include "telnet.hpp"
#include "session.hpp"
#include "logging.hpp"

TelnetDecoder::TelnetDecoder(Session &session)
    : log(Logging::getInstance())
      , session(session)
      , nawsRow(24)
      , nawsCol(80)
      , termType("undetected")
      , isBinary(false)
      , isEcho(false)
      , isSga(false)
      , isLinemode(false)
      , isNawsDetected(false)
      , teloptStage(0)
      , teloptCommand(0)
      , currentOption(0)
      , subnegoOption(0) {
    replySequence.clear();
}

TelnetDecoder::~TelnetDecoder() {
    log.write<Logging::DEBUG_LOG>("~TelnetDecoder()");
    replySequence.clear();
}

void TelnetDecoder::sendIACSequences(unsigned char command, int option) {
    std::stringstream stm;

    stm << static_cast<char>(IAC);
    stm << command;
    stm << static_cast<char>(option);
    const std::string buf = stm.str();
    stm.clear();
    session.send(buf);

    if (!checkReply(option)) {
        addReply(option);
    }
}

bool TelnetDecoder::checkReply(const unsigned char &option) const {
    return replySequence.find(option) != replySequence.end();
}

void TelnetDecoder::addReply(const unsigned char &option) {
    replySequence.insert(option);
}

int TelnetDecoder::getTermRows() const {
    return nawsRow;
}

int TelnetDecoder::getTermCols() const {
    return nawsCol;
}

void TelnetDecoder::setTermRows(int value) {
    nawsRow = value;
}

void TelnetDecoder::setTermCols(int value) {
    nawsCol = value;
}

std::string TelnetDecoder::getTermType() const {
    return termType;
}

unsigned char TelnetDecoder::telnetOptionAcknowledge(const unsigned char &command) {
    switch (command) {
        case DO: return WILL;
        case WILL: return DO;
        case DONT: return WONT;
        case WONT: return DONT;
        default: break;
    }
    return 0;
}

unsigned char TelnetDecoder::telnetOptionDeny(const unsigned char &command) {
    switch (command) {
        case DO: return WONT;
        case WILL: return DONT;
        case DONT: return WONT;
        case WONT: return DONT;
        default: break;
    }
    return 0;
}

void TelnetDecoder::decodeBuffer() {
    log.write<Logging::DEBUG_LOG>("decodeBuffer 240 - SE received");

    switch (subnegoOption) {
        case TELOPT_NAWS:
            if (dataSequence.size() >= 4) {
                nawsCol = ((unsigned char) dataSequence[0] << 8) | (unsigned char) dataSequence[1];
                nawsRow = ((unsigned char) dataSequence[2] << 8) | (unsigned char) dataSequence[3];
                log.write<Logging::DEBUG_LOG>("TELOPT_NAWS option", nawsCol, "x", nawsRow);
                isNawsDetected = true;
            }
            break;

        case TELOPT_TTYPE:
            termType = dataSequence;
            log.write<Logging::DEBUG_LOG>("TELOPT_TTYPE option", termType);
            break;

        case TELOPT_NEW_ENVIRON:
            log.write<Logging::DEBUG_LOG>("TELOPT_NEW_ENVIRON data", dataSequence);
            break;

        case TELOPT_LINEMODE:
            log.write<Logging::DEBUG_LOG>("TELOPT_LINEMODE data", dataSequence);
            break;

        default:
            log.write<Logging::DEBUG_LOG>("Invalid option:", (int) subnegoOption, subnegoOption);
            break;
    }

    teloptStage = 0;
    subnegoOption = 0;
}

// Helper method to validate Telnet commands
bool TelnetDecoder::isValidCommand(unsigned char command) {
    return command == DO || command == DONT || command == WILL || command == WONT || command == SB;
}

// Helper method to handle DO/DONT commands
void TelnetDecoder::handleDoDont(unsigned char command, unsigned char option) {
    switch (option) {
        case TELOPT_ECHO: isEcho = (command == DO);
            break;
        case TELOPT_BINARY: isBinary = (command == DO);
            break;
        case TELOPT_SGA: isSga = (command == DO);
            break;
        case TELOPT_LINEMODE: isLinemode = (command == DO);
            break;
        default:
            sendIACSequences(telnetOptionDeny(command), option);
            break;
    }
}

// Helper method to handle WILL/WONT commands
void TelnetDecoder::handleWillWont(unsigned char command, unsigned char option) {
    switch (option) {
        case TELOPT_ECHO: isEcho = (command == WILL);
            break;
        case TELOPT_BINARY: isBinary = (command == WILL);
            break;
        case TELOPT_SGA: isSga = (command == WILL);
            break;
        case TELOPT_LINEMODE: isLinemode = (command == WILL);
            break;
        case TELOPT_TTYPE: sendTTYPERequest();
            break;
        default:
            sendIACSequences(telnetOptionDeny(command), option);
            break;
    }
}

// Main Telnet option parsing method
unsigned char TelnetDecoder::telnetOptionParse(const unsigned char &c) {
    switch (teloptStage) {
        case 0:
            if (c != IAC) {
                return c;
            } else {
                teloptStage++;
            }
            break;

        case 1:
            if (c == IAC && isBinary) {
                log.write<Logging::DEBUG_LOG>("Got double IAC BINARY");
                teloptStage = 0;
                return IAC;
            } else if (c == IAC) {
                log.write<Logging::DEBUG_LOG>("Got double IAC");
                break;
            }

            if (!isValidCommand(c)) {
                log.write<Logging::DEBUG_LOG>("Invalid command:", (int) c);
                teloptStage = 0;
                break;
            }

            teloptCommand = c;
            teloptStage++;
            break;

        case 2:
            log.write<Logging::DEBUG_LOG>("[IAC]", (int) c, "STAGE 2");

            switch (teloptCommand) {
                case DO: handleDoDont(DO, c);
                    break;
                case DONT: handleDoDont(DONT, c);
                    break;
                case WILL: handleWillWont(WILL, c);
                    break;
                case WONT: handleWillWont(WONT, c);
                    break;
                case SB:
                    subnegoOption = c;
                    if (c == TELOPT_TTYPE || c == TELOPT_NEW_ENVIRON) {
                        currentOption = c;
                        teloptStage = 3;
                    } else if (c == TELOPT_NAWS) {
                        currentOption = c;
                        teloptStage = 5;
                    } else if (c == TELOPT_LINEMODE) {
                        currentOption = c;
                        teloptStage = 7;
                    } else {
                        teloptStage = 0;
                    }
                    break;
                default:
                    teloptStage = 0;
                    break;
            }
            break;

        // Handle subnegotiation stages (unchanged)
        case 3:
            log.write<Logging::DEBUG_LOG>("--> STAGE 3", (int)c);

            //Options will be 1 After SB
            switch(currentOption)
            {
                // IAC SB TTYPE TELQUAL_IS
                case TELOPT_TTYPE:
                    if(c == TELQUAL_IS)
                    {
                        log.write<Logging::DEBUG_LOG>("[IAC] TELQUAL_IS", (int)currentOption, (int)c);
                        teloptStage = 4;
                    }
                    else
                        teloptStage = 0;

                    break;

                case TELOPT_NEW_ENVIRON:
                    if(c == TELQUAL_IS)
                    {
                        log.write<Logging::DEBUG_LOG>("[IAC] TELQUAL_IS", (int)currentOption, (int)c);
                        teloptStage = 6;
                    }
                    else
                        teloptStage = 0;

                    break;

                default:

                    //printf("\r\n [Stage 3 - unregistered stuff it] - %i, %i \r\n",opt, c);
                    if(c == SE)
                    {
                        log.write<Logging::DEBUG_LOG>("[IAC] SB END", (int)currentOption, (int)c);
                        teloptStage = 0;
                    }
                    else
                    {
                        // reset
                        teloptStage = 0;
                    }

                    break;
            }

            break;

        // Only Gets here on TTYPE Sub-Negotiation.
        case 4:
            log.write<Logging::DEBUG_LOG>("--> STAGE 4 TTYPE", (int)c);

            if(c != IAC && c != SE)
            {
                if(c == '\x00')
                    dataSequence += '\0';
                else
                    dataSequence += static_cast<char>(c);

                if(dataSequence.size() >= SB_MAXLEN)
                {
                    // Invalid Sequences,, just clear.
                    dataSequence.clear();
                }
            }

            // check for End of Sequence IAC SE.
            if(c == IAC)
            {
                // IAC then 240 to close sequence for TTYPE
                teloptStage = 1;
            }

            /*
            else if (c == SE)
            {
                //data_sequence.clear();
                m_teloptStage = 0;
            }*/
            break;

        case 5:
            log.write<Logging::DEBUG_LOG>("--> STAGE 5 NAWS", (int)c);

            if(c != IAC && c != SE)
            {
                if(c == '\x00')
                    dataSequence += '\0';
                else
                    dataSequence += static_cast<char>(c);

                if(dataSequence.size() >= SB_MAXLEN)
                {
                    // Invalid Sequences,, just clear.
                    dataSequence.clear();
                }
            }

            // check for End of Sequence IAC SE.
            if(c == IAC)
            {
                // IAC then 240 to close sequence for NAWS
                teloptStage = 1;
            }

            /*
            else if (c == SE)
            {
                //data_sequence.clear();
                m_teloptStage = 0;
            }*/
            break;

        case 6:
            log.write<Logging::DEBUG_LOG>("--> STAGE 5 TELOPT_NEW_ENVIRON", (int)c);

            if(c != IAC && c != SE)
            {
                if(c == '\x00')
                    dataSequence += ' ';
                else
                    dataSequence += static_cast<char>(c);

                if(dataSequence.size() >= SB_MAXLEN)
                {
                    // Invalid Sequences,, just clear.
                    dataSequence.clear();
                    teloptStage = 0;
                }
            }

            // check for End of Sequence IAC SE.
            if(c == IAC)
            {
                // IAC then 240 to close sequence for NAWS
            }
            else if(c == SE)
            {
                //data_sequence.clear();
                teloptStage = 0;
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
            log.write<Logging::DEBUG_LOG>("--> STAGE 5 TELOPT_LINEMODE", (int)c);

            if(c != IAC && c != SE)
            {
                dataSequence += c;

                if(dataSequence.size() >= SB_MAXLEN)
                {
                    // Invalid Sequences,, just clear.
                    dataSequence.clear();
                }
            }

            // check for End of Sequence IAC SE.
            if(c == IAC)
            {
                // IAC then 240 to close sequence for NAWS
            }
            else if(c == SE)
            {
                teloptStage = 0;
                dataSequence.erase();
                // All Done, Now it's ok to turn off LINEMODE!
                sendIACSequences(telnetOptionDeny(WONT), TELOPT_LINEMODE);
            }

            break;
    }

    return '\0';
}

void TelnetDecoder::sendTTYPERequest() const {
    std::stringstream stm;
    stm << static_cast<char>(IAC) << static_cast<char>(SB)
            << static_cast<char>(TELOPT_TTYPE) << static_cast<char>(TELQUAL_SEND)
            << static_cast<char>(IAC) << static_cast<char>(SE);
    const std::string buf = stm.str();
    stm.clear();
    session.send(buf);
}

void TelnetDecoder::sendENVRequest() const {
    std::stringstream stm;
    std::vector<std::string> vars = {
        "USER", "TERM", "SHELL", "COLUMNS", "LINES",
        "C_CTYPE", "XTERM_LOCALE", "DISPLAY", "SSH_CLIENT",
        "SSH_CONNECTION", "SSH_TTY", "HOME", "HOSTNAME",
        "PWD", "MAIL", "LANG", "PWD", "UID", "USER_ID",
        "EDITOR", "LOGNAME", "SYSTEMTYPE"
    };

    stm << static_cast<char>(IAC) << static_cast<char>(SB)
            << static_cast<char>(TELOPT_NEW_ENVIRON) << static_cast<char>(TELQUAL_SEND);

    for (auto &v: vars) {
        stm << static_cast<char>(NEW_ENV_VAR) << v.data();
    }

    stm << static_cast<char>(IAC) << static_cast<char>(SE);
    const std::string buf = stm.str();
    stm.clear();
    vars.clear();
    session.send(buf);
}
