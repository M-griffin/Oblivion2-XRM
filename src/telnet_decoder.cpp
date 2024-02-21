#include "telnet_decoder.hpp"

#include <cstdio>
#include <string>
#include <fstream>

#include "telnet.hpp"
#include "logging.hpp"
#include "async_io.hpp"


TelnetDecoder::TelnetDecoder(async_io_ptr async_io)
    : m_log(Logging::getInstance())
    , m_async_io(async_io)
    , m_naws_row(24)
    , m_naws_col(80)
    , m_term_type("undetected")
    , m_is_binary(false)
    , m_is_echo(false)
    , m_is_sga(false)
    , m_is_linemode(false)
    , m_is_naws_detected(false)
    , m_teloptStage(0)
    , m_teloptCommand(0)
    , m_currentOption(0)
    , m_subnegoOption(0)
{
}

TelnetDecoder::~TelnetDecoder()
{
    m_log.write<Logging::DEBUG_LOG>("~TelnetDecoder()");
    m_async_io.reset();
    m_reply_sequence.clear();
}


/**
 * @brief Sends IAC Sequence back to Users Client for Terminal Negotiation.
 * @param command
 * @param option
 */
void TelnetDecoder::sendIACSequences(unsigned char command, int option)
{
    std::stringstream stm;
    std::string       buf;

    stm << static_cast<char>(IAC);
    stm << command;
    stm << static_cast<char>(option);
    buf = stm.str();
    stm.clear();
    deliver(buf);
    
    // Mark Sequence as Purshed then exclude senting it again.
    if(!checkReply(option)) {
        addReply(option);
    }
}

/**
 * @brief handles pending sequences waiting for reply responses
 * @param option
 * @return
 */
bool TelnetDecoder::checkReply(const unsigned char &option)
{
    return m_reply_sequence.exists(option);
}
   
void TelnetDecoder::addReply(const unsigned char &option)
{
    m_reply_sequence.push_back(option);
}

int TelnetDecoder::getTermRows() const
{
    return m_naws_row;
}

int TelnetDecoder::getTermCols() const
{
    return m_naws_col;
}

void TelnetDecoder::setTermRows(int value)
{
    m_naws_row = value;
}

void TelnetDecoder::setTermCols(int value)
{
    m_naws_col = value;
}

std::string TelnetDecoder::getTermType() const
{
    return m_term_type;
}

/**
 * @brief Telnet Option Acknowledgment Reply
 * @param cmd
 * @return
 */
unsigned char TelnetDecoder::telnetOptionAcknowledge(const unsigned char &cmd)
{
    switch(cmd)
    {
        case DO:
            return WILL;

        case WILL:
            return DO;

        case DONT:
            return WONT;

        case WONT:
            return DONT;
    }

    return 0;
}

/**
 * @brief Handle Telnet Option Negotiation
 * @param cmd
 * @return
 */
unsigned char TelnetDecoder::telnetOptionDeny(const unsigned char &cmd)
{
    switch(cmd)
    {
        case DO:
            return WONT;

        case WILL:
            return DONT;

        case DONT:
            return WONT;

        case WONT:
            return DONT;
    }

    return 0;
}

/**
 * @brief Decoded Sub-option Negotiation from Client.
 */
void TelnetDecoder::decodeBuffer()
{    
    m_log.write<Logging::DEBUG_LOG>("decodeBuffer 240 - SE received");

    // Now process the buffer per the options
    switch(m_subnegoOption)
    {
        case TELOPT_NAWS:
        
            for (std::string::size_type i = 0; i < data_sequence.size(); i++ )
            {
                // Validate First Set of Digits, then the Second Set so it's never cut off.
                if (i == 1) 
                {
                    m_naws_col = (256 * (unsigned char)data_sequence[0]) + (unsigned char)data_sequence[1];
                    
                }
                else if (i == 3)
                {
                    m_naws_row = (256 * (unsigned char)data_sequence[2]) + (unsigned char)data_sequence[3];                        
                }
            }

            m_log.write<Logging::DEBUG_LOG>("TELOPT_NAWS option", m_naws_col, "x", m_naws_row);
            m_is_naws_detected = true;
            break;

        case TELOPT_TTYPE:
            m_term_type = data_sequence;
            m_log.write<Logging::DEBUG_LOG>("TELOPT_TTYPE option", m_term_type);
            break;

        case TELOPT_NEW_ENVIRON:
            // Add Variable Or Tokenize here!!
            m_log.write<Logging::DEBUG_LOG>("TELOPT_NEW_ENVIRON data", data_sequence);
            break;

        // Not used at this time.
        case TELOPT_LINEMODE:
            // Add Variable Or Tokenize here!!
            m_log.write<Logging::DEBUG_LOG>("TELOPT_LINEMODE data", data_sequence);
            break;

        default:
            m_log.write<Logging::DEBUG_LOG>("Invalid option:", (int) m_subnegoOption, m_subnegoOption);
            break;
    }

    m_teloptStage = 0;
    m_subnegoOption = 0;
}

/**
 * @brief Parse Telnet Commands/Options from Incoming Data.
 *        Also handles responses to clients
 * @param c
 * @return
 */
unsigned char TelnetDecoder::telnetOptionParse(const unsigned char &c)
{
    // TEL-OPT Parser
    switch(m_teloptStage)
    {
        // Stage 0: Find IAC Sequence Initiator
        case 0:
            if(c != IAC)
            {
                // Pass-through normal text data.
                return c;
            }
            else
            {
                // Start of command sequence. Move to next stage.
                m_teloptStage++;
            }

            break;

        // Stage 1: Find Command
        case 1:

            // Check if Binary Mode is ON, if we get double double IAC, pass through
            if(c == IAC && m_is_binary)
            {
                // If were in Binary Mode, Than IAC IAC = IAC.
                m_log.write<Logging::DEBUG_LOG>("Got double IAC BINARY");
                m_teloptStage = 0;
                return IAC;
            }
            else if(c == IAC)
            {
                // If were NOT in binary mode, (WINDOWS TELNET CONSOLE)
                // Then IAC is sent as double IAC for single command starter!
                m_log.write<Logging::DEBUG_LOG>("Got double IAC");
                //m_teloptStage = 0;
                break;
            }

            // Parse 2 Bytes Commands, No Responses Given.
            switch(c)
            {
                // Most of these are pass through from LINE-MODE which is not used.
                case GA:    //     249        /* you may reverse the line */
                case EL:    //     248        /* erase the current line */
                case EC:    //     247        /* erase the current character */
                case AYT:   //     246        /* are you there */
                case AO:    //     245        /* abort output--but let prog finish */
                case IP:    //     244        /* interrupt process--permanently */
                case BREAK: //     243        /* break */
                case DM:    //     242        /* data mark--for connect. cleaning */
                case NOP:   //     241        /* nop */
                case EOR:   //     239        /* end of record (transparent mode) */
                case ABORT: //     238        /* Abort process */
                case SUSP:  //     237        /* Suspend process */
                case xEOF:  //     236        /* End of file: EOF is already used... */
                    // Pass Through commands that don't need Response.
                    m_log.write<Logging::DEBUG_LOG>("[IAC]", (int)c, "PASS-THROUGH");
                    m_teloptStage = 0;
                    break;

                // Handle Data Received from Client.
                case SE:    //     240        /* end sub negotiation */
                    m_log.write<Logging::DEBUG_LOG>("[IAC]", (int)c, "SE");
                    decodeBuffer();
                    data_sequence.clear();
                    m_teloptStage = 0;
                    break;

                default:
                    m_log.write<Logging::DEBUG_LOG>("[IAC]", (int)c, "MOVE to 3 BYTE");
                    // Move to 3 Byte Commands
                    m_teloptCommand = c;
                    m_teloptStage++;
                    break;
            }

            break;

        // Stage 2: Parse Commands that need a Reply (3 Byte Sequences)
        case 2:

            m_log.write<Logging::DEBUG_LOG>("[IAC]", (int)c, "STAGE 2");

            // Catch if were getting Invalid Option!.
            if(TELCMD_OK(m_teloptCommand))
            {
                m_log.write<Logging::DEBUG_LOG>("[IAC]", (int)m_teloptCommand, (int)c);
            }
            else
            {
                // Hopefully won't get here!
                m_log.write<Logging::DEBUG_LOG>("INVALID [IAC]", (int)m_teloptCommand, (int)c);
                m_teloptStage = 0;
                break;
            }

            switch(m_teloptCommand)
            {
                // DON'T, Only Valid Response is WONT
                case DONT:

                    // Handle Flags, if we received them shut it down.
                    switch(c)
                    {
                        case TELOPT_ECHO:
                            m_is_echo = false;
                            break;

                        case TELOPT_BINARY:
                            m_is_binary = false;
                            break;

                        case TELOPT_SGA:
                            m_is_sga = false;
                            break;

                        case TELOPT_LINEMODE:
                            m_is_linemode = false;
                            break;

                        default:
                            break;
                    }

                    m_log.write<Logging::DEBUG_LOG>("[IAC] RECEIVED DONT", (int)m_teloptCommand, (int)c);

                    if(!checkReply(c))
                    {
                        m_log.write<Logging::DEBUG_LOG>("[IAC] RECEIVED DONT -> WONT", (int)m_teloptCommand, (int)c);
                        sendIACSequences(telnetOptionAcknowledge(m_teloptCommand),c);
                    }
                    else
                    {
                        m_log.write<Logging::DEBUG_LOG>("[IAC] RECEIVED DONT -> REPLY", (int)m_teloptCommand, (int)c);
                    }

                    m_teloptStage = 0;
                    break;

                case DO: // Replies WILL / WON'T
                    m_log.write<Logging::DEBUG_LOG>("[IAC] RECEIVED DO", (int)m_teloptCommand, (int)c);

                    switch(c)
                    {
                        case TELOPT_ECHO:
                            if(!checkReply(c))
                            {
                                m_log.write<Logging::DEBUG_LOG>("[IAC] DO TELOPT_ECHO", (int)m_teloptCommand, (int)c);
                                sendIACSequences(telnetOptionDeny(m_teloptCommand),c);
                            }
                            else
                            {
                                m_log.write<Logging::DEBUG_LOG>("[IAC] DO TELOPT_ECHO REPLY", (int)m_teloptCommand, (int)c);
                                m_is_echo = true;
                            }

                            break;

                        case TELOPT_BINARY:
                            if(!checkReply(c))
                            {
                                m_log.write<Logging::DEBUG_LOG>("[IAC] DO TELOPT_BINARY", (int)m_teloptCommand, (int)c);
                                sendIACSequences(telnetOptionAcknowledge(m_teloptCommand),c);
                            }
                            else
                            {
                                m_log.write<Logging::DEBUG_LOG>("[IAC] DO TELOPT_BINARY REPLY", (int)m_teloptCommand, (int)c);
                                m_is_binary = true;
                            }

                            break;

                        case TELOPT_SGA:
                            if(!checkReply(c))
                            {
                                m_log.write<Logging::DEBUG_LOG>("[IAC] DO TELOPT_SGA", (int)m_teloptCommand, (int)c);
                                sendIACSequences(telnetOptionAcknowledge(m_teloptCommand),c);
                            }
                            else
                            {
                                m_log.write<Logging::DEBUG_LOG>("[IAC] DO TELOPT_SGA REPLY", (int)m_teloptCommand, (int)c);
                                m_is_sga = true;
                            }

                            break;

                        // Pass Through so we don't respond or keep looping.
                        case TELOPT_TTYPE:
                        case TELOPT_NAWS:
                            break;

                        case TELOPT_LINEMODE:
                            if(!checkReply(c))
                            {
                                m_log.write<Logging::DEBUG_LOG>("[IAC] DO LINEMODE", (int)m_teloptCommand, (int)c);
                                sendIACSequences(telnetOptionAcknowledge(m_teloptCommand),c);
                            }
                            else
                            {
                                m_log.write<Logging::DEBUG_LOG>("[IAC] DO LINEMODE REPLY", (int)m_teloptCommand, (int)c);
                                m_is_linemode = true;
                            }

                            break;

                        default:
                            m_log.write<Logging::DEBUG_LOG>("[IAC] DO -> WONT", (int)m_teloptCommand, (int)c);
                            sendIACSequences(telnetOptionDeny(m_teloptCommand),c);
                            break;
                    }

                    m_teloptStage = 0;
                    break;

                // WILL means the Server Will DO IT!
                // We reply Fine, do it!
                case WILL: // Replies DO And DONT
                    m_log.write<Logging::DEBUG_LOG>("[IAC] RECEIVED WILL", (int)m_teloptCommand, (int)c);

                    // Don't response to WILL Requests.
                    switch(c)
                    {
                        case TELOPT_ECHO:
                            if(!checkReply(c))
                            {
                                m_log.write<Logging::DEBUG_LOG>("[IAC] WILL TELOPT_ECHO", (int)m_teloptCommand, (int)c);
                                sendIACSequences(telnetOptionAcknowledge(m_teloptCommand),c);
                            }
                            else
                            {
                                m_log.write<Logging::DEBUG_LOG>("[IAC] WILL TELOPT_ECHO REPLY", (int)m_teloptCommand, (int)c);
                                m_is_echo = true;
                            }

                            break;

                        case TELOPT_BINARY:
                            if(!checkReply(c))
                            {
                                m_log.write<Logging::DEBUG_LOG>("[IAC] WILL TELOPT_BINARY", (int)m_teloptCommand, (int)c);
                                sendIACSequences(telnetOptionAcknowledge(m_teloptCommand),c);
                            }
                            else
                            {
                                m_log.write<Logging::DEBUG_LOG>("[IAC] WILL TELOPT_BINARY REPLY", (int)m_teloptCommand, (int)c);
                                m_is_binary = true;
                            }

                            break;

                        case TELOPT_SGA:
                            if(!checkReply(c))
                            {
                                m_log.write<Logging::DEBUG_LOG>("[IAC] WILL TELOPT_SGA", (int)m_teloptCommand, (int)c);
                                sendIACSequences(telnetOptionAcknowledge(m_teloptCommand),c);
                            }
                            else
                            {
                                m_log.write<Logging::DEBUG_LOG>("[IAC] WILL TELOPT_SGA REPLY", (int)m_teloptCommand, (int)c);
                                m_is_sga = true;
                            }

                            break;

                        case TELOPT_TTYPE:
                            // Ask for Terminal Type from Client!
                            m_log.write<Logging::DEBUG_LOG>("Ask for TTYPE", (int)m_teloptCommand, (int)c);
                            sendTTYPERequest();
                            break;

                        // Pass Through so we don't respond or keep looping.
                        case TELOPT_NAWS:
                            break;

                        case TELOPT_NEW_ENVIRON:
                            // Locks up Windows Telnet!!?!?!  Work this out later on.
                            //sendENVRequest();
                            //addReply(TELOPT_NEW_ENVIRON);
                            break;

                        case TELOPT_LINEMODE:
                            if(!checkReply(c))
                            {
                                m_log.write<Logging::DEBUG_LOG>("[IAC] WILL LINEMODE", (int)m_teloptCommand, (int)c);
                                sendIACSequences(telnetOptionAcknowledge(m_teloptCommand),c);
                            }
                            else
                            {
                                m_log.write<Logging::DEBUG_LOG>("[IAC] WILL LINEMODE REPLY", (int)m_teloptCommand, (int)c);
                                m_is_linemode = true;
                            }

                            break;

                        default :
                            m_log.write<Logging::DEBUG_LOG>("[IAC] WILL -> DONT", (int)m_teloptCommand, (int)c);
                            sendIACSequences(telnetOptionDeny(m_teloptCommand),c);
                            break;
                    }

                    m_teloptStage = 0;
                    break;

                // WON'T, Only Valid Response is DONT
                case WONT:

                    // Handle Flags, if we received them shit it down.
                    switch(c)
                    {
                        case TELOPT_ECHO:
                            m_is_echo = false;
                            break;

                        case TELOPT_BINARY:
                            m_is_binary = false;
                            break;

                        case TELOPT_SGA:
                            m_is_sga = false;
                            break;

                        case TELOPT_LINEMODE:
                            m_is_linemode = false;
                            break;

                        default:
                            break;
                    }

                    if(!checkReply(c))
                    {
                        m_log.write<Logging::DEBUG_LOG>("[IAC] RECEIVED WONT", (int)m_teloptCommand, (int)c);
                        sendIACSequences(telnetOptionAcknowledge(m_teloptCommand), c);
                    }
                    else
                    {
                        m_log.write<Logging::DEBUG_LOG>("[IAC] RECEIVED WONT REPLY", (int)m_teloptCommand, (int)c);
                    }

                    m_teloptStage = 0;
                    break;

                // Start of Sub Negotiations and Stages 3 - 4
                case SB: // 250
                    m_log.write<Logging::DEBUG_LOG>("[IAC] TELNET_STATE_SB", (int)m_teloptCommand, (int)c);

                    // Setup the Option when we get SE
                    // So we know how to process the buffer when it ends.
                    m_subnegoOption = c;

                    if(c == TELOPT_TTYPE)
                    {
                        m_currentOption = c;
                        m_teloptStage = 3;
                    }
                    else if(c == TELOPT_NEW_ENVIRON)
                    {
                        m_currentOption = c;
                        m_teloptStage = 3;
                    }
                    else if(c == TELOPT_NAWS)
                    {
                        m_currentOption = c;
                        m_teloptStage = 5;
                    }
                    else if(c == TELOPT_LINEMODE)
                    {
                        m_currentOption = c;
                        m_teloptStage = 7;
                    }
                    else
                    {
                        m_log.write<Logging::DEBUG_LOG>("[IAC] TELNET_STATE_SB UNSUPPORTED", (int)m_teloptCommand, (int)c);
                        // Invalid, reset back.
                        m_teloptStage = 0;
                    }

                    break;

                default:
                    // Options or Commands Not Parsed, RESET.
                    m_log.write<Logging::DEBUG_LOG>("[IAC] INVALID 3 Options", (int)m_teloptCommand, (int)c);
                    m_teloptStage = 0;
                    break;
            }

            break;

        case 3:
            m_log.write<Logging::DEBUG_LOG>("--> STAGE 3", (int)c);

            //Options will be 1 After SB
            switch(m_currentOption)
            {
                // IAC SB TTYPE TELQUAL_IS
                case TELOPT_TTYPE:
                    if(c == TELQUAL_IS)
                    {
                        m_log.write<Logging::DEBUG_LOG>("[IAC] TELQUAL_IS", (int)m_currentOption, (int)c);
                        m_teloptStage = 4;
                    }
                    else
                        m_teloptStage = 0;

                    break;

                case TELOPT_NEW_ENVIRON:
                    if(c == TELQUAL_IS)
                    {
                        m_log.write<Logging::DEBUG_LOG>("[IAC] TELQUAL_IS", (int)m_currentOption, (int)c);
                        m_teloptStage = 6;
                    }
                    else
                        m_teloptStage = 0;

                    break;

                default:

                    //printf("\r\n [Stage 3 - unregistered stuff it] - %i, %i \r\n",opt, c);
                    if(c == SE)
                    {
                        m_log.write<Logging::DEBUG_LOG>("[IAC] SB END", (int)m_currentOption, (int)c);
                        m_teloptStage = 0;
                    }
                    else
                    {
                        // reset
                        m_teloptStage = 0;
                    }

                    break;
            }

            break;

        // Only Gets here on TTYPE Sub-Negotiation.
        case 4:
            m_log.write<Logging::DEBUG_LOG>("--> STAGE 4 TTYPE", (int)c);

            if(c != IAC && c != SE)
            {
                if(c == '\x00')
                    data_sequence += '\0';
                else
                    data_sequence += static_cast<char>(c);

                if(data_sequence.size() >= SB_MAXLEN)
                {
                    // Invalid Sequences,, just clear.
                    data_sequence.clear();
                }
            }

            // check for End of Sequence IAC SE.
            if(c == IAC)
            {
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
            m_log.write<Logging::DEBUG_LOG>("--> STAGE 5 NAWS", (int)c);

            if(c != IAC && c != SE)
            {
                if(c == '\x00')
                    data_sequence += '\0';
                else
                    data_sequence += static_cast<char>(c);

                if(data_sequence.size() >= SB_MAXLEN)
                {
                    // Invalid Sequences,, just clear.
                    data_sequence.clear();
                }
            }

            // check for End of Sequence IAC SE.
            if(c == IAC)
            {
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
            m_log.write<Logging::DEBUG_LOG>("--> STAGE 5 TELOPT_NEW_ENVIRON", (int)c);

            if(c != IAC && c != SE)
            {
                if(c == '\x00')
                    data_sequence += ' ';
                else
                    data_sequence += static_cast<char>(c);

                if(data_sequence.size() >= SB_MAXLEN)
                {
                    // Invalid Sequences,, just clear.
                    data_sequence.clear();
                    m_teloptStage = 0;
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
            m_log.write<Logging::DEBUG_LOG>("--> STAGE 5 TELOPT_LINEMODE", (int)c);

            if(c != IAC && c != SE)
            {
                data_sequence += c;

                if(data_sequence.size() >= SB_MAXLEN)
                {
                    // Invalid Sequences,, just clear.
                    data_sequence.clear();
                }
            }

            // check for End of Sequence IAC SE.
            if(c == IAC)
            {
                // IAC then 240 to close sequence for NAWS
            }
            else if(c == SE)
            {
                m_teloptStage = 0;
                data_sequence.erase();
                // All Done, Now it's ok to turn off LINEMODE!
                sendIACSequences(telnetOptionDeny(WONT), TELOPT_LINEMODE);
            }

            break;
    }

    // No no valid Data, Return null character which are ignored.
    return '\0';
}


/**
 * @brief handles callback after write() for errors checking.
 * @param error
 */
void TelnetDecoder::handleWrite(const std::error_code& error, socket_handler_ptr)
{
    // Just log errors for now.
    if(error)
    {
        Logging &log = Logging::getInstance();
        log.write<Logging::ERROR_LOG>("telnet async_write error=", error.message(), __LINE__, __FILE__);
    }
    
    // Session Manager Should disconnect here or send error back to main session.
}

/**
 * @brief delivers text data to client
 * @param msg
 */
void TelnetDecoder::deliver(const std::string &string_msg)
{
    
    // TODO: NOTE change this to queue up respones, then the session will call to retieve
    // once it returns from parsing, then we don't have to pass sessions/async_io, less references.
    if(string_msg.size() == 0)
    {
        return;
    }
    
    try
    {
        async_io_ptr async_io = m_async_io.lock();
        if(async_io->getSocketHandle()->isActive())
        {            
            async_io->asyncWrite(string_msg,
                                     std::bind(
                                         &TelnetDecoder::handleWrite,
                                         shared_from_this(),
                                         std::placeholders::_1,
                                         std::placeholders::_2));
        }
    }
    catch (std::exception &ex) 
    {
        m_log.write<Logging::ERROR_LOG>("TelnetDecoder::deliver Exception=", ex.what(), __LINE__, __FILE__);
    }
}

/**
 * @brief Send Specific IAC TTYPE Message
 */
void TelnetDecoder::sendTTYPERequest()
{
    std::stringstream stm;
    std::string       buf;

    //IAC SB TTYPE SEND IAC SE
    stm << static_cast<char>(IAC);
    stm << static_cast<char>(SB);
    stm << static_cast<char>(TELOPT_TTYPE);
    stm << static_cast<char>(TELQUAL_SEND);
    stm << static_cast<char>(IAC);
    stm << static_cast<char>(SE);
    buf = stm.str();    
    stm.clear();
    deliver(buf);
}

/**
 * @brief Handles TELOPT_NEW_ENVIRON Requests
 */
void TelnetDecoder::sendENVRequest()
{
    std::stringstream stm;
    std::string       buf;

    // Loop through variables of possible responses.
    // Not getting any Responses, copied from x/84.
    std::vector<std::string> vars = { "USER", "TERM", "SHELL", "COLUMNS", "LINES",
                                      "C_CTYPE", "XTERM_LOCALE", "DISPLAY", "SSH_CLIENT",
                                      "SSH_CONNECTION", "SSH_TTY", "HOME", "HOSTNAME",
                                      "PWD", "MAIL","LANG", "PWD", "UID", "USER_ID",
                                      "EDITOR", "LOGNAME", "SYSTEMTYPE"
                                    };
    //IAC SB TTYPE SEND IAC SE
    stm << static_cast<char>(IAC);
    stm << static_cast<char>(SB);
    stm << static_cast<char>(TELOPT_NEW_ENVIRON);
    stm << static_cast<char>(TELQUAL_SEND);

    for(auto &v : vars)
    {
        stm << static_cast<char>(NEW_ENV_VAR);
        stm << v.data();
    }

    stm << static_cast<char>(IAC);
    stm << static_cast<char>(SE);
    buf = stm.str();
    stm.clear();
    vars.clear();
    deliver(buf);
} 