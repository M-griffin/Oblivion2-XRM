#ifndef DAO_CONFIG_HPP
#define DAO_CONFIG_HPP

#include <string>
#include <mutex>

class Logging;
class Config;

/**
 * @class ConfigDao
 * @author Michael Griffin
 * @date 2/21/2016
 * @file config_dao.hpp
 * @brief Handles Reading and Writing Config Class from XML
 */
class ConfigDao {
public:
    ConfigDao(Config &config, const std::string &path);
    ~ConfigDao();

    bool fileExists();
    bool saveConfig(Config &cfg);
    bool loadConfig();
    bool validation();
    void encode(const Config &rhs);

    Config &getConfig() const {
        return m_config;
    }

    Logging &m_log;
    Config &m_config;
    std::string m_path;
    std::string m_filename;
    std::mutex m;
};

#endif
