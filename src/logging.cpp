#include "logging.hpp"

#include <memory>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <mutex>
#include <thread>


thread_local unsigned int local_node_number = 0; 

Logging::Logging()
    : m_log_level(INFO_LOG)
    , m_mutex()
{
}

/**
 * @brief Helper, appends forward/backward slash to path
 * @param value
 */
void Logging::pathSeperator(std::string &value)
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
int Logging::getNumberOfLogEntries()
{
    return 0; //m_log_entries.size();
}

/**
* @brief Standard Time to Date/Time String
* @param std_time
* @return
*/
std::string Logging::standardDateTimeToString(std::time_t std_time)
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
std::string Logging::getCurrentDateTime()
{
    std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    return standardDateTimeToString(now);
}

/**
 * @brief Configuration String to Int Log Level
 * @param log_level
 * @return
 */
int Logging::getConfigurationLogState(const std::string &log_level)
{
    if(log_level == "INFO")
        return 0;
    else if(log_level == "DEBUG")
        return 1;
    else if(log_level == "WARN")
        return 2;
    else if(log_level == "ERROR")
        return 3;
    else if(log_level == "CONSOLE")
        return 4;
    else if(log_level == "ALL")
        return 5;
    else
        // Default is Info
        return 0;
}

/**
 * @brief Set Logging Level From Configurations by String and Convert to Int Level.
 * @param log_level
 */
void Logging::setLoggingLevel(std::string log_level) 
{
    m_log_level = getConfigurationLogState(log_level);
}

    
/**
 * @brief Set the Node Number for a Thread Local
 * @param node_number
 */
void Logging::setUserInfo(int node_number)
{
    local_node_number = node_number;
}


/**
 * @brief Write out Log to console in YAML formatted output.
 * @param date_time
 * @param details
 */
void Logging::writeOutYamlConsole(const std::string &date_time, std::vector<std::string> details)
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
    
    if (local_node_number > 0) 
    {
        std::cout << "Node " << local_node_number <<" | ";
    }
    else
    {
        std::cout << "System | ";
    }
    
    std::cout << date_time << " : ";
    for(std::string &d : details)
    {
        std::cout << d << " | ";
    }
    std::cout << std::endl;
    return;
}    