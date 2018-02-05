#include "ansi_processor.hpp"
#include "common_io.hpp"

#include "model-sys/structures.hpp"

#include <cstdio>
#include <cstdlib>
#include <cctype>
#include <cstring>
#include <stdint.h>

#include <boost/regex.hpp>

// Unicode Output Encoding.
#include <iostream> // cout
#include <clocale>  // locale
#include <cwchar>   // wchar_t wide characters
#include <string>   // string and wstring
#include <fstream>
#include <vector>
#include <sstream>


AnsiProcessor::AnsiProcessor(int term_height, int term_width)
    : m_ansi_output("")
    , m_is_screen_cleared(false)
    , m_is_line_wrapping(false)
    , m_position(0)
    , m_y_position(1)
    , m_number_lines(term_height)
    , m_characters_per_line(term_width)
    , m_x_position(1)
    , m_max_x_position(1)
    , m_max_y_position(1)
    , m_center_ansi_output(false)
    , m_saved_cursor_x(1)
    , m_saved_cursor_y(1)
    , m_attribute(0)
    , m_saved_attribute(0)
    , m_saved_foreground(FG_WHITE)
    , m_saved_background(BG_BLACK)
    , m_foreground_color(FG_WHITE)
    , m_background_color(BG_BLACK)
    , m_screen_pixel()
{
    std::cout << "AnsiProcessor" << std::endl;
    m_screen_buffer.reserve(m_number_lines * m_characters_per_line);
    m_screen_buffer.resize(m_number_lines * m_characters_per_line);
}

AnsiProcessor::~AnsiProcessor()
{
    std::cout << "~AnsiProcessor" << std::endl;
}


/**
 * @brief Buffer to String for Parsing
 */
std::string AnsiProcessor::screenBufferToString()
{
    m_ansi_output.erase();
    m_ansi_output = "";

    for(unsigned int i = 0; i < m_screen_buffer.size(); i++)
    {
        auto &buff = m_screen_buffer[i];

        if(buff.c == '\r')
        { } //  character = "\x1b[40m\r\n";
        else if(buff.c == '\0')
            m_ansi_output += " ";
        else
            m_ansi_output += buff.c;
    }
    return m_ansi_output;
}


/**
 * @brief Test, displays screen buffer.
 */
void AnsiProcessor::screenBufferDisplayTest()
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

    int count = 1;
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
            if(buff.c == '\r')
            { } //  character = "\x1b[40m\r\n";
            else if(buff.c == '\0')
                character = " ";
            else
                character = buff.c;

            m_ansi_output.append(character);
            ++count;
        }
        else
        {
            character = " ";
            m_ansi_output.append(character);
            ++count;
        }
    }
    // Screen should always end with reset.
    m_ansi_output.append("\x1b[0m");
}


/**
 * @brief Takes buffer and displays parsed sequences
 */
std::string AnsiProcessor::getScreenFromBuffer(bool clearScreen)
{
    int attr = 0;
    int fore = 0;
    int back = 0;

    // We want to cound \0 characters in a row.
    // These are unplotted so we use home cursor drawing
    // and ESC[C to push the cursor forward without overwritting
    int padding = 0;

    std::string ansi_output = "";
    std::string character = "";

    if(clearScreen)
    {
        ansi_output.append("\x1b[1;1H\x1b[2J");
    }

    int count = 1;
    for(unsigned int i = 0; i < m_screen_buffer.size(); i++)
    {
        auto &buff = m_screen_buffer[i];
        // If buffer parse move past current cursor positon
        if (count >= (m_x_position + (m_y_position * m_characters_per_line)))
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

            if (padding == 0)
                ansi_output.append(ss.str());

            attr = buff.attribute;
            fore = buff.foreground;
            back = buff.background;
        }
        ss.clear();
        ss.ignore();

        // Options and skip null non plotted characters by
        // moving the drawing position forward.
        if (padding > 0 && buff.c != '\0')
        {
            ansi_output += "\x1b[" + std::to_string(padding) + "C";
            // Get the Color change or first character after padding.
            ansi_output.append(ss.str());
            padding = 0;
        }

        if (padding > 0 && (i > 0 && i % m_characters_per_line == 0))
        {
            ansi_output += "\x1b[" + std::to_string(padding) + "C";
            ansi_output.append(ss.str());
            padding = 0;
            ansi_output.append("\x1B[1D\r\n");
        }
        else if ((i > 0 && i % m_characters_per_line == 0))
        {
            ansi_output.append("\x1B[1D\r\n");
        }

        if(buff.c == '\r')
        { } //  character = "\x1b[40m\r\n";
        else if(buff.c == '\0')
        {
            ++padding;
            ++count;
            continue;
        }
        else
        {
            character = buff.c;
        }

        ansi_output.append(character);
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
std::string AnsiProcessor::buildPullDownBars(int pulldown_id, bool active)
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
    return output;
}


/**
 * @brief // Clear Pull Down Bars once menu options are reset.
 */
void AnsiProcessor::clearPullDownBars()
{
    std::map<int, ScreenPixel>().swap(m_pull_down_options);
}


/**
 * @brief Return the max rows used on the screen
 * @return
 */
int AnsiProcessor::getMaxRowsUsedOnScreen()
{
    return m_max_y_position;
}


/**
 * @brief Parses through MCI Codes for Lightbars and Char Parameters.
 */
std::string AnsiProcessor::screenBufferParse()
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

    // Each Set of Codes for Expression Matches 1 set. will need more for char screens.
    boost::regex expr {"(\\|[0-9]{2}+%[0-9]{2})"};

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

        // Avoid Infinite loop and make sure the existing
        // is not the same as the next!
        if (start == matches[0].second)
        {
            std::cout << "no more matches!" << std::endl;
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

                    // Grab the highlight color from the second sequence %##.
                    m_screen_buffer[my_matches.m_offset].selected_attribute =
                        m_screen_buffer[my_matches.m_offset+3].attribute;

                    m_screen_buffer[my_matches.m_offset].selected_foreground =
                        m_screen_buffer[my_matches.m_offset+3].foreground;

                    m_screen_buffer[my_matches.m_offset].selected_background =
                        m_screen_buffer[my_matches.m_offset+3].background;

                    // tear out the y and x positions from the offset.
                    m_pull_down_options[pull_id] = m_screen_buffer[my_matches.m_offset];
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
 * @brief Plots Characters on the Screen into the Buffer.
 * @param c
 */
void AnsiProcessor::screenBufferSetPixel(char c)
{
    // Keep track of the lonest line in buffer for Centering screen.
    if(m_x_position > m_max_x_position)
    {
        m_max_x_position = m_x_position;
    }

    // catch screen screen scrolling here one shot.
    //if (m_y_position >= m_number_lines)
    if (m_y_position > m_number_lines)
    {
        screenBufferScrollUp();
        //m_y_position = m_number_lines-1;
        m_y_position = m_number_lines;
    }

    m_screen_pixel.c = c;
    m_screen_pixel.x_position = m_x_position;
    m_screen_pixel.y_position = m_y_position;
    m_screen_pixel.attribute  = m_attribute;
    m_screen_pixel.foreground = m_foreground_color;
    m_screen_pixel.background = m_background_color;

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
            std::cout << "position out of bounds: " << m_x_position-1 << std::endl;
        }
    }
    catch(std::exception &e)
    {
        std::cout << "Exception setScreenBuffer: " << e.what() << std::endl;
        std::cout << "Server sent data that exceeds screen dimensions." << std::endl;
    }

    // Clear for next sequences.
    m_screen_pixel.c = '\0';
    m_screen_pixel.x_position = 1;
    m_screen_pixel.y_position = 1;
    m_screen_pixel.attribute  = 0;
    m_screen_pixel.foreground = FG_WHITE;
    m_screen_pixel.background = BG_BLACK;

    // Move Cursor to next position after character insert.
    if(m_x_position >= m_characters_per_line)
    {
        // Move to next line
        m_x_position = 1;
        ++m_y_position;
    }
    else
    {
        ++m_x_position;
    }

}

/*
 * Moves the Screen Buffer Up a line to match the internal SDL_Surface
 */
void AnsiProcessor::screenBufferScrollUp()
{
    //*** IMPORTANT (WIP), must add check for region scrolling only!
    //TheTerminal::Instance()->scrollRegionActive &&
    //                 y_position > TheTerminal::Instance()->bottomMargin))

    // Theory, Erase Line at Top margin, then add a new line bottom margin
    // To move it back down.  That way only the middle is scrolled up.

    // This remove the top line to scroll the screen up
    // And follow the SDL Surface!  later on add history for scroll back.
    try
    {
        m_screen_buffer.erase(
            m_screen_buffer.begin(), m_screen_buffer.begin() + m_characters_per_line);
    }
    catch(std::exception &e)
    {
        std::cout << "Exception scrollScreenBuffer: " << e.what() << std::endl;
    }
    // Readd The last Line back to the buffer.
    m_screen_buffer.resize(m_number_lines * m_characters_per_line);
}

/*
 * Clear Range of Screen Buffer for Erase Sequences.
 */
void AnsiProcessor::screenBufferClearRange(int start, int end)
{
    int startPosition = ((m_y_position-1) * m_characters_per_line) + (start);
    int endPosition = startPosition + (end - start);

    //std::cout << "start " << start << " end " << end
    // << std::endl;
    //std::cout << "startPosition " << startPosition << " endPosition " << endPosition
    // << std::endl;

    // Clear out entire line.
    for(int i = startPosition; i < endPosition; i++)
    {
        try
        {
            m_screen_buffer[i].c = '\0';
        }
        catch(std::exception &e)
        {
            std::cout << "Exception clearScreenBufferRange: "
                      << e.what() << std::endl;
        }
    }
    // Debugging
    //getScreenBufferText();
}

/**
 * @brief Clears the Buffer for Fresh Parsing.
 */
void AnsiProcessor::screenBufferClear()
{
    // Allocate the Size
    m_screen_buffer.clear();
    m_screen_buffer.resize(m_number_lines * m_characters_per_line);    
}

/**
 * @brief Clears The Screen And Buffer
 */
void AnsiProcessor::clearScreen()
{
    m_is_screen_cleared = true;
    screenBufferClear();
    m_x_position = 1;
    m_y_position = 1;
    m_max_y_position = 1;
}

/**
 * @brief Parses screen data into the Screen Buffer.
 * @return
 */
void AnsiProcessor::parseAnsiScreen(char *buff)
{
    if (strlen(buff) == 0)
        return;

    std::string esc_sequence;

    int  param[10]             = {0};
    unsigned char c            = 0;
    int  p = 0, dig = 0;

    bool more_params           = false;
    bool at_least_one_digit    = false;
    bool first_param_implied   = false;

    int starting_seq = 0;
    int ending_seq   = 0;

    for(int z = 0; z <= (signed)strlen(buff); z++)
    {
        c = buff[z];

        //Handle escape sequence
        if(c == '\x1b')
        {
            // Store entire ESC Sequence
            starting_seq = z;

            // grab the left bracket
            c = buff[++z];

            if(buff[z+1] == '7' && buff[z+2] == 'h')
            {
                //std::cout << "line wrapping enabled!" << std::endl;
                m_is_line_wrapping = true;
                z+= 2;
            }

            more_params = true;
            first_param_implied = false;
            p = 0;

            while(more_params == true)
            {
                at_least_one_digit = false;
                ++z;
                for(dig = 0; (isdigit(c = buff[z])) && (dig < 3); dig++)
                {
                    at_least_one_digit = true;

                    // 3 digits at most (255) in a byte size decimal number */
                    if(dig == 0)
                    {
                        param[p] = c - '0';
                    }
                    else if(dig == 1)
                    {
                        param[p] *= 10;
                        param[p] += c - '0';
                    }
                    else
                    {
                        param[p] *= 100;
                        param[p] += c - '0';
                    }
                    z++;
                }
                //   ESC[C     p should = 0
                //   ESC[6C    p should = 1
                //   ESC[1;1H  p should = 2
                //   ESC[;79H  p should = 2
                if(c != '?')     // Skip Screen Wrap (The Draw)
                {
                    if((at_least_one_digit == true) && (c == ';'))
                    {
                        p++;
                    }
                    else if((!(at_least_one_digit == true)) && (c == ';'))
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

            } // End While (more_params)

            // Great ESC Sequence in entirity
            ending_seq = z;

            // loop and cut out full ESC Sequence to store it.
            // We only store with input key if it's parsed
            // Otherwise we skip it.
            for(int seq = starting_seq; seq < ending_seq+1; seq++)
            {
                esc_sequence += buff[seq];                
            }

            // Handle specific escape sequences
            switch(c)
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
                            if(m_y_position < 1) m_y_position = 1;
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
                            if(m_x_position > m_characters_per_line) m_x_position = m_characters_per_line;
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
                            std::cout << "###" << param[0];
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

                    if(p == 0)    // Change text attributes / All Attributes off
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

                                case 6: // BLINK RAPID MS-DOS ANSI.SYS; 150 per minute or more; not widely supported
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
        else   // otherwise output character using current color */
        {
            // Handle New Line in ANSI Files properly.
            if(c == '\r' && buff[z+1] == '\n')
            {
                ++z; // Incriment past \n (2) char combo.
                m_x_position = 1;
                ++m_y_position;

                esc_sequence.erase();

                // Set the Current Max Row Position.
                if (m_max_y_position < m_y_position)
                {
                    m_max_y_position = m_y_position;
                }

                // catch screen screen scrolling here one shot.
                //if (m_y_position >= m_number_lines)
                if (m_y_position > m_number_lines)
                {
                    screenBufferScrollUp();
                    //m_y_position = m_number_lines-1;
                    m_y_position = m_number_lines;
                }
                continue;
            }
            else if(c == '\r' || c == '\n')
            {
                m_x_position = 1;
                ++m_y_position;

                esc_sequence.erase();

                // Set the Current Max Row Position.
                if (m_max_y_position < m_y_position)
                {
                    m_max_y_position = m_y_position;
                }

                // catch screen screen scrolling here one shot.
                //if (m_y_position >= m_number_lines)
                if (m_y_position > m_number_lines)
                {
                    screenBufferScrollUp();
                    //m_y_position = m_number_lines-1;
                    m_y_position = m_number_lines;
                }
                continue;
            }

            // Append Character to Screen Buffer.
            if(c != '\0')
            {
                // Set the Current Max Row Position.
                if (m_max_y_position < m_y_position)
                {
                    m_max_y_position = m_y_position;
                }

                screenBufferSetPixel(c);
            }
            esc_sequence.erase();
        }
    }   // end while !feof
    
    std::cout << "x_position: " << m_x_position << std::endl;
    std::cout << "y_position: " << m_y_position << std::endl;
}
