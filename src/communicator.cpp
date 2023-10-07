#include "communicator.hpp"

#include "model-sys/structures.hpp"
#include "model-sys/config.hpp"

#include "data-sys/text_prompts_dao.hpp"

#include "safe_queue.hpp"
#include "session_manager.hpp"
#include "common_io.hpp"

#include <memory>
#include <algorithm>
#include <iostream>
#include <string>
#include <mutex>


/**
 * @brief Default Constructors required.
 * @return
 */
Communicator::Communicator()
    : m_filename("mod_global.yaml")
    , m_text_prompts_dao(new TextPromptsDao(GLOBAL_DATA_PATH, m_filename))
    , m_is_text_prompt_exist(false)
    , m_active(true)
    , m_config(nullptr)
{
    // Check of the Text Prompts exist.
    m_is_text_prompt_exist = m_text_prompts_dao->fileExists();

    if(!m_is_text_prompt_exist)
    {
        createTextPrompts();
    }

    // Loads all Text Prompts for current module
    m_text_prompts_dao->readPrompts();
}


/**
 * @brief Add Global Messages to Queue
 * @param line
 */
void Communicator::addMessageQueue(std::string line)
{
    std::lock_guard<std::mutex> lock(m_data_mutex);
    m_queue.enqueue(line);
}

/**
 * @brief Send Global Queue to all Connected Users
 */
void Communicator::sendGlobalMessage()
{
    std::lock_guard<std::mutex> lock(m_data_mutex);

    std::string message;

    while(!m_queue.isEmpty() && m_session_manager)
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
void Communicator::setupServer(const session_manager_ptr &session_manager)
{
    std::lock_guard<std::mutex> lock(m_data_mutex);
    m_session_manager = session_manager;
}

/**
 * @brief Searches and picks first free node number not in use.
 * @return
 */
int Communicator::getNodeNumber()
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
void Communicator::freeNodeNumber(int int_to_remove)
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
void Communicator::shutdown()
{
    std::lock_guard<std::mutex> lock(m_data_mutex);
    m_session_manager->shutdown();
    m_active = false;
}

/**
 * @brief Attach the system configuration.
 * @param config
 */
void Communicator::attachConfiguration(config_ptr config)
{
    m_config = config;
}

/**
 * @brief Release the system configuration.
 * @param config
 */
void Communicator::releaseConfiguration() {
    
    m_config.reset();
    m_active = false;
}

/**
 * @brief Return a Read Only Instance of the Configuration.
 * @param config
 */
config_ptr Communicator::getConfiguration() const
{
    return m_config;
}

/**
 * @brief Create Default Global Text Prompts
 */
void Communicator::createTextPrompts()
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
bool Communicator::isActive()
{
    return m_active;
}

/**
 * @brief Pull Global Prompts with mutex for threads.
 * @param lookup
 * @return
 */
M_StringPair Communicator::getGlobalPrompt(const std::string &lookup)
{
    std::lock_guard<std::mutex> lock(m_prompt_mutex);

    M_StringPair result = m_text_prompts_dao->getPrompt(lookup);
    return result;
}
