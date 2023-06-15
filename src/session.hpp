#ifndef SESSION_HPP
#define SESSION_HPP

#include "model-sys/users.hpp"
#include "telnet_decoder.hpp"

#include "logging.hpp"
#include "encoding.hpp"

#include <memory>
#include <string>
#include <vector>

#include "libSqliteWrapped.h"

class Session;
typedef std::shared_ptr<Session> session_ptr;
typedef std::weak_ptr<Session> session_wptr;

class AsyncIO;
typedef std::shared_ptr<AsyncIO> async_io_ptr;

class SessionManager;
typedef std::weak_ptr<SessionManager> session_manager_wptr;
typedef std::shared_ptr<SessionManager> session_manager_ptr;

class SocketHandler;
typedef std::shared_ptr<SocketHandler> socket_handler_ptr;

class StateManager;
typedef std::unique_ptr<StateManager> state_manager_uptr;
typedef std::shared_ptr<StateManager> state_manager_ptr;

class DeadlineTimer;
typedef std::unique_ptr<DeadlineTimer> deadline_timer_uptr;

/*
class TelnetDecoder;
typedef std::shared_ptr<TelnetDecoder> telnet_decoder_ptr;
*/

/**
 * @class Session
 * @author Michael Griffin
 * @date 15/08/2015
 * @file chat_session.hpp
 * @brief handles individual connection Sessions.
 */
class Session
    : public std::enable_shared_from_this<Session>
{
public:

    /**
     * @brief Session Constructor
     * @param my_async_io
     * @param my_session_manager
     * @return
     */
    Session(async_io_ptr const &my_async_io, session_manager_ptr const &my_session_manager);    
    ~Session();

    /**
     * @brief Startup Session (Connection Listener)
     *        Handle the initial Session Creation, Also start the
     *        Telnet Option Negotiation with the client.
     * @param my_async_io
     * @param my_session_manager
     * @return
     */
    static session_ptr create(async_io_ptr my_async_io, session_manager_ptr my_session_manager)
    {
        session_ptr new_session(new Session(my_async_io, my_session_manager));
        
        try        
        {
            // On initial Session Connection,  setup and send TELNET Options to
            // start the negotiation of client features.

            // On initial connection, clear and home cursor
            std::string clear_screen = "\x1b[1;1H\x1b[2J";
            new_session->deliver(clear_screen + " testing!"); /// Can be removed, testing output!

            std::cout << "Push Telnet Sequences to Client" << std::endl;
            new_session->m_telnet_decoder->sendIACSequences(DONT, TELOPT_OLD_ENVIRON);

            new_session->m_telnet_decoder->sendIACSequences(DO, TELOPT_SGA);
            new_session->m_telnet_decoder->addReply(TELOPT_SGA);

            new_session->m_telnet_decoder->sendIACSequences(WILL, TELOPT_ECHO);
            new_session->m_telnet_decoder->addReply(TELOPT_ECHO);

            new_session->m_telnet_decoder->sendIACSequences(WILL, TELOPT_SGA);
            new_session->m_telnet_decoder->addReply(TELOPT_SGA);

            new_session->m_telnet_decoder->sendIACSequences(WILL, TELOPT_BINARY);
            new_session->m_telnet_decoder->addReply(TELOPT_BINARY);

            new_session->m_telnet_decoder->sendIACSequences(DO, TELOPT_BINARY);
            new_session->m_telnet_decoder->addReply(TELOPT_BINARY);

            new_session->m_telnet_decoder->sendIACSequences(DO, TELOPT_TTYPE);
            new_session->m_telnet_decoder->addReply(TELOPT_TTYPE);

            new_session->m_telnet_decoder->sendIACSequences(DO, TELOPT_NAWS);
            new_session->m_telnet_decoder->addReply(TELOPT_NAWS);

            // Wait 1.5 Seconds for respones.
            std::cout << "startTelnetOptionNegoiation" << std::endl;
            new_session->startTelnetOptionNegoiation();

            // Setup Async Input on Session for User Input
            std::cout << "waitingForData" << std::endl;
            new_session->waitingForData();
            
        }
        catch(std::exception &ex)
        {
            Logging &log = Logging::getInstance();
            log.write<Logging::ERROR_LOG>("TelnetOptionNegoiation Exception=", ex.what(), __LINE__, __FILE__);
            new_session->logoff();
        }
        
        return new_session;
    }

    /**
     * @brief Telnet Option Negoiation Timer
     */
    void startTelnetOptionNegoiation();

    /**
     * @brief Timer Handler, When Called Starts up the Login State
     *        
     * @param timer
     */
    void handleTelnetOptionNegoiation();
    
    /**
     * @brief Deadline Detection Timer for Negotiation
     * @param timer
     */
    void handlePyBind11State();

    /**
     * @brief Callback from The Broadcaster to write data to the active sessions.
     * @param msg
     */
    void deliver(const std::string &msg);

    /**
     * @brief Callback after Writing Data, If error/hangup notifies
     *        Everyone this person has left.
     * @param error
     */
    void handleWrite(const std::error_code& error, socket_handler_ptr);
    
    
    // Previous SessionData Methods
    
    /**
     * @brief Data Handler for incoming Data (From Client)
     */
    void waitingForData();
    
    /**
     * @brief Passed data Though the State, and Checks ESC Timer
     */
    void updateState();

    /**
     * @brief ESC Sequence Timer for Determining Single Key vs. ESC Sequence
     */
    void startEscapeTimer();

    /**
     * @brief Deadline Input Timer for ESC vs ESC Sequence.
     * @param timer
     */
    void handleEscTimer();

    /**
     * @brief Callback after data received. handles telnet options
     * Then parses out normal text data from client to server.
     * @param error
     * @param bytes_transferred
     */
    void handleRead(const std::error_code& error, socket_handler_ptr);
    
    /**
     * @brief Handle Telnet Options in incoming data
     * raw data is read in from socket
     * m_parsed_data is filled with parsed out options.
     */
    void handleTeloptCodes();
    
    /**
     * @brief User Logoff
     */
    void logoff();
    
    
    // Local Member Definitions Weak Pointers
    Logging                   &m_log;
    async_io_ptr               m_async_io;
    session_manager_wptr       m_session_manager;
    
    // Local Member Definitions Unique Pointers
    state_manager_uptr         m_state_manager;
    deadline_timer_uptr        m_deadline_timer;
    deadline_timer_uptr        m_esc_input_timer;
 
    telnet_decoder_ptr         m_telnet_decoder;
    user_ptr                   m_user_record;
    
    // Local Member Variables
    int                        m_node_number;
    bool                       m_is_leaving;
    std::vector<unsigned char> m_in_data_vector;
    std::string                m_parsed_data;
    std::string                m_encoding_text;
    int                        m_encoding;
    bool                       m_is_use_ansi;
    bool                       m_is_esc_timer;
    bool                       m_is_session_authorized;
    
    SQLW::Database             m_user_database;
    SQLW::StderrLog            m_database_log;

};

#endif // SESSION_HPP
