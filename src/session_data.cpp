#include "session_data.hpp"
#include "state_manager.hpp"
#include "session_manager.hpp"
#include "logging.hpp"


/**
 * @brief Passed data Though the State, and Checks ESC Timer
 */
void SessionData::updateState()
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
        m_state_manager->update();
    }
}

/**
 * @brief Callback after data received. handles telnet options
 * Then parses out normal text data from client to server.
 * @param error
 * @param bytes_transferred
 */
void SessionData::handleRead(const std::error_code& error, socket_handler_ptr)
{
    if(!error)
    {
        // Part I: Parse Out Telnet Options and handle responses back to client.
        handleTeloptCodes();
    }

    session_manager_ptr session_manager = m_session_manager.lock();

    if(!session_manager)
    {
        Logging *log = Logging::instance();
        log->xrmLog<Logging::ERROR_LOG>("Unable to load session_manager", __FILE__, __LINE__);
        return;
    }

    if(!error)
    {
        if(m_parsed_data.size() > 0)
        {
            // Windows Console Telnet sends [CR\LF] for ENTER!
            // search and replace input buffer we only need one!
            std::string::size_type id1 = 0;

            do
            {
                // Convert CR\LF to LF!
                id1 = m_parsed_data.find("\r\n", 0);

                if(id1 != std::string::npos)
                {
                    m_parsed_data.erase(id1, 1);
                    id1 = m_parsed_data.find("\r\n", 0);
                }
            }
            while(id1 != std::string::npos);


            // If were in a process, skip stat and ESC timer.
            if(m_is_process_running)
            {
                if(m_processes.size() > 0)
                {
                    m_processes.back()->update();
                }
                else
                {
                    // If no processes then reset.
                    m_is_process_running = false;
                    updateState();
                }
            }
            else
            {
                // Check for state cleanup
                if(m_processes.size() > 0)
                {
                    clearProcess();
                }

                updateState();
            }

            /*
            // Telnet Options are parsed out.
            // Only send messages if there is parsed data!
            switch(m_session_state)
            {
                case SESSION_STATE::STATE_CMD:

                    // Only on Single ESC Sequence,  Start Deadline timer
                    // To Catch remaining broken up ESC Sequences or
                    // Determine if this is a lone ESC sequence
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
        if(m_connection->isActive())
        {
            waitingForData();
        }
    }
    else
    {
        // First error, we mark leaving, if we loop again, ignore
        if(!m_is_leaving)
        {
            Logging *log = Logging::instance();
            log->xrmLog<Logging::ERROR_LOG>("Leaving SessionData Error=", error.message(), __FILE__, __LINE__);

            session_manager_ptr session_manager = m_session_manager.lock();

            if(session_manager && error)
            {
                m_is_leaving = true;

                // Disconnect the session.
                session_manager->leave(m_node_number);
                // m_session_state = SESSION_STATE::STATE_DONE;

                if(m_connection->isActive())
                {
                    m_connection->shutdown();
                }
            }
        }
    }
}

/**
 * @brief Deadline Input Timer for ESC vs ESC Sequence.
 * @param timer
 */
void SessionData::handleEscTimer()
{
    // Move text to State Machine, Timer has passed, or remainder of Sequence caught up!
    m_state_manager->update();
    m_is_esc_timer = false;
}

/**
 * @brief Startup Session Stats
 */
void SessionData::startUpSessionStats(std::string sessionType)
{
    // Check Table setup for Session Stats
    session_stats_dao_ptr session_stat_dao(new SessionStatsDao(m_user_database));

    // Verify if the user table exists.
    if(session_stat_dao)
    {
        std::time_t current_time = 0;
        std::time_t const dateTime = std::time(&current_time);

        m_session_stats->sSessionType = sessionType;
        m_session_stats->dtStartDate = dateTime;

        long id = session_stat_dao->insertRecord(m_session_stats);

        if(id < 0)
        {
            Logging *log = Logging::instance();
            log->xrmLog<Logging::ERROR_LOG>("SessionStats, error on insert=", m_session_stats->iId, __FILE__, __LINE__);
            return;
        }

        m_session_stats->iId = id;
    }
}
