#ifndef COMMON_IO_HPP
#define COMMON_IO_HPP

#include <ctime>
#include <iostream>
#include <string>
#include <vector>
#include <cstdint>

#include "io_encoding.hpp"

class UtilLog;

// Possible Replacement for Localized Buffer.
typedef struct Utf8Glyph {
    std::string bytes;   // UTF-8 bytes
    std::size_t length;  // byte length (1–4)

    bool isAscii() const { return length == 1; }
} Utf8Glyph;

/**
 * @class IoCommon
 * @author Michael Griffin
 * @date 9/21/2015
 * @file io_common.hpp
 * @brief Low Level IO ASCII and UTF-8 Aware with locale support.
 */
class IoCommon {
    UtilLog &m_log; // UtilLog Reference

    std::string m_utf8_rx_buffer; // buffer.
    std::string m_escape_sequence; // ParseInput
    std::string m_string_buffer; // ParseInput
    std::string m_incoming_data; // ParseInput
    std::string m_line_buffer; // GetLine
    int m_column_position; // GetLine
    bool m_is_escape_sequence; // ParseInput
    bool m_is_new_getline; // GetLine
    bool m_is_new_leadoff; // GetLine

    IoEncoding m_encode;
    IoEncoding::TextEncoding m_encoding = IoEncoding::TextEncoding::ASCII;

public:
    explicit IoCommon();

    ~IoCommon();

    IoCommon(IoCommon &other) = delete;
    IoCommon &operator=(IoCommon &other) = delete;
    IoCommon(IoCommon &&other) = delete;
    IoCommon &operator=(IoCommon &&other) = delete;

    /**
    IoCommon(IoCommon &&other) noexcept
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

    IoCommon &operator=(IoCommon &&other) noexcept {
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
    }*/

    void populateInputSequenceMap();
    std::string getSequenceFromMap(const std::string &value);
    std::string getProgramPath(const std::string &program_name);
    void pathAppend(std::string &path);

    std::string::size_type numberOfChars(const std::string &str);
    std::string::size_type numberOfChars2(const std::string &str);

    std::string leftTrim(const std::string &str);
    std::string rightTrim(const std::string &str);
    std::string trim(const std::string &str);

    std::string eraseString(const std::string &str,
                            std::string::size_type start_position,
                            std::string::size_type end_position = 0);

    std::string eraseString2(const std::string &str,
                                  std::string::size_type start_position,
                                  std::string::size_type count);

    std::string rightPadding(const std::string &str, std::string::size_type space);
    std::string leftPadding(const std::string &str, std::string::size_type space);
    std::string centerPadding(const std::string &str, int term_width);
    std::string maskString(const std::string &str);
    bool isDigit(const std::string &str);

    std::string getFSEEscapeSequence();
    std::string getEscapeSequence();
    std::string parseInput(const std::string &character_buffer);
    std::string getInputBuffer();
    std::string getLine(const std::string &line, // Parsed Char input in
                        int length, // Max Input Length of Field
                        const std::string &leadoff, // Data to Display in Default Field {Optional}
                        bool hidden); // If input is hidden or masked     {Optional}

    std::string PascalToCString(int8_t *string);
    void CStringToPascal(int8_t *string);
    std::string boolAlpha(bool value);
    void parseLocalMCI(std::string &AnsiString, const std::string &mcicode, const std::string &replacement);
    bool fileExists(const std::string &file_name);
    std::string readAnsi(const std::string &FileName);
    std::vector<std::string> splitString(const std::string &s, char delimiter);
    std::string standardDateToString(std::time_t std_time);
    std::string standardDateTimeToString(std::time_t std_time);
    std::time_t stringToStandardDate(const std::string &date);
    std::time_t stringToStandardDateTime(const std::string &date_time);
    long stringToLong(const std::string &value);
    int stringToInt(const std::string &value);
    int stringToBool(const std::string &value);

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
                 IoEncoding::TextEncoding encoding,
                 Utf8Glyph& glyph);

    // UTF8 Internal to CP437
    void initCp437ReverseTable();
    bool nextCodepoint(const std::string& utf8,
                   std::string::const_iterator& it,
                   uint32_t& cp);

    std::string utf8ToCp437(const std::string& utf8);
    std::string toUpper(const std::string &value);
    std::string toLower(const std::string &value);

};
#endif
