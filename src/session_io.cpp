#include "session_io.hpp"
#include "session_data.hpp"
#include "common_io.hpp"

#include <boost/regex.hpp>
#include <string>
#include <iostream>
#include <sstream>

SessionIO::SessionIO(session_data_ptr session_data)
    : m_session_data(session_data)
{
    std::cout << "SessionIO" << std::endl;
}

SessionIO::~SessionIO()
{
    std::cout << "~SessionIO" << std::endl;
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
        return "";
    }

    std::string escape_sequence = "";
    if(input[0] == '\x1b')
    {
        escape_sequence = m_common_io.getEscapeSequence();
        if(escape_sequence.size() == 0)
        {
            //std::cout << "Single ESC" << std::endl;
            return "\x1b";
        }
        else
        {
            //std::cout << "Translated ESC Sequence: "  << escape_sequence << std::endl;
            return (escape_sequence.insert(0, "\x1b"));
        }
    }

    //std::cout << "Normal Input: "  << input << std::endl;
    return input;
}

/**
 * @brief Generates an input field with ANSI color background
 * @param field_name {Is the Field Name, preceeds the field like 'Mail From: '}
 * @param len
 */
void SessionIO::createInputField(std::string &field_name, int &len)
{
    std::string repeat;
    char formatted[1024]= {0};

    char sTmp[3]  = {0};
    char sTmp2[3] = {0};

    // Parse for Input String Modifiers
    std::string::size_type tempLength;
    std::string::size_type position;
    std::string::size_type stringSize;

    char INPUT_COLOR[255]= {0};
    bool isColorOverRide = false; //found input color

    stringSize = field_name.size()-1;
    if(len == 0)
    {
        return;
    }

    // Format Input Field, if color is enabled, otherwise just add Field Name like "Login: "
    if (!m_session_data->m_is_use_ansi)
    {
        sprintf(formatted, "%s", (char *)field_name.c_str()); // Field Name
        field_name = formatted;
        return;
    }


    // Overide Input Length for Ansi
    position = field_name.find("%IN", 0);
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
                    std::cout << "createInputField() Incorrect %IN field length.  Length cannot be larger than "
                              << "max size of the field, which is: "
                              << len
                              << std::endl;
                }
            }
            else
                field_name.erase(position, 3);
        }
    }

    // Overide Foreground/Background Input Field Colors
    position = field_name.find("%FB",0);

    std::cout << "position: " <<  position << std::endl;
    std::cout << "compare : " << position+4 << " " <<  stringSize << std::endl;

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
                //std::cout << "ESC -> Field Input aborted!" << std::endl;
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
            //std::cout << "Field: " << result << std::endl;
            string_data.erase();
            is_leadoff = true;    // Reset for next run
            return "\n";
        }
        // Updates on Keypresses.
        else
        {
            std::cout << "input_system result: " << result << std::endl;
            return string_data;
        }
    }

    std::cout << "result empty!" << std::endl;
    return "";
}


/**
 * @brief Parses for ANSI Foreground Colors (Helper for pipe2ansi)
 * @param data
 * @param foreground
 */
std::string SessionIO::pipeReplaceForground(int foreground)
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
        esc_sequence = std::move(pipeReplaceForground(color_index));
        return esc_sequence;
    }
    // Background Colors
    else if(color_index >= 16 && color_index < 24)
    {
        esc_sequence = std::move(pipeReplaceBackground(color_index));
        return esc_sequence;
    }
    return esc_sequence;
}

/**
 * @brief Parsed Pipe Codes with 1 or 2 Digits
 * @param pipe_code
 * @return
 */
std::string SessionIO::parsePipeWithCharsDigits(const std::string &code, int value)
{
    std::string sequence = "";

    std::cout << "parsePipeWithCharsDigits" << std::endl;

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

    }
    return sequence;
}

/**
 * @brief Seperate Pipe Codes with 1 or 2 Digits
 * @param pipe_code
 * @return
 */
std::string SessionIO::seperatePipeWithCharsDigits(const std::string &pipe_code)
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

    // Split Alpha from Numberic so we can process it!
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
    // Strip %% and grab the 'Filename.ext'
    std::string str = pipe_code.substr(2);

    CommonIO common_io;

    std::string buffer = "";
    common_io.readinAnsi(str, buffer);
    if (buffer.size() > 0)
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
 * @brief Parses MCI/Pipe Codes to ANSI Sequences.
 * @param sequence
 * @param interface
 * @return
 */
std::string SessionIO::pipe2ansi(const std::string &sequence, int interface)
{
    // Contains all matches found so we can iterate and reaplace
    // Without Multiple loops through the string.
    MapType my_matches;
    std::vector<MapType> code_map;

    // To make parsing a little faster, pre-fill vector with 50,
    // So it's not allocating each insert.
    //code_map.reserve(50);
    //code_map.resize(50);

    // Make a copy that we can modify and process on.
    std::string ansi_string = sequence;

    // Each Set of Codes for Expression Matches 1 - 5.
    // 1. boost::regex expr {"(\\|[0-9]{2})"};
    // 2. boost::regex expr {"(\\|[A-Z]{1,2}+[0-9]{1,2})"};
    // 3. boost::regex expr {"(\\|[A-Z]{2})"}; //(?!\\d)"};
    // 4. boost::regex expr {"(\\%[A-Z]{2})"};
    // 5. boost::regex expr {"(\\|[A-Z]{2}+[0-9]{4})"};
    // 6. boost::regex expr {"(\\%[0-9]{2})"};
    // 7. boost::regex expr {"(\\%%[A-Za-z0-9]+[.].{3})"}; // Get filename.ext from strings.

    boost::regex expr {"(\\|[0-9]{2})|(\\|[A-Z]{1,2}+[0-9]{1,2})|(\\|[A-Z]{2})|(%[A-Z]{2})|(\\|[A-Z]{2}+[0-9]{4})|(\\%[0-9]{2})|(\\%%[A-Za-z0-9]+[.].{3})"};

    boost::smatch matches;
    std::string::const_iterator start = ansi_string.begin(), end = ansi_string.end();
    //std::string::size_type offset = 0;
    //std::string::size_type length = 0;

    boost::match_flag_type flags = boost::match_default;
    while(boost::regex_search(start, end, matches, expr, flags))
    {
        // Found a match!
        /*
        std::cout << "Matched Sub '" << matches.str()
                  << "' following ' " << matches.prefix().str()
                  << "' preceeding ' " << matches.suffix().str()
                  << std::endl;*/

        // Since were replacing on the fly, we need to rescan the screen for next code
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
                std::cout << s << " :  Matched Sub " << matches[s].str()
                          << " at offset " << offset
                          << " of length " << length
                          << std::endl;*/

                // Add to Vector so we store each match.
                my_matches.m_offset = matches[s].first - ansi_string.begin();
                my_matches.m_length = matches[s].length();
                my_matches.m_match  = s;
                my_matches.m_code   = matches[s].str();
                code_map.push_back(std::move(my_matches));
            }
        }
    }

    std::cout << "code_map.size(): " << code_map.size() << std::endl;

    // All Global MCI Codes likes standard screens and colors will
    // He handled here, then specific interfaces will break out below this.
    // Break out parsing on which pattern was matched.
    while(code_map.size() > 0)
    {
        // Loop Backwards to perserve string offsets on replacement.
        my_matches = code_map.back();
        code_map.pop_back();

        // Handle parsing on expression match.
        switch(my_matches.m_match)
        {
            case 1: // Pipe w/ 2 DIDIT Colors
                {
                    std::cout << "Pipe w/ 2 DIDIT Colors" << std::endl;
                    std::string result = std::move(pipeColors(my_matches.m_code));
                    if(result.size() != 0)
                    {
                        // Replace the Color, if not ansi then remove the color!
                        if (m_session_data->m_is_use_ansi)
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

            case 2: // Pipe w/ 1 or 2 CHARS followed by 1 or 2 DIGITS
                {
                    std::cout << "Pipe w/ 1 or 2 CHARS followed by 1 or 2 DIGITS" << std::endl;
                    std::string result = std::move(seperatePipeWithCharsDigits(my_matches.m_code));
                    if(result.size() != 0)
                    {
                        // Replace the string
                        ansi_string.replace(my_matches.m_offset, my_matches.m_length, result);
                    }
                }
                break;

            case 3: // Pipe w/ 2 CHARS
                // This one will need replacement in the string parsing
                // Pass the original string becasue of |DE for delay!
                {
                    std::cout << "Pipe w/ 2 CHARS" << std::endl;
                    std::string result = std::move(parsePipeWithChars(my_matches.m_code));
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

            case 4: // Percent w/ 2 CHARS
                {
                    std::cout << "Percent w/ 2 CHARS" << std::endl;
                    // Remove for now, haven't gotten this far!
                    ansi_string.replace(my_matches.m_offset, my_matches.m_length, "   ");
                }
                break;

            case 5: // Pipe w/ 2 Chars and 4 Digits
                // Mainly used for absolute XY position |XY0101
                {
                    std::cout << "Pipe w/ 2 Chars and 4 Digits" << std::endl;
                    // Remove for now, haven't gotten this far!
                    ansi_string.replace(my_matches.m_offset, my_matches.m_length, "       ");
                }
                break;

            case 6: // Percent with 2 digits, custom codes
                {
                    // Were just removing them becasue they are processed.
                    // Now that first part of sequence |01 etc.. are processed!
                    std::cout << "replacing %## codes" << std::endl;
                    // Remove for now, haven't gotten this far!
                    ansi_string.replace(my_matches.m_offset, my_matches.m_length, "   ");
                }
                break;

            case 7: // %%FILENAME.EXT  get filenames for loading from string prompts
                {
                    std::cout << "replacing %%FILENAME.EXT codes" << std::endl;
                    std::string result = std::move(parseFilename(my_matches.m_code));
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

            default:
                break;
        }

        // This is to parse for Interface specific MCI Codes.
        switch(interface)
        {
                // Standard + Unique to Menu Prompt
            case MENU_PROMPT_MCI:
                break;

            default:
                break;
        }
    }

    // Clear Codemap.
    std::vector<MapType>().swap(code_map);
    return ansi_string;
}


/**
 * @brief Parses Text Prompt String Pair
 * @param prompt
 * @return
 */
std::string SessionIO::parseTextPrompt(const M_StringPair &prompt)
{
    // Looks like D1 is delay on not description for text prompts,
    // Need to work on a timer displya procress for delays and
    // long ansi screen scrolling that is async friends and will not hose
    // up the system loop.  in these cases might need a thread?!?
    // Delays can wait till more of the system it put togehter to test what
    // works best with multiple users online and doing stuff.

    /*
    // handle to prompt
    std::string text_prompt = prompt.second;
    std::string mci_code = "|D1";

    // If Description Flag is in Prompt, then replace code with Description
    m_common_io.parseLocalMCI(text_prompt, mci_code, prompt.first);

    // Return full mci code parsing on the new string.
    return pipe2ansi(text_prompt);*/

    // If emulation is not active, then we need to remove pipe colors!
    // WIP
    //if (!m_session_data->m_is_use_ansi)

    std::cout << "parseTextPrompt" << std::endl;

    std::string text_prompt = prompt.second;
    return pipe2ansi(text_prompt);
}