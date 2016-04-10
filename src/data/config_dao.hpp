#ifndef DAO_CONFIG_HPP
#define DAO_CONFIG_HPP

#include <boost/smart_ptr/shared_ptr.hpp>

#include <string>
#include <mutex>


struct Config;
typedef boost::shared_ptr<Config> config_ptr;

/**
 * @class ConfigDao
 * @author Michael Griffin
 * @date 2/21/2016
 * @file config_dao.hpp
 * @brief Handles Reading and Writting Config Class from XML
 */
class ConfigDao
{
public:

    ConfigDao(config_ptr config, std::string path);
    ~ConfigDao();

    /**
     * @brief Check if the file exists and we need to create a new one.
     * @return
     */
    bool fileExists();

    /**
     * @brief Creates and Saves a newly Generated Configuration File.
     * @param cfg
     * @return
     */
    bool saveConfig(config_ptr cfg);

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
    void encode(const Config &rhs);

    /**
     * @brief Grab a const handle to the loaded configuration.
     * @return
     */
    config_ptr getConfig() const
    {
        if(m_config)
        {
            return m_config;
        }
    }
   
    config_ptr  m_config;
    std::string m_path;
    std::string m_filename;
    std::mutex  m;

};

typedef boost::shared_ptr<ConfigDao> config_dao_ptr;

#endif // DAO_CONFIG_HPP
