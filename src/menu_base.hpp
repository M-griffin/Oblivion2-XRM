#ifndef MENUBASE_HPP
#define MENUBASE_HPP

#include "session_io.hpp"
#include "common_io.hpp"
#include "session_data.hpp"
#include "ansi_processor.hpp"
#include "communicator.hpp"

#include "model-sys/struct_compat.hpp"
#include "model-sys/config.hpp"
#include "model-sys/menu.hpp"
#include "model-sys/menu_prompt.hpp"

#include "data-sys/menu_dao.hpp"
#include "data-sys/menu_prompt_dao.hpp"
#include "mods/mod_base.hpp"

#include <boost/smart_ptr/shared_ptr.hpp>

#include <functional>
#include <vector>

class Config;
typedef boost::shared_ptr<Config> config_ptr;

class SessionData;
typedef boost::shared_ptr<SessionData> session_data_ptr;

/**
 * @class MenuBase
 * @author Michael Griffin
 * @date 10/11/2016
 * @file menu_base.hpp
 * @brief Base Class for Menu System and Interfaces needing dynamic command execution
 */
class MenuBase
{
public:
    explicit MenuBase(session_data_ptr session_data);
    ~MenuBase();

    // This matches the index for menu_functions.push_back
    enum
    {
        MENU_INPUT,
        MENU_YESNO_BAR,
        MENU_EDITOR_INPUT,
        MODULE_PRELOGON_INPUT,
        MODULE_LOGON_INPUT,
        MODULE_INPUT,
        FORM_INPUT
    };

    session_data_ptr m_menu_session_data;

    // This hold non-hotkey text passed through.
    // If Hotkeys are turn off, we append and loop this until we hit a CRLF. or ENTER
    //session_data_ptr m_session_data;     // Handles to the session for i/o. {in TheState Base Class!}
    SessionIO        m_session_io;         // SessionIO for Output parsing and MCI Codes etc.
    CommonIO         m_common_io;          // CommonIO
    config_ptr       m_config;             // Config
    std::string      m_line_buffer;        // Buffer used for menu system and reading field data.
    bool             m_use_hotkey;         // Toggle for Single Hotkey or GetLine input. - Not used yet!
    std::string      m_current_menu;       // Name of current menu loaded.
    std::string      m_previous_menu;      // Name of Previous Menu for Gosub
    std::string      m_fallback_menu;      // Fallback, this can set as a Global Fall Back and changed via menu command
    std::string      m_starting_menu;      // Starting Menu, also used as Fallback.
    int              m_input_index;        // Menu Input Index, for Forwarding to current function.

    menu_ptr         m_menu_info;          // Menu Info
    menu_prompt_ptr  m_menu_prompt;        // Menu Prompt

    ansi_process_ptr m_ansi_process;       // Instance for AnsiProcess Methods
    int              m_active_pulldownID;  // Active Lightbar Position.

    // Flags
    bool             m_fail_flag;                   // If menu or Option fails, kick off the fail flag.
    bool             m_pulldown_reentrace_flag;     // If menu or Option fails, kick off the fail flag.
    bool             m_is_active_pulldown_menu;     // If menu has active lightbars to display.
    bool             m_use_first_command_execution; // If menu executes firstcmd on entrance.
    bool             m_logoff;                      // If logoff, stop loop execution on commands and exit.


    // Holds all pulldown menu options.
    std::vector<MenuOption> m_loaded_pulldown_options;

    // Dynamic Async Input Function Vector.
    std::vector<std::function< void(const std::string &, const bool &is_utf8)> > m_menu_functions;

    // Handles Dynamic Menu Command Option Execution
    std::vector<std::function< bool(const MenuOption &)> > m_execute_callback;

    // Handle Dynamic modules being executed.
    std::vector<module_ptr> m_module;


    /**
     * @brief Method for Adding outgoing text data to ansi processor
     *        Then delivering the data to the client
     * @param data
     */
    void baseProcessAndDeliver(std::string data)
    {
        m_ansi_process->parseAnsiScreen((char *)data.c_str());
        m_menu_session_data->deliver(data);
    }

    /**
     * @brief Clears out Loaded Pulldown options { Called From readInMenuData() }
     */
    void clearMenuPullDownOptions();

    /**
     * @brief Validates if user has access to menu (preLoad)
     * @return
     */
    bool checkMenuAcsAccess(menu_ptr menu);

    /**
     * @brief Validates if user has access to menu options
     * @return
     */
    void checkMenuOptionsAcsAccess();

    /**
     * @brief Read Menu Data File for Info and all Options
     */
    void readInMenuData();
    void writeOutMenuData();
    void readMenuOptions();
    void writeMenuOptions();

    /**
     * @brief MenuSystemPrompts
     */
    void clearAllMenuPrompts();
    void readMenuAllPrompts();
    void readMenuPrompts(int menu_index);

    /**
     * @brief Setup light bar string, and return default display.
     */
    std::string setupYesNoMenuInput(const std::string &menu_prompt, std::vector<MapType> &code_map);

    /**
     * @brief Gets the Default Color Sequence
     * @return
     */
    std::string getDefaultColor();

    /**
     * @brief Gets the Default Input Color Sequence
     * @return
     */
    std::string getDefaultInputColor();

    /**
     * @brief Gets the Default Input Color Sequence
     * @return
     */
    std::string getDefaultInverseColor();

    /**
     * @brief Builds the menu prompt as a question/input string
     * @return
     */
    std::string parseMenuPromptString(const std::string &prompt_string);

    /**
     * @brief Loads or reloads and parses the ansi screen
     */
    std::string loadMenuScreen();

    /**
     * @brief Processes a TOP Template Screen for Menus
     * @param screen
     * @return
     */
    std::string processTopGenericTemplate(const std::string &screen);

    /**
     * @brief Processes a MID Template Screen for Menus
     * @param screen
     * @return
     */
    std::string processMidGenericTemplate(const std::string &screen);

    /**
     * @brief Generic SRT, MID, END screen processing
     * @return
     */
    std::string processGenericScreens();

    /**
     * @brief Load the Menu into the system container.
     */
    void loadInMenu(std::string menu_name);

    /**
     * @brief Import the Menu from Modules into the system container.
     */
    void importMenu(menu_ptr menu_info);

    /**
     * @brief Build string of lightbar matrices
     */
    std::string buildLightBars();

    /**
     * @brief De parse and display current menu system.
     */
    void redisplayMenuScreen();

    /**
     * @brief Execute First and Each Commands on Startup
     */
    void executeFirstAndEachCommands();

    /**
     * @brief Return Selected or Active prompt as a string.
     * @return
     */
    std::string loadMenuPrompt();

    /**
     * @brief Move to End of Display then output
     * @param output
     */
    void moveToBottomAndDisplay(const std::string &prompt);

    /**
     * @brief Move to End of Display then Setup Display for String
     * @param output
     */
    std::string moveStringToBottom(const std::string &prompt);

    // Menu System will be a (2) Function system 1st setups up and displays
    // The Second handles all I/O for the menu options, this is dynamic since
    // Input is passed through to it, then the function returns for the next input
    // Since ere async, nothing stays inside it always returns.
    void loadAndStartupMenu();

    /**
     * @brief Menu Editor, Runs through all existing menus
     */
    std::string displayMenus();

    /**
     * @brief Updates current and next lightbar positions.
     */
    void lightbarUpdate(int previous_pulldown_id);

    /**
     * @brief Handles Menu Option Call Back
     * @param input
     */
    bool executeMenuOptions(const MenuOption &option);

    /**
     * @brief Handle Standard Menu Input with Wildcard input
     * @param input
     * @param key
     * @return
     */
    bool handleStandardMenuInput(const std::string &input, const std::string &key);

    /**
     * @brief Handle updating lightbar selections and redraw
     * @param input
     * @return
     */
    bool handleLightbarSelection(const std::string &input);

    /**
     * @brief Handle Pulldown Specific Command Processing
     * @param input_text
     * @param m
     * @param is_enter
     * @return
     */
    bool handlePulldownHotKeys(const MenuOption &m, const bool &is_enter, bool &stack_reassignment);

    /**
     * @brief Handles Re-running EACH command re-executed after each refresh
     */
    void executeEachCommands();

    /**
     * @brief Parse Menu Prompt Folder and pull list of Prompt Names
     * @return
     */
    std::vector<std::string> getListOfMenuPrompts();

    /**
     * @brief Parse Menu Prompt Folder and pull list of Prompt Names
     * @return
     */
    std::string getRandomMenuPrompt();


    bool processMenuOptions(const std::string &input);

    /**
     * @brief Handle Input Specific to Pull Down Menus
     * @param character_buffer
     */
    void handlePulldownInput(const std::string &character_buffer, const bool &is_utf8);

    /**
     * @brief Handle Input Specific to Pull Down Menus
     * @param character_buffer
     */
    void handleStandardInput(const std::string &character_buffer);

    /**
     * @brief Default Menu Input Processing.
     *        Handles Processing for Loaded Menus Hotkey and Lightbars
     */
    void menuInput(const std::string &character_buffer, const bool &is_utf8);

    /**
     * @brief Yes/No Menu Bar Input Processing.
     *        Handles Processing for Loaded Menus Hotkey and Lightbars
     */
    void menuYesNoBarInput(const std::string &character_buffer, const bool &is_utf8);

};

typedef boost::shared_ptr<MenuBase> menu_base_ptr;

#endif // MENUBASE_HPP
