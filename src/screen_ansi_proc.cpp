#include "screen_ansi_proc.hpp"

#include <cstdio>
#include <cstdlib>
#include <cctype>
#include <cstring>
#include <regex>
#include <iostream>
#include <cwchar>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include <map>

#include "model-sys/structures.hpp"
#include "model-sys/screen_pixel.hpp"

#include "io_common.hpp"
#include "io_session.hpp"
#include "util_log.hpp"

ScreenAnsiProc::ScreenAnsiProc()
    : ScreenBase(24, 80) {
    m_screen_buffer.resize(m_number_lines * m_characters_per_line);
}

ScreenAnsiProc::~ScreenAnsiProc() {
    m_log.log(UtilLog::LogLevel::Console, "~ScreenAnsiProc()");

    m_screen_buffer.clear();
    m_pull_down_options.clear();
    m_line_ending_map.clear();
}

void ScreenAnsiProc::resize(const int term_height, const int term_width) {
    m_number_lines = term_height;
    m_characters_per_line = term_width;

    m_screen_buffer.clear();
    m_screen_buffer.resize(m_number_lines * m_characters_per_line);
}

/**
 * @brief Buffer to String for Parsing
 */
std::string ScreenAnsiProc::screenBufferToString() {
    m_ansi_output.erase();
    for (unsigned int i = 0; i < m_screen_buffer.size(); i++) {
        auto &buff = m_screen_buffer[i];

        if (buff.char_sequence.empty()) {
            m_ansi_output += " ";
        } else if (buff.char_sequence[0] != '\r') {
            m_ansi_output += buff.char_sequence;
        }
    }

    return m_ansi_output;
}

/**
 * @brief Takes buffer and displays parsed sequences
 * NOTE, this can add a new line at the end of screen
 * Should exclude for BOTTOM Ansi Screens.
 */
std::string ScreenAnsiProc::getScreenFromBuffer(bool clearScreen) {
    int attr = 0;
    int fore = 0;
    int back = 0;

    // We want to count \0 characters in a row.
    // These are not plotted so we use home cursor drawing
    // and ESC[C to push the cursor forward without overwriting
    int padding = 0;

    std::string ansi_output;

    // TODO , need to get context in here, so we can determine
    // TODO , scroll type, clear or scroll screen down entire!
    // NOTES: Clear screen doesn't leave scroll back buffer in terminals.
    if (clearScreen) {
        ansi_output.append("\x1b[1;1H\x1b[2J");
    }

    size_t cursor_index = (m_y_position - 1) * m_characters_per_line + (m_x_position - 1);
    for (size_t i = 0; i < cursor_index && i < m_screen_buffer.size(); ++i) {
        auto &buff = m_screen_buffer[i];
        cursor_index = (m_y_position - 1) * m_characters_per_line + (m_x_position - 1);

        std::string out;
        if (attr != buff.attribute ||
            fore != buff.foreground ||
            back != buff.background) {
            out += "\x1b["
                    + std::to_string(buff.attribute) + ";"
                    + std::to_string(buff.foreground) + ";"
                    + std::to_string(buff.background) + "m";

            if (padding == 0) {
                ansi_output.append(out);
            }

            attr = buff.attribute;
            fore = buff.foreground;
            back = buff.background;
        }

        // Options and skip empty characters by
        // ANSI OPTIMIZE moving the drawing position forward.
        // TODO, do we want to move forward, or put a space,
        // Optimized, but if we're overdrawing, can leave artifacts!!
        if (padding > 0 && !buff.char_sequence.empty()) {
            ansi_output += "\x1b[" + std::to_string(padding) + "C";
            // Get the Color change or first character after padding.
            ansi_output.append(out);
            padding = 0;
        }

        if (padding > 0 && (i > 0 && i % m_characters_per_line == 0)) {
            // Padding up to End Of Line
            ansi_output += "\x1b[" + std::to_string(padding) + "C";
            ansi_output.append(out);
            padding = 0;
            ansi_output.append("\x1B[1D\r\n");
        } else if ((i > 0 && i % m_characters_per_line == 0)) {
            // Normal Output Up to End of Line
            ansi_output.append("\x1B[1D\r\n");
        }

        // TODO review, if we can use spaces instead of padding
        // When it's not a clear screen, initial display not working.
        if (buff.char_sequence.empty()) {
            ++padding;
            continue;
        }

        if (buff.char_sequence[0] != '\r') {
            ansi_output.append(buff.char_sequence);
        }
    }

    // Screen should always end with reset.
    ansi_output.append("\x1b[0m");
    return ansi_output;
}

/**
 * @brief Build the string for Light bar coors and colors.
 * @param pullDownId
 * @param isSelected
 * @return
 */
std::string ScreenAnsiProc::buildPullDownBars(int pullDownId, bool isSelected) {
    std::string output;

    // If We have the pull down ID
    if (const auto it = m_pull_down_options.find(pullDownId); it != m_pull_down_options.end()) {
        // TODO Add a flag here for Custom Hi/Low Strings to override ansi codes
        // And use custom menu defined strings. Extra! Like Enthral.
        // if new high/low bars are populated, it would override |01%01 in lightbar ansi's.
        // most likely a new method needed, as this is only inserting colors in position.

        // First Position
        output += "\x1b["
                + std::to_string((it)->second.y_position) + ";"
                + std::to_string((it)->second.x_position) + "H";

        // Next Color Depending on if it's the selected bar.
        if (isSelected) {
            output += "\x1b["
                    + std::to_string((it)->second.selected_attribute) + ";"
                    + std::to_string((it)->second.selected_foreground) + ";"
                    + std::to_string((it)->second.selected_background) + "m";
        } else {
            output += "\x1b["
                    + std::to_string((it)->second.attribute) + ";"
                    + std::to_string((it)->second.foreground) + ";"
                    + std::to_string((it)->second.background) + "m";
        }
    }

    return output;
}


/**
 * @brief // Clear Pull Down Bars once menu options are reset.
 */
void ScreenAnsiProc::clearPullDownBars() {
    m_pull_down_options.clear();
}

/**
 * @brief Return the max rows used on the screen
 * @return
 */
int ScreenAnsiProc::getMaxRowsUsedOnScreen() {
    return m_max_y_position;
}

/**
 * @brief Parses through MCI Codes for Light bars and Char Parameters.
 */
std::string ScreenAnsiProc::screenBufferParse() {
    IoCodeMapping codeMapping;
    std::vector<CodeMapType> code_map = codeMapping.parseScreenBufferToCodeMap(
        m_ansi_output, LIGHT_BAR_EXPRESSION);

    // All Global MCI Codes likes standard screens and colors will
    // He handled here, then specific interfaces will break out below this.
    // Break out parsing on which pattern was matched.

    // Clear Pull down ID mapping.
    std::map<int, ScreenPixel>::iterator itr = m_pull_down_options.begin();

    if (code_map.empty()) {
        return m_ansi_output;
    }

    // Only Work With Pull Down Options for Display!
    while (itr != m_pull_down_options.end()) {
        itr = m_pull_down_options.erase(itr);
    }

    CodeMapType my_matches;
    while (!code_map.empty()) {
        // Loop Backwards to preserve string offsets on replacement.
        // Fastest to pop from back.
        my_matches = code_map.back();
        code_map.pop_back();

        // Handle parsing on expression match.
        switch (my_matches.m_match) {
            // Setup Lightbars for Active and Inactive Display.
            case 1: {
                int pull_id = 0;
                std::stringstream ss;
                ss.str(my_matches.m_code.substr(1, 2));
                ss >> pull_id;

                if (ss.fail()) {
                    ss.clear();
                    ss.ignore();
                    break;
                }

                if (my_matches.m_offset + 3 < m_screen_buffer.size()) {
                    // Grab the highlight color from the second sequence %##.
                    m_screen_buffer[my_matches.m_offset].selected_attribute =
                            m_screen_buffer[my_matches.m_offset + 3].attribute;

                    m_screen_buffer[my_matches.m_offset].selected_foreground =
                            m_screen_buffer[my_matches.m_offset + 3].foreground;

                    m_screen_buffer[my_matches.m_offset].selected_background =
                            m_screen_buffer[my_matches.m_offset + 3].background;

                    // tear out the y and x positions from the offset.
                    m_pull_down_options[pull_id] = m_screen_buffer[my_matches.m_offset];
                }

                ss.clear();
                ss.ignore();
            }
            break;

            default:
                break;
        }
    }

    // Clear Code map.
    code_map.clear();
    return m_ansi_output;
}

/**
 * @brief Plots Characters on the Screen into the Buffer.
 * @param c
 */
void ScreenAnsiProc::screenBufferSetGlyph(const std::string &charSequence) {
    // Keep track of the longest line in buffer for Centering screen.
    if (m_x_position > m_max_x_position) {
        m_max_x_position = m_x_position;
    }

    // catch screen scrolling here one shot.
    if (m_y_position > m_number_lines) {
        screenBufferScrollUp();
        m_y_position = m_number_lines;

        // Set the Current Max Row Position.
        m_max_y_position = m_y_position;
    }

    ScreenPixel screen_pixel;
    screen_pixel.char_sequence = charSequence;
    screen_pixel.x_position = m_x_position;
    screen_pixel.y_position = m_y_position;
    screen_pixel.attribute = m_attribute;
    screen_pixel.foreground = m_foreground_color;
    screen_pixel.background = m_background_color;

    // Setup current position in the screen buffer. 1 based for 0 based.
    // X/Y ANSI Screen Coors are 1 Based, yet, Arrays are 0 Based.
    m_position = ((m_y_position - 1) * m_characters_per_line) + (m_x_position - 1);

    // Add Sequence to Screen Buffer
    try {
        if (m_position < static_cast<signed>(m_screen_buffer.size())) {
            m_screen_buffer.at(m_position) = screen_pixel;
        } else {
            m_log.log(UtilLog::LogLevel::Error, "[screenBufferSetGlyph] out of bounds pos=",
                      m_x_position - 1, __LINE__, __FILE__);
        }
    } catch (std::exception &e) {
        m_log.log(UtilLog::LogLevel::Error, "[screenBufferSetGlyph] exceeds screen dimensions Exception=",
                  e.what(), __LINE__, __FILE__);
    }

    // Move Cursor to next position after character insert.
    if (m_x_position >= m_characters_per_line) {
        // Move to next line
        m_x_position = 1;
        ++m_y_position;
    } else {
        ++m_x_position;
    }
}

/*
 * Moves the Screen Buffer Up a line to match the internal SDL_Surface
 */
void ScreenAnsiProc::screenBufferScrollUp() {
    //*** IMPORTANT (WIP), must add check for region scrolling only!
    //TheTerminal::Instance()->scrollRegionActive &&
    //                 y_position > TheTerminal::Instance()->bottomMargin))

    // Theory, Erase Line at Top margin, then add a new line bottom margin
    // To move it back down.  That way only the middle is scrolled up.

    // This remove the top line to scroll the screen up
    // And follow the SDL Surface!  later on add history for scroll back.
    try {
        m_screen_buffer.erase(
            m_screen_buffer.begin(), m_screen_buffer.begin() + m_characters_per_line);
    } catch (std::exception &e) {
        m_log.log(UtilLog::LogLevel::Error, "[screenBufferScrollUp] Exception=", e.what(), __LINE__, __FILE__);
    }

    // Readd The last Line back to the buffer.
    m_screen_buffer.resize(m_number_lines * m_characters_per_line);
}

/*
 * Clear Range of Screen Buffer for Erase Sequences.
 */
void ScreenAnsiProc::screenBufferClearRange(int start, int end) {
    int startPosition = ((m_y_position - 1) * m_characters_per_line) + (start);
    int endPosition = startPosition + (end - start);

    //std::cout << "start " << start << " end " << end
    // << std::endl;
    //std::cout << "startPosition " << startPosition << " endPosition " << endPosition
    // << std::endl;

    // Clear out entire line.
    for (int i = startPosition; i < endPosition; i++) {
        try {
            m_screen_buffer[i].char_sequence.clear();
        } catch (std::exception &e) {
            m_log.log(UtilLog::LogLevel::Error, "[screenBufferClearRange] Exception=", e.what(),
                      "start=", start, "end=", end, __LINE__, __FILE__);
        }
    }
}

/**
 * @brief Clears the Buffer for Fresh Parsing.
 */
void ScreenAnsiProc::screenBufferClear() {
    // Allocate the Size
    m_screen_buffer.clear();
    m_screen_buffer.resize(m_number_lines * m_characters_per_line);
}

/**
 * @brief Clears The Screen And Buffer
 */
void ScreenAnsiProc::clearScreen() {
    screenBufferClear();
    m_is_screen_cleared = true;
    m_x_position = 1;
    m_y_position = 1;
    m_max_x_position = 1;
    m_max_y_position = 1;
    m_attribute = 0;
    m_foreground_color = FG_DEFAULT;
    m_background_color = BG_BLACK;
}

/**
 * @brief Parses screen data into the Screen Buffer.
 * @return
 */
void ScreenAnsiProc::parseTextToBuffer(const std::string &buff) {
    if (buff.empty()) {
        return;
    }

    std::string escSequence;

    int param[10] = {0};
    int p = 0, dig = 0;

    bool more_params = false;
    bool at_least_one_digit = false;
    bool first_param_implied = false;

    std::string incoming_data(buff);
    std::string::const_iterator it = incoming_data.begin();
    std::string::const_iterator line_end = incoming_data.end();

    IoCommon common_io;
    Utf8Glyph buffer;

    while (it != line_end) {
        common_io.nextGlyph(buffer.bytes, it, buffer);

        if (buffer.length == 1 && buffer.bytes[0] == '\x1b') {
            escSequence.erase();
            escSequence += buffer.bytes;

            // Move to next Glyph.
            common_io.nextGlyph(buffer.bytes, it, buffer);
            if (buffer.length == 0) {
                break;
            }

            // grab the left bracket
            if (buffer.length == 1) {
                if (buffer.bytes[0] == '?' || buffer.bytes[0] == '[') {
                    // Setup for ESC?7h or ESC?7l or ESC commands etc.
                    escSequence += buffer.bytes;
                }
            }

            /*
            if(buff[z+1] == '7' && buff[z+2] == 'h')
            {
                //std::cout << "line wrapping enabled!" << std::endl;
                m_is_line_wrapping = true;
                z+= 2;
            }*/

            more_params = true;
            first_param_implied = false;
            p = 0;

            while (more_params == true) {
                at_least_one_digit = false;
                common_io.nextGlyph(buffer.bytes, it, buffer);

                if (buffer.length == 0)
                    break;

                for (dig = 0; dig < 3; dig++) {
                    if (buffer.length != 1 || !isdigit(buffer.bytes[0]))
                        break;

                    at_least_one_digit = true;

                    // 3 digits at most (255) in a byte size decimal number */
                    if (dig == 0) {
                        param[p] = buffer.bytes[0] - '0';
                    } else if (dig == 1) {
                        param[p] *= 10;
                        param[p] += buffer.bytes[0] - '0';
                    } else {
                        param[p] *= 100;
                        param[p] += buffer.bytes[0] - '0';
                    }

                    escSequence += buffer.bytes;
                    common_io.nextGlyph(buffer.bytes, it, buffer);
                }

                //   ESC[C     p should = 0
                //   ESC[6C    p should = 1
                //   ESC[1;1H  p should = 2
                //   ESC[;79H  p should = 2
                if (buffer.bytes[0] != '?') // Skip Screen Wrap (The Draw)
                {
                    if ((at_least_one_digit == true) &&
                        (buffer.bytes[0] == ';')) {
                        p++;
                    } else if ((!(at_least_one_digit == true)) &&
                               (buffer.bytes[0] == ';')) {
                        p++;
                        first_param_implied = true;
                    } else if (at_least_one_digit) {
                        p++;
                        more_params = false;
                    } else
                        more_params = false;
                }

                escSequence += buffer.bytes;
            } // End While (more_params)

            switch (buffer.bytes[0]) {
                case CURSOR_POSITION:
                case CURSOR_POSITION_ALT:
                    if (p == 0) {
                        m_x_position = 1;
                        m_y_position = 1;
                    } else if (p == 1) {
                        m_x_position = 1;
                        m_y_position = param[0];
                    } else if (first_param_implied) {
                        m_x_position = param[1];
                    } else {
                        m_x_position = param[1];
                        m_y_position = param[0];
                    }

                    //screen_buff.esc_sequence += esc_sequence;
                    escSequence.clear();
                    break;

                case CURSOR_PREV_LIVE:
                case CURSOR_UP:
                    if (p == 0) {
                        if (m_y_position > 1)
                            --m_y_position;
                    } else {
                        if (param[0] > m_y_position) {
                            m_y_position = 1;
                        } else {
                            m_y_position -= param[0];

                            if (m_y_position < 1) m_y_position = 1;
                        }
                    }

                    escSequence.clear();
                    break;

                case CURSOR_NEXT_LINE:
                case CURSOR_DOWN:
                    if (p == 0) {
                        if (m_y_position < m_number_lines)
                            ++m_y_position;
                    } else {
                        if (param[0] > (m_number_lines) - m_y_position) {
                            //m_y_position = m_number_lines - 1;
                            m_y_position = m_number_lines;
                        } else {
                            m_y_position += param[0];

                            if (m_y_position > m_number_lines) m_y_position = m_number_lines;
                        }
                    }

                    escSequence.clear();
                    break;

                case CURSOR_FORWARD:
                    if (p == 0) {
                        if (m_x_position < m_characters_per_line)
                            ++m_x_position;
                    } else {
                        if (param[0] > m_characters_per_line - m_x_position) {
                            m_x_position = m_characters_per_line;
                        } else {
                            m_x_position += param[0];

                            if (m_x_position > m_characters_per_line) m_x_position = m_characters_per_line;
                        }
                    }

                    escSequence.clear();
                    break;

                case CURSOR_BACKWARD:
                    if (p == 0) {
                        if (m_x_position > 1) {
                            --m_x_position;
                        }
                    } else {
                        if (param[0] > m_x_position) {
                            m_x_position = 1;
                        } else {
                            m_x_position -= param[0];

                            if (m_x_position < 1) m_x_position = 1;
                        }
                    }

                    escSequence.clear();
                    break;

                case SAVE_CURSOR_POS:
                    m_saved_cursor_x = m_x_position;
                    m_saved_cursor_y = m_y_position;
                    m_saved_attribute = m_attribute;
                    m_saved_foreground = m_foreground_color;
                    m_saved_background = m_background_color;
                    escSequence.clear();
                    break;

                case RESTORE_CURSOR_POS:
                    m_x_position = m_saved_cursor_x;
                    m_y_position = m_saved_cursor_y;
                    m_attribute = m_saved_attribute;
                    m_foreground_color = m_saved_foreground;
                    m_background_color = m_saved_background;
                    escSequence.clear();
                    break;

                case CURSOR_X_POSITION: // XTERM
                    if (p == 0)
                        m_x_position = 1;
                    else
                        m_x_position = param[0];

                    escSequence.clear();
                    break;

                case ERASE_DISPLAY:
                    if (param[0] == 2) {
                        clearScreen();
                    }

                    escSequence.clear();
                    break;

                case ERASE_TO_EOL:
                    //clreol ();
                    //screen_buff.esc_sequence += esc_sequence;
                    escSequence.clear();
                    break;

                case SET_GRAPHICS_MODE:
                    // Rebuild ESC Sequences and Attach Character Position
                    // So when we draw out the ansi we can keep track
                    // Of what color everything is.
                    escSequence.erase(); // Ignore ESC in color, we get that separately.

                    //position = ((y_position-1) * characters_per_line) + (x_position-1);

                    if (p == 0) // Change text attributes / All Attributes off
                    {
                        m_attribute = 0;
                        m_foreground_color = FG_DEFAULT;
                        m_background_color = BG_BLACK;
                    } else {
                        //current_color = "\x1b[";
                        for (int i = 0; i < p; i++) {
                            switch (param[i]) {
                                case 0: // All Attributes off
                                    m_attribute = 0;
                                    m_foreground_color = FG_DEFAULT;
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

                                case 4: // UNDERSCORE
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

                                default:
                                    m_attribute = param[i];
                                    break;
                            } // End Switch
                        } // End For
                    } // End Else

                    break;

                case RESET_MODE: // ?7h
                    escSequence.clear();

                    if (param[0] == 7) {
                        m_is_line_wrapping = false;
                    }

                    break;

                case SET_MODE: // ?7h  & 25 ?!?
                    escSequence.clear();

                    if (param[0] == 7) {
                        m_is_line_wrapping = true;
                    }

                    break;

                case SET_KEYBOARD_STRINGS:
                    escSequence.clear();
                    // Ignored!
                    break;

                default:
                    escSequence.clear();
                    // Rest are ignored.
                    break;
            } // End of Switch(c) Case Statements
        } // end of main escape sequence handler
        else // otherwise output character using current color */
        {
            Utf8Glyph nextBuffer;

            // Only Peak Next if were at CR.
            if (buffer.bytes[0] == '\r')
                common_io.peekGlyph(buffer.bytes, it, nextBuffer);

            // Handle New Line in ANSI Files properly.
            if (buffer.length == 1 && nextBuffer.length == 1 &&
                buffer.bytes[0] == '\r' && nextBuffer.bytes[0] == '\n') {
                ++it; // Increment to \n (2) char combo.
                m_x_position = 1;
                ++m_y_position;

                escSequence.erase();

                // Set the Current Max Row Position.
                if (m_max_y_position < m_y_position) {
                    m_max_y_position = m_y_position;
                }

                // catch screen screen scrolling here one shot.
                //if (m_y_position >= m_number_lines)
                if (m_y_position > m_number_lines) {
                    screenBufferScrollUp();
                    //m_y_position = m_number_lines-1;
                    m_y_position = m_number_lines;
                }

                continue;
            } else if (buffer.length == 1 && buffer.bytes[0] == '\n') {
                //m_x_position = 1;
                ++m_y_position;

                escSequence.erase();

                // Set the Current Max Row Position.
                if (m_max_y_position < m_y_position) {
                    m_max_y_position = m_y_position;
                }

                // catch screen screen scrolling here one shot.
                //if (m_y_position >= m_number_lines)
                if (m_y_position > m_number_lines) {
                    screenBufferScrollUp();
                    //m_y_position = m_number_lines-1;
                    m_y_position = m_number_lines;
                }

                continue;
            } else if (buffer.length == 1 && buffer.bytes[0] == '\r') {
                m_x_position = 1;
                //++m_y_position;

                escSequence.erase();

                // Set the Current Max Row Position.
                if (m_max_y_position < m_y_position) {
                    m_max_y_position = m_y_position;
                }

                // catch screen screen scrolling here one shot.
                //if (m_y_position >= m_number_lines)
                if (m_y_position > m_number_lines) {
                    screenBufferScrollUp();
                    //m_y_position = m_number_lines-1;
                    m_y_position = m_number_lines;
                }

                continue;
            }

            // Append Character to Screen Buffer.
            if (buffer.bytes[0] != '\0' && buffer.length >= 1) {
                // Set the Current Max Row Position.
                if (m_max_y_position < m_y_position) {
                    m_max_y_position = m_y_position;
                }

                screenBufferSetGlyph(buffer.bytes);
            }

            escSequence.erase();
        }
    } // end while !feof
}


std::map<int, int> ScreenAnsiProc::getLineEndingMap() const {
    return m_line_ending_map;
}

int ScreenAnsiProc::getPullDownMenuSize() const {
    return m_pull_down_options.size();
}

int ScreenAnsiProc::getMaxLines() const {
    return m_number_lines;
}
