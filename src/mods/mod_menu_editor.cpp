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
        std::cout << "ModMenuEditor() !m_is_active" << std::endl;
        return false;
    }

    // Return True when were keeping module active / else false;
    if(character_buffer.size() == 0)
    {
        std::cout << "ModMenuEditor() !character_buffer size 0" << std::endl;
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
    std::cout << "OnEnter() ModMenuEditor\n";
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
bool ModMenuEditor::onExit()
{
    std::cout << "OnExit() ModMenuEditor\n";
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

    value[PROMPT_HEADER]     = std::make_pair("Editor Header", "|CS|CR --- Menu Editor --- |CR");
    value[PROMPT_PAUSE]      = std::make_pair("Pause Prompt", "|CR - Hit any key to continue or (a)bort listing - ");
    value[PROMPT_INPUT_TEXT] = std::make_pair("User Prompt", "|CRA/dd Menu C/hange Menu D/elete Menu Q/uit : ");
    value[PROMPT_INVALID]    = std::make_pair("Invalid input", "|04Invalid Input! Try again.|CR");

    m_text_prompts_dao->writeValue(value);
}

/**
 * @brief Sets an indivdual input module index.
 * @param mod_function_index
 */
void ModMenuEditor::changeInputModule(int mod_function_index)
{
    // Setup input module
    m_mod_function_index = mod_function_index;
}

/**
 * @brief Sets an indivdual setup method module index.
 * @param mod_function_index
 */
void ModMenuEditor::changeSetupModule(int mod_function_index)
{
    // Set, and Execute the Setup module.
    m_mod_setup_index = mod_function_index;
    m_setup_functions[m_mod_setup_index]();
}

/**
 * @brief Redisplay's the current module prompt.
 * @param mod_function_index
 */
void ModMenuEditor::redisplayModulePrompt()
{
    m_setup_functions[m_mod_setup_index]();
}

/**
 * @brief Pull and Display Prompts
 * @param prompt
 */
void ModMenuEditor::displayPrompt(const std::string &prompt)
{
    baseDisplayPrompt(prompt, m_text_prompts_dao);
}

/**
 * @brief Setup for the Menu Header 
 * @return
 */
void ModMenuEditor::setupMenuEditor() 
{
    std::cout << "setupMenuEditor()" << std::endl;
    displayPrompt(PROMPT_HEADER);
    
    // Build a list of screen lines for the menu display
    // So we know when to pause in large listing, or use pagenation.
    std::string menu_display_output = displayMenuList();    
    
    if (m_menu_display_list.size() == 0) 
    {
        // Clear Out list if anything already exists.
        std::vector<std::string>().swap(m_menu_display_list);
    }
    
    m_menu_display_list = m_common_io.splitString(menu_display_output, '\n');
        
    // std::string display_prompt = moveStringToBottom(prompt_string);

    // Translate Pipe Coles to ESC Sequences prior to parsing to keep
    // String length calculations.
    // display_prompt = m_session_io.pipe2ansi(display_prompt);
        
    
    // Set the available rows to display.
    // Add (2) rows from space and input prompt.
    m_page = 0;
        
    displayCurrentPage();
}

/**
 * @brief Displays the current page of menu items
 */
void ModMenuEditor::displayCurrentPage() 
{
    
    // calculate the rows_per_page.
    unsigned int rows_used = m_ansi_process->getMaxRowsUsedOnScreen();
    unsigned int max_rows = m_ansi_process->getMaxLines();
    
    std::cout << "*** rows_used " << rows_used << std::endl;
    std::cout << "*** max_rows " << max_rows << std::endl;
    m_rows_per_page = max_rows - rows_used + 2;
    
    std::cout << "*** m_rows_per_page " << m_rows_per_page << std::endl;
    
    bool displayed_all_rows = true;
    for (unsigned int i = (m_page*m_rows_per_page); i < m_menu_display_list.size(); i++) 
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
    
    // If we displayed all rows, then display propmpt, otherwise
    // Ask to hit anykey for next page.
    if (displayed_all_rows)
    {
        // Reset Page back to Zero for next display.
        m_page = 0;
        displayPrompt(PROMPT_INPUT_TEXT);
        changeInputModule(MOD_PROMPT);
    }
    else 
    {
        displayPrompt(PROMPT_PAUSE);
        changeInputModule(MOD_PAUSE);
    }
}
 
/**
 * @brief Handles Input (Waiting for Any Key Press)
 * @param input
 */
void ModMenuEditor::menuEditorPausedInput(const std::string &input)
{    
    
    std::cout << " *** menuEditorPausedInput !!!" << std::endl;
    std::cout << "**************************" << std::endl;
    
    // Check for abort on pause for next.
    if (input.size() == 1 && std::toupper(input[0]) == 'A')
    {
        displayPrompt(PROMPT_INPUT_TEXT);
        changeInputModule(MOD_PROMPT);
        return;
    }
    
    // Any input coming in here is valid
    // Increment to next page, then display remaining results.
    ++m_page;
    displayCurrentPage();
}


/**
 * @brief Handles Menu Editor Command Selection
 * @param input
 */
void ModMenuEditor::menuEditorInput(const std::string &input)
{
    std::cout << " *** menuEditorInput !!!" << std::endl;
    std::cout << "**************************" << std::endl;
    
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
            case 'A': // Add
                std::cout << "add" << std::endl;
                output_buffer += "Enter Menu Name to Add : ";
                m_session_data->deliver(output_buffer);
                break;
            case 'C': // Change/Edit
                std::cout << "change" << std::endl;
                output_buffer += "Enter Menu Name to Change : ";
                m_session_data->deliver(output_buffer);
                break;
            case 'D': // Delete
                std::cout << "delete" << std::endl;
                output_buffer += "Enter Menu to Delete : ";
                m_session_data->deliver(output_buffer);
                break;
            case 'Q': // Quit
                std::cout << "quit" << std::endl;
                // Reload fall back, or gosub to last menu!
                m_is_active = false;
                return;
        }
        
        /*
        // Check if users enter valid identifier.
        if (checkUserLogon(key))
        {               
            // Testing print user name.
            std::cout << m_logon_user->sHandle << std::endl;
            changeNextModule();
        }
        else
        {
            displayPromptAndNewLine(PROMPT_INVALID_USERNAME);
            ++m_failure_attempts;
            
            // If max, then exit back to matrix.  
            // NOTE Separate login/password attempts or change to login?
            if (m_failure_attempts >= m_config->invalid_password_attempts)
            {
                m_is_active = false;
                return false;
            }
               
            redisplayModulePrompt();
        }*/     
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
 * @brief Menu Editor, Read and Modify Menus
 * Remake of the orignal Menu Editor Screen
 */
std::string ModMenuEditor::displayMenuList()
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
    std::string buffer = "";
    for(int rows = 0; rows < total_rows; rows++)
    {
        buffer += "   "; // 3 Leading spaces per row.
        for(int cols = 0; cols < max_cols; cols++)
        {
            // Top Row
            if(rows == 0 && cols == 0)
            {
                buffer += top_left;
            }
            else if(rows == 0 && cols == max_cols-1)
            {
                buffer += top_right;
            }
            else if(rows == 0 && cols % 9 == 0)
            {
                buffer += mid_top;
            }
            else if(rows == 0)
            {
                buffer += row;
            }

            // Bottom Row
            else if(rows == total_rows-1 && cols == 0)
            {
                buffer += bot_left;
            }
            else if(rows == total_rows-1 && cols == max_cols-1)
            {
                buffer += bot_right;
            }
            else if(rows == total_rows-1 && cols % 9 == 0)
            {
                buffer += mid_bot;
            }
            else if(rows == total_rows-1)
            {
                buffer += row;
            }
            else if(cols % 9 == 0)
            {
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
}

