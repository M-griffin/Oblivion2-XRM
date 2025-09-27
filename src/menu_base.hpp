#ifndef MENU_BASE_HPP
#define MENU_BASE_HPP

#include <memory>
#include <functional>
#include <vector>

#include "model-sys/struct_compat.hpp"
#include "model-sys/structures.hpp"
#include "model-sys/menu.hpp"
#include "model-sys/menu_prompt.hpp"

#include "data-sys/menu_dao.hpp"
#include "data-sys/menu_prompt_dao.hpp"
//#include "mods/mod_base.hpp"

class Logging;

class Config;
typedef std::shared_ptr<Config> config_ptr;

class Session;

class SessionIO;
typedef std::shared_ptr<SessionIO> session_io_ptr;

class CommonIO;
typedef std::shared_ptr<CommonIO> common_io_ptr;

class Directory;
typedef std::shared_ptr<Directory> directory_ptr;

/**
 * @class MenuBase
 * @author Michael Griffin
 * @date 10/11/2016
 * @file menu_base.hpp
 * @brief Base Class for Menu System and Interfaces needing dynamic command execution
 */
class MenuBase {
public:
    explicit MenuBase(Session &session);

    ~MenuBase();

    // This matches the index for menu_functions.push_back
    enum {
        MENU_INPUT,
        MENU_YESNO_BAR,
        MODULE_PRELOGON_INPUT,
        MODULE_LOGON_INPUT,
        MODULE_INPUT,
        FORM_INPUT
    };

    Logging &m_log;
    Session &m_session;

    // This hold non-hotkey text passed through.
    // If Hotkeys are turn off, we append and loop this until we hit a CRLF. or ENTER
    common_io_ptr m_common_io; // CommonIO
    session_io_ptr m_session_io; // SessionIO for Output parsing and MCI Codes etc.
    config_ptr m_config; // Config
    directory_ptr m_directory; // Directory File Lists.
    std::string m_line_buffer; // Buffer used for menu system and reading field data.
    bool m_use_hotkey; // Toggle for Single Hotkey or GetLine input. - Not used yet!
    std::string m_current_menu; // Name of current menu loaded.
    std::string m_previous_menu; // Name of Previous Menu for Gosub
    std::string m_fallback_menu; // Fallback, this can set as a Global Fall Back and changed via menu command
    std::string m_starting_menu; // Starting Menu, also used as Fallback.
    int m_input_index; // Menu Input Index, for Forwarding to current function.

    menu_ptr m_menu_info; // Menu Info
    menu_prompt_ptr m_menu_prompt; // Menu Prompt

    //processor_ansi_ptr m_ansi_process; // Instance for AnsiProcess Methods
    unsigned int m_active_pulldownID; // Active Lightbar Position.

    // Flags
    bool m_fail_flag; // If menu or Option fails, kick off the fail flag.
    bool m_pulldown_reentrace_flag; // If menu or Option fails, kick off the fail flag.
    bool m_is_active_pulldown_menu; // If menu has active light bars to display.
    bool m_use_first_command_execution; // If menu executes firstcmd on entrance.
    bool m_logoff; // If logoff, stop loop execution on commands and exit.

    // Holds all pulldown menu options.
    std::vector<MenuOption> m_loaded_pulldown_options;

    // Dynamic Async Input Function Vector.
    std::vector<std::function<void(const std::string &, const bool &is_utf8)> > m_menu_functions;

    // Handles Dynamic Menu Command Option Execution
    std::vector<std::function<bool(const MenuOption &)> > m_execute_callback;

    // Handle Dynamic modules being executed.
    //std::vector<module_ptr> m_module_stack;

    std::string upper_case(const std::string &string_sequence);
    std::string lower_case(const std::string &string_sequence);

    void baseProcessAndDeliver(std::string data);
    void clearMenuPullDownOptions();
    bool checkMenuAcsAccess(menu_ptr menu);
    void checkMenuOptionsAcsAccess();

    void readInMenuData();
    void writeOutMenuData();
    void readMenuOptions();
    void writeMenuOptions();
    void clearAllMenuPrompts();
    void readMenuAllPrompts();
    void readMenuPrompts(int menu_index);

    std::string setupYesNoMenuInput(const std::string &menu_prompt, std::vector<MapType> &code_map);
    std::string getDefaultColor();
    std::string getDefaultInputColor();
    std::string getDefaultInverseColor();
    std::string parseMenuPromptString(const std::string &prompt_string);
    std::string loadMenuScreen();
    std::string processTopGenericTemplate(const std::string &screen);
    std::string processMidGenericTemplate(const std::string &screen);
    std::string processGenericScreens();

    void loadInMenu(std::string menu_name);
    void importMenu(menu_ptr menu_info);
    std::string buildLightBars();
    void redisplayMenuScreen();
    void executeFirstAndEachCommands();
    std::string loadMenuPrompt();
    void moveToBottomAndDisplay(const std::string &prompt);
    std::string moveStringToBottom(const std::string &prompt);
    void loadAndStartupMenu();
    void lightbarUpdate(unsigned int previous_pulldown_id);
    bool executeMenuOptions(const MenuOption &option);

    bool handleStandardMenuInput(const std::string &input, const std::string &key);
    bool handleLightbarSelection(const std::string &input);
    bool handlePulldownHotKeys(const MenuOption &m, const bool &is_enter, bool &stack_reassignment);
    void executeEachCommands();

    std::vector<std::string> getListOfMenuPrompts();
    std::string getRandomMenuPrompt();

    bool processMenuOptions(const std::string &input);
    void handlePulldownInput(const std::string &character_buffer, const bool &is_utf8);
    void handleStandardInput(const std::string &character_buffer);
    void menuInput(const std::string &character_buffer, const bool &is_utf8);
    void menuYesNoBarInput(const std::string &character_buffer, const bool &is_utf8);
};

#endif
