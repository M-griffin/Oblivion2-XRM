#ifndef SESSION_HPP
#define SESSION_HPP

#include "chat_state.hpp"
#include "system_state.hpp"
#include "menu_manager.hpp"
#include "connection_tcp.hpp"
#include "broad_caster.hpp"
#include "telnet_decoder.hpp"
#include "communicator.hpp"
#include "session_data.hpp"

#include <boost/enable_shared_from_this.hpp>
#include <boost/smart_ptr/shared_ptr.hpp>

#include <boost/asio.hpp>
#include <boost/asio/io_service.hpp>

#include <list>
#include <string>

/*
 * NOTES: Sessions should also have incoming notification buffer
 * or OLM Buffer for message from other nodes and sysops
 * when not in chat.
 */

using boost::asio::deadline_timer;
using boost::asio::ip::tcp;

class Server;
class Session;

typedef boost::shared_ptr<Session> session_ptr;

/**
 * @class Session
 * @author Michael Griffin
 * @date 15/08/2015
 * @file chat_session.hpp
 * @brief handles TCP and SSL individual connection Sessions.
 */
class Session
    : public boost::enable_shared_from_this<Session>
{
public:

    ~Session()
    {
        // Free the Node Number from this session.
        TheCommunicator::Instance()->freeNodeNumber(m_session_data->m_node_number);
        std::cout << "~Session, Node: " << m_session_data->m_node_number << std::endl;
    }

    /**
     * @brief Handle the initial Session Creation, Also start the
     * Telnet Option Negotiation with the client.
     * @param tcp_connection
     * @param room
     * @return
     */
    static session_ptr create(boost::asio::io_service& io_service, connection_ptr connection, board_caster_ptr room)
    {
        session_ptr new_session(new Session(io_service, connection, room));

        if(connection->is_open())
        {
            if(connection->m_is_secure)
            {
                // Secure Sessions do handshake first!!
                new_session->m_session_data->handshake();
            }
            else
            {
                new_session->m_session_data->waitingForData();
            }
        }

        // Send out Telnet Negoiation Options
        if(connection->is_open() && !connection->m_is_secure)
        {
            // On initial Session Connection,  setup and send TELNET Options to
            // start the negotiation of client features.
            std::cout << "send initial IAC sequences started." << std::endl;

            // Need to negotiate this first, then turn off for Linux/osx to switch
            // Otherwise they both ignore the DONT and do not turn it off.
            new_session->m_session_data->m_telnet_state->sendIACSequences(DO, TELOPT_LINEMODE);
            new_session->m_session_data->m_telnet_state->addReply(TELOPT_LINEMODE);

            // Don't need to tell other DO when we say we WILL!
            //session->m_telnet_state->send_iac(DO, TELOPT_BINARY);
            //session->m_telnet_state->add_reply(TELOPT_BINARY);

            new_session->m_session_data->m_telnet_state->sendIACSequences(WILL, TELOPT_BINARY);
            new_session->m_session_data->m_telnet_state->addReply(TELOPT_BINARY);

            new_session->m_session_data->m_telnet_state->sendIACSequences(DO, TELOPT_SGA);
            new_session->m_session_data->m_telnet_state->addReply(TELOPT_SGA);

            // Don't need to tell other DON'T when we say we WILL!
            //session->m_telnet_state->send_iac(DONT, TELOPT_ECHO);
            //session->m_telnet_state->add_reply(TELOPT_ECHO);

            new_session->m_session_data->m_telnet_state->sendIACSequences(WILL, TELOPT_ECHO);
            new_session->m_session_data->m_telnet_state->addReply(TELOPT_ECHO);

            new_session->m_session_data->m_telnet_state->sendIACSequences(DO, TELOPT_TTYPE);
            new_session->m_session_data->m_telnet_state->addReply(TELOPT_TTYPE);

            new_session->m_session_data->m_telnet_state->sendIACSequences(DO, TELOPT_NAWS);
            new_session->m_session_data->m_telnet_state->addReply(TELOPT_NAWS);

            // No replies, this can really not be used, only informational.
            new_session->m_session_data->m_telnet_state->sendIACSequences(DO, TELOPT_NEW_ENVIRON);
            new_session->m_session_data->m_telnet_state->addReply(TELOPT_NEW_ENVIRON);

            std::cout << "send initial IAC sequences ended." << std::endl;
        }

        return new_session;
    }

    /**
     * @brief State Machine Switcher.  Select Active Modules per the Enum
     */
    void switchStates(int state)
    {
        switch(state)
        {
            case MACHINE_STATE::SYSTEM_STATE:
                {
                    state_ptr new_state(new SystemState(m_session_data));
                    m_menu_manager->changeState(new_state);
                    break;
                }

            /* Not working chat state at this time!
             * Probably Change to Services or something else!
            case MACHINE_STATE::CHAT_STATE:
                {
                    state_ptr new_state(new ChatState(m_session_data));
                    m_menu_manager->changeState(new_state);
                    break;
                }*/
            default:
                {
                    std::cout << "Error, Invalid state for switch_satates in Session." << std::endl;
                    break;
                }
        }
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

        if(m_connection->is_open())
        {
            if(m_connection->m_is_secure)
            {
                boost::asio::async_write(m_connection->m_secure_socket,
                                         boost::asio::buffer(msg, msg.size()),
                                         boost::bind(&Session::handleWrite, shared_from_this(),
                                                     boost::asio::placeholders::error));
            }
            else
            {
                boost::asio::async_write(m_connection->m_normal_socket,
                                         boost::asio::buffer(msg, msg.size()),
                                         boost::bind(&Session::handleWrite, shared_from_this(),
                                                     boost::asio::placeholders::error));
            }
        }
    }


    /**
     * @brief Callback after Writing Data, If error/hangup notifies
     * everything this person has left.
     * @param error
     */
    void handleWrite(const boost::system::error_code& error)
    {
        if(error)
        {
            std::cout << "async_write error: " << error.message() << std::endl;
            std::cout << "Session Closed()" << std::endl;
        }

        board_caster_ptr room = m_session_data->m_room.lock();
        if(room && error && (!m_session_data->m_is_leaving))
        {
            m_session_data->m_is_leaving = true;

            // Disconenct the session.
            room->leave(m_session_data->m_node_number);

            if(m_connection->is_open())
            {
                if(m_connection->m_is_secure)
                {
                    try
                    {
                        std::cout << "Leaving (SECURE SESSION) Client IP: "
                                  << m_connection->m_secure_socket.lowest_layer().remote_endpoint().address().to_string()
                                  << std::endl;

                        m_connection->m_secure_socket.lowest_layer().shutdown(tcp::socket::shutdown_both);
                        m_connection->m_secure_socket.lowest_layer().close();
                    }
                    catch(std::exception ex)
                    {
                        std::cout << "Exception closing socket(): " << ex.what() << std::endl;
                    }
                }
                else
                {
                    try
                    {
                        std::cout << "Leaving (NORMAL SESSION) Client IP: "
                                  << m_connection->m_normal_socket.remote_endpoint().address().to_string()
                                  << std::endl;

                        m_connection->m_normal_socket.shutdown(tcp::socket::shutdown_both);
                        m_connection->m_normal_socket.close();
                    }
                    catch(std::exception ex)
                    {
                        std::cout << "Exception closing socket(): " << ex.what() << std::endl;
                    }
                }
            }
        }
    }

    void resolve_handler(const boost::system::error_code &ec,
                         tcp::resolver::iterator it)
    {
        if(!ec)
        {
            tcp::resolver::iterator end;
            for(; it != end; ++it)
            {
                std::cout << "hostname: " << it->host_name() << std::endl;
            }
        }
        else
        {
            std::cout << "Error resolving hostname: " << ec.message() << std::endl;
        }
    }


public:

    /**
     * @brief Session Constructor Initiate the Connection, State, Room.
     * @param tcp_connection
     * @param room
     * @return
     */
    Session(boost::asio::io_service& io_service, connection_ptr connection, board_caster_ptr room)
        : m_session_state(SESSION_STATE::STATE_CMD)
        , m_connection(connection)
        , m_menu_manager(new MenuManager())
        , m_session_data(new SessionData(connection, room, io_service, m_menu_manager))
        , m_resolv(io_service)

    {

        /* TESTING
        std::string server =  m_tcp_connection->m_socket.remote_endpoint().address().to_string();
        std::string::size_type idx = server.rfind(":", server.size());
        if (idx != std::string::npos)
            server.erase(0, idx+1);

        std::cout << "ipv6: " << server << std::endl;
        tcp::resolver::query query(server, "");

        //resolv.async_resolve(endpoint, resolve_handler);
        m_resolv.async_resolve(query,
                               boost::bind(&Session::resolve_handler, this,
                                           boost::asio::placeholders::error,
                                           boost::asio::placeholders::iterator));
        */

        if(m_connection->is_open())
        {
            if(!m_connection->m_is_secure)
            {
                try
                {
                    std::cout << "New TCP Session ! " << std::endl;
                    std::cout << "Client IP Address: "
                              << m_connection->m_secure_socket.lowest_layer().remote_endpoint().address().to_string()
                              << std::endl;
                }
                catch(std::exception ex)
                {
                    std::cout << "Exception remote_endpoint(): " << ex.what() << std::endl;
                }
            }
            else
            {
                try
                {
                    std::cout << "New Secure Session ! " << std::endl;
                    std::cout << "Client IP Address: "
                              << m_connection->m_normal_socket.remote_endpoint().address().to_string()
                              << std::endl;
                }
                catch(std::exception ex)
                {
                    std::cout << "Exception remote_endpoint(): " << ex.what() << std::endl;
                }


            }
        }

        // Get The First available node number.
        m_session_data->m_node_number = TheCommunicator::Instance()->getNodeNumber();
        std::cout << "Node Number: " << m_session_data->m_node_number << std::endl;

        // Setup the Menu Manager for this session.
        state_ptr new_state(new MenuSystem(m_session_data));
        m_menu_manager->changeState(new_state);
    }

    int                 m_session_state;
    connection_ptr	    m_connection;
    menu_manager_ptr    m_menu_manager;
    session_data_ptr    m_session_data;
    tcp::resolver       m_resolv;

};


#endif // SESSION_HPP
