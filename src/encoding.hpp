#ifndef ENCODING_H
#define ENCODING_H

#include <iostream>
#include <string>
#include <mutex>
#include <map>

class Logging;

/**
 * @class Encoding
 * @author Michael Griffin
 * @date 15/12/2018
 * @file encoding.hpp
 * @brief Singleton Encoding Class to handle text conversions.
 */
class Encoding
{
public:

    static const std::string ENCODING_TEXT_UTF8;
    static const std::string ENCODING_TEXT_CP437;

    static constexpr int ENCODE_CP437 = 0;
    static constexpr int ENCODE_UTF8  = 1;

    /**
    * @brief Creates Singleton Instance of Class
    * @return
    */
    static Encoding& getInstance()
    {
        static Encoding instance;
        return instance;
    }
    
    // Multi-Byte to WIDE (UTF-8 to UTF-16)
    std::wstring multibyte_to_wide(const char* mbstr);

    /**
     * @brief Used for printing multibyte (Unicode Translations)
     * @param wide_string
     */
    std::string wide_to_multibyte(const std::wstring &wide_string);

    /**
     * @brief Translation from CP437 to UTF-8
     * @param standard_string
     */
    std::string utf8Encode(const std::string &standard_string);

    /**
     * @brief Translation from UTF-8 to CP437
     * @param standard_string
     */
    std::string utf8Decode(const std::string &standard_string);

private:

    Logging               &m_log;
    mutable std::mutex     m_encoding_mutex;

    explicit Encoding();
    
    Encoding(const Encoding&) = delete;             // Copy ctor
    Encoding(Encoding&&) = delete;                  // Move ctor
    Encoding& operator=(const Encoding&) = delete;  // Copy assignment
    Encoding& operator=(Encoding&&) = delete;       // Move assignment

};

#endif // ENCODING_H
