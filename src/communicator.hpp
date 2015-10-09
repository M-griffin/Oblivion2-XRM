#ifndef COMMUNICATOR_HPP
#define COMMUNICATOR_HPP

#include "struct.hpp"
#include "safe_queue.hpp"
#include "broad_caster.hpp"
#include "common_io.hpp"

#include <boost/smart_ptr/shared_ptr.hpp>
#include <iostream>
#include <string>
#include <mutex>

/**
 * @class communicator
 * @author Michael Griffin
 * @date 15/08/2015
 * @file communicator.hpp
 * @brief Singleton to share between sessions & Global Config.
  */
class Communicator
{
public:
    /**
     * @brief Creates Singleton Instatce of Class
     * @return
     */
    static Communicator* Instance()
    {
        if(!m_globalInstance)
        {
            std::cout << "Communicator Created." << std::endl;
            m_globalInstance = new Communicator();
            return m_globalInstance;
        }
        return m_globalInstance;
    }

    /**
     * @brief Releases the Instance from Memory
     */
    static void ReleaseInstance()
    {
        if(m_globalInstance)
        {
            std::cout << "Communicator Released." << std::endl;
            delete m_globalInstance;
            m_globalInstance = nullptr;
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
            m_room->deliver(message);
        }
    }

    /**
     * @brief Links the Communicator with all active seesions
     * in the System so we can send notifications or chat
     * from anywhere in the system.
     * @param room
     */
    void setupServer(board_caster_ptr &room)
    {
        std::lock_guard<std::mutex> lock(m_data_mutex);
        m_room = room;
    }

    /**
     * @brief Searchs and picks first free node number not in use.
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
        if (it != m_node_array.end()) {
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
        m_room->shutdown();
    }

    /**
     * @brief Writing Binary Data Records to Disk
     * @param t
     * @param filename
     * @param idx
     * @return
     */
    template <typename T>
    int writeConfigurationRecord(T *t, std::string filename, int idx)
    {
        std::string path = BBS_PATH;
        #ifdef _WIN32
            path.append("\\");
        #else
            path.append("/");
        #endif
        path.append(filename);

        int x = 0;
        FILE *stream = fopen(path.c_str(), "rb+");
        if(stream == nullptr)
        {
            // Create File if it doesn't exist.
            stream = fopen(path.c_str(), "wb");
            if(stream == nullptr)
            {
                std::cout << "Error writting " << filename << std::endl;
                return x;
            }
        }
        if(fseek(stream, (int)idx * sizeof(T), SEEK_SET) == 0)
            x = fwrite(t, sizeof(T), 1, stream);
        fclose(stream);
        return x;
    }

    template <typename T>
    int readConfigurationRecord(T *t, std::string filename, int idx)
    {
        std::string path = BBS_PATH;
        #ifdef _WIN32
            path.append("\\");
        #else
            path.append("/");
        #endif
        path.append(filename);

        int x = 0;
        FILE *stream = fopen(path.c_str(), "rb+");
        if(stream == nullptr)
        {
            // Create File if it doesn't exist.
            std::cout << "Error Reading, Re-creating file. " << filename << std::endl;
            stream = fopen(path.c_str(), "wb");
            if(stream == nullptr)
            {
                std::cout << "Error Reading " << filename << std::endl;
                return x;
            }
        }
        fclose(stream);

        stream = fopen(path.c_str(), "rb");
        if(fseek(stream, (int)idx * sizeof(T), SEEK_SET) == 0)
            x = fread(t, sizeof(T), 1, stream);
        fclose(stream);
        return x;
    }

    /**
     * @brief Loads the System configuration
     */
    void loadSystemConfig();


    // ThreadSafe Message Queue
    SafeQueue<std::string> m_queue;
    board_caster_ptr       m_room;
    ConfigRec              m_config_record;

private:
    mutable std::mutex    m_node_mutex;
    mutable std::mutex    m_data_mutex;
    std::vector<int>      m_node_array;
    static Communicator*  m_globalInstance;
    CommonIO              m_common_io;

    Communicator();
    ~Communicator();
    Communicator(const Communicator&);
    Communicator& operator=(const Communicator&);
};

/**
 * Singleton Class Accessor
 */
typedef Communicator TheCommunicator;

#endif // COMMUNICATOR_HPP
