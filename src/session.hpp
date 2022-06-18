#ifndef SESSION_HPP
#define SESSION_HPP

#include "state_manager.hpp"
#include "async_io.hpp"
#include "deadline_timer.hpp"
#include "socket_handler.hpp"
#include "session_manager.hpp"
#include "telnet_decoder.hpp"
#include "communicator.hpp"
#include "session_data.hpp"
#include "session_io.hpp"
#include "menu_system.hpp"
#include "python_system.hpp"
#include "logging.hpp"

#include <memory>
#include <list>
#include <string>
#include <cassert>

/*
 * NOTES TODO: Sessions should also have incoming notification buffer
 * or OLM Buffer for message from other nodes and sysops
 * when not in chat.
 */
class Session;
typedef std::shared_ptr<Session> session_ptr;

/**
 * @class Session
 * @author Michael Griffin
 * @date 15/08/2015
 * @file chat_session.hpp
 * @brief handles TCP and SSL individual connection Sessions.
 */
class Session
    : public std::enable_shared_from_this<Session>
{
public:

    ~Session()
    {
        // Free the Node Number from this session.
        TheCommunicator::instance()->freeNodeNumber(m_session_data->m_node_number);

        // Free the menu system state and modules when session closes.
        m_state_manager->clean();
    }

    /**
     * @brief Handle the initial Session Creation, Also start the
     *        Telnet Option Negotiation with the client.
     * @param tcp_connection
     * @param session_manager
     * @return
     */
    static session_ptr create(IOService& io_service, connection_ptr connection, deadline_timer_ptr deadline_timer,
                              session_manager_ptr session_manager)
    {
        session_ptr new_session(new Session(io_service, connection, deadline_timer, session_manager));

        if(connection->isActive())
        {
            new_session->m_session_data->startUpSessionStats("Telnet");
            new_session->m_session_data->waitingForData();
        }

        // Send out Telnet Negotiation Options
        if(connection->isActive())
        {
            // On initial Session Connection,  setup and send TELNET Options to
            // start the negotiation of client features.

            // On initial connection, clear and home cursor
            std::string clear_screen = "\x1b[1;1H\x1b[2J";
            new_session->deliver(clear_screen);

            // Need to negotiate this first, then turn off for Linux/osx to switch
            // Otherwise they both ignore the DONT and do not turn it off.
            //new_session->m_session_data->m_telnet_state->sendIACSequences(DO, TELOPT_LINEMODE);
            //new_session->m_session_data->m_telnet_state->addReply(TELOPT_LINEMODE);

            //new_session->m_session_data->m_telnet_state->sendIACSequences(DONT, TELOPT_LINEMODE);

            new_session->m_session_data->m_telnet_state->sendIACSequences(DONT, TELOPT_OLD_ENVIRON);

            new_session->m_session_data->m_telnet_state->sendIACSequences(DO, TELOPT_SGA);
            new_session->m_session_data->m_telnet_state->addReply(TELOPT_SGA);

            new_session->m_session_data->m_telnet_state->sendIACSequences(WILL, TELOPT_ECHO);
            new_session->m_session_data->m_telnet_state->addReply(TELOPT_ECHO);

            new_session->m_session_data->m_telnet_state->sendIACSequences(WILL, TELOPT_SGA);
            new_session->m_session_data->m_telnet_state->addReply(TELOPT_SGA);

            new_session->m_session_data->m_telnet_state->sendIACSequences(WILL, TELOPT_BINARY);
            new_session->m_session_data->m_telnet_state->addReply(TELOPT_BINARY);

            new_session->m_session_data->m_telnet_state->sendIACSequences(DO, TELOPT_BINARY);
            new_session->m_session_data->m_telnet_state->addReply(TELOPT_BINARY);

            new_session->m_session_data->m_telnet_state->sendIACSequences(DO, TELOPT_TTYPE);
            new_session->m_session_data->m_telnet_state->addReply(TELOPT_TTYPE);

            new_session->m_session_data->m_telnet_state->sendIACSequences(DO, TELOPT_NAWS);
            new_session->m_session_data->m_telnet_state->addReply(TELOPT_NAWS);

            // No replies, this can really not be used, only informational.
            //new_session->m_session_data->m_telnet_state->sendIACSequences(DO, TELOPT_NEW_ENVIRON);
            //new_session->m_session_data->m_telnet_state->addReply(TELOPT_NEW_ENVIRON);

            // Wait 1.5 Seconds for respones.
            new_session->startDetectionTimer();
        }

        return new_session;
    }

    /**
     * @brief Telnet option Sequence Timer
     */
    void startDetectionTimer()
    {
        // Add Deadline Timer for 1.5 seconds for complete Telopt Sequences responses
        m_deadline_timer->setWaitInMilliseconds(1500);
        m_deadline_timer->asyncWait(
            std::bind(&Session::handleDetectionTimer, shared_from_this())
        );
    }

    /**
     * @brief Deadline Detection Timer for Negotiation
     * @param timer
     */
    void handleDetectionTimer()
    {
        // Detection Completed, start ip the Pre-Logon Sequence State.
        state_ptr new_state(new MenuSystem(m_session_data));
        m_state_manager->changeState(new_state);
    }
    
    /**
     * @brief Deadline Detection Timer for Negotiation
     * @param timer
     */
    void handlePyBind11State()
    {
        // Detection Completed, start ip the Pre-Logon Sequence State.
        state_ptr new_state(new PythonSystem(m_session_data));
        m_state_manager->changeState(new_state);
    }

    /**
     * @brief Callback from The Broadcaster to write data to the active sessions.
     * @param msg
     */
    void deliver(const std::string &msg)
    {
        if(msg.size() == 0 || msg[0] == '\0')
        {
            return;
        }

        // handle output encoding, if utf-8 translate data accordingly.
        std::string outputBuffer = "";

        // On Output, We have internal UTF8 now, translate to CP437
        if(m_session_data->m_encoding == Encoding::ENCODE_CP437)
        {
            outputBuffer = msg; //Encoding::instance()->utf8Decode(msg);
        }
        else
        {
            outputBuffer = msg;
        }

        if(m_connection->isActive() && TheCommunicator::instance()->isActive())
        {
            m_connection->asyncWrite(outputBuffer,
                                     std::bind(
                                         &Session::handleWrite,
                                         shared_from_this(),
                                         std::placeholders::_1,
                                         std::placeholders::_2));
        }
    }

    /**
     * @brief Callback after Writing Data, If error/hangup notifies
     *        Everyone this person has left.
     * @param error
     */
    void handleWrite(const std::error_code& error, socket_handler_ptr)
    {
        Logging *log = Logging::instance();

        if(error)
        {
            log->write<Logging::ERROR_LOG>("Async_Write Session Closed() error=", error.message(), __LINE__, __FILE__);
        }

        session_manager_ptr session_manager = m_session_data->m_session_manager.lock();

        if(session_manager && error && (!m_session_data->m_is_leaving))
        {
            m_session_data->m_is_leaving = true;

            // Disconnect the session.
            session_manager->leave(m_session_data->m_node_number);

            if(m_connection->isActive())
            {
                try
                {
                    log->write<Logging::DEBUG_LOG>("Leaving (NORMAL SESSION)", __LINE__, __FILE__);
                    m_connection->shutdown();
                }
                catch(std::exception &ex)
                {
                    log->write<Logging::ERROR_LOG>("Exception closing socket()", ex.what(), __LINE__, __FILE__);
                }
            }
        }
    }

    /**
     * @brief Session Constructor Initiate the Connection, State, Room.
     * @param tcp_connection
     * @param session_manager
     * @return
     */
    Session(IOService& io_service, connection_ptr connection, deadline_timer_ptr deadline_timer,
            session_manager_ptr session_manager)
        : m_connection(connection)
        , m_state_manager(new StateManager())
        , m_session_data(new SessionData(connection, session_manager, io_service, m_state_manager))
        , m_deadline_timer(deadline_timer)
    {
        Logging *log = Logging::instance();

        if(m_connection->isActive())
        {
            try
            {
                log->write<Logging::DEBUG_LOG>("New Session Accepted", __LINE__, __FILE__);
            }
            catch(std::exception &ex)
            {
                log->write<Logging::ERROR_LOG>("Exception remote_endpoint()=", ex.what(), __LINE__, __FILE__);
            }
        }

        // Get The First available node number.
        m_session_data->m_node_number = TheCommunicator::instance()->getNodeNumber();
        log->write<Logging::CONSOLE_LOG>("New Session ConnectionNode Number=", m_session_data->m_node_number);
    }

    connection_ptr	    m_connection;
    state_manager_ptr   m_state_manager;
    session_data_ptr    m_session_data;
    socket_handler_ptr  m_socket_handler;
    deadline_timer_ptr  m_deadline_timer;

};


#endif // SESSION_HPP
