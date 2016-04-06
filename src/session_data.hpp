#ifndef SESSION_DATA_HPP
#define SESSION_DATA_HPP

#include "connection_base.hpp"
#include "telnet_decoder.hpp"
#include "session_manager.hpp"

#include "model/structures.hpp"
#include "model/struct_compat.hpp"

#include <boost/asio.hpp>
#include <boost/asio/deadline_timer.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/smart_ptr/weak_ptr.hpp>

#include <string>

using boost::asio::deadline_timer;
using boost::asio::ip::tcp;

class StateManager;
typedef boost::shared_ptr<StateManager>	state_manager_ptr;

/**
 * @class SessionData
 * @author Blue
 * @date 9/19/2015
 * @file session_data.hpp
 * @brief Used for passing Session Data to and From States.
 */
class SessionData
    : public boost::enable_shared_from_this<SessionData>
{
public:

    SessionData(connection_ptr           connection,
                session_manager_ptr      room,
                boost::asio::io_service& io_service,
                state_manager_ptr        state_manager)
        : m_connection(connection)
        , m_room(room)
        , m_telnet_state(new TelnetDecoder(connection))
        , m_input_deadline(io_service)
        , m_state_manager(state_manager)
        , m_io_service(io_service)
        , m_user_record()
        , m_node_number(0)
        , m_is_use_ansi(true)
        , m_output_encoding("cp437")
        , m_is_session_authorized(false)
        , m_is_leaving(false)
        , m_is_esc_timer(false)
        , m_raw_data()
        , m_parsed_data("")
    {
        std::cout << "SessionData" << std::endl;
    }

    ~SessionData()
    {
        std::cout << "~SessionData" << std::endl;
    }

    /**
     * @brief Data Handler for incoming Data (From Client)
     */
    void waitingForData()
    {
        // Important, clear out buffer before each read.
        memset(&m_raw_data, 0, max_length);
        if(m_connection->is_open())
        {
            if(m_connection->m_is_secure)
            {
                m_connection->m_secure_socket.async_read_some(boost::asio::buffer(m_raw_data, max_length),
                        boost::bind(&SessionData::handleRead, shared_from_this(),
                                    boost::asio::placeholders::error,
                                    boost::asio::placeholders::bytes_transferred));
            }
            else
            {
                m_connection->m_normal_socket.async_read_some(boost::asio::buffer(m_raw_data, max_length),
                        boost::bind(&SessionData::handleRead, shared_from_this(),
                                    boost::asio::placeholders::error,
                                    boost::asio::placeholders::bytes_transferred));
            }
        }
    }


    /**
     * @brief Handle Telnet Options in incoming data
     * raw data is read in from socket
     * m_parsed_data is filled with parsed out options.
     */
    void handleTeloptCodes()
    {
        unsigned char ch = 0;

        for(unsigned char c : m_raw_data)
        {
            try
            {
                // Enter the Telnet_State and handle parsing options.
                ch = m_telnet_state->telnetOptionParse(c);
            }
            catch(std::exception& e)
            {
                std::cout << "Exception telnet_process_char: " << e.what() << std::endl;
            }

            // Skip any incoming nulls, nulls are also return on Telnet options received
            // So we know that there is no valid text data to send to the client.
            if(ch == '\0')
            {
                continue;
            }

            // Incoming Buffer is filled and Telnet options are parsed out.
            m_parsed_data += ch;
        }
        // Clear the Session's Socket Buffer for next set of data.
        memset(&m_raw_data, 0, max_length);
    }

    /**
     * @brief Callback after data received. handles telnet options
     * Then parses out normal text data from client to server.
     * @param error
     * @param bytes_transferred
     */
    void handleRead(const boost::system::error_code& error, size_t bytes_transferred);


    /**
     * @brief delivers text data to client
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
                                         boost::bind(&SessionData::handleWrite, shared_from_this(),
                                                     boost::asio::placeholders::error));

            }
            else
            {
                boost::asio::async_write(m_connection->m_normal_socket,
                                         boost::asio::buffer(msg, msg.size()),
                                         boost::bind(&SessionData::handleWrite, shared_from_this(),
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

        session_manager_ptr room = m_room.lock();
        if(room && error && (!m_is_leaving))
        {
            m_is_leaving = true;

            // Disconenct the session.
            room->leave(m_node_number);

            if(m_connection->is_open())
            {
                if(m_connection->m_is_secure)
                {
                    std::cout << "Leaving 111 Peer IP: "
                              << m_connection->m_secure_socket.lowest_layer().remote_endpoint().address().to_string()
                              << std::endl;

                    m_connection->m_secure_socket.lowest_layer().shutdown(tcp::socket::shutdown_both);
                    m_connection->m_secure_socket.lowest_layer().close();
                }
                else
                {
                    std::cout << "Leaving 111 Peer IP: "
                              << m_connection->m_normal_socket.remote_endpoint().address().to_string()
                              << std::endl;

                    m_connection->m_normal_socket.shutdown(tcp::socket::shutdown_both);
                    m_connection->m_normal_socket.close();
                }
            }
        }
    }

    /*
     * @brief Start Secutiry handshake.
     */
    void handshake();

    /**
     * @brief Handles setting up the first read() after successful handshake.
     * @param error
     */
    void handleHandshake(const boost::system::error_code& error);

    /**
     * @brief Start ESC Key input timer
     */
    void startEscapeTimer()
    {
        // Add Deadline Timer for .400 milliseconds for complete ESC Sequences.
        // Is no other input or part of ESC Sequecnes ie.. [A following the ESC
        // Then it's an ESC key, otherwise capture the rest of the sequence.
        m_input_deadline.expires_from_now(boost::posix_time::milliseconds(400));
        m_input_deadline.async_wait(
            boost::bind(&SessionData::handleEscTimer, shared_from_this(), &m_input_deadline));
    }

private:

    /**
     * @brief Deadline Input Timer for ESC vs ESC Sequence.
     * @param timer
     */
    void handleEscTimer(boost::asio::deadline_timer* timer);

public:

    connection_ptr        m_connection;
    session_manager_wptr  m_room;
    telnet_ptr            m_telnet_state;
    deadline_timer        m_input_deadline;
    state_manager_ptr     m_state_manager;
    boost::asio::io_service& m_io_service;

    // Temp while testing.
    UserRec               m_user_record;

    int                   m_node_number;
    bool                  m_is_use_ansi;
    std::string           m_output_encoding;
    bool                  m_is_session_authorized;
    bool                  m_is_leaving;
    bool                  m_is_esc_timer;

    enum { max_length = 4096 };
    char m_raw_data[max_length];  // Raw Incoming
    std::string m_parsed_data;    // Telnet Opts parsed out

};

typedef boost::shared_ptr<SessionData> session_data_ptr;
typedef boost::weak_ptr<SessionData> session_data_wptr;

#endif // SESSION_DATA_HPP
