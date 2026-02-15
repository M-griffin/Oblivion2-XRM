#ifndef MENU_BASE_HPP
#define MENU_BASE_HPP

#include <string>
#include <vector>
#include <functional>
#include <optional>

#include "util_dir.hpp"

#include "model-sys/structures.hpp"
#include "model-sys/menu.hpp"
#include "model-sys/menu_prompt.hpp"
#include "model-sys/context.hpp"

#include "menu_cmd_chain.hpp"

class Context;
class UtilLog;


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

    enum class MenuLoadReason {
        Initial,
        Jump,
        Redisplay
    };

    std::string MenuLoadReasonToString(MenuLoadReason mode) const {
        switch (mode) {
            case MenuLoadReason::Initial: return "Initial";
            case MenuLoadReason::Jump: return "Jump";
            case MenuLoadReason::Redisplay: return "Redisplay";
            default: return "Unknown";
        }
    }

    // This matches the index for menu_functions.push_back
    enum class BaseState : uint8_t {
        MENU_INPUT = 0,
        MENU_YESNO_BAR
    };

    enum class MenuJumpMode {
        Normal,
        PushCurrent,
        PushStarting,
        KeepFallback,
        PopFallback,
        SkipFirstCmd,
        PreviousNoFirst
    };

    std::string MenuJumpModeToString(MenuJumpMode mode) const {
        switch (mode) {
            case MenuJumpMode::Normal: return "Normal";
            case MenuJumpMode::PushCurrent: return "PushCurrent";
            case MenuJumpMode::PushStarting: return "PushStarting";
            case MenuJumpMode::KeepFallback: return "KeepFallback";
            case MenuJumpMode::PopFallback: return "PopFallback";
            case MenuJumpMode::SkipFirstCmd: return "SkipFirstCmd";
            case MenuJumpMode::PreviousNoFirst: return "PreviousNoFirst";
            default: return "Unknown";
        }
    }

    struct CommandChain {
        std::vector<MenuOption> commands;
    };

    UtilLog &m_log;
    Context &m_ctx;
    UtilDir m_directory;

    // Handles Stack of Current and Fallback menus.
    std::deque<std::string> m_menuStack;

protected:
    CommandChainExecutor m_cmdChainExecutor;

public:
    // Internal Menu and Prompt Holders
    Menu m_menu_info; // Menu Info
    MenuPrompt m_menu_prompt; // Menu Prompt

    std::string m_line_buffer; // Buffer used for menu system and reading field data.
    bool m_use_hotkey; // Toggle for Single Hotkey or GetLine input. - Not used yet!
    std::string m_current_menu; // Name of current menu loaded.
    std::string m_previous_menu; // Name of Previous Menu for Gosub
    std::string m_starting_menu; // Starting Menu, also used as Fallback.
    BaseState m_baseState; // Menu Input Index, for Forwarding to current function.

    unsigned int m_active_pulldownID; // Active Lightbar Position.

    // Flags
    //bool m_fail_flag; // If menu or Option fails, kick off the fail flag.
    bool m_pulldown_reentrace_flag; // If menu or Option fails, kick off the fail flag.
    bool m_is_active_pulldown_menu; // If menu has active light bars to display.
    bool m_logoff; // If logoff, stop loop execution on commands and exit.
    bool m_is_active;
    bool m_failFlag;
    bool m_pendingMenuJump;

    std::string m_pendingMenuName;
    MenuJumpMode m_pendingJumpMode;

    // Holds all pull down menu options.
    std::vector<MenuOption> m_loaded_pulldown_options;

    // Handles Dynamic Menu Command Option Execution
    std::vector<std::function<bool(const MenuOption &)> > m_execute_callback;

    void baseProcessAndDeliver(std::string data);

    void clearMenuPullDownOptions();

    bool checkMenuAcsAccess(const Menu &menu);

    void buildMenuOptionsFromAcs();

    void injectFirstCommands();

    void requestMenuJump(const std::string &menu, MenuJumpMode mode);

    std::string resolveFallbackMenu();

    std::string setupYesNoMenuInput(const std::string &menu_prompt, std::vector<CodeMapType> &code_map);

    std::string getDefaultColor();

    std::string getDefaultInputColor();

    std::string getDefaultInverseColor();

    void loadMenuDefinition(const std::string &menuName);

    bool shouldExecuteFirst(MenuLoadReason reason);

    void prepareMenuState(MenuLoadReason reason);

    void enterMenu(const std::string &menuName, MenuLoadReason reason);

    bool handleSpecialPulldownModes();

    void setupPulldownsAndLightbars(const std::string &raw_buffer, std::string &output);

    std::string parseMenuPromptString(const std::string &prompt_string);

    std::string loadMenuScreen();

    std::string processTopGenericTemplate(const std::string &screen);

    std::string processMidGenericTemplate(const std::string &screen);

    std::string processGenericScreens();

    std::string buildLightBars();

    void redisplayMenuScreen();

    bool executeWithAcs(const MenuOption &opt);

    /*
    void executeFirstCmds();
    */

    std::string loadMenuPrompt();

    void moveToBottomAndDisplay(const std::string &prompt);

    std::string moveStringToBottom(const std::string &prompt);

    void lightbarUpdate(unsigned int previous_pulldown_id);

    bool executeMenuOptions(const MenuOption &option);

    bool handleStandardMenuInput(const std::string &input, const std::string &key);

    bool handleLightbarSelection(const std::string &input);

    bool handlePullDownHotKeys(const MenuOption &m, const bool &is_enter, bool &stack_reassignment);

    std::vector<std::string> getListOfMenuPrompts();

    std::string getRandomMenuPrompt();

    bool processMenuOptions(const std::string &input);

    void handlePullDownInput(const std::string &character_buffer, const bool &is_utf8);

    void handleFieldInput(const std::string &character_buffer);

    void menuInput(const std::string &character_buffer, const bool &is_utf8);

    void menuYesNoBarInput(const std::string &character_buffer, const bool &is_utf8);

private:
    void executeEachCommands();
};

#endif
