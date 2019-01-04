#include "common_io.hpp"

#include "model-sys/structures.hpp"
#include "encoding.hpp"
#include "logging.hpp"

#include <unistd.h>
#include <sys/types.h>

#ifdef TARGET_OS_MAC
#include <mach-o/dyld.h>
#elif _WIN32
#include <windows.h>
#endif

#include <unistd.h>
#include <sys/types.h>

#ifndef _WIN32
#include <pwd.h>
#endif

#include <cstdio>
#include <cstring>
#include <climits>
#include <cstdlib>
#include <ctime>
#include <iomanip>

#include <stdexcept>

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <functional>
#include <cctype>
#include <sstream>
#include <fstream>
#include <regex>

#include <utf-cpp/utf8.h>

CommonIO::CommonIO()
    : m_escape_sequence("")
    , m_string_buffer("")
    , m_incoming_data("")
    , m_line_buffer("")
    , m_column_position(0)
    , m_is_escape_sequence(false)
    , m_is_new_getline(true)
    , m_is_new_leadoff(true)
{
    // Arror Keys, Hardware OA are translated to [A on the fly
    m_sequence_map.insert(std::make_pair("[A",    "up_arrow"));
    m_sequence_map.insert(std::make_pair("[B",    "dn_arrow"));
    m_sequence_map.insert(std::make_pair("[C",    "rt_arrow"));
    m_sequence_map.insert(std::make_pair("[D",    "lt_arrow"));

    // Hardware Keys, or Numpad.
    m_sequence_map.insert(std::make_pair("OA",    "up_arrow"));
    m_sequence_map.insert(std::make_pair("OB",    "dn_arrow"));
    m_sequence_map.insert(std::make_pair("OC",    "rt_arrow"));
    m_sequence_map.insert(std::make_pair("OD",    "lt_arrow"));
    m_sequence_map.insert(std::make_pair("OE",    "clear"));
    m_sequence_map.insert(std::make_pair("OF",    "end"));
    m_sequence_map.insert(std::make_pair("OH",    "home"));

    // Shift Arrow Keys
    m_sequence_map.insert(std::make_pair("[1;2A", "shift_up_arrow"));
    m_sequence_map.insert(std::make_pair("[1;2B", "shift_dn_arrow"));
    m_sequence_map.insert(std::make_pair("[1;2C", "shift_rt_arrow"));
    m_sequence_map.insert(std::make_pair("[1;2D", "shift_lt_arrow"));

    // Shift TAB
    m_sequence_map.insert(std::make_pair("[Z",    "shift_tab"));

    // Function Keys ANSI
    m_sequence_map.insert(std::make_pair("[@",    "insert"));
    m_sequence_map.insert(std::make_pair("[H",    "home"));
    m_sequence_map.insert(std::make_pair("[K",    "end"));
    m_sequence_map.insert(std::make_pair("[F",    "end")); // = 0F
    m_sequence_map.insert(std::make_pair("[V",    "pg_up"));
    m_sequence_map.insert(std::make_pair("[U",    "pg_dn"));
    m_sequence_map.insert(std::make_pair("[OP",   "f1"));
    m_sequence_map.insert(std::make_pair("[OQ",   "f2"));
    m_sequence_map.insert(std::make_pair("[OR",   "f3"));
    m_sequence_map.insert(std::make_pair("[OS",   "f4"));
    m_sequence_map.insert(std::make_pair("[OT",   "f5"));
    m_sequence_map.insert(std::make_pair("[[17~", "f6"));
    m_sequence_map.insert(std::make_pair("[[18~", "f7"));
    m_sequence_map.insert(std::make_pair("[[19~", "f8"));
    m_sequence_map.insert(std::make_pair("[[20~", "f9"));
    m_sequence_map.insert(std::make_pair("[[21~", "f10"));
    m_sequence_map.insert(std::make_pair("[[23~", "f11"));
    m_sequence_map.insert(std::make_pair("[[24~", "f12"));

    // VT-100 Putty
    m_sequence_map.insert(std::make_pair("[1~",   "home"));
    m_sequence_map.insert(std::make_pair("[2~",   "insert"));
    m_sequence_map.insert(std::make_pair("[3~",   "del"));
    m_sequence_map.insert(std::make_pair("[4~",   "end"));
    m_sequence_map.insert(std::make_pair("[5~",   "pg_up"));
    m_sequence_map.insert(std::make_pair("[6~",   "pg_dn"));
    m_sequence_map.insert(std::make_pair("[OU",   "f6"));
    m_sequence_map.insert(std::make_pair("[OV",   "f7"));
    m_sequence_map.insert(std::make_pair("[OW",   "f8"));
    m_sequence_map.insert(std::make_pair("[OX",   "f9"));
    m_sequence_map.insert(std::make_pair("[OY",   "f10"));
    m_sequence_map.insert(std::make_pair("[OZ",   "f11"));
    m_sequence_map.insert(std::make_pair("[O[",   "f12"));

    // Linux Console
    m_sequence_map.insert(std::make_pair("[[A",   "f1"));
    m_sequence_map.insert(std::make_pair("[[B",   "f2"));
    m_sequence_map.insert(std::make_pair("[[C",   "f3"));
    m_sequence_map.insert(std::make_pair("[[D",   "f4"));
    m_sequence_map.insert(std::make_pair("[[E",   "f5"));

    // SCO
    m_sequence_map.insert(std::make_pair("[L",    "insert"));
    m_sequence_map.insert(std::make_pair("[I",    "pg_up"));
    m_sequence_map.insert(std::make_pair("[G",    "pg_dn"));

    m_sequence_map.insert(std::make_pair("[[M",   "f1"));
    m_sequence_map.insert(std::make_pair("[[N",   "f2"));
    m_sequence_map.insert(std::make_pair("[[O",   "f3"));
    m_sequence_map.insert(std::make_pair("[[P",   "f4"));
    m_sequence_map.insert(std::make_pair("[[Q",   "f5"));
    m_sequence_map.insert(std::make_pair("[[R",   "f6"));
    m_sequence_map.insert(std::make_pair("[[S",   "f7"));
    m_sequence_map.insert(std::make_pair("[[T",   "f8"));
    m_sequence_map.insert(std::make_pair("[[U",   "f9"));
    m_sequence_map.insert(std::make_pair("[[V",   "f10"));
    m_sequence_map.insert(std::make_pair("[[W",   "f11"));
    m_sequence_map.insert(std::make_pair("[[X",   "f12"));

    // rxvt
    m_sequence_map.insert(std::make_pair("[7~",   "home"));
    m_sequence_map.insert(std::make_pair("[8~",   "end"));

    // Shift Arrow Keys
    m_sequence_map.insert(std::make_pair("[a",    "shift_up_arrow"));
    m_sequence_map.insert(std::make_pair("[b",    "shift_dn_arrow"));
    m_sequence_map.insert(std::make_pair("[c",    "shift_rt_arrow"));
    m_sequence_map.insert(std::make_pair("[d",    "shift_lt_arrow"));
    m_sequence_map.insert(std::make_pair("[e",    "shift_clear"));

    // Shift Function
    m_sequence_map.insert(std::make_pair("[2$",   "insert"));
    m_sequence_map.insert(std::make_pair("[3$",   "del"));
    m_sequence_map.insert(std::make_pair("[5$",   "pg_up"));
    m_sequence_map.insert(std::make_pair("[6$",   "pg_dn"));
    m_sequence_map.insert(std::make_pair("[7$",   "home"));
    m_sequence_map.insert(std::make_pair("[8$",   "end"));

    // Ctrl
    m_sequence_map.insert(std::make_pair("Oa",    "ctrl_up_arrow"));
    m_sequence_map.insert(std::make_pair("Ob",    "ctrl_dn_arrow"));
    m_sequence_map.insert(std::make_pair("Oc",    "ctrl_rt_arrow"));
    m_sequence_map.insert(std::make_pair("Od",    "ctrl_lt_arrow"));
    m_sequence_map.insert(std::make_pair("Oe",    "ctrl_clear"));

    // Shift Function
    m_sequence_map.insert(std::make_pair("[2^",   "ctrl_insert"));
    m_sequence_map.insert(std::make_pair("[3^",   "ctrl_del"));
    m_sequence_map.insert(std::make_pair("[5^",   "ctrl_pg_up"));
    m_sequence_map.insert(std::make_pair("[6^",   "ctrl_pg_dn"));
    m_sequence_map.insert(std::make_pair("[7^",   "ctrl_home"));
    m_sequence_map.insert(std::make_pair("[8^",   "ctrl_end"));
}

CommonIO::~CommonIO()
{
    m_sequence_map.clear();
    m_escape_sequence.erase();
}

/**
 * @brief Determine where the executable is located.
 * This has only been tested in Windows, Linux, OSX.
 * @return
 */
std::string CommonIO::getProgramPath(const std::string &program_name)
{
    // NOTE This methid can not use logging, called prior to configuration load.
    std::string program_path;
    std::string program = "/" + program_name;

    // First check for SYSTEM environmental variable:
    char *pPath;
    pPath = std::getenv((char *)"OBV2");

    if(pPath != nullptr)
    {
        std::cout << "Found OBV2 Enviroment Variable=" << pPath << std::endl;
        program_path = pPath;

#ifdef _WIN32

        if(program_path[program_path.size()-1] != '\\')
        {
            program_path.append("\\");
        }

#else

        if(program_path[program_path.size()-1] != '/')
        {
            program_path.append("/");
        }

#endif

        return program_path;
    }
    else
    {
        std::cout << "looking up program path" << std::endl;
    }

    // Get the Folder the Executable runs in.
#ifdef TARGET_OS_MAC
    char current_path[PATH_MAX];
    uint32_t size = sizeof(current_path);

    if(_NSGetExecutablePath(current_path, &size) != 0)
    {
        std::cout << "Error, getProgramPath: OSX Path empty!" << std::endl;
        throw std::runtime_error("getProgramPath: OSX Path");
    }

    // Remove Executable
    program_path = current_path;
    std::string::size_type position;
    position = program_path.rfind(program);

    if(position != std::string::npos)
    {
        program_path.erase(position+1,program_path.size()-1);
    }

    // remove extra './'
    position = program_path.rfind("./");

    if(position != std::string::npos)
    {
        program_path.erase(position,2);
    }

#elif _WIN32
    // Get the Current Program path.
    char current_path[PATH_MAX];

    int result = GetModuleFileName(NULL, current_path, PATH_MAX-1);

    if(result == 0)
    {
        std::cout << "Error, getProgramPath: Win32 Path empty!" << std::endl;
        throw std::runtime_error("GetProgramPath: Win32 Path");
    }

    program_path = current_path;
    std::string::size_type position = program_path.rfind("\\", program_path.size()-1);

    if(position != std::string::npos)
    {
        program_path.erase(position+1);
    }

#else

    char exe_path[PATH_MAX] = {0};
    ssize_t result = readlink("/proc/self/exe", exe_path, PATH_MAX);

    if(result < 0)
    {
        std::cout << "Error, getProgramPath: Linux Path empty!" << std::endl;
        throw std::runtime_error("getProgramPath: Linux Path");
    }

    const char* t = " \t\n\r\f\v";
    program_path = exe_path;

    std::cout << "Original Path=" << program_path << std::endl;
    program_path = program_path.erase(program_path.find_last_not_of(t) + 1);
    program_path += "/";

    // Remove Executable
    std::string::size_type position;
    position = program_path.rfind(program);

    if(position != std::string::npos)
    {
        program_path.erase(position+1,program_path.size()-1);
    }

#endif
    return program_path;
}

#ifndef _WIN32
/**
 * @brief Get The BBS System Users HOME directory
 * @return
 */
std::string CommonIO::getSystemHomeDirectory()
{
    std::string home_directory;
    const char *homedir;

    homedir = getenv("HOME");

    if(!homedir)
    {
        homedir = getpwuid(getuid())->pw_dir;

        if(!homedir)
        {
            std::cout << "!WIN32, Unable to locate home directory" << std::endl;

            home_directory = "";
            return home_directory;
        }
    }

    home_directory = homedir;

    return home_directory;
}
#endif

/**
 * @brief Appends Path Separator depending on environment.
 * @param path
 * @return
 */
void CommonIO::pathAppend(std::string &path)
{
#ifdef _WIN32
    path.append("\\");
#else
    path.append("/");
#endif
}

/**
 * @brief String Length counting actual characters not bytes
 * @param str
 * @return
 */
std::string::size_type CommonIO::numberOfChars(const std::string &str)
{
    std::string::size_type number_characters = 0;

    if(str.size() == 0)
    {
        return number_characters;
    }

    std::string string_builder = str;
    std::string::iterator it = string_builder.begin();
    std::string::iterator line_end = string_builder.end();

    while(it != line_end)
    {
        int byte_value = static_cast<int>((uint8_t)*it);

        if(byte_value < 128)
        {
            *it++;
            ++number_characters;
        }
        else
        {
            try
            {
                // Iterate quickly to next sequence.
                utf8::next(it, line_end);
                ++number_characters;
            }
            catch(utf8::exception &ex)
            {
                *it++;
                ++number_characters;
                Logging *log = Logging::instance();
                log->xrmLog<Logging::ERROR_LOG>("[numberOfChars] UTF8 Parsing Exception=", ex.what(), __LINE__, __FILE__);
            }
        }
    }

    return number_characters;
}

/**
 * @brief Left Trim White spaces (Front)
 * @param str
 * @return
 */
std::string CommonIO::leftTrim(const std::string &str)
{
    std::string new_string = str;

    if(new_string.empty())
    {
        return new_string;
    }

    new_string.erase(
        new_string.begin(),
        std::find_if(new_string.begin(),
                     new_string.end(),
                     std::not1(std::ptr_fun<int, int>(std::isspace)))
    );

    return new_string;
}

/**
 * @brief Right Trim Whitepsaces (Back)
 * @param str
 * @return
 */
std::string CommonIO::rightTrim(const std::string &str)
{
    std::string new_string = str;

    if(new_string.empty())
    {
        return new_string;
    }

    new_string.erase(std::find_if(
                         new_string.rbegin(),
                         new_string.rend(),
                         std::not1(std::ptr_fun<int, int>(std::isspace))).base(),
                     new_string.end()
                    );

    return new_string;
}

/**
 * @brief Trim White spaces from both ends
 * @param str
 * @return
 */
std::string CommonIO::trim(const std::string &str)
{
    std::string new_string = str;

    if(new_string.empty())
    {
        return new_string;
    }

    return leftTrim(rightTrim(new_string));
}

/**
 * @brief Removes All Data after specified number of characters
 * @param str
 * @param start_position
 * @param end_position
 */
std::string CommonIO::eraseString(const std::string &str,
                                  std::string::size_type start_position,
                                  std::string::size_type end_position)
{
    std::string new_string = str;
    std::string::size_type string_size = new_string.size();

    // 0 defaults to end of string.
    // Otherwise End Position is added for number of characters to remove.
    if(end_position == 0)
    {
        end_position = string_size;
    }
    else
    {
        end_position = start_position + (end_position-1);

        // Make sure we can never go past end of string!
        if(end_position > string_size)
        {
            end_position = string_size;
        }
    }

    std::string new_string_builder = "";

    if(new_string.empty())
    {
        Logging *log = Logging::instance();
        log->xrmLog<Logging::DEBUG_LOG>("(Common::EraseString) string length == 0", __LINE__, __FILE__);
        return new_string;
    }

    unsigned char_count = 0;
    std::string::iterator it = new_string.begin();
    std::string::iterator line_end = new_string.end();

    while(it != line_end)
    {
        int byte_value = static_cast<int>((uint8_t)*it);

        if(byte_value < 128)
        {
            if(char_count < start_position || char_count > end_position)
            {
                new_string_builder += std::string(1, *it);
            }

            *it++;
        }
        else
        {
            try
            {
                uint32_t code_point = utf8::next(it, line_end);

                if(char_count < start_position || char_count > end_position)
                {
                    // This convert the uint32_t code point to char array
                    // So each sequence can be writen as seperate byte.
                    unsigned char character[5] = {0,0,0,0,0};
                    utf8::append(code_point, character);

                    for(int i = 0; i < 5; i++)
                    {
                        if(character[i] != 0)
                        {
                            new_string_builder += std::string(1, character[i]);
                        }
                    }
                }
            }
            catch(utf8::exception &ex)
            {
                Logging *log = Logging::instance();
                log->xrmLog<Logging::DEBUG_LOG>("(Common::EraseString) UTF8 Parsing Exception=", ex.what(), __LINE__, __FILE__);
                *it++;
            }
        }

        ++char_count;
    }

    return new_string_builder;
}

/**
 * @brief Right String Padding
 * @param str
 * @param space
 * @return
 */
std::string CommonIO::rightPadding(const std::string &str, std::string::size_type space)   // Pad Right
{
    std::string padded_line = "";
    std::string new_string = str;

    if(space == 0)
        return new_string;

    // If empty, return padded with spaces!
    if(new_string.empty())
    {
        for(std::string::size_type i = 0; i < space; i++)
        {
            padded_line += ' ';
        }

        return padded_line;
    }

    std::string::size_type s = numberOfChars(new_string);

    // if Line > Sapce, Erase to match length
    if(s > space)
    {
        std::string erased = eraseString(new_string, space);
        return erased;
    }

    for(std::string::size_type i = 0; i < (space-s); i++)
    {
        padded_line += ' ';
    }

    new_string.append(padded_line);
    return new_string;
}

/**
 * @brief Left String Padding
 * @param str
 * @param space
 * @return
 */
std::string CommonIO::leftPadding(const std::string &str, std::string::size_type space)
{
    std::string new_string = str;

    if(space == 0)
    {
        return new_string;
    }

    // If empty, return padded with spaces!
    if(new_string.empty())
    {
        std::string padded_line = "";

        for(std::string::size_type i = 0; i < space; i++)
        {
            padded_line += ' ';
        }

        return padded_line;
    }

    std::string::size_type s = numberOfChars(new_string);

    // if Line > Sapce, Erase to match length
    if(s >= space)
    {
        std::string erased = eraseString(new_string, 0, s-space);
        return erased;
    }

    for(std::string::size_type i = 0; i < (space-s); i++)
    {
        new_string.insert(0, " ");
    }

    return new_string;
}

/**
 * @brief Center String Padding (Note: Need to add Code to Strip Pipes and ANSI)
 * @param str
 * @param term_width
 * @return
 */
std::string CommonIO::centerPadding(const std::string &str, int term_width)
{
    std::string new_string = str;

    if(new_string.empty())
    {
        return new_string;
    }

    std::string::size_type length = numberOfChars(new_string);

    if(length == 0)
    {
        return new_string;
    }

    int space = term_width / 2;
    space -= term_width % 2;

    space -= length / 2;
    space -= length % 2;

    // Text larger then screen, then leave alone.
    if(space <= 0) return new_string;

    std::string padded_line;

    for(int i = 0; i < space; i++)
    {
        padded_line += ' ';
    }

    // Appending currnet data after Padding.
    new_string.insert(0,padded_line);
    return new_string;
}

/**
 * @brief Mask String
 * @param str
 * @return
 */
std::string CommonIO::maskString(const std::string &str)
{
    std::string new_string = str;

    if(new_string.empty())
    {
        return new_string;
    }

    std::string::size_type string_size = numberOfChars(new_string);

    if(string_size == 0)
    {
        return new_string;
    }

    new_string.erase();

    for(std::string::size_type i = 0; i < string_size; i++)
    {
        new_string.append("*");
    }

    return new_string;
}

/**
 * @brief Check if Digit or Numeric's in a String
 * @param str
 * @return
 */
bool CommonIO::isDigit(const std::string &str)
{
    // Later reference for
    // Better wide characters.
    // https://www.cs.helsinki.fi/group/boi2016/doc/cppreference/reference
    //     /en.cppreference.com/w/cpp/locale/isdigit.html
    std::string::size_type num_digits = std::count_if(str.begin(), str.end(),
                                        [](unsigned char c)
    {
        return std::isdigit(c);
    }
                                                     );

    return num_digits == str.size();
}

/**
 * @brief Return the Escape Sequence Parsed.
 * @return
 */
std::string CommonIO::getEscapeSequence()
{
    // Check if Sequences Exists, otherwise return blank.
    if(m_sequence_map.find(m_escape_sequence) != m_sequence_map.end())
    {
        return m_sequence_map[m_escape_sequence];
    }

    return "";
}

/**
 * @brief Parses Input and breaks out ESC Sequences.
 *        Sequences are handled 1 character at a time.
 * @return
 */
std::string CommonIO::parseInput(const std::string &character_buffer)
{
    int num = numberOfChars(character_buffer);

    if((num == 0  ||  character_buffer[0] == '\x1b') &&
            m_is_escape_sequence &&
            m_string_buffer.size() == 1)
    {
        // This a null after an ESC..
        m_is_escape_sequence = false;
        m_escape_sequence.erase();
        m_string_buffer.erase();
        return "\x1b";
    }
    else if(num != 1)
    {
        Logging *log = Logging::instance();
        log->xrmLog<Logging::ERROR_LOG>("This function expects single characters/glyphs=", character_buffer, __LINE__, __FILE__);
        return "";
    }

    // Don't process concurent multiple esc sequences.
    // if we get a second, return the first and check if we
    // are getting a single second ESC or start of Sequence.
    if(character_buffer[0] == 27 && character_buffer.size() == 1)
    {
        if(!m_is_escape_sequence)
        {
            m_is_escape_sequence = true;
            m_escape_sequence.erase();
            m_string_buffer.erase();
            m_string_buffer = character_buffer;
            return "";
        }
        else
        {
            // If we get here again and second char is another ESC,
            // to single the first key press!
            m_escape_sequence.erase();
            m_string_buffer.erase();
            return "\x1b";
        }
    }

    // Reset, handle if we got a second Escape or two ESC keys in a row.
    m_is_escape_sequence = false;

    // Check if were appending to current buffer or returning
    // Only appending if were in an Active ESC Sequence!
    if(!m_string_buffer.empty())
    {
        // Check for Overflow, Input ESC Sequences Should
        // Be Short and Sweet, over 8 then lets kill it here!
        if(m_string_buffer.size() >= 8)
        {
            m_is_escape_sequence = false;
            m_escape_sequence.erase();
            m_string_buffer.erase();
            return "";
        }

        // Parse ESC Sequence for Match on bracket [ or O.
        // First Bracket overwrite the ESC in the buffer for easier matching.
        if((character_buffer[0] == '[' && m_string_buffer[0] == '\x1b') ||
                (character_buffer[0] == 0x4f && m_string_buffer[0] == '\x1b'))
        {
            // Overwrite ESC and continue Sequence
            // No Need to Test for Esc in the Sequence Buffer, We'll
            // Test for everything following ESC.
            m_string_buffer.erase();
            m_string_buffer = character_buffer;
            return "";
        }
        else
        {
            switch(character_buffer[0])
            {
                case '\n': // Handle Bad Sequences with ENTER following
                    m_escape_sequence.erase();
                    return "\n";

                case '[': // [[ Double Brackets F1 Keys.
                    if(m_string_buffer == "[")
                    {
                        m_string_buffer += character_buffer;
                        return "";
                    }
                    else if(m_string_buffer == "[O")
                    {
                        // case '[': // F12 = [O[
                        m_string_buffer += character_buffer;
                        m_escape_sequence = m_string_buffer;
                        m_string_buffer.erase();
                        return "\x1b";
                    }

                    // Bad Sequence, just return ESC as is.
                    m_escape_sequence = m_string_buffer;
                    m_string_buffer.erase();
                    return "\x1b";

                // Numbers all end in Tildes ~
                case '1': // Home
                case '2': // Insert
                case '3': // DEL
                case '4': // ESC
                case '5': // PG UP
                case '6': // PG DN
                case '7': // Function Keys.
                case '8': // Function Keys.
                case '9': // Function Keys.
                case '0': // Function Keys.
                case ';': // Seperator for Shift ARROW Keys! ie [1;2A
                    m_string_buffer += character_buffer;
                    return "";

                // Only SCO F3 ends with [[O, otherwise it
                // preceeds in other sequences.
                case 'O': // Precursor to Fucntion Keys [OA etc..
                    if(m_string_buffer == "[[")
                    {
                        // End of SCO Sequence
                        m_string_buffer += character_buffer;
                        m_escape_sequence = m_string_buffer;
                        m_string_buffer.erase();
                        return "\x1b";
                    }

                    // Continue;
                    m_string_buffer += character_buffer;
                    return "";

                case 'a': // shift up arrow
                case 'b': // shift dn arrow
                case 'c': // shift rt arrow
                case 'd': // shift lt arrow
                case 'e': // shift clear

                case 'A': // Up Arrow
                case 'B': // Dn Arrow
                case 'C': // Lt Arrow
                case 'D': // Rt Arrow
                case 'E': // F5
                case 'F': // End = 0F
                case 'G': // pg_down
                case 'K': // End
                case 'H': // Home
                case 'I': // page_up
                case 'M': // F1
                case 'N': // F2

                case 'P': // F1 / F4
                case 'Q': // F2 / F5
                case 'R': // F3 / F6
                case 'S': // F4 / F7
                case 'T': // F5 / F8
                case 'U': // PageDn / F9
                case 'V': // PageUP / F10
                case 'W': // F8 / F11
                case 'X': // F9 / F12
                case 'Y': // 10
                case 'Z': // 11
                    m_string_buffer += character_buffer;
                    m_escape_sequence = m_string_buffer;
                    m_string_buffer.erase();
                    return "\x1b";

                // End of Number Sequence.
                case '@': // Insert
                case '~': // Function
                case '$': // Shift Function RXVT
                case '^': // CTRL Function RXVT
                    m_string_buffer += character_buffer;
                    m_escape_sequence = m_string_buffer;
                    m_string_buffer.erase();
                    return "\x1b";

                case '\0':
                    //current_buffer += ch;
                    m_escape_sequence = m_string_buffer;
                    m_string_buffer.erase();
                    return "\x1b";

                default :
                    // Not ESC Sequence.
                    // Done with loop.
                    //m_string_buffer += ' ';
                    m_escape_sequence = m_string_buffer;
                    m_string_buffer.erase();
                    return "\x1b";
            }
        }
    }

    // If ESC key received wtih no trailing sequence,
    // clear sequence buffer and just return ESC.
    if(m_string_buffer.size() == 1 && m_string_buffer[0] == 27)
    {
        m_string_buffer.erase();
        return "\x1b";
    }

    // Translate ENTER Key, *NIX Terms send CR always test for LF.
    // CRLF is a newline on output only, input is either or.
    if(character_buffer[0] == '\r')
    {
        return "\n";
    }

    return character_buffer;
}

/**
* @brief Returns the InputFieldBuffer
* @return
*/
std::string CommonIO::getInputBuffer()
{
    return m_line_buffer;
}

/**
 * @brief Returns processing of field input until NL is received.
 * @param line
 * @param length
 * @param leadoff
 * @param hidden
 * @return
 */
std::string CommonIO::getLine(const std::string &line,    // Parsed Char input in
                              int   length,               // Max Input Length of Field
                              const std::string &leadoff, // Data to Display in Default Field {Optional}
                              bool  hidden)               // If input is hidden or masked     {Optional}
{
    Logging *log = Logging::instance();
    std::string output_buffer = "";    // Used for Data to send back
    std::string character_buffer = ""; // Used for Data being processed.

    // Flag on when to reset buffer and position for next run.
    if(m_is_new_getline)
    {
        m_is_new_getline = false;
        m_line_buffer.erase();
        m_column_position = 0;
    }

    // If were starting Off Input with a String already in buffer!  display it!
    if(m_is_new_leadoff && leadoff.size() > 0)
    {
        m_is_new_leadoff = false;
        m_column_position = leadoff.size();
        m_line_buffer += leadoff;

        // Trim if were past the length limit
        if((signed)numberOfChars(m_line_buffer) > length)
        {
            std::string temp = leftPadding(m_line_buffer, length);
            m_line_buffer = std::move(temp);
        }
    }

    // Gets Parsed input by Char, Multibyte or ESC Sequence.
    // Catch Aborts here!
    character_buffer = parseInput(line);

    if(character_buffer.size() == 0 || character_buffer[0] == '\0')
    {
        // No data or in mid ESC sequence
        // Need to wait for next character.
        return "\x1b"; // ""
    }

    // If we got an ENTER CR/LF then were done!
    // Set the Flag, so on next call to method, we reset, not before
    // Otherwise we'll clear the buffer we just filled.  :)
    if(character_buffer[0] == '\n')
    {
        m_is_new_getline = true;
        m_is_new_leadoff = true;
        return "\n";
    }

    // Escape in this case, ignore, later add movement in string
    std::string sequence = "";

    if(character_buffer[0] == 27)
    {
        sequence = getEscapeSequence();

        if(sequence.size() == 0)
        {
            return "\x1b";
        }
        else
        {
            // Received DEL Escape Sequence.
            if(sequence == "del")
            {
                if(m_line_buffer.size() > 0)
                {
                    log->xrmLog<Logging::DEBUG_LOG>("Received DEL ESC Sequence", __LINE__, __FILE__);
                    std::string temp = eraseString(m_line_buffer, numberOfChars(m_line_buffer)-1, 1);
                    m_line_buffer = std::move(temp);
                    m_column_position = m_line_buffer.size();
                    return "\x1b[D \x1b[D";
                }
                else
                {
                    // Nothing to delete at begining Skip.
                    log->xrmLog<Logging::DEBUG_LOG>("Received DEL ESC Sequence beginning of line=", character_buffer, __LINE__, __FILE__);
                    return "empty";
                }
            }
            else
            {
                // Unhandled sequence! Skip and return
                log->xrmLog<Logging::DEBUG_LOG>("Received Unhandled ESC Sequence beginning=", character_buffer, __LINE__, __FILE__);
                return "empty";
            }
        }
    }
    // CTRL Y - Clear Line
    else if((int)character_buffer[0] == 25)
    {
        if(m_line_buffer.size() > 0)
        {
            log->xrmLog<Logging::DEBUG_LOG>("Received CTRL+Y Sequence=", character_buffer, __LINE__, __FILE__);

            for(int i = numberOfChars(m_line_buffer); i > 0; i--)
            {
                output_buffer += "\x1b[D \x1b[D";
            }

            m_line_buffer.erase();
            m_column_position = 0;
            return output_buffer;
        }
        else
        {
            // At beginign of line, nothing to delete!
            log->xrmLog<Logging::DEBUG_LOG>("Received CTRL+Y Sequence beginning of line=", character_buffer, __LINE__, __FILE__);
            return "empty";
        }
    }
    // Handle BS and DEL as both Destructive Backspace on Fields
    // At this time, arrow keys are not setup to move through the string!
    else if((int)character_buffer[0] == 0x08 || (int)character_buffer[0] == 0x7f || character_buffer[0] == '\b')
    {
        if(m_line_buffer.size() > 0)
        {
            log->xrmLog<Logging::DEBUG_LOG>("Received backspace Sequence=", character_buffer, __LINE__, __FILE__);
            std::string temp = eraseString(m_line_buffer, numberOfChars(m_line_buffer)-1, 1);
            m_line_buffer = std::move(temp);
            m_column_position =  m_line_buffer.size();
            return "\x1b[D \x1b[D";
        }
        else
        {
            // At begining of Line, nothing to delete.
            log->xrmLog<Logging::DEBUG_LOG>("Received backspace Sequence begining of line=", character_buffer, __LINE__, __FILE__);
            return "empty";
        }
    }

    // Normal Input processing, ASCII and Unicode. Add new functions for size!
    if(((signed)m_line_buffer.size() <= length) &&
            ((signed)(character_buffer.size() + m_line_buffer.size()) <= length))
    {
        if(hidden)
        {
            log->xrmLog<Logging::DEBUG_LOG>("hidden field input=", character_buffer, __LINE__, __FILE__);
            m_line_buffer += character_buffer;
            m_column_position = numberOfChars(m_line_buffer);
            return "*";
        }
        else
        {
            log->xrmLog<Logging::DEBUG_LOG>("normal field input=", character_buffer, __LINE__, __FILE__);
            m_line_buffer += character_buffer;
            m_column_position = numberOfChars(m_line_buffer);
            return character_buffer;
        }
    }

    log->xrmLog<Logging::DEBUG_LOG>("Past the max length, nothing to add!", __LINE__, __FILE__);
    return "empty";
}

/**
 * @brief Converts Pascal Strings to C-Strings Also return std::string for conversions.
 * @param string
 */
std::string CommonIO::PascalToCString(int8_t *string)
{
    if(string[0] == 0)
    {
        return "";
    }

    std::string newstring = "";

    for(auto i = 1; i <= string[0]; i++)
    {
        newstring += string[i];
    }

    for(auto i = 0; i < (signed)newstring.size(); i++)
    {
        *string = (int)newstring.at(i);
        ++string;
    }

    *string = '\0';

    return newstring;
}

/**
 * @brief Converts Pascal Strings to C-Strings
 * @param string
 */
void CommonIO::CStringToPascal(int8_t *string)
{
    if(string[0] == '\0')
    {
        string[0] = 0;
        return;
    }

    std::string newstring = "";
    int length = std::strlen((char *) string);
    newstring[0] = length;

    // Pascal Strings can't be longer then 254 with first byte length.
    if(length >= 255) length = 254;

    for(auto i = 1; i <= length; i++)
    {
        newstring += string[i];
    }

    newstring += '\0';

    for(auto i = 0; i < (signed)newstring.size(); i++)
    {
        *string = (int)newstring.at(i);
        ++string;
    }
}

/**
 * @brief Helper Method to display bool as string.
 * @param value
 * @return
 */
std::string CommonIO::boolAlpha(bool value)
{
    if(value)
    {
        return "True";
    }

    return "False";
}

/**
 * @brief Parse / Replace MCI String from given string.
 * @param AnsiString
 * @param mcicode
 * @param replacement
 */
void CommonIO::parseLocalMCI(std::string &AnsiString, const std::string &mcicode, const std::string &replacement)
{
    std::string::size_type id1 = 0;

    do
    {
        // Parse New Message's MCI Code
        id1 = AnsiString.find(mcicode, 0);

        if(id1 != std::string::npos)
        {
            AnsiString.replace(id1, mcicode.size(), replacement);
            id1 = AnsiString.find(mcicode, 0);
        }
    }
    while(id1 != std::string::npos);
}

/**
 * @brief Check if the file exists
 * @return
 */
bool CommonIO::fileExists(std::string file_name)
{
    std::string path = GLOBAL_TEXTFILE_PATH;
    pathAppend(path);
    path += file_name;

    std::ifstream ifs(path);

    if(!ifs.is_open())
    {
        return false;
    }

    ifs.close();
    return true;
}

/**
 * Reads in ANSI file into Buffer Only
 */
std::string CommonIO::readinAnsi(std::string file_name)
{
    std::string path = GLOBAL_TEXTFILE_PATH;
    pathAppend(path);
    path += file_name;

    Logging *log = Logging::instance();
    log->xrmLog<Logging::DEBUG_LOG>("readinAnsi=", path);

    std::string buff;
    FILE *fp;
    int c = 0;

    if((fp = fopen(path.c_str(), "r+")) ==  NULL)
    {
        return "";
    }

    do
    {
        c = getc(fp);

        if(c != EOF)
        {
            buff += c;
        }
    }
    while(c != EOF);

    fclose(fp);

    // Normalize Line Ending for consistent display to users.
    std::regex exp("(\\r\\n|\\r|\\n)+");
    std::string fixed_newlines = std::regex_replace(buff, exp, "\r\n");
    return Encoding::instance()->utf8Encode(fixed_newlines);
}

/**
* @brief Split Strings by delimiter into Vector of Strings.
* @param s
* @param delimiter
* @return
*/
std::vector<std::string> CommonIO::splitString(const std::string& s, char delimiter)
{
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);

    while(std::getline(tokenStream, token, delimiter))
    {
        tokens.push_back(token);
    }

    return tokens;
}

/**
* @brief Standard Time to Date String
* @param std_time
* @return
*/
std::string CommonIO::standardDateToString(std::time_t std_time)
{
    std::ostringstream oss;
    oss << std::put_time(std::localtime(&std_time), "%Y-%m-%d");
    std::string time_string = oss.str();
    return time_string;
}

/**
* @brief Standard Time to Date/Time String
* @param std_time
* @return
*/
std::string CommonIO::standardDateTimeToString(std::time_t std_time)
{
    std::ostringstream oss;
    oss << std::put_time(std::localtime(&std_time), "%Y-%m-%d %H:%M:%S %z");
    std::string datetime_string = oss.str();
    return datetime_string;
}

/**
* @brief String to Date Format
* @param date
* @return
*/
std::time_t CommonIO::stringToStandardDate(std::string date)
{
    // Append Time For Dates, need formattings
    std::string key = date;
    key += " 00:00:00";
    struct std::tm tm;

    std::istringstream ss(key);
    ss >> std::get_time(&tm, "%Y-%m-%d %H:%M:%S");

    if(ss.fail())
    {
        ss.clear();
        return -1;
    }

    std::time_t const time = mktime(&tm);
    return time;
}

/**
* @brief String to Date/Time Format
* @param date_time
* @return
*/
std::time_t CommonIO::stringToStandardDateTime(std::string date_time)
{
    struct std::tm tm;
    std::istringstream ss(date_time);
    ss >> std::get_time(&tm, "%Y-%m-%d %H:%M:%S");

    if(ss.fail())
    {
        ss.clear();
        return -1;
    }

    std::time_t const time = mktime(&tm);
    return time;
}

/**
* @brief Converts std::strings to Long values
* @param value
* @return
*/
long CommonIO::stringToLong(std::string value)
{
    long result_id = -1;
    std::stringstream ss(value);
    ss >> result_id;

    // check for Invalid Index.
    if(ss.fail() || result_id < 0)
    {
        ss.clear();
        ss.ignore();
        result_id = -1;
    }

    return result_id;
}

/**
* @brief Converts std::strings to Int values
* @param value
* @return
*/
int CommonIO::stringToInt(std::string value)
{
    int result_id = -1;
    std::stringstream ss(value);
    ss >> result_id;

    // check for Invalid Index.
    if(ss.fail() || result_id < 0)
    {
        ss.clear();
        ss.ignore();
        result_id = -1;
    }

    return result_id;
}

/**
* @brief Tests first char of string for starting T/F returns int with -1 for invalid
* @param value
* @return
*/
int CommonIO::stringToBool(std::string value)
{
    // Test if string starts with T or F instead of typing True/False
    if(toupper(value[0]) == 'T')
        return 1;
    else if(toupper(value[0]) == 'F')
        return 0;
    else
        return -1;
}

/**
* @brief Parses screen data into the Screen Buffer.
* @return
*/
void CommonIO::getNextGlyph(LocalizedBuffer &buffer, std::string::iterator &it,
                            std::string::iterator &line_end)
{
    buffer.clear();

    if(it == line_end)
    {
        return;
    }

    int byte_value = static_cast<int>((uint8_t)*it);

    if(byte_value < 128)
    {
        buffer.character = std::string(1, *it);
        buffer.length = 1;
        *it++;
    }
    else
    {
        try
        {
            uint32_t code_point = utf8::next(it, line_end);
            unsigned char character[5] = {0, 0, 0, 0, 0};
            utf8::append(code_point, character);

            for(int i = 0; i < 5; i++)
            {
                if(character[i] != 0)
                {
                    buffer.character += std::string(1, character[i]);
                }
            }

            buffer.length = buffer.character.size();
        }
        catch(utf8::exception &ex)
        {
            Logging *log = Logging::instance();
            log->xrmLog<Logging::ERROR_LOG>("[getNextGlyph] UTF8 Parsing Exception=", ex.what(), __LINE__, __FILE__);
            ++*it; // Bad, other iterate past it, otherwise stuck in endless loop.
        }
    }
}

/**
* @brief Parses screen data into the Screen Buffer.
* @return
*/
void CommonIO::peekNextGlyph(LocalizedBuffer &buffer, std::string::iterator &it,
                             std::string::iterator &line_end)
{
    buffer.clear();

    if(it == line_end)
    {
        return;
    }

    int byte_value = static_cast<int>((uint8_t)*it);

    if(byte_value < 128)
    {
        buffer.character = std::string(1, *it);
        buffer.length = 1;
    }
    else
    {
        try
        {
            uint32_t code_point = utf8::next(it, line_end);
            unsigned char character[5] = {0, 0, 0, 0, 0};
            utf8::append(code_point, character);

            for(int i = 0; i < 5; i++)
            {
                if(character[i] != 0)
                {
                    buffer.character += std::string(1, character[i]);
                }
            }

            buffer.length = buffer.character.size();
            *it--;
        }
        catch(utf8::exception &ex)
        {
            Logging *log = Logging::instance();
            log->xrmLog<Logging::ERROR_LOG>("[peekNextGlyph] UTF8 Parsing Exception=", ex.what(), __LINE__, __FILE__);
            ++*it; // Bad, iterate past otherwise stuck in endless loop!
        }
    }
}
