#ifndef DAO_CONFIG_HPP
#define DAO_CONFIG_HPP

#include <boost/smart_ptr/shared_ptr.hpp>
#include <string>

struct Config;
typedef boost::shared_ptr<Config> config_ptr;

/**
 * @class ConfigDAO
 * @author Michael Griffin
 * @date 2/21/2016
 * @file dao_config.hpp
 * @brief Handles Reading and Writting Config Class from XML
 */
class ConfigDAO
{
public:

    ConfigDAO(config_ptr config, std::string path);
    ~ConfigDAO();

    void saveConfig();
    void loadConfig();

    void test();

    config_ptr  m_config;
    std::string m_path;

};

#endif // DAO_CONFIG_HPP
