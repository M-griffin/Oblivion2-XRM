#ifndef SESSION_DATA_HPP
#define SESSION_DATA_HPP

#ifdef _WIN32
#include "process_win.hpp"
#else
#include "process_posix.hpp"
#endif

#include "encoding.hpp"

#include "io_service.hpp"
#include "async_io.hpp"
#include "telnet_decoder.hpp"
#include "session_manager.hpp"
#include "common_io.hpp"
#include "deadline_timer.hpp"
#include "logging.hpp"

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
        , m_esc_input_timer(new DeadlineTimer())
        , m_state_manager(state_manager)
        , m_io_service(io_service)
        , m_common_io()
        , m_user_database(USERS_DATABASE, &m_database_log)
        , m_user_record(new Users())
        , m_session_stats(new SessionStats())
        , m_node_number(0)
        , m_is_use_ansi(true)
        , m_encoding_text(Encoding::ENCODING_TEXT_UTF8)
        , m_encoding(Encoding::ENCODE_UTF8)
        , m_is_session_authorized(false)
        , m_is_leaving(false)
        , m_is_esc_timer(false)
        , m_is_process_running(false)
        , m_parsed_data("")
    {
    }

    ~SessionData()
    {
        for(unsigned int i = 0; i < m_processes.size(); i++)
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
        if (m_is_leaving) 
        {
            return;
        }
        
        unsigned char ch = 0;
        std::string incoming_data = "";

        for(auto c : m_in_data_vector)
        {
            try
            {
                // Enter the Telnet_State and handle parsing options.
                ch = m_telnet_state->telnetOptionParse(c);
            }
            catch(std::exception& e)
            {
                Logging *log = Logging::instance();
                log->write<Logging::ERROR_LOG>("Exception telnet_process_char", e.what(), __LINE__, __FILE__);
            }

            // Skip any incoming nulls, nulls are also return on Telnet options received
            // So we know that there is no valid text data to send to the client.
            if(ch == '\0')
            {
                continue;
            }

            // Incoming Buffer is filled and Telnet options are parsed out.
            incoming_data += ch;
        }

        // Encode all incoming data as UTF8 unless we are not utf8
        if(m_encoding != Encoding::ENCODE_UTF8)
            m_parsed_data = Encoding::instance()->utf8Encode(incoming_data);
        else
            m_parsed_data = incoming_data;
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
        if(msg.size() == 0 || msg[0] == '\0' || m_is_leaving)
        {
            return;
        }

        // handle output encoding, if utf-8 translate data accordingly.
        std::string outputBuffer = "";

        // On Output, We have internal UTF8 now, translate to CP437
        if(m_encoding == Encoding::ENCODE_CP437)
        {
            outputBuffer = Encoding::instance()->utf8Decode(msg);
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
        Logging *log = Logging::instance();

        if(error)
        {
            log->write<Logging::ERROR_LOG>("async_write error - session closed()", error.message(), __LINE__, __FILE__);
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
                    // Sometime this doesn't close when it's already existed, just extra checking here.
                    log->write<Logging::DEBUG_LOG>("Exception connection shutdown()", e.what(), __LINE__, __FILE__);
                }
            }
        }
    }

    /*
     * @brief Start Security handshake.
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
        // Is no other input or part of ESC Sequences ie.. [A following the ESC
        // Then it's an ESC key, otherwise capture the rest of the sequence.
        m_esc_input_timer->setWaitInMilliseconds(400);
        m_esc_input_timer->asyncWait(
            std::bind(&SessionData::handleEscTimer, shared_from_this())
        );
    }

    /**
     * @brief User Logoff
     */
    void logoff()
    {
        m_is_leaving = true;
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
                // Sometime this doesn't close when it's already existed, just extra checking here.
                Logging *log = Logging::instance();
                log->write<Logging::DEBUG_LOG>("Exception connection shutdown()", e.what(), __LINE__, __FILE__);
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
        //std::string path = GLOBAL_SCRIPT_PATH + "\\" + cmdline;
        std::string path = cmdline;

        // Only (1) Process will run at a time per session.
#ifdef _WIN32
        process_ptr proc(new ProcessWin(shared_from_this(), path));

        if(proc)
        {
            m_is_process_running = true;
            m_processes.push_back(proc);
        }
        else
        {
            m_is_process_running = false;
        }

#else
        process_ptr proc(new ProcessPosix(shared_from_this(), path));

        if(proc)
        {
            m_is_process_running = true;
            m_processes.push_back(proc);
        }
        else
        {
            m_is_process_running = false;
        }

#endif
    }

    /**
     * @brief Clears Processes once it's shutdown and returned.
     */
    void clearProcess()
    {
        for(unsigned int i = 0; i < m_processes.size(); i++)
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
    void handleEscTimer();

public:

    connection_ptr        m_connection;
    session_manager_wptr  m_session_manager;
    telnet_ptr            m_telnet_state;
    deadline_timer_ptr    m_esc_input_timer;
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
    std::string           m_encoding_text;
    int                   m_encoding;
    bool                  m_is_session_authorized;
    bool                  m_is_leaving;
    bool                  m_is_esc_timer;
    bool                  m_is_process_running;

    enum { max_length = 16384 };
    //char m_raw_data[max_length];  // Raw Incoming
    std::vector<unsigned char> m_in_data_vector;
    std::string m_parsed_data;      // Telnet Opts parsed out

    // Handle to Processes.
    std::vector<process_ptr> m_processes;

};

#endif // SESSION_DATA_HPP
