#ifndef COMMUNICATOR_HPP
#define COMMUNICATOR_HPP

#include "safe_queue.hpp"

#include <memory>
#include <string>
#include <mutex>

class TextPromptsDao;
typedef std::shared_ptr<TextPromptsDao> text_prompts_dao_ptr;

class Config;
typedef std::shared_ptr<Config> config_ptr;

class SessionManager;
typedef std::shared_ptr<SessionManager> session_manager_ptr;

// Setup for TextPrompt Key/Value
typedef std::pair<std::string, std::string> M_StringPair;

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
    static Communicator& getInstance()
    {
        static Communicator instance;
        return instance;
    }    

    /**
     * @brief Add Global Messages to Queue
     * @param line
     */
    void addMessageQueue(std::string line);

    /**
     * @brief Send Global Queue to all Connected Users
     */
    void sendGlobalMessage();

    /**
     * @brief Links the Communicator with all active sessions
     * in the System so we can send notifications or chat
     * from anywhere in the system.
     * @param session_manager
     */
    void setupServer(const session_manager_ptr &session_manager);

    /**
     * @brief Searches and picks first free node number not in use.
     * @return
     */
    int getNodeNumber();

    /**
     * @brief Removes a Node number from array on disconnect.
     * @param int_to_remove
     * @return
     */
    void freeNodeNumber(int int_to_remove);

    /**
     * @brief Shutdown, Clears all nodes from Broadcast and shuts down sockets.
     * @param
     * @return
     */
    void shutdown();

    /**
     * @brief Attach the system configuration.
     * @param config
     */
    void attachConfiguration(config_ptr config);
    
    /**
     * @brief Release the system configuration.
     * @param config
     */
    void releaseConfiguration();

    /**
     * @brief Return a Read Only Instance of the Configuration.
     * @param config
     */
    config_ptr getConfiguration() const;

    /**
     * @brief Create Default Global Text Prompts
     */
    void createTextPrompts();

    /**
     * @brief Check if the System is Active
     * Used for io_service reloading
     * @return
     */
    bool isActive();

    /**
     * @brief Pull Global Prompts with mutex for threads.
     * @param lookup
     * @return
     */
    M_StringPair getGlobalPrompt(const std::string &lookup);

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
    
    explicit Communicator();
    
    Communicator(const Communicator&) = delete;             // Copy ctor
    Communicator(Communicator&&) = delete;                  // Move ctor
    Communicator& operator=(const Communicator&) = delete;  // Copy assignment
    Communicator& operator=(Communicator&&) = delete;       // Move assignment

};


#endif // COMMUNICATOR_HPP
