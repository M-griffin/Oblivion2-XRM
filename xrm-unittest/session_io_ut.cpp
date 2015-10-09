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
    TEST(InputKeyNormalCharacter)
    {
        // Normal Characters and sequences should pass through
        session_data_ptr session_data;
        SessionIO sess(session_data);

        std::string character_buffer = "A";
        std::string result = sess.getKeyInput(character_buffer);
        CHECK(result.compare(character_buffer) == 0);
    }

    TEST(InputKeyDoubleCharacterExpectEmpty)
    {
        session_data_ptr session_data;
        SessionIO sess(session_data);

        std::string character_buffer = "AA";
        std::string result = sess.getKeyInput(character_buffer);
        CHECK(result.compare("") == 0);
    }

    TEST(InputKeyUniCodeCharacter)
    {
        // Normal Unicode Characters and sequences should pass through
        session_data_ptr session_data;
        SessionIO sess(session_data);

        std::string character_buffer = "あ";
        std::string result = sess.getKeyInput(character_buffer);
        CHECK(result.compare(character_buffer) == 0);
    }

    TEST(InputKeyTwoUniCodeCharacterExpectEmpty)
    {
        session_data_ptr session_data;
        SessionIO sess(session_data);

        std::string character_buffer = "ああ";
        std::string result = sess.getKeyInput(character_buffer);
        CHECK(result.compare("") == 0);
    }

    TEST(InputKeyMixNormalAndUniCodeCharacterExpectEmpty)
    {
        session_data_ptr session_data;
        SessionIO sess(session_data);

        std::string character_buffer = "あAあ";
        std::string result = sess.getKeyInput(character_buffer);
        CHECK(result.compare("") == 0);
    }

    TEST(InputKeyMixNormalAndUniCodeCharacter2ExpectEmpty)
    {
        session_data_ptr session_data;
        SessionIO sess(session_data);

        std::string character_buffer = "Aあ";
        std::string result = sess.getKeyInput(character_buffer);
        CHECK(result.compare("") == 0);
    }

    TEST(InputKeyMixNormalAndUniCodeCharacter3ExpectEmpty)
    {
        session_data_ptr session_data;
        SessionIO sess(session_data);

        std::string character_buffer = "あA";
        std::string result = sess.getKeyInput(character_buffer);
        CHECK(result.compare("") == 0);
    }

    TEST(InputKeyEscapeExpectEmptyThenEsc)
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

    TEST(InputKeyEscapeSequenceUpArrowFailure)
    {
        // One key per sequence at a time.
        session_data_ptr session_data;
        SessionIO sess(session_data);

        std::string character_buffer = "\x1b[A";
        std::string result = sess.getKeyInput(character_buffer);
        CHECK((result.compare("") == 0));
    }

    TEST(InputKeyEscapeSequenceUpArrowSequence)
    {
        // One key per sequence at a time.
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

    TEST(InputKeyEscapeSequenceDnArrowSequence)
    {
        // One key per sequence at a time.
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
        std::cout << "\n RESULT: [" << result << "]" << std::endl;
        CHECK((result.compare("\x1b""dn_arrow") == 0));
    }

    TEST(InputKeyEscapeSequenceLtArrowSequence)
    {
        // One key per sequence at a time.
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
        std::cout << "\n RESULT: [" << result << "]" << std::endl;
        CHECK((result.compare("\x1b""lt_arrow") == 0));
    }

    TEST(InputKeyEscapeSequenceRtArrowSequence)
    {
        // One key per sequence at a time.
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
        std::cout << "\n RESULT: [" << result << "]" << std::endl;
        CHECK((result.compare("\x1b""rt_arrow") == 0));
    }

    TEST(InputKeyEscapeSequenceHardwareUpArrowSequence)
    {
        // One key per sequence at a time.
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

    TEST(InputKeyEscapeSequenceHardwareDnArrowSequence)
    {
        // One key per sequence at a time.
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

    TEST(InputKeyEscapeSequenceHardwareLtArrowSequence)
    {
        // One key per sequence at a time.
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

    TEST(InputKeyEscapeSequenceHardwareRtArrowSequence)
    {
        // One key per sequence at a time.
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


    TEST(InputKeyEscapeSequenceEscTwiceReturnsEsc)
    {
        // One key per sequence at a time.
        session_data_ptr session_data;
        SessionIO sess(session_data);

        std::string character_buffer = "\x1b";
        std::string result = sess.getKeyInput(character_buffer);
        CHECK((result.compare("") == 0));

        character_buffer = "\x1b";
        result = sess.getKeyInput(character_buffer);
        std::cout << "\n RESULT: [" << result << "]" << std::endl;
        CHECK((result.compare("\x1b") == 0));
    }



}
