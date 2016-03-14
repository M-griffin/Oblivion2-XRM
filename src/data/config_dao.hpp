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

    bool saveConfig();
    bool loadConfig();

    void test();

    config_ptr  m_config;
    std::string m_path;
    std::mutex  m;

};

#endif // DAO_CONFIG_HPP
