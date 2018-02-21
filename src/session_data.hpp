#ifndef SESSION_DATA_HPP
#define SESSION_DATA_HPP

#ifdef _WIN32
#include "process_win.hpp"
#else
#include "process_posix.hpp"
#endif

#include "io_service.hpp"
#include "async_connection.hpp"
#include "telnet_decoder.hpp"
#include "session_manager.hpp"
#include "common_io.hpp"

#include "model-sys/structures.hpp"
#include "model-sys/struct_compat.hpp"
#include "model-sys/session_stats.hpp"
#include "model-sys/users.hpp"

#include "data-sys/session_stats_dao.hpp"
#include "data-sys/users_dao.hpp"

#include "libSqliteWrapped.h"

#include <memory>
#include <string>

class StateManager;
typedef std::shared_ptr<StateManager> state_manager_ptr;

class SessionData;
typedef std::shared_ptr<SessionData> session_data_ptr;
typedef std::weak_ptr<SessionData> session_data_wptr;

/**
 * @class SessionData
 * @author Michael Griffin
 * @date 9/19/2015
 * @file session_data.hpp
 * @brief Used for passing Session Data to and From States.
 */
class SessionData
    : public std::enable_shared_from_this<SessionData>
{
public:

    SessionData(connection_ptr           connection,
                session_manager_ptr      room,
                IOService&               io_service,
                state_manager_ptr        state_manager)
        : m_connection(connection)
        , m_session_manager(room)
        , m_telnet_state(new TelnetDecoder(connection))
//      , m_input_deadline(io_service)
        , m_state_manager(state_manager)
        , m_io_service(io_service)
        , m_common_io()
        , m_user_database(USERS_DATABASE, &m_database_log)
        , m_user_record(new Users())
        , m_session_stats(new SessionStats())
        , m_node_number(0)
        , m_is_use_ansi(true)
        , m_output_encoding("cp437")
        , m_is_session_authorized(false)
        , m_is_leaving(false)
        , m_is_esc_timer(false)
        , m_is_process_running(false)
//        , m_raw_data()
        , m_parsed_data("")
    {
        std::cout << "SessionData" << std::endl;
    }

    ~SessionData()
    {
        std::cout << "~SessionData" << std::endl;
        for (unsigned int i = 0; i < m_processes.size(); i++)
        {
            m_processes[i]->terminate();
        }
        std::vector<process_ptr>().swap(m_processes);
    }

    /**
     * @brief Data Handler for incoming Data (From Client)
     */
    void waitingForData()
    {
        // Important, clear out buffer before each read.
        //memset(&m_raw_data, 0, max_length);
        std::vector<unsigned char>().swap(m_in_data_vector);
        if(m_connection->isActive() && TheCommunicator::instance()->isActive())
        {
            m_connection->asyncRead(m_in_data_vector,
                                     std::bind(
                                         &SessionData::handleRead,
                                         shared_from_this(),
                                         std::placeholders::_1,
                                         std::placeholders::_2));
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
        for(auto c : m_in_data_vector)
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
        //memset(&m_raw_data, 0, max_length);
    }

    /**
     * @brief Passed data Though the State, and Checks ESC Timer
     */
    void updateState();

    /**
     * @brief Callback after data received. handles telnet options
     * Then parses out normal text data from client to server.
     * @param error
     * @param bytes_transferred
     */
    void handleRead(const std::error_code& error, socket_handler_ptr);

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

        // handle output encoding, if utf-8 translate data accordingly.
        std::string outputBuffer = "";
        if (m_output_encoding != "cp437")
        {
            outputBuffer = m_common_io.translateUnicode(msg);
        }
        else
        {
            outputBuffer = msg;
        }

        if(m_connection->isActive() && TheCommunicator::instance()->isActive())
        {
            m_connection->asyncWrite(outputBuffer,
                                      std::bind(
                                          &SessionData::handleWrite,
                                          shared_from_this(),
                                          std::placeholders::_1,
                                          std::placeholders::_2));
        }
    }

    /**
     * @brief Callback after Writing Data, If error/hangup notifies
     * everything this person has left.
     * @param error
     */
    void handleWrite(const std::error_code& error, socket_handler_ptr)
    {
        std::cout << "async_write " << error.message() << std::endl;
        if(error)
        {
            std::cout << "async_write error: " << error.message() << std::endl;
            std::cout << "Session Closed()" << std::endl;
        }

        session_manager_ptr session_manager = m_session_manager.lock();
        if(session_manager && error && (!m_is_leaving))
        {
            m_is_leaving = true;
            session_manager->leave(m_node_number);

            if(m_connection && m_connection->isActive())
            {
                try
                {
                    m_connection->shutdown();
                }
                catch(std::exception &e)
                {
                    std::cout << "Caught: " << e.what();
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
    void handleHandshake(const std::error_code& error);

    /**
     * @brief Start ESC Key input timer
     */
    void startEscapeTimer()
    {
        // Add Deadline Timer for .400 milliseconds for complete ESC Sequences.
        // Is no other input or part of ESC Sequecnes ie.. [A following the ESC
        // Then it's an ESC key, otherwise capture the rest of the sequence.

        /* TODO ADD ASYN Timer Job.
         m_input_deadline.expires_from_now(boost::posix_time::milliseconds(400));
        m_input_deadline.async_wait(
            boost::bind(&SessionData::handleEscTimer, shared_from_this(), &m_input_deadline));
        */
    }

    /**
     * @brief User Logoff
     */
    void logoff()
    {
        session_manager_ptr session_manager = m_session_manager.lock();
        if(session_manager)
        {
            // Room is the session.
            session_manager->leave(m_node_number);
        }

        if(m_connection && m_connection->isActive())
        {
            try
            {
                m_connection->shutdown();
            }
            catch(std::exception &e)
            {
                std::cout << "Caught: " << e.what();
            }
        }
    }

    /**
     * @brief Startup Session Stats
     * @param sessionType
     */
    void startUpSessionStats(std::string sessionType);

    /**
     * @brief Executes External Processes
     * @param cmdline
     */
    void startExternalProcess(const std::string &cmdline)
    {
        std::cout << "SessionData Starting Process" << std::endl;
        std::cout << GLOBAL_SCRIPT_PATH << std::endl;

        //std::string path = GLOBAL_SCRIPT_PATH + "\\" + cmdline;
        std::string path = cmdline;

        // Only (1) Process will run at a time per session.
#ifdef _WIN32
        process_ptr proc(new ProcessWin(shared_from_this(), path));
        if (proc)
        {
            std::cout << "SessionData Starting Process SUCCESS!" << std::endl;
            m_is_process_running = true;
            m_processes.push_back(proc);
        }
        else
        {
            std::cout << "SessionData Starting Process FAILED!" << std::endl;
            m_is_process_running = false;
        }
#else
        process_ptr proc(new ProcessPosix(shared_from_this(), path));
        if (proc)
        {
            std::cout << "SessionData Starting Process SUCCESS!" << std::endl;
            m_is_process_running = true;
            m_processes.push_back(proc);
        }
        else
        {
            std::cout << "SessionData Starting Process FAILED!" << std::endl;
            m_is_process_running = false;
        }
#endif
        std::cout << "SessionData Starting Done" << std::endl;
    }

    /**
     * @brief Clears Processes once it's shutdown and returned.
     */
    void clearProcess()
    {
        for (unsigned int i = 0; i < m_processes.size(); i++)
        {
            m_processes[i]->terminate();
        }

        std::vector<process_ptr>().swap(m_processes);
    }

private:

    /**
     * @brief Deadline Input Timer for ESC vs ESC Sequence.
     * @param timer
     */
    //  void handleEscTimer(boost::asio::deadline_timer* timer);

public:

    connection_ptr        m_connection;
    session_manager_wptr  m_session_manager;
    telnet_ptr            m_telnet_state;
    //deadline_timer        m_input_deadline;
    state_manager_ptr     m_state_manager;
    IOService            &m_io_service;

    CommonIO              m_common_io;

    // Temp while testing.
    SQLW::Database        m_user_database;
    SQLW::StderrLog       m_database_log;

    user_ptr              m_user_record;
    session_stats_ptr     m_session_stats;

    int                   m_node_number;
    bool                  m_is_use_ansi;
    std::string           m_output_encoding;
    bool                  m_is_session_authorized;
    bool                  m_is_leaving;
    bool                  m_is_esc_timer;
    bool                  m_is_process_running;

    enum { max_length = 8192 };
    //char m_raw_data[max_length];  // Raw Incoming
    std::vector<unsigned char> m_in_data_vector;
    std::string m_parsed_data;      // Telnet Opts parsed out

    // Handle to Processes.
    std::vector<process_ptr> m_processes;

};

#endif // SESSION_DATA_HPP
