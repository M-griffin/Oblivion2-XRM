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
        
        // Method Call to Test
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
        
        // Method Call to Test
        text.parseTextToBuffer((char *)"\x1b[?7h");
        CHECK(text.m_is_line_wrapping);
    }
    
    TEST(parseText_LineEnding_InitialText_Entry)
    {        
        const int LINE_NUMBER = 1;
        
        // Make a small box easier to test wrapping
        ProcessorText text(10,10);
        
        // Method Call to Test
        text.parseTextToBuffer((char *)"ABC1234");
     
        // Line Number Array is 1 Based! for actual lines.
        std::map<int, int> line_ending_map = text.getLineEndingMap();
                
        // m_line_ending_map stores on last positions, not actual data.
        CHECK(line_ending_map[LINE_NUMBER] == 8);
    }
    
    TEST(parseText_LineEnding_InitialText_Entry_With_Backspace)
    {        
        const int LINE_NUMBER = 1;
        
        // Make a small box easier to test wrapping
        ProcessorText text(10,10);
        
        // Method Call to Test
        text.parseTextToBuffer((char *)"ABC1234\b");
     
        // Line Number Array is 1 Based! for actual lines.
        std::map<int, int> line_ending_map = text.getLineEndingMap();       
        
        CHECK(line_ending_map[LINE_NUMBER] == 7);
    }
    
    TEST(parseText_LineEnding_InitialText_Entry_Multiple_Lines_With_Backspace)
    {        
        const int LINE_NUMBER = 1;
        const int NEXT_LINE_NUMBER = 2;
        
        // Make a small box easier to test wrapping
        ProcessorText text(10,10);
        
        // Method Call to Test
        text.parseTextToBuffer((char *)"ABC1234\n");
        text.parseTextToBuffer((char *)"ABC123456\b\b");
     
        // Line Number Array is 1 Based! for actual lines.
        std::map<int, int> line_ending_map = text.getLineEndingMap();
        
        CHECK(line_ending_map[LINE_NUMBER] == 8);
        CHECK(line_ending_map[NEXT_LINE_NUMBER] == 8);
    }
    
    TEST(parseText_LineEnding_InitialText_Entry_Multiple_Lines_With_Backspace_MoveUp)
    {        
        const int LINE_NUMBER = 1;
        const int NEXT_LINE_NUMBER = 2;
        
        // Make a small box easier to test wrapping
        ProcessorText text(10,10);
        
        // Method Call to Test
        text.parseTextToBuffer((char *)"ABC1234\n");
        text.parseTextToBuffer((char *)"ABC123456\b\b\b\b\b\b\b\b\b\b");
     
        // Line Number Array is 1 Based! for actual lines.
        std::map<int, int> line_ending_map = text.getLineEndingMap();
        
        CHECK(line_ending_map[LINE_NUMBER] == 8);
        CHECK(line_ending_map[NEXT_LINE_NUMBER] == 0);
        CHECK(text.getCurrentLine() == LINE_NUMBER);
    }
    
    TEST(parseText_LineEnding_Multiple_Lines_With_Backspace_MoveUp_CheckEndOfLine)
    {        
        const int LINE_NUMBER = 1;
        const int NEXT_LINE_NUMBER = 2;
        
        // Make a small box easier to test wrapping
        ProcessorText text(10,10);
        
        // Method Call to Test
        text.parseTextToBuffer((char *)"ABC1234\n");
        text.parseTextToBuffer((char *)"ABC123456\b\b\b\b\b\b\b\b\b\b");
        text.parseTextToBuffer((char *)"ABC123456\b\b\b\b\b\b\b\b\b\b");
     
        // Line Number Array is 1 Based! for actual lines.
        std::map<int, int> line_ending_map = text.getLineEndingMap();

        CHECK(line_ending_map[LINE_NUMBER] == 7);                       
        CHECK(line_ending_map[NEXT_LINE_NUMBER] == 0);
        CHECK(line_ending_map[NEXT_LINE_NUMBER+1] == 0);
        CHECK(text.getCurrentLine() == LINE_NUMBER);        
        CHECK(text.m_x_position = line_ending_map[LINE_NUMBER]);
        CHECK(text.m_y_position = LINE_NUMBER);
        CHECK(text.m_line_number = LINE_NUMBER);
    }
    
    // Extra Testing.. make sure if we're properly tracking X_position and line_ending_map[LINE_NUMBER]
    // Which should be tracking the last or greatest X_Position per line
    TEST(parseText_LineEnding_And_XPoisiton_1)
    {        
        const int LINE_NUMBER = 1;
        
        // Make a small box easier to test wrapping
        ProcessorText text(10,10);
        
        // Method Call to Test
        text.parseTextToBuffer((char *)"A");
     
        // Line Number Array is 1 Based! for actual lines.
        std::map<int, int> line_ending_map = text.getLineEndingMap();

        CHECK(line_ending_map[LINE_NUMBER] == 2);              
        CHECK(text.getCurrentLine() == LINE_NUMBER);        
        CHECK(text.m_x_position = line_ending_map[LINE_NUMBER]);
    }
    
    // Extra Testing.. make sure if we're properly tracking X_position and line_ending_map[LINE_NUMBER]
    // Which should be tracking the last or greatest X_Position per line
    TEST(parseText_LineEnding_And_XPoisiton_2)
    {        
        const int LINE_NUMBER = 1;
        
        // Make a small box easier to test wrapping
        ProcessorText text(10,10);
        
        // Method Call to Test
        text.parseTextToBuffer((char *)"AB");
     
        // Line Number Array is 1 Based! for actual lines.
        std::map<int, int> line_ending_map = text.getLineEndingMap();
        
        std::cout << "lem[LINE_NUMBER]: " << line_ending_map[LINE_NUMBER] << ", ln: " << text.m_line_number << std::endl;
        
        CHECK(line_ending_map[LINE_NUMBER] == 3);        
        CHECK(text.getCurrentLine() == LINE_NUMBER);        
        CHECK(text.m_x_position = line_ending_map[LINE_NUMBER]);
    }
    
    
    
    // WIP Not wrapping to next line when just entering chars!
    TEST(parseText_LineEnding_InitialText_Entry_Multiple_Lines_Wrap_MoveDn)
    {        
        const int LINE_NUMBER = 1;
        const int NEXT_LINE_NUMBER = 2;
        
        // Make a small box easier to test wrapping
        // A-7 Should be Line 1, where 89 are wrapped to line 2
        ProcessorText text(10,10);
        
        // Method Call to Test
        text.parseTextToBuffer((char *)"ABC123456789");

     
        // Line Number Array is 1 Based! for actual lines.
        std::map<int, int> line_ending_map = text.getLineEndingMap();
               
        CHECK(line_ending_map[LINE_NUMBER] == 10);
        // 3 is the position after wrapping (2) letters.
        CHECK(line_ending_map[NEXT_LINE_NUMBER] == 3);
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
            // Method Call to Test
            text.parseTextToBuffer((char *)&c);            
        }

     
        // Line Number Array is 1 Based! for actual lines.
        std::map<int, int> line_ending_map = text.getLineEndingMap();
        
        
        CHECK(line_ending_map[LINE_NUMBER] == 10);
        // 3 is the position after wrapping (2) letters.
        CHECK(line_ending_map[NEXT_LINE_NUMBER] == 3);
        CHECK(text.getCurrentLine() == NEXT_LINE_NUMBER);
    }
    
    // WIP Not wrapping to next line when just entering chars!
    // Same as above but pushing through one char at a time vs. String.
    TEST(parseText_LineEnding_Text_Entry_Loop_Chars_Wrap_MoveDn_2)
    {        
        const int LINE_NUMBER = 1;
        const int NEXT_LINE_NUMBER = 2;
        
        // Make a small box easier to test wrapping
        // A-7 Should be Line 1, where 89 are wrapped to line 2
        ProcessorText text(10,10);
        
        std::string new_string("ABC123456789");
        
        for (int c : new_string) {
            // Method Call to Test
            text.parseTextToBuffer((char *)&c);            
        }

     
        // Line Number Array is 1 Based! for actual lines.
        std::map<int, int> line_ending_map = text.getLineEndingMap();
        
        
        CHECK(line_ending_map[LINE_NUMBER] == 10);
        // 3 is the position after wrapping (2) letters.
        CHECK(line_ending_map[NEXT_LINE_NUMBER] == 3);
        CHECK(text.getCurrentLine() == NEXT_LINE_NUMBER);
    }
    
    TEST(base_clearScreen) 
    {
        // Make a small box easier to test wrapping
        // A-7 Should be Line 1, where 89 are wrapped to line 2
        ProcessorText text(10,10);
        // Method Call to Test
        text.clearScreen();
        
        const int LINE_NUMBER = 1;
        CHECK(text.getCurrentLine() == LINE_NUMBER);
        CHECK(text.getMaxLines());
        CHECK(text.getYPosition() == 1);
        CHECK(text.getXPosition() == 1);
        CHECK(text.getMaxCharactersPerLine() == 10);
        CHECK(text.m_max_y_position == 1);
        CHECK(text.getLineEndingMap().size() == 0);
        
    }
    
    TEST(base_moveHomePosition) 
    {
        // Make a small box easier to test wrapping
        // A-7 Should be Line 1, where 89 are wrapped to line 2
        ProcessorText text(10,10);
        
        const int LINE_NUMBER = 1;
        // Method Call to Test
        text.moveHomePosition();
        
        CHECK(text.getCurrentLine() == LINE_NUMBER);
        CHECK(text.getYPosition() == 1);
        CHECK(text.getXPosition() == 1);
    }
    
    TEST(base_moveEndPoisiton) 
    {
        // Make a small box easier to test wrapping
        // A-7 Should be Line 1, where 89 are wrapped to line 2
        ProcessorText text(10,10);
        // Method Call to Test
        text.moveEndPosition();
        
        const int LINE_NUMBER = 1;
        CHECK(text.getCurrentLine() == LINE_NUMBER);
        CHECK(text.getYPosition() == 1);
        CHECK(text.getXPosition() == text.getMaxCharactersPerLine());
        CHECK(text.getMaxCharactersPerLine() == 10);
    }
    
    TEST(base_moveEndPoisiton_AfterBackSpace) 
    {
        // Make a small box easier to test wrapping
        // A-7 Should be Line 1, where 89 are wrapped to line 2
        ProcessorText text(10,10);
        
        // Move 5 Positions forward with real data.
        text.parseTextToBuffer((char *)"12345");
        
        // Move down, keep same X Position
        text.moveNextYPosition();
        
        // Backspace would move back one x position
        text.parseTextToBuffer((char *)"\b");
        
        // Move to end of Line, with no real text data on current line
        // We should move to last position in box. - NOT where BS was hit.
        text.moveEndPosition();
        
        // Here we want to use arror key movements, then move to end of Box
        // Make Sure Backspace when no real chars are entered cause it to move short of the end of the screen.
        //const int LINE_NUMBER = 2;
        //CHECK(text.getCurrentLine() == LINE_NUMBER);
        CHECK(text.getYPosition() == 2);
        
        // Main check is EndPosition should match end of Screen
        CHECK(text.getXPosition() == text.getMaxCharactersPerLine());
        
        // Line Ending Map should be 0, without any real test data entered yet.
        CHECK(text.getLineEndingMap().at(2) == 0);
        CHECK(text.getMaxCharactersPerLine() == 10);
    }
    
    TEST(base_movePreviousXPosition) 
    {
        // Make a small box easier to test wrapping
        // A-7 Should be Line 1, where 89 are wrapped to line 2
        ProcessorText text(10,10);
        
        // Setup Test
        text.m_x_position = 5;
        
        // Method Call to Test
        text.movePreviousXPosition();
        
        const int LINE_NUMBER = 1;
        CHECK(text.getCurrentLine() == LINE_NUMBER);
        CHECK(text.getYPosition() == 1);
        CHECK(text.getXPosition() == 4);
        CHECK(text.getMaxCharactersPerLine() == 10);
    }
    
    TEST(base_movePreviousXPosition_AtbegininngOfLine_NoChange) 
    {
        // Make a small box easier to test wrapping
        // A-7 Should be Line 1, where 89 are wrapped to line 2
        ProcessorText text(10,10);
        
        // Setup Test
        text.m_x_position = 1;
        
        // Method Call to Test
        text.movePreviousXPosition();
        
        const int LINE_NUMBER = 1;
        CHECK(text.getCurrentLine() == LINE_NUMBER);
        CHECK(text.getYPosition() == 1);
        CHECK(text.getXPosition() == 1);
        CHECK(text.getMaxCharactersPerLine() == 10);
    }
        
    TEST(base_movePreviousYPosition) 
    {
        // Make a small box easier to test wrapping
        // A-7 Should be Line 1, where 89 are wrapped to line 2
        ProcessorText text(10,10);
        
        // Setup Test
        text.m_y_position = 5;
        
        // Method Call to Test
        text.movePreviousYPosition();
        
        // TODO Line Number not changed at this time.
        //const int LINE_NUMBER = 1;
        //CHECK(text.getCurrentLine() == LINE_NUMBER);
        CHECK(text.getYPosition() == 4);
        CHECK(text.getXPosition() == 1);
        CHECK(text.getMaxCharactersPerLine() == 10);
    }
    
    TEST(base_movePreviousYPosition_AtTopOfBox_NoChange) 
    {
        // Make a small box easier to test wrapping
        // A-7 Should be Line 1, where 89 are wrapped to line 2
        ProcessorText text(10,10);
        
        // Setup Test
        text.m_y_position = 1;
        
        // Method Call to Test
        text.movePreviousYPosition();
        
        // TODO Line Number not changed at this time.
        //const int LINE_NUMBER = 1;
        //CHECK(text.getCurrentLine() == LINE_NUMBER);
        CHECK(text.getYPosition() == 1);
        CHECK(text.getXPosition() == 1);
        CHECK(text.getMaxCharactersPerLine() == 10);
    }
    
    // moveNewLine
    
    TEST(base_moveNewLine) 
    {
        // Make a small box easier to test wrapping
        // A-7 Should be Line 1, where 89 are wrapped to line 2
        ProcessorText text(10,10);
        
        // Method Call to Test
        text.moveNewLine();
        
        // Line Number should match the actual depth
        const int LINE_NUMBER = 2;
        CHECK(text.getCurrentLine() == LINE_NUMBER);
        
        // Y Position sticks with text as long as it's visable.
        CHECK(text.getYPosition() == 2);
        CHECK(text.getXPosition() == 1);
        CHECK(text.getMaxCharactersPerLine() == 10);
    }
    
    TEST(base_moveNewLine_BottomOfBox_NoYChange) 
    {
        // Make a small box easier to test wrapping
        // A-7 Should be Line 1, where 89 are wrapped to line 2
        ProcessorText text(5,5);
                
        // Method Call to Test
        text.moveNewLine();
        text.moveNewLine();
        text.moveNewLine();
        text.moveNewLine();
        text.moveNewLine();
        
        // Line Number should match the actual depth
        const int LINE_NUMBER = 6;
        CHECK(text.getCurrentLine() == LINE_NUMBER);
        
        // Y Should be at the bottom of the box and never go past.
        CHECK(text.getYPosition() == 5); 
        CHECK(text.getXPosition() == 1);
        CHECK(text.getMaxCharactersPerLine() == 5);
        CHECK(text.m_max_y_position == 5);
    }
    
    
    // m_line_ending_map stores on last positions, not actual data.
    //std::cout << "LEM BS: " << line_ending_map[LINE_NUMBER] << ", LineNum: " << text.getCurrentLine() << std::endl;
    //std::cout << "LEM BS: " << line_ending_map[NEXT_LINE_NUMBER] << std::endl;

}
