#ifndef ENCODING_H
#define ENCODING_H

#include <iostream>
#include <string>
#include <mutex>
#include <map>

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
    static Encoding* instance()
    {
        if(!m_global_encoding_instance)
        {
            m_global_encoding_instance = new Encoding();
            return m_global_encoding_instance;
        }

        return m_global_encoding_instance;
    }

    /**
     * @brief Releases the Instance from Memory
     */
    static void releaseInstance()
    {
        if(m_global_encoding_instance)
        {
            delete m_global_encoding_instance;
            m_global_encoding_instance = nullptr;
        }

        return;
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

    mutable std::mutex     m_encoding_mutex;
    static Encoding*       m_global_encoding_instance;

    explicit Encoding();
    ~Encoding();
    Encoding(const Encoding&);
    Encoding& operator=(const Encoding&);

};

#endif // ENCODING_H
