#ifndef SESSION_HPP
#define SESSION_HPP

#include "state_manager.hpp"
#include "async_connection.hpp"
#include "deadline_timer.hpp"
#include "socket_handler.hpp"
#include "session_manager.hpp"
#include "telnet_decoder.hpp"
#include "communicator.hpp"
#include "session_data.hpp"
#include "session_io.hpp"
#include "menu_system.hpp"

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
        std::cout << "~Session, Node: " << m_session_data->m_node_number << std::endl;

        // Free the menu system state and modules when session closes.
        m_state_manager->clean();
        std::cout << "m_menu_manager->clean() Completd!" << std::endl;
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

        // Send out Telnet Negoiation Options
        if(connection->isActive())
        {
            // On initial Session Connection,  setup and send TELNET Options to
            // start the negotiation of client features.
            std::cout << "send initial IAC sequences started." << std::endl;

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

            std::cout << "send initial IAC sequences ended." << std::endl;

            // Wait 1.5 Seconds for respones.
            new_session->startDetectionTimer();

            std::cout << "sleep ended." << std::endl;
        }

        return new_session;
    }

    /**
     * @brief Telopt Sequences timer
     */
    void startDetectionTimer()
    {       
        // Add Deadline Timer for 1.5 seconds for complete Telopt Sequences reponses
        m_deadline_timer->setWaitInMilliseconds(1500);
        m_deadline_timer->asyncWait(
            std::bind(&Session::handleDetectionTimer, shared_from_this())
        );
    }

    /**
     * @brief Deadline Detection Timer for Negoiation
     * @param timer
     */
    void handleDetectionTimer()
    {
        std::cout << "handleDetectionTimer Completed!" << std::endl;

        // Detection Completed, start ip the Pre-Logon Sequence State.
        state_ptr new_state(new MenuSystem(m_session_data));
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

        if(m_connection->isActive() && TheCommunicator::instance()->isActive())
        {
            m_connection->asyncWrite(msg,
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
        if(error)
        {
            std::cout << "async_write error: " << error.message() << std::endl;
            std::cout << "Session Closed()" << std::endl;
        }

        session_manager_ptr session_manager = m_session_data->m_session_manager.lock();
        if(session_manager && error && (!m_session_data->m_is_leaving))
        {
            m_session_data->m_is_leaving = true;

            // Disconenct the session.
            session_manager->leave(m_session_data->m_node_number);

            if(m_connection->isActive())
            {
                try
                {
                    std::cout << "Leaving (NORMAL SESSION) Client IP: "
                              //<< m_connection->m_normal_socket.remote_endpoint().address().to_string()
                              << std::endl;

                    m_connection->shutdown();
                }
                catch(std::exception &ex)
                {
                    std::cout << "Exception closing socket(): " << ex.what() << std::endl;
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
        if(m_connection->isActive())
        {
            try
            {
                std::cout << "New Connection Session ! " << std::endl;
                std::cout << "Client IP Address: "
                          //<< m_connection->m_normal_socket.remote_endpoint().address().to_string()
                          << std::endl;
            }
            catch(std::exception &ex)
            {
                std::cout << "Exception remote_endpoint(): " << ex.what() << std::endl;
            }
        }

        // Get The First available node number.
        m_session_data->m_node_number = TheCommunicator::instance()->getNodeNumber();
        std::cout << " **** Node Number: " << m_session_data->m_node_number << std::endl;
    }

    connection_ptr	    m_connection;
    state_manager_ptr   m_state_manager;
    session_data_ptr    m_session_data;
    socket_handler_ptr  m_socket_handler;
    deadline_timer_ptr  m_deadline_timer;

};


#endif // SESSION_HPP
