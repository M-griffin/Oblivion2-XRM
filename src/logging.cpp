#include "logging.hpp"

#include <iostream>
#include <chrono>
#include <ctime>
#include <iomanip>


// Example Usage:
/*
Logging& log = Logging::getInstance();

log.setLogLevelFromString("DEBUG");
log.setNode(42);

log.log(Logging::LogLevel::Info, "Server started on port", 8080);
log.log(Logging::LogLevel::Debug, "Connection id:", 1234);
log.log(Logging::LogLevel::Error, "Failed to open file");
*/

thread_local uint32_t local_node_number = 0;

Logging& Logging::getInstance() {
    static Logging instance;
    return instance;
}

Logging::Logging()
    : m_logLevel(LogLevel::Info) {}

void Logging::setLogLevel(LogLevel level) {
    m_logLevel = level;
}

void Logging::setLogLevelFromString(const std::string& level) {
    if (level == "DEBUG") m_logLevel = LogLevel::Debug;
    else if (level == "INFO") m_logLevel = LogLevel::Info;
    else if (level == "WARN") m_logLevel = LogLevel::Warn;
    else if (level == "ERROR") m_logLevel = LogLevel::Error;
    else if (level == "CONSOLE") m_logLevel = LogLevel::Console;
    else if (level == "ALL") m_logLevel = LogLevel::All;
}

void Logging::setNode(uint32_t node) {
    local_node_number = node;
}

void Logging::append(std::ostringstream& oss,
                     const std::vector<uint8_t>& data) const {
    oss << "[";
    for (size_t i = 0; i < data.size(); ++i) {
        oss << std::hex << std::setw(2) << std::setfill('0')
            << static_cast<int>(data[i]);
        if (i + 1 < data.size())
            oss << ' ';
    }
    oss << "]";
    oss << std::dec;
}

bool Logging::shouldLog(LogLevel level) const {
    if (m_logLevel == LogLevel::All)
        return true;

    return static_cast<uint8_t>(level) >= static_cast<uint8_t>(m_logLevel);
}

const char* Logging::levelToString(LogLevel level) const {
    switch (level) {
        case LogLevel::Debug:   return "Debug";
        case LogLevel::Info:    return "Info";
        case LogLevel::Warn:    return "Warn";
        case LogLevel::Error:   return "Error";
        case LogLevel::Console: return "Console";
        case LogLevel::All:     return "All";
        default:                return "Unknown";
    }
}

std::string Logging::currentDateTimeMillis() const {
    using namespace std::chrono;

    auto now = system_clock::now();
    auto ms = duration_cast<milliseconds>(now.time_since_epoch()) % 1000;

    std::time_t t = system_clock::to_time_t(now);
    std::tm tm;

#if defined(_WIN32)
    localtime_s(&tm, &t);
#else
    localtime_r(&t, &tm);
#endif

    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S")
        << '.'
        << std::setw(3)
        << std::setfill('0')
        << ms.count();

    return oss.str();
}

void Logging::write(const std::string& message) const {
    std::lock_guard<std::mutex> lock(m_mutex);

    if (local_node_number > 0)
        std::cout << "Node " << local_node_number << " | ";
    else
        std::cout << "System | ";

    std::cout << message << std::endl;
}
