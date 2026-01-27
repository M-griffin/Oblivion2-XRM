#include "screen_base.hpp"

#include <string>
#include "logging.hpp"

/**
 * @brief ProcessorBase Constructor
 * @param term_height
 * @param term_width
 * @return
 */
ScreenBase::ScreenBase(const int term_height, const int term_width)
    : m_log(Logging::getInstance())
      , m_ansi_output("")
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
      , m_saved_foreground(FG_DEFAULT)
      , m_saved_background(BG_BLACK)
      , m_foreground_color(FG_DEFAULT)
      , m_background_color(BG_BLACK) {
}
