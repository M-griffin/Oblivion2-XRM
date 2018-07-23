#include "mod_user_editor.hpp"


/**
 * @brief Handles Updates or Data Input from Client
 * @return bool, not used anymore?!?
 */
bool ModUserEditor::update(const std::string &character_buffer, const bool &)
{
    // Make sure system is active, when system is done, success or failes
    // We change this is inactive to single the login process is completed.
    if(!m_is_active)
    {
        std::cout << "ModUserEditor() !m_is_active" << std::endl;
        return false;
    }

    // Return True when were keeping module active / else false;
    if(character_buffer.size() == 0)
    {
        std::cout << "ModUserEditor() !character_buffer size 0" << std::endl;
        return true;
    }

    // Process all incoming data stright to the input functions.
    m_mod_functions[m_mod_function_index](character_buffer);

    return true;
}

/**
 * @brief Startup class, setup and display initial screens / interface.
 * @return
 */
bool ModUserEditor::onEnter()
{
    std::cout << "OnEnter() ModUserEditor\n";
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
bool ModUserEditor::onExit()
{
    std::cout << "OnExit() ModUserEditor\n";
    m_is_active = false;
    return true;
}

/**
 * @brief Create Default Text Prompts for module
 */
void ModUserEditor::createTextPrompts()
{
    // Create Mapping to pass for file creation (default values)
    M_TextPrompt value;

    value[PROMPT_HEADER]                  = std::make_pair("User Editor Header", "|CS|CR|03--- |15[|03Oblivion/2 XRM |07// |11User Editor|15] |03--- |CR");
    value[PROMPT_INPUT_TEXT]              = std::make_pair("Menu Editor Prompt", "|CR|03A|15/dd Menu |03E|15/dit Menu |03D|15/elete Menu |03F|15/ilter Listing |03Q|15/uit : ");

    m_text_prompts_dao->writeValue(value);
}

/**
 * @brief Sets an indivdual input module index.
 * @param mod_function_index
 */
void ModUserEditor::changeInputModule(int mod_function_index)
{
    // Setup input module
    m_mod_function_index = mod_function_index;
}

/**
 * @brief Sets an indivdual setup method module index.
 * @param mod_function_index
 */
void ModUserEditor::changeSetupModule(int mod_function_index)
{
    // Set, and Execute the Setup module.
    m_mod_setup_index = mod_function_index;
    m_setup_functions[m_mod_setup_index]();
}

/**
 * @brief Sets an indivdual Menu Input State Add/Change/Delete
 * @param mod_menu_state_index
 */
void ModUserEditor::changeMenuInputState(int mod_menu_state_index)
{
    m_mod_menu_state_index = mod_menu_state_index;
}

/**
 * @brief Redisplay's the current module prompt.
 * @return
 */
void ModUserEditor::redisplayModulePrompt()
{
    m_setup_functions[m_mod_setup_index]();
}
   
/**
 * @brief Pull and Display Prompts
 * @param prompt
 */
void ModUserEditor::displayPrompt(const std::string &prompt)
{
    baseDisplayPrompt(prompt, m_text_prompts_dao);
}

/**
 * @brief Pull and Display Prompts with MCI Code
 * @param prompt
 * @param mci_field
 */
void ModUserEditor::displayPromptMCI(const std::string &prompt, const std::string &mci_field)
{
    baseDisplayPromptMCI(prompt, m_text_prompts_dao, mci_field);
}

/**
 * @brief Pull and Display Prompts with following newline
 * @param prompt
 */
void ModUserEditor::displayPromptAndNewLine(const std::string &prompt)
{
    baseDisplayPromptAndNewLine(prompt, m_text_prompts_dao);
}

/**
 * @brief Setup for the User Editor 
 * @return
 */
void ModUserEditor::setupUserList() 
{
    
}

/**
 * @brief Handles input command for User Editor Prompt
 * @return
 */
void ModUserEditor::userListInput(const std::string &input) 
{

}




/**
 * @brief Displays the current page of users
 * @param input_state
 */
void ModUserEditor::displayCurrentPage(const std::string &input_state) 
{    
    // calculate the rows_per_page.
    unsigned int rows_used = m_ansi_process->getMaxRowsUsedOnScreen();
    unsigned int max_rows = m_ansi_process->getMaxLines();
    
    if (m_page > 0)
        rows_used -= (m_ansi_process->m_number_lines - 2);
        
    m_rows_per_page = max_rows - (rows_used + 2);
    
    bool displayed_all_rows = true;
    for (unsigned int i = (m_page*(m_rows_per_page-2)); i < m_menu_display_list.size(); i++) 
    {
        std::string display_line = m_session_io.pipe2ansi(m_menu_display_list[i]);
        display_line.append("\r\n");
        baseProcessAndDeliver(display_line);
        
        if (i >= (m_page*m_rows_per_page) + m_rows_per_page)
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
        case MOD_DISPLAY_USER_LIST:
            current_module_input = MOD_USER_INPUT;
            break;
                       
        default:
            std::cout << "Error, forgot to add new STATE index displayCurrentPage!!";
            return;
    }  
    
    // If we displayed all rows, then display propmpt, otherwise
    // Ask to hit anykey for next page.
    if (displayed_all_rows)
    {
        // Reset Page back to Zero for next display.
        m_page = 0;
        displayPrompt(input_state);
        changeInputModule(current_module_input);
    }
    else 
    {
//        displayPrompt(PROMPT_PAUSE);
//        changeInputModule(MOD_PAUSE);
    }
}
