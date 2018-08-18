#ifndef PROTOCOL_DAO_HPP
#define PROTOCOL_DAO_HPP

#include <memory>
#include <string>
#include <mutex>

class Protocols;
typedef std::shared_ptr<Protocols> protocols_ptr;

/**
 * @class ProtocolDao
 * @author Michael Griffin
 * @date 05/05/2017
 * @file protocol_dao.hpp
 * @brief Handles Reading and Writing Protocols Class from XML
 */
class ProtocolDao
{
public:

    ProtocolDao(protocols_ptr prots, std::string path);
    ~ProtocolDao();

    /**
     * @brief Helper, appends forward/backward slash to path
     * @param value
     */
    void pathSeperator(std::string &value);

    /**
     * @brief Check if the file exists and we need to create a new one.
     * @return
     */
    bool fileExists();

    /**
     * @brief Creates and Saves a newly Generated Configuration File.
     * @param aco
     * @return
     */
    bool saveConfig(protocols_ptr prots);

    /**
     * @brief Loads a Configuration file into the m_config stub for access.
     * @return
     */
    bool loadConfig();
    
    /**
     * @brief Moves the Loaded config to the shared pointer.
     * @param rhs
     * @return
     */
    void encode(const Protocols &rhs);

    /**
     * @brief Grab a const handle to the loaded configuration.
     * @return
     */
    protocols_ptr getConfig() const
    {
        if(m_protocols)
        {
            return m_protocols;
        }
		
		return nullptr;
    }
   
    protocols_ptr     m_protocols;
    std::string       m_path;
    std::string       m_filename;
    std::mutex        m;

};

typedef std::shared_ptr<ProtocolDao> protocol_dao_ptr;

#endif // PROTOCOL_DAO_HPP
