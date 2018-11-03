#include "mod_message_editor.hpp"

#include <string>
#include <vector>

/**
 * @brief Handles Updates or Data Input from Client
 * @return
 */
bool ModMessageEditor::update(const std::string &character_buffer, const bool &)
{
    // Make sure system is active, when system is done, success or failes
    // We change this is inactive to single the login process is completed.
    if(!m_is_active)
    {
        std::cout << "ModMessageEditor() !m_is_active" << std::endl;
        return false;
    }

    // Return True when were keeping module active / else false;
    if(character_buffer.size() == 0)
    {
        std::cout << "ModMessageEditor() !character_buffer size 0" << std::endl;
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
bool ModMessageEditor::onEnter()
{
    std::cout << "OnEnter() ModMessageEditor\n";
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
    std::cout << "OnExit() ModMessageEditor\n";
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
 * @brief Setup for the Message Editor
 * @return
 */
void ModMessageEditor::setupEditor()
{
    //displayPromptMCI(PROMPT_HEADER, string_filter);

    // Build a list of screen lines for the menu display
    // So we know when to pause in large listing, or use pagenation.
    //std::string user_display_output = displayUserList();

    /*
    if(m_user_display_list.size() > 0)
    {
        // Clear Out list if anything already exists.
        std::vector<std::string>().swap(m_user_display_list);
    }

    m_user_display_list = m_common_io.splitString(user_display_output, '\n');
    m_page = 0;
    displayCurrentPage(PROMPT_INPUT_TEXT);
    */
}

/**
 * @brief General Input for Editor
 * @return
 */
void ModMessageEditor::editorInput(const std::string &input)
{

}
