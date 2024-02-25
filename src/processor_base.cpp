#include "processor_base.hpp"

#include <string>
#include <vector>
#include <map>

#include "logging.hpp"

/**
 * @brief ProcessorBase Constructor
 * @param term_height
 * @param term_width
 * @return
 */
ProcessorBase::ProcessorBase(int term_height, int term_width)
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
    , m_saved_foreground(FG_WHITE)
    , m_saved_background(BG_BLACK)
    , m_foreground_color(FG_WHITE)
    , m_background_color(BG_BLACK)
{

}


// Helpers
int ProcessorBase::getYPosition() const
{
    return m_y_position;
}

int ProcessorBase::getXPosition() const
{
    return m_x_position;
}

int ProcessorBase::getMaxLines() const
{
    return m_number_lines;
}

