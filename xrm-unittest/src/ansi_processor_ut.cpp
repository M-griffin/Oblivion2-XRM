/**
 * @brief Oblivion/2 XRM Unit Tests for CommonIO.
 * @return
 */

#include "processor_ansi.hpp"

#include <UnitTest++.h>

// C++ Standard
#include <iostream>
#include <string>
#include <stdexcept>

/**
 * @brief Test Suit for CommonIO Class.
 * @return
 */
SUITE(XRMProcessorAnsi)
{
    // Test the Program Path is being read correctly.
    TEST(parseAnsi_Empty_Verify_Startup_Defaults)
    {
        ProcessorAnsi ansi(25,80);
        ansi.parseTextToBuffer((char *)"");
        // check screen size!
        CHECK(ansi.m_attribute == 0);
        CHECK(ansi.m_foreground_color == ProcessorAnsi::FG_WHITE);
        CHECK(ansi.m_background_color == ProcessorAnsi::BG_BLACK);
        CHECK(ansi.m_is_screen_cleared == false);
        CHECK(ansi.m_number_lines == 25);
        CHECK(ansi.m_characters_per_line == 80);
        CHECK(ansi.m_x_position == 1);
        CHECK(ansi.m_y_position == 1);
    }

    // Test the Program Path is being read correctly.
    TEST(parseAnsi_Normal_Text)
    {
        ProcessorAnsi ansi(25,80);
        ansi.parseTextToBuffer((char *)"ABC1234");
        CHECK(ansi.m_screen_buffer[0].char_sequence == "A");
        CHECK(ansi.m_screen_buffer[6].char_sequence == "4");

    }

    // Test the Program Path is being read correctly.
    TEST(parseAnsi_Test_Line_Wraping)
    {
        ProcessorAnsi ansi(25,80);
        ansi.parseTextToBuffer((char *)"\x1b[?7h");
        CHECK(ansi.m_is_line_wrapping);
    }
}
