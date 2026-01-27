#ifndef PROCESSOR_BASE_HPP
#define PROCESSOR_BASE_HPP

#include <string>

#include "logging.hpp"

/**
 * @class ScreenBase
 * @author Michael Griffin
 * @date 01/12/2019
 * @file screen_base.hpp
 * @brief Base class for text processors
 */
class ScreenBase {
public:
    ScreenBase(int term_height, int term_width);

    Logging &m_log;
    std::string m_ansi_output;

    bool m_is_screen_cleared;
    bool m_is_line_wrapping;

    int m_position;
    int m_y_position;
    int m_number_lines; // TERM Height
    int m_characters_per_line; // TERM Width
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

    // ANSI escape sequences
    enum {
        CURSOR_POSITION = 'H',
        CURSOR_POSITION_ALT = 'f', // equivalent to 'H'
        CURSOR_UP = 'A',
        CURSOR_DOWN = 'B',
        CURSOR_FORWARD = 'C',
        CURSOR_BACKWARD = 'D',

        CURSOR_X_POSITION = 'G', // XTERM
        CURSOR_NEXT_LINE = 'E', // XTERM
        CURSOR_PREV_LIVE = 'F', // XTERM

        SAVE_CURSOR_POS = 's',
        RESTORE_CURSOR_POS = 'u',
        ERASE_DISPLAY = 'J', // 2J
        ERASE_TO_EOL = 'K',
        SET_GRAPHICS_MODE = 'm',
        SET_MODE = 'h', // Line Wraparound ?7h
        RESET_MODE = 'l',
        SET_KEYBOARD_STRINGS = 'p',

        // Text attributes
        ALL_ATTRIBUTES_OFF = 0,
        BOLD_ON = 1,
        UNDERSCORE = 4,
        BLINK_ON = 5,
        REVERSE_VIDEO_ON = 7,
        CONCEALED_ON = 8,

        // Standard Foreground Colors
        FG_BLACK = 30,
        FG_RED = 31,
        FG_GREEN = 32,
        FG_YELLOW = 33,
        FG_BLUE = 34,
        FG_MAGENTA = 35,
        FG_CYAN = 36,
        FG_WHITE = 37,
        FG_DEFAULT = 38,

        // high intensity colors
        FGH_BLACK = 90,
        FGH_RED = 91,
        FGH_GREEN = 92,
        FGH_YELLOW = 93,
        FGH_BLUE = 94,
        FGH_MAGENTA = 95,
        FGH_CYAN = 96,
        FGH_WHITE = 97,

        // Background Colors
        BG_BLACK = 40,
        BG_RED = 41,
        BG_GREEN = 42,
        BG_YELLOW = 43,
        BG_BLUE = 44,
        BG_MAGENTA = 45,
        BG_CYAN = 46,
        BG_WHITE = 47
    };

    // Copy Constructors
    ScreenBase(const ScreenBase &) = delete;
    ScreenBase &operator=(const ScreenBase &) = delete;

    // Move Constructor
    ScreenBase(ScreenBase &&other) noexcept
        : m_log(other.m_log) // references are copied
          , m_ansi_output(std::move(other.m_ansi_output))
          , m_is_screen_cleared(other.m_is_screen_cleared)
          , m_is_line_wrapping(other.m_is_line_wrapping)
          , m_position(other.m_position)
          , m_y_position(other.m_y_position)
          , m_number_lines(other.m_number_lines)
          , m_characters_per_line(other.m_characters_per_line)
          , m_x_position(other.m_x_position)
          , m_max_x_position(other.m_max_x_position)
          , m_max_y_position(other.m_max_y_position)
          , m_center_ansi_output(other.m_center_ansi_output)
          , m_saved_cursor_x(other.m_saved_cursor_x)
          , m_saved_cursor_y(other.m_saved_cursor_y)
          , m_attribute(other.m_attribute)
          , m_saved_attribute(other.m_saved_attribute)
          , m_saved_foreground(other.m_saved_foreground)
          , m_saved_background(other.m_saved_background)
          , m_foreground_color(other.m_foreground_color)
          , m_background_color(other.m_background_color) {
    }

    ScreenBase &operator=(ScreenBase &&other) noexcept {
        if (this != &other) {
            // Create a NEW Logging instance
            m_ansi_output = std::move(other.m_ansi_output);
            m_is_screen_cleared = other.m_is_screen_cleared;
            m_is_line_wrapping = other.m_is_line_wrapping;
            m_position = other.m_position;
            m_y_position = other.m_y_position;
            m_number_lines = other.m_number_lines;
            m_characters_per_line = other.m_characters_per_line;
            m_x_position = other.m_x_position;
            m_max_x_position = other.m_max_x_position;
            m_max_y_position = other.m_max_y_position;
            m_center_ansi_output = other.m_center_ansi_output;
            m_saved_cursor_x = other.m_saved_cursor_x;
            m_saved_cursor_y = other.m_saved_cursor_y;
            m_attribute = other.m_attribute;
            m_saved_attribute = other.m_saved_attribute;
            m_saved_foreground = other.m_saved_foreground;
            m_saved_background = other.m_saved_background;
            m_foreground_color = other.m_foreground_color;
            m_background_color = other.m_background_color;
        }
        return *this;
    }
};

#endif
