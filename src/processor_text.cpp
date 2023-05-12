#include "processor_text.hpp"
#include "model-sys/structures.hpp"
#include "common_io.hpp"
#include "logging.hpp"

#include "utf-cpp/utf8.h"

#include <cstdio>
#include <cstdlib>
#include <cctype>
#include <cstring>
#include <stdint.h>

#include <regex>

// Unicode Output Encoding.
#include <iostream> // cout
#include <clocale>  // locale
#include <cwchar>   // wchar_t wide characters
#include <string>   // string and wstring
#include <fstream>
#include <vector>
#include <sstream>


/**
 * @brief Buffer to String for Parsing
 */
std::string ProcessorText::screenBufferToString()
{
    m_ansi_output.erase();
    m_ansi_output = "";

    for(unsigned int i = 0; i < m_screen_buffer.size(); i++)
    {
        auto &buff = m_screen_buffer[i];

        if(buff.char_sequence[0] == '\r')
        { } //  character = "\x1b[40m\r\n";
        else if(buff.char_sequence[0] == '\0')
            m_ansi_output += " ";
        else
            m_ansi_output += buff.char_sequence;
    }

    return m_ansi_output;
}

/**
 * @brief Test, displays screen buffer.
 */
void ProcessorText::screenBufferDisplayTest()
{
    int attr = 0;
    int fore = 0;
    int back = 0;

    m_ansi_output.erase();
    m_ansi_output = "";
    std::string character = "";

    if(m_is_screen_cleared)
    {
        m_ansi_output.append("\x1b[1;1H\x1b[2J");
    }

    for(unsigned int i = 0; i < m_screen_buffer.size(); i++)
    {
        auto &buff = m_screen_buffer[i];
        std::stringstream ss;

        if(buff.x_position >= 1)
        {
            if(attr !=  buff.attribute ||
                    fore != buff.foreground ||
                    back != buff.background)
            {
                ss  << "\x1b["
                    << buff.attribute << ";"
                    << buff.foreground << ";"
                    << buff.background << "m";

                m_ansi_output.append(ss.str());

                attr = buff.attribute;
                fore = buff.foreground;
                back = buff.background;
            }

            ss.clear();
            ss.ignore();

            // buff.c;
            if(buff.char_sequence[0] == '\r')
            { } //  character = "\x1b[40m\r\n";
            else if(buff.char_sequence[0] == '\0')
                character = " ";
            else
                character = buff.char_sequence;

            m_ansi_output.append(character);
        }
        else
        {
            character = " ";
            m_ansi_output.append(character);
        }
    }

    // Screen should always end with reset.
    m_ansi_output.append("\x1b[0m");
}

/**
 * @brief MCI Off-Set if Mid ANSI
 * Used for Single Line ANSI Inserts to determine where special MCI Codes are located.
 *
 * @param clearScreen
 * @return
 */
int ProcessorText::getMCIOffSet(std::string mci_code)
{
    unsigned int max = (m_x_position + (m_y_position * m_characters_per_line));

    for(unsigned int i = 0; i < m_screen_buffer.size(); i++)
    {

        // If buffer parse move past current cursor positon
        if((i + 1) >= max)
        {
            break;
        }

        // Check for MCI Code, if it matches, return position.
        if(i + 2 < max)
        {
            if(m_screen_buffer[i].char_sequence[0] == (unsigned char)mci_code[0] &&
                    m_screen_buffer[i+1].char_sequence[0] == (unsigned char)mci_code[1] &&
                    m_screen_buffer[i+2].char_sequence[0] == (unsigned char)mci_code[2])
            {
                return i+1;
            }
        }
    }

    return 0;
}

/**
 * @brief Takes buffer and displays parsed sequences
 * NOTE, this can add a new line at the end of screen
 * Should exclude for BOTTOM Ansi Screens.
 */
std::string ProcessorText::getScreenFromBuffer(bool clearScreen)
{
    int attr = 0;
    int fore = 0;
    int back = 0;

    // We want to cound \0 characters in a row.
    // These are unplotted so we use home cursor drawing
    // and ESC[C to push the cursor forward without overwritting
    int padding = 0;

    std::string ansi_output = "";

    if(clearScreen)
    {
        ansi_output.append("\x1b[1;1H\x1b[2J");
    }

    int count = 1;

    for(unsigned int i = 0; i < m_screen_buffer.size(); i++)
    {
        auto &buff = m_screen_buffer[i];

        // If buffer parse move past current cursor positon
        if(count >= (m_x_position + (m_y_position * m_characters_per_line)))
        {
            break;
        }

        std::stringstream ss;

        if(attr !=  buff.attribute ||
                fore != buff.foreground ||
                back != buff.background)
        {
            ss  << "\x1b["
                << buff.attribute << ";"
                << buff.foreground << ";"
                << buff.background << "m";

            if(padding == 0)
                ansi_output.append(ss.str());

            attr = buff.attribute;
            fore = buff.foreground;
            back = buff.background;
        }

        ss.clear();
        ss.ignore();

        // Options and skip null non plotted characters by
        // moving the drawing position forward.
        if(padding > 0 && buff.char_sequence[0] != '\0')
        {
            ansi_output += "\x1b[" + std::to_string(padding) + "C";
            // Get the Color change or first character after padding.
            ansi_output.append(ss.str());
            padding = 0;
        }

        if(padding > 0 && (i > 0 && i % m_characters_per_line == 0))
        {
            ansi_output += "\x1b[" + std::to_string(padding) + "C";
            ansi_output.append(ss.str());
            padding = 0;
            ansi_output.append("\x1B[1D\r\n");
        }
        else if((i > 0 && i % m_characters_per_line == 0))
        {
            ansi_output.append("\x1B[1D\r\n");
        }

        if(buff.char_sequence[0] == '\r')
        { } //  character = "\x1b[40m\r\n";
        else if(buff.char_sequence[0] == '\0')
        {
            ++padding;
            ++count;
            continue;
        }
        else
        {
            ansi_output.append(buff.char_sequence);
        }

        ++count;
    }

    // Screen should always end with reset.
    ansi_output.append("\x1b[0m");
    return ansi_output;
}

/**
 * @brief Build the string for Lightbar coors and colors.
 * @param pulldown_id
 * @return
 */
std::string ProcessorText::buildPullDownBars(int pulldown_id, bool active)
{
    std::string output = "";
    std::stringstream ss;

    std::map<int, ScreenPixel>::iterator it;
    it = m_pull_down_options.find(pulldown_id);

    // If We have the pulldown ID
    if(it != m_pull_down_options.end())
    {
        // First Position
        ss  << "\x1b["
            << (it)->second.y_position
            << ";"
            << (it)->second.x_position
            << "H";

        // Next Color Depending if it's the selected bar.
        if(active)
        {
            ss << "\x1b["
               << (it)->second.selected_attribute
               << ";"
               << (it)->second.selected_foreground
               << ";"
               << (it)->second.selected_background
               << "m";
        }
        else
        {
            ss << "\x1b["
               << (it)->second.attribute
               << ";"
               << (it)->second.foreground
               << ";"
               << (it)->second.background
               << "m";
        }

        output = ss.str();
    }

    ss.clear();
    ss.ignore();

    return output;
}

/**
 * @brief // Clear Pull Down Bars once menu options are reset.
 */
void ProcessorText::clearPullDownBars()
{
    std::map<int, ScreenPixel>().swap(m_pull_down_options);
}

/**
 * @brief Return the max rows used on the screen
 * @return
 */
int ProcessorText::getMaxRowsUsedOnScreen()
{
    return m_max_y_position;
}

/**
 * @brief Parses through MCI Codes for Lightbars and Char Parameters.
 */
std::string ProcessorText::screenBufferParse()
{
    // Contains all matches found so we can iterate and reaplace
    // Without Multiple loops through the string.
    MapType my_matches;
    std::vector<MapType> code_map;
    CommonIO common_io;

    // To make parsing a little faster, pre-fill vector with 99,
    // So it's not allocating each insert.
    code_map.reserve(99);
    code_map.resize(99);

    // Make a copy that we can modify and process on.
    std::string ansi_string = m_ansi_output;

    //std::cout << "exp: (\\|[0-9]{2}[%][0-9]{2}) " << std::endl;
    // Each Set of Codes for Expression Matches 1 set. will need more for char screens.
    try
    {
        std::regex expr {"(\\|[0-9]{2}[%][0-9]{2})"};

        std::smatch matches;
        std::string::const_iterator start = ansi_string.begin(), end = ansi_string.end();

        std::regex_constants::match_flag_type flags = std::regex_constants::match_default;

        while(std::regex_search(start, end, matches, expr, flags))
        {
            // Avoid Infinite loop and make sure the existing
            // is not the same as the next!
            if(start == matches[0].second)
            {
                Logging &log = Logging::getInstance();
                log.write<Logging::DEBUG_LOG>("[screenBufferParse] no matches!",
                                                __LINE__, __FILE__);
                break;
            }

            // Since were replacing on the fly, we need to rescan the screen for next code
            start = matches[0].second;

            // Loop each match, and grab the starting position and length to replace.
            for(size_t s = 1; s < matches.size(); ++s)
            {
                // Make sure the Match is true! otherwise skip.
                if(matches[s].matched)
                {
                    // Add to Vector so we store each match.
                    my_matches.m_offset = matches[s].first - ansi_string.begin();
                    my_matches.m_length = matches[s].length();
                    my_matches.m_match  = s;
                    my_matches.m_code   = matches[s].str();

                    // UTF-8. meed to use a utf8-distance to get actual char off-set to match
                    // screen buffer now vs. raw byte off-set.
                    int match_off_set = my_matches.m_offset;
                    auto new_it = ansi_string.begin() + match_off_set;
                    int char_length = utf8::distance(ansi_string.begin(), new_it);

                    my_matches.m_offset = char_length;
                    code_map.push_back(std::move(my_matches));
                }
            }
        }
    }
    catch(std::regex_error &ex)
    {
        Logging &log = Logging::getInstance();
        log.write<Logging::ERROR_LOG>("[screenBufferParse] regex=",
                                        ex.what(), ex.code(), __LINE__, __FILE__);
    }

    // All Global MCI Codes likes standard screens and colors will
    // He handled here, then specific interfaces will break out below this.
    // Break out parsing on which pattern was matched.

    // Clear Pulldown ID mapping.
    std::map<int, ScreenPixel>::iterator itr = m_pull_down_options.begin();

    if(code_map.size() > 0)
    {
        while(itr != m_pull_down_options.end())
        {
            itr = m_pull_down_options.erase(itr);
        }
    }

    while(code_map.size() > 0)
    {
        // Loop Backwards to perserve string offsets on replacement.
        // Fastest to pop from back.
        my_matches = code_map.back();
        code_map.pop_back();

        // Handle parsing on expression match.
        switch(my_matches.m_match)
        {
            case 1:
                // Then process and display the lightbars.
            {
                int pull_id = 0;
                std::stringstream ss;
                ss.str(my_matches.m_code.substr(1, 2));
                ss >> pull_id;

                if(ss.fail())
                {
                    ss.clear();
                    ss.ignore();
                    break;
                }

                // Grab the highlight color from the second sequence %##.
                m_screen_buffer[my_matches.m_offset].selected_attribute =
                    m_screen_buffer[my_matches.m_offset+3].attribute;

                m_screen_buffer[my_matches.m_offset].selected_foreground =
                    m_screen_buffer[my_matches.m_offset+3].foreground;

                m_screen_buffer[my_matches.m_offset].selected_background =
                    m_screen_buffer[my_matches.m_offset+3].background;

                // tear out the y and x positions from the offset.
                m_pull_down_options[pull_id] = m_screen_buffer[my_matches.m_offset];

                ss.clear();
                ss.ignore();
            }
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
 * @brief Text Handler pre-positioning prior to storing character
 * @param char_sequence
 */
void ProcessorText::handleTextInput(const std::string &char_sequence)
{
    
    // Set the Current Max Row Position.
    if(m_max_y_position < m_y_position)
    {
        m_max_y_position = m_y_position;
    }

    // Keep track of the lonest line in buffer for Centering screen.
    if(m_x_position > m_line_ending_map[m_line_number])
    {
        m_line_ending_map[m_line_number] = m_x_position;
    }

    // catch screen screen scrolling here one shot.
    if(m_y_position > m_number_lines)
    {        
        // Not Yet Setup 
        screenBufferScrollUp();
        m_y_position = m_number_lines;
        m_max_y_position = m_y_position;        
    }
    
    
    // Writes the Char Sequence to the Text Screen Buffer    
    screenBufferSetGlyph(char_sequence);
    
    
    // Move Cursor to next position after character insert.
    if(m_x_position >= m_characters_per_line)
    {
        // Move to next line
        m_x_position = 1;
        ++m_y_position;
        ++m_line_number;
    }
    else
    {
        ++m_x_position;

        // Make sure the x/y position does go over num lines
        if(m_y_position > m_number_lines)
        {
            m_y_position = m_number_lines;
        }
    }
    
    // Setup Mapping for Max Line X Position per Line for END keys.
    // And special Funcations like move up and end of line.
    try
    {
        int line_x_pos = m_line_ending_map.at(m_line_number);

        if(line_x_pos < m_x_position)
        {
            m_line_ending_map[m_line_number] = m_x_position;
        }

    }
    catch(const std::out_of_range&)
    {
        m_line_ending_map.insert(std::pair<int, int>(m_line_number, m_x_position));
    }
}

/**
 * @brief Plots Characters on the Screen into the Buffer.
 * @param char_sequence
 */
void ProcessorText::screenBufferSetGlyph(const std::string &char_sequence)
{
    m_screen_pixel.char_sequence = char_sequence;
    m_screen_pixel.x_position = m_x_position;
    m_screen_pixel.y_position = m_y_position;
    m_screen_pixel.attribute  = m_attribute;
    m_screen_pixel.foreground = m_foreground_color;
    m_screen_pixel.background = m_background_color;
    
    
    // FIXME So we need a Text buffer to store the pixel (character) info per each line
    // can we use the screen buffer, or do we want something else?
    // YES, use screen bummer, fast vector of <pixel> 

    /*
    // Setup current position in the screen buffer. 1 based for 0 based.
    m_position = ((m_y_position-1) * m_characters_per_line) + (m_x_position-1);

    // Add Sequence to Screen Buffer
    try
    {
        if(m_position < (signed)m_screen_buffer.size())
        {
            m_screen_buffer.at(m_position) = m_screen_pixel;
        }
        else
        {
            Logging &log = Logging::getInstance();
            log.write<Logging::ERROR_LOG>("[screenBufferSetGlyph] out of bounds pos=",
                                            m_x_position-1, __LINE__, __FILE__);
        }
    }
    catch(std::exception &e)
    {
        Logging &log = Logging::getInstance();
        log.write<Logging::ERROR_LOG>("[screenBufferSetGlyph] exceeds screen dimensions Exception=",
                                        e.what(), __LINE__, __FILE__);
    }*/

    // Clear for next sequences.
    m_screen_pixel.char_sequence = '\0';
    m_screen_pixel.x_position = 1;
    m_screen_pixel.y_position = 1;
    m_screen_pixel.attribute  = 0;
    m_screen_pixel.foreground = FG_WHITE;
    m_screen_pixel.background = BG_BLACK;

}

/**
 * @brief Moves the Screen Buffer Up a line to match the internal SDL_Surface
 */
void ProcessorText::screenBufferScrollUp()
{
    // Screen Buffer is continious, scroll up
    // Buffer and re-display on screen
}

/**
 * @brief Moves the Screen Buffer Up a line to match the internal SDL_Surface
 */
void ProcessorText::screenBufferScrollDown()
{
    // Screen Buffer is continious, scroll down
    // Buffer and re-display on screen
}

/*
 * Clear Range of Screen Buffer for Erase Sequences.
 */
void ProcessorText::screenBufferClearRange(int start, int end)
{
    int startPosition = ((m_y_position-1) * m_characters_per_line) + (start);
    int endPosition = startPosition + (end - start);

    // Clear out entire line.
    for(int i = startPosition; i < endPosition; i++)
    {
        try
        {
            m_screen_buffer[i].char_sequence = '\0';
        }
        catch(std::exception &e)
        {
            Logging &log = Logging::getInstance();
            log.write<Logging::ERROR_LOG>("[screenBufferClearRange] Exception=", e.what(),
                                            "start=", start, "end=", end, __LINE__, __FILE__);
        }
    }
}

/**
 * @brief Clears the Buffer for Fresh Parsing.
 */
void ProcessorText::screenBufferClear()
{
    // Allocate the Size
    m_screen_buffer.clear();
    m_screen_buffer.resize(m_number_lines * m_characters_per_line);
}

/**
 * @brief Clears The Screen And Buffer
 */
void ProcessorText::clearScreen()
{
    std::cout << "ProcessorT clearScreen" << std::endl;
    
    m_is_screen_cleared = true;
    screenBufferClear();
    m_x_position = 1;
    m_y_position = 1;
    m_max_y_position = 1;
    m_line_number = 1;
    m_number_lines = 1;
    std::map<int, int>().swap(m_line_ending_map);
}

/**
 * @brief Move to Start of the current line
 */
void ProcessorText::moveHomePosition()
{
    std::cout << "ProcessorT moveHomePosition" << std::endl;
    
    m_x_position = 1;
}

/**
 * @brief Move End of the current line (Not 100%) WIP.
 */
void ProcessorText::moveEndPosition()
{
    std::cout << "ProcessorT moveEndPosition" << std::endl;
    
    m_x_position = m_line_ending_map[m_line_number];
    if (m_x_position == 0) 
    {
        // When no line ending, we'll move to last position on the screen
        m_x_position = m_characters_per_line;
    }
}

/**
 * @brief Handle FORWARD chacter movements
 */
void ProcessorText::moveNextXPosition()
{
    std::cout << "ProcessorT moveNextXPosition" << std::endl;
    
    if(m_x_position < m_characters_per_line)
        ++m_x_position;
}

/**
 * @brief Handle DOWN character movements
 */
void ProcessorText::moveNextYPosition()
{
    std::cout << "ProcessorT moveNextYPosition" << std::endl;
    
    if(m_y_position < m_number_lines)
        ++m_y_position;
        
    // Update for max lines for scroll up/dn lateron
    ++m_line_number;
}

/**
 * @brief Handle BACKWARDS chacters movements
 */
void ProcessorText::movePreviousXPosition()
{
    std::cout << "ProcessorT movePreviousXPosition" << std::endl;

    if(m_x_position > 1)
        --m_x_position;
}

/**
 * @brief Handle UP character movements
 */
void ProcessorText::movePreviousYPosition()
{
    std::cout << "ProcessorT movePreviousYPosition" << std::endl;

    if(m_y_position > 1)
        --m_y_position;
        
    // Update for max lines for scroll up/dn lateron
    --m_line_number;
}

/**
 * @brief Move to Next Line.
 */
void ProcessorText::moveNewLine()
{
    std::cout << "ProcessorT moveNewLine" << std::endl;

    m_x_position = 1;
    ++m_y_position;
    ++m_line_number;

    // catch screen screen scrolling here one shot.
    if(m_y_position > m_number_lines)
    {
        screenBufferScrollUp();
        m_y_position = m_number_lines;
    }

    // Set the Current Max Row Position.
    if(m_max_y_position < m_y_position)
    {
        m_max_y_position = m_y_position;
    }
}

/**
 * @brief Handle Descructive Backspace
 */
void ProcessorText::moveBackSpace()
{
    std::cout << "ProcessorT moveBackSpace" << std::endl;
    
    // Backspace or Delete, the entire lines loses a char.
    if (m_line_ending_map[m_line_number] > 0) {
        m_line_ending_map[m_line_number]--;
    }

    if(m_x_position > 1)
    {        
        --m_x_position;
        return;
    }

    // Else, Were as first positiong moving up to previous line
    // If it's not the very top of the text area.
    if(m_y_position > 1)
    {
        --m_y_position;
    }

    // Each line represents a line of text that can run off the screen
    // so the actual lines in the message buffer, storing line we are on.
    if(m_line_number > 1)
    {                
        // Reset the current line, we're at the fist spot moving up on Backspace.
        m_line_ending_map[m_line_number] = 0;
        if (m_line_ending_map[m_line_number] > 0) {
            m_line_ending_map[m_line_number]--;
        }
        
        // Setup for Previous Line, if we move up, move back 1 automatically.
        --m_line_number;
        
        std::cout << "ProcessorT moveBackSpace (Move Line Up)" << "m_line_ending_map[m_line_number]="<< m_line_ending_map[m_line_number] << std::endl;        
        
        // We moved up a line, now we need to
        // move to end of current line, as long as were not at top left of box.
        moveEndPosition();
        
        std::cout << "ProcessorT moveBackSpace (Move Line Up)" << "moveEndPosition="<< m_x_position << std::endl;
        
        // Double Escape is when you're Backspace up and move to the end of the line
        // There is 1 chat at the right edge, when we backspace in this instance
        // we do a second one to remove that character then move left 1 spot to take
        // it's position and not overwrite the templates.
        
        setDoubleBackSpace(true);
    }
    // Were on Top Line Now, just moved there.
    else 
    {
        std::cout << "ProcessorT moveBackSpace (Move Line Up ! ELSE)" << std::endl;
        
        if (m_line_ending_map[m_line_number] > 0) {
            m_line_ending_map[m_line_number]--;
        }
    }

}

/**
 * @brief Handle Destructive Delete key
 */
void ProcessorText::moveDelete()
{
    std::cout << "ProcessorT moveDelete" << std::endl;
    // clear current, or erase on on line so next char move back
}

/**
 * @brief Move Tab Width = 4 Spaces if Available.
 */
void ProcessorText::moveTabWidth()
{
    std::cout << "ProcessorT moveTabWidth" << std::endl;

        
    // Move forward m_tab_width amount of spaces, not moving past end of box.
    for (int i = 0; i < m_tab_width; i++) 
    {
        moveNextXPosition();  
    }

}

/**
 * @brief Escape Sequence Parsing
 */
void ProcessorText::escapeSequenceParsing(LocalizedBuffer &buffer,
        std::string::iterator &it,
        std::string::iterator &line_end)
{
    
    std::cout << "escapeSequenceParsing !! " << std::endl;
                
    CommonIO common_io;
    std::string esc_sequence = "";

    int  param[10]             = {0};
    int  p = 0, dig = 0;

    bool more_params           = false;
    bool at_least_one_digit    = false;
    bool first_param_implied   = false;

    if(it == line_end)
        return;

    if(buffer.length == 1 && buffer.character[0] == '\x1b')
    {
        esc_sequence.erase();
        esc_sequence += buffer.character;

        common_io.getNextGlyph(buffer, it, line_end);

        if(buffer.length == 0)
            return;

        // grab the left bracket
        if(buffer.length == 1 && buffer.character[0] == '?')
        {
            // Setup for ESC?7h or ESC?7l commands etc..
            esc_sequence += buffer.character;
        }
        else if(buffer.length == 1 && buffer.character[0] == '[')
        {
            // Else Normal ESC Sequence, check parameters.
            esc_sequence += buffer.character;
        }

        more_params = true;
        first_param_implied = false;
        p = 0;

        while(more_params == true)
        {
            at_least_one_digit = false;
            common_io.getNextGlyph(buffer, it, line_end);

            if(buffer.length == 0)
                break;

            for(dig = 0; dig < 3; dig++)
            {
                if(buffer.length != 1 || !isdigit(buffer.character[0]))
                    break;

                at_least_one_digit = true;

                // 3 digits at most (255) in a byte size decimal number */
                if(dig == 0)
                {
                    param[p] = buffer.character[0] - '0';
                }
                else if(dig == 1)
                {
                    param[p] *= 10;
                    param[p] += buffer.character[0] - '0';
                }
                else
                {
                    param[p] *= 100;
                    param[p] += buffer.character[0] - '0';
                }

                esc_sequence += buffer.character;
                common_io.getNextGlyph(buffer, it, line_end);
            }

            //   ESC[C     p should = 0
            //   ESC[6C    p should = 1
            //   ESC[1;1H  p should = 2
            //   ESC[;79H  p should = 2
            if(buffer.character[0] != '?')     // Skip Screen Wrap (The Draw)
            {
                if((at_least_one_digit == true) &&
                        (buffer.character[0] == ';'))
                {
                    p++;
                }
                else if((!(at_least_one_digit == true)) &&
                        (buffer.character[0] == ';'))
                {
                    p++;
                    first_param_implied = true;
                }
                else if(at_least_one_digit)
                {
                    p++;
                    more_params = false;
                }
                else
                    more_params = false;
            }

            esc_sequence += buffer.character;

        } // End While (more_params)

        std::cout << "ESCAPE SEQUENCE: " << esc_sequence << std::endl;
        std::cout << "buffer.character[0]: " << buffer.character[0] << std::endl;
        std::cout << "p: " << p << std::endl;

        switch(buffer.character[0])
        {

            case CURSOR_POSITION:
            case CURSOR_POSITION_ALT:
                if(p == 0)
                {
                    m_x_position = 1;
                    m_y_position = 1;
                }
                else if(p == 1)
                {
                    m_x_position = 1;
                    m_y_position = param[0];
                }
                else if(first_param_implied)
                {
                    m_x_position = param[1];
                }
                else
                {
                    m_x_position = param[1];
                    m_y_position = param[0];
                }

                //screen_buff.esc_sequence += esc_sequence;
                esc_sequence.clear();
                break;

            case CURSOR_PREV_LIVE:
            case CURSOR_UP:
                if(p == 0)
                {
                    if(m_y_position > 1)
                        --m_y_position;
                }
                else
                {
                    if(param[0] > m_y_position)
                    {
                        m_y_position = 1;
                    }
                    else
                    {
                        m_y_position -= param[0];

                        if(m_y_position < 1)
                        {
                            m_y_position = 1;
                        }
                    }
                }

                esc_sequence.clear();
                break;

            case CURSOR_NEXT_LINE:
            case CURSOR_DOWN:
                if(p == 0)
                {
                    if(m_y_position < m_number_lines)
                        ++m_y_position;
                }
                else
                {
                    if(param[0] > (m_number_lines) - m_y_position)
                    {
                        //m_y_position = m_number_lines - 1;
                        m_y_position = m_number_lines;
                    }
                    else
                    {
                        m_y_position += param[0];

                        if(m_y_position > m_number_lines) m_y_position = m_number_lines;
                    }
                }

                esc_sequence.clear();
                break;

            case CURSOR_FORWARD:
                if(p == 0)
                {
                    if(m_x_position < m_characters_per_line)
                        ++m_x_position;
                }
                else
                {
                    if(param[0] > m_characters_per_line - m_x_position)
                    {
                        m_x_position = m_characters_per_line;
                    }
                    else
                    {
                        m_x_position += param[0];

                        if(m_x_position > m_characters_per_line)
                        {
                            m_x_position = m_characters_per_line;
                        }
                    }

                }

                esc_sequence.clear();
                break;

            case CURSOR_BACKWARD:
                if(p == 0)
                {
                    if(m_x_position > 1)
                    {
                        --m_x_position;
                    }
                }
                else
                {
                    if(param[0] > m_x_position)
                    {
                        m_x_position = 1;
                    }
                    else
                    {
                        m_x_position -= param[0];

                        if(m_x_position < 1) m_x_position = 1;
                    }
                }

                esc_sequence.clear();
                break;

            /*
            case SAVE_CURSOR_POS:
                m_saved_cursor_x = m_x_position;
                m_saved_cursor_y = m_y_position;
                m_saved_attribute = m_attribute;
                m_saved_foreground = m_foreground_color;
                m_saved_background = m_background_color;
                esc_sequence.clear();
                break;

            case RESTORE_CURSOR_POS:
                m_x_position = m_saved_cursor_x;
                m_y_position = m_saved_cursor_y;
                m_attribute = m_saved_attribute;
                m_foreground_color = m_saved_foreground;
                m_background_color = m_saved_background;
                esc_sequence.clear();
                break;
            */
            case CURSOR_X_POSITION: // XTERM
                if(p == 0)
                    m_x_position = 1;
                else
                    m_x_position = param[0];

                esc_sequence.clear();
                break;

            case ERASE_DISPLAY:
                if(param[0] == 2)
                {
                    clearScreen();
                }

                esc_sequence.clear();
                break;

            case ERASE_TO_EOL:
                //clreol ();
                //screen_buff.esc_sequence += esc_sequence;
                esc_sequence.clear();
                break;

            case SET_GRAPHICS_MODE:
                // Rebuild ESC Sequqnces and Attach Character Position
                // So when we draw out the ansi we can keep track
                // Of what color everything is.
                esc_sequence.erase(); // Ignore ESC in color, we get that sepeartely.

                //position = ((y_position-1) * characters_per_line) + (x_position-1);

                // Change text attributes / All Attributes off
                if(p == 0)
                {
                    m_attribute = 0;
                    m_foreground_color = FG_WHITE;
                    m_background_color = BG_BLACK;
                }
                else
                {
                    //current_color = "\x1b[";
                    for(int i = 0; i < p; i++)
                    {
                        switch(param[i])
                        {
                            case 0: // All Attributes off
                                m_attribute = 0;
                                m_foreground_color = FG_WHITE;
                                m_background_color = BG_BLACK;
                                break;

                            case 1: // BOLD_ON (increase intensity)
                                m_attribute = 1;
                                break;

                            case 2: // FAINT (decreased intensity)
                                m_attribute = 2;
                                break;

                            case 3: // ITALICS ON
                                m_attribute = 3;
                                break;

                            case 4:	// UNDERSCORE
                                m_attribute = 4;
                                break;

                            case 5: // BLINK_ON
                                m_attribute = 5;
                                break;

                            case 6: // BLINK RAPID MS-DOS ANSI.SYS;
                                m_attribute = 6;
                                break;

                            case 7: // REVERSE_VIDEO_ON
                                m_attribute = 7;
                                break;

                            case 8: // CONCEALED_ON
                                m_attribute = 8;
                                break;

                            case 9: // STRICKTHROUGH
                                m_attribute = 9;
                                break;

                            case 22: // BOLD OFF
                                m_attribute = 22;
                                break;

                            case 23: // ITALICS OFF
                                m_attribute = 23;
                                break;

                            case 24: // UNDERLINE OFF
                                m_attribute = 24;
                                break;

                            case 27: // INVERSE OFF
                                m_attribute = 27;
                                break;

                            case 28: // CONCEALED_OFF
                                m_attribute = 28;
                                break;

                            case 29: // STRIKETHROUGH OFF
                                m_attribute = 29;
                                break;

                            case 30: // FG_BLACK
                                m_foreground_color = FG_BLACK;
                                break;

                            case 31: // FG_RED
                                m_foreground_color = FG_RED;
                                break;

                            case 32: // FG_GREEN
                                m_foreground_color = FG_GREEN;
                                break;

                            case 33: // FG_YELLOW
                                m_foreground_color = FG_YELLOW;
                                break;

                            case 34: // FG_BLUE
                                m_foreground_color = FG_BLUE;
                                break;

                            case 35: // FG_MAGENTA
                                m_foreground_color = FG_MAGENTA;
                                break;

                            case 36: // FG_CYAN
                                m_foreground_color = FG_CYAN;
                                break;

                            case 37: // FG_WHITE
                                m_foreground_color = FG_WHITE;
                                break;

                            case 40: // BG_BLACK
                                m_background_color = BG_BLACK;
                                break;

                            case 41: // BG_RED
                                m_background_color = BG_RED;
                                break;

                            case 42: // BG_GREEN
                                m_background_color = BG_GREEN;
                                break;

                            case 43: // BG_YELLOW
                                m_background_color = BG_YELLOW;
                                break;

                            case 44: // BG_BLUE
                                m_background_color = BG_BLUE;
                                break;

                            case 45: // BG_MAGENTA
                                m_background_color = BG_MAGENTA;
                                break;

                            case 46: // BG_CYAN
                                m_background_color = BG_CYAN;
                                break;

                            case 47: // BG_WHITE
                                m_background_color = BG_WHITE;
                                break;

                            default :
                                m_attribute = param[i];
                                break;

                        } // End Switch
                    } // End For
                } // End Else

                break;

            case RESET_MODE: // ?7h
                esc_sequence.clear();

                if(param[0] == 7)
                {
                    m_is_line_wrapping = false;
                }

                break;

            case SET_MODE:  // ?7h  & 25 ?!?
                esc_sequence.clear();

                if(param[0] == 7)
                {
                    m_is_line_wrapping = true;
                }

                break;

            case SET_KEYBOARD_STRINGS:
                esc_sequence.clear();
                // Ingored!
                break;

            default:
                esc_sequence.clear();
                // Rest are ignored.
                break;

        } // End of Switch(c) Case Statements

    } // end of main escape sequence handler
}

/**
 * @brief Parses screen data into the Screen Buffer.
 * @return
 */
void ProcessorText::parseTextToBuffer(char *buff)
{
    if(strlen(buff) == 0)
    {
        return;
    }

    std::string incoming_data = std::string(
                                    reinterpret_cast<const char *>(buff),
                                    strlen((const char *)buff)
                                );

    std::string::iterator it = incoming_data.begin();
    std::string::iterator line_end = incoming_data.end();

    CommonIO common_io;
    LocalizedBuffer buffer;

    while(it != line_end)
    {
        common_io.getNextGlyph(buffer, it, line_end);

        std::cout << "loop char <int>: " << static_cast<int>(buffer.character[0]) << std::endl;

        // ESC Sequences
        if(buffer.length == 1 && buffer.character[0] == '\x1b')
        {
            // Most likely won't get ESC sequences here,
            // They are translated to key inputs prior to reaching here.
            std::cout << "escapeSequenceParsing: " << buffer.character << std::endl;
            escapeSequenceParsing(buffer, it, line_end);
        }
        // Back Space
        else if(buffer.character[0] == '\b')
        {
            moveBackSpace();
        }
        // Catch Tabs
        else if(buffer.character[0] == '\t')
        {
            moveTabWidth();
        }
        else if(buffer.length == 1 && buffer.character[0] == '\n')
        {
            moveNewLine();
        }
        // Append Character to Screen Buffer.
        else if(buffer.character[0] != '\0' && buffer.length >= 1)
        {            
            handleTextInput(buffer.character);
        }
        else {
            std::cout << "not handled charater sequence: " << static_cast<int>(buffer.character[0]) << std::endl;
        }
        
        // End of Position Updates for movement.
    }
}
