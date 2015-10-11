/**
 * @brief Oblivion/2 XRM Unit Tests for SessionIO.
 * @return
 */

#include "session_io.hpp"

#ifdef _WIN32
#include <UnitTest++.h>
#else
#include <unittest++/UnitTest++.h>
#endif

// C++ Standard
#include <iostream>
#include <string>
#include <stdexcept>


SUITE(XRMSessionIO)
{
    // We Can only test some of the inital setup here.
    // The Rest of the functions all rely on STDIO
    // So this is more integration Testing.
    // Can be overide functions and pass basic input!  Might do this!!

    // Set Term to not echo and coan
/*
    TEST_FIXTURE(StandardIO, InitStdio)
    {
        bool result = InitConsoleTTY();
        CHECK_EQUAL(result, true);
    }
*/

    /**
     * @brief session_data is a shared_ptr that is passed along to other instances
     *        session_data also keeps track of
     * @return
     */


     /**
     * @brief Input Key Tetes, Receives Characters 1 at a time, stores and processes.
     *        Only passing through or building esc sequences.
     * @return
     */
    TEST(InputKey_Empty)
    {
        // Normal Characters and sequences should pass through
        session_data_ptr session_data;
        SessionIO sess(session_data);

        std::string character_buffer = "";
        std::string result = sess.getKeyInput(character_buffer);
        CHECK(result.compare(character_buffer) == 0);
    }

    TEST(InputKey_Normal_Character)
    {
        // Normal Characters and sequences should pass through
        session_data_ptr session_data;
        SessionIO sess(session_data);

        std::string character_buffer = "A";
        std::string result = sess.getKeyInput(character_buffer);
        CHECK(result.compare(character_buffer) == 0);
    }

    TEST(InputKey_Double_Character_Expect_Empty)
    {
        session_data_ptr session_data;
        SessionIO sess(session_data);

        std::string character_buffer = "AA";
        std::string result = sess.getKeyInput(character_buffer);
        CHECK(result.compare("") == 0);
    }

    TEST(InputKey_UniCode_Character)
    {
        // Normal Unicode Characters and sequences should pass through
        session_data_ptr session_data;
        SessionIO sess(session_data);

        std::string character_buffer = "あ";
        std::string result = sess.getKeyInput(character_buffer);
        CHECK(result.compare(character_buffer) == 0);
    }

    TEST(InputKey_Two_UniCode_Character_Expect_Empty)
    {
        session_data_ptr session_data;
        SessionIO sess(session_data);

        std::string character_buffer = "ああ";
        std::string result = sess.getKeyInput(character_buffer);
        CHECK(result.compare("") == 0);
    }

    TEST(InputKey_Mix_Normal_And_UniCode_Character_Expect_Empty)
    {
        session_data_ptr session_data;
        SessionIO sess(session_data);

        std::string character_buffer = "あAあ";
        std::string result = sess.getKeyInput(character_buffer);
        CHECK(result.compare("") == 0);
    }

    TEST(InputKey_Mix_Normal_And_UniCode_Characters2_Expect_Empty)
    {
        session_data_ptr session_data;
        SessionIO sess(session_data);

        std::string character_buffer = "Aあ";
        std::string result = sess.getKeyInput(character_buffer);
        CHECK(result.compare("") == 0);
    }

    TEST(InputKey_Mix_Normal_And_UniCode_Character3_Expect_Empty)
    {
        session_data_ptr session_data;
        SessionIO sess(session_data);

        std::string character_buffer = "あA";
        std::string result = sess.getKeyInput(character_buffer);
        CHECK(result.compare("") == 0);
    }

    TEST(InputKey_Escape_Expect_Empty_Then_Esc)
    {
        // First ESC waits for \0 to signal it's single ESC key
        session_data_ptr session_data;
        SessionIO sess(session_data);

        std::string character_buffer = "\x1b";
        std::string result = sess.getKeyInput(character_buffer);
        CHECK((result.compare("") == 0));

        // Verify Second Sequence return the ESC to signal it's alone.
        character_buffer = "\0";
        result = sess.getKeyInput(character_buffer);
        CHECK((result.compare("\x1b") == 0));
    }

    TEST(InputKey_Escape_Sequence_Up_Arrow_Failure)
    {
        session_data_ptr session_data;
        SessionIO sess(session_data);

        std::string character_buffer = "\x1b[A";
        std::string result = sess.getKeyInput(character_buffer);
        CHECK((result.compare("") == 0));
    }

    TEST(InputKey_Escape_Sequence_Up_Arrow_Sequence)
    {
        session_data_ptr session_data;
        SessionIO sess(session_data);

        std::string character_buffer = "\x1b";
        std::string result = sess.getKeyInput(character_buffer);
        CHECK((result.compare("") == 0));

        character_buffer = "[";
        result = sess.getKeyInput(character_buffer);
        CHECK((result.compare("") == 0));

        character_buffer = "A";
        result = sess.getKeyInput(character_buffer);
        CHECK((result.compare("\x1b""up_arrow") == 0));
    }

    TEST(InputKey_Escape_Sequence_Dn_Arrow_Sequence)
    {
        session_data_ptr session_data;
        SessionIO sess(session_data);

        std::string character_buffer = "\x1b";
        std::string result = sess.getKeyInput(character_buffer);
        CHECK((result.compare("") == 0));

        character_buffer = "[";
        result = sess.getKeyInput(character_buffer);
        CHECK((result.compare("") == 0));

        character_buffer = "B";
        result = sess.getKeyInput(character_buffer);
        CHECK((result.compare("\x1b""dn_arrow") == 0));
    }

    TEST(InputKey_Escape_Sequence_Lt_Arrow_Sequence)
    {
        session_data_ptr session_data;
        SessionIO sess(session_data);

        std::string character_buffer = "\x1b";
        std::string result = sess.getKeyInput(character_buffer);
        CHECK((result.compare("") == 0));

        character_buffer = "[";
        result = sess.getKeyInput(character_buffer);
        CHECK((result.compare("") == 0));

        character_buffer = "D";
        result = sess.getKeyInput(character_buffer);
        CHECK((result.compare("\x1b""lt_arrow") == 0));
    }

    TEST(InputKey_Escape_Sequence_Rt_Arrow_Sequence)
    {
        session_data_ptr session_data;
        SessionIO sess(session_data);

        std::string character_buffer = "\x1b";
        std::string result = sess.getKeyInput(character_buffer);
        CHECK((result.compare("") == 0));

        character_buffer = "[";
        result = sess.getKeyInput(character_buffer);
        CHECK((result.compare("") == 0));

        character_buffer = "C";
        result = sess.getKeyInput(character_buffer);
        CHECK((result.compare("\x1b""rt_arrow") == 0));
    }

    TEST(InputKey_Escape_Sequence_Hardware_Up_Arrow_Sequence)
    {
        session_data_ptr session_data;
        SessionIO sess(session_data);

        std::string character_buffer = "\x1b";
        std::string result = sess.getKeyInput(character_buffer);
        CHECK((result.compare("") == 0));

        character_buffer = "O";
        result = sess.getKeyInput(character_buffer);
        CHECK((result.compare("") == 0));

        character_buffer = "A";
        result = sess.getKeyInput(character_buffer);
        CHECK((result.compare("\x1b""up_arrow") == 0));
    }

    TEST(InputKey_Escape_Sequence_Hardware_Dn_Arrow_Sequence)
    {
        session_data_ptr session_data;
        SessionIO sess(session_data);

        std::string character_buffer = "\x1b";
        std::string result = sess.getKeyInput(character_buffer);
        CHECK((result.compare("") == 0));

        character_buffer = "O";
        result = sess.getKeyInput(character_buffer);
        CHECK((result.compare("") == 0));

        character_buffer = "B";
        result = sess.getKeyInput(character_buffer);
        CHECK((result.compare("\x1b""dn_arrow") == 0));
    }

    TEST(InputKey_Escape_Sequence_Hardware_Lt_Arrow_Sequence)
    {
        session_data_ptr session_data;
        SessionIO sess(session_data);

        std::string character_buffer = "\x1b";
        std::string result = sess.getKeyInput(character_buffer);
        CHECK((result.compare("") == 0));

        character_buffer = "O";
        result = sess.getKeyInput(character_buffer);
        CHECK((result.compare("") == 0));

        character_buffer = "D";
        result = sess.getKeyInput(character_buffer);
        CHECK((result.compare("\x1b""lt_arrow") == 0));
    }

    TEST(InputKey_Escape_Sequence_Hardware_Rt_Arrow_Sequence)
    {
        session_data_ptr session_data;
        SessionIO sess(session_data);

        std::string character_buffer = "\x1b";
        std::string result = sess.getKeyInput(character_buffer);
        CHECK((result.compare("") == 0));

        character_buffer = "O";
        result = sess.getKeyInput(character_buffer);
        CHECK((result.compare("") == 0));

        character_buffer = "C";
        result = sess.getKeyInput(character_buffer);
        CHECK((result.compare("\x1b""rt_arrow") == 0));
    }

    TEST(InputKey_Escape_Sequence_Esc_Twice_Returns_Esc)
    {
        session_data_ptr session_data;
        SessionIO sess(session_data);

        std::string character_buffer = "\x1b";
        std::string result = sess.getKeyInput(character_buffer);
        CHECK((result.compare("") == 0));

        character_buffer = "\x1b";
        result = sess.getKeyInput(character_buffer);
        CHECK((result.compare("\x1b") == 0));
    }


    /**
     * @brief Create Input Field Tests
     * @return
     */
    TEST(createInputField_Empty_Returns_Empty)
    {
        session_data_ptr session_data;
        SessionIO sess(session_data);

        std::string character_buffer = "";
        int length = 0;
        sess.createInputField(character_buffer, length);
        CHECK((character_buffer.compare("") == 0));
    }

    TEST(createInputField_0_Length_Retuns_Field_Name)
    {
        session_data_ptr session_data;
        SessionIO sess(session_data);

        std::string character_buffer = "Testing";
        int length = 0;
        sess.createInputField(character_buffer, length);
        CHECK((character_buffer.compare("Testing") == 0));
    }

    TEST(createInputField_1_Length_Retuns_Field_Name_1_Length_ANSI_Field)
    {
        session_data_ptr session_data;
        SessionIO sess(session_data);

        std::string character_buffer = "Testing ";
        int length = 1;
        sess.createInputField(character_buffer, length);
        CHECK((character_buffer.compare("Testing |00|19 \x1b[2D") == 0));
    }

    TEST(createInputField_10_Length_Retuns_Field_Name_10_Length_ANSI_Field)
    {
        session_data_ptr session_data;
        SessionIO sess(session_data);

        std::string character_buffer = "Testing ";
        int length = 10;
        sess.createInputField(character_buffer, length);
        CHECK((character_buffer.compare("Testing |00|19          \x1b[11D") == 0));
    }

    TEST(createInputField_10_Length_Retuns_Field_Name_Override_Input_Length)
    {
        session_data_ptr session_data;
        SessionIO sess(session_data);

        std::string character_buffer = "Testing %IN17";
        int length = 10;
        sess.createInputField(character_buffer, length);
        CHECK((character_buffer.compare("Testing |00|19                 \x1b[18D") == 0));
    }

    TEST(createInputField_10_Length_Retuns_Field_Name_Override_Input_Length_And_Color)
    {
        session_data_ptr session_data;
        SessionIO sess(session_data);

        std::string character_buffer = "Testing %IN17%FB0116";
        int length = 10;
        sess.createInputField(character_buffer, length);
        std::cout << "\n RESULT: [" << character_buffer << "]" << std::endl;
        CHECK((character_buffer.compare("Testing |01|16                 \x1b[18D") == 0));
    }

    TEST(createInputField_10_Length_Retuns_Field_Name_Override_Color_Only)
    {
        session_data_ptr session_data;
        SessionIO sess(session_data);

        std::string character_buffer = "Testing: %FB0116";
        int length = 10;
        sess.createInputField(character_buffer, length);
        std::cout << "\n RESULT: [" << character_buffer << "]" << std::endl;
        CHECK((character_buffer.compare("Testing: |01|16          \x1b[11D") == 0));
    }




}
