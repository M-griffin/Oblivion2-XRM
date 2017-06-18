#ifndef ANSI_PROCESSOR_HPP
#define ANSI_PROCESSOR_HPP

#include <boost/smart_ptr/shared_ptr.hpp>

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
    unsigned char c;
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
 * @class AnsiProcessor
 * @author Michael Griffin
 * @date 10/8/2015
 * @file ansi_processor.hpp
 * @brief Processes Screen data into a Screen Buffer
 */
class AnsiProcessor
{
public:
    AnsiProcessor(int term_height, int term_width);
    ~AnsiProcessor();

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

    ScreenPixel m_screen_pixel;
    std::vector <ScreenPixel> m_screen_buffer;
    std::map<int, ScreenPixel> m_pull_down_options;

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

    // Process Build strings from pull_down array.
    std::string buildPullDownBars(int pulldown_id, bool active); // 4

    // Clear Pull Down Bars once menu options are reset.
    void clearPullDownBars();

    // Return the max rows used on the screen
    int getMaxRowsUsedOnScreen();

    // Parses the Screen Buffer and Builds pulldown options
    std::string screenBufferParse();          // 3

    // Converts the Screen Buffer to a single string for parsing.
    std::string screenBufferToString();       // 2

    // For testing display the output of the buffer.
    void screenBufferDisplayTest();

    /**
     * @brief Takes buffer and displays parsed sequences
     */
    std::string getScreenFromBuffer(bool clearScreen);


    // Screen Buffer Modifiers
    void screenBufferSetPixel(char c);
    void screenBufferScrollUp();
    void screenBufferClearRange(int start, int end);
    void screenBufferClear();

    /**
     * @brief Clears The Screen And Buffer
     */
    void clearScreen();

    // Process ANSI screen inserts into the Screen Buffer
    void parseAnsiScreen(char *buff);  // 1
    
    
    // Helpers
    int getYPosition() const { return m_y_position; }
    int getXPosition() const { return m_x_position; }

};

typedef boost::shared_ptr<AnsiProcessor> ansi_process_ptr;

#endif // ANSI_PROCESSOR_HPP
