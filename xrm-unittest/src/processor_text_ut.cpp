/**
 * @brief Oblivion/2 XRM Unit Tests for CommonIO.
 * @return
 */

#include "processor_text.hpp"

#include <UnitTest++.h>

// C++ Standard
#include <iostream>
#include <string>
#include <stdexcept>
#include <map>

/**
 * @brief Test Suit for Message Editor Processor Text Class.
 * 
 * This differs from the Screen Processor Ansi Class in that the Ansi class
 * is more for the users display and full screen.  Where as the Text processor
 * Is designed more for text input along with placement on the screen.
 * Instead of using the screen_buffer that ansi uses, the text editor will
 * use a line map that traces each line written to in an editor
 * 
 * @return
 */
SUITE(XRMProcessorText)
{
    TEST(parseText_Empty_Verify_Startup_Defaults)
    {
        // Make a small box easier to test wrapping
        ProcessorText text(10,10);
        text.parseTextToBuffer((char *)"");
        
        // check screen size!
        CHECK(text.m_attribute == 0);
        CHECK(text.m_foreground_color == ProcessorText::FG_WHITE);
        CHECK(text.m_background_color == ProcessorText::BG_BLACK);
        CHECK(text.m_is_screen_cleared == false);
        CHECK(text.m_number_lines == 10);
        CHECK(text.m_characters_per_line == 10);
        CHECK(text.m_x_position == 1);
        CHECK(text.m_y_position == 1);
        CHECK(text.getCurrentLine() == 1);
    }

    TEST(parseText_Test_Line_Wrapping)
    {
        // Make a small box easier to test wrapping
        ProcessorText text(10,10);
        text.parseTextToBuffer((char *)"\x1b[?7h");
        CHECK(text.m_is_line_wrapping);
    }
    
    TEST(parseText_LineEnding_InitialText_Entry)
    {        
        const int LINE_NUMBER = 1;
        
        // Make a small box easier to test wrapping
        ProcessorText text(10,10);
        text.parseTextToBuffer((char *)"ABC1234");
     
        // Line Number Array is 1 Based! for actual lines.
        std::map<int, int> line_ending_map = text.getLineEndingMap();
        
        // m_line_ending_map stores on last positions, not actual data.
        CHECK(line_ending_map[LINE_NUMBER] == 7);
    }
    
    TEST(parseText_LineEnding_InitialText_Entry_With_Backspace)
    {        
        const int LINE_NUMBER = 1;
        
        // Make a small box easier to test wrapping
        ProcessorText text(10,10);
        text.parseTextToBuffer((char *)"ABC1234\b");
     
        // Line Number Array is 1 Based! for actual lines.
        std::map<int, int> line_ending_map = text.getLineEndingMap();
        
        CHECK(line_ending_map[LINE_NUMBER] == 6);
    }
    
    TEST(parseText_LineEnding_InitialText_Entry_Multiple_Lines_With_Backspace)
    {        
        const int LINE_NUMBER = 1;
        const int NEXT_LINE_NUMBER = 2;
        
        // Make a small box easier to test wrapping
        ProcessorText text(10,10);
        text.parseTextToBuffer((char *)"ABC1234\n");
        text.parseTextToBuffer((char *)"ABC123456\b\b");
     
        // Line Number Array is 1 Based! for actual lines.
        std::map<int, int> line_ending_map = text.getLineEndingMap();
                
        CHECK(line_ending_map[LINE_NUMBER] == 7);
        CHECK(line_ending_map[NEXT_LINE_NUMBER] == 7);
    }
    
    TEST(parseText_LineEnding_InitialText_Entry_Multiple_Lines_With_Backspace_MoveUp)
    {        
        const int LINE_NUMBER = 1;
        const int NEXT_LINE_NUMBER = 2;
        
        // Make a small box easier to test wrapping
        ProcessorText text(10,10);
        text.parseTextToBuffer((char *)"ABC1234\n");
        text.parseTextToBuffer((char *)"ABC123456\b\b\b\b\b\b\b\b\b\b");
     
        // Line Number Array is 1 Based! for actual lines.
        std::map<int, int> line_ending_map = text.getLineEndingMap();
        
        CHECK(line_ending_map[LINE_NUMBER] == 7);
        CHECK(line_ending_map[NEXT_LINE_NUMBER] == 0);
        CHECK(text.getCurrentLine() == LINE_NUMBER);
    }
    
    TEST(parseText_LineEnding_Multiple_Lines_With_Backspace_MoveUp_CheckEndOfLine)
    {        
        const int LINE_NUMBER = 1;
        const int NEXT_LINE_NUMBER = 2;
        
        // Make a small box easier to test wrapping
        ProcessorText text(10,10);
        text.parseTextToBuffer((char *)"ABC1234\n");
        text.parseTextToBuffer((char *)"ABC123456\b\b\b\b\b\b\b\b\b\b");
        text.parseTextToBuffer((char *)"ABC123456\b\b\b\b\b\b\b\b\b\b");
     
        // Line Number Array is 1 Based! for actual lines.
        std::map<int, int> line_ending_map = text.getLineEndingMap();
        
        std::cout << "lem[LINE_NUMBER]: " << line_ending_map[LINE_NUMBER] << ", ln: " << text.m_line_number << std::endl;
        
        CHECK(line_ending_map[LINE_NUMBER] == 6);
        
        std::cout << "lem[NEXT_LINE_NUMBER]: " << line_ending_map[NEXT_LINE_NUMBER] << std::endl;
        std::cout << "lem[NEXT_LINE_NUMBER+1]: " << line_ending_map[NEXT_LINE_NUMBER+1] << std::endl;
        
        std::cout << "m_characters_per_line: " << text.m_characters_per_line << std::endl;
        std::cout << "getMaxLines(): " << text.getMaxLines() << std::endl;
        std::cout << "getMaxRowsUsedOnScreen(): " << text.getMaxRowsUsedOnScreen() << std::endl;
        
        CHECK(line_ending_map[NEXT_LINE_NUMBER] == 0);
        CHECK(line_ending_map[NEXT_LINE_NUMBER+1] == 0);
        CHECK(text.getCurrentLine() == LINE_NUMBER);
        
        CHECK(text.m_x_position = line_ending_map[LINE_NUMBER]);
        CHECK(text.m_y_position = LINE_NUMBER);
        CHECK(text.m_line_number = LINE_NUMBER);
    }
    
    // WIP Not wrapping to next line when just entering chars!
    /*
    TEST(parseText_LineEnding_InitialText_Entry_Multiple_Lines_Wrap_MoveDn)
    {        
        const int LINE_NUMBER = 1;
        const int NEXT_LINE_NUMBER = 2;
        
        // Make a small box easier to test wrapping
        // A-7 Should be Line 1, where 89 are wrapped to line 2
        ProcessorText text(10,10);
        text.parseTextToBuffer((char *)"ABC123456789");

     
        // Line Number Array is 1 Based! for actual lines.
        std::map<int, int> line_ending_map = text.getLineEndingMap();
        
        // m_line_ending_map stores on last positions, not actual data.
        std::cout << "LEM BS: " << line_ending_map[LINE_NUMBER] << ", LineNum: " << text.getCurrentLine() << std::endl;
        std::cout << "LEM BS: " << line_ending_map[NEXT_LINE_NUMBER] << std::endl;
        
        CHECK(line_ending_map[LINE_NUMBER] == 10);
        CHECK(line_ending_map[NEXT_LINE_NUMBER] == 2);
        CHECK(text.getCurrentLine() == NEXT_LINE_NUMBER);
    }
    
    // WIP Not wrapping to next line when just entering chars!
    // Same as above but pushing through one char at a time vs. String.
    TEST(parseText_LineEnding_Text_Entry_Loop_Chars_Wrap_MoveDn)
    {        
        const int LINE_NUMBER = 1;
        const int NEXT_LINE_NUMBER = 2;
        
        // Make a small box easier to test wrapping
        // A-7 Should be Line 1, where 89 are wrapped to line 2
        ProcessorText text(10,10);
        
        std::string new_string("ABC123456789");
        
        for (int c : new_string) {
            text.parseTextToBuffer((char *)&c);            
        }

     
        // Line Number Array is 1 Based! for actual lines.
        std::map<int, int> line_ending_map = text.getLineEndingMap();
        
        // m_line_ending_map stores on last positions, not actual data.
        std::cout << "LEM BS: " << line_ending_map[LINE_NUMBER] << ", LineNum: " << text.getCurrentLine() << std::endl;
        std::cout << "LEM BS: " << line_ending_map[NEXT_LINE_NUMBER] << std::endl;
        
        CHECK(line_ending_map[LINE_NUMBER] == 10);
        CHECK(line_ending_map[NEXT_LINE_NUMBER] == 2);
        CHECK(text.getCurrentLine() == NEXT_LINE_NUMBER);
    }*/
    
}
