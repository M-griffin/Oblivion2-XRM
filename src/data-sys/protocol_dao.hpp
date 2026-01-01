#ifndef PROTOCOL_DAO_HPP
#define PROTOCOL_DAO_HPP

#include <string>
#include <mutex>

#include "../model-sys/protocol.hpp"


/**
 * @class ProtocolDao
 * @author Michael Griffin
 * @date 05/05/2017
 * @file protocol_dao.hpp
 * @brief Handles Reading and Writing Protocols Class from XML
 */
class ProtocolDao {
public:
    ProtocolDao(Protocols &prots, std::string path);
    ~ProtocolDao() = default;

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
     * @param prots
     * @return
     */
    bool saveConfig(const Protocols &prots);

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
    Protocols getConfig() const {
       return m_protocols;
    }

    Protocols m_protocols;
    std::string m_path;
    std::string m_filename;
    std::mutex m;
};

#endif
