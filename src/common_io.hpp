#ifndef COMMONIO_HPP
#define COMMONIO_HPP

#include <ctime>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <stdint.h>


// Localized Buffer.
typedef struct localized_buffer
{
    std::string character;
    uint8_t     length;

    void clear()
    {
        character.erase();
        length = 0;
    }

} LocalizedBuffer;

/**
 * @class CommonIO
 * @author Michael Griffin
 * @date 9/21/2015
 * @file common_io.hpp
 * @brief Low Level IO ASCII and UTF-8 Aware with locale support.
 */
class CommonIO
{

public:

    explicit CommonIO();
    ~CommonIO();

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
    std::string getLine(const std::string &line,    // Parsed Char input in
                        int   length,               // Max Input Length of Field
                        const std::string &leadoff, // Data to Display in Default Field {Optional}
                        bool  hidden);              // If input is hidden or masked     {Optional}

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
    bool fileExists(std::string file_name);

    /**
     * @brief Reads in Ansi file into Buffer Only
     * @param FileName
     * @return
     */
    std::string readinAnsi(std::string file_name);

    /**
     * @brief Split Strings by delimiter into Vector of Strings.
     * @param s
     * @param delimiter
     * @return
     */
    std::vector<std::string> splitString(const std::string& s, char delimiter);

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
    std::time_t stringToStandardDate(std::string date);

    /**
     * @brief String to Date/Time Format
     * @param date_time
     * @return
     */
    std::time_t stringToStandardDateTime(std::string date_time);

    /**
     * @brief Converts std::strings to Long values
     * @param value
     * @return
     */
    long stringToLong(std::string value);

    /**
     * @brief Converts std::strings to Int values
     * @param value
     * @return
     */
    int stringToInt(std::string value);

    /**
     * @brief Tests first char of string for starting T/F returns int with -1 for invalid
     * @param value
     * @return
     */
    int stringToBool(std::string value);

    /**
     * @brief Template search quick find
     * @param element
     * @param container
     * @return
     */
    template<class Element, class Container>
    bool in_array(const Element & element, const Container & container)
    {
        return std::find(std::begin(container), std::end(container), element)
               != std::end(container);
    }

    template<typename octet_type>
    inline uint8_t mask8(octet_type oc)
    {
        return static_cast<uint8_t>(0xff & oc);
    }

    /**
     * @brief Parses screen data into the Screen Buffer.
     * @return
     */
    void getNextGlyph(LocalizedBuffer &buffer, std::string::iterator &it,
                      std::string::iterator &line_end);

    /**  TEMP EXTRA LOGGING!
     * @brief Parses screen data into the Screen Buffer.
     * @return
     */
    void getNextGlyph2(LocalizedBuffer &buffer, std::string::iterator &it,
                       std::string::iterator &line_end);

    /**
     * @brief Parses screen data into the Screen Buffer.
     * @return
     */
    void peekNextGlyph(LocalizedBuffer &buffer, std::string::iterator &it,
                       std::string::iterator &line_end);

    void testUnicode(std::string incoming_data);

private:

    /**
     * @brief Globals for Input Methods
     * Unique per session so don't want these static.
     */
    std::string m_escape_sequence; // ParseInput
    std::string m_string_buffer;   // ParseInput
    std::string m_incoming_data;   // ParseInput
    std::string m_line_buffer;     // GetLine
    int m_column_position;         // GetLine
    bool m_is_escape_sequence;     // ParseInput
    bool m_is_new_getline;         // GetLine
    bool m_is_new_leadoff;         // GetLine

    // Parameterized ESC Sequcnes Translations.
    std::map<std::string, std::string> m_sequence_map;

};
#endif
