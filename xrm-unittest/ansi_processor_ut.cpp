/**
 * @brief Oblivion/2 XRM Unit Tests for CommonIO.
 * @return
 */

#include "ansi_processor.hpp"

#include <UnitTest++.h>

// C++ Standard
#include <iostream>
#include <string>
#include <stdexcept>

/**
 * @brief Test Suit for CommonIO Class.
 * @return
 */
SUITE(XRMAnsiProcessor)
{
    // Test the Program Path is being read correctly.
    TEST(parseAnsi_Empty_Verify_Startup_Defaults)
    {
        AnsiProcessor ansi(25,80);
        ansi.parseAnsiScreen((char *)"");
        // check screen size!
        CHECK(ansi.m_attribute == 0);
        CHECK(ansi.m_foreground_color == AnsiProcessor::FG_WHITE);
        CHECK(ansi.m_background_color == AnsiProcessor::BG_BLACK);
        CHECK(ansi.m_is_screen_cleared == false);
        CHECK(ansi.m_number_lines == 25);
        CHECK(ansi.m_characters_per_line == 80);
        CHECK(ansi.m_x_position == 1);
        CHECK(ansi.m_y_position == 1);
    }

    // Test the Program Path is being read correctly.
    TEST(parseAnsi_Normal_Text)
    {
        AnsiProcessor ansi(25,80);
        ansi.parseAnsiScreen((char *)"ABC1234");
        CHECK(ansi.m_screen_buffer[0].c == 'A');
        CHECK(ansi.m_screen_buffer[6].c == '4');

    }

    // Test the Program Path is being read correctly.
    TEST(parseAnsi_Test_Line_Wraping)
    {
        AnsiProcessor ansi(25,80);
        ansi.parseAnsiScreen((char *)"\x1b[?7h");
        CHECK(ansi.m_is_line_wrapping);
    }
}
