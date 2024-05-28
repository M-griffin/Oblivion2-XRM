#ifndef SESSION_HPP
#define SESSION_HPP

#include <memory>
#include <string>
#include <vector>

#include "model-sys/users.hpp"
#include "telnet_decoder.hpp"

#include "logging.hpp"
#include "encoding.hpp"

#include "libSqliteWrapped.h"
#include "uuid.hpp"

#include <boost/asio.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/asio/deadline_timer.hpp>

class Session;
typedef std::shared_ptr<Session> session_ptr;
typedef std::weak_ptr<Session> session_wptr;

class SessionManager;
typedef std::weak_ptr<SessionManager> session_manager_wptr;
typedef std::shared_ptr<SessionManager> session_manager_ptr;

class StateManager;
typedef std::shared_ptr<StateManager> state_manager_ptr;

class ConnectionBase;
typedef std::shared_ptr<ConnectionBase> connection_ptr;

using boost::asio::deadline_timer;
using boost::asio::ip::tcp;

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
     * @param io_service
     * @param connection
     * @param my_session_manager
     * @return
     */
    Session(boost::asio::io_service& io_service, connection_ptr connection, session_manager_ptr my_session_manager);    
    ~Session();
    
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
    void deliver(const std::string &msg, bool is_disconnection=false);

    
    // Previous SessionData Methods
    
    /**
     * @brief Data Handler for incoming Data (From Client)
     */
    void waitingForData();
    
    /**
     * @brief Resolves the Hostname
     * @param ec
     * @param it
     */
    void resolveHandler(const boost::system::error_code &ec, tcp::resolver::iterator it);
    
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
    void handleRead(const boost::system::error_code &error, std::size_t length);
    
    /**
     * @brief Handle Telnet Options in incoming data
     * raw data is read in from socket
     * m_parsed_data is filled with parsed out options.
     */
    void handleTeloptCodes();
        
    void logoff();
        
    /**
     * @brief Shutdown Socket Connections
     */
    void disconnectUser();
    
    // Local Member Definitions Weak Pointers
    Logging                   &m_log;
    connection_ptr             m_connection;
    session_manager_wptr       m_session_manager;
    
    // Local Member Definitions Unique Pointers
    state_manager_ptr          m_state_manager;
 
    telnet_decoder_ptr         m_telnet_decoder;
    user_ptr                   m_user_record;
    
    // Local Member Variables
    int                        m_node_number;
    bool                       m_is_leaving;    
    
    // Raw Incoming Byte Data
    enum { max_length = 4096 };
    unsigned char m_raw_data[max_length];  
    std::string                m_parsed_data;    
    
    std::string                m_session_id;
    std::string                m_encoding_text;
    int                        m_encoding;    
    bool                       m_is_use_ansi;
    bool                       m_is_esc_timer;
    bool                       m_is_session_authorized;
    
    deadline_timer             m_detection_deadline;
            
    SQLW::Database             m_user_database;
    SQLW::StderrLog            m_database_log;    
};

#endif // SESSION_HPP
