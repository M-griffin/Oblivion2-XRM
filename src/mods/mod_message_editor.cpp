#include "mod_message_editor.hpp"

#include "../data-sys/text_prompts_dao.hpp"
#include "../model-sys/config.hpp"

#include "../session.hpp"
#include "../session_io.hpp"

#include "../processor_ansi.hpp"
#include "../processor_text.hpp"
#include "../logging.hpp"
#include "../common_io.hpp"

#include <string>
#include <vector>
#include <memory>

ModMessageEditor::ModMessageEditor(session_ptr session_data, config_ptr config, processor_ansi_ptr ansi_process,
        common_io_ptr common_io, session_io_ptr session_io)
    : ModBase(session_data, config, ansi_process,"mod_message_editor.yaml", common_io, session_io)
    , m_text_prompts_dao(new TextPromptsDao(GLOBAL_DATA_PATH, m_filename))
    , m_text_process(nullptr)
    , m_mod_function_index(MOD_PROMPT)
    , m_mod_setup_index(MOD_DISPLAY_EDITOR)
    , m_mod_user_state_index(MOD_FSE_INPUT)
    , m_failure_attempts(0)
    , m_is_text_prompt_exist(false)

      // Later on default to 1, and screen width!!!
    , m_text_box_top(0)
    , m_text_box_bottom(24)
    , m_text_box_left(0)
    , m_text_box_right(80)
    , m_text_box_height(0)
    , m_text_box_width(0)

{
    // Push function pointers to the stack.
    m_setup_functions.push_back(std::bind(&ModMessageEditor::setupEditor, this));
    m_mod_functions.push_back(std::bind(&ModMessageEditor::editorInput, this, std::placeholders::_1));

    // Check of the Text Prompts exist.
    m_is_text_prompt_exist = m_text_prompts_dao->fileExists();

    if(!m_is_text_prompt_exist)
    {
        createTextPrompts();
    }

    // Loads all Text Prompts for current module
    m_text_prompts_dao->readPrompts();
}

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
    
    //value[DEFAULT_TEXT_COLORS]                = std::make_pair("Default Text Color when typing", "|03|16");
    value[DEFAULT_TEXT_COLORS]                = std::make_pair("Selected Text when CTRL+B is keyed for selection", "|02|20");
    value[BACKSPACE_TEXT_COLORS]              = std::make_pair("Match your template background color", "|04|17");
    
    value[SELECTED_TEXT_COLORS]               = std::make_pair("Selected Text when CTRL+B is keyed for selection", "|02|20");
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
std::string ModMessageEditor::processTopTemplate(processor_ansi_ptr ansi_process, const std::string &screen)
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
    ansi_process->parseTextToBuffer((char *)new_screen.c_str());
    m_text_box_top = ansi_process->getMaxRowsUsedOnScreen() + 1;

    return ansi_process->getScreenFromBuffer(true);
}

/**
 * @brief Processes a Bottom Template Screen
 * @param ansi_process
 * @param screen
 * @return
 */
std::string ModMessageEditor::processBottomTemplate(processor_ansi_ptr ansi_process, const std::string &screen)
{
    std::string new_screen = screen;
    ansi_process->clearScreen();
    ansi_process->parseTextToBuffer((char *)new_screen.c_str());
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
std::string ModMessageEditor::processMidTemplate(processor_ansi_ptr ansi_process, const std::string &screen)
{
    std::string new_screen = screen;
    scrubNewLinesChars(new_screen);

    // Clear All Mappings
    m_session_io->clearAllMCIMapping();
    m_session_io->addMCIMapping("|LT", "");
    m_session_io->addMCIMapping("|RT", "");

    // Build a single code map that can be reused.
    std::vector<MapType> code_map = m_session_io->pipe2genericCodeMap(new_screen);


    // Load, then pull MCI off-sets on the screen for margins.
    ansi_process->clearScreen();
    ansi_process->parseTextToBuffer((char *)new_screen.c_str());
    m_text_box_left  = ansi_process->getMCIOffSet("|LT");
    m_text_box_right = ansi_process->getMCIOffSet("|RT");

    // remove length of left side MCI code if both exists.
    if(m_text_box_right > 0 && m_text_box_left > 0)
    {
        m_text_box_right -= 3;
    }

    // Remove Code Mappings from screen the sets up ansi screen
    ansi_process->clearScreen();
    std::string output_screen = m_session_io->parseCodeMapGenerics(new_screen, code_map);

    ansi_process->parseTextToBuffer((char *)output_screen.c_str());

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
    std::string top_template = m_common_io->readinAnsi("FSESRT.ANS");
    std::string mid_template = m_common_io->readinAnsi("FSEMID.ANS");
    std::string bot_template = m_common_io->readinAnsi("FSEEND.ANS");

    // Use a Local Ansi Parser for Parsing Menu Templates and determine boundaries.
    processor_ansi_ptr ansi_process(new ProcessorAnsi(
                                        m_session_data->m_telnet_decoder->getTermRows(),
                                        m_session_data->m_telnet_decoder->getTermCols())
                                   );

    // Parse the TOP Screen to get Top Text Margin
    std::string top_screen = processTopTemplate(ansi_process, top_template);

    // Parse the BOTTOM Screen to get Bot Text Margin
    std::string bot_screen = processBottomTemplate(ansi_process, bot_template);

    // Parse The Mid to get Left / Right Margins |RT and |LT specify Right and Left.
    // Only after Top and Bottom are calculated, this will generate the appropriate
    // Rows uses between top / bot templates on current screen size.
    std::string mid_screen = processMidTemplate(ansi_process, mid_template);

    m_log.write<Logging::DEBUG_LOG>("m_text_box_top=", m_text_box_top, "m_text_box_bottom=", m_text_box_bottom,
                                    "m_text_box_left=", m_text_box_left, "m_text_box_right=", m_text_box_right);

    // Setup the Text Parser Init the Parser with template data.
    m_text_box_height = m_text_box_bottom - m_text_box_top;
    m_text_box_width = m_text_box_right - m_text_box_left;

    // Adjust to minus 1 from bottom.
    m_text_box_height += 1;


    m_log.write<Logging::DEBUG_LOG>("m_text_process - height=", m_text_box_height, "width=", m_text_box_width);
    m_text_process.reset(new ProcessorText(m_text_box_height, m_text_box_width));

    // Next combine and output.. Move cursor to top left in box.
    std::string screen_output = top_screen + mid_screen + bot_screen;
    screen_output += "\x1b[" + std::to_string(m_text_box_top) + ";" + std::to_string(m_text_box_left) + "H";

    screen_output += getDisplayPromptRaw(DEFAULT_TEXT_COLORS);
    baseProcessDeliverInput(screen_output);
}

/**
 * @brief General Input for Editor
 * @return
 */
void ModMessageEditor::editorInput(const std::string &input)
{
    std::string result = m_session_io->getFSEKeyInput(input);

    if(result.size() == 0)
    {
        return;
    }
    
    m_log.write<Logging::DEBUG_LOG>("result=", result, "input=", input);

    if(result[0] == 13 || result[0] == 10)
    {
        // Translations for ENTER next line
        //input = "ENTER";
        m_log.write<Logging::CONSOLE_LOG>("[editorInput] [ENTER HIT] input result=", result);
        //std::string output = "\r\n\x1b[" + std::to_string(m_text_box_left - 1) + "C";

        processTextInput(result, input);
    }
    else if(result[0] == '\x1b' && result.size() > 2)
    {
        // ESC SEQUENCE - check movement / arrow keys.
        std::string escape_sequence = m_common_io->getEscapeSequence();        
        m_log.write<Logging::DEBUG_LOG>("ESC=", escape_sequence);
                
        m_log.write<Logging::CONSOLE_LOG>("[editorInput] [ESC Sequence 1] input result=", result);
        processEscapedInput(result.substr(1), input);
    }
    else if(result[0] == '\x1b' && result.size() == 2 && result[1] == '\x1b')
    {
        // Check Single ESC KEY - command options
        //input = "ESC";  - quit for now
        m_log.write<Logging::CONSOLE_LOG>("[editorInput] [ESC HIT!] input result=", result);
        m_is_active = false;
    }
    else
    {
        // Handle Input Characters here and control chars.
        m_log.write<Logging::CONSOLE_LOG>("[editorInput] [STDIO] input result=", result);

        if(result[0] == '\x1b')
        {
            std::string escape_sequence = m_common_io->getEscapeSequence();
            std::cout << "ESC= " << escape_sequence << std::endl;
            m_log.write<Logging::CONSOLE_LOG>("[editorInput] [ESC Sequence 2] input result=", static_cast<int>(escape_sequence[0]));

            // Hot Key Input, cutoff leading escape for control key.
            processEscapedInput(result.substr(1), input);
            return;
        }

        for(char c : result)
            m_log.write<Logging::CONSOLE_LOG>("[editorInput] [c HIT] input result=", static_cast<int>(c));

        processTextInput(result, input);
    }

    return;
}

/**
 * @brief Move to the Cursor to the Buffer Position
 */
std::string ModMessageEditor::moveCursorToPosition()
{
    std::string output = "";
    output += "\x1b[";
    output += std::to_string((m_text_process->getYPosition()-1) + m_text_box_top) + ";";
    output += std::to_string((m_text_process->getXPosition()-1) + m_text_box_left) + "H";
    return output;
}

/**
 * @brief Scroll Screen the Text Up
 */
std::string ModMessageEditor::scrollTextBoxUp(std::string &output)
{
    // Placeholder
    return output;
}

/**
 * @brief Scroll Screen the Text Down
 */
std::string ModMessageEditor::scrollTextBoxDown(std::string &output)
{
    // Placeholder
    return output;
}

/**
 * @brief Handle Backspaces
 * @param output
 */
void ModMessageEditor::handleBackSpace(std::string &output)
{
    /** Need to Write code to delete char or space and move everything left **/    
    int x_position = m_text_process->getXPosition();    
    m_log.write<Logging::DEBUG_LOG>("handleBackSpace max_chars=", m_text_process->getMaxCharactersPerLine(), "x_pos=", x_position);

    std::string default_text_color = getDisplayPromptRaw(DEFAULT_TEXT_COLORS);
    std::string backspace_color = getDisplayPromptRaw(BACKSPACE_TEXT_COLORS);

    // destructive backspace OR Move Up a Line Backspace.
    if(x_position > 1)
    {
        m_text_process->parseTextToBuffer((char *)"\b");
        output += backspace_color + "\x1b[D \x1b[D" + default_text_color;
    }
    // detsructive backspace move up and end of line
    else if(x_position == 1)
    {        
        // Check Updated Position after parse - If we move from 1, up and to end of line, then 
        // remove last character for cursor space.
        m_text_process->parseTextToBuffer((char *)"\b");
        
        m_log.write<Logging::DEBUG_LOG>("handleBackSpaceAfter m_text_process->x_position=", 
            m_text_process->getXPosition(), "x_pos=", x_position);
        
        if(m_text_process->getXPosition() == m_text_process->getMaxCharactersPerLine()) {
            output += backspace_color + " \x1b[D" + default_text_color;
        }    
    }
}

/**
 * @brief Handle Deletes Inline
 * @param output
 */
void ModMessageEditor::handleDelete(std::string &output)
{
    /** Need to Write code to delete char or space and move everything left **/    
    int x_position = m_text_process->getXPosition();
    
    m_log.write<Logging::DEBUG_LOG>("handleDelete max_chars=", m_text_process->getMaxCharactersPerLine(), "x_pos=", x_position);

    std::string default_text_color = getDisplayPromptRaw(DEFAULT_TEXT_COLORS);
    std::string backspace_color = getDisplayPromptRaw(BACKSPACE_TEXT_COLORS);

    // destructive backspace OR Move Up a Line Backspace.
    if(x_position > 1)
    {
        m_text_process->parseTextToBuffer((char *)"\b");
        output += backspace_color + "\x1b[D \x1b[D" + default_text_color;
    }
    // detsructive backspace move up and end of line
    else if(x_position == 1)
    {        
        // Check Updated Position after parse - If we move from 1, up and to end of line, then 
        // remove last character for cursor space.
        m_text_process->parseTextToBuffer((char *)"\b");
        
        m_log.write<Logging::DEBUG_LOG>("handleBackSpaceAfter m_text_process->x_position=", 
            m_text_process->getXPosition(), "x_pos=", x_position);
            
        if(m_text_process->getXPosition() == m_text_process->getMaxCharactersPerLine()) {
            output += backspace_color + " \x1b[D" + default_text_color;
        }    
    }

}

/**
 * @brief Handle New Lines
 * @param output
 */
void ModMessageEditor::handleNewLines(std::string &output)
{
    m_text_process->parseTextToBuffer((char *)"\n");
    output += moveCursorToPosition();
}

/**
 * @brief Process Input Text for Editor
 * @return
 */
void ModMessageEditor::processTextInput(std::string result, std::string input)
{

    // Just some testing figuring out the best design.

    std::string output = "";
    int x_position = m_text_process->getXPosition();
    int y_position = m_text_process->getYPosition();
    std::cout << "x_pos: " << x_position << std::endl;
    std::cout << "y_pos: " << y_position << std::endl;

    if(result[0] != '\x7f' && result[0] != '\x08' && result[0] != '\r' && result[0] != '\n') // && result[0] != '\x1b')
    {
        m_text_process->parseTextToBuffer((char *)input.c_str());
        output = input;
    }

    // Handle Backspaces or Delete depending on User Switch
    if(result[0] == '\x7f')
    {
        if(m_session_data->m_user_record->bBackSpaceVt100)
            handleBackSpace(output);
        else
            handleDelete(output);
    }
    else if(result[0] == '\x08')
    {
        if(!m_session_data->m_user_record->bBackSpaceVt100)
            handleBackSpace(output);
        else
            handleDelete(output);
    }
    // CRLF New Line
    else if(result[0] == '\r' || result[0] == '\n')
    {
        handleNewLines(output);
    }
    // Wrap Around and Move Down at end of line
    else if(x_position == m_text_box_width && y_position != m_text_box_height)
    {
        //output += moveCursorToPosition();
    }
    // Bottom of box, scroll up a line up, and move to begininng.
    else if(y_position == m_text_box_height && x_position == m_text_box_width)
    {
        //output += moveCursorToPosition();
        m_text_process->parseTextToBuffer((char *)"\n");
        //output += moveCursorToPosition();
        // Scroll Box Up!

    }
    
    std::cout << "=========w max " << m_text_box_width << ", lft " << m_text_box_left << std::endl;
    std::cout << "=========h max " << m_text_box_height << ", top " << m_text_box_top << std::endl;

    std::cout << "x_pos: " << m_text_process->getXPosition() << std::endl;
    std::cout << "y_pos: " << m_text_process->getYPosition() << std::endl;
     
    std::cout << "getCurrentLine: " << m_text_process->getCurrentLine() << std::endl;
    
    std::map<int, int> line_ending_map = m_text_process->getLineEndingMap();
    int lineEndPosition = line_ending_map[m_text_process->getCurrentLine()];
    std::cout << "lineEndPosition: " << lineEndPosition << std::endl;
    
    
    // Important Feature, resets to correct position when lines change etc.. 
    // Add a second backspace only when flag is set
    // Flag is set when we move up and end of previous line
    // We then remove the last char so we're off the screen border.        
    output += moveCursorToPosition();
    
    // This is required to move up to the previous line
    // Then clear the char in the spot we just occipied.
    if (m_text_process->isDoubleBackSpace()) {
        
        std::string default_text_color = getDisplayPromptRaw(DEFAULT_TEXT_COLORS);
        std::string backspace_color = getDisplayPromptRaw(BACKSPACE_TEXT_COLORS);
    
        m_log.write<Logging::DEBUG_LOG>("isDoubleBackSpace max_chars=", m_text_process->getMaxCharactersPerLine());
        
        if (m_text_process->getMaxCharactersPerLine() == m_text_process->getXPosition()) 
        {
            output += backspace_color + " \x1b[D" + default_text_color; // clear the current position
            // TODO Probably need to clear buffer at this spot to for current position.
        }        
        m_text_process->setDoubleBackSpace(false);
    }
    
    baseProcessDeliverInput(output);
}

/**
 * @brief Process Escaped and Control Input Keys for Editor
 * @return
 */
void ModMessageEditor::processEscapedInput(std::string result, std::string input)
{

    // Just some testing figuring out the best design.

    std::string output = "";    
    int x_position = m_text_process->getXPosition();
    int y_position = m_text_process->getYPosition();
    
    std::cout << "=========w max " << m_text_box_width << " " << m_text_box_left << std::endl;
    std::cout << "=========h max " << m_text_box_height << " " << m_text_box_top << std::endl;
    
    std::cout << "x_pos: " << x_position << std::endl;
    std::cout << "y_pos: " << y_position << std::endl;

    std::cout << "processedEscapedInput result= " << result << ", input= " << input << std::endl;

    if(result == "rt_arrow")
    {
        if(x_position < m_text_box_width)
        {
            m_text_process->moveNextXPosition();
            //output = "\x1b[C";
        }
    }
    else if(result == "lt_arrow")
    {
        if(x_position > 1)
        {
            m_text_process->movePreviousXPosition();
            //output = "\x1b[D";
        }
    }
    else if(result == "up_arrow")
    {
        if(y_position > 1)
        {
            m_text_process->movePreviousYPosition();
            //output = "\x1b[A";
        }
    }
    else if(result == "dn_arrow")
    {
        if(y_position < m_text_box_height)
        {
            m_text_process->moveNextYPosition();
            //output = "\x1b[B";
        }
    }
    else if(result == "home")
    {
        m_text_process->moveHomePosition();
        //output = moveCursorToPosition();
    }
    else if(result == "end")
    {
        m_text_process->moveEndPosition();
        //output = moveCursorToPosition();
    }


    std::cout << "output: " << output << std::endl;

    output += moveCursorToPosition();
    baseProcessDeliverInput(output);
}
