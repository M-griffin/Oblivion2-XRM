#include "mod_level_editor.hpp"

#include "data-sys/access_level_dao.hpp"
#include "model-sys/access_level.hpp"
#include "../access_condition.hpp"

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
    value[PROMPT_INPUT_TEXT]               = std::make_pair("Level Edit Prompt", "|CR|03A|15/dd Level |03E|15/dit Level |03C|15/opy Level |03D|15/elete Level |03Q|15/uit : ");
    value[PROMPT_OPTION_INPUT_TEXT]        = std::make_pair("Level Option Edit Prompt", "|CR|03A|15/dd |03E|15/dit |03C|15/opy |03D|15/elete |03Q|15/uit : ");
    value[PROMPT_INVALID]                  = std::make_pair("Invalid input", "|CR|04Invalid Input! Try again.|CR");

    value[PROMPT_LEVEL_ADD]                = std::make_pair("Level Value To Add", "|CR|15Enter Level to |11CREATE|15 : ");
    value[PROMPT_LEVEL_DELETE]             = std::make_pair("Level Value To Delete", "|CR|15Enter Level to |11DELETE|15 : ");
    value[PROMPT_LEVEL_CHANGE]             = std::make_pair("Level Value To Change", "|CR|15Enter Level to |11EDIT|15 : ");
    value[PROMPT_LEVEL_COPY_FROM]          = std::make_pair("Level Value To Copy From", "|CR|15Enter level to |11COPY|15 : ");
    value[PROMPT_LEVEL_COPY_TO]            = std::make_pair("Level Value To Copy To", "|15Enter level to |11SAVE|15 as : ");

    value[PROMPT_INVALID_LEVEL_EXISTS]     = std::make_pair("Invalid Level Exists", "|CR|04Invalid, Level already exist.|CR");
    value[PROMPT_INVALID_LEVEL_NOT_EXISTS] = std::make_pair("Invalid Level Doesn't Exist", "|CR|04Invalid, Level doesn't exist.|CR");

    // Menu Field Edit
    value[PROMPT_LEVEL_FIELD_INPUT_TEXT]   = std::make_pair("Level Field Edit Prompt", "|CR|15Level Editor C|07om|08mand |15: |07");

    // NOTE, added |PD will display the prompt description as HELP text to the user
    // Usefull when editing fields - Specific to XRM.
    value[PROMPT_LEVEL_FIELD_NAME]            = std::make_pair("Level Name", "|CR|03%   |15|PD|CR|11!   |03(|11A|03) |15Level Name         : ");
    value[PROMPT_LEVEL_FIELD_START_MENU]      = std::make_pair("Start Menu", "|CR|03%   |15|PD|CR|11!   |03(|11B|03) |15Start Menu         : ");
    value[PROMPT_LEVEL_FIELD_FILE_LEVEL]      = std::make_pair("File Level", "|CR|03%   |15|PD|CR|11!   |03(|11C|03) |15File Level         : ");
    value[PROMPT_LEVEL_FIELD_MESG_LEVEL]      = std::make_pair("Message Level", "|CR|03%   |15|PD|CR|11!   |03(|11D|03) |15Message Level      : ");
    value[PROMPT_LEVEL_FIELD_POST_CALL_RATIO] = std::make_pair("Post/Call Ratio", "|CR|03%   |15|PD|CR|11!   |03(|11E|03) |15Post/Call Ratio    : ");
    value[PROMPT_LEVEL_FIELD_FILE_RATIO]      = std::make_pair("File Ratio - Uploads to Downloads", "|CR|03%   |15|PD|CR|11!   |03(|11F|03) |15File Ratio         : ");
    value[PROMPT_LEVEL_FIELD_TIME_LIMIT]      = std::make_pair("Time Limit Per Day", "|CR|03%   |15|PD|CR|11!   |03(|11G|03) |15Time Limit         : ");
    value[PROMPT_LEVEL_FIELD_CALL_LIMIT]      = std::make_pair("Call Limit Per Day", "|CR|03%   |15|PD|CR|11!   |03(|11H|03) |15Call Limit         : ");
    value[PROMPT_LEVEL_FIELD_DOWNLOADS]       = std::make_pair("Download Limits By # of Files", "|CR|03%   |15|PD|CR|11!   |03(|11I|03) |15Download Files     : ");
    value[PROMPT_LEVEL_FIELD_DOWNLOAD_MB]     = std::make_pair("Download Limit in MBs", "|CR|03%   |15|PD|CR|11!   |03(|11J|03) |15Download MBs       : ");
    value[PROMPT_LEVEL_FIELD_ARFLAGS1]        = std::make_pair("Access Restriction Flags Group 1 - Type Letters to Add/Remove/Toggle", "|CR|03%   |15|PD|CR|11!   |03(|11K|03) |15AR Flags 1         : ");
    value[PROMPT_LEVEL_FIELD_ARFLAGS2]        = std::make_pair("Access Restriction Flags Group 2 - Type Letters to Add/Remove/Toggle", "|CR|03%   |15|PD|CR|11!   |03(|11L|03) |15AR Flags 2         : ");

    value[PROMPT_LEVEL_BOOL_POST_CALL_RATIO]  = std::make_pair("Enable Post/Call Ratio", "|CR|03%   |15|PD|CR|11!   |03(|11M|03) |15Use Post/Call Ratio |07(|15T|07/|15F|07)|15: ");
    value[PROMPT_LEVEL_BOOL_FILE_RATIO]       = std::make_pair("Enable File Ratio", "|CR|03%   |15|PD|CR|11!   |03(|11N|03) |15Use File Ratio |07(|15T|07/|15F|07)|15    : ");
    value[PROMPT_LEVEL_BOOL_TIME_LIMIT]       = std::make_pair("Enable Time Limit", "|CR|03%   |15|PD|CR|11!   |03(|11P|03) |15Use Time Limit |07(|15T|07/|15F|07)|15    : ");
    value[PROMPT_LEVEL_BOOL_CALL_LIMIT]       = std::make_pair("Enable Call Limit", "|CR|03%   |15|PD|CR|11!   |03(|11P|03) |15Use Call Limit |07(|15T|07/|15F|07)|15    : ");
    value[PROMPT_LEVEL_BOOL_DOWNLOADS]        = std::make_pair("Enable Download Limits by Files", "|CR|03%   |15|PD|CR|11!   |03(|11R|03) |15Use Download Files |07(|15T|07/|15F|07)|15: ");
    value[PROMPT_LEVEL_BOOL_DOWNLOAD_MB]      = std::make_pair("Enable Download Limit in MBs", "|CR|03%   |15|PD|CR|11!   |03(|11S|03) |15Use Download MBs |07(|15T|07/|15F|07)|15  : ");

    // Display Page for Menu Fields
    value[DISPLAY_LEVEL_FIELDS_BORDER_ROW_COLOR] = std::make_pair("Border Row Color", " |07");

    value[DISPLAY_LEVEL_FIELD_NAME]            = std::make_pair("Level Name", " |03(|11A|03) |15Level Name         : ");
    value[DISPLAY_LEVEL_FIELD_START_MENU]      = std::make_pair("Start Menu", " |03(|11B|03) |15Start Menu         : ");
    value[DISPLAY_LEVEL_FIELD_FILE_LEVEL]      = std::make_pair("File Level", " |03(|11C|03) |15File Level         : ");
    value[DISPLAY_LEVEL_FIELD_MESG_LEVEL]      = std::make_pair("Message Level", " |03(|11D|03) |15Message Level      : ");
    value[DISPLAY_LEVEL_FIELD_POST_CALL_RATIO] = std::make_pair("Post/Call Ratio", " |03(|11E|03) |15Post/Call Ratio    : ");
    value[DISPLAY_LEVEL_FIELD_FILE_RATIO]      = std::make_pair("File Ratio - Uploads to Downloads", " |03(|11F|03) |15File Ratio         : ");
    value[DISPLAY_LEVEL_FIELD_TIME_LIMIT]      = std::make_pair("Time Limit Per Day", " |03(|11G|03) |15Time Limit         : ");
    value[DISPLAY_LEVEL_FIELD_CALL_LIMIT]      = std::make_pair("Call Limit Per Day", " |03(|11H|03) |15Call Limit         : ");
    value[DISPLAY_LEVEL_FIELD_DOWNLOADS]       = std::make_pair("Download Limits By # of Files", " |03(|11I|03) |15Download Files     : ");
    value[DISPLAY_LEVEL_FIELD_DOWNLOAD_MB]     = std::make_pair("Download Limit in MBs", " |03(|11J|03) |15Download MBs       : ");
    value[DISPLAY_LEVEL_FIELD_ARFLAGS1]        = std::make_pair("Access Control Flags 1", " |03(|11K|03) |15AR Flags 1         : ");
    value[DISPLAY_LEVEL_FIELD_ARFLAGS2]        = std::make_pair("Access Control Flags 2", " |03(|11L|03) |15AR Flags 2         : ");
    value[DISPLAY_LEVEL_BOOL_POST_CALL_RATIO]  = std::make_pair("Enable Post/Call Ratio", " |03(|11M|03) |15Use Post/Call Ratio: ");
    value[DISPLAY_LEVEL_BOOL_FILE_RATIO]       = std::make_pair("Enable File Ratio", " |03(|11N|03) |15Use File Ratio     : ");
    value[DISPLAY_LEVEL_BOOL_TIME_LIMIT]       = std::make_pair("Enable Time Limit", " |03(|11O|03) |15Use Time Limit     : ");
    value[DISPLAY_LEVEL_BOOL_CALL_LIMIT]       = std::make_pair("Enable Call Limit", " |03(|11P|03) |15Use Call Limit     : ");
    value[DISPLAY_LEVEL_BOOL_DOWNLOADS]        = std::make_pair("Enable Download Limits by Files", " |03(|11R|03) |15Use Download Files : ");
    value[DISPLAY_LEVEL_BOOL_DOWNLOAD_MB]      = std::make_pair("Enable Download Limit in MBs", " |03(|11S|03) |15Use Download MBs   : ");

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

            case 'C': // Copy
                changeLevelInputState(LEVEL_COPY_FROM);
                displayPrompt(PROMPT_LEVEL_COPY_FROM);
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

        AccessCondition acs;
        access_level_ptr current_level = getCurrentLevel();

        if(current_level == nullptr)
        {
            return;
        }

        switch(toupper(key[0]))
        {
            case 'A': // Level Name
                m_current_field = toupper(key[0]);
                changeInputModule(MOD_LEVEL_FIELD);
                displayPrompt(PROMPT_LEVEL_FIELD_NAME);
                m_session_io.getInputField("", key, Config::sName_length, current_level->sName);
                break;

            case 'B': // Level Start Menu
                m_current_field = toupper(key[0]);
                changeInputModule(MOD_LEVEL_FIELD);
                displayPrompt(PROMPT_LEVEL_FIELD_START_MENU);
                m_session_io.getInputField("", key, Config::sName_length, current_level->sStartMenu);
                break;

            case 'C': // Level File Level
                m_current_field = toupper(key[0]);
                changeInputModule(MOD_LEVEL_FIELD);
                displayPrompt(PROMPT_LEVEL_FIELD_FILE_LEVEL);
                m_session_io.getInputField("", key, Config::sName_length, std::to_string(current_level->iFileLevel));
                break;

            case 'D': // Level Message Level
                m_current_field = toupper(key[0]);
                changeInputModule(MOD_LEVEL_FIELD);
                displayPrompt(PROMPT_LEVEL_FIELD_MESG_LEVEL);
                m_session_io.getInputField("", key, Config::sName_length, std::to_string(current_level->iMessageLevel));
                break;

            case 'E': // Level Post/Call Ratio
                m_current_field = toupper(key[0]);
                changeInputModule(MOD_LEVEL_FIELD);
                displayPrompt(PROMPT_LEVEL_FIELD_POST_CALL_RATIO);
                m_session_io.getInputField("", key, Config::sName_length, std::to_string(current_level->iPostCallRatio));
                break;

            case 'F': // Level File Ratio
                m_current_field = toupper(key[0]);
                changeInputModule(MOD_LEVEL_FIELD);
                displayPrompt(PROMPT_LEVEL_FIELD_FILE_RATIO);
                m_session_io.getInputField("", key, Config::sName_length, std::to_string(current_level->iFileRatio));
                break;

            case 'G': // Level Time Limit
                m_current_field = toupper(key[0]);
                changeInputModule(MOD_LEVEL_FIELD);
                displayPrompt(PROMPT_LEVEL_FIELD_TIME_LIMIT);
                m_session_io.getInputField("", key, Config::sName_length, std::to_string(current_level->iTimeLimit));
                break;

            case 'H': // Level Call Limit
                m_current_field = toupper(key[0]);
                changeInputModule(MOD_LEVEL_FIELD);
                displayPrompt(PROMPT_LEVEL_FIELD_CALL_LIMIT);
                m_session_io.getInputField("", key, Config::sName_length, std::to_string(current_level->iCallLimit));
                break;

            case 'I': // Level Downloads
                m_current_field = toupper(key[0]);
                changeInputModule(MOD_LEVEL_FIELD);
                displayPrompt(PROMPT_LEVEL_FIELD_DOWNLOADS);
                m_session_io.getInputField("", key, Config::sName_length, std::to_string(current_level->iDownloads));
                break;

            case 'J': // Level Downloads MB
                m_current_field = toupper(key[0]);
                changeInputModule(MOD_LEVEL_FIELD);
                displayPrompt(PROMPT_LEVEL_FIELD_DOWNLOAD_MB);
                m_session_io.getInputField("", key, Config::sName_length, std::to_string(current_level->iDownloadMB));
                break;

            case 'K': // Level AR Flags 1
                m_current_field = toupper(key[0]);
                changeInputModule(MOD_LEVEL_FIELD);
                displayPrompt(PROMPT_LEVEL_FIELD_ARFLAGS1);
                m_session_io.getInputField("", key, Config::sName_length);
                break;

            case 'L': // Level AR Flags 2
                m_current_field = toupper(key[0]);
                changeInputModule(MOD_LEVEL_FIELD);
                displayPrompt(PROMPT_LEVEL_FIELD_ARFLAGS2);
                m_session_io.getInputField("", key, Config::sName_length);
                break;

            case 'M': // Bool PostCallRatio
                {
                    m_current_field = toupper(key[0]);
                    changeInputModule(MOD_LEVEL_FIELD);
                    displayPrompt(PROMPT_LEVEL_BOOL_POST_CALL_RATIO);
                    std::string bool_value = "";
                    bool_value = m_common_io.boolAlpha(current_level->bPostCallRatio).at(0);
                    m_session_io.getInputField("", key, Config::sName_length, bool_value);
                    break;
                }

            case 'N': // Bool File Ratio
                {
                    m_current_field = toupper(key[0]);
                    changeInputModule(MOD_LEVEL_FIELD);
                    displayPrompt(PROMPT_LEVEL_BOOL_FILE_RATIO);
                    std::string bool_value = "";
                    bool_value = m_common_io.boolAlpha(current_level->bPostCallRatio).at(0);
                    m_session_io.getInputField("", key, Config::sName_length, bool_value);
                    break;
                }

            case 'O': // Bool Time Limit
                {
                    m_current_field = toupper(key[0]);
                    changeInputModule(MOD_LEVEL_FIELD);
                    displayPrompt(PROMPT_LEVEL_BOOL_TIME_LIMIT);
                    std::string bool_value = "";
                    bool_value = m_common_io.boolAlpha(current_level->bPostCallRatio).at(0);
                    m_session_io.getInputField("", key, Config::sName_length, bool_value);
                    break;
                }

            case 'P': // Bool Call Limit
                {
                    m_current_field = toupper(key[0]);
                    changeInputModule(MOD_LEVEL_FIELD);
                    displayPrompt(PROMPT_LEVEL_BOOL_CALL_LIMIT);
                    std::string bool_value = "";
                    bool_value = m_common_io.boolAlpha(current_level->bPostCallRatio).at(0);
                    m_session_io.getInputField("", key, Config::sName_length, bool_value);
                    break;
                }

            case 'R': // Bool Download File Limit
                {
                    m_current_field = toupper(key[0]);
                    changeInputModule(MOD_LEVEL_FIELD);
                    displayPrompt(PROMPT_LEVEL_BOOL_DOWNLOADS);
                    std::string bool_value = "";
                    bool_value = m_common_io.boolAlpha(current_level->bPostCallRatio).at(0);
                    m_session_io.getInputField("", key, Config::sName_length, bool_value);
                    break;
                }

            case 'S': // Bool Download Limit MB
                {
                    m_current_field = toupper(key[0]);
                    changeInputModule(MOD_LEVEL_FIELD);
                    displayPrompt(PROMPT_LEVEL_BOOL_DOWNLOAD_MB);
                    std::string bool_value = "";
                    bool_value = m_common_io.boolAlpha(current_level->bPostCallRatio).at(0);
                    m_session_io.getInputField("", key, Config::sName_length, bool_value);
                    break;
                }

            case 'Q': // Quit
                saveLevelChanges();
                std::vector<access_level_ptr>().swap(m_loaded_levels);

                // Reload fall back, or gosub to Menu Editor Main
                changeInputModule(MOD_LEVEL_INPUT);
                changeSetupModule(MOD_DISPLAY_LEVEL);
                return;

            case 'X': // Exit without Saving
                std::vector<access_level_ptr>().swap(m_loaded_levels);

                // Reload fall back, or gosub to Menu Editor Main
                changeInputModule(MOD_LEVEL_INPUT);
                changeSetupModule(MOD_DISPLAY_LEVEL);
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

        access_level_ptr current_level = getCurrentLevel();

        // Handle the assigned input received for field
        switch(m_current_field)
        {
            case 'A': // Level Name
                current_level->sName = key;
                break;

            case 'B': // Level Start Menu
                current_level->sStartMenu = key;
                break;

            case 'C': // Level File Level
                current_level->iFileLevel = m_common_io.stringToInt(key);
                break;

            case 'D': // Level Message Level
                current_level->iMessageLevel = m_common_io.stringToInt(key);
                break;

            case 'E': // Level Post/Call Ratio
                current_level->iPostCallRatio = m_common_io.stringToInt(key);
                break;

            case 'F': // Level File Ratio
                current_level->iFileRatio = m_common_io.stringToInt(key);
                break;

            case 'G': // Level Time Limit
                current_level->iTimeLimit = m_common_io.stringToInt(key);
                break;

            case 'H': // Level Call Limit
                current_level->iCallLimit = m_common_io.stringToInt(key);
                break;

            case 'I': // Level Downloads
                current_level->iDownloads = m_common_io.stringToInt(key);
                break;

            case 'J': // Level Downloads MB
                current_level->iDownloadMB = m_common_io.stringToInt(key);
                break;

            case 'K': // Level AR Flags 1
                {
                    AccessCondition acs;

                    for(char c : key)
                        acs.setFlagLevelToggle(c, true, current_level);

                    break;
                }

            case 'L': // Level AR Flags 2
                {
                    AccessCondition acs;

                    for(char c : key)
                        acs.setFlagLevelToggle(c, false, current_level);

                    break;
                }

            case 'M': // Bool PostCallRatio
                current_level->bPostCallRatio = m_common_io.stringToBool(key);
                break;

            case 'N': // Bool File Ratio
                current_level->bFileRatio = m_common_io.stringToBool(key);
                break;

            case 'O': // Bool Time Limit
                current_level->bTimeLimit = m_common_io.stringToBool(key);
                break;

            case 'P': // Bool Call Limit
                current_level->bCallLimit = m_common_io.stringToBool(key);
                break;

            case 'R': // Bool Download File Limit
                current_level->bDownloads = m_common_io.stringToBool(key);
                break;

            case 'S': // Bool Download Limit MB
                current_level->bDownloadMB = m_common_io.stringToBool(key);
                break;
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

        case LEVEL_COPY_FROM:

            // [Source]
            // Notes, this will take the source, then move to the
            // LEVEL_COPY_TO for destination.  Source is saved as m_current Menu
            if(does_level_exist)
            {
                m_current_level = level_code;
                changeLevelInputState(LEVEL_COPY_TO);
                displayPrompt(PROMPT_LEVEL_COPY_TO);
            }
            else
            {
                // Error, can't remove a menu that doesn't exist!
                displayPrompt(PROMPT_INVALID_LEVEL_NOT_EXISTS);
                displayPrompt(PROMPT_INPUT_TEXT);
                changeInputModule(MOD_LEVEL_INPUT);
            }

            break;

        case LEVEL_COPY_TO:

            // [Destination]
            if(does_level_exist)
            {
                // Error, can't remove a menu that doesn't exist!
                displayPrompt(PROMPT_INVALID_LEVEL_NOT_EXISTS);
                displayPrompt(PROMPT_INPUT_TEXT);
                changeInputModule(MOD_LEVEL_INPUT);
            }
            else
            {
                copyExistingLevel(level_code);
                changeInputModule(MOD_LEVEL_INPUT);
                redisplayModulePrompt();
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

    // Check if Level Already Exists.
    access_level_ptr destination_level = level_dao->getAccessLevelByLevel(level_code);

    if(checkLevelExistsByLevel(level_code))
    {
        std::cout << "Destination level already exists!" << std::endl;
        return;
    }

    // Set New Level Code assignment and save.
    new_level->iLevel = level_code;

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

    level_dao->deleteRecord(existing_level->iId);
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
    existing_level->sName = "Copied From " + std::to_string(m_current_level);

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
        return;
    }

    level_dao->updateRecord(existing_level);
}

/**
 * @brief Check if the level exists in the current listing
 * @param level_code
 */
bool ModLevelEditor::checkLevelExistsByLevel(int level_code)
{
    for(std::string::size_type i = 0; i < m_loaded_levels.size(); i++)
    {
        if(m_loaded_levels[i]->iLevel == level_code)
            return true;
    }

    return false;
}

/**
 * @brief Retrieve Current Level
 */
access_level_ptr ModLevelEditor::getCurrentLevel()
{
    for(std::string::size_type i = 0; i < m_loaded_levels.size(); i++)
    {
        if(m_loaded_levels[i]->iLevel == m_current_level)
            return m_loaded_levels[i];
    }

    return nullptr;
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

    sort(
        m_loaded_levels.begin(), m_loaded_levels.end(),
        [ ](const access_level_ptr& lhs, const access_level_ptr& rhs)
    {
        return lhs->iLevel < rhs->iLevel;
    });

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
                buffer += baseGetEncodedBoxCharAndColor(M_BORDER_TOP_LEFT);
            }
            else if(rows == 0 && cols == max_cols-1)
            {
                buffer += baseGetEncodedBoxCharAndColor(M_BORDER_TOP_RIGHT);
            }
            else if(rows == 0 && cols % 25 == 0)
            {
                buffer += baseGetEncodedBoxCharAndColor(M_BORDER_MID_TOP);
            }
            else if(rows == 0)
            {
                buffer += baseGetEncodedBoxCharAndColor(M_BORDER_ROW);
            }

            // Bottom Row
            else if(rows == total_rows-1 && cols == 0)
            {
                buffer += baseGetEncodedBoxCharAndColor(M_BORDER_BOT_LEFT);
            }
            else if(rows == total_rows-1 && cols == max_cols-1)
            {
                buffer += baseGetEncodedBoxCharAndColor(M_BORDER_BOT_RIGHT);
            }
            else if(rows == total_rows-1 && cols % 25 == 0)
            {
                buffer += baseGetEncodedBoxCharAndColor(M_BORDER_MID_BOT);
            }
            else if(rows == total_rows-1)
            {
                buffer += baseGetEncodedBoxCharAndColor(M_BORDER_ROW);
            }
            else if(cols % 25 == 0)
            {
                buffer += baseGetEncodedBoxCharAndColor(M_BORDER_MID);
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
 * @brief Menu Editor, for Displaying Level Fields to Edit
 * @return
 */
std::string ModLevelEditor::displayLevelEditScreen()
{
    // Create Menu Pointer then load the menu into it.
    access_level_ptr current_level = nullptr;

    current_level = getCurrentLevel();

    if(current_level == nullptr)
    {
        std::cout << "Level Not Found: " << std::to_string(m_current_level);
        return "Level Not Found: " + std::to_string(m_current_level);
    }

    // Build a string list of individual menu options, then loop to fit as many per screen!
    std::vector<std::string> result_set;
    AccessCondition acs;

    result_set.push_back(getDisplayPromptRaw(DISPLAY_LEVEL_FIELD_NAME) + baseGetDefaultStatColor() +
                         m_common_io.rightPadding(current_level->sName, 48));

    result_set.push_back(getDisplayPromptRaw(DISPLAY_LEVEL_FIELD_START_MENU) + baseGetDefaultStatColor() +
                         m_common_io.rightPadding(current_level->sStartMenu, 48));


    result_set.push_back(getDisplayPromptRaw(DISPLAY_LEVEL_FIELD_FILE_LEVEL) + baseGetDefaultStatColor() +
                         m_common_io.rightPadding(std::to_string(current_level->iFileLevel), 48));

    result_set.push_back(getDisplayPromptRaw(DISPLAY_LEVEL_FIELD_MESG_LEVEL) + baseGetDefaultStatColor() +
                         m_common_io.rightPadding(std::to_string(current_level->iMessageLevel), 48));

    result_set.push_back(getDisplayPromptRaw(DISPLAY_LEVEL_FIELD_POST_CALL_RATIO) + baseGetDefaultStatColor() +
                         m_common_io.rightPadding(std::to_string(current_level->iPostCallRatio), 48));

    result_set.push_back(getDisplayPromptRaw(DISPLAY_LEVEL_FIELD_FILE_RATIO) + baseGetDefaultStatColor() +
                         m_common_io.rightPadding(std::to_string(current_level->iFileRatio), 48));

    result_set.push_back(getDisplayPromptRaw(DISPLAY_LEVEL_FIELD_TIME_LIMIT) + baseGetDefaultStatColor() +
                         m_common_io.rightPadding(std::to_string(current_level->iTimeLimit), 48));

    result_set.push_back(getDisplayPromptRaw(DISPLAY_LEVEL_FIELD_CALL_LIMIT) + baseGetDefaultStatColor() +
                         m_common_io.rightPadding(std::to_string(current_level->iCallLimit), 48));

    result_set.push_back(getDisplayPromptRaw(DISPLAY_LEVEL_FIELD_DOWNLOADS) + baseGetDefaultStatColor() +
                         m_common_io.rightPadding(std::to_string(current_level->iDownloads), 48));

    result_set.push_back(getDisplayPromptRaw(DISPLAY_LEVEL_FIELD_DOWNLOAD_MB) + baseGetDefaultStatColor() +
                         m_common_io.rightPadding(std::to_string(current_level->iDownloadMB), 48));

    result_set.push_back(getDisplayPromptRaw(DISPLAY_LEVEL_FIELD_ARFLAGS1) + baseGetDefaultStatColor() +
                         m_common_io.rightPadding(acs.getAccessConditionFlagStringFromBits(current_level->iARFlags1), 48));

    result_set.push_back(getDisplayPromptRaw(DISPLAY_LEVEL_FIELD_ARFLAGS2) + baseGetDefaultStatColor() +
                         m_common_io.rightPadding(acs.getAccessConditionFlagStringFromBits(current_level->iARFlags2), 48));


    result_set.push_back(m_common_io.rightPadding(getDisplayPromptRaw(DISPLAY_LEVEL_BOOL_POST_CALL_RATIO) + baseGetDefaultStatColor() + m_common_io.boolAlpha(current_level->bPostCallRatio), 56) +
                         m_common_io.rightPadding(getDisplayPromptRaw(DISPLAY_LEVEL_BOOL_CALL_LIMIT) + baseGetDefaultStatColor() + m_common_io.boolAlpha(current_level->bCallLimit), 56));


    result_set.push_back(m_common_io.rightPadding(getDisplayPromptRaw(DISPLAY_LEVEL_BOOL_FILE_RATIO) + baseGetDefaultStatColor() + m_common_io.boolAlpha(current_level->bFileRatio), 56) +
                         m_common_io.rightPadding(getDisplayPromptRaw(DISPLAY_LEVEL_BOOL_DOWNLOADS) + baseGetDefaultStatColor() + m_common_io.boolAlpha(current_level->bDownloads), 56));

    result_set.push_back(m_common_io.rightPadding(getDisplayPromptRaw(DISPLAY_LEVEL_BOOL_TIME_LIMIT) + baseGetDefaultStatColor() + m_common_io.boolAlpha(current_level->bTimeLimit), 56) +
                         m_common_io.rightPadding(getDisplayPromptRaw(DISPLAY_LEVEL_BOOL_DOWNLOAD_MB) + baseGetDefaultStatColor() + m_common_io.boolAlpha(current_level->bDownloadMB), 56));


    result_set.push_back(baseGetDefaultPromptColor() + " " + Encoding::instance()->utf8Encode(std::string(72, M_BORDER_ROW)) + " ");

    result_set.push_back(getDisplayPromptRaw(DISPLAY_LEVEL_FIELDS_QUIT_SAVE) + baseGetDefaultStatColor() +
                         m_common_io.rightPadding("", 48));

    result_set.push_back(getDisplayPromptRaw(DISPLAY_LEVEL_FIELDS_QUIT_ABORT) + baseGetDefaultStatColor() +
                         m_common_io.rightPadding("", 48));


    // iterate through and print out
    int total_rows = result_set.size()+2;

    // Could re-calc this on screen width lateron.
    int max_cols = 76;

    return baseCreateBorderedDisplay(result_set, total_rows, max_cols);
}
