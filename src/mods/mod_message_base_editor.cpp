#include "mod_message_base_editor.hpp"

#include "../data-sys/text_prompts_dao.hpp"
#include "../data-sys/access_level_dao.hpp"

#include "../model-sys/access_level.hpp"
#include "../model-sys/config.hpp"

#include "../processor_ansi.hpp"
#include "../session_io.hpp"
#include "../session.hpp"
#include "../common_io.hpp"
#include "../logging.hpp"

#include <stdint.h>
#include <string>
#include <vector>
#include <sstream>


ModMessageBaseEditor::ModMessageBaseEditor(session_ptr session_data, config_ptr config, processor_ansi_ptr ansi_process)
    : ModBase(session_data, config, ansi_process)
    , m_session_io(new SessionIO(session_data))
    , m_filename("mod_message_base_editor.yaml")
    , m_text_prompts_dao(new TextPromptsDao(GLOBAL_DATA_PATH, m_filename))
    , m_mod_setup_index(MOD_DISPLAY_MENU)
    , m_mod_function_index(MOD_MENU_INPUT)
    , m_mod_menu_state_index(MENU_ADD)
    , m_mod_toggle_view_index(VIEW_DEFAULT)
    , m_max_toggled_view_index(VIEW_PULLDOWN)
    , m_is_text_prompt_exist(false)
    , m_page(0)
    , m_rows_per_page(0)
    , m_current_menu("")
    , m_current_option(0)
    , m_current_field(0)
{
    // Setup Modules
    m_setup_functions.push_back(std::bind(&ModMessageBaseEditor::setupMenuEditor, this));
    m_setup_functions.push_back(std::bind(&ModMessageBaseEditor::setupMenuEditFields, this));

    // Input or Method Modules that handle incoming input per state.
    m_mod_functions.push_back(std::bind(&ModMessageBaseEditor::menuEditorInput, this, std::placeholders::_1));
    m_mod_functions.push_back(std::bind(&ModMessageBaseEditor::menuEditorPausedInput, this, std::placeholders::_1));
    m_mod_functions.push_back(std::bind(&ModMessageBaseEditor::menuEditorMenuNameInput, this, std::placeholders::_1));

    // Menu Field Input Commands
    m_mod_functions.push_back(std::bind(&ModMessageBaseEditor::menuEditorMenuFieldInput, this, std::placeholders::_1));
    m_mod_functions.push_back(std::bind(&ModMessageBaseEditor::menuEditorMenuFieldHandler, this, std::placeholders::_1));

    // Display Pause, waits for a key, then returns (Used in View Generic Menu)
    m_mod_functions.push_back(std::bind(&ModMessageBaseEditor::menuEditorDisplayPause, this, std::placeholders::_1));


    // Check of the Text Prompts exist.
    m_is_text_prompt_exist = m_text_prompts_dao->fileExists();
    if (!m_is_text_prompt_exist)
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
bool ModMessageBaseEditor::update(const std::string &character_buffer, const bool &)
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
bool ModMessageBaseEditor::onEnter()
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
bool ModMessageBaseEditor::onExit()
{
    m_is_active = false;
    return true;
}

/**
 * @brief Create Default Text Prompts for module
 */
void ModMessageBaseEditor::createTextPrompts()
{
    // Create Mapping to pass for file creation (default values)
    M_TextPrompt value;

    value[PROMPT_HEADER]                  = std::make_pair("Level Editor Header", "|CS|CR|03--- |15[|03Oblivion/2 XRM |07// |11Level Editor|15] |03--- |CR");
    value[PROMPT_LEVEL_EDIT_HEADER]       = std::make_pair("Level Fields Editor Header |OT Level ID", "|CS|CR|03--- |15[|03Oblivion/2 XRM |07// |11Level Editor|15] |03--- |11Level ID : |15|OT |CR");

    value[PROMPT_PAUSE]                   = std::make_pair("Pause Prompt", "|CR |03- |15Hit any key to continue or (|03a|15)bort listing |03-|15 |CR");
    value[PROMPT_INPUT_TEXT]              = std::make_pair("Level Edit Prompt", "|CR|03A|15/dd Level |03E|15/dit Level |03D|15/elete Level |03Q|15/uit : ");
    value[PROMPT_OPTION_INPUT_TEXT]       = std::make_pair("Level Option Edit Prompt", "|CR|03A|15/dd |03E|15/dit |03D|15/elete |03Q|15/uit : ");
    value[PROMPT_INVALID]                 = std::make_pair("Invalid input", "|CR|04Invalid Input! Try again.|CR");

    value[PROMPT_LEVEL_ADD]                = std::make_pair("Level Name To Add", "|CR|15Enter Level to |11CREATE|15 : ");
    value[PROMPT_LEVEL_DELETE]             = std::make_pair("Level Name To Delete", "|CR|15Enter Level to |11DELETE|15 : ");
    value[PROMPT_LEVEL_CHANGE]             = std::make_pair("Level Name To Change", "|CR|15Enter Level to |11EDIT|15 : ");
    value[PROMPT_INVALID_LEVEL_EXISTS]     = std::make_pair("Invalid Level Exists", "|CR|04Invalid, Level already exist.|CR");
    value[PROMPT_INVALID_LEVEL_NOT_EXISTS] = std::make_pair("Invalid Level Doesn't Exist", "|CR|04Invalid, Level doesn't exist.|CR");

    // Menu Field Edit
    value[PROMPT_LEVEL_FIELD_INPUT_TEXT]   = std::make_pair("Level Field Edit Prompt", "|CR|15Level Editor C|07om|08mand |15: |07");


// WIP

    // NOTE, added |PD will display the prompt description as HELP text to the user
    // Usefull when editing fields - Specific to XRM.
    value[PROMPT_LEVEL_FIELD_TITLE]        = std::make_pair("Menu Title (Shown in ANSI screens with 'MT' MCI code)", "|CR|03%   |15|PD|CR|11!   |03(|11A|03) |15Menu Title         : ");
    value[PROMPT_LEVEL_FIELD_PASSWORD]     = std::make_pair("Menu Password (Clear Text right now, will be encrypted lateron)", "|CR|03%   |15|PD|CR|11!   |03(|11B|03) |15Password           : ");
    value[PROMPT_LEVEL_FIELD_FALLBACK]     = std::make_pair("Fallback Menu (Example 'MAIN')", "|CR|03%   |15|PD|CR|11!   |03(|11C|03) |15Fallback Menu      : ");
    value[PROMPT_LEVEL_FIELD_HELP_ID]      = std::make_pair("ANSI Help File (Example 'MATRIX' Without extension) Overrides Generic Menu", "|CR|03%   |15|PD|CR|11!   |03(|11D|03) |15Help ID            : ");
    value[PROMPT_LEVEL_FIELD_NAME]         = std::make_pair("Menu Name Displayed in Prompts", "|CR|03%   |15|PD|CR|11!   |03(|11E|03) |15Name in Prompt     : ");
    value[PROMPT_LEVEL_FIELD_PULLDOWN]     = std::make_pair("Pulldown ANSI with extension (Example 'MATRIX.ANS')", "|CR|03%   |15|PD|CR|11!   |03(|11F|03) |15Pulldown File      : ");

    // Display Page for Menu Fields
    value[DISPLAY_LEVEL_FIELDS_VERSION_ID]        = std::make_pair("File Version ID", "     |15File Version       : ");
    value[DISPLAY_LEVEL_FIELDS_BORDER_ROW_COLOR]  = std::make_pair("Border Row Color", " |07");
    value[DISPLAY_LEVEL_FIELDS_TITLE]             = std::make_pair("Menu Title", " |03(|11A|03) |15Menu Title         : ");
    value[DISPLAY_LEVEL_FIELDS_PASSWORD]          = std::make_pair("Menu Password", " |03(|11B|03) |15Password           : ");
    value[DISPLAY_LEVEL_FIELDS_FALLBACK]          = std::make_pair("Menu Fallback", " |03(|11C|03) |15Fallback Menu      : ");
    value[DISPLAY_LEVEL_FIELDS_HELP_ID]           = std::make_pair("Menu Help ID ANSI", " |03(|11D|03) |15Help ID            : ");
    value[DISPLAY_LEVEL_FIELDS_NAME]              = std::make_pair("Menu Name in Prompt", " |03(|11E|03) |15Name in Prompt     : ");
    value[DISPLAY_LEVEL_FIELDS_PULLDOWN_FILE]     = std::make_pair("Pulldown File", " |03(|11F|03) |15Pulldown File      : ");
    value[DISPLAY_LEVEL_FIELDS_VIEW_GENERIC]      = std::make_pair("View Generic Menu", " |03(|11G|03) |15View Generic Menu    ");
    value[DISPLAY_LEVEL_FIELDS_EDIT_OPTIONS]      = std::make_pair("Edit Options", " |03(|11H|03) |15Edit Options         ");
    value[DISPLAY_LEVEL_FIELDS_QUIT_SAVE]         = std::make_pair("Quit and Save", " |03(|11Q|03) |15Quit & Save          ");
    value[DISPLAY_LEVEL_FIELDS_QUIT_ABORT]        = std::make_pair("Quit without Save", " |03(|11X|03) |15Exit without Saving  ");

    m_text_prompts_dao->writeValue(value);
}

/**
 * @brief Sets an individual input module index.
 * @param mod_function_index
 */
void ModMessageBaseEditor::changeInputModule(int mod_function_index)
{
    // Setup input module
    m_mod_function_index = mod_function_index;
}

/**
 * @brief Sets an individual setup method module index.
 * @param mod_function_index
 */
void ModMessageBaseEditor::changeSetupModule(int mod_function_index)
{
    // Set, and Execute the Setup module.
    m_mod_setup_index = mod_function_index;
    m_setup_functions[m_mod_setup_index]();
}

/**
 * @brief Sets an individual Menu Input State Add/Change/Delete
 * @param mod_menu_state_index
 */
void ModMessageBaseEditor::changeMenuInputState(int mod_menu_state_index)
{
    m_mod_menu_state_index = mod_menu_state_index;
}

/**
 * @brief Redisplay's the current module prompt.
 * @return
 */
void ModMessageBaseEditor::redisplayModulePrompt()
{
    m_setup_functions[m_mod_setup_index]();
}

/**
 * @brief Pull and Display Prompts
 * @param prompt
 */
void ModMessageBaseEditor::displayPrompt(const std::string &prompt)
{
    baseDisplayPrompt(prompt, m_text_prompts_dao);
}

/**
 * @brief Pull and parse and return Display Prompts for use in interfaces
 * @param prompt
 */
std::string ModMessageBaseEditor::getDisplayPrompt(const std::string &prompt)
{
    return baseGetDisplayPrompt(prompt, m_text_prompts_dao);
}

/**
 * @brief Pull and parse and return Display Prompts for use in interfaces
 * @param prompt
 */
std::string ModMessageBaseEditor::getDisplayPromptRaw(const std::string &prompt)
{
    return baseGetDisplayPromptRaw(prompt, m_text_prompts_dao);
}

/**
 * @brief Pull and Display Prompts with MCI Code
 * @param prompt
 * @param mci_field
 */
void ModMessageBaseEditor::displayPromptMCI(const std::string &prompt, const std::string &mci_field)
{
    baseDisplayPromptMCI(prompt, m_text_prompts_dao, mci_field);
}

/**
 * @brief Pull and Display Prompts with following newline
 * @param prompt
 */
void ModMessageBaseEditor::displayPromptAndNewLine(const std::string &prompt)
{
    baseDisplayPromptAndNewLine(prompt, m_text_prompts_dao);
}

/**
 * @brief Setup for the Menu Editor
 * @return
 */
void ModMessageBaseEditor::setupMenuEditor()
{
    displayPrompt(PROMPT_HEADER);

    // Build a list of screen lines for the menu display
    // So we know when to pause in large listing, or use pagination.
    std::string menu_display_output = displayMenuList();

    if(m_menu_display_list.size() == 0)
    {
        // Clear Out list if anything already exists.
        std::vector<std::string>().swap(m_menu_display_list);
    }

    m_menu_display_list = m_common_io.splitString(menu_display_output, '\n');
    m_page = 0;
    displayCurrentPage(PROMPT_INPUT_TEXT);
}

/**
 * @brief Setup for the Menu Editor
 * @return
 */
void ModMessageBaseEditor::setupMenuEditFields()
{
    std::string display_name = m_current_menu;
    baseTransformToUpper(display_name);
    displayPromptMCI(PROMPT_LEVEL_EDIT_HEADER, display_name);

    // Build a list of screen lines for the menu display
    // So we know when to pause in large listing, or use pagination.
    std::string menu_display_output = displayMenuEditScreen();

    if(m_menu_display_list.size() == 0)
    {
        // Clear Out list if anything already exists.
        std::vector<std::string>().swap(m_menu_display_list);
    }

    m_menu_display_list = m_common_io.splitString(menu_display_output, '\n');
    m_page = 0;
    displayCurrentEditPage(PROMPT_LEVEL_FIELD_INPUT_TEXT);
}

/**
 * @brief Displays the current page of menu items
 * @param input_state
 */
void ModMessageBaseEditor::displayCurrentPage(const std::string &input_state)
{
    // calculate the rows_per_page.
    unsigned int rows_used = m_ansi_process->getMaxRowsUsedOnScreen();
    unsigned int max_rows = m_ansi_process->getMaxLines();

    if(m_page > 0)
        rows_used -= (m_ansi_process->m_number_lines - 2);

    m_rows_per_page = max_rows - (rows_used + 2);

    bool displayed_all_rows = true;

    for(unsigned int i = (m_page*(m_rows_per_page-2)); i < m_menu_display_list.size(); i++)
    {
        std::string display_line = m_session_io->pipe2ansi(m_menu_display_list[i]);
        display_line.append("\r\n");
        baseProcessAndDeliver(display_line);

        if(i >= (m_page*m_rows_per_page) + m_rows_per_page)
        {
            // We've displayed the max amount of rows per the current
            // screen break out and wait for prompt or next page.
            displayed_all_rows = false;
            break;
        }
    }

    // Default Page Input Method
    unsigned int current_module_input;

    switch(m_mod_setup_index)
    {
        case MOD_DISPLAY_MENU:
            current_module_input = MOD_MENU_INPUT;
            break;

        default:
            m_log.write<Logging::ERROR_LOG>("Error, forgot to add new STATE index displayCurrentPage!!", __FILE__, __LINE__);
            return;
    }

    // If we displayed all rows, then display prompt, otherwise
    // Ask to hit any key for next page.
    if(displayed_all_rows)
    {
        // Reset Page back to Zero for next display.
        m_page = 0;
        displayPrompt(input_state);
        changeInputModule(current_module_input);
    }
    else
    {
        displayPrompt(PROMPT_PAUSE);
        changeInputModule(MOD_PAUSE);
    }
}

/**
 * @brief Displays the current page of menu items
 * @param input_state
 */
void ModMessageBaseEditor::displayCurrentEditPage(const std::string &input_state)
{
    for(unsigned int i = 0; i < m_menu_display_list.size(); i++)
    {
        std::string display_line = m_session_io->pipe2ansi(m_menu_display_list[i]);
        display_line.append("\r\n");
        baseProcessAndDeliver(display_line);
    }

    // Default Page Input Method
    unsigned int current_module_input;

    switch(m_mod_setup_index)
    {
        case MOD_DISPLAY_MENU_EDIT:
            current_module_input = MOD_MENU_FIELD_INPUT;
            break;

        default:
            m_log.write<Logging::ERROR_LOG>("Error, forgot to add new STATE index displayCurrentEditPage!!", __FILE__, __LINE__);
            return;
    }

    displayPrompt(input_state); // prompt for edit
    changeInputModule(current_module_input); // switch to the input module for field edits.
}

/**
 * @brief Handles Input (Waiting for Any Key Press) on Page Display.
 * @param input
 */
void ModMessageBaseEditor::menuEditorPausedInput(const std::string &input)
{
    std::string current_state_input;
    unsigned int current_module_input;

    switch(m_mod_setup_index)
    {
        case MOD_DISPLAY_MENU:
            current_state_input = PROMPT_INPUT_TEXT;
            current_module_input = MOD_MENU_INPUT;
            break;
    }

    // Check for abort on pause for next.
    if(input.size() == 1 && std::toupper(input[0]) == 'A')
    {
        displayPrompt(current_state_input);
        changeInputModule(current_module_input);
        return;
    }

    // Any input coming in here is valid
    // Increment to next page, then display remaining results.
    ++m_page;
    displayCurrentPage(current_state_input);
}

/**
 * @brief Handles Input (Waiting for Any Key Press) View Generic Menu
 * @param input
 */
void ModMessageBaseEditor::menuEditorDisplayPause(const std::string &)
{
    changeInputModule(MOD_MENU_FIELD_INPUT);
    redisplayModulePrompt();
}

/**
 * @brief Handles Menu Editor Command Selection
 * @param input
 */
void ModMessageBaseEditor::menuEditorInput(const std::string &input)
{
    std::string key = "";
    std::string result = m_session_io->getInputField(input, key, Config::sSingle_key_length);

    // ESC was hit
    if(result == "aborted")
    {
        return;
    }
    else if(result[0] == '\n')
    {
        // Key == 0 on [ENTER] pressed alone. then invalid!
        if(key.size() == 0)
        {
            // Return and don't do anything.
            return;
        }

        baseProcessDeliverNewLine();

        switch(toupper(key[0]))
        {
            case 'A': // Add
                changeMenuInputState(MENU_ADD);
                displayPrompt(PROMPT_LEVEL_ADD);
                changeInputModule(MOD_MENU_NAME);
                break;

            case 'E': // Change/Edit
                changeMenuInputState(MENU_CHANGE);
                displayPrompt(PROMPT_LEVEL_CHANGE);
                changeInputModule(MOD_MENU_NAME);
                break;

            case 'D': // Delete
                changeMenuInputState(MENU_DELETE);
                displayPrompt(PROMPT_LEVEL_DELETE);
                changeInputModule(MOD_MENU_NAME);
                break;

            case 'Q': // Quit
                // Reload fall back, or gosub to last menu!
                m_is_active = false;
                return;

            default:
                redisplayModulePrompt();
                break;
        }
    }
    else
    {
        // Send back the single input received to show client key presses.
        // Only if return data shows a processed key returned.
        if(result != "empty")
        {
            baseProcessDeliverInput(result);
        }
    }
}

/**
 * @brief Handles Menu Field Editor Command Selection
 * @param input
 */
void ModMessageBaseEditor::menuEditorMenuFieldInput(const std::string &input)
{
    std::string key = "";
    std::string result = m_session_io->getInputField(input, key, Config::sSingle_key_length);

    // ESC was hit
    if(result == "aborted")
    {
        return;
    }
    else if(result[0] == '\n')
    {
        // Key == 0 on [ENTER] pressed alone. then invalid!
        if(key.size() == 0)
        {
            // Return and don't do anything.
            return;
        }

        baseProcessDeliverNewLine();

        switch(toupper(key[0]))
        {
            /*
            case 'A': // Menu Title
                m_current_field = toupper(key[0]);
                changeInputModule(MOD_MENU_FIELD);
                displayPrompt(PROMPT_MENU_FIELD_TITLE);
                m_session_io->getInputField("", key, Config::sName_length, m_loaded_level.back()->menu_title);
                break;

            case 'B': // Menu Password
                m_current_field = toupper(key[0]);
                changeInputModule(MOD_MENU_FIELD);
                displayPrompt(PROMPT_MENU_FIELD_PASSWORD);
                m_session_io->getInputField("", key, Config::sName_length, m_loaded_level.back()->menu_password);
                break;

            case 'C': // Menu Password
                m_current_field = toupper(key[0]);
                changeInputModule(MOD_MENU_FIELD);
                displayPrompt(PROMPT_MENU_FIELD_FALLBACK);
                m_session_io->getInputField("", key, Config::sName_length, m_loaded_level.back()->menu_fall_back);
                break;

            case 'D': // Menu Help ID
                m_current_field = toupper(key[0]);
                changeInputModule(MOD_MENU_FIELD);
                displayPrompt(PROMPT_MENU_FIELD_HELP_ID);
                m_session_io->getInputField("", key, Config::sName_length, m_loaded_level.back()->menu_help_file);
                break;

            case 'E': // Menu Name
                m_current_field = toupper(key[0]);
                changeInputModule(MOD_MENU_FIELD);
                displayPrompt(PROMPT_MENU_FIELD_NAME);
                m_session_io->getInputField("", key, Config::sName_length, m_loaded_level.back()->menu_name);
                break;

            case 'F': // Menu Pulldown file
                m_current_field = toupper(key[0]);
                changeInputModule(MOD_MENU_FIELD);
                displayPrompt(PROMPT_MENU_FIELD_PULLDOWN);
                m_session_io->getInputField("", key, Config::sName_length, m_loaded_level.back()->menu_pulldown_file);
                break;

            case 'G': // View Generate Menu
                displayGenericMenu();
                changeInputModule(MOD_DISPLAY_PAUSE);
                break;

            case 'H': // Jump into Options Editing.
                changeInputModule(MOD_MENU_OPTION_INPUT);
                changeSetupModule(MOD_DISPLAY_MENU_OPTIONS);
                break;

            case 'Q': // Quit
                saveMenuChanges();
                std::vector<menu_ptr>().swap(m_loaded_level);

                // Reload fall back, or gosub to Menu Editor Main
                changeInputModule(MOD_MENU_INPUT);
                changeSetupModule(MOD_DISPLAY_MENU);
                return;

            case 'X': // Exit without Saving
                std::vector<menu_ptr>().swap(m_loaded_level);

                // Reload fall back, or gosub to Menu Editor Main
                changeInputModule(MOD_MENU_INPUT);
                changeSetupModule(MOD_DISPLAY_MENU);
                return;
            */
            default:
                redisplayModulePrompt();
                break;
        }
    }
    else
    {
        // Send back the single input received to show client key presses.
        // Only if return data shows a processed key returned.
        if(result != "empty")
        {
            baseProcessDeliverInput(result);
        }
    }
}

/**
 * @brief Handles Field Updates for Menu Data
 * @param input
 */
void ModMessageBaseEditor::menuEditorMenuFieldHandler(const std::string &input)
{
    std::string key = "";
    std::string result = m_session_io->getInputField(input, key, Config::sName_length);

    // ESC was hit
    if(result == "aborted")
    {
        changeInputModule(MOD_MENU_FIELD_INPUT);
        changeSetupModule(MOD_DISPLAY_MENU_EDIT);
        return;
    }
    else if(result[0] == '\n')
    {
        baseProcessDeliverNewLine();

        // Handle the assigned input received for field
        switch(m_current_field)
        {
/*
            case 'A': // Menu Title
                m_loaded_level.back()->menu_title = key;
                break;

            case 'B': // Menu Password
                m_loaded_level.back()->menu_password = key;
                break;

            case 'C': // Menu Fallback
                m_loaded_level.back()->menu_fall_back = key;
                break;

            case 'D': // Menu Help ID
                m_loaded_level.back()->menu_help_file = key;
                break;

            case 'E': // Menu Name
                m_loaded_level.back()->menu_name = key;
                break;

            case 'F': // Menu Pulldown
                m_loaded_level.back()->menu_pulldown_file = key;
                break;
*/
        }

        changeInputModule(MOD_MENU_FIELD_INPUT);
        changeSetupModule(MOD_DISPLAY_MENU_EDIT);
    }
    else
    {
        // Send back the single input received to show client key presses.
        // Only if return data shows a processed key returned.
        if(result != "empty")
        {
            baseProcessDeliverInput(result);
        }
    }
}

/**
 * @brief Handles Menu Name Input, Parses Strings and checks Valid Menus
 * @param input
 */
void ModMessageBaseEditor::menuEditorMenuNameInput(const std::string &input)
{
    std::string key = "";
    std::string result = m_session_io->getInputField(input, key, Config::sName_length);

    // ESC was hit
    if(result == "aborted")
    {
        changeInputModule(MOD_MENU_INPUT);
        redisplayModulePrompt();
        return;
    }
    else if(result[0] == '\n')
    {
        // Key == 0 on [ENTER] pressed alone. then invalid!
        if(key.size() == 0)
        {
            changeInputModule(MOD_MENU_INPUT);
            redisplayModulePrompt();
            return;
        }

        baseProcessDeliverNewLine();

        if(checkMenuExists(key))
        {
            handleMenuInputState(true, key);
        }
        else
        {
            handleMenuInputState(false, key);
        }
    }
    else
    {
        // Send back the single input received to show client key presses.
        // Only if return data shows a processed key returned.
        if(result != "empty")
        {
            baseProcessDeliverInput(result);
        }
    }
}

/**
 * @brief handle each menu separate state and what to do next on input.
 * @param does_menu_exist
 * @param menu_name
 */
void ModMessageBaseEditor::handleMenuInputState(bool does_menu_exist, const std::string &menu_name)
{
    switch(m_mod_menu_state_index)
    {
        case MENU_ADD:
            if(does_menu_exist)
            {
                // Error, can't create a menu that already exists!
                displayPrompt(PROMPT_INVALID_LEVEL_EXISTS);
                displayPrompt(PROMPT_INPUT_TEXT);
                changeInputModule(MOD_MENU_INPUT);
            }
            else
            {
                createNewMenu(menu_name);
                changeInputModule(MOD_MENU_INPUT);
                redisplayModulePrompt();
            }

            break;

        case MENU_CHANGE:
            if(does_menu_exist)
            {
                // Move to new Default setup for Options vs Menus.
                // Also set the curent menu for the system to load
                // to pull the commands from.
                m_current_menu = menu_name;
                changeInputModule(MOD_MENU_FIELD_INPUT);
                changeSetupModule(MOD_DISPLAY_MENU_EDIT);  // Change to Menu Edit Fields!
            }
            else
            {
                // Error, can't remove a menu that doesn't exist!
                displayPrompt(PROMPT_INVALID_LEVEL_NOT_EXISTS);
                displayPrompt(PROMPT_INPUT_TEXT);
                changeInputModule(MOD_MENU_INPUT);
            }

            break;

        case MENU_DELETE:
            if(does_menu_exist)
            {
                deleteExistingMenu(menu_name);
                changeInputModule(MOD_MENU_INPUT);
                redisplayModulePrompt();
            }
            else
            {
                // Error, can't remove a menu that doesn't exist!
                displayPrompt(PROMPT_INVALID_LEVEL_NOT_EXISTS);
                displayPrompt(PROMPT_INPUT_TEXT);
                changeInputModule(MOD_MENU_INPUT);
            }

            break;
    }
}

/**
 * @brief Create a new empty Menu
 * @param menu_name
 */
void ModMessageBaseEditor::createNewMenu(const std::string &) //menu_name)
{
    /*
        // Pre-Load Menu, check access, if not valid, then fall back to previous.
        menu_ptr new_menu(new Menu());

        // Add a default menu option command to the menu
        std::vector<MenuOption> new_menu_options;

        MenuOption new_option;
        new_menu_options.push_back(new_option);
        new_menu->menu_options = new_menu_options;

        // Call MenuDao to save .yaml menu file
        MenuDao mnu(new_menu, menu_name, GLOBAL_MENU_PATH);
        if (!mnu.fileExists())
        {
            mnu.saveMenu(new_menu);
        }
    */
}

/**
 * @brief Create a new empty Menu
 * @param option_index
 */
void ModMessageBaseEditor::createNewMenuOption(unsigned int) //option_index)
{
    /*
        MenuOption new_option;
        new_option.index = option_index;

        reorderMenuIndexesInsertion(option_index);
        m_loaded_level.back()->menu_options.push_back(new_option);

        sort(
            m_loaded_level.back()->menu_options.begin(), m_loaded_level.back()->menu_options.end(),
            [ ] (const MenuOption& lhs, const MenuOption& rhs)
        {
            return lhs.index < rhs.index;
        });
    */
}

/**
 * @brief Delete an existing Menu Option
 * @param option_index
 */
void ModMessageBaseEditor::deleteExistingMenuOption(unsigned int) // option_index)
{
    /*
        unsigned int option_size = m_loaded_level.back()->menu_options.size();

        // Check if the last entry, then no need to swap and just pop_back!
        if (option_index == m_loaded_level.back()->menu_options[option_size-1].index)
        {
            // Last entry, just remove it and done, no need to reorder or sort either.
            m_loaded_level.back()->menu_options.pop_back();
            return;
        }

        // Remove at Index
        for(unsigned int i = 0; i < option_size; i++)
        {
            auto &m = m_loaded_level.back()->menu_options[i];
            if (m.index == option_index)
            {
                // swap with last item then pop_back
                MenuOption temp = m_loaded_level.back()->menu_options[i];
                m_loaded_level.back()->menu_options[i] = m_loaded_level.back()->menu_options[option_size-1];
                m_loaded_level.back()->menu_options[option_size-1] = temp;
                m_loaded_level.back()->menu_options.pop_back();
                break;
            }
        }

        reorderMenuIndexesDeletion(option_index);

        sort(
            m_loaded_level.back()->menu_options.begin(), m_loaded_level.back()->menu_options.end(),
            [ ] (const MenuOption& lhs, const MenuOption& rhs)
        {
            return lhs.index < rhs.index;
        });
    */
}

/**
 * @brief On Insertion of Menu Options, reorder all after index
 * @param option_index
 */
void ModMessageBaseEditor::reorderMenuIndexesInsertion(unsigned int) //option_index)
{
    /*
        for(unsigned int i = 0; i < m_loaded_level.back()->menu_options.size(); i++)
        {
            auto &m = m_loaded_level.back()->menu_options[i];
            if (m.index >= option_index)
            {
                ++m.index;
            }
        }
    */
}

/**
 * @brief On Deletion of Menu Options, reorder all after index
 * @param option_index
 */
void ModMessageBaseEditor::reorderMenuIndexesDeletion(unsigned int) //option_index)
{
    /*
        for(unsigned int i = 0; i < m_loaded_level.back()->menu_options.size(); i++)
        {
            auto &m = m_loaded_level.back()->menu_options[i];
            if (m.index >= option_index)
            {
                --m.index;
            }
        }
    */
}

/**
 * @brief Delete an existing Menu
 * @param menu_name
 */
void ModMessageBaseEditor::deleteExistingMenu(const std::string &)//menu_name)
{
    /*
        // Pre-Load Menu, check access, if not valid, then fall back to previous.
        menu_ptr new_menu(new Menu());

        // Call MenuDao to save .yaml menu file
        MenuDao mnu(new_menu, menu_name, GLOBAL_MENU_PATH);
        if (mnu.fileExists())
        {
            mnu.deleteMenu();
        }
    */
}

/**
 * @brief Create a new empty Menu
 * @param menu_name
 */
void ModMessageBaseEditor::copyExistingMenu(const std::string &) //menu_name)
{
    /*
        // Pre-Load Menu, check access, if not valid, then fall back to previous.
        menu_ptr new_menu(new Menu());

        // First load the Source Menu [m_current_menu] file name
        MenuDao mnu_source(new_menu, m_current_menu, GLOBAL_MENU_PATH);
        if (mnu_source.fileExists())
        {
            mnu_source.loadMenu();
        }
        else
        {
            std::cout << "Source menu file doesn't exist!" << std::endl;
            return;
        }

        // Next Save a new Destination Menu [menu_name] file name
        MenuDao mnu_destination(new_menu, menu_name, GLOBAL_MENU_PATH);
        if (!mnu_destination.fileExists())
        {
            mnu_destination.saveMenu(new_menu);
        }
        else
        {
            std::cout << "Destination menu file already exists!" << std::endl;
        }
    */
}

/**
 * @brief Copy an Existing Menu Option
 * @param option_index
 */
void ModMessageBaseEditor::copyExistingMenuOption(int) //option_index)
{
    /*
        unsigned int option_size = m_loaded_level.back()->menu_options.size();
        for(unsigned int i = 0; i < option_size; i++)
        {
            auto &m = m_loaded_level.back()->menu_options[i];
            if (m.index == m_current_option)
            {
                // Grab source, then update and add as new to the list.
                MenuOption new_option = m_loaded_level.back()->menu_options[i];
                new_option.index = option_index;

                // Update existing indexes and up
                reorderMenuIndexesInsertion(option_index);
                m_loaded_level.back()->menu_options.push_back(new_option);
                break;
            }
        }

        sort(
            m_loaded_level.back()->menu_options.begin(), m_loaded_level.back()->menu_options.end(),
            [ ] (const MenuOption& lhs, const MenuOption& rhs)
        {
            return lhs.index < rhs.index;
        });
    */
}

/**
 * @brief Save Menu Changes
 * @return
 */
void ModMessageBaseEditor::saveMenuChanges()
{
    /*
        MenuDao mnu_source(m_loaded_level.back(), m_current_menu, GLOBAL_MENU_PATH);
        if (mnu_source.saveMenu(m_loaded_level.back()))
        {
            std::cout << "Menu Saved Successful!" << std::endl;
        }
        else
        {
            std::cout << "Menu Save Failed!" << std::endl;
        }
    */
}

/**
 * @brief Check if the menu exists in the current listing
 * @param menu_name
 */
bool ModMessageBaseEditor::checkMenuExists(std::string) //menu_name)
{
    /*
        directory_ptr directory(new Directory());
        std::vector<std::string> result_set = directory->getFileListPerDirectory(GLOBAL_MENU_PATH, "yaml");

        // Append the extension to match the directory files.
        menu_name.append(".yaml");
        baseTransformToLower(menu_name);

        // Case Insensitive Search for Menu name, with transformation to lower case
        for (std::string::size_type i = 0; i < result_set.size(); i++)
        {
            std::string name = result_set[i];
            baseTransformToLower(name);

            if (name == menu_name)
                return true;
        }

    */
    return false;
}

/**
 * @brief Check if the menu option exists in the current listing
 * @param option_index
 */
bool ModMessageBaseEditor::checkMenuOptionExists(unsigned int) //option_index)
{
    /*
        // Check if it's out of bounds, negative already checked on caller.
        if (option_index >= m_loaded_level.back()->menu_options.size())
        {
            return false;
        }

        // Check input index, and get command that matches.
        for(unsigned int i = 0; i < m_loaded_level.back()->menu_options.size(); i++)
        {
            auto &m = m_loaded_level.back()->menu_options[i];
            if (m.index == option_index)
            {
                return true;
            }
        }
    */
    return false;
}

/**
 * @brief Menu Editor, Read and Modify Menus
 * @return
 */
std::string ModMessageBaseEditor::displayMenuList()
{
    /*
        directory_ptr directory(new Directory());
        std::vector<std::string> result_set = directory->getFileListPerDirectory(GLOBAL_MENU_PATH, "yaml");

        // check result set, if no menu then return gracefully.
        if(result_set.size() == 0)
        {
            std::cout << "\r\n*** No Menus .yaml files found!" << std::endl;
            return "No Menu Files found!";
        }

        // Sort Menu's in ascending order
        std::sort(result_set.begin(), result_set.end());

        // iterate through and print out
        int total_rows = result_set.size() / 8;
        int remainder = result_set.size() % 8;

        // Add for Header and Footer Row!
        total_rows += 2;
        if(remainder > 0)
            ++total_rows;

        // Could re-calc this on screen width lateron.
        int max_cols = 73; // out of 80

        // Vector or Menus, Loop through
        std::vector<std::string>::iterator i = result_set.begin();
        std::string menu_name;
        std::string buffer = "";
        for(int rows = 0; rows < total_rows; rows++)
        {
            buffer += "   "; // 3 Leading spaces per row.
            for(int cols = 0; cols < max_cols; cols++)
            {
                // Top Row
                if(rows == 0 && cols == 0)
                {
                    buffer += baseGetDefaultBoxColor();
                    buffer += BORDER_TOP_LEFT;
                }
                else if(rows == 0 && cols == max_cols-1)
                {
                    buffer += baseGetDefaultBoxColor();
                    buffer += BORDER_TOP_RIGHT;
                }
                else if(rows == 0 && cols % 9 == 0)
                {
                    buffer += baseGetDefaultBoxColor();
                    buffer += BORDER_MID_TOP;
                }
                else if(rows == 0)
                {
                    buffer += baseGetDefaultBoxColor();
                    buffer += BORDER_ROW;
                }

                // Bottom Row
                else if(rows == total_rows-1 && cols == 0)
                {
                    buffer += baseGetDefaultBoxColor();
                    buffer += BORDER_BOT_LEFT;
                }
                else if(rows == total_rows-1 && cols == max_cols-1)
                {
                    buffer += baseGetDefaultBoxColor();
                    buffer += BORDER_BOT_RIGHT;
                }
                else if(rows == total_rows-1 && cols % 9 == 0)
                {
                    buffer += baseGetDefaultBoxColor();
                    buffer += BORDER_MID_BOT;
                }
                else if(rows == total_rows-1)
                {
                    buffer += baseGetDefaultBoxColor();
                    buffer += BORDER_ROW;
                }
                else if(cols % 9 == 0)
                {
                    buffer += baseGetDefaultBoxColor();
                    buffer += BORDER_MID;
                }
                else
                {
                    // Here we insert the Menu name and pad through to 8 characters.
                    if(cols % 10 == 0 || cols == 1)
                    {
                        if(i != result_set.end())
                        {
                            // Strip Extension, then pad 8 characters.
                            menu_name = i->substr(0, i->size()-5);
                            menu_name = m_common_io.rightPadding(menu_name, 8);

                            baseTransformToUpper(menu_name);

                            buffer += baseGetDefaultInputColor();
                            buffer += menu_name;
                            ++i;
                        }
                        else
                        {
                            // Empty, 8 Spaces default menu name size.
                            buffer += "        ";
                        }
                    }
                }
            }

            // Were going to split on \n, which will get replaced lateron
            // with \r\n for full carriage returns.
            buffer += "\n";
        }
        return (buffer);
    */
    return "";
}

/**
 * REFERENCE ONLY!
 * @brief Menu Editor, Read and Modify Menus Options
 * @return
 *
std::string ModLevelEditor::displayMenuOptionList()
{

    menu_ptr current_menu = m_loaded_level.back();

    // Build a string list of individual menu options, then loop to fit as many per screen!
    std::vector<std::string> result_set;
    for(unsigned int i = 0; i < current_menu->menu_options.size(); i++)
    {
        auto &m = current_menu->menu_options[i];

        std::string option_string = m_common_io.rightPadding(std::to_string(m.index), 3);

        // Toggled View will show commands strings, while default shows command key info.
        switch (m_mod_toggle_view_index)
        {
            case VIEW_DEFAULT:
                option_string.append(baseGetDefaultColor() + m_common_io.rightPadding(m.menu_key, 9));
                option_string.append(baseGetDefaultInputColor() + m_common_io.rightPadding(m.command_key, 4));
                option_string.append(baseGetDefaultStatColor() + m_common_io.rightPadding(m.acs_string, 8));
                break;

            case VIEW_NAMES:
                option_string.append(baseGetDefaultColor() + m_common_io.rightPadding(m.name, 21));
                break;

            case VIEW_STRINGS:
                option_string.append(baseGetDefaultColor() + m_common_io.rightPadding(m.command_string, 15));
                option_string.append(baseGetDefaultInputColor() + m_common_io.rightPadding("", 1));
                option_string.append(baseGetDefaultStatColor() + m_common_io.rightPadding(m_common_io.boolAlpha(m.hidden), 5));
                break;

            case VIEW_PULLDOWN:
                option_string.append(baseGetDefaultColor() + m_common_io.rightPadding(m.menu_key, 9));
                option_string.append(baseGetDefaultInputColor() + m_common_io.rightPadding(m.command_key, 4));

                if (m.pulldown_id > 0)
                {
                    option_string.append(baseGetDefaultStatColor() + m_common_io.rightPadding(std::to_string(m.pulldown_id), 8));
                }
                else
                {
                    option_string.append(m_common_io.rightPadding("", 8));
                }
        }

        result_set.push_back(option_string);
    }

    // iterate through and print out
    int total_rows = result_set.size() / 3;
    int remainder = result_set.size() % 3;

    // Add for Header and Footer Row!
    total_rows += 2;
    if(remainder > 0)
        ++total_rows;

    // Could re-calc this on screen width lateron.
    int max_cols = 76;

    // Vector or Menus, Loop through
    std::vector<std::string>::iterator i = result_set.begin();
    std::string buffer = "";

    for(int rows = 0; rows < total_rows; rows++)
    {
        buffer += "  "; // 3 Leading spaces per row.
        for(int cols = 0; cols < max_cols; cols++)
        {
            // Top Row
            if(rows == 0 && cols == 0)
            {
                buffer += baseGetDefaultBoxColor();
                buffer += BORDER_TOP_LEFT;
            }
            else if(rows == 0 && cols == max_cols-1)
            {
                buffer += baseGetDefaultBoxColor();
                buffer += BORDER_TOP_RIGHT;
            }
            else if(rows == 0 && cols % 25 == 0)
            {
                buffer += baseGetDefaultBoxColor();
                buffer += BORDER_MID_TOP;
            }
            else if(rows == 0)
            {
                buffer += baseGetDefaultBoxColor();
                buffer += BORDER_ROW;
            }

            // Bottom Row
            else if(rows == total_rows-1 && cols == 0)
            {
                buffer += baseGetDefaultBoxColor();
                buffer += BORDER_BOT_LEFT;
            }
            else if(rows == total_rows-1 && cols == max_cols-1)
            {
                buffer += baseGetDefaultBoxColor();
                buffer += BORDER_BOT_RIGHT;
            }
            else if(rows == total_rows-1 && cols % 25 == 0)
            {
                buffer += baseGetDefaultBoxColor();
                buffer += BORDER_MID_BOT;
            }
            else if(rows == total_rows-1)
            {
                buffer += baseGetDefaultBoxColor();
                buffer += BORDER_ROW;
            }
            else if(cols % 25 == 0)
            {
                buffer += baseGetDefaultBoxColor();
                buffer += BORDER_MID;
            }
            else
            {
                // Here we insert the Menu name and pad through to 8 characters.
                if(cols % 26 == 0 || cols == 1)
                {
                    if(i != result_set.end())
                    {
                        buffer += baseGetDefaultInputColor();
                        buffer += *i;
                        ++i;
                    }
                    else
                    {
                        // Empty, 24 Spaces default menu name size.
                        buffer += "                        ";
                    }
                }
            }
        }

        // Were going to split on \n, which will get replaced lateron
        // with \r\n for full carriage returns.
        buffer += "\n";
    }

    return (buffer);
}
*/

/**
 * @brief Menu Editor, for Displaying Menu Fields to Edit
 * @return
 */
std::string ModMessageBaseEditor::displayMenuEditScreen()
{
    /*
        // Create Menu Pointer then load the menu into it.
        menu_ptr current_menu(new Menu());

        if (m_loaded_level.size() == 0)
        {
            // First load the Source Menu [m_current_menu] file name
            MenuDao mnu_source(current_menu, m_current_menu, GLOBAL_MENU_PATH);
            if (mnu_source.fileExists())
            {
                mnu_source.loadMenu();

                // Save Menu into memory for changes, and rollbacks
                m_loaded_level.push_back(current_menu);
            }
            else
            {
                // Error, drop back to Menu Editor.
                changeSetupModule(MOD_DISPLAY_MENU);
                return "";
            }
        }
        else
        {
            //Menu is already loaded, just assigned to current_menu
            current_menu = m_loaded_level.back();
        }

        // Build a string list of individual menu options, then loop to fit as many per screen!
        std::vector<std::string> result_set;

        result_set.push_back(getDisplayPromptRaw(DISPLAY_MENU_FIELDS_VERSION_ID) + baseGetDefaultStatColor() + m_common_io.rightPadding(current_menu->file_version, 48));
        result_set.push_back(baseGetDefaultPromptColor() + " " + std::string(72, BORDER_ROW) + " ");
        result_set.push_back(getDisplayPromptRaw(DISPLAY_MENU_FIELDS_TITLE) + baseGetDefaultStatColor() + m_common_io.rightPadding(current_menu->menu_title, 48));
        result_set.push_back(getDisplayPromptRaw(DISPLAY_MENU_FIELDS_PASSWORD) + baseGetDefaultStatColor() + m_common_io.rightPadding(current_menu->menu_password, 48));
        result_set.push_back(getDisplayPromptRaw(DISPLAY_MENU_FIELDS_FALLBACK) + baseGetDefaultStatColor() + m_common_io.rightPadding(current_menu->menu_fall_back, 48));
        result_set.push_back(getDisplayPromptRaw(DISPLAY_MENU_FIELDS_HELP_ID) + baseGetDefaultStatColor() + m_common_io.rightPadding(current_menu->menu_help_file, 48));
        result_set.push_back(getDisplayPromptRaw(DISPLAY_MENU_FIELDS_NAME) + baseGetDefaultStatColor() + m_common_io.rightPadding(current_menu->menu_name, 48));
        result_set.push_back(getDisplayPromptRaw(DISPLAY_MENU_FIELDS_PULLDOWN_FILE) + baseGetDefaultStatColor() + m_common_io.rightPadding(current_menu->menu_pulldown_file, 48));
        result_set.push_back(getDisplayPromptRaw(DISPLAY_MENU_FIELDS_VIEW_GENERIC) + baseGetDefaultStatColor() + m_common_io.rightPadding("", 48));
        result_set.push_back(getDisplayPromptRaw(DISPLAY_MENU_FIELDS_EDIT_OPTIONS) + baseGetDefaultStatColor() + m_common_io.rightPadding("", 48));
        result_set.push_back(baseGetDefaultPromptColor() + " " + std::string(72, BORDER_ROW) + " ");
        result_set.push_back(getDisplayPromptRaw(DISPLAY_MENU_FIELDS_QUIT_SAVE) + baseGetDefaultStatColor() + m_common_io.rightPadding("", 48));
        result_set.push_back(getDisplayPromptRaw(DISPLAY_MENU_FIELDS_QUIT_ABORT) + baseGetDefaultStatColor() + m_common_io.rightPadding("", 48));

        // Not in use Yet, seems legacy only does ACS in option commands.
        // option_string.append("Menu ACS           : " + m_common_io.rightPadding(current_menu->menu_acs_string, 35);
        // option_string.append("Menu FormMenu      : " + m_common_io.rightPadding(current_menu->menu_form_menu, 8);

        // iterate through and print out
        int total_rows = result_set.size();
        total_rows += 2;

        // Could re-calc this on screen width lateron.
        int max_cols = 76;

        // Vector or Menus, Loop through
        std::vector<std::string>::iterator i = result_set.begin();
        std::string buffer = "";

        for(int rows = 0; rows < total_rows; rows++)
        {
            buffer += "  "; // 3 Leading spaces per row.
            for(int cols = 0; cols < max_cols; cols++)
            {
                // Top Row
                if(rows == 0 && cols == 0)
                {
                    buffer += baseGetDefaultBoxColor();
                    buffer += BORDER_TOP_LEFT;
                }
                else if(rows == 0 && cols == max_cols-1)
                {
                    buffer += baseGetDefaultBoxColor();
                    buffer += BORDER_TOP_RIGHT;
                }
                else if(rows == 0 && cols % 75 == 0)
                {
                    buffer += baseGetDefaultBoxColor();
                    buffer += BORDER_MID_TOP;
                }
                else if(rows == 0)
                {
                    buffer += baseGetDefaultBoxColor();
                    buffer += BORDER_ROW;
                }

                // Bottom Row
                else if(rows == total_rows-1 && cols == 0)
                {
                    buffer += baseGetDefaultBoxColor();
                    buffer += BORDER_BOT_LEFT;
                }
                else if(rows == total_rows-1 && cols == max_cols-1)
                {
                    buffer += baseGetDefaultBoxColor();
                    buffer += BORDER_BOT_RIGHT;
                }
                else if(rows == total_rows-1 && cols % 75 == 0)
                {
                    buffer += baseGetDefaultBoxColor();
                    buffer += BORDER_MID_BOT;
                }
                else if(rows == total_rows-1)
                {
                    buffer += baseGetDefaultBoxColor();
                    buffer += BORDER_ROW;
                }
                else if(cols % 75 == 0)
                {
                    buffer += baseGetDefaultBoxColor();
                    buffer += BORDER_MID;
                }
                else
                {
                    // Here we insert the Menu name and pad through to 8 characters.
                    if(cols == 1)
                    {
                        if(i != result_set.end())
                        {
                            buffer += *i;
                            ++i;
                        }
                    }
                }
            }

            // Were going to split on \n, which will get replaced lateron
            // with \r\n for full carriage returns.
            buffer += "\n";
        }

        return (buffer);
    */
    return "";
}
