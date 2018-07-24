#include "mod_user_editor.hpp"

#include "../model-sys/security.hpp"
#include "../model-sys/users.hpp"
#include "../data-sys/security_dao.hpp"
#include "../data-sys/users_dao.hpp"

#include <string>
#include <vector>

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
    value[PROMPT_PAUSE]                   = std::make_pair("Pause Prompt", "|CR |03- |15Hit any key to continue or (|03a|15)bort listing |03-|15 |CR");

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
    m_mod_user_state_index = mod_menu_state_index;
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
    displayPrompt(PROMPT_HEADER);
    
    // Build a list of screen lines for the menu display
    // So we know when to pause in large listing, or use pagenation.
    std::string user_display_output = displayUserList();    
    
    if (m_user_display_list.size() == 0) 
    {
        // Clear Out list if anything already exists.
        std::vector<std::string>().swap(m_user_display_list);
    }
    
    m_user_display_list = m_common_io.splitString(user_display_output, '\n');           
    m_page = 0;        
    displayCurrentPage(PROMPT_INPUT_TEXT);
}

/**
 * @brief Handles input command for User Editor Prompt
 * @return
 */
void ModUserEditor::userListInput(const std::string &input) 
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
        
        std::string output_buffer = m_config->default_color_regular;
        switch (toupper(key[0]))
        {
            /*
            case 'A': // Add
                changeMenuInputState(MENU_ADD);
                displayPrompt(PROMPT_MENU_ADD);
                changeInputModule(MOD_MENU_NAME);
                break;
                
            case 'E': // Change/Edit
                changeMenuInputState(MENU_CHANGE);
                displayPrompt(PROMPT_MENU_CHANGE);
                changeInputModule(MOD_MENU_NAME);
                break;
                
            case 'D': // Delete
                changeMenuInputState(MENU_DELETE);
                displayPrompt(PROMPT_MENU_DELETE);
                changeInputModule(MOD_MENU_NAME);
                break;
                
            case 'C': // Copy
                changeMenuInputState(MENU_COPY_FROM);
                displayPrompt(PROMPT_MENU_COPY_FROM);
                changeInputModule(MOD_MENU_NAME);
                break;*/
                
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
        if (result != "empty") 
        {
            baseProcessDeliverInput(result);
        }
    }
}

/**
 * @brief Handles Input (Waiting for Any Key Press)
 * @param input
 */
void ModUserEditor::menuEditorPausedInput(const std::string &input)
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
    for (unsigned int i = (m_page*(m_rows_per_page-2)); i < m_user_display_list.size(); i++) 
    {
        std::string display_line = m_session_io.pipe2ansi(m_user_display_list[i]);
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
        displayPrompt(PROMPT_PAUSE);
        changeInputModule(MOD_PAUSE);
    }
}

/**
 * @brief User Editor, Read and Modify User Preferences
 * @return
 */
std::string ModUserEditor::displayUserList()
{  

    users_dao_ptr user_data(new UsersDao(m_session_data->m_user_database));
    std::vector<user_ptr> users = user_data->getAllRecords();

    std::string header_string = "";
    std::string boarder_string = "";
    
     // Build a string list of individual menu options, then loop to fit as many per screen!
    std::vector<std::string> result_set;  
    for(unsigned int i = 0; i < users.size(); i++)
    {  
        std::string option_string = m_common_io.rightPadding(std::to_string(users[i]->iId), 5);                                     
        option_string.append("|03" + m_common_io.rightPadding(users[i]->sHandle, 19));
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
                buffer += baseGetDefaultColor();
                buffer += BORDER_TOP_LEFT;
            }
            else if(rows == 0 && cols == max_cols-1)
            {
                buffer += baseGetDefaultColor();
                buffer += BORDER_TOP_RIGHT;
            }
            else if(rows == 0 && cols % 25 == 0)
            {
                buffer += baseGetDefaultColor();
                buffer += BORDER_MID_TOP;
            } 
            else if(rows == 0)
            {
                buffer += baseGetDefaultColor();
                buffer += BORDER_ROW;
            }

            // Bottom Row
            else if(rows == total_rows-1 && cols == 0)
            {
                buffer += baseGetDefaultColor();
                buffer += BORDER_BOT_LEFT;
            }
            else if(rows == total_rows-1 && cols == max_cols-1)
            {
                buffer += baseGetDefaultColor();
                buffer += BORDER_BOT_RIGHT;
            }
            else if(rows == total_rows-1 && cols % 25 == 0)
            {
                buffer += baseGetDefaultColor();
                buffer += BORDER_MID_BOT;
            } 
            else if(rows == total_rows-1)
            {
                buffer += baseGetDefaultColor();
                buffer += BORDER_ROW;
            }
            else if(cols % 25 == 0)
            {
                buffer += baseGetDefaultColor();
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
