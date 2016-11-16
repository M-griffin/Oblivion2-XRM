#include "menu_system.hpp"

#include "data/config_dao.hpp"
#include "mods/mod_prelogon.hpp"
#include "mods/mod_logon.hpp"
#include "mods/mod_signup.hpp"

#include <string>
#include <vector>
#include <functional>


const std::string MenuSystem::m_menuID = "MENU_SYSTEM";

MenuSystem::MenuSystem(session_data_ptr session_data)
    : StateBase(session_data)
    , MenuBase(session_data)
{
    std::cout << "MenuSystem" << std::endl;

    // Setup std::function array with available options to pass input to.
    m_menu_functions.push_back(std::bind(&MenuBase::menuInput, this, std::placeholders::_1, std::placeholders::_2));
    m_menu_functions.push_back(std::bind(&MenuSystem::menuEditorInput, this, std::placeholders::_1, std::placeholders::_2));
    m_menu_functions.push_back(std::bind(&MenuSystem::modulePreLogonInput, this, std::placeholders::_1, std::placeholders::_2));
    m_menu_functions.push_back(std::bind(&MenuSystem::moduleInput, this, std::placeholders::_1, std::placeholders::_2));

    // Setup Menu Option Calls for executing menu commands.
    m_execute_callback.push_back(std::bind(&MenuSystem::menuOptionsCallback, this, std::placeholders::_1));


    // Load the configuration file to the class
    if (!m_config_dao->loadConfig())
    {
        std::cout << "Error: unable to load configuration file" << std::endl;
        assert(false);
    }
}


MenuSystem::~MenuSystem()
{
    std::cout << "~MenuSystem" << std::endl;
}


/**
 * @brief Handles Updates or Data Input from Client
 */
void MenuSystem::update(const std::string &character_buffer, const bool &is_utf8)
{
    if(!m_is_active)
    {
        return;
    }

    // This simplily passed through the input to the current system fuction were at.
    m_menu_functions[m_input_index](character_buffer, is_utf8);
}


/**
 * @brief Startup class, setup initial screens / interface, flags etc..
 *        This is only called when switch to the state, not for menu instances.
 * @return
 */
bool MenuSystem::onEnter()
{
    std::cout << "OnEnter() MenuSystem" << std::endl;

    // Startup the Prelogon sequence
    startupModulePreLogon();

    m_is_active = true;

    return true;
}


/**
 * @brief Exit, close down, display screens to change over data.
 * @return
 */
bool MenuSystem::onExit()
{
    std::cout << "OnExit() MenuSystem\n";
    m_is_active = false;
    return true;
}


/**
 * @brief Process Command Keys passed from menu selection (Callback)
 * @param option
 */
void MenuSystem::menuOptionsCallback(const MenuCompatOption &option)
{
    /* Run through the case and switch over the new interface.
    std::string mnuOption = option.CKeys;
    std::string mnuString = option.CString;
    std::string mnuAccess = option.Acs; */

    // If Invalid then return
    if(strlen((const char *)option.CKeys) != 2)
    {
        return;
    }

    // Run through Comamnd Keys for Method
    switch(option.CKeys[0])
    {
        case '-':
            switch(option.CKeys[1])
            {
                    // Turns on Pulldown Menu Re-entrance
                    // This option returns to the selected option
                    // when the user re-enters the pulldown menu.
                    // This works ONLY if the command that the user
                    // executed does not go to another menu.
                case '\'':
                    break;

                    // Turns off Pulldown Menu Re-Entrance
                case '`':
                    break;
            }
            break;

            // Matrix Menu Commands
        case '{':
            switch(option.CKeys[1])
            {
                    // Logon
                case 'S':
                    std::cout << "Executing startupModuleLogon()" << std::endl;
                    startupModuleLogon();
                    break;

                    // Apply
                case 'A':
                    std::cout << "Executing startupModuleSignup();" << std::endl;
                    startupModuleSignup();
                    break;

                    // Check
                case 'C':
                    break;

                    // Feedback
                case 'F':
                    break;

                    // Chat
                case 'P':
                    break;

                    // Logoff
                case 'G':
                    std::cout << "Goodbye;" << std::endl;
                    m_session_data->logoff();
                    break;
            }
            break;

            // Sysop Commands
        case '*':
            switch(option.CKeys[1])
            {
                    // Menu Editor
                case '#':
                    break;

                    // Configuration Menu
                case 'C':
                    break;
            }
            break;
    }
}


/**
 * @brief Startup Menu Editor and Switch to MenuEditorInput
 */
void MenuSystem::startupMenuEditor()
{
    std::cout << "Entering MenuEditor Input " << std::endl;

    // Setup the input processor
    m_input_index = MENU_EDITOR_INPUT;

    // 2. Handle any init and startup Screen Displays

    // WIP, nothing completed just yet for the startup.
}


/**
 * @brief Menu Editor, Read and Modify Menus
 */
void MenuSystem::menuEditorInput(const std::string &character_buffer, const bool &)
{
    // If were not using hot keys, loop input untill we get ENTER
    // Then handle only the first key in the buffer.  Other "SYSTEMS"
    // Will parse for entire line for matching Command Keys.
    if (!m_use_hotkey)
    {
        // Received ENTER, grab the previous input.
        if (!(character_buffer[0] == 13))
        {
            m_line_buffer += character_buffer[0];
            m_session_data->deliver(m_line_buffer);
            //return output_buffer;
        }
    }
    else
    {
        m_line_buffer += character_buffer[0];
    }

    std::string output_buffer = "";
    switch (std::toupper(m_line_buffer[0]))
    {
        case 'A': // Add
            output_buffer = "Enter Menu Name to Add : ";
            break;
        case 'C': // Change/Edit
            output_buffer = "Enter Menu Name to Change : ";
            break;
        case 'D': // Delete
            output_buffer = "Enter Menu to Delete : ";
            break;
        case 'Q': // Quit
            // Reload fall back, or gosub to last menu!


            return;
        default : // Return
            return;
    }

    m_session_data->deliver(output_buffer);
}


/**
 * @brief Start up the Normal Login Process.
 */
void MenuSystem::startupModulePreLogon()
{
    // Setup the input processor
    m_input_index = MODULE_PRELOGON_INPUT;

    // Allocate the Module here and push to container
    module_ptr module(new ModPreLogon(m_session_data, m_config, m_ansi_process));
    if (!module)
    {
        std::cout << "ModPreLogon Allocation Error!" << std::endl;
        assert(false);
    }

    // First clear any left overs if they exist.
    if (m_module.size() > 0)
    {
        std::vector<module_ptr>().swap(m_module);
    }

    // Run the startup for the module
    module->onEnter();

    // Push to stack now the new module.
    m_module.push_back(module);
}


/**
 * @brief Start up the Normal Login Process.
 */
void MenuSystem::startupModuleLogon()
{
    // Setup the input processor
    m_input_index = MODULE_INPUT;

    // Allocate the Module here and push to container
    module_ptr module(new ModLogon(m_session_data, m_config, m_ansi_process));
    if (!module)
    {
        std::cout << "ModLogon Allocation Error!" << std::endl;
        assert(false);
    }

    // First clear any left overs if they exist.
    if (m_module.size() > 0)
    {
        std::vector<module_ptr>().swap(m_module);
    }

    // Run the startup for the module
    module->onEnter();

    // Push to stack now the new module.
    m_module.push_back(module);
}


/**
 * @brief Starts up Signup Module
 */
void MenuSystem::startupModuleSignup()
{
    // Setup the input processor
    m_input_index = MODULE_INPUT;

    // Allocate the Module here and push to container
    module_ptr module(new ModSignup(m_session_data, m_config, m_ansi_process));
    if (!module)
    {
        std::cout << "ModSignup Allocation Error!" << std::endl;
        assert(false);
    }

    // First clear any left overs if they exist.
    if (m_module.size() > 0)
    {
        std::vector<module_ptr>().swap(m_module);
    }

    // Run the startup for the module
    module->onEnter();

    // Push to stack now the new module.
    m_module.push_back(module);
}


/**
 * @brief Handles parsing input for modules
 *
 */
void MenuSystem::modulePreLogonInput(const std::string &character_buffer, const bool &is_utf8)
{
    std::cout << "modulePreLogonInput" << std::endl;
    // Make sure we have an allocated module before processing.
    if (m_module.size() == 0)
    {
        std::cout << "modulePreLogonInput size 0" << std::endl;
        return;
    }

    // Make sure we have data
    if (character_buffer.size() == 0)
    {
        std::cout << "modulePreLogonInput char_buff size 0" << std::endl;
        return;
    }

    // Execute the modules update passing through input.
    // result = true, means were still active, false means completed, return to menu!
    //bool result = m_module[0]->update(character_buffer, is_utf8);

    // Don't need return result on this.
    m_module[0]->update(character_buffer, is_utf8);

    // Finished modules processing.
    if (!m_module[0]->m_is_active)
    {
        // Do module shutdown /// NEED A MODULE FALL BACK TO register menu or a new module to load.
        m_module[0]->onExit();

        // First pop the module off the stack to deallocate
        m_module.pop_back();


        // Check if the current user has been logged in yet.
        if (!m_session_data->m_is_session_authorized)
        {
            std::cout << "!m_is_session_authorized" << std::endl;

            m_current_menu = "MATRIX.MNU";
            startupMenu();

            // Access any needed global configuration values
            // For Example...
            /*
            m_config_dao->use_matrix_login  etc..
            if(cfg->use_matrix_login)
            {
                // Setup Matrix Menu
                std::cout << "cfg->use_matrix_login" << std::endl;

                // Set the Next Menu to Load, Matrix for Login
                m_current_menu = "MATRIX.MNU";
                startupMenu();
            }
            else
            {
                // Setup the login Module for normal login sequence.
                std::cout << "!cfg->use_matrix_login" << std::endl;

                // Set The Default menu to jump to after logon
                m_current_menu = "TOP.MNU";
                startupModuleLogon();
            }*/
        }
        else
        {
            std::cout << "m_is_session_authorized" << std::endl;
            // Reset the Input back to the Menu System
        }
    }
}


/**
 * @brief Handles parsing input for modules
 *
 */
void MenuSystem::moduleInput(const std::string &character_buffer, const bool &is_utf8)
{
    // Make sure we have an allocated module before processing.
    if (m_module.size() == 0)
    {
        return;
    }

    // Make sure we have data
    if (character_buffer.size() == 0)
    {
        return;
    }

    // Execute the modules update passing through input.
    // result = true, means were still active, false means completed, return to menu!

    // Only check result when m_is_active == false!
    bool result = m_module[0]->update(character_buffer, is_utf8);

    // Finished modules processing.
    if (!m_module[0]->m_is_active)
    {
        // Do module shutdown /// NEED A MODULE FALL BACK TO register menu or a new module to load.
        m_module[0]->onExit();

        // First pop the module off the stack to deallocate
        m_module.pop_back();

        if (result)
        {
            // Check if authorized, if not, then hangup here!
        }

        // Reset the Input back to the Menu System
        m_input_index = MENU_INPUT;

        // Redisplay,  may need to startup() again, but menu data should still be active and loaded!
        redisplayMenuScreen();
    }
}
