#ifndef COMMON_IO_HPP
#define COMMON_IO_HPP

#include <ctime>
#include <iostream>
#include <string>
#include <vector>
#include <cstdint>

#include "encoding.hpp"

class Logging;

// Possible Replacement for Localized Buffer.
typedef struct Utf8Glyph {
    std::string bytes;   // UTF-8 bytes
    std::size_t length;  // byte length (1–4)

    bool isAscii() const { return length == 1; }
} Utf8Glyph;

/**
 * @class CommonIO
 * @author Michael Griffin
 * @date 9/21/2015
 * @file common_io.hpp
 * @brief Low Level IO ASCII and UTF-8 Aware with locale support.
 */
class CommonIO {
    Logging &m_log; // Logging Reference

    std::string m_escape_sequence; // ParseInput
    std::string m_string_buffer; // ParseInput
    std::string m_incoming_data; // ParseInput
    std::string m_line_buffer; // GetLine
    int m_column_position; // GetLine
    bool m_is_escape_sequence; // ParseInput
    bool m_is_new_getline; // GetLine
    bool m_is_new_leadoff; // GetLine

    Encoding m_encode;
    Encoding::TextEncoding m_encoding = Encoding::TextEncoding::ASCII;

public:
    explicit CommonIO();

    ~CommonIO();

    CommonIO(CommonIO &&other) noexcept
        : m_log(other.m_log) // reference copied, not moved
          , m_escape_sequence(std::move(other.m_escape_sequence))
          , m_string_buffer(std::move(other.m_string_buffer))
          , m_incoming_data(std::move(other.m_incoming_data))
          , m_line_buffer(std::move(other.m_line_buffer))
          , m_column_position(other.m_column_position)
          , m_is_escape_sequence(other.m_is_escape_sequence)
          , m_is_new_getline(other.m_is_new_getline)
          , m_is_new_leadoff(other.m_is_new_leadoff) {
    }

    CommonIO &operator=(CommonIO &&other) noexcept {
        if (this != &other) {
            m_escape_sequence = std::move(other.m_escape_sequence);
            m_string_buffer = std::move(other.m_string_buffer);
            m_incoming_data = std::move(other.m_incoming_data);
            m_line_buffer = std::move(other.m_line_buffer);

            m_column_position = other.m_column_position;
            m_is_escape_sequence = other.m_is_escape_sequence;
            m_is_new_getline = other.m_is_new_getline;
            m_is_new_leadoff = other.m_is_new_leadoff;
        }
        return *this;
    }

    /**
     * @brief Set up a Static GLobal Map for Key Input that can be resued.
     */
    void populateInputSequenceMap();

    /**
     * @brief Retrieve Key Sequence by Value
     * @param value
     */
    std::string getSequenceFromMap(const std::string &value);

    /* This function will read the OS specific functions
     * To Determine where the executable is located.
     * This has only been tested in Windows, Linux, OSX.
     */
    std::string getProgramPath(const std::string &program_name);

    /**
     * Get The BBS System Users HOME directory
     */
    std::string getSystemHomeDirectory();

    /**
     * @brief Appends Path Separator depending on environment.
     * @param path
     * @return
     */
    void pathAppend(std::string &path);

    /**
     * String Length counting actual characters not bytes
     * This is for mixed ASCII And UTF-8 Strings.
     */
    std::string::size_type numberOfChars(const std::string &str);
    std::string::size_type numberOfChars2(const std::string &str);

    /**
     * Left Trim White spaces (Front)
     */
    std::string leftTrim(const std::string &str);

    /**
     * Right Trim White spaces (Back)
     */
    std::string rightTrim(const std::string &str);

    /**
     * Trim White spaces from both ends
     */
    std::string trim(const std::string &str);

    /**
     * UTF-8 Aware
     * Removes All Characters Between Start and End Positions.
     */
    std::string eraseString(const std::string &str,
                            std::string::size_type start_position,
                            std::string::size_type end_position = 0);

    std::string eraseString2(const std::string &str,
                                  std::string::size_type start_position,
                                  std::string::size_type count);

    /**
     * Right String Padding
     */
    std::string rightPadding(const std::string &str, std::string::size_type space);

    /**
     * Left String Padding
     */
    std::string leftPadding(const std::string &str, std::string::size_type space);

    /**
     * Center String Padding
     * Note: Need to add ANSI / PIPE parsing!
     */
    std::string centerPadding(const std::string &str, int term_width);

    /**
     * Mask String
     */
    std::string maskString(const std::string &str);

    /**
     * Check Digit or Numbers in String
     * This is for mixed ASCII And UTF-8 Strings.
     */
    bool isDigit(const std::string &str);

    /**
     * @brief Return the Input Full Screen Editor Escape Sequence Parsed.
     * @return
     */
    std::string getFSEEscapeSequence();

    /**
    * @brief Return the Escape Sequence
    * @return
    */
    std::string getEscapeSequence();

    /**
     * @brief Parses Input and breaks out ESC Sequences.
     * @return
     */
    std::string parseInput(const std::string &character_buffer);

    /**
    * @brief Returns the InputFieldBuffer
    * @return
    */
    std::string getInputBuffer();

    /**
     * @brief Returns processing of InputField until NL is received.
     * @param line
     * @param length
     * @param leadoff
     * @param hidden
     * @return
     */
    std::string getLine(const std::string &line, // Parsed Char input in
                        int length, // Max Input Length of Field
                        const std::string &leadoff, // Data to Display in Default Field {Optional}
                        bool hidden); // If input is hidden or masked     {Optional}

    /**
     * @brief Converts Pascal Strings to C-Strings Also return std::string for conversions.
     * @param string
     */
    std::string PascalToCString(int8_t *string);

    /**
     * @brief Converts C-Strings to Pascal Strings
     * @param string
     */
    void CStringToPascal(int8_t *string);

    /**
    * @brief Helper Method to display bool as string.
    * @param value
    * @return
    */
    std::string boolAlpha(bool value);

    /**
     * @brief Parse / Replace MCI String from given string.
     * @param AnsiString
     * @param mcicode
     * @param replacement
     */
    void parseLocalMCI(std::string &AnsiString, const std::string &mcicode, const std::string &replacement);

    /**
     * @brief Check if the file exists
     * @return
     */
    bool fileExists(const std::string &file_name);

    /**
     * @brief Reads in Ansi file into Buffer Only
     * @param FileName
     * @return
     */
    std::string readinAnsi(const std::string &file_name);

    /**
     * @brief Split Strings by delimiter into Vector of Strings.
     * @param s
     * @param delimiter
     * @return
     */
    std::vector<std::string> splitString(const std::string &s, char delimiter);

    /**
     * @brief Standard Time to Date String
     * @param std_time
     * @return
     */
    std::string standardDateToString(std::time_t std_time);

    /**
     * @brief Standard Time to Date/Time String
     * @param std_time
     * @return
     */
    std::string standardDateTimeToString(std::time_t std_time);

    /**
     * @brief String to Date Format
     * @param date
     * @return
     */
    std::time_t stringToStandardDate(const std::string &date);

    /**
     * @brief String to Date/Time Format
     * @param date_time
     * @return
     */
    std::time_t stringToStandardDateTime(const std::string &date_time);

    /**
     * @brief Converts std::strings to Long values
     * @param value
     * @return
     */
    long stringToLong(const std::string &value);

    /**
     * @brief Converts std::strings to Int values
     * @param value
     * @return
     */
    int stringToInt(const std::string &value);

    /**
     * @brief Tests first char of string for starting T/F returns int with -1 for invalid
     * @param value
     * @return
     */
    int stringToBool(const std::string &value);

    /**
     * @brief Template search quick find
     * @param element
     * @param container
     * @return
     */
    template<class Element, class Container>
    bool in_array(const Element &element, const Container &container) {
        return std::find(std::begin(container), std::end(container), element)
               != std::end(container);
    }

    template<typename octet_type>
    inline uint8_t mask8(octet_type oc) {
        return static_cast<uint8_t>(0xff & oc);
    }

    bool nextGlyph(const std::string& s,
               std::string::const_iterator& it,
               Utf8Glyph& glyph);

    bool peekGlyph(const std::string& s,
               std::string::const_iterator it,
               Utf8Glyph& glyph);

    void onTcpReceive(const std::string& chunk);

    bool decodeNextGlyph(const std::string& bytes,
                 std::string::const_iterator& it,
                 Encoding::TextEncoding encoding,
                 Utf8Glyph& glyph);

    // UTF8 Internal to CP437

    void initCp437ReverseTable();

    bool nextCodepoint(const std::string& utf8,
                   std::string::const_iterator& it,
                   uint32_t& cp);

    std::string utf8ToCp437(const std::string& utf8);

};
#endif
