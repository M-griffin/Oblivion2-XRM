#ifndef LOGGING_HPP
#define LOGGING_HPP

#include <string>
#include <sstream>
#include <mutex>
#include <cstdint>
#include <vector>
#include <iomanip>
#include <type_traits>
#include <utility>

class Logging {
public:
    enum class LogLevel : uint8_t {
        Debug = 0,
        Info,
        Warn,
        Error,
        Console,
        All
    };

    static Logging& getInstance();

    // Configuration
    void setLogLevel(LogLevel level);
    void setLogLevelFromString(const std::string& level);
    void setNode(uint32_t node);

    // Logging API
    template<typename... Args>
    void log(LogLevel level, Args&&... args);

    // Deleted copy/move
    Logging(const Logging&) = delete;
    Logging& operator=(const Logging&) = delete;
    Logging(Logging&&) = delete;
    Logging& operator=(Logging&&) = delete;

    template<typename T, typename = void>
    struct is_streamable : std::false_type {};

    template<typename T>
    struct is_streamable<
        T,
        decltype(void(std::declval<std::ostringstream&>() << std::declval<T>()))
    > : std::true_type {};

private:
    Logging();

    // Helpers
    bool shouldLog(LogLevel level) const;
    std::string currentDateTimeMillis() const;
    const char* levelToString(LogLevel level) const;

    // 0️⃣ Base case (REQUIRED)
    void append(std::ostringstream&) const {}

    // 1️⃣ Byte buffer overload
    void append(std::ostringstream& oss,
                const std::vector<uint8_t>& data) const;

    // 2️⃣ Streamable types only
    template<typename T>
    typename std::enable_if<is_streamable<T>::value>::type
    append(std::ostringstream& oss, T&& value) const {
        oss << std::forward<T>(value);
    }

    // 3️⃣ Variadic dispatcher
    template<typename T, typename... Args>
    void append(std::ostringstream& oss, T&& first, Args&&... rest) const {
        append(oss, std::forward<T>(first));
        if (sizeof...(rest) > 0) {
            oss << ' ';
        }
        append(oss, std::forward<Args>(rest)...);
    }

    void write(const std::string& message) const;

private:
    LogLevel m_logLevel;
    mutable std::mutex m_mutex;
};

template<typename... Args>
void Logging::log(LogLevel level, Args&&... args) {
    if (!shouldLog(level))
        return;

    std::ostringstream oss;
    oss << currentDateTimeMillis()
        << " | "
        << levelToString(level)
        << " | ";

    append(oss, std::forward<Args>(args)...);

    write(oss.str());
}

#endif