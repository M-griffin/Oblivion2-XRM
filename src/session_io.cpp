#include "session_io.hpp"
#include "session.hpp"
#include "common_io.hpp"
#include "encoding.hpp"
#include "logging.hpp"

#include "model-sys/config.hpp"
#include "utf-cpp/utf8.h"

#include <clocale>
#include <regex>
#include <string>
#include <iostream>
#include <sstream>


SessionIO::SessionIO()
    : m_log(Logging::getInstance())
    , m_session_data(nullptr)
{
}

SessionIO::SessionIO(session_ptr session_data)
    : m_log(Logging::getInstance())
    , m_session_data(session_data)
{
}

SessionIO::~SessionIO()
{
    std::cout << "~SessionIO()" << std::endl;
    clearAllMCIMapping();
}

/**
 * @brief Single Key Input For Full Screen Editor or Esc Sequences
 * @param character_buffer
 * @return
 */
std::string SessionIO::getFSEKeyInput(const std::string &character_buffer)
{    
    std::string input = m_common_io.parseInput(character_buffer);

    if(input.size() == 0)
    {
        // No Data received, could be in mid ESC sequence
        // Return for next key.
        m_log.write<Logging::DEBUG_LOG>("getKeyInput Mid Escape");
        return "";
    }

    std::string escape_sequence = "";

    if(input[0] == '\x1b')
    {
        escape_sequence = m_common_io.getFSEEscapeSequence();

        std::cout << "FSE escape_sequence: " << escape_sequence << std::endl;

        if(escape_sequence.size() == 0)
        {
            m_log.write<Logging::DEBUG_LOG>("getKeyInput Single Escape");
            return "\x1b";
        }
        else
        {
            m_log.write<Logging::DEBUG_LOG>("getKeyInput Translated Escape Sequence=", escape_sequence);
            return (escape_sequence.insert(0, "\x1b"));
        }
    }

    m_log.write<Logging::DEBUG_LOG>("getKeyInput Normal Input=", input);
    return input;
}

/**
 * @brief Single Key Input or Esc Sequence
 * @param character_buffer
 * @return
 */
std::string SessionIO::getKeyInput(const std::string &character_buffer)
{
    std::string input = m_common_io.parseInput(character_buffer);

    if(input.size() == 0)
    {
        // No Data received, could be in mid ESC sequence
        // Return for next key.
        m_log.write<Logging::DEBUG_LOG>("getKeyInput Mid Escape");
        return "";
    }

    std::string escape_sequence = "";

    if(input[0] == '\x1b')
    {
        escape_sequence = m_common_io.getEscapeSequence();

        if(escape_sequence.size() == 0)
        {
            m_log.write<Logging::DEBUG_LOG>("getKeyInput Single Escape");
            return "\x1b";
        }
        else
        {
            m_log.write<Logging::DEBUG_LOG>("getKeyInput Translated Escape Sequence=", escape_sequence);
            return (escape_sequence.insert(0, "\x1b"));
        }
    }

    m_log.write<Logging::DEBUG_LOG>("getKeyInput Normal Input=", input);
    return input;
}

/**
 * @brief Generates an input field with ANSI color background
 * @param field_name {Is the Field label, precedes the field like 'Mail From: '}
 * @param len
 */
void SessionIO::createInputField(std::string &field_name, int &len)
{
    std::string repeat;
    char formatted[1024]= {0};
    char sTmp[3]  = {0};
    char sTmp2[3] = {0};

    // Parse for Input String Modifiers
    std::string::size_type tempLength = 0;
    std::string::size_type stringSize = 0;
    std::string::size_type position   = 0;


    char INPUT_COLOR[255]= {0};
    bool isColorOverRide = false; //found input color

    stringSize = field_name.size()-1;

    if(len == 0)
    {
        return;
    }

    // Format Input Field, if color is enabled, otherwise just add Field Name like "Login: "
    if(!m_session_data->m_is_use_ansi)
    {
        sprintf(formatted, "%s", (char *)field_name.c_str()); // Field Name
        field_name = formatted;
        return;
    }

    // Override Field Input Length for Input Field.
    position = field_name.find("|FL", 0);

    if(position != std::string::npos)
    {
        // Make sure we don't go past the bounds
        if(position+4 <= stringSize)
        {
            // (Unit Test Notes)
            // Need to Test if idDigit!  And only if, both are
            // Then we cut these out and erase!,  Otherwise
            // We only remove the |IN pipe sequence.
            if(isdigit(field_name[position+3]) && isdigit(field_name[position+4]))
            {
                sTmp[0] = field_name[position+3];
                sTmp[1] = field_name[position+4];
                field_name.erase(position, 5);
                tempLength = atoi(sTmp);

                if((signed)tempLength > 0 && (signed)tempLength <= len)
                {
                    len = tempLength; // Set new Length
                }
                else
                {
                    m_log.write<Logging::ERROR_LOG>("createInputField() Incorrect |FL field length=", tempLength, "cannot exceed max size=", len);
                }
            }
            else
            {
                field_name.erase(position, 3);
            }
        }
    }

    // Override Foreground/Background Input Field Colors
    // This is now for OBV/2 .. Not in Legacy.
    position = field_name.find("|FB",0);
    m_log.write<Logging::DEBUG_LOG>("createInputField() |FB position=", position, "compare=", position+4, stringSize);

    if(position != std::string::npos)
    {
        // (Unit Test Notes)
        // Need to Test if isDigit!  And only if, both are
        // Then we cut these out and erase!,  Otherwise
        // We only remove the |FB pipe sequence.

        memset(&sTmp, 0, 3);
        memset(&sTmp2, 0, 3);

        // Make sure we don't go past the bounds
        if(position+6 <= stringSize)
        {
            if(isdigit(field_name[position+3]) && isdigit(field_name[position+4]) &&
                    isdigit(field_name[position+5]) && isdigit(field_name[position+6]))
            {
                sTmp[0]  = field_name[position+3]; // Foreground 00-15
                sTmp[1]  = field_name[position+4];
                sTmp2[0] = field_name[position+5]; // Background 16-23
                sTmp2[1] = field_name[position+6];
                field_name.erase(position, 7);
                sprintf(INPUT_COLOR, "|%s|%s", sTmp, sTmp2);
                isColorOverRide = true;
            }
            else
            {
                field_name.erase(position, 3);
            }
        }
    }

    // Pad len amount of spaces.
    if(len > 0)
    {
        repeat.insert(0, len, ' ');
    }

    // Set Default Input Color if none was passed.
    if(!isColorOverRide)
    {
        // Make this a theme color to set a variable...
        sprintf(INPUT_COLOR,"|00|19"); // make theme
    }

    // Format the input field
    sprintf(formatted, "%s%s%s\x1b[%iD",
            (char *)field_name.c_str(), // Field Name
            INPUT_COLOR,          // Field Fg,Bg Color
            repeat.c_str(),       // Padding length of Field
            len+1);               // Move back to starting position of field.

    field_name = formatted;
}

/**
 * @brief Input Field, Returns output for keypresses, field stored in result on ENTER.
 * @param character_buffer
 * @param result
 * @param length
 * @param leadoff
 * @param hidden
 * @return
 */
std::string SessionIO::getInputField(const std::string &character_buffer,
                                     std::string &result,
                                     int length,
                                     std::string leadoff,
                                     bool hidden)
{
    // Setup the leadoff, if it's first time, then print it out
    // Other if empty or follow-up calls to inputfield field skip it!
    static bool is_leadoff = true;

    if(leadoff.size() == 0)
    {
        is_leadoff = false;
    }

    // Display Leadoff of field.. ie.. 'Mail to: Mercyful Name'
    // put the name in the field.
    if(is_leadoff)
    {
        m_session_data->deliver(leadoff);
        is_leadoff = false;
    }

    std::string string_data = m_common_io.getLine(character_buffer, length, leadoff, hidden);

    if((signed)string_data.size() > 0)
    {
        // Check for ESC for Abort!
        if(string_data[0] == 27 && string_data.size() == 1)
        {
            std::string esc_sequence = m_common_io.getEscapeSequence();

            if(esc_sequence.size() == 0 && character_buffer[0] == '\0')
            {
                is_leadoff = true;    // Reset for next run
                esc_sequence.erase();
                string_data.erase();
                return "aborted";
            }
        }
        // Check for Completed Field Entry
        else if((string_data[0] == '\n' && string_data.size() == 1) || character_buffer[0] == '\n')
        {
            result = m_common_io.getInputBuffer();
            string_data.erase();
            is_leadoff = true;    // Reset for next run
            return "\n";
        }
        // Updates on Keypresses.
        else
        {
            m_log.write<Logging::DEBUG_LOG>("getInputField() result=", result, "string_data=", string_data);
            return string_data;
        }
    }

    m_log.write<Logging::DEBUG_LOG>("getInputField() result empty");
    return "";
}


/**
 * @brief Parses for ANSI Foreground Colors (Helper for pipe2ansi)
 * @param data
 * @param foreground
 */
std::string SessionIO::pipeReplaceForeground(int foreground)
{
    std::string escape_sequence = "";

    switch(foreground)
    {
        case 0:
            escape_sequence = "\x1b[0;30m";
            break;

        case 1:
            escape_sequence = "\x1b[0;34m";
            break;

        case 2:
            escape_sequence = "\x1b[0;32m";
            break;

        case 3:
            escape_sequence = "\x1b[0;36m";
            break;

        case 4:
            escape_sequence = "\x1b[0;31m";
            break;

        case 5:
            escape_sequence = "\x1b[0;35m";
            break;

        case 6:
            escape_sequence = "\x1b[0;33m";
            break;

        case 7:
            escape_sequence = "\x1b[0;37m";
            break;

        case 8:
            escape_sequence = "\x1b[1;30m";
            break;

        case 9:
            escape_sequence = "\x1b[1;34m";
            break;

        case 10:
            escape_sequence = "\x1b[1;32m";
            break;

        case 11:
            escape_sequence = "\x1b[1;36m";
            break;

        case 12:
            escape_sequence = "\x1b[1;31m";
            break;

        case 13:
            escape_sequence = "\x1b[1;35m";
            break;

        case 14:
            escape_sequence = "\x1b[1;33m";
            break;

        case 15:
            escape_sequence = "\x1b[1;37m";
            break;

        default :
            break;
    }

    return escape_sequence;
}

/**
 * @brief Parses for ANSI Background Colors (Helper for pipe2ansi)
 * @param data
 * @param background
 */
std::string SessionIO::pipeReplaceBackground(int background)
{
    std::string escape_sequence = "";

    switch(background)
    {
        case 16:
            escape_sequence = "\x1b[40m";
            break;

        case 17:
            escape_sequence = "\x1b[44m";
            break;

        case 18:
            escape_sequence = "\x1b[42m";
            break;

        case 19:
            escape_sequence = "\x1b[46m";
            break;

        case 20:
            escape_sequence = "\x1b[41m";
            break;

        case 21:
            escape_sequence = "\x1b[45m";
            break;

        case 22:
            escape_sequence = "\x1b[43m";
            break;

        case 23:
            escape_sequence = "\x1b[47m";
            break;

        // Default to none.
        case 24:
            escape_sequence = "\x1b[0m";
            break;

        default :
            break;
    }

    return escape_sequence;
}

/**
 * @brief Parsed Pipe Colors Codes to ANSI ESC Sequence.
 * @param color_string
 * @return
 */
std::string SessionIO::pipeColors(const std::string &color_string)
{
    // Skip PIPE and grab next two digits.
    std::string str = color_string.substr(1);
    std::string esc_sequence = "";

    // String to Int
    std::istringstream ss(str);
    int color_index = 0;
    ss >> color_index;

    if(ss.fail())
    {
        return "";
    }

    // Foreground Colors
    if(color_index >= 0 && color_index < 16)
    {
        esc_sequence = pipeReplaceForeground(color_index);
        return esc_sequence;
    }
    // Background Colors
    else if(color_index >= 16 && color_index < 24)
    {
        esc_sequence = pipeReplaceBackground(color_index);
        return esc_sequence;
    }

    return esc_sequence;
}


/**
 * @brief Gets the Default Color Sequence
 * @return
 */
std::string SessionIO::getDefaultColor(config_ptr config)
{
    return pipeColors(config->default_color_regular);
}

/**
 * @brief Gets the Default Input Color Sequence
 * @return
 */
std::string SessionIO::getDefaultInputColor(config_ptr config)
{
    return pipeColors(config->default_color_input);
}

/**
 * @brief Gets the Default Inverse Color Sequence
 * @return
 */
std::string SessionIO::getDefaultInverseColor(config_ptr config)
{
    return pipeColors(config->default_color_inverse);
}

/**
 * @brief Gets the Default Prompt Color Sequence
 * @return
 */
std::string SessionIO::getDefaultPromptColor(config_ptr config)
{
    return pipeColors(config->default_color_prompt);
}

/**
 * @brief Gets the Default stat Color Sequence
 * @return
 */
std::string SessionIO::getDefaultStatColor(config_ptr config)
{
    return pipeColors(config->default_color_stat);
}

/**
 * @brief Gets the Default box Color Sequence
 * @return
 */
std::string SessionIO::getDefaultBoxColor(config_ptr config)
{
    return pipeColors(config->default_color_box);
}

/**
 * @brief Parsed Pipe Codes with 1 or 2 Digits (Handle OBV/2 Legacy Movements)
 * @param pipe_code
 * @return
 */
std::string SessionIO::parsePipeWithCharsDigits(const std::string &code, int value)
{
    std::string sequence = "";

    // Check Single letter Sequences
    if(code.size() == 1)
    {
        switch(code[0])
        {
            case 'U': // Up
                if(value == 0 || value == 1)
                    sequence = "\x1b[A";
                else
                    sequence = "\x1b[" + std::to_string(value) + "A";

                break;

            case 'D': // Down
                if(value == 0 || value == 1)
                    sequence = "\x1b[B";
                else
                    sequence = "\x1b[" + std::to_string(value) + "B";

                break;

            case 'F': // Forward
                if(value == 0 || value == 1)
                    sequence = "\x1b[C";
                else
                    sequence = "\x1b[" + std::to_string(value) + "C";

                break;

            case 'B': // Backwards
                if(value == 0 || value == 1)
                    sequence = "\x1b[D";
                else
                    sequence = "\x1b[" + std::to_string(value) + "D";

                break;

            case 'X': // Absolute X Position
                if(value == 0 || value == 1)
                    sequence = "\x1b[G";
                else
                    sequence = "\x1b[" + std::to_string(value) + "G";

                break;

            default :
                break;
        }
    }
    // Double Letter Sequences
    else
    {
        // Updates, is this still needed? test lateron.
    }

    return sequence;
}

/**
 * @brief Separate Pipe Codes with 1 or 2 Digits
 * @param pipe_code
 * @return
 */
std::string SessionIO::separatePipeWithCharsDigits(const std::string &pipe_code)
{
    // Strip PIPE and grab the Sequences
    std::string str = pipe_code.substr(1);
    int alpha_chars  = 0;

    // Figure out if were dealing with 1 or 2 Digit Character Codes.
    if(isdigit(str[1]))   // |Xxx ?
    {
        alpha_chars = 1;
    }
    // We have 2 Characters, now test for 1 or 2 digits.
    else // |XXxx ?
    {
        alpha_chars = 2;
    }

    // Split Alpha from Numeric so we can process it!
    std::string str_alpha = str.substr(0, alpha_chars);
    str.erase(0, alpha_chars);

    // String to Int for numeric remainder for switch/case.
    // Also removes leading 0 if not needed!
    std::istringstream ss(str);
    int pipe_index = 0;
    ss >> pipe_index;

    if(ss.fail())
    {
        return "";
    }

    // Parse and return result.
    return (parsePipeWithCharsDigits(str_alpha, pipe_index));
}


/**
 * @brief Parse Files %%Filename.ext
 * @param pipe_code
 * @return
 */
std::string SessionIO::parseFilename(const std::string &pipe_code)
{
    // Strip %%DF and grab the 'Filename.ext
    CommonIO common_io;
    std::string str = pipe_code.substr(4);
    std::string buffer = common_io.readinAnsi(str);

    if(buffer.size() > 0)
    {
        return pipe2ansi(buffer);
    }

    return buffer;
}


/**
 * @brief Parse Pipe Codes with no Following Digits
 * @param pipe_code
 * @return
 */
std::string SessionIO::parsePipeWithChars(const std::string &pipe_code)
{
    // Strip PIPE and grab the Sequences
    std::string esc_sequence = "";

    // Make this more dynamic?!?

    switch(pipe_code[1])
    {
        // Most likely will break these out later bye pipe[1] and send [2] to other functions
        // To Keep small and compact!
        case 'C':
            switch(pipe_code[2])
            {
                case 'S': // Clear Screen / Home Cursor
                    esc_sequence = "\x1b[2J\x1b[1;1H";
                    break;

                case 'R': // Carriage Return / NewLine
                    esc_sequence = "\r\n";
                    break;

                default:
                    break;
            }

            break;

        case 'D':
            switch(pipe_code[2])
            {
                case 'S': // Clear Screen / Home Cursor
                    esc_sequence = "\x1b[2J\x1b[1;1H";
                    break;

                case 'R': // Carriage Return / NewLine
                    esc_sequence = "\r\n";
                    break;

                default:
                    break;
            }

            break;

        default:
            break;
    }

    return esc_sequence;
}


/**
 * @brief Parses Code Map and replaces screen codes with ANSI Sequences.
 * @param screen
 * @param code_map
 * @return
 */
std::string SessionIO::parseCodeMap(const std::string &screen, std::vector<MapType> &code_map)
{
    m_log.write<Logging::DEBUG_LOG>("[parseCodeMap]", __LINE__, __FILE__);

    std::string ansi_string = screen;
    MapType my_matches;

    // All Global MCI Codes likes standard screens and colors will
    // He handled here, then specific interfaces will break out below this.
    // Break out parsing on which pattern was matched.
    while(code_map.size() > 0)
    {
        // Loop Backwards to preserve string offsets on replacement.
        my_matches = code_map.back();
        code_map.pop_back();

        // Check for Custom Screen Translation Mappings
        // If these exist, they take presidency over standard codes
        if(m_mapped_codes.size() > 0)
        {
            std::map<std::string, std::string>::iterator it;
            it = m_mapped_codes.find(my_matches.m_code);

            if(it != m_mapped_codes.end())
            {
                // If found, replace mci sequence with text
                ansi_string.replace(my_matches.m_offset, my_matches.m_length, it->second);
                continue;
            }
        }

        // Handle parsing on expression match.
        switch(my_matches.m_match)
        {
            case 1: // Pipe w/ 2 DIGIT Colors
            {
                m_log.write<Logging::DEBUG_LOG>("Pipe w/ 2 DIGIT Colors |00");
                std::string result = pipeColors(my_matches.m_code);

                if(result.size() != 0)
                {
                    // Replace the Color, if not ansi then remove the color!
                    if(m_session_data->m_is_use_ansi)
                    {
                        ansi_string.replace(my_matches.m_offset, my_matches.m_length, result);
                    }
                    else
                    {
                        ansi_string.replace(my_matches.m_offset, my_matches.m_length, "");
                    }
                }
                else
                {
                    ansi_string.replace(my_matches.m_offset, my_matches.m_length, "   ");
                }
            }
            break;

            case 2: // Pipe w/ 2 Chars and 4 Digits // |XY0101
            {
                m_log.write<Logging::DEBUG_LOG>("Pipe w/ 2 Chars and 4 Digits // |XY0101");
                // Remove for now, haven't gotten this far!
                ansi_string.replace(my_matches.m_offset, my_matches.m_length, "       ");
            }
            break;

            case 3: // Pipe w/ 1 or 2 CHARS followed by 1 or 2 DIGITS
            {
                m_log.write<Logging::DEBUG_LOG>("Pipe w/ 1 or 2 CHARS followed by 1 or 2 DIGITS // |A1 A22  AA2  AA33");
                std::string result = separatePipeWithCharsDigits(my_matches.m_code);

                if(result.size() != 0)
                {
                    // Replace the string
                    ansi_string.replace(my_matches.m_offset, my_matches.m_length, result);
                }
            }
            break;

            case 4: // Pipe w/ 2 CHARS
                // This one will need replacement in the string parsing
                // Pass the original string because of |DE for delay!
            {
                m_log.write<Logging::DEBUG_LOG>("Pipe w/ 2 CHARS // |AA");
                std::string result = parsePipeWithChars(my_matches.m_code);

                if(result.size() != 0)
                {
                    // Replace the string
                    ansi_string.replace(my_matches.m_offset, my_matches.m_length, result);
                }
                else
                {
                    ansi_string.replace(my_matches.m_offset, my_matches.m_length, "   ");
                }
            }
            break;

            case 5: // %%FILENAME.EXT  get filenames for loading from string prompts
            {
                m_log.write<Logging::DEBUG_LOG>("Replacing %%FILENAME.EXT codes");
                std::string result = parseFilename(my_matches.m_code);

                if(result.size() != 0)
                {
                    ansi_string.replace(my_matches.m_offset, my_matches.m_length, result);
                }
                else
                {
                    std::string s(my_matches.m_length, ' ');
                    ansi_string.replace(my_matches.m_offset, my_matches.m_length, s);
                }
            }
            break;

            case 6: // Percent w/ 2 CHARS
            {
                m_log.write<Logging::DEBUG_LOG>("Percent w/ 2 CHARS");
                // Remove for now, haven't gotten this far!
                ansi_string.replace(my_matches.m_offset, my_matches.m_length, "   ");
            }
            break;

            case 7: // Percent with 2 digits, custom codes
            {
                // Were just removing them because they are processed.
                // Now that first part of sequence |01 etc.. are processed!
                m_log.write<Logging::DEBUG_LOG>("replacing %## codes");
                // Remove for now, haven't gotten this far!
                ansi_string.replace(my_matches.m_offset, my_matches.m_length, "   ");
            }
            break;

            default:
                break;
        }
    }

    // Clear Code map.
    std::vector<MapType>().swap(code_map);

    // Clear Custom MCI Screen Translation Mappings
    clearAllMCIMapping();
    return ansi_string;
}

/**
 * @brief Parses Code Map and replaces screen codes with Generic Items.
 * @param screen
 * @param code_map
 * @return
 */
std::string SessionIO::parseCodeMapGenerics(const std::string &screen, const std::vector<MapType> &code_map)
{
    m_log.write<Logging::DEBUG_LOG>("[parseCodeMapGenerics]", __LINE__, __FILE__);

    std::string ansi_string = screen;
    MapType my_matches;

    // Make a copy so the original is not modified.
    std::vector<MapType> code_mapping;
    code_mapping.assign(code_map.begin(), code_map.end());

    // All Global MCI Codes likes standard screens and colors will
    // He handled here, then specific interfaces will break out below this.
    // Break out parsing on which pattern was matched.
    while(code_mapping.size() > 0)
    {
        // Loop Backwards to preserve string offsets on replacement.
        my_matches = code_mapping.back();
        code_mapping.pop_back();

        // Check for Custom Screen Translation Mappings
        // If these exist, they take presidency over standard codes
        if(m_mapped_codes.size() > 0)
        {
            std::map<std::string, std::string>::iterator it;
            it = m_mapped_codes.find(my_matches.m_code);

            if(it != m_mapped_codes.end())
            {
                m_log.write<Logging::DEBUG_LOG>("[parseCodeMapGenerics] gen found=", my_matches.m_code, it->second, __LINE__, __FILE__);
                // If found, replace mci sequence with text
                ansi_string.replace(my_matches.m_offset, my_matches.m_length, it->second);
            }
            else
            {
                m_log.write<Logging::DEBUG_LOG>("[parseCodeMapGenerics] gen not found=", __LINE__, __FILE__);
                std::string remove_code = "";
                ansi_string.replace(my_matches.m_offset, my_matches.m_length, remove_code);
            }
        }
    }

    // Clear MCI And Code Mappings
    clearAllMCIMapping();
    std::vector<MapType>().swap(code_mapping);
    return ansi_string;
}

/**
 * @brief Parses string and returns code mapping and positions per expression
 * @param sequence
 * @return
 */
std::vector<MapType> SessionIO::parseToCodeMap(const std::string &sequence, const std::string &expression)
{
    // Contains all matches found so we can iterate and replace
    // Without Multiple loops through the string.
    MapType my_matches;
    std::vector<MapType> code_map;

    // Make a copy that we can modify and process on.
    std::string ansi_string = sequence;

    // MCI Code Groups 1 - 7
    /*
        ([|]{1}[0-9]{2})                // |00
        ([|]{1}[X][Y][0-9]{4}           // |XY0101
        ([|]{1}[A-Z]{1,2}[0-9]{1,2})    // |A1 A22  AA2  AA33
        ([|]{1}[A-Z]{2})                // |AA
        ([%]{2}[\w]+[.]{1}[\w]{3})      // %%filename.ans
        ([%]{1}[A-Z]{2})                // %AA
        ([%]{1}[0-9]{2})                // %11
    */

    //std::cout << "exp: " << expression << std::endl;
    try
    {
        std::regex expr(expression);
        std::smatch matches;
        std::string::const_iterator start = ansi_string.begin(), end = ansi_string.end();
        //    std::string::size_type offset = 0;
        //    std::string::size_type length = 0;

        std::regex_constants::match_flag_type flags = std::regex_constants::match_default;

        while(std::regex_search(start, end, matches, expr, flags))
        {
            // Found a match!
            /*
            std::cout << "Matched Sub '" << matches.str()
            		  << "' following ' " << matches.prefix().str()
            		  << "' preceding ' " << matches.suffix().str()
            		  << std::endl;*/

            // Avoid Infinite loop and make sure the existing
            // is not the same as the next!
            if(start == matches[0].second)
            {
                m_log.write<Logging::DEBUG_LOG>("[parseToCodeMap] no Code Maps Found", __LINE__, __FILE__);
                break;
            }

            // Since were replacing on the fly, we need to re-scan the
            // string for next code
            start = matches[0].second;

            // Loop each match, and grab the starting position and length to replace.
            for(size_t s = 1; s < matches.size(); ++s)
            {
                // Make sure the Match is true! otherwise skip.
                if(matches[s].matched)
                {
                    //offset = matches[s].first - ansi_string.begin();
                    //length = matches[s].length();

                    // Test output s registers which pattern matched, 1, 2, or 3!
                    /*
                    std::cout << s << " :  Matched Sub 2" << matches[s].str()
                    		  << " at offset " << offset
                    		  << " of length " << length
                    		  << std::endl;*/

                    // Add to Vector so we store each match.
                    my_matches.m_offset = matches[s].first - ansi_string.begin();
                    my_matches.m_length = matches[s].length();
                    my_matches.m_match  = s;
                    my_matches.m_code   = matches[s].str();

                    // TODO, check, might not be needed here, might be pre-parsing!!
                    // UTF-8. meed to use a utf8-distance to get actual char off-set to match
                    // screen buffer now vs. raw byte off-set.
                    /*
                    int match_off_set = my_matches.m_offset;
                    auto new_it = ansi_string.begin() + match_off_set;
                    int char_length = utf8::distance(ansi_string.begin(), new_it);

                    my_matches.m_offset = char_length;
                    */

                    code_map.push_back(std::move(my_matches));
                }
            }
        }
    }
    catch(std::regex_error &ex)
    {
        m_log.write<Logging::ERROR_LOG>("[parseToCodeMap] Exception=", ex.what(), ex.code(), __LINE__, __FILE__);
    }

    return code_map;
}

/**
 * @brief Converts MCI Sequences to Ansi screen output.
 * @param sequence
 * @return
 */
std::string SessionIO::pipe2ansi(const std::string &sequence)
{
    std::vector<MapType> code_map = parseToCodeMap(sequence, STD_EXPRESSION);
    return parseCodeMap(sequence, code_map);
}

/**
 * @brief Converts MCI Sequences to Code Maps for Multiple Parses of same string data
 * @param sequence
 * @return
 */
std::vector<MapType> SessionIO::pipe2genericCodeMap(const std::string &sequence)
{
    std::vector<MapType> code_map = parseToCodeMap(sequence, MID_EXPRESSION);
    return code_map;
}

/**
 * @brief Converts MCI Sequences to Code Maps for Prompt Strings
 * @param sequence
 * @return
 */
std::vector<MapType> SessionIO::pipe2promptCodeMap(const std::string &sequence)
{
    // This will handle parsing the sequence, and replacement
    std::vector<MapType> code_map = parseToCodeMap(sequence, PROMPT_EXPRESSION);
    return code_map;
}

/**
 * @brief Converts MCI Sequences to Code Maps for Prompt Formatting Strings
 * @param sequence
 * @return
 */
std::vector<MapType> SessionIO::pipe2promptFormatCodeMap(const std::string &sequence)
{
    // This will handle parsing the sequence, and replacement
    std::vector<MapType> code_map = parseToCodeMap(sequence, FORMAT_EXPRESSION);
    return code_map;
}

/**
 * @brief Colorizes Brackets and Text between brackets to system theme colors
 * @param sequence
 * @return
 */
std::string SessionIO::parseFormatColorsBrackets(const std::string &sequence, config_ptr config)
{
    // Mocked up for now
    std::string output = sequence;
    output += getDefaultPromptColor(config);
    return output;
}

/**
 * @brief Colorizes Colons to system theme colors
 * @param sequence
 * @return
 */
std::string SessionIO::parseFormatColorsColon(const std::string &sequence, config_ptr config)
{
    // Mocked up for now
    std::string output = sequence;
    output += getDefaultPromptColor(config);
    return output;
}

/**
 * @brief Parses unformatted prompt text and adds colors to brackets and colon's.
 * @param sequence
 * @return
 */
std::string SessionIO::pipe2promptFormat(const std::string &sequence, config_ptr config)
{
    std::vector<MapType> code_map = pipe2promptFormatCodeMap(sequence);
    std::string output = "";
    std::string key;
    std::string value;

    // Loop codes and build MCI Parsing List
    for(unsigned int i = 0; i < code_map.size(); i++)
    {
        auto &map = code_map[i];
        m_log.write<Logging::DEBUG_LOG>("[pipe2promptFormat] Menu Format Code=", map.m_code, __LINE__, __FILE__);

        // Control Codes are in Group 2
        switch(map.m_match)
        {
            case 1: // Handle [ text ] inside brackets
                // Build replacement here
                key = map.m_code;
                value = parseFormatColorsBrackets(map.m_code, config);
                addMCIMapping(key, value);
                break;

            case 2: // Handle :
                // Build replacement here
                key = map.m_code;
                value = parseFormatColorsColon(map.m_code, config);
                addMCIMapping(key, value);
                break;

            default:
                break;
        }
    }

    // Then feed though and return the updated string.
    output = parseCodeMapGenerics(sequence, code_map);
    return output;
}

/**
 * @brief Checks a String if it matches the expression passed.
 * @param sequence
 * @param expression
 * @return
 */
bool SessionIO::checkRegex(const std::string &sequence, const std::string &expression)
{
    // Create system default locale
    std::setlocale(LC_ALL, Encoding::ENCODING_TEXT_UTF8.c_str());
    std::cout.imbue(std::locale());

    std::smatch match;
    bool result = false;

    //std::cout << "exp: " << expression << std::endl;
    try
    {
        std::regex regExpression(expression);
        result = std::regex_match(sequence, match, regExpression);
    }
    catch(std::regex_error &ex)
    {
        m_log.write<Logging::ERROR_LOG>("[checkRegex] Expression=", expression, "Exception=", ex.what(), ex.code(), __LINE__, __FILE__);
    }

    return result;
}

/**
 * @brief Parses Text Prompt String Pair
 * @param prompt
 * @return
 */
std::string SessionIO::parseTextPrompt(const M_StringPair &prompt)
{
    // |PD is new for XRM, Prompt Description.
    std::string text_prompt = prompt.second;
    std::string mci_code = "|PD";

    // If Description Flag is in Prompt, then replace code with Description
    m_common_io.parseLocalMCI(text_prompt, mci_code, prompt.first);

    // Return full mci code parsing on the new string.
    return pipe2ansi(text_prompt);
}

/**
 * @brief Stores Key (MCI Code) Value (String for Replacement) in Mapping
 * @param key
 * @param value
 * @return
 */
void SessionIO::addMCIMapping(const std::string &key, const std::string &value)
{
    m_mapped_codes.insert(std::pair<std::string, std::string>(std::move(key), std::move(value)));
}

/**
 * @brief Clears all mappings
 */
void SessionIO::clearAllMCIMapping()
{
    if(m_mapped_codes.size() > 0)
    {
        std::map<std::string, std::string>().swap(m_mapped_codes);
    }
}

/**
 * @brief Get a Count of all Mapped MCI Codes
 * @return
 */
int SessionIO::getMCIMappingCount()
{
    return m_mapped_codes.size();
}
