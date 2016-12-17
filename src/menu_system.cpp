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
    m_menu_functions.push_back(std::bind(&MenuSystem::moduleLogonInput, this, std::placeholders::_1, std::placeholders::_2));
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
 * TODO - Menu Options, lets break out each section
 * option.command_key[1] into their own methods
 * Making this call back smaller.
 */
 
 
/**
 * @brief Control Commands
 * @param option
 */
bool MenuSystem::menuOptionsControlCommands(const MenuOption &option)
{
    switch(option.command_key[1])
    {
            // Turns on Pulldown Menu Re-entrance
            // This option returns to the selected option
            // when the user re-enters the pulldown menu.
            // This works ONLY if the command that the user
            // executed does not go to another menu.
        case '\'':
            return false;
            // Turns off Pulldown Menu Re-Entrance
            
        case '`':
            return false;
            
        default:
            return false;
    }

    return true;
}

/**
 * @brief MultiNode Commands
 * @param option
 */
bool MenuSystem::menuOptionsMultiNodeCommands(const MenuOption &option)
{
    switch(option.command_key[1])
    {   
        default:
            return false;
    }

    return true;
}

/**
 * @brief Matrix Commands
 * @param option
 */
bool MenuSystem::menuOptionsMatrixCommands(const MenuOption &option)
{
    switch(option.command_key[1])
    {
            // Logon
            // {Not Implimented yet!}
            //: When a CString is specified, PRELOGON.X, an
            //: USERLOG.X, and SYSPASS.X will be displayed.
            // { Note: add 0 for random! }
        case 'S':
            std::cout << "Executing startupModuleLogon()" << std::endl;
            startupModuleLogon();
            break;
            
            // Command Key: {T  {Research more how this is used!}
            // Function   : Sets FailFlag to false if user is able to get to
            //     : main system through knowing the system password
            //     : and having an account.
            //     : This would be used for stacking commands at the
            //     : matrix without actually logging onto to the
            //     : board.
        case 'T':
            return false;

            // Apply
        case 'A':
            std::cout << "Executing startupModuleSignup();" << std::endl;
            startupModuleSignup();
            return true;

            // Check
        case 'C':
            return false;

            // Feedback
        case 'F':
            return false;

            // Chat
        case 'P':
            return false;

            // Logoff
        case 'G':
            std::cout << "Goodbye;" << std::endl;
            m_session_data->logoff();
            break;
                       
            // Drops into the BBS
        case 'X':
            return false;
            
        default:
            return false;
    }

    return true;
}

/**
 * @brief Global New Scan Commands
 * @param option
 */
bool MenuSystem::menuOptionsGlobalNewScanCommands(const MenuOption &option)
{
    switch(option.command_key[1])
    {   
        default:
            return false;
    }

    return true;
}

/**
 * @brief Main Menu Commands
 * @param option
 */
bool MenuSystem::menuOptionsMainMenuCommands(const MenuOption &option)
{
    switch(option.command_key[1])
    {   
        default:
            return false;
    }

    return true;
}

/**
 * @brief Process Command Keys passed from menu selection (Callback)
 * @param option
 */
bool MenuSystem::menuOptionsCallback(const MenuOption &option)
{
    /* Run through the case and switch over the new interface.
    std::string mnuOption = option.CKeys;
    std::string mnuString = option.CString;
    std::string mnuAccess = option.Acs; */

    // If Invalid then return
    if(option.command_key.size() != 2)
    {
        return false;
    }
    
    
    // NOTE: to make even better, 
    // we can do a function array!


    // Run through Comamnd Keys for Method
    switch(option.command_key[0])
    {
        // "-" = Control Commands
        case '-':
            menuOptionsControlCommands(option);
            break;

        // & = MultiNode Commands
        case '&':
            menuOptionsMultiNodeCommands(option);
            break;
                        
        // { = Matrix Commands
        case '{':
            menuOptionsMatrixCommands(option);
            break;            
            
            // ! = Global Newscan Commands
        case '!':
            menuOptionsGlobalNewScanCommands(option);
            break;
            
            // [ = Main Menu Commands
        case '[':
            menuOptionsMainMenuCommands(option);
            break;

            // . = Door Commands
        case '.':
            break;

            // * = Sysop Commands
        case '*':
            switch(option.command_key[1])
            {
                    // Menu Editor
                case '#':
                    return false;

                    // Configuration Menu
                case 'C':
                    return false;
                    
                default:
                    return false;
            }
            break;
            
            // ^ = New User Voting Commands
        case '^':
            break;
        
            // C = Conference Editor    
        case 'C':
            break;
            
            // D = Data Area Menu
        case 'D':
            break;
        
            //  E = Email Commands
        case 'E':
            break;
            
            // F = File Commands
        case 'F':
            break;
            
            // J = Join Conference
        case 'J':
            break;
            
            // M = Message Commands
        case 'M':
            break;
            
            // Q = QWKMail Menu
        case 'Q':
            break;
            
            // R = Top Ten Listing
        case 'R':
            break;
        
            // S = Message Base Sponsor
        case 'S':
            break;
            
            // T = File Sponsor Commands
        case 'T':
            break;
            
            //  V = Voting Commands
        case 'V':
            break;
            
            // + = Color Setting Commands
        case '+':
            break;
        
    }
    
    return false;
}

/**
 * @brief Resets the Menu Input Method in the Function Array
 * @param index
 */
void MenuSystem::resetMenuInputIndex(int index)
{
    m_input_index = index;
}

/**
 * @brief Startup Menu Editor and Switch to MenuEditorInput
 */
void MenuSystem::startupMenuEditor()
{
    std::cout << "Entering MenuEditor Input " << std::endl;

    // Setup the input processor
    resetMenuInputIndex(MENU_EDITOR_INPUT);

    // 2. Handle any init and startup Screen Displays

    // WIP, nothing completed just yet for the startup.
}

/**
 * @brief Menu Editor, Read and Modify Menus
 */
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
 * @brief Clears All Modules
 */
void MenuSystem::clearAllModules()
{
    if (m_module.size() > 0)
    {
        std::vector<module_ptr>().swap(m_module);
    }
}

/**
 * @brief Exists and Shutsdown the current module
 */
void MenuSystem::shutdownModule()
{
    // Do module shutdown, only single modules are loaded
    // This makes it easy to allocate and kill on demand.
    m_module[0]->onExit();
    m_module.pop_back();
}

/**
 * @brief Exists and Shutsdown the current module
 */
void MenuSystem::startupModule(module_ptr module)
{
    // First clear any left overs if they exist.
    clearAllModules();

    // Run the startup for the module
    module->onEnter();

    // Push to stack now the new module.
    m_module.push_back(module);
}

/**
 * @brief Start up the Normal Login Process.
 */
void MenuSystem::startupModulePreLogon()
{
    // Setup the input processor
    resetMenuInputIndex(MODULE_PRELOGON_INPUT);

    // Allocate and Create
    module_ptr module(new ModPreLogon(m_session_data, m_config, m_ansi_process));
    if (!module)
    {
        std::cout << "ModPreLogon Allocation Error!" << std::endl;
        assert(false);
    }

    startupModule(module);
}

/**
 * @brief Start up the Normal Login Process.
 */
void MenuSystem::startupModuleLogon()
{
    // Setup the input processor
    resetMenuInputIndex(MODULE_LOGON_INPUT);

    // Allocate and Create
    module_ptr module(new ModLogon(m_session_data, m_config, m_ansi_process));
    if (!module)
    {
        std::cout << "ModLogon Allocation Error!" << std::endl;
        assert(false);
    }

    startupModule(module);
}

/**
 * @brief Starts up Signup Module
 */
void MenuSystem::startupModuleSignup()
{
    // Setup the input processor
    resetMenuInputIndex(MODULE_INPUT);

    // Allocate and Create
    module_ptr module(new ModSignup(m_session_data, m_config, m_ansi_process));
    if (!module)
    {
        std::cout << "ModSignup Allocation Error!" << std::endl;
        assert(false);
    }

    startupModule(module);
}

/**
 * @brief Handles Input for Login and PreLogin Sequences.
 * @param character_buffer
 * @param is_utf8
 */
void MenuSystem::handleLoginInputSystem(const std::string &character_buffer, const bool &is_utf8)
{
    // Make sure we have an allocated module before processing.
    if (m_module.size() == 0 || character_buffer.size() == 0)
    {
        return;
    }

    // Allocate and Create
    m_module[0]->update(character_buffer, is_utf8);

    // Finished modules processing.
    if (!m_module[0]->m_is_active)
    {
        shutdownModule();

        // Check if the current user has been logged in yet.
        if (!m_session_data->m_is_session_authorized)
        {            
            std::cout << "!m_is_session_authorized" << std::endl;
            m_current_menu = "matrix";
        }
        else
        {
            // If Authorized, then we want to move to main! Startup menu should be TOP or
            // Specified in Config file!  TODO
            std::cout << "m_is_session_authorized" << std::endl;
            m_current_menu = "main";
                     
        }
        
        loadAndStartupMenu();               
    }
}

/**
 * @brief Handles parsing input for preLogon module
 *
 */
void MenuSystem::modulePreLogonInput(const std::string &character_buffer, const bool &is_utf8)
{    
    std::cout << " *** modulePreLogonInput" << std::endl;    
    handleLoginInputSystem(character_buffer, is_utf8);
}

/**
 * @brief Handles parsing input for Logon module
 *
 */
void MenuSystem::moduleLogonInput(const std::string &character_buffer, const bool &is_utf8)
{   
    std::cout << " *** modulePreLogonInput" << std::endl;    
    handleLoginInputSystem(character_buffer, is_utf8);
}

/**
 * @brief Handles parsing input for modules
 *
 */
void MenuSystem::moduleInput(const std::string &character_buffer, const bool &is_utf8)
{
    std::cout << " *** moduleInput" << std::endl;
    
    // Make sure we have an allocated module before processing.
    if (m_module.size() == 0 || character_buffer.size() == 0)
    {
        return;
    }

    // Execute the modules update pass through input.
    m_module[0]->update(character_buffer, is_utf8);

    // Finished modules processing.
    if (!m_module[0]->m_is_active)
    {
        shutdownModule();

        // Reset the Input back to the Menu System
        resetMenuInputIndex(MENU_INPUT);

        // Redisplay,  may need to startup() again, but menu data should still be active and loaded!
        redisplayMenuScreen();
    }
}
