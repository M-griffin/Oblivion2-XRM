#include "mod_menu_editor.hpp"
#include "model-sys/struct_compat.hpp"
#include "directory.hpp"

#include <stdint.h>
#include <string>
#include <vector>

/**
 * @brief Handles Updates or Data Input from Client
 * @return bool, not used anymore?!?
 */
bool ModMenuEditor::update(const std::string &character_buffer, const bool &)
{
    // Make sure system is active, when system is done, success or failes
    // We change this is inactive to single the login process is completed.
    if(!m_is_active)
    {
        std::cout << "logon() !m_is_active" << std::endl;
        return false;
    }

    // Return True when were keeping module active / else false;
    if(character_buffer.size() == 0)
    {
        std::cout << "logon() !character_buffer size 0" << std::endl;
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
bool ModMenuEditor::onEnter()
{
    std::cout << "OnEnter() ModLogin\n";
    m_is_active = true;

    // Grab ANSI Screen, display, if desired.. logon.ans maybe?
    std::string prompt = "\x1b[?25h"; // Turn on Cursor.
    baseProcessAndDeliver(prompt);

    // Execute the initial setup index.
    m_setup_functions[m_mod_function_index]();
        
    return true;
}

/**
 * @brief Exit, close down, display screens to change over data.
 * @return
 */
bool ModMenuEditor::onExit()
{
    std::cout << "OnExit() ModLogin\n";
    m_is_active = false;
    return true;
}

/**
 * @brief Create Default Text Prompts for module
 */
void ModMenuEditor::createTextPrompts()
{
    // Create Mapping to pass for file creation (default values)
    M_TextPrompt value;

    value[PROMPT_HEADER]     = std::make_pair("Editor Header", "|CR|CR|15[|07Enter Username, Real Name, Email or User # to Login|15] |CRlogon: ");
    value[PROMPT_INPUT_TEXT] = std::make_pair("User Prompt", "|CR|15Use your user number |03|OT |15for quick logins |CR");
    value[PROMPT_INVALID]    = std::make_pair("Invalid input", "|04Invalid Input! Try again.|CR");

    m_text_prompts_dao->writeValue(value);
}

/**
 * @brief Sets an indivdual module index.
 * @param mod_function_index
 */
void ModMenuEditor::changeModule(int mod_function_index)
{
    // Set, and Execute the Setup module.
    m_mod_function_index = mod_function_index;
    m_setup_functions[m_mod_function_index]();
}


/**
 * @brief Menu Editor, Read and Modify Menus
 * Remake of the orignal Menu Editor Screen
 */
std::string ModMenuEditor::displayOfMenus()
{
    // Setup Extended ASCII Box Drawing characters.
    char top_left  = (char)214; // ╓
    char bot_left  = (char)211; // ╙
    char row       = (char)196; // ─
    char top_right = (char)183; // ╖
    char bot_right = (char)189; // ╜

    char mid_top   = (char)210; // ╥
    char mid_bot   = (char)208; // ╨
    char mid       = (char)186; // ║
   
    directory_ptr directory(new Directory());
    std::vector<std::string> result_set = directory->getFileListPerDirectory(GLOBAL_MENU_PATH, "yaml");

    // check result set, if no menu then return gracefully.
    if(result_set.size() == 0)
    {
        std::cout << "\r\n*** No Menus .yaml files found!" << std::endl;
        return "No Menu Files found!";
    }
    
    // Sort Menu's in accending order
    std::sort(result_set.begin(), result_set.end());

    // iterate through and print out
    int total_rows = result_set.size() / 8;
    int remainder = result_set.size() % 8;

    // Add for Header and Footer Row!
    total_rows += 2;
    if(remainder > 0)
        ++total_rows;

    // Could re-calc this on screen width lateron.
    int max_cols  = 73; // out of 80

    // Vector or Menus, Loop through
    std::vector<std::string>::iterator i = result_set.begin();
    std::string menu_name;
    std::string buffer = "\r\n";
    for(int rows = 0; rows < total_rows; rows++)
    {
        buffer += "   "; // 3 Leading spaces per row.
        for(int cols = 0; cols < max_cols; cols++)
        {
            // Top Row
            if(rows == 0 && cols == 0)
            {
                // std::cout << top_left << std::flush;
                buffer += top_left;
            }
            else if(rows == 0 && cols == max_cols-1)
            {
                //std::cout << top_right << std::flush;
                buffer += top_right;
            }
            else if(rows == 0 && cols % 9 == 0)
            {
                //std::cout << mid_top << std::flush;
                buffer += mid_top;
            }
            else if(rows == 0)
            {
                //std::cout << row << std::flush;
                buffer += row;
            }

            // Bottom Row
            else if(rows == total_rows-1 && cols == 0)
            {
                //std::cout << bot_left << std::flush;
                buffer += bot_left;
            }
            else if(rows == total_rows-1 && cols == max_cols-1)
            {
                //std::cout << bot_right << std::flush;
                buffer += bot_right;
            }
            else if(rows == total_rows-1 && cols % 9 == 0)
            {
                //std::cout << mid_bot << std::flush;
                buffer += mid_bot;
            }
            else if(rows == total_rows-1)
            {
                //std::cout << row << std::flush;
                buffer += row;
            }
            else if(cols % 9 == 0)
            {
                //std::cout << mid << std::flush;
                buffer += mid;
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
                        //std::cout << menu_name << std::flush;
                        buffer += menu_name;
                        ++i;
                    }
                    else
                    {
                        // Empty, 8 Spaces.
                        //std::cout << "        " << std::flush;
                        buffer += "        ";
                    }
                }
            }
        }
        //std::cout << std::endl;
        buffer += "\r\n";
    }

    // Display the prompt, then for wait for next command.
    
    buffer += "A/dd Menu C/hange Menu D/elete Menu Q/uit : ";
    //std::string prompt_buffer = "A/dd Menu C/hange Menu D/elete Menu Q/uit : ";
    return (buffer);    
}



/**
 * @brief Menu Editor, Read and Modify Menus
 * TODO reference moved from Menu System, for rewrite.
void MenuSystem::menuEditorInput(const std::string &character_buffer, const bool &)
{
    // If were not using hot keys, loop input until we get ENTER
    // Then handle only the first key in the buffer.  Other "SYSTEMS"
    // Will parse for entire line for matching Command Keys.
    if (!m_use_hotkey)
    {
        // Received ENTER, grab the previous input.
        if (!(character_buffer[0] == 13))
        {
            m_line_buffer += character_buffer[0];
            m_session_data->deliver(m_line_buffer);
        }
    }
    else
    {
        m_line_buffer += character_buffer[0];
    }

    std::string output_buffer = m_config->default_color_regular;
    switch (std::toupper(m_line_buffer[0]))
    {
        case 'A': // Add
            output_buffer += "Enter Menu Name to Add : ";
            break;
        case 'C': // Change/Edit
            output_buffer += "Enter Menu Name to Change : ";
            break;
        case 'D': // Delete
            output_buffer += "Enter Menu to Delete : ";
            break;
        case 'Q': // Quit
            // Reload fall back, or gosub to last menu!

            return;
        default : // Return
            return;
    }

    m_session_data->deliver(output_buffer);
}*/
