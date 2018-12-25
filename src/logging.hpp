#ifndef LOGGING_H
#define LOGGING_H

#include <iostream>
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
        INFO = 0,
        DEBUG = 1,
        ERROR = 2,
        CONSOLE = 3,
        ALL = 4

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

    void console_level();
    void info_level();
    void debug_level();
    void error_level();
    void all_levels();
    void writeOutYamlLog(std::string path, std::string file_name);

private:

    mutable std::mutex     m_encoding_mutex;
    static Logging*        m_global_logging_instance;

    explicit Logging();
    ~Logging();
    Logging(const Logging&);
    Logging& operator=(const Logging&);

};

#endif // LOGGING_H
