#include "session_data.hpp"
#include "menu_manager.hpp"
#include "broad_caster.hpp"

#include <boost/asio.hpp>
#include <boost/asio/deadline_timer.hpp>
#include <boost/enable_shared_from_this.hpp>

using boost::asio::deadline_timer;
using boost::asio::ip::tcp;

/**
 * @brief Callback after data received. handles telnet options
 * Then parses out normal text data from client to server.
 * @param error
 * @param bytes_transferred
 */
void SessionData::handleRead(const boost::system::error_code& error, size_t bytes_transferred)
{
    if(!error)
    {
        // Part I: Parse Out Telnet Options and handle responses back to client.
        handleTeloptCodes();
    }

    board_caster_ptr room = m_room.lock();
    if(room)
    {
        if(!error)
        {
            if(m_parsed_data.size() > 0)
            {
                // Last Character Received is ESC, then Check for
                // ESC Sequence, or Lone ESC Key.
                if(m_parsed_data[m_parsed_data.size()-1] == '\x1b')
                {
                    startEscapeTimer();
                    m_is_esc_timer = true;
                }
                else if(!m_is_esc_timer)
                {
                    m_menu_manager->update();
                }

                /*
                // Telnet Options are parsed out.
                // Only send messages if there is parsed data!
                switch(m_session_state)
                {
                    case SESSION_STATE::STATE_CMD:

                        // Only on Single ESC Sequence,  Start Deadline timer
                        // To Catch remaining broken up ESC Sequences or
                        // Determine if this is a lone ESC secuence
                        if (m_parsed_data[m_parsed_data.size()-1] == '\x1b')
                        {
                            start_esc_timer();
                            m_is_esc_timer = true;
                        }
                        else if (!m_is_esc_timer)
                        {
                            m_state_machine->update(std::move(m_parsed_data));
                        }
                        break;

                    case SESSION_STATE::STATE_CHAT:
                        room->deliver(std::move(m_parsed_data));
                        break;

                    case SESSION_STATE::STATE_DONE:
                        break;

                    default:
                        break;
                }*/
            }

            // Restart Callback to wait for more data.
            // If this step is skipped, then the node will exit
            // since io_service will have no more work!
            if(m_tcp_connection->m_socket.is_open())
            {
                waitingForData();
            }
        }
        else
        {
            // First error, we mark leaving, if we loop again, ignore
            if(!m_is_leaving)
            {

                board_caster_ptr room = m_room.lock();
                if(room && error)
                {
                    m_is_leaving = true;

                    // Disconenct the session.
                    std::cout << "Leaving chat room! " << std::endl;
                    room->leave(m_node_number);
                    // m_session_state = SESSION_STATE::STATE_DONE;

                    if(m_tcp_connection->m_socket.is_open())
                    {
                        std::cout << "Leaving (SESSION_DATA) Peer IP: "
                                  << m_tcp_connection->m_socket.remote_endpoint().address().to_string()
                                  << std::endl << "Host-name: "
                                  //<< boost::asio::ip::host_name() // Local host_name
                                  << m_tcp_connection->m_socket.remote_endpoint()
                                  << std::endl
                                  << "Bytes: "
                                  << bytes_transferred
                                  << std::endl;

                        m_tcp_connection->m_socket.shutdown(tcp::socket::shutdown_both);
                        m_tcp_connection->m_socket.close();
                    }
                }
            }
        }
    }
    else
    {
        std::cout << "Error, Not connected to the board_caster!" << std::endl;
    }
}


/**
 * @brief Deadline Input Timer for ESC vs ESC Sequence.
 * @param timer
 */
void SessionData::handleEscTimer(boost::asio::deadline_timer* timer)
{
    if(timer->expires_at() <= deadline_timer::traits_type::now())
    {
        // The deadline has passed. Stop the session. The other actors will
        // terminate as soon as possible.
        //stop();
        std::cout << "Deadline Checking, EXPIRED!" << std::endl;
    }
    else
    {
        // Got more input while waiting, FOUND MORE DATA!
        std::cout << "Deadline Checking, CAUGHT REMAINING SEQUENCE!" << std::endl;
    }
    // Move text to State Machine, Timer has passed, or remainer of Sequence caught up!
    m_menu_manager->update();
    m_is_esc_timer = false;
}