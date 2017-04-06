#ifndef ACCESS_LEVEL_DAO_HPP
#define ACCESS_LEVEL_DAO_HPP

#include <boost/smart_ptr/shared_ptr.hpp>

#include <string>
#include <mutex>


class AccessLevels;
typedef boost::shared_ptr<AccessLevels> access_level_ptr;

/**
 * @class ConfigDao
 * @author Michael Griffin
 * @date 2/21/2016
 * @file config_dao.hpp
 * @brief Handles Reading and Writting Config Class from XML
 */
class AccessLevelDao
{
public:

    AccessLevelDao(access_level_ptr acl, std::string path);
    ~AccessLevelDao();

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
    bool saveConfig(access_level_ptr acl);

    /**
     * @brief Loads a Configuation file into the m_config stub for access.
     * @return
     */
    bool loadConfig();
    
    /**
     * @brief Moves the Loaded config to the shared pointer.
     * @param rhs
     * @return
     */
    void encode(const AccessLevels &rhs);

    /**
     * @brief Grab a const handle to the loaded configuration.
     * @return
     */
    access_level_ptr getConfig() const
    {
        if(m_access_level)
        {
            return m_access_level;
        }
    }
   
    access_level_ptr  m_access_level;
    std::string       m_path;
    std::string       m_filename;
    std::mutex        m;

};

typedef boost::shared_ptr<AccessLevelDao> access_level_dao_ptr;

#endif // ACCESS_LEVEL_DAO_HPP
