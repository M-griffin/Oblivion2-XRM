
#include "mods/mod_message_editor.hpp"

#include "../model-sys/config.hpp"

#include "../processor_ansi.hpp"
#include "../common_io.hpp"
#include "../session_io.hpp"

#include <UnitTest++.h>

#include <iostream>
#include <string>
#include <memory>

/**
 * Handle Setup and Tear Down of Integration Test for SQLite
 * - Incomplete, needs work, Fixture is throwing Unhandled Excetpion, rework this!
 */
session_ptr session_data;
common_io_ptr common_io;
session_io_ptr session_io;
 
class MyFixtureMessageEditor 
    //: public ModMessageEditor
{
public:

    //ModMessageEditor(session_ptr session_data, config_ptr config, processor_ansi_ptr ansi_process,
    //    common_io_ptr common_io, session_io_ptr session_io);

    MyFixtureMessageEditor()
      //  : ModMessageEditor(session_data, config_ptr(new Config()), processor_ansi_ptr(new ProcessorAnsi(10, 10)),
      //      common_io, session_io)
    { 
        std::cout << "MyFixtureMessageEditor" << std::endl;
    }

    ~MyFixtureMessageEditor()
    { 
    }

    // Create Prompt Constants, these are the keys for key/value lookup
    const std::string DEFAULT_TEXT_COLORS = "default_text_colors";
    const std::string BACKSPACE_TEXT_COLORS = "backspace_text_colors";
    const std::string SELECTED_TEXT_COLORS = "selected_text_colors";
    
    // Just an example of override test for testing mock ups.
    std::string getDisplayPromptRaw(const std::string &prompt) {
        return "test";
    }
};

/**
 * @brief Unit Testing for Initial Sqlite Database DAO and BaseDao.
 * @return
 */
SUITE(XRMMessageEditorTest)
{

    // Test DAO with All Base Dao Calls.
    TEST_FIXTURE(MyFixtureMessageEditor, MessageEditorTest)
    {
                        
        // Create Prompt Constants, these are the keys for key/value lookup
        const std::string DEFAULT_TEXT_COLORS = "default_text_colors";
        const std::string BACKSPACE_TEXT_COLORS = "backspace_text_colors";
        const std::string SELECTED_TEXT_COLORS = "selected_text_colors";
    
        // Right now testing the propmt is returns with the correct color codes
        // just noticed, we want to switch MCI to system color codes for themes colors.
        std::string default_color = getDisplayPromptRaw(DEFAULT_TEXT_COLORS);
        
        // Just an example of override for now.
        CHECK(default_color == "test");

    }

}
