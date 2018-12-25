#ifndef LOGGING_H
#define LOGGING_H

#include "communicator.hpp"
#include "model-sys/config.hpp"

#include <yaml-cpp/yaml.h>

#include <iostream>
#include <sstream>
#include <string>
#include <mutex>
#include <map>

/**
 * @class Logging
 * @author Michael Griffin
 * @date 24/12/2018
 * @file logging.hpp
 * @brief Global Logging class
 */
class Logging
{
public:

    static enum
    {
        INFO_LOG = 0,
        DEBUG_LOG = 1,
        ERROR_LOG = 2,
        CONSOLE_LOG = 3,
        ALL_LOGS = 4

    } LOGGING_LEVELS;

    const std::string INFO_LEVEL = "INFO";
    const std::string DEBUG_LEVEL = "DEBUG";
    const std::string ERROR_LEVEL = "ERROR";
    const std::string CONSOLE_LEVEL = "CONSOLE";
    const std::string ALL_LEVELS = "ALL";

    /**
     * @brief Creates Singleton Instatce of Class
     * @return
     */
    static Logging* instance()
    {
        if(!m_global_logging_instance)
        {
            std::cout << "Logging" << std::endl;
            m_global_logging_instance = new Logging();
            return m_global_logging_instance;
        }

        return m_global_logging_instance;
    }

    /**
     * @brief Releases the Instance from Memory
     */
    static void releaseInstance()
    {
        if(m_global_logging_instance)
        {
            std::cout << "~Logging" << std::endl;
            delete m_global_logging_instance;
            m_global_logging_instance = nullptr;
        }

        return;
    }

    template<int level>
    std::string log()
    {
        return "";
    }

    template<int level, typename T>
    std::string log(const T& t)
    {
        std::ostringstream oss;
        oss << t;
        return oss.str();
    }

    template<int level, typename T, typename ... Types>
    std::string log(const T& first, Types ... rest)
    {
        return log<level>(first) + " " + log<level>(rest...);
    }

    template<int level, typename ... Types>
    void xrmLog(Types ... rest)
    {
        config_ptr config = Communicator::instance()->getConfiguration();
        std::string config_log_level = config->logging_level;

        std::string log_string = log<level>(rest...);

        switch(level)
        {
            // Incoming Logging Level
            case INFO_LOG:
                break;

            case DEBUG_LOG:
                break;

            case ERROR_LOG:
                break;

            case CONSOLE_LOG:
                break;

            case ALL_LOGS:
                break;

            default:
                break;
        }

        // Testing Parameter Packs
        std::cout << log_string << std::endl;
    }

    void writeOutYamlLog(std::string path, std::string file_name)
    {
        std::string log_file;
        log_file.append(path);
        log_file.append(file_name);

        YAML::Emitter out;

        out << YAML::BeginMap;
        out << YAML::Flow;

        out << YAML::Key << "DateTime" << YAML::Value << "10pm etc.. ";
        out << YAML::Key << "Detail";
        out << YAML::Value << YAML::BeginSeq << "INFO" << "Testing Info Level" << YAML::EndSeq;

        out << YAML::EndMap;


        // Setup file to Write out File.
        std::ofstream ofs(log_file, std::ios_base::app);

        if(!ofs.is_open())
        {
            std::cout << "Error, unable to open file: " << log_file << std::endl;
            return;
        }

        ofs << out.c_str();
        ofs.close();

        return;
    }

private:

    mutable std::mutex     m_encoding_mutex;
    static Logging*        m_global_logging_instance;

    explicit Logging() { };
    ~Logging() { };
    Logging(const Logging&);
    Logging& operator=(const Logging&);

};

Logging* Logging::m_global_logging_instance = nullptr;

#endif // LOGGING_H
