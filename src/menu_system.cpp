#include "menu_system.hpp"

#include "data-sys/config_dao.hpp"
#include "mods/mod_prelogon.hpp"
#include "mods/mod_logon.hpp"
#include "mods/mod_signup.hpp"
#include "mods/mod_menu_editor.hpp"
#include "mods/mod_user_editor.hpp"
#include "mods/mod_level_editor.hpp"
#include "mods/mod_message_editor.hpp"

#include <locale>
#include <string>
#include <vector>
#include <functional>
#include <cassert>

const std::string MenuSystem::m_menuID = "MENU_SYSTEM";

MenuSystem::MenuSystem(session_data_ptr session_data)
    : StateBase(session_data)
    , MenuBase(session_data)
{
    std::cout << "MenuSystem" << std::endl;

    // [Vector] Setup std::function array with available options to pass input to.
    m_menu_functions.push_back(std::bind(&MenuBase::menuInput, this, std::placeholders::_1, std::placeholders::_2));
    m_menu_functions.push_back(std::bind(&MenuBase::menuYesNoBarInput, this, std::placeholders::_1, std::placeholders::_2));
    m_menu_functions.push_back(std::bind(&MenuSystem::modulePreLogonInput, this, std::placeholders::_1, std::placeholders::_2));
    m_menu_functions.push_back(std::bind(&MenuSystem::moduleLogonInput, this, std::placeholders::_1, std::placeholders::_2));
    m_menu_functions.push_back(std::bind(&MenuSystem::moduleInput, this, std::placeholders::_1, std::placeholders::_2));

    // [Vector] Setup Menu Option Calls for executing menu commands.
    m_execute_callback.push_back(std::bind(&MenuSystem::menuOptionsCallback, this, std::placeholders::_1));

    // [Mapped] Setup Menu Command Functions
    m_menu_command_functions['-'] = std::bind(&MenuSystem::menuOptionsControlCommands, this, std::placeholders::_1);
    m_menu_command_functions['&'] = std::bind(&MenuSystem::menuOptionsMultiNodeCommands, this, std::placeholders::_1);
    m_menu_command_functions['{'] = std::bind(&MenuSystem::menuOptionsMatrixCommands, this, std::placeholders::_1);
    m_menu_command_functions['!'] = std::bind(&MenuSystem::menuOptionsGlobalNewScanCommands, this, std::placeholders::_1);
    m_menu_command_functions['['] = std::bind(&MenuSystem::menuOptionsMainMenuCommands, this, std::placeholders::_1);
    m_menu_command_functions['.'] = std::bind(&MenuSystem::menuOptionsDoorCommands, this, std::placeholders::_1);
    m_menu_command_functions['*'] = std::bind(&MenuSystem::menuOptionsSysopCommands, this, std::placeholders::_1);
    m_menu_command_functions['^'] = std::bind(&MenuSystem::menuOptionsNewUserVotingCommands, this, std::placeholders::_1);
    m_menu_command_functions['C'] = std::bind(&MenuSystem::menuOptionsConferenceEditorCommands, this, std::placeholders::_1);
    m_menu_command_functions['D'] = std::bind(&MenuSystem::menuOptionsDataAreaCommands, this, std::placeholders::_1);
    m_menu_command_functions['E'] = std::bind(&MenuSystem::menuOptionsEmailCommands, this, std::placeholders::_1);
    m_menu_command_functions['F'] = std::bind(&MenuSystem::menuOptionsFileCommands, this, std::placeholders::_1);
    m_menu_command_functions['J'] = std::bind(&MenuSystem::menuOptionsJoinConference, this, std::placeholders::_1);
    m_menu_command_functions['M'] = std::bind(&MenuSystem::menuOptionsMessageCommands, this, std::placeholders::_1);
    m_menu_command_functions['Q'] = std::bind(&MenuSystem::menuOptionsQWKMailCommands, this, std::placeholders::_1);
    m_menu_command_functions['R'] = std::bind(&MenuSystem::menuOptionsTopTenListingCommands, this, std::placeholders::_1);
    m_menu_command_functions['S'] = std::bind(&MenuSystem::menuOptionsMessageBaseSponsorCommands, this, std::placeholders::_1);
    m_menu_command_functions['T'] = std::bind(&MenuSystem::menuOptionsFileBaseSponsorCommands, this, std::placeholders::_1);
    m_menu_command_functions['V'] = std::bind(&MenuSystem::menuOptionsVotingCommands, this, std::placeholders::_1);
    m_menu_command_functions['+'] = std::bind(&MenuSystem::menuOptionsColorSettingCommands, this, std::placeholders::_1);

    // Load the configuration file into m_config
    ConfigDao config_dao(m_config, GLOBAL_BBS_PATH);

    if(!config_dao.loadConfig())
    {
        std::cout << "Error: unable to load configuration file" << std::endl;
        assert(false);
    }
}

MenuSystem::~MenuSystem()
{
    std::cout << "~MenuSystem" << std::endl;

    // Clear All Menu Command Functions.
    MappedCommandFunctions().swap(m_menu_command_functions);
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
 * @brief Control Commands
 * @param option
 */
bool MenuSystem::menuOptionsControlCommands(const MenuOption &option)
{
    // Some of these options set actual flags for behavior.
    // In this case, we will need to parse for specific Control commands
    // and set Menu System Flags!

    // Create system default locale
    std::locale::global(std::locale(""));
    std::cout.imbue(std::locale());

    switch(option.command_key[1])
    {
        // Turns on Pulldown Menu Re-entrance
        // This option returns to the selected option
        // when the user re-enters the pulldown menu.
        // This works ONLY if the command that the user
        // executed does not go to another menu.

        // And some might be stacked ..
        // Reference Legacy and figure out behavior.

        case '\'':
            return false;

        // Turns off Pulldown Menu Re-Entrance
        case '`':
            return false;

        // Writes CString into the Header
        case 'A':
            return false;

        // Sets the Bad Prompt to CString
        // When invalid command is executed.
        case 'B':
            return false;

        // Clears Fail Flag
        case 'C':
            return false;

        // Cstring to modem (not used)
        case 'D':
            return false;

        // Write characters with ^ char support (display string)
        case 'E':
            return false;

        // Display ANSI File
        case 'F':
            return false;

        // Goto Speicific X/Y Position on screen
        case 'G':
            return false;

        // Gets String with hidden input
        case 'H':
            return false;

        // Gets String with normal input
        case 'I':
            return false;

        // Check Input H or I matches C String, sets fail flag.
        case 'M':
            return false;

        // Display Stat Screen From Library
        case 'N':
            return false;

        // Sets fail flag if sysop not available, otherwise true
        case 'J':
            return false;

        // Change Starting Menu, fallback using -^ change to old starting menu
        case 'K':
            return false;

        // Clears Screen
        case 'L':
            return false;

        // Display files from current stat screen library
        case 'O':
            return false;

        // Pauses X number of seconds
        case 'P':
            return false;

        // Sets current menu prompt to # in cstring
        case 'Q':
            return false;

        // Prints a CR [RETURN]
        case 'R':
            return false;

        // Changes user flags ACS <+,-> <Flag Letter>
        case 'S':
            return false;

        // Sets Status Library to # in cstring
        case 'T':
            return false;

        // Sets Menu Libraray to # in cstring
        case 'V':
            return false;

        //Function   : Sets the Starting Menu to cstring and sets the
        //           : fallback of all the : menus called with the -^
        //           : command to the menu name in the cstring.
        case 'U':
            return false;

        // Write cstring to log
        case 'W':
            return false;

        // Return fail flag if file specified doesn't exist.
        case 'X':
            return false;

        // Changes users flags for second set of ASC flags.
        case 'Y':
            return false;

        // Pauses other commands untill output buffer is completed
        // Might need to research this one more.
        case 'Z':
            return false;

        // Display Random Welcome.x ANSI screen
        case '+':
            return false;

        // Check mandatory infoforms, if not filled out will force user to complete
        case '|':
            return false;

        // Read system notices
        case '@':
            return false;

        // pages system if available flag.
        case '_':
            return false;

        // TODO main menu switching commands first.

        // goto menu sets fallback current
        case '/':
            if(m_current_menu.size() > 0)
            {
                m_system_fallback.push_back(m_current_menu);
            }

            m_current_menu = lower_case(option.command_string);
            loadAndStartupMenu();
            m_use_first_command_execution = true;
            break;

        // goes to fallback menu, sets fallback to previous fallback
        case '\\':
            if(m_system_fallback.size() > 0)
            {
                m_current_menu = m_system_fallback.back();
                std::cout << "FallBack reset to current: " << m_current_menu << std::endl;
                m_system_fallback.pop_back();
            }
            else
            {
                std::cout << "FallBack reset to menu_fall_back: " << m_menu_info->menu_fall_back << std::endl;
                m_current_menu = m_menu_info->menu_fall_back;
            }

            loadAndStartupMenu();
            m_use_first_command_execution = true;
            break;

        // Goes to menu, sets fallback as starting menu
        case '^':
            if(m_starting_menu.size() == 0)
            {
                m_starting_menu = m_current_menu;
            }

            std::cout << "Set Fallback Starting Menu: " << m_starting_menu << std::endl;
            m_system_fallback.push_back(m_starting_menu);
            m_current_menu = lower_case(option.command_string);
            loadAndStartupMenu();
            m_use_first_command_execution = true;
            break;

        // END

        // Sets input variable with -I or -J to cstring
        case '*':
            return false;

        // Sets user level to one specified
        case '=':
            return false;

        // Door (external) Error level (input) return into input variable
        // Like external mod doing password or
        case '&':
            return false;

        // Hit Enter Prompt
        case '(':
            return false;

        // Changes to infoform set in cstring
        case ')':
            return false;

        // Goes to the menu specified in the CString, does not exe firstcmd
        case '{':
            if(m_starting_menu.size() == 0)
            {
                m_starting_menu = m_current_menu;
            }

            m_system_fallback.push_back(m_starting_menu);
            m_current_menu = lower_case(option.command_string);
            m_use_first_command_execution = false;
            loadAndStartupMenu();
            // TODO Add Flags to not run firstcmd!
            break;

        // Drops to Previous Menu, does not exe firstcmd
        case '}':
            m_current_menu = m_previous_menu;
            m_use_first_command_execution = false;
            loadAndStartupMenu();
            // TODO Add Flags to not run firstcmd!
            break;

        // Toggles locking of output to the modem.
        case ':':
            return false;

        // Toggles locking of input from the modem.
        case ';':
            return false;

        // Goes to a menu keeping the current fallback menu does firstcmd
        case '$':
            return false;

        // Goes to a menu keeping the current fallback menu doesn't firstcmd
        case '%':
            return false;

        // Displays and gets input in same format as prompt
        case '-':
            return false;

        // Sets Time left to value found in CString.
        case '#':
            return false;

        // Displays current menu prompt using CString name in prompt
        case '!':
            return false;

        // Sends the file specified in the CString.
        case '.':
            return false;

        //  Displays Prompt String
        case '<':
            return false;

        // Sets Chat Reason in Status Bar to Value in cstring
        case '~':
            return false;

        // Sets the number of lines scrolled to 0. Stop screen pausing.
        case '"':
            return false;

        // Sets Screen Pausing (variable toggled in config)
        case '1':
            return false;

        // Suspends Screen Pausing until next textfile
        case '2':
            return false;

        // Sets starting Option in a pulldown Menu PullDown ID
        case ',':
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
        {
            // Testing processes

            startupModuleMessageEditor();
            return true;
            /*
            #ifdef _WIN32
            std::string cmdline = "C:\\windows\\system32\\cmd.exe";
            #else
            std::string cmdline = "/bin/bash";
            #endif
            startupExternalProcess(cmdline);
            return true;
            */
            //return false;
        }

        // Feedback
        case 'F':
            return false;

        // Chat
        case 'P':
            return false;

        // Logoff
        case 'G':
            std::cout << "Goodbye;" << std::endl;
            // Base Class
            m_logoff = true;
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
        // autosig
        case 'A':
            return false;

        // page sysop
        case 'C':
            return false;

        // infoform
        case 'D':
            return false;

        // Fill out info form to file
        case 'F':
            return false;

        // Logsoff
        case 'G':
            std::cout << "Goodbye;" << std::endl;
            // Add Logoff ANSI Display here.
            // Base Class
            m_logoff = true;
            m_session_data->logoff();
            break;

        // logoff without ansi
        case 'H':
            std::cout << "Goodbye;" << std::endl;
            // Base Class
            m_logoff = true;
            m_session_data->logoff();
            break;

        // Fill out info form
        case 'I':
            return false;

        // User Configuration
        case 'K':
            return false;

        // Lists Users
        case 'L':
            return false;

        // System Stats
        case 'S':
            return false;

        // Time Bank
        case 'U':
            return false;

        // View Daily Log
        case 'V':
            return false;

        // Last Callers
        case 'W':
            return false;

        // Transfer to user
        case 'X':
            return false;

        // user stats
        case 'Y':
            return false;

        // change password
        case '+':
            return false;

        // force use to change password
        case '-':
            return false;

        default:
            return false;
    }

    return true;
}

/**
 * @brief Door Commands
 * @param option
 */
bool MenuSystem::menuOptionsDoorCommands(const MenuOption &option)
{
    switch(option.command_key[1])
    {
        default:
            return false;
    }

    return true;
}

/**
 * @brief Sysop Commands
 * @param option
 */
bool MenuSystem::menuOptionsSysopCommands(const MenuOption &option)
{
    switch(option.command_key[1])
    {
        case '#':  // Menu Editor
            std::cout << "Executing startupModuleMenuEditor();" << std::endl;
            startupModuleMenuEditor();
            break;

        case 'U': // User Editor
            std::cout << "Executing startupModuleUserEditor();" << std::endl;
            startupModuleUserEditor();
            break;

        case 'Y': // Level Editor
            std::cout << "Executing startupModuleLevelEditor();" << std::endl;
            startupModuleLevelEditor();
            break;

        // Configuration Menu
        case 'C':
            break;

        default:
            return false;
    }

    return true;
}

/**
 * @brief New User Voting Commands
 * @param option
 */
bool MenuSystem::menuOptionsNewUserVotingCommands(const MenuOption &option)
{
    switch(option.command_key[1])
    {
        default:
            return false;
    }

    return true;
}

/**
 * @brief Conference Editor Commands
 * @param option
 */
bool MenuSystem::menuOptionsConferenceEditorCommands(const MenuOption &option)
{
    switch(option.command_key[1])
    {
        default:
            return false;
    }

    return true;
}

/**
 * @brief Data Area Commands
 * @param option
 */
bool MenuSystem::menuOptionsDataAreaCommands(const MenuOption &option)
{
    switch(option.command_key[1])
    {
        default:
            return false;
    }

    return true;
}

/**
 * @brief Email Commands
 * @param option
 */
bool MenuSystem::menuOptionsEmailCommands(const MenuOption &option)
{
    switch(option.command_key[1])
    {
        default:
            return false;
    }

    return true;
}

/**
 * @brief File Commands
 * @param option
 */
bool MenuSystem::menuOptionsFileCommands(const MenuOption &option)
{
    switch(option.command_key[1])
    {
        default:
            return false;
    }

    return true;
}

/**
 * @brief Message Commands
 * @param option
 */
bool MenuSystem::menuOptionsMessageCommands(const MenuOption &option)
{
    switch(option.command_key[1])
    {
        default:
            return false;
    }

    return true;
}

/**
 * @brief Join Conference Commands
 * @param option
 */
bool MenuSystem::menuOptionsJoinConference(const MenuOption &option)
{
    switch(option.command_key[1])
    {
        default:
            return false;
    }

    return true;
}

/**
 * @brief QWK Mail Commands
 * @param option
 */
bool MenuSystem::menuOptionsQWKMailCommands(const MenuOption &option)
{
    switch(option.command_key[1])
    {
        default:
            return false;
    }

    return true;
}

/**
 * @brief Top 10 Listing Commands
 * @param option
 */
bool MenuSystem::menuOptionsTopTenListingCommands(const MenuOption &option)
{
    switch(option.command_key[1])
    {
        default:
            return false;
    }

    return true;
}

/**
 * @brief Message Base Sponsor Commands
 * @param option
 */
bool MenuSystem::menuOptionsMessageBaseSponsorCommands(const MenuOption &option)
{
    switch(option.command_key[1])
    {
        default:
            return false;
    }

    return true;
}

/**
 * @brief File Base Sponsor Commands
 * @param option
 */
bool MenuSystem::menuOptionsFileBaseSponsorCommands(const MenuOption &option)
{
    switch(option.command_key[1])
    {
        default:
            return false;
    }

    return true;
}

/**
 * @brief Voting Commands
 * @param option
 */
bool MenuSystem::menuOptionsVotingCommands(const MenuOption &option)
{
    switch(option.command_key[1])
    {
        default:
            return false;
    }

    return true;
}

/**
 * @brief Color Setting Commands
 * @param option
 */
bool MenuSystem::menuOptionsColorSettingCommands(const MenuOption &option)
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

    std::string firstCommandKeyIndex = "-&{![.*^CDEFJMQRSTV+";
    std::string::size_type idx = 0;

    // If valid then execute the related Menu Command Function
    idx = firstCommandKeyIndex.find(option.command_key[0], 0);

    if(idx != std::string::npos)
    {
        return m_menu_command_functions[option.command_key[0]](option);
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
 * @brief Startup External (Door / Script Process)
 * @param cmdline
 */
void MenuSystem::startupExternalProcess(const std::string &cmdline)
{
    std::cout << "MenuSystem Starting Process" << std::endl;
    m_menu_session_data->startExternalProcess(cmdline);
}

/**
 * @brief Clears All Modules
 */
void MenuSystem::clearAllModules()
{
    if(m_module_stack.size() > 0)
    {
        std::vector<module_ptr>().swap(m_module_stack);
    }
}

/**
 * @brief Exists and Shutsdown the current module
 */
void MenuSystem::shutdownModule()
{
    // Do module shutdown, only single modules are loaded
    // This makes it easy to allocate and kill on demand.
    m_module_stack.back()->onExit();
    m_module_stack.pop_back();
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
    m_module_stack.push_back(module);
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

    if(!module)
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

    if(!module)
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

    if(!module)
    {
        std::cout << "ModSignup Allocation Error!" << std::endl;
        assert(false);
    }

    startupModule(module);
}

/**
 * @brief Startup the Menu Editor Module
 */
void MenuSystem::startupModuleMenuEditor()
{
    // Setup the input processor
    resetMenuInputIndex(MODULE_INPUT);

    // Allocate and Create
    module_ptr module(new ModMenuEditor(m_session_data, m_config, m_ansi_process));

    if(!module)
    {
        std::cout << "ModMenuEditor Allocation Error!" << std::endl;
        assert(false);
    }

    startupModule(module);
}

/**
 * @brief Startup the User Editor Module
 */
void MenuSystem::startupModuleUserEditor()
{
    // Setup the input processor
    resetMenuInputIndex(MODULE_INPUT);

    // Allocate and Create
    module_ptr module(new ModUserEditor(m_session_data, m_config, m_ansi_process));

    if(!module)
    {
        std::cout << "ModUserEditor Allocation Error!" << std::endl;
        assert(false);
    }

    startupModule(module);
}

/**
 * @brief Startup the Level Editor Module
 */
void MenuSystem::startupModuleLevelEditor()
{
    // Setup the input processor
    resetMenuInputIndex(MODULE_INPUT);

    // Allocate and Create
    module_ptr module(new ModLevelEditor(m_session_data, m_config, m_ansi_process));

    if(!module)
    {
        std::cout << "ModLevelEditor Allocation Error!" << std::endl;
        assert(false);
    }

    startupModule(module);
}

/**
 * @brief Startup the Full Screen Message Editor Module
 */
void MenuSystem::startupModuleMessageEditor()
{
    // Setup the input processor
    resetMenuInputIndex(MODULE_INPUT);

    // Allocate and Create
    module_ptr module(new ModMessageEditor(m_session_data, m_config, m_ansi_process));

    if(!module)
    {
        std::cout << "ModMessageEditor Allocation Error!" << std::endl;
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
    if(m_module_stack.size() == 0 || character_buffer.size() == 0)
    {
        return;
    }

    // Allocate and Create
    m_module_stack.back()->update(character_buffer, is_utf8);

    // Finished modules processing.
    if(!m_module_stack.back()->m_is_active)
    {
        shutdownModule();

        // Check if the current user has been logged in yet.
        if(!m_session_data->m_is_session_authorized)
        {
            std::cout << " *** !m_is_session_authorized" << std::endl;
            m_current_menu = "matrix";
        }
        else
        {
            // If Authorized, then we want to move to main! Startup menu should be TOP or
            // Specified in Config file!  TODO
            std::cout << " *** m_is_session_authorized: " << m_config->starting_menu_name << std::endl;

            if(m_config->starting_menu_name.size() > 0)
            {
                m_current_menu = m_config->starting_menu_name;
                m_starting_menu = m_config->starting_menu_name;
            }
            else
            {
                // Default to main if nothing is set in config file.
                m_current_menu = "main";
                m_starting_menu = "main";
            }
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
    if(m_module_stack.size() == 0 || character_buffer.size() == 0)
    {
        return;
    }

    // Execute the modules update pass through input.
    m_module_stack.back()->update(character_buffer, is_utf8);

    // Finished modules processing.
    if(!m_module_stack.back()->m_is_active)
    {
        shutdownModule();

        // Reset the Input back to the Menu System
        resetMenuInputIndex(MENU_INPUT);

        // Redisplay,  may need to startup() again, but menu data should still be active and loaded!
        redisplayMenuScreen();
    }
}
