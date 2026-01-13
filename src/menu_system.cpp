#include "menu_system.hpp"

#include <locale>
#include <cassert>
#include <cstdint>
#include <string>
#include <vector>
#include <map>
#include <functional>

#include "model-sys/config.hpp"

/*
#include "mods/mod_logon.hpp"
#include "mods/mod_signup.hpp"
#include "mods/mod_menu_editor.hpp"
#include "mods/mod_user_editor.hpp"
#include "mods/mod_level_editor.hpp"
#include "mods/mod_message_editor.hpp"
*/

#include "mods/mod_logon.hpp"

#include "model-sys/context.hpp"
#include "tcp_session.hpp"
#include "logging.hpp"

MenuSystem::MenuSystem(Context &ctx)
    : MenuBase(ctx)
      , m_log(Logging::getInstance())
      , currentState(State::MenuSystem) {

    // Setup Menu Option Calls for executing menu commands.
    m_execute_callback.emplace_back(bind_member(this, &MenuSystem::menuOptionsCallback));

    // Menu Input Commands (Base Class) - goes directly to base class.
    m_menu_functions.emplace_back(bind_member(static_cast<MenuBase*>(this), &MenuBase::menuInput));
    m_menu_functions.emplace_back(bind_member(static_cast<MenuBase*>(this), &MenuBase::menuYesNoBarInput));

    // Menu Input Commands (System Class)
    m_menu_functions.emplace_back(bind_member(this, &MenuSystem::moduleLogonInput));
    m_menu_functions.emplace_back(bind_member(this, &MenuSystem::moduleInput));

    // [Mapped] Menu Command Key Functions
    m_menu_command_functions['-'] = bind_member(this, &MenuSystem::menuOptionsControlCommands);
    m_menu_command_functions['&'] = bind_member(this, &MenuSystem::menuOptionsMultiNodeCommands);
    m_menu_command_functions['{'] = bind_member(this, &MenuSystem::menuOptionsMatrixCommands);
    m_menu_command_functions['!'] = bind_member(this, &MenuSystem::menuOptionsGlobalNewScanCommands);
    m_menu_command_functions['['] = bind_member(this, &MenuSystem::menuOptionsMainMenuCommands);
    m_menu_command_functions['.'] = bind_member(this, &MenuSystem::menuOptionsDoorCommands);
    m_menu_command_functions['*'] = bind_member(this, &MenuSystem::menuOptionsSysopCommands);
    m_menu_command_functions['^'] = bind_member(this, &MenuSystem::menuOptionsNewUserVotingCommands);
    m_menu_command_functions['C'] = bind_member(this, &MenuSystem::menuOptionsConferenceEditorCommands);
    m_menu_command_functions['D'] = bind_member(this, &MenuSystem::menuOptionsDataAreaCommands);
    m_menu_command_functions['E'] = bind_member(this, &MenuSystem::menuOptionsEmailCommands);
    m_menu_command_functions['F'] = bind_member(this, &MenuSystem::menuOptionsFileCommands);
    m_menu_command_functions['J'] = bind_member(this, &MenuSystem::menuOptionsJoinConference);
    m_menu_command_functions['M'] = bind_member(this, &MenuSystem::menuOptionsMessageCommands);
    m_menu_command_functions['Q'] = bind_member(this, &MenuSystem::menuOptionsQWKMailCommands);
    m_menu_command_functions['R'] = bind_member(this, &MenuSystem::menuOptionsTopTenListingCommands);
    m_menu_command_functions['S'] = bind_member(this, &MenuSystem::menuOptionsMessageBaseSponsorCommands);
    m_menu_command_functions['T'] = bind_member(this, &MenuSystem::menuOptionsFileBaseSponsorCommands);
    m_menu_command_functions['V'] = bind_member(this, &MenuSystem::menuOptionsVotingCommands);
    m_menu_command_functions['+'] = bind_member(this, &MenuSystem::menuOptionsColorSettingCommands);
}

MenuSystem::~MenuSystem() {
    m_log.log(Logging::LogLevel::Debug, "~MenuSystem()");

    // Clear All Menu Command Functions.
    m_menu_command_functions.clear();
    MappedCommandFunctions().swap(m_menu_command_functions);
}

/**
 * @brief Handles Updates or Data Input from Client
 */
void MenuSystem::update(const std::string &character_buffer, const bool &is_utf8) {
    if (!m_is_active) {
        return;
    }

    // This simply passed through the input to the current system function were at.
    m_menu_functions[m_input_index](character_buffer, is_utf8);
}

/**
 * @brief Startup class, setup initial screens / interface, flags etc..
 *        This is only called when switch to the state, not for menu instances.
 * @return
 */
bool MenuSystem::onEnter() {
    m_is_active = true;
    return true;
}

/**
 * @brief Exit, close down, display screens to change over data.
 * @return
 */
bool MenuSystem::onExit() {
    m_is_active = false;
    return true;
}

bool MenuSystem::pollTimers() {
    return true;
}

// For Modules
void MenuSystem::bindStateHandlers() {
    clearHandlers.clear();
    createHandlers.clear();
    inputHandlers.clear();
    pollHandlers.clear();

    // Menu System
    clearHandlers.emplace(State::MenuSystem,
                          [this]() { clearMenuSystem(); });

    createHandlers.emplace(State::MenuSystem,
                           [this]() { createMenuSystem(); });

    pollHandlers.emplace(State::MenuSystem,
                         [this]() { pollMenuSystem(); });

    inputHandlers.emplace(State::MenuSystem,
                          [this](const std::string &input) {
                              inputMenuSystem(input);
                          });

    // Logon
    clearHandlers.emplace(State::ModLogon,
                          [this]() { clearPreLogon(); });

    createHandlers.emplace(State::ModLogon,
                           [this]() { createPreLogon(); });

    pollHandlers.emplace(State::ModLogon,
                         [this]() { pollPreLogon(); });

    inputHandlers.emplace(State::ModLogon,
                          [this](const std::string &input) {
                              inputPreLogon(input);
                          });

    // Runtime guarantee (debug)
    assert(clearHandlers.size() == StateCount);
    assert(createHandlers.size() == StateCount);
    assert(pollHandlers.size() == StateCount);
    assert(inputHandlers.size() == StateCount);
}

void MenuSystem::setState(State newState) {
    clearHandlers.at(currentState)();
    currentState = newState;
    createHandlers.at(currentState)();
}

/**
 * @brief Control Commands
 * @param option
 */
bool MenuSystem::menuOptionsControlCommands(const MenuOption &option) {
    // Some of these options set actual flags for behavior.
    // In this case, we will need to parse for specific Control commands
    // and set Menu System Flags!
    switch (option.command_key[1]) {
        // Turns on Pull down Menu Re-entrance
        // This option returns to the selected option
        // when the user re-enters the pull down menu.
        // This works ONLY if the command that the user
        // executed does not go to another menu.

        // And some might be stacked ..
        // Reference Legacy and figure out behavior.

        case '\'':
            return false;

        // Turns off Pull down Menu Re-Entrance
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

        // Goto Specific X/Y Position on screen
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

        // Sets Menu Library to # in cstring
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

        // Pauses other commands until output buffer is completed
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
            if (!m_current_menu.empty()) {
                m_system_fallback.push_back(m_current_menu);
            }

            m_current_menu = lower_case(option.command_string);
            loadAndStartupMenu();
            m_use_first_command_execution = true;
            break;

        // goes to fallback menu, sets fallback to previous fallback
        case '\\':
            if (!m_system_fallback.empty()) {
                m_current_menu = m_system_fallback.back();
                m_log.log(Logging::LogLevel::Debug, "FallBack reset to current=", m_current_menu);

                m_system_fallback.pop_back();
            } else {
                m_log.log(Logging::LogLevel::Debug, "FallBack reset to menu_fall_back=", m_menu_info.menu_fall_back);
                m_current_menu = m_menu_info.menu_fall_back;
            }

            loadAndStartupMenu();
            m_use_first_command_execution = true;
            break;

        // Goes to menu, sets fallback as starting menu
        case '^':
            if (m_starting_menu.empty()) {
                m_starting_menu = m_current_menu;
            }

            m_log.log(Logging::LogLevel::Debug, "Set Fallback Starting Menu=", m_starting_menu);
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
            if (m_starting_menu.empty()) {
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
bool MenuSystem::menuOptionsMultiNodeCommands(const MenuOption &option) {
    switch (option.command_key[1]) {
        default:
            return false;
    }

    return true;
}

/**
 * @brief Matrix Commands
 * @param option
 */
bool MenuSystem::menuOptionsMatrixCommands(const MenuOption &option) {
    switch (option.command_key[1]) {
        // Logon
        // {Not Implemented yet!}
        //: When a CString is specified, PRELOGON.X, an
        //: USERLOG.X, and SYSPASS.X will be displayed.
        // { Note: add 0 for random! }
        case 'S':
            m_log.log(Logging::LogLevel::Debug, "Executing startupModuleLogon()");
            //startupModuleLogon();
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
            m_log.log(Logging::LogLevel::Debug, "Executing startupModuleSignup()");
            //startupModuleSignup();
            return true;

        // Check
        case 'C':
            return false;

        // Check
        case 'E': {
            // Testing processes
            m_log.log(Logging::LogLevel::Debug, "Executing startupModuleMessageEditor()");
            //startupModuleMessageEditor();
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
            m_log.log(Logging::LogLevel::Console, "User Logoff()");
            // Base Class
            m_logoff = true;
            m_ctx.getSessionWrite().hangup();
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
bool MenuSystem::menuOptionsGlobalNewScanCommands(const MenuOption &option) {
    switch (option.command_key[1]) {
        default:
            return false;
    }

    return true;
}

/**
 * @brief Disconnect a user on the Session.
 */
void MenuSystem::disconnectUser() {
    m_ctx.getSessionWrite().hangup();
}

/**
 * @brief Main Menu Commands
 * @param option
 */
bool MenuSystem::menuOptionsMainMenuCommands(const MenuOption &option) {
    switch (option.command_key[1]) {
        // autosig
        case 'A':
            return false;

        // page sysop
        case 'C':
            return false;

        // info form
        case 'D':
            return false;

        // Fill out info form to file
        case 'F':
            return false;

        // Logoff
        case 'G':
            m_log.log(Logging::LogLevel::Debug, "Logoff()");
            // Add Logoff ANSI Display here.
            // Base Class
            m_logoff = true;
            disconnectUser();
            break;

        // logoff without ansi
        case 'H':
            m_log.log(Logging::LogLevel::Debug, "Logoff() Without ANSI");
            // Base Class
            m_logoff = true;
            disconnectUser();
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
bool MenuSystem::menuOptionsDoorCommands(const MenuOption &option) {
    switch (option.command_key[1]) {
        default:
            return false;
    }

    return true;
}

/**
 * @brief Sysop Commands
 * @param option
 */
bool MenuSystem::menuOptionsSysopCommands(const MenuOption &option) {
    switch (option.command_key[1]) {
        case '#': // Menu Editor
            m_log.log(Logging::LogLevel::Debug, "Executing startupModuleMenuEditor()");
            //startupModuleMenuEditor();
            break;

        case 'U': // User Editor
            m_log.log(Logging::LogLevel::Debug, "Executing startupModuleUserEditor()");
            //startupModuleUserEditor();
            break;

        case 'Y': // Level Editor
            m_log.log(Logging::LogLevel::Debug, "Executing startupModuleLevelEditor()");
            //startupModuleLevelEditor();
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
bool MenuSystem::menuOptionsNewUserVotingCommands(const MenuOption &option) {
    switch (option.command_key[1]) {
        default:
            return false;
    }

    return true;
}

/**
 * @brief Conference Editor Commands
 * @param option
 */
bool MenuSystem::menuOptionsConferenceEditorCommands(const MenuOption &option) {
    switch (option.command_key[1]) {
        default:
            return false;
    }

    return true;
}

/**
 * @brief Data Area Commands
 * @param option
 */
bool MenuSystem::menuOptionsDataAreaCommands(const MenuOption &option) {
    switch (option.command_key[1]) {
        default:
            return false;
    }

    return true;
}

/**
 * @brief Email Commands
 * @param option
 */
bool MenuSystem::menuOptionsEmailCommands(const MenuOption &option) {
    switch (option.command_key[1]) {
        default:
            return false;
    }

    return true;
}

/**
 * @brief File Commands
 * @param option
 */
bool MenuSystem::menuOptionsFileCommands(const MenuOption &option) {
    switch (option.command_key[1]) {
        default:
            return false;
    }

    return true;
}

/**
 * @brief Message Commands
 * @param option
 */
bool MenuSystem::menuOptionsMessageCommands(const MenuOption &option) {
    switch (option.command_key[1]) {
        default:
            return false;
    }

    return true;
}

/**
 * @brief Join Conference Commands
 * @param option
 */
bool MenuSystem::menuOptionsJoinConference(const MenuOption &option) {
    switch (option.command_key[1]) {
        default:
            return false;
    }

    return true;
}

/**
 * @brief QWK Mail Commands
 * @param option
 */
bool MenuSystem::menuOptionsQWKMailCommands(const MenuOption &option) {
    switch (option.command_key[1]) {
        default:
            return false;
    }

    return true;
}

/**
 * @brief Top 10 Listing Commands
 * @param option
 */
bool MenuSystem::menuOptionsTopTenListingCommands(const MenuOption &option) {
    switch (option.command_key[1]) {
        default:
            return false;
    }

    return true;
}

/**
 * @brief Message Base Sponsor Commands
 * @param option
 */
bool MenuSystem::menuOptionsMessageBaseSponsorCommands(const MenuOption &option) {
    switch (option.command_key[1]) {
        default:
            return false;
    }

    return true;
}

/**
 * @brief File Base Sponsor Commands
 * @param option
 */
bool MenuSystem::menuOptionsFileBaseSponsorCommands(const MenuOption &option) {
    switch (option.command_key[1]) {
        default:
            return false;
    }

    return true;
}

/**
 * @brief Voting Commands
 * @param option
 */
bool MenuSystem::menuOptionsVotingCommands(const MenuOption &option) {
    switch (option.command_key[1]) {
        default:
            return false;
    }

    return true;
}

/**
 * @brief Color Setting Commands
 * @param option
 */
bool MenuSystem::menuOptionsColorSettingCommands(const MenuOption &option) {
    switch (option.command_key[1]) {
        default:
            return false;
    }

    return true;
}

/**
 * @brief Process Command Keys passed from menu selection (Callback)
 * @param option
 */
bool MenuSystem::menuOptionsCallback(const MenuOption &option) {
    /* Run through the case and switch over the new interface.
    std::string mnuOption = option.CKeys;
    std::string mnuString = option.CString;
    std::string mnuAccess = option.Acs; */

    // If Invalid then return
    if (option.command_key.size() != 2) {
        return false;
    }

    std::string firstCommandKeyIndex = "-&{![.*^CDEFJMQRSTV+";
    std::string::size_type idx = 0;

    // If valid then execute the related Menu Command Function
    idx = firstCommandKeyIndex.find(option.command_key[0], 0);

    if (idx != std::string::npos) {
        return m_menu_command_functions[option.command_key[0]](option);
    }

    return false;
}

/**
 * @brief Resets the Menu Input Method in the Function Array
 * @param index
 */
void MenuSystem::resetMenuInputIndex(int index) {
    m_input_index = index;
}

/**
 * @brief Startup External (Door / Script Process)
 * @param cmdline
 */
void MenuSystem::startupExternalProcess(const std::string &cmdline) {
    m_log.log(Logging::LogLevel::Console, "Executing startExternalProcess()=", cmdline);
    //m_menu_session_data->startExternalProcess(cmdline);
}

/**
 * @brief Clears All Modules
 */
void MenuSystem::clearAllModules() {
    m_log.log(Logging::LogLevel::Debug, "Menu System: clearAllModules()");
    //if(m_module_stack.size() > 0)
    {
        //std::vector<module_ptr>().swap(m_module_stack);
    }
}

/**
 * @brief Exists and Shuts down the current module
 */
void MenuSystem::shutdownModule() {
    // Do module shutdown, only single modules are loaded
    // This makes it easy to allocate and kill on demand.
    //m_log.log(Logging::LogLevel::Console, "shutdownModule in MenuSystem() Module=", m_module_stack.back()->m_filename);
    //m_module_stack.back()->onExit();
    //m_module_stack.pop_back();
}

/**
 * @brief Start up the Normal Login Process.
 *
void MenuSystem::startupModulePreLogon()
{
    // Setup the input processor
    resetMenuInputIndex(MODULE_PRE_LOGON_INPUT);

    // Allocate and Create
    module_ptr module = std::make_shared<ModPreLogon>(
        getLockedSession(), m_config, m_ansi_process, m_common_io, m_session_io
    );

    if(!module)
    {
        m_log.log(Logging::LogLevel::Error, "startupModulePreLogon Allocation Error");
        return;
    }

    startupModule(module);
}*/

/**
 * @brief Start up the Normal Login Process.
 */
void MenuSystem::startupModuleLogon()
{
    // Setup the input processor
    resetMenuInputIndex(MODULE_LOGON_INPUT);

    // Allocate and Create
    module_ptr module = std::make_shared<ModLogon>(
        getLockedSession(), m_config, m_ansi_process, m_common_io, m_session_io
    );

    if(!module)
    {
        m_log.log(Logging::LogLevel::Error, "startupModuleLogon Allocation Error");
        return;
    }

    startupModule(module);
}

/**
 * @brief Starts up Signup Module
 *
void MenuSystem::startupModuleSignup()
{
    // Setup the input processor
    resetMenuInputIndex(MODULE_INPUT);

    // Allocate and Create
    module_ptr module = std::make_shared<ModSignup>(
        getLockedSession(), m_config, m_ansi_process, m_common_io, m_session_io
    );

    if(!module)
    {
        m_log.log(Logging::LogLevel::Error, "startupModuleSignup Allocation Error");
        return;
    }

    startupModule(module);
}*/

/**
 * @brief Startup the Menu Editor Module
 *
void MenuSystem::startupModuleMenuEditor()
{
    // Setup the input processor
    resetMenuInputIndex(MODULE_INPUT);

    // Allocate and Create
    module_ptr module = std::make_shared<ModMenuEditor>(
        getLockedSession(), m_config, m_ansi_process, m_common_io, m_session_io
    );

    if(!module)
    {
        m_log.log(Logging::LogLevel::Error, "startupModuleMenuEditor Allocation Error");
        return;
    }

    startupModule(module);
}*/

/**
 * @brief Startup the User Editor Module
 *
void MenuSystem::startupModuleUserEditor()
{
    // Setup the input processor
    resetMenuInputIndex(MODULE_INPUT);

    // Allocate and Create
    module_ptr module = std::make_shared<ModUserEditor>(
        getLockedSession(), m_config, m_ansi_process, m_common_io, m_session_io
    );

    if(!module)
    {
        m_log.log(Logging::LogLevel::Error, "startupModuleUserEditor Allocation Error");
        return;
    }

    startupModule(module);
}*/

/**
 * @brief Startup the Level Editor Module
 *
void MenuSystem::startupModuleLevelEditor()
{
    // Setup the input processor
    resetMenuInputIndex(MODULE_INPUT);

    // Allocate and Create
    module_ptr module = std::make_shared<ModLevelEditor>(
        getLockedSession(), m_config, m_ansi_process, m_common_io, m_session_io
    );

    if(!module)
    {
        m_log.log(Logging::LogLevel::Error, "startupModuleLevelEditor Allocation Error");
        return;
    }

    startupModule(module);
}*/

/**
 * @brief Startup the Full Screen Message Editor Module
 *
void MenuSystem::startupModuleMessageEditor()
{
    // Setup the input processor
    resetMenuInputIndex(MODULE_INPUT);

    // Allocate and Create
    module_ptr module = std::make_shared<ModMessageEditor>(
        getLockedSession(), m_config, m_ansi_process, m_common_io, m_session_io
    );

    if(!module)
    {
        m_log.log(Logging::LogLevel::Error, "startupModuleMessageEditor Allocation Error");
        return;
    }

    startupModule(module);
}*/


/**
 * @brief Handles Input for Login and PreLogin Sequences.
 *        On Login Failures kicks back out to the Matrix.
 * @param character_buffer
 * @param is_utf8
 */
void MenuSystem::handleLoginInputSystem(const std::string &character_buffer, const bool &is_utf8) {

    // Make sure we have an allocated module before processing.
    if(m_module_stack.size() == 0 || character_buffer.size() == 0)
    {
        return;
    }

    // Allocate and Create
    m_module_stack.back()->update(character_buffer, is_utf8);

    m_log.log(Logging::LogLevel::Debug, "update - handleLoginInputSystem");

    // Finished modules processing.
    if(!m_module_stack.back()->m_is_active)
    {
        m_log.log(Logging::LogLevel::Debug,
            "*** !m_module_stack.back()->m_is_active - shutting down module: "
            , m_module_stack.back()->m_filename);
        shutdownModule();

        // Check if the current user has been logged in yet.
        if(session && session->m_is_session_authorized)
        {
            // If Authorized, then we want to move to main! Startup menu should be TOP or
            // Specified in Config file!  TODO
            m_log.log(Logging::LogLevel::Debug, "m_is_session_authorized");

            // TODO This should be individual users start menu!
            if(m_config.starting_menu_name.size() > 0)
            {
                m_current_menu = m_config.starting_menu_name;
                m_starting_menu = m_config.starting_menu_name;
            }
            else
            {
                // Default to main if nothing is set in config file.
                m_current_menu = "main";
                m_starting_menu = "main";
            }
        }
        else
        {
            m_log.log(Logging::LogLevel::Debug, "!m_is_session_authorized");
            m_current_menu = "matrix";
        }

        m_log.log(Logging::LogLevel::Debug, "loadAndStartupMenu on initial login");

        if (session)
        {
            loadAndStartupMenu();
        }
    }
}

/**
 * @brief Handles parsing input for Logon module
 *
 */
void MenuSystem::moduleLogonInput(const std::string &character_buffer, const bool &is_utf8) {
    handleLoginInputSystem(character_buffer, is_utf8);
}

/**
 * @brief Handles parsing input for modules
 * (Other Than Logon)
 */
void MenuSystem::moduleInput(const std::string &character_buffer, const bool &is_utf8) {

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
