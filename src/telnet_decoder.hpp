#ifndef __TELNET_DECODER_H_
#define __TELNET_DECODER_H_

#include "connection_base.hpp"
#include "telnet.hpp"

#include <boost/enable_shared_from_this.hpp>
#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/asio.hpp>

#include <iostream>
#include <string>
#include <vector>

/**
 * @class TelnetDecoder
 * @author Michael Griffin
 * @date 15/08/2015
 * @file telnet_decoder.hpp
 * @brief Handles Telnet Options and Feature Negotiation
 */
class TelnetDecoder
    : public boost::enable_shared_from_this<TelnetDecoder>
{
public:

    explicit TelnetDecoder(connection_ptr connection)
        : m_connection(connection)
        , m_naws_row(24)
        , m_naws_col(80)
        , m_term_type("undetected")
        , m_is_binary(false)
        , m_is_echo(false)
        , m_is_sga(false)
        , m_is_linemode(false)
        , m_teloptStage(0)
        , m_teloptCommand(0)
        , m_currentOption(0)
        , m_subnegoOption(0)
    {
        std::cout << "TelnetState Loaded!" << std::endl;
    }

    ~TelnetDecoder()
    {
        std::vector<unsigned char>().swap(reply_sequence);
        std::vector<unsigned char>().swap(active_sequence);
        std::cout << "Shutting Down ~TelnetState" << std::endl;
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
    }

    /**
     * @brief handles pending sequences waiting for reply responses
     * @param option
     * @return
     */
    bool checkReply(unsigned char option)
    {
        return checkSequence(reply_sequence, option);
    }
    void deleteReply(unsigned char option)
    {
        deleteSequence(reply_sequence, option);
    }
    void addReply(unsigned char option)
    {
        addSequence(reply_sequence, option);
    }

    /**
     * @brief handles active sequences negotiated.
     * @param option
     * @return
     */
    bool checkActive(unsigned char option)
    {
        return checkSequence(active_sequence, option);
    }
    void deleteActive(unsigned char option)
    {
        deleteSequence(active_sequence, option);
    }
    void addActive(unsigned char option)
    {
        addSequence(active_sequence, option);
    }

    void decodeBuffer();
    unsigned char telnetOptionParse(unsigned char c);

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

    connection_ptr		m_connection;

    int         m_naws_row;
    int         m_naws_col;
    std::string m_term_type;

    bool        m_is_binary;
    bool        m_is_echo;
    bool        m_is_sga;
    bool        m_is_linemode;

    // Global Option State for Telnet Options Parsing.
    int m_teloptStage;
    int m_teloptCommand;
    unsigned char m_currentOption;
    unsigned char m_subnegoOption;

    // Holds Sequences Waiting for Reply and Active.
    std::vector<unsigned char> reply_sequence;
    std::vector<unsigned char> active_sequence;

    // Holds Incoming SB Data Sequences to parse.
    std::string data_sequence;

    // Telnet Protocol Functions.
    unsigned char telnetOptionAcknowledge(unsigned char m_teloptCommand);
    unsigned char telnetOptionDeny(unsigned char m_teloptCommand);

    /**
     * @brief handles callback after write() for errors checking.
     * @param error
     */
    void handleWrite(const boost::system::error_code& error)
    {
        // Just log errors for now.
        if(error)
        {
            std::cout << "async_write error: " << error.message() << std::endl;
        }
    }

    /**
     * @brief delivers text data to client
     * @param msg
     */
    void deliver(const std::string &msg)
    {
        if(msg.size() == 0)
        {
            return;
        }

        if(m_connection->is_open())
        {
            if(m_connection->m_is_secure)
            {
                boost::asio::async_write(m_connection->m_secure_socket,
                                         boost::asio::buffer(msg, msg.size()),
                                         boost::bind(&TelnetDecoder::handleWrite, shared_from_this(),
                                                     boost::asio::placeholders::error));

            }
            else
            {
                boost::asio::async_write(m_connection->m_normal_socket,
                                         boost::asio::buffer(msg, msg.size()),
                                         boost::bind(&TelnetDecoder::handleWrite, shared_from_this(),
                                                     boost::asio::placeholders::error));
            }
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
                                          "PWD", "MAIL" ,"LANG", "PWD", "UID", "USER_ID",
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

    /**
     * @class FindFirst
     * @brief Comparatior for options in vectors.
     */
    struct FindFirst
    {
        FindFirst(unsigned char i) : to_find(i) { }
        unsigned char to_find;
        bool operator()
        (const unsigned char &p)
        {
            return p == to_find;
        }
    };


    /**
     * @brief Templates for Handling Sequence Vector Operations
     * @param t
     * @param option
     * @return
     */
    template <typename T>
    bool checkSequence(T &t, unsigned char option)
    {
        typename T::iterator it =
            find_if(t.begin(), t.end(), FindFirst(option));

        if(it == t.end())
        {
            //std::cout << option << " not found" << std::endl;
            return false;
        }
        else
        {
            //std::cout << option << " found" << std::endl;
        }
        return true;
    }

    template <typename T>
    void addSequence(T &t, unsigned char option)
    {
        typename T::iterator it =
            find_if(t.begin(), t.end(), FindFirst(option));

        // Sequence Not Found, add new sequence.
        //if(it == t.end())
        t.push_back(option);
    }

    template <typename T>
    void deleteSequence(T &t, unsigned char option)
    {
        typename T::iterator it =
            find_if(t.begin(), t.end(), FindFirst(option));

        typename T::iterator it_back = t.end();

        if(t.size() == 0)
        {
            return;
        }
        else if(t.size() > 1)
        {
            // If Sequence Found, Swap to Back and Remove Back.
            if(it != t.end())
            {
                iter_swap(it, it_back);
                t.pop_back();
            }
            else
            {
                // Clear all.
                std::vector<unsigned char>().swap(t);
            }
        }
    }
};

typedef boost::shared_ptr<TelnetDecoder> telnet_ptr;

#endif
