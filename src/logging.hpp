#ifndef LOGGING_H
#define LOGGING_H

#include <memory>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
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
        WARN_LOG = 2,
        ERROR_LOG = 3,
        CONSOLE_LOG = 4,
        ALL_LOGS = 5

    } LOGGING_LEVELS;

    // Descriptions
    const std::string INFO_LEVEL = "Info";
    const std::string DEBUG_LEVEL = "Debug";
    const std::string WARN_LEVEL = "Warn";
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
    void pathSeperator(std::string &value);

    /**
     * @brief Return number of Logs in Queue
     * @return
     */
    int getNumberOfLogEntries();

    /**
    * @brief Standard Time to Date/Time String
    * @param std_time
    * @return
    */
    std::string standardDateTimeToString(std::time_t std_time);


    /**
     * @brief Current Time Stamp (LOCAL TIME)
     * @return
     */
    std::string getCurrentDateTime();
    
    /**
     * @brief Current Time Stamp (LOCAL TIME)
     * @return
     */
    std::string getCurrentDateTimeMillis();

    /**
     * @brief Configuration String to Int Log Level
     * @param log_level
     * @return
     */
    int getConfigurationLogState(const std::string &log_level);
    
    /**
     * @brief Set Logging Level From Configurations by String and Convert to Int Level.
     * @param log_level
     */
    void setLoggingLevel(std::string log_level);
    
    /**
     * @brief Set the Node Number for a Thread Local
     * @param node_number
     */
    void setUserInfo(int node_number);

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
        // Thread Safety, since we have main look and worker thread.
        std::unique_lock<std::mutex> lock(m_mutex);
        
        // Quick Case Statement, in Logging level, if were not logging anything
        // then return right away to save processing.
        switch(level)
        {
            // Incoming Logging Level Filtering by Configuration
            case INFO_LOG:
                if(m_log_level != INFO_LOG && m_log_level != ALL_LOGS)
                    return;
                break;

            case DEBUG_LOG:
                if(m_log_level != DEBUG_LOG && m_log_level != ALL_LOGS)
                    return;
                break;
                
            case WARN_LOG:
                if(m_log_level != WARN_LOG && m_log_level != ALL_LOGS)
                    return;
                break;
                
            default:
                break;
        }

        std::vector<std::string> details;
        std::string date_time = getCurrentDateTimeMillis();
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

            case WARN_LOG:
                details.push_back(WARN_LEVEL);
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
                details.clear();
                return;
        }

        details.clear();

    }


    /**
     * @brief Write out Log to console in YAML formatted output.
     * @param date_time
     * @param details
     */
    void writeOutYamlConsole(const std::string &date_time, std::vector<std::string> &details);


private:

    int                    m_log_level;
    mutable std::mutex     m_mutex;
    
    /**
     * @brief Constructor for the Singleton.
     * @return 
     */
    
    explicit Logging();

    Logging(const Logging&) = delete;             // Copy ctor
    Logging(Logging&&) = delete;                  // Move ctor
    Logging& operator=(const Logging&) = delete;  // Copy assignment
    Logging& operator=(Logging&&) = delete;       // Move assignment

};


#endif // LOGGING_H
