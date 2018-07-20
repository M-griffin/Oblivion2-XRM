#include "mod_menu_editor.hpp"
#include "model-sys/menu.hpp"
#include "data-sys/menu_dao.hpp"
#include "directory.hpp"

#include <stdint.h>
#include <string>
#include <vector>
#include <sstream>

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

    value[PROMPT_HEADER]                  = std::make_pair("Menu Editor Header", "|CS|CR|03--- |15[|11Menu Editor|15] |03--- |CR");
    value[PROMPT_OPTION_HEADER]           = std::make_pair("Menu Options Editor Header |OT Menu Name", "|CS|CR|03--- |15[|11Menu Option Editor|15] |03--- |15|OT |CR");
    value[PROMPT_MENU_EDIT_HEADER]        = std::make_pair("Menu Fields Editor Header |OT Menu Name", "|CS|CR|03--- |15[|11Menu Editor|15] |03--- |15|OT |CR");
    value[PROMPT_PAUSE]                   = std::make_pair("Pause Prompt", "|CR |03- |15Hit any key to continue or (|03a|15)bort listing |03-|15 ");
    value[PROMPT_INPUT_TEXT]              = std::make_pair("Menu Edit Prompt", "|CR|03A|15/dd Menu |03E|15/dit Menu |03D|15/elete Menu |03C|15/opy Menu |03Q|15/uit : ");
    value[PROMPT_OPTION_INPUT_TEXT]       = std::make_pair("Menu Option Edit Prompt", "|CR|03A|15/dd |03E|15/dit |03D|15/elete |03C|15/opy |03M|15/ove |03T|15/oggle |03Q|15/uit : ");
    value[PROMPT_INVALID]                 = std::make_pair("Invalid input", "|04Invalid Input! Try again.|CR");
    value[PROMPT_MENU_ADD]                = std::make_pair("Menu Name To Add", "|15Enter menu name to |11CREATE|15 : ");
    value[PROMPT_MENU_DELETE]             = std::make_pair("Menu Name To Delete", "|15Enter menu name to |11DELETE|15 : ");
    value[PROMPT_MENU_CHANGE]             = std::make_pair("Menu Name To Change", "|15Enter menu name to |11EDIT|15 : ");
    value[PROMPT_MENU_COPY_FROM]          = std::make_pair("Menu Name To Copy From", "|15Enter menu name to |11COPY|15 : ");
    value[PROMPT_MENU_COPY_TO]            = std::make_pair("Menu Name To Copy To", "|15Enter menu name |11SAVE|15 as : ");    
    value[PROMPT_INVALID_MENU_EXISTS]     = std::make_pair("Invalid Menu Exists", "|04Invalid, Menu already exist.|CR");
    value[PROMPT_INVALID_MENU_NOT_EXISTS] = std::make_pair("Invalid Menu Doesn't Exist", "|04Invalid, Menu doesn't exist.|CR");

    // Menu Field Edit
    value[PROMPT_MENU_FIELD_INPUT_TEXT]   = std::make_pair("Menu Field Edit Prompt", "|CR|15C|07om|08mand |15: |07");
    value[PROMPT_MENU_FIELD_TITLE]        = std::make_pair("Menu Field Title", "|CR |03(|11A|03) |15Menu Title         : ");



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
 * @brief Sets an indivdual Menu Input State Add/Change/Delete
 * @param mod_menu_state_index
 */
void ModMenuEditor::changeMenuInputState(int mod_menu_state_index)
{
    m_mod_menu_state_index = mod_menu_state_index;
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
 * @brief Toggle the Option View.
 */
void ModMenuEditor::toggleNextOptionView()
{
    if (m_mod_toggle_view_index == m_max_toggled_view_index)
        m_mod_toggle_view_index = VIEW_DEFAULT;
    else
        ++m_mod_toggle_view_index;
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
 * @brief Pull and Display Prompts with MCI Code
 * @param prompt
 */
void ModMenuEditor::displayPromptMCI(const std::string &prompt, const std::string &mci_field)
{
    baseDisplayPromptMCI(prompt, m_text_prompts_dao, mci_field);
}

/**
 * @brief Pull and Display Prompts with following newline
 * @param prompt
 */
void ModMenuEditor::displayPromptAndNewLine(const std::string &prompt)
{
    baseDisplayPromptAndNewLine(prompt, m_text_prompts_dao);
}

/**
 * @brief Setup for the Menu Editor 
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
    m_page = 0;        
    displayCurrentPage(PROMPT_INPUT_TEXT);
}

/**
 * @brief Setup for the Menu Options
 * @return
 */
void ModMenuEditor::setupMenuOptionEditor() 
{
    std::cout << "setupMenuOptionEditor() - " << m_current_menu << std::endl;    
    std::string display_name = m_current_menu;
    baseTransformToUpper(display_name);
    displayPromptMCI(PROMPT_OPTION_HEADER, display_name);
    
    // Build a list of screen lines for the menu display
    // So we know when to pause in large listing, or use pagenation.
    std::string menu_display_output = displayMenuOptionList();    
    
    if (m_menu_display_list.size() == 0) 
    {
        // Clear Out list if anything already exists.
        std::vector<std::string>().swap(m_menu_display_list);
    }
    
    m_menu_display_list = m_common_io.splitString(menu_display_output, '\n');
    m_page = 0;
        
    displayCurrentPage(PROMPT_OPTION_INPUT_TEXT);
}

/**
 * @brief Setup for the Menu Editor 
 * @return
 */
void ModMenuEditor::setupMenuEditFields() 
{
    std::cout << "setupMenuEditFields() - " << m_current_menu << std::endl;
    std::string display_name = m_current_menu;
    baseTransformToUpper(display_name);
    displayPromptMCI(PROMPT_MENU_EDIT_HEADER, display_name);
    
    // Build a list of screen lines for the menu display
    // So we know when to pause in large listing, or use pagenation.
    std::string menu_display_output = displayMenuEditScreen();    
    
    if (m_menu_display_list.size() == 0) 
    {
        // Clear Out list if anything already exists.
        std::vector<std::string>().swap(m_menu_display_list);
    }
    
    m_menu_display_list = m_common_io.splitString(menu_display_output, '\n');           
    m_page = 0;        
    displayCurrentEditPage(PROMPT_MENU_FIELD_INPUT_TEXT);
}

/**
 * @brief Displays the current page of menu items
 */
void ModMenuEditor::displayCurrentPage(const std::string &input_state) 
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
    
    // Default Page Input Method
    unsigned int current_module_input;
    switch(m_mod_setup_index)
    {
        case MOD_DISPLAY_MENU:
            current_module_input = MOD_MENU_INPUT;
            break;
            
        case MOD_DISPLAY_MENU_OPTIONS:
            current_module_input = MOD_MENU_OPTION_INPUT;
            break;
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
 * @brief Displays the current page of menu items
 */
void ModMenuEditor::displayCurrentEditPage(const std::string &input_state) 
{
    for (unsigned int i = 0; i < m_menu_display_list.size(); i++) 
    {
        std::string display_line = m_session_io.pipe2ansi(m_menu_display_list[i]);
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
    }  
    
    displayPrompt(input_state); // prompt for edit
    changeInputModule(current_module_input); // switch to the input module for field edits.
}

/**
 * @brief Handles Input (Waiting for Any Key Press)
 * @param input
 */
void ModMenuEditor::menuEditorPausedInput(const std::string &input)
{    
    
    std::cout << " *** menuEditorPausedInput !!!" << std::endl;
    std::cout << "**************************" << std::endl;
    
    std::string current_state_input;    
    unsigned int current_module_input;    
    switch(m_mod_setup_index)
    {
        case MOD_DISPLAY_MENU:
            current_state_input = PROMPT_INPUT_TEXT;
            current_module_input = MOD_MENU_INPUT;
            break;
            
        case MOD_DISPLAY_MENU_OPTIONS:
            current_state_input = PROMPT_OPTION_INPUT_TEXT;
            current_module_input = MOD_MENU_OPTION_INPUT;
            break;
    }    
    
    // Check for abort on pause for next.
    if (input.size() == 1 && std::toupper(input[0]) == 'A')
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
                changeMenuInputState(MENU_ADD);
                displayPrompt(PROMPT_MENU_ADD);
                changeInputModule(MOD_MENU_NAME);
                break;
                
            case 'E': // Change/Edit
                std::cout << "change" << std::endl;
                changeMenuInputState(MENU_CHANGE);
                displayPrompt(PROMPT_MENU_CHANGE);
                changeInputModule(MOD_MENU_NAME);
                break;
                
            case 'D': // Delete
                std::cout << "delete" << std::endl;
                changeMenuInputState(MENU_DELETE);
                displayPrompt(PROMPT_MENU_DELETE);
                changeInputModule(MOD_MENU_NAME);
                break;
                
            case 'C': // Copy
                std::cout << "copy" << std::endl;
                changeMenuInputState(MENU_COPY_FROM);
                displayPrompt(PROMPT_MENU_COPY_FROM);
                changeInputModule(MOD_MENU_NAME);
                break;
                
            case 'Q': // Quit
                std::cout << "quit" << std::endl;
                // Reload fall back, or gosub to last menu!
                m_is_active = false;
                return;
                
            default:
                // Roll back to main Editor screen and input method.
                //displayPromptAndNewLine(PROMPT_INPUT_TEXT);
                //changeInputModule(MOD_MENU_INPUT);
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
 * @brief Handles Menu Option Editor Command Selection
 * @param input
 */
void ModMenuEditor::menuEditorOptionInput(const std::string &input)
{
    std::cout << " *** menuEditorOptionInput !!!" << std::endl;
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
                changeMenuInputState(MENU_ADD);
                displayPrompt(PROMPT_MENU_ADD);
                changeInputModule(MOD_MENU_OPTION);
                break;
                
            case 'E': // Change/Edit
                std::cout << "change" << std::endl;
                changeMenuInputState(MENU_CHANGE);
                displayPrompt(PROMPT_MENU_CHANGE);
                changeInputModule(MOD_MENU_OPTION);
                break;
                
            case 'D': // Delete
                std::cout << "delete" << std::endl;
                changeMenuInputState(MENU_DELETE);
                displayPrompt(PROMPT_MENU_DELETE);
                changeInputModule(MOD_MENU_OPTION);
                break;
                
            case 'C': // Copy
                std::cout << "copy" << std::endl;
                changeMenuInputState(MENU_COPY_FROM);
                displayPrompt(PROMPT_MENU_COPY_FROM);
                changeInputModule(MOD_MENU_OPTION);
                break;
                
            case 'Q': // Quit
                std::cout << "quit option" << std::endl;
                // Reload fall back, or gosub to Menu Editor Main
                changeInputModule(MOD_MENU_FIELD_INPUT);
                changeSetupModule(MOD_DISPLAY_MENU_EDIT);  // Change to Menu Edit Fields!            
                return;
                
            case 'T': // Toggle Option View
                toggleNextOptionView();
                redisplayModulePrompt();
                break;
                
            default:
                // Roll back to main Editor screen and input method.
                //displayPromptAndNewLine(PROMPT_INPUT_TEXT);                
                //changeInputModule(MOD_MENU_INPUT);
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
 * @brief Handles Menu Field Editor Command Selection
 * @param input
 */
void ModMenuEditor::menuEditorMenuFieldInput(const std::string &input)
{
    std::cout << " *** menuEditorMenuFieldInput !!!" << std::endl;
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
                
            case 'A': // Menu Title
                std::cout << "change menu title" << std::endl;
                m_current_field = toupper(key[0]);
                changeInputModule(MOD_MENU_FIELD);
                displayPrompt(PROMPT_MENU_FIELD_TITLE);
            
            case 'H': // Jump into Options Editing.
                std::cout << "change options" << std::endl;
                changeInputModule(MOD_MENU_OPTION_INPUT);
                changeSetupModule(MOD_DISPLAY_MENU_OPTIONS);                                
                break; 
                
            case 'Q': // Quit
                std::cout << "menu field quit" << std::endl;
                // Reload fall back, or gosub to Menu Editor Main
                changeInputModule(MOD_MENU_INPUT);
                changeSetupModule(MOD_DISPLAY_MENU);                
                return;
                
            default:
                // Roll back to main Editor screen and input method.
                //displayPromptAndNewLine(PROMPT_INPUT_TEXT);                
                //changeInputModule(MOD_MENU_INPUT);
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
 * @brief Handles Field Updates for Menu Data
 * @param input
 */
void ModMenuEditor::menuEditorMenuFieldHandler(const std::string &input)
{
    std::cout << " *** menuEditorMenuNameInput !!!" << std::endl;
    std::cout << "**************************" << std::endl;
    
    std::string key = "";
    std::string result = m_session_io.getInputField(input, key, Config::sName_length);

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
        
         // Handle the assigned input field
        switch(m_current_field)
        {
            case 'A': // Menu Title
                
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
 * @brief Handles Menu Name Input for Add/Change/Delete Methods calls.
 * @param input
 */
void ModMenuEditor::menuEditorMenuNameInput(const std::string &input)
{
    std::cout << " *** menuEditorMenuNameInput !!!" << std::endl;
    std::cout << "**************************" << std::endl;
    
    std::string key = "";
    std::string result = m_session_io.getInputField(input, key, Config::sName_length);

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
        
        if (checkMenuExists(key))
        {            
            std::cout << " * Menu name matches!" << std::endl;            
            handleMenuInputState(true, key);
        }
        else 
        {            
            std::cout << " * Menu name doesn't match!" << std::endl;            
            handleMenuInputState(false, key);
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
 * @brief Handles Menu Option Input for Add/Change/Delete/Copy/Move Methods calls.
 * @param input
 */
void ModMenuEditor::menuEditorMenuOptionInput(const std::string &input)
{
    std::cout << " *** menuEditorMenuOptionInput !!!" << std::endl;
    std::cout << "**************************" << std::endl;
    
    std::string key = "";
    std::string result = m_session_io.getInputField(input, key, Config::sName_length);

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
        
        int option_index = 0;
        std::stringstream ss(key);
        ss >> option_index;
        
        // check for Invalid Index.
        if (ss.fail() || option_index < 0)
        {
            return;
        }
        
        if (checkMenuOptionExists(option_index))
        {            
            std::cout << " * Menu option matches!" << std::endl;            
            handleMenuOptionInputState(true, option_index);
        }
        else 
        {            
            std::cout << " * Menu option doesn't match!" << std::endl;            
            handleMenuOptionInputState(false, option_index);
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
 * @brief handle each menu seperate state and what to do next on input.
 * @param does_menu_exist
 * @param menu_name
 */
void ModMenuEditor::handleMenuInputState(bool does_menu_exist, const std::string &menu_name) 
{    
    switch (m_mod_menu_state_index)
    {
        case MENU_ADD:
            if (does_menu_exist)
            {
                // Error, can't create a menu that already exists!
                displayPrompt(PROMPT_INVALID_MENU_EXISTS);
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
            if (does_menu_exist)
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
                displayPrompt(PROMPT_INVALID_MENU_NOT_EXISTS);
                displayPrompt(PROMPT_INPUT_TEXT);
                changeInputModule(MOD_MENU_INPUT);
            }            
            break;
            
        case MENU_DELETE:
            if (does_menu_exist)
            {
                deleteExistingMenu(menu_name);
                changeInputModule(MOD_MENU_INPUT);
                redisplayModulePrompt();
            }
            else
            {
                // Error, can't remove a menu that doesn't exist!
                displayPrompt(PROMPT_INVALID_MENU_NOT_EXISTS);
                displayPrompt(PROMPT_INPUT_TEXT);
                changeInputModule(MOD_MENU_INPUT);
            }
            break;
            
        case MENU_COPY_FROM:
            // [Source]
            // Notes, this will take the source, then move to the 
            // MENU_COPY_TO for destination.  Source is saved as m_current Menu
            if (does_menu_exist) 
            {
                m_current_menu = menu_name;
                changeMenuInputState(MENU_COPY_TO);
                displayPrompt(PROMPT_MENU_COPY_TO);
            }
            else 
            {
                // Error, can't copy a menu that doesn't exist!
                displayPrompt(PROMPT_INVALID_MENU_NOT_EXISTS);
                displayPrompt(PROMPT_INPUT_TEXT);
                changeInputModule(MOD_MENU_INPUT);
            }
            break;
            
        case MENU_COPY_TO:
            // [Destination]
            if (does_menu_exist) 
            {
                // Error, can't copy or overwrite a destination menu that exists!
                displayPrompt(PROMPT_INVALID_MENU_NOT_EXISTS);
                displayPrompt(PROMPT_INPUT_TEXT);
                changeInputModule(MOD_MENU_INPUT);
            }
            else 
            {
                copyExistingMenu(menu_name);
                changeInputModule(MOD_MENU_INPUT);
                redisplayModulePrompt();
            }
            break;
    }        
}

/**
 * @brief handle each seperate state and what to do next on input.
 * @param does_option_exist
 * @param option_index
 */
void ModMenuEditor::handleMenuOptionInputState(bool does_option_exist, int option_index) 
{    
    switch (m_mod_menu_state_index)
    {
        case MENU_ADD:
            if (does_option_exist)
            {
                // Error, can't create a menu that already exists!
                displayPrompt(PROMPT_INVALID_MENU_EXISTS);
                displayPrompt(PROMPT_INPUT_TEXT);
                changeInputModule(MOD_MENU_OPTION_INPUT);
            }
            else 
            {
               // createNewMenu(menu_name);
                changeInputModule(MOD_MENU_OPTION_INPUT);
                redisplayModulePrompt();
            }
            break;
            
        case MENU_CHANGE:
            if (does_option_exist)
            {
                // Move to new Default setup for Options vs Menus.
                // Also set the curent menu for the system to load
                // to pull the commands from.
                m_current_option = option_index;
                changeSetupModule(MOD_DISPLAY_MENU_OPTIONS);                
            }
            else
            {
                // Error, can't remove a menu that doesn't exist!
                displayPrompt(PROMPT_INVALID_MENU_NOT_EXISTS);
                displayPrompt(PROMPT_INPUT_TEXT);
                changeInputModule(MOD_MENU_OPTION_INPUT);
            }            
            break;
            
        case MENU_DELETE:
            if (does_option_exist)
            {
               // deleteExistingMenu(menu_name);
                changeInputModule(MOD_MENU_OPTION_INPUT);
                redisplayModulePrompt();
            }
            else
            {
                // Error, can't remove a menu that doesn't exist!
                displayPrompt(PROMPT_INVALID_MENU_NOT_EXISTS);
                displayPrompt(PROMPT_INPUT_TEXT);
                changeInputModule(MOD_MENU_OPTION_INPUT);
            }
            break;
            
        case MENU_COPY_FROM:
            // [Source]
            // Notes, this will take the source, then move to the 
            // MENU_COPY_TO for destination.  Source is saved as m_current Menu
            if (does_option_exist) 
            {
                m_current_option = option_index;
                changeMenuInputState(MENU_COPY_TO);
                displayPrompt(PROMPT_MENU_COPY_TO);
            }
            else 
            {
                // Error, can't copy a menu that doesn't exist!
                displayPrompt(PROMPT_INVALID_MENU_NOT_EXISTS);
                displayPrompt(PROMPT_INPUT_TEXT);
                changeInputModule(MOD_MENU_OPTION_INPUT);
            }
            break;
            
        case MENU_COPY_TO:
            // [Source]
            // Notes, this will take the source, then move to the 
            // MENU_COPY_TO for destination.  Source is saved as m_current Menu
            if (does_option_exist) 
            {
                m_current_option = option_index;
                changeMenuInputState(MENU_COPY_TO);
                displayPrompt(PROMPT_MENU_COPY_TO);
            }
            else 
            {
                // Error, can't copy a menu that doesn't exist!
                displayPrompt(PROMPT_INVALID_MENU_NOT_EXISTS);
                displayPrompt(PROMPT_INPUT_TEXT);
                changeInputModule(MOD_MENU_OPTION_INPUT);
            }
            break;
            
        case MENU_MOVE_FROM:
            // [Destination]
            if (does_option_exist) 
            {
                // Error, can't copy or overwrite a destination menu that exists!
                displayPrompt(PROMPT_INVALID_MENU_NOT_EXISTS);
                displayPrompt(PROMPT_INPUT_TEXT);
                changeInputModule(MOD_MENU_OPTION_INPUT);
            }
            else 
            {
                //copyExistingMenu(menu_name);
                changeInputModule(MOD_MENU_OPTION_INPUT);
                redisplayModulePrompt();
            }
            break;
            
        case MENU_MOVE_TO:
            // [Destination]
            if (does_option_exist) 
            {
                // Error, can't copy or overwrite a destination menu that exists!
                displayPrompt(PROMPT_INVALID_MENU_NOT_EXISTS);
                displayPrompt(PROMPT_INPUT_TEXT);
                changeInputModule(MOD_MENU_OPTION_INPUT);
            }
            else 
            {
               // copyExistingMenu(menu_name);
                changeInputModule(MOD_MENU_OPTION_INPUT);
                redisplayModulePrompt();
            }
            break;
    }        
}

/**
 * @brief Create a new empty Menu
 * @param menu_name
 */
void ModMenuEditor::createNewMenu(const std::string &menu_name)
{
    // Pre-Load Menu, check access, if not valud, then fall back to previous.
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
}

/**
 * @brief Delete an existing Menu
 * @param menu_name
 */
void ModMenuEditor::deleteExistingMenu(const std::string &menu_name)
{
    // Pre-Load Menu, check access, if not valud, then fall back to previous.
    menu_ptr new_menu(new Menu());
    
    // Call MenuDao to save .yaml menu file
    MenuDao mnu(new_menu, menu_name, GLOBAL_MENU_PATH);
    if (mnu.fileExists())
    {
        mnu.deleteMenu();
    }
}

/**
 * @brief Create a new empty Menu
 * @param menu_name
 */
void ModMenuEditor::copyExistingMenu(const std::string &menu_name)
{
    // Pre-Load Menu, check access, if not valud, then fall back to previous.
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
}

/**
 * @brief Check if the menu exists in the current listing
 * @param menu_name
 * @return 
 */
bool ModMenuEditor::checkMenuExists(std::string menu_name)
{
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
    
    return false;
}

/**
 * @brief Check if the menu option exists in the current listing
 * @param menu_option
 * @return 
 */
bool ModMenuEditor::checkMenuOptionExists(int option_index)
{
    // Pre-Load Menu, check access, if not valud, then fall back to previous.
    menu_ptr current_menu(new Menu());
    
    // First load the Source Menu [m_current_menu] file name
    MenuDao mnu_source(current_menu, m_current_menu, GLOBAL_MENU_PATH);
    if (mnu_source.fileExists())
    {
        mnu_source.loadMenu();
    }
    else
    {
        std::cout << "Source menu file doesn't exist!" << std::endl;
        return false;
    }
    
    // Check if it's out of bounds, negative already checked on caller.
    if ((unsigned int)option_index >= current_menu->menu_options.size())
    {
        return false;        
    }
        
    // Check input index, and get command that matches.
    for(unsigned int i = 0; i < current_menu->menu_options.size(); i++)
    {
        auto &m = current_menu->menu_options[i];        
        if (m.index == option_index)
        {
            return true;            
        }
    }    
    
    return false;
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
                buffer += baseGetDefaultColor();
                buffer += top_left;
            }
            else if(rows == 0 && cols == max_cols-1)
            {
                buffer += baseGetDefaultColor();
                buffer += top_right;
            }
            else if(rows == 0 && cols % 9 == 0)
            {
                buffer += baseGetDefaultColor();
                buffer += mid_top;
            }
            else if(rows == 0)
            {
                buffer += baseGetDefaultColor();
                buffer += row;
            }

            // Bottom Row
            else if(rows == total_rows-1 && cols == 0)
            {
                buffer += baseGetDefaultColor();
                buffer += bot_left;
            }
            else if(rows == total_rows-1 && cols == max_cols-1)
            {
                buffer += baseGetDefaultColor();
                buffer += bot_right;
            }
            else if(rows == total_rows-1 && cols % 9 == 0)
            {
                buffer += baseGetDefaultColor();
                buffer += mid_bot;
            }
            else if(rows == total_rows-1)
            {
                buffer += baseGetDefaultColor();
                buffer += row;
            }
            else if(cols % 9 == 0)
            {
                buffer += baseGetDefaultColor();
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
}

/**
 * @brief Menu Editor, Read and Modify Menus Options
 */
std::string ModMenuEditor::displayMenuOptionList()
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
   
    menu_ptr current_menu = m_loaded_menu.back();

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
                option_string.append(m_common_io.rightPadding(m.menu_key, 9));
                option_string.append(m_common_io.rightPadding(m.command_key, 4));
                option_string.append(m_common_io.rightPadding(m.acs_string, 8));
                break;
        
            case VIEW_NAMES:            
                option_string.append(m_common_io.rightPadding(m.name, 21));
                break;
                
            case VIEW_STRINGS:
                option_string.append(m_common_io.rightPadding(m.command_string, 15));
                option_string.append(m_common_io.rightPadding("", 1));
                option_string.append(m_common_io.rightPadding(m_common_io.boolAlpha(m.hidden), 5));
                break;
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
    int max_cols  = 76; // out of 80

    // Vector or Menus, Loop through
    std::vector<std::string>::iterator i = result_set.begin();
    //std::string menu_name;
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
                buffer += top_left;
            }
            else if(rows == 0 && cols == max_cols-1)
            {
                buffer += baseGetDefaultColor();
                buffer += top_right;
            }
            else if(rows == 0 && cols % 25 == 0)
            {
                buffer += baseGetDefaultColor();
                buffer += mid_top;
            } 
            else if(rows == 0)
            {
                buffer += baseGetDefaultColor();
                buffer += row;
            }

            // Bottom Row
            else if(rows == total_rows-1 && cols == 0)
            {
                buffer += baseGetDefaultColor();
                buffer += bot_left;
            }
            else if(rows == total_rows-1 && cols == max_cols-1)
            {
                buffer += baseGetDefaultColor();
                buffer += bot_right;
            }
            else if(rows == total_rows-1 && cols % 25 == 0)
            {
                buffer += baseGetDefaultColor();
                buffer += mid_bot;
            } 
            else if(rows == total_rows-1)
            {
                buffer += baseGetDefaultColor();
                buffer += row;
            }
            else if(cols % 25 == 0)
            {
                buffer += baseGetDefaultColor();
                buffer += mid;
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
 * @brief Menu Editor, for Dispalying Menu Fields to Edit
 */
std::string ModMenuEditor::displayMenuEditScreen()
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
   

    // Create Menu Pointer then load the menu into it.
    menu_ptr current_menu(new Menu());
    
    // First load the Source Menu [m_current_menu] file name
    MenuDao mnu_source(current_menu, m_current_menu, GLOBAL_MENU_PATH);
    if (mnu_source.fileExists())
    {
        mnu_source.loadMenu();
        
        // Save Menu into memory for changes, and rollbacks
        std::vector<menu_ptr>().swap(m_loaded_menu);
        m_loaded_menu.push_back(current_menu);
    }
    else 
    {
        // Error, drop back to Menu Editor.
        changeSetupModule(MOD_DISPLAY_MENU);
        return "";
    }

    // Build a string list of individual menu options, then loop to fit as many per screen!
    std::vector<std::string> result_set;

    result_set.push_back("     |11File Version       : |03" + m_common_io.rightPadding(current_menu->file_version, 48));
    result_set.push_back(" |03(|11A|03) |15Menu Title         : |03" + m_common_io.rightPadding(current_menu->menu_title, 48));
    result_set.push_back(" |03(|11B|03) |15Password           : |03" + m_common_io.rightPadding(current_menu->menu_password, 48));    
    result_set.push_back(" |03(|11C|03) |15Fallback Menu      : |03" + m_common_io.rightPadding(current_menu->menu_fall_back, 48));
    result_set.push_back(" |03(|11D|03) |15Help ID            : |03" + m_common_io.rightPadding(current_menu->menu_help_file, 48));
    result_set.push_back(" |03(|11E|03) |15Name in Prompt     : |03" + m_common_io.rightPadding(current_menu->menu_name, 48));
    result_set.push_back(" |03(|11F|03) |15Pulldown File      : |03" + m_common_io.rightPadding(current_menu->menu_password, 48));
    result_set.push_back(" |03(|11H|03) |15Edit Options         " + m_common_io.rightPadding("", 48));
    result_set.push_back(" |03(|11F|03) |15View Generic Menu    " + m_common_io.rightPadding("", 48));
    result_set.push_back(" |03(|11Q|03) |15Quit                 " + m_common_io.rightPadding("", 48));
                
    // Not in use Yet, seems legacy only does ACS in option commands.
    // option_string.append("Menu ACS           : " + m_common_io.rightPadding(current_menu->menu_acs_string, 35);  
    // option_string.append("Menu FormMenu      : " + m_common_io.rightPadding(current_menu->menu_form_menu, 8); 

    // iterate through and print out
    int total_rows = result_set.size();
    total_rows += 2;

    // Could re-calc this on screen width lateron.
    int max_cols  = 76;

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
                buffer += top_left;
            }
            else if(rows == 0 && cols == max_cols-1)
            {
                buffer += baseGetDefaultColor();
                buffer += top_right;
            }
            else if(rows == 0 && cols % 75 == 0)
            {
                buffer += baseGetDefaultColor();
                buffer += mid_top;
            } 
            else if(rows == 0)
            {
                buffer += baseGetDefaultColor();
                buffer += row;
            }

            // Bottom Row
            else if(rows == total_rows-1 && cols == 0)
            {
                buffer += baseGetDefaultColor();
                buffer += bot_left;
            }
            else if(rows == total_rows-1 && cols == max_cols-1)
            {
                buffer += baseGetDefaultColor();
                buffer += bot_right;
            }
            else if(rows == total_rows-1 && cols % 75 == 0)
            {
                buffer += baseGetDefaultColor();
                buffer += mid_bot;
            } 
            else if(rows == total_rows-1)
            {
                buffer += baseGetDefaultColor();
                buffer += row;
            }
            else if(cols % 75 == 0)
            {
                buffer += baseGetDefaultColor();
                buffer += mid;
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
}
