#ifndef COMMUNICATOR_HPP
#define COMMUNICATOR_HPP

#include "data-sys/text_prompts_dao.hpp"
#include "model-sys/structures.hpp"
#include "model-sys/config.hpp"

#include "safe_queue.hpp"
#include "session_manager.hpp"
#include "common_io.hpp"

#include <algorithm>
#include <iostream>
#include <string>
#include <mutex>

/**
 * @class Communicator
 * @author Michael Griffin
 * @date 15/08/2015
 * @file communicator.hpp
 * @brief Singleton to share between Sessions & Global Config.
  */
class Communicator
{
public:
    /**
     * @brief Creates Singleton Instance of Class
     * @return
     */
    static Communicator* instance()
    {
        if(!m_global_instance)
        {
            m_global_instance = new Communicator();
            return m_global_instance;
        }

        return m_global_instance;
    }

    /**
     * @brief Releases the Instance from Memory
     */
    static void releaseInstance()
    {
        if(m_global_instance)
        {
            delete m_global_instance;
            m_global_instance = nullptr;
        }

        return;
    }

    /**
     * @brief Add Global Messages to Queue
     * @param line
     */
    void addMessageQueue(std::string line)
    {
        std::lock_guard<std::mutex> lock(m_data_mutex);
        m_queue.enqueue(line);
    }

    /**
     * @brief Send Global Queue to all Connected Users
     */
    void sendGlobalMessage()
    {
        std::lock_guard<std::mutex> lock(m_data_mutex);

        std::string message;

        while(!m_queue.isEmpty())
        {
            message.erase();
            message = m_queue.dequeue();
            m_session_manager->deliver(message);
        }
    }

    /**
     * @brief Links the Communicator with all active sessions
     * in the System so we can send notifications or chat
     * from anywhere in the system.
     * @param session_manager
     */
    void setupServer(session_manager_ptr &session_manager)
    {
        std::lock_guard<std::mutex> lock(m_data_mutex);
        m_session_manager = session_manager;
    }

    /**
     * @brief Searches and picks first free node number not in use.
     * @return
     */
    int getNodeNumber()
    {
        std::lock_guard<std::mutex> lock(m_node_mutex);
        int node = 1;

        while(1)
        {
            auto it = std::find(m_node_array.begin(), m_node_array.end(), node);

            if(it != m_node_array.end())
            {
                // Found, try next.
                ++node;
            }
            else
            {
                // Not Found, Use This
                m_node_array.push_back(node);
                return node;
            }
        }
    }

    /**
     * @brief Removes a Node number from array on disconnect.
     * @param int_to_remove
     * @return
     */
    void freeNodeNumber(int int_to_remove)
    {
        std::lock_guard<std::mutex> lock(m_node_mutex);
        auto it = std::find(m_node_array.begin(), m_node_array.end(), int_to_remove);

        if(it != m_node_array.end())
        {
            std::swap(*it, m_node_array.back());
            m_node_array.pop_back();
        }
    }

    /**
     * @brief Shutdown, Clears all nodes from Broadcast and shuts down sockets.
     * @param
     * @return
     */
    void shutdown()
    {
        std::lock_guard<std::mutex> lock(m_data_mutex);
        m_session_manager->shutdown();
        m_active = false;
    }

    /**
     * @brief Attach the system configuration.
     * @param config
     */
    void attachConfiguration(config_ptr config)
    {
        m_config = config;
    }

    /**
     * @brief Return a Read Only Instance of the Configuration.
     * @param config
     */
    config_ptr getConfiguration() const
    {
        return m_config;
    }

    /**
     * @brief Create Default Global Text Prompts
     */
    void createTextPrompts()
    {
        std::lock_guard<std::mutex> lock(m_prompt_mutex);

        // Create Mapping to pass for file creation (default values)
        M_TextPrompt value;
        value[GLOBAL_PROMPT_PAUSE] = std::make_pair("Displayed for Pause Prompts", "|03Hit any Key |08-- |03OBV/2 XRM");

        m_text_prompts_dao->writeValue(value);
    }

    /**
     * @brief Check if the System is Active
     * Used for io_service reloading
     * @return
     */
    bool isActive()
    {
        return m_active;
    }

    /**
     * @brief Pull Global Prompts with mutex for threads.
     * @param lookup
     * @return
     */
    M_StringPair getGlobalPrompt(const std::string &lookup)
    {
        std::lock_guard<std::mutex> lock(m_prompt_mutex);

        M_StringPair result = m_text_prompts_dao->getPrompt(lookup);
        return result;
    }

    // ThreadSafe Message Queue
    SafeQueue<std::string> m_queue;
    session_manager_ptr    m_session_manager;

private:

    std::string            m_filename;
    text_prompts_dao_ptr   m_text_prompts_dao;
    bool                   m_is_text_prompt_exist;
    bool                   m_active;
    config_ptr             m_config;

    mutable std::mutex     m_node_mutex;
    mutable std::mutex     m_data_mutex;
    mutable std::mutex     m_config_mutex;
    mutable std::mutex     m_prompt_mutex;

    std::vector<int>       m_node_array;
    static Communicator*   m_global_instance;
    CommonIO               m_common_io;


    explicit Communicator();
    ~Communicator();
    Communicator(const Communicator&);
    Communicator& operator=(const Communicator&);
};

/**
 * Singleton Class Accessor
 */
typedef Communicator TheCommunicator;

#endif // COMMUNICATOR_HPP
