#include "menu_system.hpp"

#include <cassert>
#include <string>
#include <vector>
#include <map>
#include <functional>

#include "model-sys/config.hpp"

/*
#include "mods/mod_signup.hpp"
#include "mods/mod_user_editor.hpp"
#include "mods/mod_level_editor.hpp"
#include "mods/mod_message_editor.hpp"
*/

#include "mods/mod_logon.hpp"
#include "mods/mod_signup.hpp"
#include "mods/mod_menu_editor.hpp"

#include "model-sys/context.hpp"
#include "tcp_session.hpp"
#include "logging.hpp"

MenuSystem::MenuSystem(Context &ctx)
    : MenuBase(ctx)
      , m_log(Logging::getInstance())
      , currentState(State::MenuSystem) {
    // Setup Menu Option Calls for executing menu commands.
    m_execute_callback.emplace_back(bind_member(this, &MenuSystem::menuOptionsCallback));

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

    m_log.log(Logging::LogLevel::Console, "MenuSystem()");

    // -------------------------------------------------
    // STATE HANDLERS
    // -------------------------------------------------
    bindStateHandlers();

    createHandlers.at(currentState)();
}

MenuSystem::~MenuSystem() {
    m_log.log(Logging::LogLevel::Console, "~MenuSystem()");

    // Clear All Menu Command Functions.
    m_menu_command_functions.clear();
    clearHandlers.clear();
    createHandlers.clear();
    inputHandlers.clear();
    pollHandlers.clear();
}

/**
 * @brief Handles Updates or Data Input from Client (StateManager)
 */
void MenuSystem::update(const std::string &character_buffer, const bool &is_utf8) {
    if (!m_is_active) {
        return;
    }

    inputHandlers.at(currentState)(character_buffer);
}

/**
 * @brief Startup class, setup initial screens / interface, flags etc..
 *        This is only called when switch to the state, not for menu instances.
 * @return
 */
bool MenuSystem::onEnter() {
    m_is_active = true;
    m_log.log(Logging::LogLevel::Console, "MenuSystem() - onEnter, state=", stateToString());
    return true;
}

/**
 * @brief Exit, close down, display screens to change over data.
 * @return
 */
bool MenuSystem::onExit() {
    m_log.log(Logging::LogLevel::Console, "MenuSystem() - onExit, state=", stateToString());
    m_is_active = false;
    return true;
}

bool MenuSystem::pollTimers() {
    std::cout << "Core MenuSystemPollTimers()" << std::endl;
    if (!m_is_active) {
        return true;
    }

    pollHandlers.at(currentState)();

    std::cout << "~Core MenuSystemPollTimers()" << std::endl;
    return true;
}

// For Modules
void MenuSystem::bindStateHandlers() {
    m_log.log(Logging::LogLevel::Console, "bindStateHandlers()");

    clearHandlers.clear();
    createHandlers.clear();
    inputHandlers.clear();
    pollHandlers.clear();

    // MenuSystem - Core Menu System is always Running!, so it doesn't clear
    // But we can switch to other states, then Menu System should Resume where it left off!
    clearHandlers.emplace(State::MenuSystem, [this]() { clearMenuSystem(); });
    createHandlers.emplace(State::MenuSystem, [this]() { createMenuSystem(); });
    pollHandlers.emplace(State::MenuSystem, [this]() { pollMenuSystem(); });
    inputHandlers.emplace(State::MenuSystem, [this](
                      const std::string &input) {
                              inputMenuSystem(input);
                          });

    // Logon
    clearHandlers.emplace(State::ModLogon, [this]() { clearLogon(); });
    createHandlers.emplace(State::ModLogon, [this]() { createLogon(); });
    pollHandlers.emplace(State::ModLogon, [this]() { pollLogon(); });
    inputHandlers.emplace(State::ModLogon, [this](
                      const std::string &input) {
                              inputLogon(input);
                          });

    // Signup
    clearHandlers.emplace(State::ModSignup, [this]() { clearSignup(); });
    createHandlers.emplace(State::ModSignup, [this]() { createSignup(); });
    pollHandlers.emplace(State::ModSignup, [this]() { pollSignup(); });
    inputHandlers.emplace(State::ModSignup, [this](
                      const std::string &input) {
                              inputSignup(input);
                          });

    // MenuEditor
    clearHandlers.emplace(State::ModMenuEditor, [this]() { clearMenuEditor(); });
    createHandlers.emplace(State::ModMenuEditor, [this]() { createMenuEditor(); });
    pollHandlers.emplace(State::ModMenuEditor, [this]() { pollMenuEditor(); });
    inputHandlers.emplace(State::ModMenuEditor, [this](
                      const std::string &input) {
                              inputMenuEditor(input);
                          });

    // Runtime guarantee (debug) MenuSystem is Default State, not a Module Loaded.
    assert(clearHandlers.size() == StateCount);
    assert(createHandlers.size() == StateCount);
    assert(pollHandlers.size() == StateCount);
    assert(inputHandlers.size() == StateCount);

    m_log.log(Logging::LogLevel::Console, "~bindStateHandlers()");
}

/**
 * Set a new State and Call it's Handlers
 * MenuSystem State is default always running, so we don't clear,
 * We switch away then back and resume where it left off.
 * @param newState
 */
void MenuSystem::setState(State newState) {
    if (!m_is_active) {
        return;
    }

    // Menu System is not a Module, just a default state flowing input back to Menu Base.
    if (newState == State::MenuSystem) {
        clearHandlers.at(currentState)();
        currentState = newState;
        // MenuSystem has no create, just defaults to Menu Input.
        return;
    }

    // Switch to Module, if Menu System, no clearing needed.
    if (currentState != State::MenuSystem) {
        clearHandlers.at(currentState)();
        currentState = newState;
        createHandlers.at(currentState)();
        return;
    }

    // currentState == State::MenuSystem
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
            requestMenuJump(option.command_string, MenuJumpMode::PushCurrent);
            break;

        // goes to fallback menu, sets fallback to previous fallback
        case '\\':
            requestMenuJump("", MenuJumpMode::PopFallback);
            break;

        // Goes to menu, sets fallback as starting menu
        case '^':
            requestMenuJump(option.command_string, MenuJumpMode::PushStarting);
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
            requestMenuJump(option.command_string, MenuJumpMode::SkipFirstCmd);
            break;

        // Drops to Previous Menu, does not exe firstcmd
        case '}':
            requestMenuJump("", MenuJumpMode::PreviousNoFirst);
            break;

        // Toggles locking of output to the modem.
        case ':':
            return false;

        // Toggles locking of input from the modem.
        case ';':
            return false;

        // Goes to a menu keeping the current fallback menu does firstcmd
        case '$':
            requestMenuJump(option.command_string, MenuJumpMode::PushCurrent);
            break;

        // Goes to a menu keeping the current fallback menu doesn't firstcmd
        case '%':
            // TODO Update to new CurrentNoFirst
            requestMenuJump(option.command_string, MenuJumpMode::SkipFirstCmd);
            break;

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

    m_log.log(Logging::LogLevel::Console, "Matrix CommandKey=", option.command_key);

    switch (option.command_key[1]) {
        // Logon
        // {Not Implemented yet!}
        // When a CString is specified, PRELOGON.X, an
        // USERLOG.X, and SYSPASS.X will be displayed.
        // { Note: add 0 for random! }
        case 'S':
            m_log.log(Logging::LogLevel::Info, "Executing startupModuleLogon()");
            setState(State::ModLogon);
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
            m_log.log(Logging::LogLevel::Info, "Executing startupModuleSignup()");
            setState(State::ModSignup);
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
 * @brief Changes States in the MenuBase (Core) System
 * @param newState
 */
void MenuSystem::setMenuBaseState(BaseState newState) {
    m_baseState = newState;
}

/**
 * @brief Retrieves Base Menu State
 * @param
 */
MenuBase::BaseState MenuSystem::getMenuBaseState() {
    return m_baseState;
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
 * @brief Startup the Menu Editor Module
 *
void MenuSystem::startupModuleMenuEditor()
{
    // Setup the input processor
    resetMenuInputIndex(MODULE_INPUT);

    // Allocate and Create
    module_ptr module = std::make_shared<ModMenuEditor>(
        getLockedSession(), m_config, m_ansi_process, m_io_common, m_session_io
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
        getLockedSession(), m_config, m_ansi_process, m_io_common, m_session_io
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
        getLockedSession(), m_config, m_ansi_process, m_io_common, m_session_io
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
        getLockedSession(), m_config, m_ansi_process, m_io_common, m_session_io
    );

    if(!module)
    {
        m_log.log(Logging::LogLevel::Error, "startupModuleMessageEditor Allocation Error");
        return;
    }

    startupModule(module);
}*/


// -------------------------
// Logon Module
// -------------------------

void MenuSystem::createMenuSystem() {
    m_log.log(Logging::LogLevel::Console, "MenuSystem() createMenuSystem");
    m_current_menu = "matrix";
    m_starting_menu = "matrix";
    requestMenuJump(m_current_menu, MenuJumpMode::Normal);
    //loadAndStartupMenu();
}

void MenuSystem::clearMenuSystem() {
    std::cout << "MenuSystem cleared (NOT USED)\n";
}

void MenuSystem::pollMenuSystem() {
    // No Timbers Setup Yet,  This could be Rumors, Properties,
    // Realtime Clock etc.. or Node Messages
}

void MenuSystem::inputMenuSystem(const std::string &input) {
    m_log.log(Logging::LogLevel::Console, "MenuSystem() inputMenuSystem");

    if (!m_is_active) {
        return;
    }

    // Default, have to check if we need to detect and goto YesNo Bar For Prompts!!
    // Note sure if this will detect it properly yet!
    if (getMenuBaseState() == BaseState::MENU_INPUT) {
        // Manages General Menu Input and Lightbar Menu HotKeys
        menuInput(input, false);
    } else {
        // Manages Hotkey Input for Yes/No Menu Bar Prompts.
        menuYesNoBarInput(input, false);
    }
}

// -------------------------
// Logon Module
// -------------------------

void MenuSystem::createLogon() {
    m_log.log(Logging::LogLevel::Console, "MenuSystem() createLogon");

    // Make Sure we cover any unexpected errors in Creating the Module.
    try {
        logonState.emplace(m_ctx);
        logonState->onEnter();
    } catch (std::exception &ex) {
        std::cout << "createLogon Exception: " << ex.what() << std::endl;
        throw;
    }
}

void MenuSystem::clearLogon() {
    if (logonState) {
        logonState->onExit();
        logonState.reset();
    }
    std::cout << "Logon cleared\n";
}

void MenuSystem::pollLogon() {
    if (logonState) {
        // No timers currently setup for Logon.
        // logonState->pollTimers();
    }
}

void MenuSystem::inputLogon(const std::string &input) {
    m_log.log(Logging::LogLevel::Console, "MenuSystem() inputLogon");

    if (logonState) {
        logonState->update(input, false);
    }

    // Finished modules processing.
    if (!logonState->m_is_active) {
        m_log.log(Logging::LogLevel::Info, "!LogonState->m_is_active - shutting down module: ");

        if (!logonState->m_is_active) {
            m_log.log(Logging::LogLevel::Console, "MenuSystem() logonState is Inactive");

            // After Logon, we Move back to Menu System
            setState(State::MenuSystem);

            // Reset the Input back to the Menu System
            setMenuBaseState(BaseState::MENU_INPUT);

            // Redisplay,  may need to startup() again, but menu data should still be active and loaded!
            // redisplayMenuScreen();
        }

        // Check if the current user has been logged in yet.
        // And is Authorized to Continue In the system.
        if (m_ctx.getSessionWrite().isAuthorized()) {
            // If Authorized, then we want to move to main! Startup menu should be TOP or
            // Specified in Config file!  TODO
            m_log.log(Logging::LogLevel::Info, "m_is_session_authorized=", m_ctx.getSessionWrite().isAuthorized());

            // TODO This should be individual users start menu!
            if (m_ctx.getCfg().starting_menu_name.size() > 0) {
                m_current_menu = m_ctx.getCfg().starting_menu_name;
                m_starting_menu = m_ctx.getCfg().starting_menu_name;
            } else {
                // Default to main if nothing is set in config file.
                // Eg PreLogon, Welcome, top, any other menus.
                m_current_menu = "main";
                m_starting_menu = "main";
            }
        } else {
            m_log.log(Logging::LogLevel::Debug, "!m_is_session_authorized");
            m_current_menu = "matrix";
        }

        m_log.log(Logging::LogLevel::Debug, "loadAndStartupMenu on initial login");

        if (m_ctx.getSessionWrite().isActive()) {
            // After Successful Logon, we Move back to Menu System
            setState(State::MenuSystem);

            // Reset the Input back to the Menu System
            setMenuBaseState(BaseState::MENU_INPUT);

            if (m_ctx.getSessionWrite().isAuthorized()) {
                requestMenuJump(m_current_menu, MenuJumpMode::PushStarting);
            }
            else {
                redisplayMenuScreen();
            }
        } else {
            m_is_active = false;
        }
    }
}

// -------------------------
// Signup Module
// -------------------------

void MenuSystem::createSignup() {
    m_log.log(Logging::LogLevel::Console, "MenuSystem() createSignup");

    // Make Sure we cover any unexpected errors in Creating the Module.
    try {
        signupState.emplace(m_ctx);
        signupState->onEnter();
    } catch (std::exception &ex) {
        std::cout << "createSignup Exception: " << ex.what() << std::endl;
        throw;
    }
}

void MenuSystem::clearSignup() {
    if (signupState) {
        signupState->onExit();
        signupState.reset();
    }
    std::cout << "Signup cleared\n";
}

void MenuSystem::pollSignup() {
    if (signupState) {
        // No timers currently setup for Logon.
        // logonState->pollTimers();
    }
}

void MenuSystem::inputSignup(const std::string &input) {
    m_log.log(Logging::LogLevel::Console, "MenuSystem() inputSignup");

    if (signupState) {
        signupState->update(input, false);
    }

    // Finished modules processing.
    if (!signupState->m_is_active) {
        m_log.log(Logging::LogLevel::Info, "!signupState->m_is_active - shutting down module: ");

        if (!signupState->m_is_active) {
            m_log.log(Logging::LogLevel::Console, "MenuSystem() signupState is Inactive");

            // After Signup, we Move back to Menu System
            setState(State::MenuSystem);

            // Reset the Input back to the Menu System
            setMenuBaseState(BaseState::MENU_INPUT);
            redisplayMenuScreen();
        }
    }
}

// -------------------------
// Menu Editor Module
// -------------------------

void MenuSystem::createMenuEditor() {
    m_log.log(Logging::LogLevel::Console, "MenuSystem() createMenuEditor");

    // Make Sure we cover any unexpected errors in Creating the Module.
    try {
        menuEditorState.emplace(m_ctx);
        menuEditorState->onEnter();
    } catch (std::exception &ex) {
        std::cout << "createMenuEditor Exception: " << ex.what() << std::endl;
        throw;
    }
}

void MenuSystem::clearMenuEditor() {
    if (menuEditorState) {
        menuEditorState->onExit();
        menuEditorState.reset();
    }
    std::cout << "MenuEditor cleared\n";
}

void MenuSystem::pollMenuEditor() {
    if (menuEditorState) {
        // No timers currently setup for Logon.
        // logonState->pollTimers();
    }
}

void MenuSystem::inputMenuEditor(const std::string &input) {
    m_log.log(Logging::LogLevel::Console, "MenuSystem() inputMenuEditor");

    if (menuEditorState) {
        menuEditorState->update(input, false);
    }

    // Finished modules processing.
    if (!menuEditorState->m_is_active) {
        m_log.log(Logging::LogLevel::Info, "!menuEditorState->m_is_active - shutting down module: ");

        if (!menuEditorState->m_is_active) {
            m_log.log(Logging::LogLevel::Console, "MenuSystem() menuEditorState is Inactive");

            // After Signup, we Move back to Menu System
            setState(State::MenuSystem);

            // Reset the Input back to the Menu System
            setMenuBaseState(BaseState::MENU_INPUT);
            redisplayMenuScreen();
        }
    }
}