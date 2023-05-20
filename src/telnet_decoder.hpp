#ifndef __TELNET_DECODER_H_
#define __TELNET_DECODER_H_

#include "async_io.hpp"
#include "telnet.hpp"
#include "logging.hpp"
#include "safe_vector.hpp"

#include <memory>
#include <iostream>
#include <string>

class SocketHandler;
typedef std::shared_ptr<SocketHandler> socket_handler_ptr;

/**
 * @class TelnetDecoder
 * @author Michael Griffin
 * @date 15/08/2015
 * @file telnet_decoder.hpp
 * @brief Handles Telnet Options and Feature Negotiation
 */
class TelnetDecoder
    : public std::enable_shared_from_this<TelnetDecoder>
{
public:

    explicit TelnetDecoder(async_io_ptr async_io)
        : m_log(Logging::getInstance())
        , m_async_io(async_io)
        , m_naws_row(0)
        , m_naws_col(0)
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

    ~TelnetDecoder()
    {
        std::cout << "~TelnetDecoder()" << std::endl;
        m_async_io.reset();
    }

    /**
     * @brief Sends IAC Sequence back to Users Client for Terminal Negotiation.
     * @param command
     * @param option
     */
    void sendIACSequences(unsigned char command, int option)
    {
        std::stringstream stm;
        std::string       buf;

        stm << static_cast<char>(IAC);
        stm << command;
        stm << static_cast<char>(option);
        buf = stm.str();
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
    bool checkReply(const unsigned char &option)
    {
        return m_reply_sequence.exists(option);
    }
       
    void addReply(const unsigned char &option)
    {
        m_reply_sequence.push_back(option);
    }
    

    void decodeBuffer();
    unsigned char telnetOptionParse(const unsigned char &c);

    int getTermRows() const
    {
        return (m_naws_row > 0 ? m_naws_row : 24);
    }
    int getTermCols() const
    {
        return (m_naws_col > 0 ? m_naws_col : 80);
    }

    std::string getTermType() const
    {
        return m_term_type;
    }

private:

    Logging      &m_log;
    async_io_wptr m_async_io;

    int           m_naws_row;
    int           m_naws_col;
    std::string   m_term_type;

    bool          m_is_binary;
    bool          m_is_echo;
    bool          m_is_sga;
    bool          m_is_linemode;
    bool          m_is_naws_detected;

    // Global Option State for Telnet Options Parsing.
    int           m_teloptStage;
    int           m_teloptCommand;
    unsigned char m_currentOption;
    unsigned char m_subnegoOption;

    // Holds Sequences Waiting for Reply and Active.
    SafeVector<int> m_reply_sequence;

    // Holds Incoming SB Data Sequences to parse.
    std::string data_sequence;

    // Telnet Protocol Functions.
    unsigned char telnetOptionAcknowledge(const unsigned char &m_teloptCommand);
    unsigned char telnetOptionDeny(const unsigned char &m_teloptCommand);

    /**
     * @brief handles callback after write() for errors checking.
     * @param error
     */
    void handleWrite(const std::error_code& error, socket_handler_ptr)
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
    void deliver(const std::string &string_msg)
    {
        
        // TODO: NOTE change this to queue up respones, then the session will call to retieve
        // once it returns from parsing, then we don't have to pass sessions/async_io, less references.
        if(string_msg.size() == 0)
        {
            return;
        }

        std::cout << "telnetdecoder - prior to asyncio deliver telnet: " << string_msg << std::endl;
        async_io_ptr async_io = m_async_io.lock();
        if(async_io->getSocketHandle()->isActive())
        {
            std::cout << "telnetdecoder - after deliver telnet: " << string_msg << std::endl;
            
            async_io->asyncWrite(string_msg,
                                     std::bind(
                                         &TelnetDecoder::handleWrite,
                                         shared_from_this(),
                                         std::placeholders::_1,
                                         std::placeholders::_2));
                                         
           std::cout << "telnetdecoder - after write deliver telnet: " << string_msg << std::endl;
        }

    }

    /**
     * @brief Send Specific IAC TTYPE Message
     */
    void sendTTYPERequest()
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
        deliver(buf);
    }

    /**
     * @brief Handles TELOPT_NEW_ENVIRON Requests
     */
    void sendENVRequest()
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
        deliver(buf);
    }    
};

typedef std::shared_ptr<TelnetDecoder> telnet_decoder_ptr;

#endif
