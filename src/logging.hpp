#ifndef LOGGING_H
#define LOGGING_H

#include "model-sys/config.hpp"

#include "communicator.hpp"

#include <memory>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <mutex>

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
    const std::string INFO_LEVEL = "Info";
    const std::string DEBUG_LEVEL = "Debug";
    const std::string ERROR_LEVEL = "Error";
    const std::string CONSOLE_LEVEL = "Console";
    const std::string ALL_LEVELS = "All";

    /**
     * @brief Creates Singleton Instance of Class
     * @return
     */
    static Logging &getInstance()
    {      
        static Logging instance;
        return instance;
    }        

    /**
     * @brief Helper, appends forward/backward slash to path
     * @param value
     */
    void pathSeperator(std::string &value)
    {
#ifdef _WIN32
        value.append("\\");
#else
        value.append("/");
#endif
    }

    /**
     * @brief Return number of Logs in Queue
     * @return
     */
    int getNumberOfLogEntries()
    {
        return 0; //m_log_entries.size();
    }

    /**
    * @brief Standard Time to Date/Time String
    * @param std_time
    * @return
    */
    std::string standardDateTimeToString(std::time_t std_time)
    {
        std::ostringstream oss;
        oss << std::put_time(std::localtime(&std_time), "%Y-%m-%d %H:%M:%S %z");
        std::string datetime_string = oss.str();
        return datetime_string;
    }


    /**
     * @brief Current Time Stamp (LOCAL TIME)
     * @return
     */
    std::string getCurrentDateTime()
    {
        std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        return standardDateTimeToString(now);
    }

    /**
     * @brief Configuration String to Int Log Level
     * @param log_level
     * @return
     */
    int getConfigurationLogState(const std::string &log_level)
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
    void write(Types ... rest)
    {        
        int config_level = INFO_LOG;
// For Conversion Utils, we share some common files, but don't need to allication Session Objcets
// And waste compile time, resources, and all of the required include files.
#ifndef CONVERSION        
        config_ptr config = Communicator::getInstance().getConfiguration();
        
        if (config)
        {
            config_level = getConfigurationLogState(config->logging_level);
        }
#endif        

        // Quick Case Statement, in Logging level, if were not logging anything
        // then return right away to save processing.
        switch(level)
        {
            // Incoming Logging Level Filtering by Configuration
            case INFO_LOG:
                if(config_level != INFO_LOG && config_level != ALL_LOGS)
                    return;
                break;

            case DEBUG_LOG:
                if(config_level != DEBUG_LOG && config_level != ALL_LOGS)
                    return;
                break;

            default:
                break;
        }

        std::vector<std::string> details;
        std::string date_time = getCurrentDateTime();
        std::string log_string = log<level>(rest...);

        switch(level)
        {
            // Incoming Logging Level
            case INFO_LOG:
                details.push_back(INFO_LEVEL);
                details.push_back(log_string);
                writeOutYamlConsole(date_time, details);
                break;

            case DEBUG_LOG:
                details.push_back(DEBUG_LEVEL);
                details.push_back(log_string);
                writeOutYamlConsole(date_time, details);
                break;

            case ERROR_LOG:

                details.push_back(ERROR_LEVEL);
                details.push_back(log_string);
                writeOutYamlConsole(date_time, details);             
                break;

            case CONSOLE_LOG:
                details.push_back(CONSOLE_LEVEL);
                details.push_back(log_string);
                writeOutYamlConsole(date_time, details);
                break;

            default:
                return;
        }

        if(details.size() > 0)
        {
            details.clear();
        }
    }

    /**
     * @brief Write out Log to console in YAML formatted output.
     * @param date_time
     * @param details
     */
    void writeOutYamlConsole(const std::string &date_time, std::vector<std::string> details)
    {        
        /*  Overkill on logging yaml formated logs.
        YAML::Emitter out;

        out << YAML::BeginMap;
        out << YAML::Flow;

        out << YAML::Key << "LogDateTime" << YAML::Value << date_time;
        out << YAML::Key << "Details";
        out << YAML::Value << YAML::BeginSeq;

        for(std::string &d : details)
        {
            out << d;
        }

        out << YAML::EndSeq;
        out << YAML::EndMap;
        */
        
        std::cout << date_time << " : ";
        for(std::string &d : details)
        {
            std::cout << d << " | ";
        }
        std::cout << std::endl;
        return;
    }    


private:

    mutable std::mutex     m_encoding_mutex;
    
    /**
     * @brief Constructor for the Singleton.
     * @return 
     */
    
    explicit Logging() 
    {
        std::cout << "Logging()" << std::endl;
    }
    
    Logging(const Logging&) = delete;             // Copy ctor
    Logging(Logging&&) = delete;                  // Move ctor
    Logging& operator=(const Logging&) = delete;  // Copy assignment
    Logging& operator=(Logging&&) = delete;       // Move assignment

};


#endif // LOGGING_H
