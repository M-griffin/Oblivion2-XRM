#include "mod_message_editor.hpp"
#include "../ansi_processor.hpp"
#include "../logging.hpp"

#include <string>
#include <vector>

/**
 * @brief Handles Updates or Data Input from Client
 * @return
 */
bool ModMessageEditor::update(const std::string &character_buffer, const bool &)
{
    // Make sure system is active, when system is done, success or fails
    // We change this is inactive to single the login process is completed.
    if(!m_is_active)
    {
        return false;
    }

    // Return True when were keeping module active / else false;
    if(character_buffer.size() == 0)
    {
        return true;
    }

    // Process all incoming data straight to the input functions.
    m_mod_functions[m_mod_function_index](character_buffer);

    return true;
}

/**
 * @brief Start-up class, setup and display initial screens / interface.
 * @return
 */
bool ModMessageEditor::onEnter()
{
    m_is_active = true;

    // Grab ANSI Screen, display, if desired.. logon.ans maybe?
    std::string prompt = "\x1b[?25h"; // Turn on Cursor.
    baseProcessAndDeliver(prompt);

    // Execute the initial setup index.
    m_setup_functions[m_mod_setup_index]();

    return true;
}

/**
 * @brief Exit, close down, display screens to change over data.
 * @return
 */
bool ModMessageEditor::onExit()
{
    m_is_active = false;
    return true;
}

/**
 * @brief Create Default Text Prompts for module
 */
void ModMessageEditor::createTextPrompts()
{
    // Create Mapping to pass for file creation (default values)
    M_TextPrompt value;
    /*
        value[PROMPT_HEADER]                  = std::make_pair("User Editor Header", "|CS|CR|03--- |15[|03Oblivion/2 XRM |07// |11User Editor|15] |03--- |11Filtering View : |15|OT |CR");
        value[PROMPT_INPUT_TEXT]              = std::make_pair("User Editor Prompt", "|CR|03E|15/dit User |03D|15/elete User |03C|15/opy User |03F|15/ilter Users |03Q|15/uit : ");
        value[PROMPT_PAUSE]                   = std::make_pair("Pause Prompt", "|CR |03- |15Hit any key to continue or (|03a|15)bort listing |03-|15 |CR");
    */
    m_text_prompts_dao->writeValue(value);
}

/**
 * @brief Sets an individual input module index.
 * @param mod_function_index
 */
void ModMessageEditor::changeInputModule(int mod_function_index)
{
    // Setup input module
    m_mod_function_index = mod_function_index;
}

/**
 * @brief Sets an individual setup method module index.
 * @param mod_function_index
 */
void ModMessageEditor::changeSetupModule(int mod_function_index)
{
    // Set, and Execute the Setup module.
    m_mod_setup_index = mod_function_index;
    m_setup_functions[m_mod_setup_index]();
}

/**
 * @brief Sets an individual Menu Input State Add/Change/Delete
 * @param mod_menu_state_index
 */
void ModMessageEditor::changeMenuInputState(int mod_menu_state_index)
{
    m_mod_user_state_index = mod_menu_state_index;
}

/**
 * @brief Redisplay's the current module prompt.
 * @return
 */
void ModMessageEditor::redisplayModulePrompt()
{
    m_setup_functions[m_mod_setup_index]();
}

/**
 * @brief Pull and Display Prompts
 * @param prompt
 */
void ModMessageEditor::displayPrompt(const std::string &prompt)
{
    baseDisplayPrompt(prompt, m_text_prompts_dao);
}

/**
 * @brief Pull and parse and return Display Prompts for use in interfaces
 * @param prompt
 */
std::string ModMessageEditor::getDisplayPrompt(const std::string &prompt)
{
    return baseGetDisplayPrompt(prompt, m_text_prompts_dao);
}

/**
 * @brief Pull and parse and return Display Prompts for use in interfaces
 * @param prompt
 */
std::string ModMessageEditor::getDisplayPromptRaw(const std::string &prompt)
{
    return baseGetDisplayPromptRaw(prompt, m_text_prompts_dao);
}

/**
 * @brief Pull and Display Prompts with MCI Code
 * @param prompt
 * @param mci_field
 */
void ModMessageEditor::displayPromptMCI(const std::string &prompt, const std::string &mci_field)
{
    baseDisplayPromptMCI(prompt, m_text_prompts_dao, mci_field);
}

/**
 * @brief Pull and Display Prompts with following newline
 * @param prompt
 */
void ModMessageEditor::displayPromptAndNewLine(const std::string &prompt)
{
    baseDisplayPromptAndNewLine(prompt, m_text_prompts_dao);
}

/**
 * @brief Processes a TOP Template Screen
 * @param ansi_process
 * @param screen
 * @return
 */
std::string ModMessageEditor::processTopTemplate(ansi_process_ptr ansi_process, const std::string &screen)
{
    /**
     * When we get 2J alone, it clears but leaves cursor.
     * In most cases we need to add a pre-home cursor!
     */
    std::string new_screen = screen;
    std::string::size_type index = 0;

    while(index != std::string::npos)
    {
        index = new_screen.find("\x1b[2J", index);

        if(index != std::string::npos)
        {
            new_screen.replace(index, 4, "\x1b[1;1H\x1b[2J");
            // Increment past previous replacement.
            index += 9;
        }
    }

    ansi_process->clearScreen();
    ansi_process->parseAnsiScreen((char *)new_screen.c_str());
    m_text_box_top = ansi_process->getMaxRowsUsedOnScreen() + 1;

    return ansi_process->getScreenFromBuffer(true);
}

/**
 * @brief Processes a Bottom Template Screen
 * @param ansi_process
 * @param screen
 * @return
 */
std::string ModMessageEditor::processBottomTemplate(ansi_process_ptr ansi_process, const std::string &screen)
{
    std::string new_screen = screen;
    ansi_process->clearScreen();
    ansi_process->parseAnsiScreen((char *)new_screen.c_str());
    int rows_used = ansi_process->getMaxRowsUsedOnScreen();

    // We have size of footer, now subtract from screen height to get bottom margin.
    int max_lines = ansi_process->getMaxLines();
    m_text_box_bottom = max_lines - rows_used;

    return new_screen;
}

/**
 * @brief Scrub CR LF from Screen Templates
 * @param screen
 */
void ModMessageEditor::scrubNewLinesChars(std::string &screen)
{
    // MID Ansi, remove any extra CR / LF carriage returns.
    std::string::size_type index = 0;

    while(index != std::string::npos)
    {
        index = screen.find("\r", index);

        if(index != std::string::npos)
        {
            screen.erase(index, 1);
        }
    }

    index = 0;

    while(index != std::string::npos)
    {
        index = screen.find("\n", index);

        if(index != std::string::npos)
        {
            screen.erase(index, 1);
        }
    }
}

/**
 * @brief Processes a MID Template Screen
 * @param ansi_process
 * @param screen
 * @return
 */
std::string ModMessageEditor::processMidTemplate(ansi_process_ptr ansi_process, const std::string &screen)
{
    std::string new_screen = screen;
    scrubNewLinesChars(new_screen);

    // Clear All Mappings
    m_session_io.clearAllMCIMapping();
    m_session_io.addMCIMapping("|LT", "");
    m_session_io.addMCIMapping("|RT", "");

    // Build a single code map that can be reused.
    std::vector<MapType> code_map = m_session_io.pipe2genericCodeMap(new_screen);


    // Load, then pull MCI off-sets on the screen for margins.
    ansi_process->clearScreen();
    ansi_process->parseAnsiScreen((char *)new_screen.c_str());
    m_text_box_left  = ansi_process->getMCIOffSet("|LT");
    m_text_box_right = ansi_process->getMCIOffSet("|RT");

    // remove length of left side MCI code if both exists.
    if(m_text_box_right > 0 && m_text_box_left > 0)
    {
        m_text_box_right -= 3;
    }

    // Remove Code Mappings from screen the sets up ansi screen
    ansi_process->clearScreen();
    std::string output_screen = m_session_io.parseCodeMapGenerics(new_screen, code_map);

    ansi_process->parseAnsiScreen((char *)output_screen.c_str());

    std::string mid_screen_line = ansi_process->getScreenFromBuffer(false);
    new_screen.erase();
    new_screen = "";

    for(int i = 0; i < (m_text_box_bottom - m_text_box_top) + 1; i++)
    {
        new_screen += mid_screen_line;
    }

    return new_screen;
}

/**
 * @brief Setup for the Message Editor
 * @return
 */
void ModMessageEditor::setupEditor()
{
    // NOTE Possible make these class instances, so we don't have to keep reloading.
    std::string top_template = m_common_io.readinAnsi("FSESRT.ANS");
    std::string mid_template = m_common_io.readinAnsi("FSEMID.ANS");
    std::string bot_template = m_common_io.readinAnsi("FSEEND.ANS");

    // Use a Local Ansi Parser for Pasrsing Menu Templates and determine boundaries.
    ansi_process_ptr ansi_process(new AnsiProcessor(
                                      m_session_data->m_telnet_state->getTermRows(),
                                      m_session_data->m_telnet_state->getTermCols())
                                 );

    // Parse the TOP Screen to get Top Text Margin
    std::string top_screen = processTopTemplate(ansi_process, top_template);

    // Parse the BOTTOM Screen to get Bot Text Margin
    std::string bot_screen = processBottomTemplate(ansi_process, bot_template);

    // Parse The Mid to get Left / Right Margins |RT and |LT specify Right and Left.
    // Only after Top and Bottom are calculated, this will generate the appropriate
    // Rows uses between top / bot templates on current screen size.
    std::string mid_screen = processMidTemplate(ansi_process, mid_template);

    Logging *log = Logging::instance();
    log->xrmLog<Logging::DEBUG_LOG>("m_text_box_top=", m_text_box_top, "m_text_box_bottom=", m_text_box_bottom,
                                    "m_text_box_left=", m_text_box_left, "m_text_box_right=", m_text_box_right);

    // Next combine and output.. Move cursor to top left in box.
    std::string screen_output = top_screen + mid_screen + bot_screen;
    screen_output += "\x1b[" + std::to_string(m_text_box_top) + ";" + std::to_string(m_text_box_left) + "H";

    baseProcessDeliverInput(screen_output);
}

/**
 * @brief General Input for Editor
 * @return
 */
void ModMessageEditor::editorInput(const std::string &input)
{
    Logging *log = Logging::instance();
    std::string result = m_session_io.getKeyInput(input);

    if(result.size() == 0)
    {
        log->xrmLog<Logging::DEBUG_LOG>("[editorInput] [result.size() == 0] input result=", result);
        return;
    }
    else if(result[0] == 13 || result[0] == 10)
    {
        // Translations for ENTER next line
        //input = "ENTER";
        log->xrmLog<Logging::DEBUG_LOG>("[editorInput] [ENTER HIT] input result=", result);
        std::string output = "\r\n\x1b[" + std::to_string(m_text_box_left - 1) + "C";
        baseProcessDeliverInput(output);
    }
    else if(result[0] == '\x1b' && result.size() > 2)
    {
        // ESC SEQUENCE - check movement / arrow keys.
        //input = result;
        log->xrmLog<Logging::DEBUG_LOG>("[editorInput] [ESC Sequence] input result=", result);
    }
    else if(result[0] == '\x1b' && result.size() == 1)
    {
        // Check Single ESC KEY - command options
        //input = "ESC";  - quit for now
        log->xrmLog<Logging::DEBUG_LOG>("[editorInput] [ESC HIT!] input result=", result);

        m_is_active = false;
    }
    else
    {
        // Handle Input Characters here and control chars.
        log->xrmLog<Logging::DEBUG_LOG>("[editorInput] [STDIO] input result=", result);

        for(char c : result)
            log->xrmLog<Logging::DEBUG_LOG>("[editorInput] [c HIT] input result=", static_cast<int>(c));

        std::string escape_sequence = m_common_io.getEscapeSequence();
        log->xrmLog<Logging::DEBUG_LOG>("[editorInput] [ESC Sequence] input result=", static_cast<int>(escape_sequence[0]));

        // Hot Key Input.
        baseProcessDeliverInput(result);
    }

    return;
}
