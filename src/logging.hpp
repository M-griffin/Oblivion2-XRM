#ifndef LOGGING_H
#define LOGGING_H

#include "communicator.hpp"
#include "model-sys/config.hpp"

#include <yaml-cpp/yaml.h>

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <chrono>
#include <ctime>
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

    // States
    static enum
    {
        INFO_LOG = 0,
        DEBUG_LOG = 1,
        ERROR_LOG = 2,
        CONSOLE_LOG = 3,
        ALL_LOGS = 4

    } LOGGING_LEVELS;

    // Descriptions
    const std::string INFO_LEVEL = "Information";
    const std::string DEBUG_LEVEL = "Debugging";
    const std::string ERROR_LEVEL = "Critial Error";
    const std::string CONSOLE_LEVEL = "Console Output";
    const std::string ALL_LEVELS = "All Level Information";

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

    std::string getCurrentDateTime()
    {
        std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        std::string s(20, '\0');
        std::strftime(&s[0], s.size(), "%Y-%m-%d %H:%M:%S", std::localtime(&now));
        return s.erase(s.size()-1, 1);
    }

    int getConfigurationLogState(std::string log_level)
    {
        if(log_level == "INFO")
            return 0;
        else if(log_level == "DEBUG")
            return 1;
        else if(log_level == "ERROR")
            return 2;
        else if(log_level == "CONSOLE")
            return 3;
        else if(log_level == "ALL")
            return 4;
        else
            // Default is Error
            return 2;
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
        std::vector<std::string> details;
        config_ptr config = Communicator::instance()->getConfiguration();
        std::string config_log_text = config->logging_level;

        std::string date_time = getCurrentDateTime();
        std::string log_string = log<level>(rest...);

        int config_level = getConfigurationLogState(config_log_text);

        switch(level)
        {
            // Incoming Logging Level
            case INFO_LOG:
                if(config_level == INFO_LOG || INFO_LOG == ALL_LOGS)
                {
                    details.push_back(INFO_LEVEL);
                }

                break;

            case DEBUG_LOG:
                if(config_level == DEBUG_LOG || INFO_LOG == ALL_LOGS)
                {
                    details.push_back(DEBUG_LEVEL);
                }

                break;

            case ERROR_LOG:
                // Always write out logs set to error
                details.push_back(ERROR_LEVEL);
                writeOutYamlConsole(date_time, details);
                break;

            case CONSOLE_LOG:
                // Always write out logs set to console
                details.push_back(CONSOLE_LEVEL);
                break;

            default:
                break;
        }
    }

    void writeOutYamlConsole(std::string date_time, std::vector<std::string> details)
    {
        YAML::Emitter out;

        out << YAML::BeginMap;
        out << YAML::Flow;

        out << YAML::Key << "Log DateTime" << YAML::Value << date_time;
        out << YAML::Key << "Log Details";
        out << YAML::Value << YAML::BeginSeq;

        for(std::string &d : details)
        {
            out << d;
        }

        out << YAML::EndSeq;
        out << YAML::EndMap;

        std::cout << std::endl << out.c_str() << std::endl;
        return;
    }

    /*
        void writeOutYamlFile(int level, std::vector<std::string> details)
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
         */

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
