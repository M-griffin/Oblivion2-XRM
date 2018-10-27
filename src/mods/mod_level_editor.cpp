#include "mod_level_editor.hpp"

#include "data-sys/access_level_dao.hpp"
#include "model-sys/access_level.hpp"

#include <sstream>
#include <stdint.h>
#include <string>
#include <vector>

/**
 * @brief Handles Updates or Data Input from Client
 * @return
 */
bool ModLevelEditor::update(const std::string& character_buffer, const bool&)
{
    // Make sure system is active, when system is done, success or failes
    // We change this is inactive to single the login process is completed.
    if(!m_is_active)
    {
        std::cout << "ModLevelEditor() !m_is_active" << std::endl;
        return false;
    }

    // Return True when were keeping module active / else false;
    if(character_buffer.size() == 0)
    {
        std::cout << "ModLevelEditor() !character_buffer size 0" << std::endl;
        return true;
    }

    // Process all incoming data stright to the input functions.
    m_mod_functions[m_mod_function_index](character_buffer);

    return true;
}

/**
 * @brief Start-up class, setup and display initial screens / interface.
 * @return
 */
bool ModLevelEditor::onEnter()
{
    std::cout << "OnEnter() ModLevelEditor\n";
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
bool ModLevelEditor::onExit()
{
    std::cout << "OnExit() ModLevelEditor\n";
    m_is_active = false;
    return true;
}

/**
 * @brief Create Default Text Prompts for module
 */
void ModLevelEditor::createTextPrompts()
{
    // Create Mapping to pass for file creation (default values)
    M_TextPrompt value;

    value[PROMPT_HEADER]                   = std::make_pair("Level Editor Header", "|CS|CR|03--- |15[|03Oblivion/2 XRM |07// |11Level Editor|15] |03--- |CR");
    value[PROMPT_LEVEL_EDIT_HEADER]        = std::make_pair("Level Fields Editor Header |OT Level ID", "|CS|CR|03--- |15[|03Oblivion/2 XRM |07// |11Level Editor|15] |03--- |11Level ID : |15|OT |CR");
    value[PROMPT_PAUSE]                    = std::make_pair("Pause Prompt", "|CR |03- |15Hit any key to continue or (|03a|15)bort listing |03-|15 |CR");
    value[PROMPT_INPUT_TEXT]               = std::make_pair("Level Edit Prompt", "|CR|03A|15/dd Level |03E|15/dit Level |03D|15/elete Level |03Q|15/uit : ");
    value[PROMPT_OPTION_INPUT_TEXT]        = std::make_pair("Level Option Edit Prompt", "|CR|03A|15/dd |03E|15/dit |03D|15/elete |03Q|15/uit : ");
    value[PROMPT_INVALID]                  = std::make_pair("Invalid input", "|CR|04Invalid Input! Try again.|CR");

    value[PROMPT_LEVEL_ADD]                = std::make_pair("Level Value To Add", "|CR|15Enter Level to |11CREATE|15 : ");
    value[PROMPT_LEVEL_DELETE]             = std::make_pair("Level Value To Delete", "|CR|15Enter Level to |11DELETE|15 : ");
    value[PROMPT_LEVEL_CHANGE]             = std::make_pair("Level Value To Change", "|CR|15Enter Level to |11EDIT|15 : ");
    value[PROMPT_INVALID_LEVEL_EXISTS]     = std::make_pair("Invalid Level Exists", "|CR|04Invalid, Level already exist.|CR");
    value[PROMPT_INVALID_LEVEL_NOT_EXISTS] = std::make_pair("Invalid Level Doesn't Exist", "|CR|04Invalid, Level doesn't exist.|CR");

    // Menu Field Edit
    value[PROMPT_LEVEL_FIELD_INPUT_TEXT]   = std::make_pair("Level Field Edit Prompt", "|CR|15Level Editor C|07om|08mand |15: |07");

    // WIP

    // NOTE, added |PD will display the prompt description as HELP text to the user
    // Usefull when editing fields - Specific to XRM.
    value[PROMPT_LEVEL_FIELD_TITLE]      = std::make_pair("Menu Title (Shown in ANSI screens with 'MT' MCI code)", "|CR|03%   |15|PD|CR|11!   |03(|11A|03) |15Menu Title         : ");
    value[PROMPT_LEVEL_FIELD_PASSWORD]   = std::make_pair("Menu Password (Clear Text right now, will be encrypted lateron)", "|CR|03%   |15|PD|CR|11!   |03(|11B|03) |15Password           : ");
    value[PROMPT_LEVEL_FIELD_FALLBACK]   = std::make_pair("Fallback Menu (Example 'MAIN')", "|CR|03%   |15|PD|CR|11!   |03(|11C|03) |15Fallback Menu      : ");
    value[PROMPT_LEVEL_FIELD_HELP_ID]    = std::make_pair("ANSI Help File (Example 'MATRIX' Without extension) Overrides Generic Menu", "|CR|03%   |15|PD|CR|11!   |03(|11D|03) |15Help ID            : ");
    value[PROMPT_LEVEL_FIELD_NAME]       = std::make_pair("Menu Name Displayed in Prompts", "|CR|03%   |15|PD|CR|11!   |03(|11E|03) |15Name in Prompt     : ");
    value[PROMPT_LEVEL_FIELD_PULLDOWN]   = std::make_pair("Pulldown ANSI with extension (Example 'MATRIX.ANS')", "|CR|03%   |15|PD|CR|11!   |03(|11F|03) |15Pulldown File      : ");

    // Display Page for Menu Fields
    value[DISPLAY_LEVEL_FIELDS_VERSION_ID]       = std::make_pair("File Version ID", "     |15File Version       : ");
    value[DISPLAY_LEVEL_FIELDS_BORDER_ROW_COLOR] = std::make_pair("Border Row Color", " |07");
    value[DISPLAY_LEVEL_FIELDS_TITLE]            = std::make_pair("Menu Title", " |03(|11A|03) |15Menu Title         : ");
    value[DISPLAY_LEVEL_FIELDS_PASSWORD]         = std::make_pair("Menu Password", " |03(|11B|03) |15Password           : ");
    value[DISPLAY_LEVEL_FIELDS_FALLBACK]         = std::make_pair("Menu Fallback", " |03(|11C|03) |15Fallback Menu      : ");
    value[DISPLAY_LEVEL_FIELDS_HELP_ID]          = std::make_pair("Menu Help ID ANSI", " |03(|11D|03) |15Help ID            : ");
    value[DISPLAY_LEVEL_FIELDS_NAME]             = std::make_pair("Menu Name in Prompt", " |03(|11E|03) |15Name in Prompt     : ");
    value[DISPLAY_LEVEL_FIELDS_PULLDOWN_FILE]    = std::make_pair("Pulldown File", " |03(|11F|03) |15Pulldown File      : ");
    value[DISPLAY_LEVEL_FIELDS_VIEW_GENERIC]     = std::make_pair("View Generic Menu", " |03(|11G|03) |15View Generic Menu    ");
    value[DISPLAY_LEVEL_FIELDS_EDIT_OPTIONS]     = std::make_pair("Edit Options", " |03(|11H|03) |15Edit Options         ");
    value[DISPLAY_LEVEL_FIELDS_QUIT_SAVE]        = std::make_pair("Quit and Save", " |03(|11Q|03) |15Quit & Save          ");
    value[DISPLAY_LEVEL_FIELDS_QUIT_ABORT]       = std::make_pair("Quit without Save", " |03(|11X|03) |15Exit without Saving  ");

    m_text_prompts_dao->writeValue(value);
}

/**
 * @brief Sets an individual input module index.
 * @param mod_function_index
 */
void ModLevelEditor::changeInputModule(int mod_function_index)
{
    // Setup input module
    m_mod_function_index = mod_function_index;
}

/**
 * @brief Sets an individual setup method module index.
 * @param mod_function_index
 */
void ModLevelEditor::changeSetupModule(int mod_function_index)
{
    // Set, and Execute the Setup module.
    m_mod_setup_index = mod_function_index;
    m_setup_functions[m_mod_setup_index]();
}

/**
 * @brief Sets an individual Level Input State Add/Change/Delete
 * @param mod_level_state_index
 */
void ModLevelEditor::changeLevelInputState(int mod_level_state_index)
{
    m_mod_level_state_index = mod_level_state_index;
}

/**
 * @brief Redisplay's the current module prompt.
 * @return
 */
void ModLevelEditor::redisplayModulePrompt()
{
    m_setup_functions[m_mod_setup_index]();
}

/**
 * @brief Pull and Display Prompts
 * @param prompt
 */
void ModLevelEditor::displayPrompt(const std::string& prompt)
{
    baseDisplayPrompt(prompt, m_text_prompts_dao);
}

/**
 * @brief Pull and parse and return Display Prompts for use in interfaces
 * @param prompt
 */
std::string ModLevelEditor::getDisplayPrompt(const std::string& prompt)
{
    return baseGetDisplayPrompt(prompt, m_text_prompts_dao);
}

/**
 * @brief Pull and parse and return Display Prompts for use in interfaces
 * @param prompt
 */
std::string ModLevelEditor::getDisplayPromptRaw(const std::string& prompt)
{
    return baseGetDisplayPromptRaw(prompt, m_text_prompts_dao);
}

/**
 * @brief Pull and Display Prompts with MCI Code
 * @param prompt
 * @param mci_field
 */
void ModLevelEditor::displayPromptMCI(const std::string& prompt, const std::string& mci_field)
{
    baseDisplayPromptMCI(prompt, m_text_prompts_dao, mci_field);
}

/**
 * @brief Pull and Display Prompts with following newline
 * @param prompt
 */
void ModLevelEditor::displayPromptAndNewLine(const std::string& prompt)
{
    baseDisplayPromptAndNewLine(prompt, m_text_prompts_dao);
}

/**
 * @brief Setup for the Menu Editor
 * @return
 */
void ModLevelEditor::setupLevelEditor()
{
    displayPrompt(PROMPT_HEADER);

    // Build a list of screen lines for the menu display
    // So we know when to pause in large listing, or use pagenation.
    std::string level_display_output = displayLevelList();

    if(m_level_display_list.size() > 0)
    {
        // Clear Out list if anything already exists.
        std::vector<std::string>().swap(m_level_display_list);
    }

    m_level_display_list = m_common_io.splitString(level_display_output, '\n');
    m_page = 0;
    displayCurrentPage(PROMPT_INPUT_TEXT);
}

/**
 * @brief Setup for the Menu Editor
 * @return
 */
void ModLevelEditor::setupLevelEditFields()
{
    std::string display_name = std::to_string(m_current_level);
    baseTransformToUpper(display_name);
    displayPromptMCI(PROMPT_LEVEL_EDIT_HEADER, display_name);

    // Build a list of screen lines for the menu display
    // So we know when to pause in large listing, or use pagenation.
    std::string level_display_output = displayLevelEditScreen();

    if(m_level_display_list.size() > 0)
    {
        // Clear Out list if anything already exists.
        std::vector<std::string>().swap(m_level_display_list);
    }

    m_level_display_list = m_common_io.splitString(level_display_output, '\n');
    m_page = 0;
    displayCurrentEditPage(PROMPT_LEVEL_FIELD_INPUT_TEXT);
}

/**
 * @brief Displays the current page of menu items
 * @param input_state
 */
void ModLevelEditor::displayCurrentPage(const std::string& input_state)
{
    // calculate the rows_per_page.
    unsigned int rows_used = m_ansi_process->getMaxRowsUsedOnScreen();
    unsigned int max_rows = m_ansi_process->getMaxLines();

    if(m_page > 0)
        rows_used -= (m_ansi_process->m_number_lines - 2);

    m_rows_per_page = max_rows - (rows_used + 2);

    bool displayed_all_rows = true;

    for(unsigned int i = (m_page * (m_rows_per_page - 2)); i < m_level_display_list.size(); i++)
    {
        std::string display_line = m_session_io.pipe2ansi(m_level_display_list[i]);
        display_line.append("\r\n");
        baseProcessAndDeliver(display_line);

        if(i >= (m_page * m_rows_per_page) + m_rows_per_page)
        {
            // We've displayed the max amount of rows per the currnet
            // screen break out and wait for prompt or next page.
            displayed_all_rows = false;
            break;
        }
    }

    // Default Page Input Method
    unsigned int current_module_input;

    switch(m_mod_setup_index)
    {
        case MOD_DISPLAY_LEVEL:
            current_module_input = MOD_LEVEL_INPUT;
            break;

        default:
            std::cout << "Error, forgot to add new STATE index displayCurrentPage!!";
            return;
    }

    // If we displayed all rows, then display propmpt, otherwise
    // Ask to hit anykey for next page.
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
void ModLevelEditor::displayCurrentEditPage(const std::string& input_state)
{
    for(unsigned int i = 0; i < m_level_display_list.size(); i++)
    {
        std::string display_line = m_session_io.pipe2ansi(m_level_display_list[i]);
        display_line.append("\r\n");
        baseProcessAndDeliver(display_line);
    }

    // Default Page Input Method
    unsigned int current_module_input;

    switch(m_mod_setup_index)
    {
        case MOD_DISPLAY_LEVEL_EDIT:
            current_module_input = MOD_LEVEL_FIELD_INPUT;
            break;

        default:
            std::cout << "Error, forgot to add new STATE index displayCurrentEditPage!!";
            return;
    }

    displayPrompt(input_state);              // prompt for edit
    changeInputModule(current_module_input); // switch to the input module for field edits.
}

/**
 * @brief Handles Input (Waiting for Any Key Press) on Page Display.
 * @param input
 */
void ModLevelEditor::levelEditorPausedInput(const std::string& input)
{
    std::string current_state_input;
    unsigned int current_module_input;

    switch(m_mod_setup_index)
    {
        case MOD_DISPLAY_LEVEL:
            current_state_input = PROMPT_INPUT_TEXT;
            current_module_input = MOD_LEVEL_INPUT;
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
void ModLevelEditor::levelEditorDisplayPause(const std::string&)
{
    changeInputModule(MOD_LEVEL_FIELD_INPUT);
    redisplayModulePrompt();
}

/**
 * @brief Handles Menu Editor Command Selection
 * @param input
 */
void ModLevelEditor::levelEditorInput(const std::string& input)
{
    std::string key = "";
    std::string result = m_session_io.getInputField(input, key, Config::sSingle_key_length);

    // ESC was hit
    if(result == "aborted")
    {
        std::cout << "aborted!" << std::endl;
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
                changeLevelInputState(LEVEL_ADD);
                displayPrompt(PROMPT_LEVEL_ADD);
                changeInputModule(MOD_LEVEL_NAME);
                break;

            case 'E': // Change/Edit
                changeLevelInputState(LEVEL_CHANGE);
                displayPrompt(PROMPT_LEVEL_CHANGE);
                changeInputModule(MOD_LEVEL_NAME);
                break;

            case 'D': // Delete
                changeLevelInputState(LEVEL_DELETE);
                displayPrompt(PROMPT_LEVEL_DELETE);
                changeInputModule(MOD_LEVEL_NAME);
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
void ModLevelEditor::levelEditorLevelFieldInput(const std::string& input)
{
    std::string key = "";
    std::string result = m_session_io.getInputField(input, key, Config::sSingle_key_length);

    // ESC was hit
    if(result == "aborted")
    {
        std::cout << "aborted!" << std::endl;
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

        /*
                , sName("")
                , sStartMenu("")
                , iLevel(0)
                , iFileLevel(0)
                , iMessageLevel(0)
                , iPostCallRatio(0)
                , iFileRatio(0)
                , iTimeLimit(0)
                , iCallLimit(0)
                , iDownloads(0)
                , iDownloadMB(0)
                , iARFlags1(0)
                , iARFlags2(0)
                // Flags
                , bPostCallRatio(false)
                , bFileRatio(false)
                , bTimeLimit(false)
                , bCallLimit(false)
                , bDownloads(false)
                , bDownloadMB(false)
        */

        switch(toupper(key[0]))
        {
            /*
            case 'A': // Menu Title
                m_current_field = toupper(key[0]);
                changeInputModule(MOD_LEVEL_FIELD);
                displayPrompt(PROMPT_LEVEL_FIELD_TITLE);
                m_session_io.getInputField("", key, Config::sName_length, m_loaded_level.back()->menu_title);
                break;

            case 'B': // Menu Password
                m_current_field = toupper(key[0]);
                changeInputModule(MOD_LEVEL_FIELD);
                displayPrompt(PROMPT_LEVEL_FIELD_PASSWORD);
                m_session_io.getInputField("", key, Config::sName_length, m_loaded_level.back()->menu_password);
                break;

            case 'C': // Menu Password
                m_current_field = toupper(key[0]);
                changeInputModule(MOD_LEVEL_FIELD);
                displayPrompt(PROMPT_LEVEL_FIELD_FALLBACK);
                m_session_io.getInputField("", key, Config::sName_length, m_loaded_level.back()->menu_fall_back);
                break;

            case 'D': // Menu Help ID
                m_current_field = toupper(key[0]);
                changeInputModule(MOD_LEVEL_FIELD);
                displayPrompt(PROMPT_LEVEL_FIELD_HELP_ID);
                m_session_io.getInputField("", key, Config::sName_length, m_loaded_level.back()->menu_help_file);
                break;

            case 'E': // Menu Name
                m_current_field = toupper(key[0]);
                changeInputModule(MOD_LEVEL_FIELD);
                displayPrompt(PROMPT_LEVEL_FIELD_NAME);
                m_session_io.getInputField("", key, Config::sName_length, m_loaded_level.back()->level_code);
                break;

            case 'F': // Menu Pulldown file
                m_current_field = toupper(key[0]);
                changeInputModule(MOD_LEVEL_FIELD);
                displayPrompt(PROMPT_LEVEL_FIELD_PULLDOWN);
                m_session_io.getInputField("", key, Config::sName_length,
            m_loaded_level.back()->menu_pulldown_file); break;

            case 'G': // View Generate Menu
                displayGenericMenu();
                changeInputModule(MOD_DISPLAY_PAUSE);
                break;

            case 'H': // Jump into Options Editing.
                changeInputModule(MOD_LEVEL_OPTION_INPUT);
                changeSetupModule(MOD_DISPLAY_LEVEL_OPTIONS);
                break;

            case 'Q': // Quit
                saveMenuChanges();
                std::vector<menu_ptr>().swap(m_loaded_level);

                // Reload fall back, or gosub to Menu Editor Main
                changeInputModule(MOD_LEVEL_INPUT);
                changeSetupModule(MOD_DISPLAY_LEVEL);
                return;

            case 'X': // Exit without Saving
                std::vector<menu_ptr>().swap(m_loaded_level);

                // Reload fall back, or gosub to Menu Editor Main
                changeInputModule(MOD_LEVEL_INPUT);
                changeSetupModule(MOD_DISPLAY_LEVEL);
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
void ModLevelEditor::levelEditorLevelFieldHandler(const std::string& input)
{
    std::string key = "";
    std::string result = m_session_io.getInputField(input, key, Config::sName_length);

    // ESC was hit
    if(result == "aborted")
    {
        std::cout << "aborted!" << std::endl;
        changeInputModule(MOD_LEVEL_FIELD_INPUT);
        changeSetupModule(MOD_DISPLAY_LEVEL_EDIT);
        return;
    }
    else if(result[0] == '\n')
    {
        baseProcessDeliverNewLine();

        /*
                , sName("")
                , sStartMenu("")
                , iLevel(0)
                , iFileLevel(0)
                , iMessageLevel(0)
                , iPostCallRatio(0)
                , iFileRatio(0)
                , iTimeLimit(0)
                , iCallLimit(0)
                , iDownloads(0)
                , iDownloadMB(0)
                , iARFlags1(0)
                , iARFlags2(0)
                // Flags
                , bPostCallRatio(false)
                , bFileRatio(false)
                , bTimeLimit(false)
                , bCallLimit(false)
                , bDownloads(false)
                , bDownloadMB(false)
                */

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
                                m_loaded_level.back()->level_code = key;
                                break;

                            case 'F': // Menu Pulldown
                                m_loaded_level.back()->menu_pulldown_file = key;
                                break;
                */
        }

        changeInputModule(MOD_LEVEL_FIELD_INPUT);
        changeSetupModule(MOD_DISPLAY_LEVEL_EDIT);
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
 * @brief Handles Level Input, Parses Strings and checks Valid Menus
 * @param input
 */
void ModLevelEditor::levelEditorLevelInput(const std::string& input)
{
    std::string key = "";
    std::string result = m_session_io.getInputField(input, key, Config::sName_length);

    // ESC was hit
    if(result == "aborted")
    {
        std::cout << "aborted!" << std::endl;
        changeInputModule(MOD_LEVEL_INPUT);
        redisplayModulePrompt();
        return;
    }
    else if(result[0] == '\n')
    {
        // Key == 0 on [ENTER] pressed alone. then invalid!
        if(key.size() == 0)
        {
            changeInputModule(MOD_LEVEL_INPUT);
            redisplayModulePrompt();
            return;
        }

        baseProcessDeliverNewLine();

        int level_code = -1;
        std::stringstream ss(key);
        ss >> level_code;

        // check for Invalid Index.
        if(ss.fail() || level_code < 0)
        {
            ss.clear();
            level_code = -1;
        }

        if(level_code != -1 && checkLevelExistsByLevel(level_code))
        {
            handleLevelInputState(true, level_code);
        }
        else
        {
            handleLevelInputState(false, level_code);
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
 * @param does_level_exist
 * @param level_code
 */
void ModLevelEditor::handleLevelInputState(bool does_level_exist, int level_code)
{
    switch(m_mod_level_state_index)
    {
        case LEVEL_ADD:
            if(does_level_exist)
            {
                // Error, can't create a menu that already exists!
                displayPrompt(PROMPT_INVALID_LEVEL_EXISTS);
                displayPrompt(PROMPT_INPUT_TEXT);
                changeInputModule(MOD_LEVEL_INPUT);
            }
            else
            {
                createNewLevel(level_code);
                changeInputModule(MOD_LEVEL_INPUT);
                redisplayModulePrompt();
            }

            break;

        case LEVEL_CHANGE:
            if(does_level_exist)
            {
                // Move to new Default setup for Options vs Menus.
                // Also set the curent menu for the system to load
                // to pull the commands from.
                m_current_level = level_code;
                changeInputModule(MOD_LEVEL_FIELD_INPUT);
                changeSetupModule(MOD_DISPLAY_LEVEL_EDIT); // Change to Level Edit Fields!
            }
            else
            {
                // Error, can't remove a menu that doesn't exist!
                displayPrompt(PROMPT_INVALID_LEVEL_NOT_EXISTS);
                displayPrompt(PROMPT_INPUT_TEXT);
                changeInputModule(MOD_LEVEL_INPUT);
            }

            break;

        case LEVEL_DELETE:
            if(does_level_exist)
            {
                deleteExistingLevel(level_code);
                changeInputModule(MOD_LEVEL_INPUT);
                redisplayModulePrompt();
            }
            else
            {
                // Error, can't remove a menu that doesn't exist!
                displayPrompt(PROMPT_INVALID_LEVEL_NOT_EXISTS);
                displayPrompt(PROMPT_INPUT_TEXT);
                changeInputModule(MOD_LEVEL_INPUT);
            }

            break;
    }
}

/**
 * @brief Create a new empty Level
 * @param level_code
 */
void ModLevelEditor::createNewLevel(int level_code)
{
    access_level_dao_ptr level_dao(new AccessLevelDao(m_session_data->m_user_database));
    access_level_ptr new_level(new AccessLevel());

    // Test Distincation Level
    access_level_ptr destination_level = level_dao->getAccessLevelByLevel(level_code);

    if(checkLevelExistsByLevel(level_code))
    {
        std::cout << "Destination level already exists!" << std::endl;
        return;
    }

    if(level_dao->insertRecord(new_level) < 0)
    {
        std::cout << "Error, unable to insert new level: " << level_code << std::endl;
    }
}

/**
 * @brief Delete an existing Menu
 * @param level_code
 */
void ModLevelEditor::deleteExistingLevel(int level_code)
{
    access_level_dao_ptr level_dao(new AccessLevelDao(m_session_data->m_user_database));
    access_level_ptr existing_level = level_dao->getAccessLevelByLevel(level_code);

    if(existing_level->iId == -1 || !level_dao->deleteRecord(existing_level->iId))
    {
        std::cout << "Error, unable to delete existing level: " << level_code << std::endl;
    }
}

/**
 * @brief Create a new empty Menu
 * @param level_code
 */
void ModLevelEditor::copyExistingLevel(int level_code)
{
    access_level_dao_ptr level_dao(new AccessLevelDao(m_session_data->m_user_database));
    access_level_ptr existing_level = level_dao->getAccessLevelByLevel(m_current_level);

    if(checkLevelExistsByLevel(level_code))
    {
        std::cout << "Destination level already exists!" << std::endl;
        return;
    }

    existing_level->iId = -1;
    existing_level->iLevel = level_code;
    existing_level->sName = "Copied Level for " + std::to_string(level_code);

    if(level_dao->insertRecord(existing_level) < 0)
    {
        std::cout << "Error, unable to copy existing level: " << m_current_level << " to "
                  << level_code << std::endl;
    }
}

/**
 * @brief Save Menu Changes
 * @return
 */
void ModLevelEditor::saveLevelChanges()
{
    access_level_dao_ptr level_dao(new AccessLevelDao(m_session_data->m_user_database));
    access_level_ptr existing_level = nullptr;

    for(unsigned int i = 0; i < m_loaded_levels.size(); i++)
    {
        if(m_loaded_levels[i]->iLevel == m_current_level)
        {
            existing_level = m_loaded_levels[i];
        }
    }

    if(existing_level == nullptr || !level_dao->updateRecord(existing_level))
    {
        std::cout << "Error, unable to update existing level: " << m_current_level;
    }
}

/**
 * @brief Check if the level exists in the current listing
 * @param level_code
 */
bool ModLevelEditor::checkLevelExistsByLevel(int level_code)
{
    // Case Insensitive Search for Menu name, with transformation to lower case
    for(std::string::size_type i = 0; i < m_loaded_levels.size(); i++)
    {
        if(m_loaded_levels[i]->iLevel == level_code)
            return true;
    }

    return false;
}

/**
 * @brief Level Editor, Read and Modify
 * @return
 */
std::string ModLevelEditor::displayLevelList()
{
    access_level_dao_ptr level_dao(new AccessLevelDao(m_session_data->m_user_database));

    // Clear All Levels
    if(m_loaded_levels.size() > 0)
        std::vector<access_level_ptr>().swap(m_loaded_levels);

    m_loaded_levels = level_dao->getAllRecords();

    // If no records, add message to user.
    std::vector<std::string> result_set;

    if(m_loaded_levels.size() == 0)
    {
        result_set.push_back(baseGetDefaultStatColor() + m_common_io.rightPadding("No Records Found!", 24));
    }

    // Build a string list of individual menu options, then loop to fit as many per screen!
    for(unsigned int i = 0; i < m_loaded_levels.size(); i++)
    {
        std::string option_string = m_common_io.rightPadding(std::to_string(m_loaded_levels[i]->iLevel), 5);
        option_string.append(baseGetDefaultStatColor() + m_common_io.rightPadding(m_loaded_levels[i]->sName, 19));
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

/**
 * REFERENCE ONLY!
 * @brief Menu Editor, Read and Modify Menus Options
 * @return
 *
std::string ModLevelEditor::displayLevelOptionList()
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
                option_string.append(baseGetDefaultStatColor() +
m_common_io.rightPadding(m_common_io.boolAlpha(m.hidden), 5)); break;

            case VIEW_PULLDOWN:
                option_string.append(baseGetDefaultColor() + m_common_io.rightPadding(m.menu_key, 9));
                option_string.append(baseGetDefaultInputColor() + m_common_io.rightPadding(m.command_key, 4));

                if (m.pulldown_id > 0)
                {
                    option_string.append(baseGetDefaultStatColor() +
m_common_io.rightPadding(std::to_string(m.pulldown_id), 8));
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
std::string ModLevelEditor::displayLevelEditScreen()
{
    /*
        // Create Menu Pointer then load the menu into it.
        menu_ptr current_menu(new Menu());

        if (m_loaded_level.size() == 0)
        {
            // First load the Source Menu [m_current_menu] file name
            MenuDao mnu_source(current_menu, m_current_menu, GLOBAL_LEVEL_PATH);
            if (mnu_source.fileExists())
            {
                mnu_source.loadMenu();

                // Save Menu into memory for changes, and rollbacks
                m_loaded_level.push_back(current_menu);
            }
            else
            {
                // Error, drop back to Menu Editor.
                changeSetupModule(MOD_DISPLAY_LEVEL);
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


                , sName("")
                , sStartMenu("")
                , iLevel(0)
                , iFileLevel(0)
                , iMessageLevel(0)
                , iPostCallRatio(0)
                , iFileRatio(0)
                , iTimeLimit(0)
                , iCallLimit(0)
                , iDownloads(0)
                , iDownloadMB(0)
                , iARFlags1(0)
                , iARFlags2(0)
                // Flags
                , bPostCallRatio(false)
                , bFileRatio(false)
                , bTimeLimit(false)
                , bCallLimit(false)
                , bDownloads(false)
                , bDownloadMB(false)


           result_set.push_back(getDisplayPromptRaw(DISPLAY_LEVEL_FIELDS_VERSION_ID) + baseGetDefaultStatColor() +
          m_common_io.rightPadding(current_menu->file_version, 48)); result_set.push_back(baseGetDefaultPromptColor() + " "
          + std::string(72, BORDER_ROW) + " "); result_set.push_back(getDisplayPromptRaw(DISPLAY_LEVEL_FIELDS_TITLE) +
          baseGetDefaultStatColor() + m_common_io.rightPadding(current_menu->menu_title, 48));
           result_set.push_back(getDisplayPromptRaw(DISPLAY_LEVEL_FIELDS_PASSWORD) + baseGetDefaultStatColor() +
          m_common_io.rightPadding(current_menu->menu_password, 48));
           result_set.push_back(getDisplayPromptRaw(DISPLAY_LEVEL_FIELDS_FALLBACK) + baseGetDefaultStatColor() +
          m_common_io.rightPadding(current_menu->menu_fall_back, 48));
           result_set.push_back(getDisplayPromptRaw(DISPLAY_LEVEL_FIELDS_HELP_ID) + baseGetDefaultStatColor() +
          m_common_io.rightPadding(current_menu->menu_help_file, 48));
           result_set.push_back(getDisplayPromptRaw(DISPLAY_LEVEL_FIELDS_NAME) + baseGetDefaultStatColor() +
          m_common_io.rightPadding(current_menu->level_code, 48));
           result_set.push_back(getDisplayPromptRaw(DISPLAY_LEVEL_FIELDS_PULLDOWN_FILE) + baseGetDefaultStatColor() +
          m_common_io.rightPadding(current_menu->menu_pulldown_file, 48));
           result_set.push_back(getDisplayPromptRaw(DISPLAY_LEVEL_FIELDS_VIEW_GENERIC) + baseGetDefaultStatColor() +
          m_common_io.rightPadding("", 48)); result_set.push_back(getDisplayPromptRaw(DISPLAY_LEVEL_FIELDS_EDIT_OPTIONS) +
          baseGetDefaultStatColor() + m_common_io.rightPadding("", 48)); result_set.push_back(baseGetDefaultPromptColor() +
          " " + std::string(72, BORDER_ROW) + " ");
           result_set.push_back(getDisplayPromptRaw(DISPLAY_LEVEL_FIELDS_QUIT_SAVE) + baseGetDefaultStatColor() +
          m_common_io.rightPadding("", 48)); result_set.push_back(getDisplayPromptRaw(DISPLAY_LEVEL_FIELDS_QUIT_ABORT) +
          baseGetDefaultStatColor() + m_common_io.rightPadding("", 48));

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
