#ifndef __TELNET_DECODER_H_
#define __TELNET_DECODER_H_

#include <memory>
#include <iostream>
#include <string>

#include "telnet.hpp"
#include "safe_vector.hpp"

class Logging;

class AsyncIO;
typedef std::shared_ptr<AsyncIO> async_io_ptr;
typedef std::weak_ptr<AsyncIO> async_io_wptr;

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

    explicit TelnetDecoder(async_io_ptr async_io);
    ~TelnetDecoder();

    /**
     * @brief Sends IAC Sequence back to Users Client for Terminal Negotiation.
     * @param command
     * @param option
     */
    void sendIACSequences(unsigned char command, int option);

    /**
     * @brief handles pending sequences waiting for reply responses
     * @param option
     * @return
     */
    bool checkReply(const unsigned char &option);
       
    void addReply(const unsigned char &option);
    

    void decodeBuffer();
    
    unsigned char telnetOptionParse(const unsigned char &c);

    int getTermRows() const;
    int getTermCols() const;
    
    void setTermRows(int value);
    void setTermCols(int value);

    std::string getTermType() const;

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
    void handleWrite(const std::error_code& error, socket_handler_ptr);

    /**
     * @brief delivers text data to client
     * @param msg
     */
    void deliver(const std::string &string_msg);

    /**
     * @brief Send Specific IAC TTYPE Message
     */
    void sendTTYPERequest();

    /**
     * @brief Handles TELOPT_NEW_ENVIRON Requests
     */
    void sendENVRequest();
};

typedef std::shared_ptr<TelnetDecoder> telnet_decoder_ptr;

#endif
