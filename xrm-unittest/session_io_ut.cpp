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
        // Mock SessionData Class
        connection_ptr          connection;
        session_manager_ptr     room;
        boost::asio::io_service io_service;
        state_manager_ptr       state_manager;

        session_data_ptr session_data(new SessionData(connection, room, io_service, state_manager));
        SessionIO sess(session_data);

        std::string character_buffer = "Testing ";
        int length = 1;
        sess.createInputField(character_buffer, length);
        CHECK((character_buffer.compare("Testing |00|19 \x1b[2D") == 0));
    }

    TEST(createInputField_10_Length_Retuns_Field_Name_10_Length_ANSI_Field)
    {
        // Mock SessionData Class
        connection_ptr          connection;
        session_manager_ptr     room;
        boost::asio::io_service io_service;
        state_manager_ptr       state_manager;

        session_data_ptr session_data(new SessionData(connection, room, io_service, state_manager));
        SessionIO sess(session_data);

        std::string character_buffer = "Testing ";
        int length = 10;
        sess.createInputField(character_buffer, length);
        CHECK((character_buffer.compare("Testing |00|19          \x1b[11D") == 0));
    }

    TEST(createInputField_10_Length_Retuns_Default_Becasue_IN17_Larger_Than_length20)
    {
        // Mock SessionData Class
        connection_ptr          connection;
        session_manager_ptr     room;
        boost::asio::io_service io_service;
        state_manager_ptr       state_manager;

        session_data_ptr session_data(new SessionData(connection, room, io_service, state_manager));
        SessionIO sess(session_data);

        std::string character_buffer = "Testing %IN17";
        int length = 10;
        sess.createInputField(character_buffer, length);
        CHECK((character_buffer.compare("Testing |00|19          \x1b[11D") == 0));
    }

    TEST(createInputField_10_Length_Retuns_Default_Becasue_IN10_Larger_Than_length20)
    {
        // Mock SessionData Class
        connection_ptr          connection;
        session_manager_ptr     room;
        boost::asio::io_service io_service;
        state_manager_ptr       state_manager;

        session_data_ptr session_data(new SessionData(connection, room, io_service, state_manager));
        SessionIO sess(session_data);

        std::string character_buffer = "Testing %IN10";
        int length = 10;
        sess.createInputField(character_buffer, length);
        CHECK((character_buffer.compare("Testing |00|19          \x1b[11D") == 0));
    }

    TEST(createInputField_10_Length_Retuns_Default_Becasue_IN10_Larger_Than_length20_Failure)
    {
       // Mock SessionData Class
        connection_ptr          connection;
        session_manager_ptr     room;
        boost::asio::io_service io_service;
        state_manager_ptr       state_manager;

        session_data_ptr session_data(new SessionData(connection, room, io_service, state_manager));
        SessionIO sess(session_data);

        std::string character_buffer = "Testing %IN10";
        int length = 11;
        sess.createInputField(character_buffer, length);
        CHECK((!character_buffer.compare("Testing |00|19          \x1b[12D") == 0));
    }

    TEST(createInputField_20_Length_Retuns_Field_Name_Override_Input_Length)
    {
        // Mock SessionData Class
        connection_ptr          connection;
        session_manager_ptr     room;
        boost::asio::io_service io_service;
        state_manager_ptr       state_manager;

        session_data_ptr session_data(new SessionData(connection, room, io_service, state_manager));
        SessionIO sess(session_data);

        std::string character_buffer = "Testing %IN17";
        int length = 20;
        sess.createInputField(character_buffer, length);
        CHECK((character_buffer.compare("Testing |00|19                 \x1b[18D") == 0));
    }

    TEST(createInputField_20_Length_Retuns_Field_Name_Override_Input_Length_And_Color)
    {
        // Mock SessionData Class
        connection_ptr          connection;
        session_manager_ptr     room;
        boost::asio::io_service io_service;
        state_manager_ptr       state_manager;

        session_data_ptr session_data(new SessionData(connection, room, io_service, state_manager));
        SessionIO sess(session_data);

        std::string character_buffer = "Testing %IN17%FB0116";
        int length = 20;
        sess.createInputField(character_buffer, length);
        CHECK((character_buffer.compare("Testing |01|16                 \x1b[18D") == 0));
    }

    TEST(createInputField_10_Length_Retuns_Field_Name_Override_Color_Only)
    {
        // Mock SessionData Class
        connection_ptr          connection;
        session_manager_ptr     room;
        boost::asio::io_service io_service;
        state_manager_ptr       state_manager;

        session_data_ptr session_data(new SessionData(connection, room, io_service, state_manager));
        SessionIO sess(session_data);

        std::string character_buffer = "Testing: %FB0116";
        int length = 10;
        sess.createInputField(character_buffer, length);
        CHECK((character_buffer.compare("Testing: |01|16          \x1b[11D") == 0));
    }

    /**
     * @brief Get Input Field Tests
     * @return
     */

    TEST(getInputField_Test_Field_Return_No_LF)
    {
        // Mock SessionData Class
        connection_ptr          connection;
        session_manager_ptr     room;
        boost::asio::io_service io_service;
        state_manager_ptr       state_manager;

        session_data_ptr session_data(new SessionData(connection, room, io_service, state_manager));
        SessionIO sess(session_data);

        std::string character_buffer;
        std::string result = "";
        int length = 10;

        character_buffer = "T";
        std::string value = sess.getInputField(character_buffer, result, length);

        // Value is always returned to display back to the user.
        CHECK((value.compare("T") == 0));

        character_buffer = "E";
        sess.getInputField(character_buffer, result, length);

        character_buffer = "S";
        sess.getInputField(character_buffer, result, length);

        character_buffer = "T";
        sess.getInputField(character_buffer, result, length);

        character_buffer = "\n";
        sess.getInputField(character_buffer, result, length);
        CHECK((result.compare("TEST") == 0));
    }

    TEST(getInputField_Test_Field_Return_No_LF_Received_Return_Empty)
    {
        // Mock SessionData Class
        connection_ptr          connection;
        session_manager_ptr     room;
        boost::asio::io_service io_service;
        state_manager_ptr       state_manager;

        session_data_ptr session_data(new SessionData(connection, room, io_service, state_manager));
        SessionIO sess(session_data);

        std::string character_buffer;
        std::string result = "";
        int length = 10;

        character_buffer = "T";
        sess.getInputField(character_buffer, result, length);

        character_buffer = "E";
        sess.getInputField(character_buffer, result, length);

        character_buffer = "S";
        sess.getInputField(character_buffer, result, length);

        character_buffer = "T";
        sess.getInputField(character_buffer, result, length);
        CHECK((result.compare("") == 0));
    }

    TEST(getInputField_Test_Field_Return_Aborted_On_ESC)
    {
        // Mock SessionData Class
        connection_ptr          connection;
        session_manager_ptr     room;
        boost::asio::io_service io_service;
        state_manager_ptr       state_manager;

        session_data_ptr session_data(new SessionData(connection, room, io_service, state_manager));
        SessionIO sess(session_data);

        std::string character_buffer;
        std::string result = "";
        int length = 10;

        character_buffer = "\x1b";
        sess.getInputField(character_buffer, result, length);
        CHECK((result.compare("") == 0));

        character_buffer = "\0";
        result.clear();
        std::string value = sess.getInputField(character_buffer, result, length);

        CHECK_EQUAL(value, "aborted"); // String Aborted Return on ESC Press.
        CHECK_EQUAL(result, "");       // // Result Blank on Aborts only returns data after ENTER
    }

    TEST(getInputField_Test_Field_Return_Aborted_On_ESC_Mid_Field)
    {
        // Mock SessionData Class
        connection_ptr          connection;
        session_manager_ptr     room;
        boost::asio::io_service io_service;
        state_manager_ptr       state_manager;

        session_data_ptr session_data(new SessionData(connection, room, io_service, state_manager));
        SessionIO sess(session_data);

        std::string character_buffer;
        std::string result = "";
        int length = 10;

        character_buffer = "T";
        std::string value = sess.getInputField(character_buffer, result, length);
        CHECK((value.compare("T") == 0));

        character_buffer = "E";
        sess.getInputField(character_buffer, result, length);

        character_buffer = "S";
        sess.getInputField(character_buffer, result, length);

        character_buffer = "T";
        sess.getInputField(character_buffer, result, length);


        character_buffer = "\x1b";
        sess.getInputField(character_buffer, result, length);
        CHECK((result.compare("") == 0));

        character_buffer = "\0";
        result.clear();
        value = sess.getInputField(character_buffer, result, length);

        CHECK_EQUAL(value, "aborted"); // String Aborted Return on ESC Press.
        CHECK_EQUAL(result, "");       // Result Blank on Aborts only returns data after ENTER
    }

    TEST(getInputField_Test_Field_Returns_Empty_On_ESC_Squences_Returns_Following_Character)
    {
        // We want to make sure ESC sequences, arrow keys etc..
        // will not be mistaken for ESC and abort the field.

        // Mock SessionData Class
        connection_ptr          connection;
        session_manager_ptr     room;
        boost::asio::io_service io_service;
        state_manager_ptr       state_manager;

        session_data_ptr session_data(new SessionData(connection, room, io_service, state_manager));
        SessionIO sess(session_data);

        std::string character_buffer;
        std::string result = "";
        int length = 10;

        character_buffer = "T";
        sess.getInputField(character_buffer, result, length);

        character_buffer = "E";
        sess.getInputField(character_buffer, result, length);

        character_buffer = "S";
        sess.getInputField(character_buffer, result, length);

        character_buffer = "T";
        sess.getInputField(character_buffer, result, length);


        character_buffer = "\x1b";
        std::string value = sess.getInputField(character_buffer, result, length);
        CHECK_EQUAL(value, "");       // String Aborted Return on ESC Press.
        CHECK_EQUAL(result, "");      // Result Blank on Aborts

        character_buffer = "[";
        result.clear();
        value = sess.getInputField(character_buffer, result, length);
        CHECK_EQUAL(value, "");       // String Aborted Return on ESC Press.
        CHECK_EQUAL(result, "");      // Result Blank on Aborts

        character_buffer = "A";
        result.clear();
        value = sess.getInputField(character_buffer, result, length);

        std::cout << value << std::endl;
        CHECK_EQUAL(value, "empty");  // String Aborted Return on ESC Press.
        CHECK_EQUAL(result, "");      // Result Blank on Aborts

        // Now return the character after the ESC Sequence.
        character_buffer = "A";
        result.clear();
        value = sess.getInputField(character_buffer, result, length);
        CHECK_EQUAL(value, "A");      // String Aborted Return on ESC Press.
        CHECK_EQUAL(result, "");      // Result Blank on Aborts only returns data after ENTER
    }

    TEST(getInputField_Test_Field_Returns_Result_String_On_Enter)
    {
        // We want to make sure ESC sequences, arrow keys etc..
        // will not be mistaken for ESC and abort the field.

        // Mock SessionData Class
        connection_ptr          connection;
        session_manager_ptr     room;
        boost::asio::io_service io_service;
        state_manager_ptr       state_manager;

        session_data_ptr session_data(new SessionData(connection, room, io_service, state_manager));
        SessionIO sess(session_data);

        std::string character_buffer;
        std::string result = "";
        int length = 10;

        character_buffer = "T";
        sess.getInputField(character_buffer, result, length);

        character_buffer = "E";
        sess.getInputField(character_buffer, result, length);

        character_buffer = "S";
        sess.getInputField(character_buffer, result, length);

        character_buffer = "T";
        sess.getInputField(character_buffer, result, length);

        character_buffer = "\n";
        std::string value = sess.getInputField(character_buffer, result, length);
        CHECK_EQUAL(value, "\n");  // String Aborted Return on ESC Press.
        CHECK_EQUAL(result, "TEST");        // Result Blank on Aborts
    }


    TEST(getInputField_Test_Field_Incomplete_ESC_ignored_Returns_Empty)
    {
        // We want to make sure ESC sequences, arrow keys etc..
        // will not be mistaken for ESC and abort the field.

        // Mock SessionData Class
        connection_ptr          connection;
        session_manager_ptr     room;
        boost::asio::io_service io_service;
        state_manager_ptr       state_manager;

        session_data_ptr session_data(new SessionData(connection, room, io_service, state_manager));
        SessionIO sess(session_data);

        std::string character_buffer;
        std::string result = "";
        int length = 10;


        character_buffer = "\x1b";
        std::string value = sess.getInputField(character_buffer, result, length);
        CHECK_EQUAL(value, "");    // String Aborted Return on ESC Press.
        CHECK_EQUAL(result, "");   // Result Blank on Aborts

        character_buffer = "\n";
        value = sess.getInputField(character_buffer, result, length);
        std::cout << "RESULT: " << result << " : " << value << std::endl;
        CHECK_EQUAL(value, "\n");  // String Aborted Return on ESC Press.
        CHECK_EQUAL(result, "");   // Result Blank on Aborts
    }

}
