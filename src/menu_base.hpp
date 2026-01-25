#ifndef MENU_BASE_HPP
#define MENU_BASE_HPP

#include <string>
#include <vector>
#include <functional>

#include "directory.hpp"

#include "model-sys/structures.hpp"
#include "model-sys/menu.hpp"
#include "model-sys/menu_prompt.hpp"
#include "model-sys/context.hpp"

class Context;
class Logging;

/**
 * @class MenuBase
 * @author Michael Griffin
 * @date 10/11/2016
 * @file menu_base.hpp
 * @brief Base Class for Menu System and Interfaces needing dynamic command execution
 */
class MenuBase {
public:
    explicit MenuBase(Context &ctx);
    ~MenuBase();

    // This matches the index for menu_functions.push_back
    enum class BaseState : uint8_t {
        MENU_INPUT = 0,
        MENU_YESNO_BAR
    };

    Logging &m_log;
    Context &m_ctx;
    Directory m_directory;

    // Internal Menu and Prompt Holders
    Menu m_menu_info; // Menu Info
    MenuPrompt m_menu_prompt; // Menu Prompt

    std::string m_line_buffer; // Buffer used for menu system and reading field data.
    bool m_use_hotkey; // Toggle for Single Hotkey or GetLine input. - Not used yet!
    std::string m_current_menu; // Name of current menu loaded.
    std::string m_previous_menu; // Name of Previous Menu for Gosub
    std::string m_fallback_menu; // Fallback, this can set as a Global Fallback and changed via menu command
    std::string m_starting_menu; // Starting Menu, also used as Fallback.
    BaseState m_baseState; // Menu Input Index, for Forwarding to current function.

    unsigned int m_active_pulldownID; // Active Lightbar Position.

    // Flags
    bool m_fail_flag; // If menu or Option fails, kick off the fail flag.
    bool m_pulldown_reentrace_flag; // If menu or Option fails, kick off the fail flag.
    bool m_is_active_pulldown_menu; // If menu has active light bars to display.
    bool m_use_first_command_execution; // If menu executes firstcmd on entrance.
    bool m_logoff; // If logoff, stop loop execution on commands and exit.
    bool m_is_active;

    // Holds all pull down menu options.
    std::vector<MenuOption> m_loaded_pulldown_options;

    // Handles Dynamic Menu Command Option Execution
    std::vector<std::function<bool(const MenuOption &)> > m_execute_callback;

    void baseProcessAndDeliver(std::string data);

    void clearMenuPullDownOptions();

    bool checkMenuAcsAccess(const Menu &menu);

    void checkMenuOptionsAcsAccess();

    void readInMenuData();

    std::string setupYesNoMenuInput(const std::string &menu_prompt, std::vector<CodeMapType> &code_map);

    std::string getDefaultColor();

    std::string getDefaultInputColor();

    std::string getDefaultInverseColor();

    std::string parseMenuPromptString(const std::string &prompt_string);

    std::string loadMenuScreen();

    std::string processTopGenericTemplate(const std::string &screen);

    std::string processMidGenericTemplate(const std::string &screen);

    std::string processGenericScreens();

    void loadInMenu(std::string menu_name);

    void importMenu(Menu &menu_info);

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

    bool handlePullDownHotKeys(const MenuOption &m, const bool &is_enter, bool &stack_reassignment);

    void executeEachCommands();

    std::vector<std::string> getListOfMenuPrompts();

    std::string getRandomMenuPrompt();

    bool processMenuOptions(const std::string &input);

    void handlePullDownInput(const std::string &character_buffer, const bool &is_utf8);

    void handleStandardInput(const std::string &character_buffer);

    void menuInput(const std::string &character_buffer, const bool &is_utf8);

    void menuYesNoBarInput(const std::string &character_buffer, const bool &is_utf8);
};

#endif
