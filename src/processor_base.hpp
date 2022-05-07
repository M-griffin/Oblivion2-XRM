#ifndef PROCESSOR_BASE_HPP
#define PROCESSOR_BASE_HPP

#include <memory>
#include <stdint.h>
#include <string>
#include <vector>
#include <map>

/**
 * @class ScreenPixel
 * @author Michael Griffin
 * @date 10/8/2015
 * @file ansi_processor.hpp
 * @brief Managers Characters and their attributes
 */
typedef struct ScreenPixel
{
    std::string char_sequence;
    int x_position;
    int y_position;
    int attribute;
    int foreground;
    int background;
    int selected_attribute;
    int selected_foreground;
    int selected_background;

} ScreenPixel;


/**
 * @class ProcessorBase
 * @author Michael Griffin
 * @date 01/12/2019
 * @file processor_base.hpp
 * @brief Base class for text processors
 */
class ProcessorBase
{
public:

    virtual ~ProcessorBase() { };

    // Process Build strings from pull_down array.
    virtual std::string buildPullDownBars(int pulldown_id, bool active) = 0;

    // Clear Pull Down Bars once menu options are reset.
    virtual void clearPullDownBars() = 0;

    // Return the max rows used on the screen
    virtual int getMaxRowsUsedOnScreen() = 0;

    // Parses the Screen Buffer and Builds pulldown options
    virtual std::string screenBufferParse() = 0;

    // Converts the Screen Buffer to a single string for parsing.
    virtual std::string screenBufferToString() = 0;

    // For testing display the output of the buffer.
    virtual void screenBufferDisplayTest() = 0;

    /**
     * @brief MCI Off-Set if Mid ANSI
     * Used for Single Line ANSI Inserts to determine where special MCI Codes are located.
     *
     * @param clearScreen
     * @return
     */
    virtual int getMCIOffSet(std::string mci_code) = 0;

    /**
     * @brief Takes buffer and displays parsed sequences
     */
    virtual std::string getScreenFromBuffer(bool clearScreen) = 0;


    // Screen Buffer Modifiers
    virtual void screenBufferSetGlyph(std::string char_sequence) = 0;
    virtual void screenBufferScrollUp() = 0;
    virtual void screenBufferClearRange(int start, int end) = 0;
    virtual void screenBufferClear() = 0;

    /**
     * @brief Clears The Screen And Buffer
     */
    virtual void clearScreen() = 0;

    // Process ANSI screen inserts into the Screen Buffer
    virtual void parseTextToBuffer(char *buff) = 0;

    // Helpers
    int getYPosition() const
    {
        return m_y_position;
    }
    int getXPosition() const
    {
        return m_x_position;
    }
    int getMaxLines() const
    {
        return m_number_lines;
    }

    /**
     * @brief ProcessorBase Constructor
     * @param term_height
     * @param term_width
     * @return
     */
    ProcessorBase(int term_height, int term_width)
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
        m_screen_buffer.reserve(m_number_lines * m_characters_per_line);
        m_screen_buffer.resize(m_number_lines * m_characters_per_line);
    }

    std::string m_ansi_output;

    bool m_is_screen_cleared;
    bool m_is_line_wrapping;

    int m_position;
    int m_y_position;
    int m_number_lines;             // TERM Height
    int m_characters_per_line;      // TERM Width
    int m_x_position;
    int m_max_x_position;
    int m_max_y_position;
    int m_center_ansi_output;
    int m_saved_cursor_x;
    int m_saved_cursor_y;
    int m_attribute;
    int m_saved_attribute;
    int m_saved_foreground;
    int m_saved_background;
    int m_foreground_color;
    int m_background_color;

    ScreenPixel                m_screen_pixel;
    std::vector <ScreenPixel>  m_screen_buffer;
    std::map<int, ScreenPixel> m_pull_down_options;
    std::map<int, int>         m_line_ending_map;

    // ANSI escape sequences
    enum
    {
        CURSOR_POSITION =      'H',
        CURSOR_POSITION_ALT =  'f',   // equivalent to 'H'
        CURSOR_UP =            'A',
        CURSOR_DOWN =          'B',
        CURSOR_FORWARD =       'C',
        CURSOR_BACKWARD =      'D',

        CURSOR_X_POSITION =    'G',   // XTERM
        CURSOR_NEXT_LINE =     'E',   // XTERM
        CURSOR_PREV_LIVE =     'F',   // XTERM

        SAVE_CURSOR_POS =      's',
        RESTORE_CURSOR_POS =   'u',
        ERASE_DISPLAY =        'J',   // 2J
        ERASE_TO_EOL =         'K',
        SET_GRAPHICS_MODE =    'm',
        SET_MODE =             'h',  // Line Wraparound ?7h
        RESET_MODE =           'l',
        SET_KEYBOARD_STRINGS = 'p',

        // Text attributes
        ALL_ATTRIBUTES_OFF = 0,
        BOLD_ON            = 1,
        UNDERSCORE         = 4,
        BLINK_ON           = 5,
        REVERSE_VIDEO_ON   = 7,
        CONCEALED_ON       = 8,

        // Foreground Colors
        FG_BLACK           = 30,
        FG_RED             = 31,
        FG_GREEN           = 32,
        FG_YELLOW          = 33,
        FG_BLUE            = 34,
        FG_MAGENTA         = 35,
        FG_CYAN            = 36,
        FG_WHITE           = 37,

        // Background Colors
        BG_BLACK           = 40,
        BG_RED             = 41,
        BG_GREEN           = 42,
        BG_YELLOW          = 43,
        BG_BLUE            = 44,
        BG_MAGENTA         = 45,
        BG_CYAN            = 46,
        BG_WHITE           = 47
    };

};

typedef std::shared_ptr<ProcessorBase> process_base_ptr;

#endif // PROCESSOR_BASE_HPP
