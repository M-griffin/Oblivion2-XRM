#include "menu_base.hpp"

#define DEBUG_MENU_CHAIN

#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <random>
#include <cassert>

#include "acs_base.hpp"
#include "data-sys/yml_menu.hpp"
#include "data-sys/yml_menu_prompt.hpp"
#include "model-sys/config.hpp"
#include "model-sys/users.hpp"
#include "model-sys/context.hpp"
#include "screen_ansi_proc.hpp"
#include "util_dir.hpp"
#include "io_session.hpp"
#include "util_log.hpp"
#include "io_common.hpp"
#include "io_encoding.hpp"
#include "tcp_session.hpp"

#include "menu_system.hpp"

MenuBase::MenuBase(Context &ctx)
    : m_log(UtilLog::getInstance())
      , m_ctx(ctx)
      , m_cmdChainExecutor(*static_cast<MenuSystem *>(this))
      , m_use_hotkey(false)
      , m_baseState(BaseState::MENU_INPUT)
      , m_active_pulldownID(0)
      , m_pulldown_reentrace_flag(false)
      , m_is_active_pulldown_menu(false)
      , m_logoff(false)
      , m_is_active(false)
      , m_failFlag(false)
      , m_pendingMenuJump(false)
      , m_pendingJumpMode(MenuJumpMode::Normal) {
    m_log.log(UtilLog::LogLevel::Console, "MenuBase()");
}

MenuBase::~MenuBase() {
    m_log.log(UtilLog::LogLevel::Console, "~MenuBase()");
    m_execute_callback.clear();
    m_loaded_pulldown_options.clear();
}

void MenuBase::baseProcessAndDeliver(std::string data) {
    m_log.log(UtilLog::LogLevel::Debug, "MenuSystem() - baseProcessAndDeliver");
    m_ctx.getScreenAnsi().parseTextToBuffer(data);
    m_ctx.getSessionWrite().send(data);
}

void MenuBase::clearMenuPullDownOptions() {
    if (!m_loaded_pulldown_options.empty()) {
        m_loaded_pulldown_options.clear();
        m_loaded_pulldown_options.shrink_to_fit();
    }

    m_ctx.getScreenAnsi().clearPullDownBars();
}

bool MenuBase::checkMenuAcsAccess(const Menu &menu) {
    AcsBase acs;
    bool result = acs.validateAcsString(
        menu.menu_acs_string,
        m_ctx.getUser()
    );

    if (!result) {
        m_failFlag = true;
    }

    return result;
}

void MenuBase::buildMenuOptionsFromAcs() {
    auto it = m_menu_info.menu_options.begin();
    auto end = m_menu_info.menu_options.end();
    std::vector<MenuOption> new_options;
    AcsBase acs;

    for (; it != end; it++) {
        if (acs.validateAcsString(
            (*it).acs_string,
            m_ctx.getUser())) {
            new_options.push_back(*it);
        }
    }

    // Swap Validated Options with Existing.
    m_menu_info.menu_options.swap(new_options);
}

void MenuBase::injectFirstCommands() {

    m_failFlag = false;
    std::vector<MenuOption> first;
    AcsBase acs;

    for (auto &m : m_menu_info.menu_options) {
        if (m.menu_key == "FIRSTCMD") {
            if (acs.validateAcsString(m.acs_string, m_ctx.getUser())) {
                first.push_back(m);
            }
        }
    }

    if (!first.empty() && !m_cmdChainExecutor.isActive()) {
        m_cmdChainExecutor.start(first);
    }
}

void MenuBase::requestMenuJump(const std::string &targetMenu, MenuJumpMode mode) {
    m_log.log(UtilLog::LogLevel::Info, "MenuBase() - requestMenuJump", targetMenu, MenuJumpModeToString(mode));
    std::string menu = m_ctx.getIoCommon().toLower(targetMenu);

    // FIRSTCMD SUPPRESSION
    if (mode == MenuJumpMode::SkipFirstCmd ||
        mode == MenuJumpMode::PreviousNoFirst) {
    }

    // PREVIOUS MENU HANDLING
    if (mode == MenuJumpMode::PreviousNoFirst) {
        if (!m_previous_menu.empty()) {
            menu = m_previous_menu;
        } else {
            // Defensive fallback
            menu = resolveFallbackMenu();
        }
    }

    // FALLBACK STACK HANDLING
    switch (mode) {
        case MenuJumpMode::PushCurrent:
            if (!m_current_menu.empty()) {
                m_menuStack.push_back(m_current_menu);
            }
            break;

        case MenuJumpMode::PushStarting:
            // Only set the Starting Menu the first time if Empty.
            if (m_starting_menu.empty()) {
                m_starting_menu = m_current_menu;
            }

            // Starting Menu should be immutable, and stick to main, or top etc.
            // once logon is completed.
            m_menuStack.push_back(m_starting_menu);
            break;

        case MenuJumpMode::PopFallback:
            menu = resolveFallbackMenu();
            break;

        default:
            break;
    }

    // Setup State and Stack.
    if (m_menuStack.size() >= 5) m_menuStack.pop_front();

    m_pendingMenuJump = true;
    m_pendingMenuName = menu;
    m_pendingJumpMode = mode;
}

std::string MenuBase::resolveFallbackMenu() {
    m_log.log(UtilLog::LogLevel::Info, "MenuBase() - resolveFallbackMenu");

    if (!m_menuStack.empty()) {
        std::string prev = m_menuStack.back();
        m_menuStack.pop_back();
        m_log.log(UtilLog::LogLevel::Info, "MenuBase() - resolveFallbackMenu prev=", prev);
        return prev;
    }

    if (!m_menu_info.menu_fall_back.empty()) {
        m_log.log(UtilLog::LogLevel::Info, "MenuBase() - resolveFallbackMenu=", m_menu_info.menu_fall_back);
        return m_menu_info.menu_fall_back;
    }

    m_log.log(UtilLog::LogLevel::Info, "MenuBase() - resolveFallbackMenu empty!");
    if (!m_starting_menu.empty()) {
        return m_starting_menu;
    }

    // Hard Reset to Matrix or Main Menu
    if (!m_ctx.getSessionWrite().isAuthorized())
        return "matrix";

    return "main";
}

std::string MenuBase::processTopGenericTemplate(const std::string &screen) {
    /**
     * When we get 2J alone, it clears but leaves cursor.
     * In most cases we need to add a pre-home cursor!
     */
    std::string new_screen = screen;
    std::string::size_type index = 0;

    while (index != std::string::npos) {
        index = new_screen.find("\x1b[2J", index);

        if (index != std::string::npos) {
            new_screen.replace(index, 4, "\x1b[1;1H\x1b[2J");
            // Increment past previous replacement.
            index += 9;
        }
    }

    return new_screen;
}

std::string MenuBase::processMidGenericTemplate(const std::string &screen) {
    // Use a Local Ansi Parser for Parsing Menu Template with Mid.
    ScreenAnsiProc ansi_process;
    ansi_process.resize(
        m_ctx.getTelnet().getTermRows(),
        m_ctx.getTelnet().getTermCols());

    std::string output_screen;
    std::string new_screen(screen);
    std::string::size_type index = 0;

    while (index != std::string::npos) {
        index = new_screen.find('\r', index);

        if (index != std::string::npos) {
            new_screen.erase(index, 1);
        }
    }

    index = 0;
    while (index != std::string::npos) {
        index = new_screen.find('\n', index);

        if (index != std::string::npos) {
            new_screen.erase(index, 1);
        }
    }

    // Clear All Mappings
    m_ctx.getIoSession().clearAllMCIMapping();

    // Build a single code map that can be reused.
    std::vector<CodeMapType> code_map = m_ctx.getIoSession().pipe2genericCodeMap(new_screen);

    // Loop the code map and determine the number of unique columns for parsing.
    int key_columns = 0;
    int des_columns = 0;

    // Loop codes and get max number of columns per code.
    for (unsigned int i = 0; i < code_map.size(); i++) {
        auto &map = code_map[i];

        if (map.m_code[1] == 'K') {
            ++key_columns;
        }

        if (map.m_code[1] == 'D') {
            ++des_columns;
        }
    }

    // No codes found in ansi, or invalid combination exit!
    if (key_columns == 0 || key_columns != des_columns) {
        return output_screen;
    }

    // Loop all menu options, and pass the number of columns at a time.
    int column = 1;
    std::string key, value;
    std::string::size_type idx;

    for (unsigned int i = 0; i < m_menu_info.menu_options.size(); i++) {
        auto &m = m_menu_info.menu_options[i];

        // Skip Options that are Automatic Execution, or Stacked with no name and hidden
        if (m.menu_key == "FIRSTCMD" || m.menu_key == "EACH" ||
            m.name.empty() || m.hidden) {
            continue;
        }

        // Build Key/Value for Menu Key
        key = "|K" + std::to_string(column);

        // Clean any wildcard from menu key.
        idx = m.menu_key.find("*");

        if (idx != std::string::npos) {
            value = m.menu_key.substr(0, idx);
        } else {
            value = m.menu_key;
        }

        m_ctx.getIoSession().addMCIMapping(key, value);

        // Build Key/Value for Menu Description
        key = "|D" + std::to_string(column);
        value = m.name;
        m_ctx.getIoSession().addMCIMapping(key, value);

        if (column % key_columns == 0) {
            // Process template menu row and all columns added.
            output_screen += m_ctx.getIoSession().parseCodeMapGenerics(new_screen, code_map);
            output_screen += "\x1b[D\r\n";
            column = 0;
        }

        ++column;
    }

    // Process any remaining not caught in offset.
    if (m_ctx.getIoSession().getMCIMappingCount() > 0) {
        output_screen += m_ctx.getIoSession().parseCodeMapGenerics(new_screen, code_map);
        output_screen += "\x1b[D\r\n";
    }

    // Clear Code map.
    std::vector<CodeMapType>().swap(code_map);
    ansi_process.parseTextToBuffer(output_screen);

    // Return with no clear screen, since this is a mid ansi.
    return ansi_process.getScreenFromBuffer(false);
}

std::string MenuBase::processGenericScreens() {
    std::string top_screen = m_ctx.getIoCommon().readAnsi("GENSRT.ANS");
    std::string mid_screen = m_ctx.getIoCommon().readAnsi("GENMID.ANS");
    std::string bot_screen = m_ctx.getIoCommon().readAnsi("GENEND.ANS");
    std::string screen_output;

    // Add the Top section of the template
    // Do a simple MCI Code replace for title
    // |TI - Menu Title
    std::string::size_type idx = top_screen.find("|TI");

    if (idx != std::string::npos) {
        top_screen.replace(
            idx,
            3,
            m_menu_info.menu_title
        );
    }

    screen_output += processTopGenericTemplate(top_screen);

    // |K? - key,  |D? - Description
    //|K1 |D1   |K2 |D2  |K3 |D3 ...
    screen_output += processMidGenericTemplate(mid_screen);
    screen_output += bot_screen;
    return screen_output;
}

std::string MenuBase::setupYesNoMenuInput(const std::string &menu_prompt, std::vector<CodeMapType> &code_map) {
    m_baseState = BaseState::MENU_YESNO_BAR;
    clearMenuPullDownOptions();

    // Then feed though and return the updated string.
    std::string prompt_string = m_ctx.getIoSession().parseCodeMapGenerics(menu_prompt, code_map);
    std::string display_prompt = moveStringToBottom(prompt_string);

    // Translate Pipe Coles to ESC Sequences prior to parsing to keep
    // String length calculations.
    display_prompt = m_ctx.getIoSession().pipe2ansi(display_prompt);

    std::string yesNoBars = getDefaultColor() + "|01";
    yesNoBars += getDefaultInputColor() + getDefaultInverseColor() + "%01\x1b[0m";
    yesNoBars += " ";
    yesNoBars += getDefaultColor() + "|02";
    yesNoBars += getDefaultInputColor() + getDefaultInverseColor() + "%02\x1b[0m";
    yesNoBars.insert(0, display_prompt);

    // Parse the Screen to the Screen Buffer.
    m_ctx.getScreenAnsi().parseTextToBuffer(yesNoBars);

    // Screen to String so it can be processed.
    m_ctx.getScreenAnsi().screenBufferToString();

    // Process buffer for PullDown Codes.
    // only if we want result, ignore.., result just for testing at this time!
    std::string result = m_ctx.getScreenAnsi().screenBufferParse();

    // Update Light bars, by default they have no names for YES/NO/Continue prompts.
    for (unsigned int i = 0; i < m_menu_info.menu_options.size(); i++) {
        auto &m = m_menu_info.menu_options[i];

        // TODO Default setup for Yes No with default to Yes!
        // Need to add to prompts file for customization!!
        if (i == 0) {
            m.pulldown_id = 1;
            m.name = "  Yes  ";
        } else if (i == 1) {
            m.pulldown_id = 2;
            m.name = "  No  ";
        } else {
            m.pulldown_id = 3;
            m.name = "  quit  ";
        }

        m_loaded_pulldown_options.push_back(m);
    }

    // Now Build the Light bars
    std::string light_bars = buildLightBars();
    display_prompt.append(light_bars);

    // Hide Cursor on light bars
    display_prompt.append("\x1b[?25l");
    return display_prompt;
}

std::string MenuBase::getDefaultColor() {
    return m_ctx.getIoSession().pipeColors(m_ctx.getUser().sRegColor);
}

std::string MenuBase::getDefaultInputColor() {
    return m_ctx.getIoSession().pipeColors(m_ctx.getUser().sInputColor);
}

std::string MenuBase::getDefaultInverseColor() {
    return m_ctx.getIoSession().pipeColors(m_ctx.getUser().sInverseColor);
}

void MenuBase::loadMenuDefinition(const std::string &) {

    // Note, Incoming Menu Name is not used, goes by m_current_menu.
    m_log.log(UtilLog::LogLevel::Info, "MenuBase() - loadMenuDefinition=", m_current_menu);

    // Default PullDown ID, reset.
    m_active_pulldownID = 0;

    // Reset on First Load.
    m_is_active_pulldown_menu = false;

    m_log.log(UtilLog::LogLevel::Debug, "MenuBase() - readInMenuData");

    clearMenuPullDownOptions();

    constexpr int MAX_ATTEMPTS = 5;
    int attempts = 0;

    while (attempts++ < MAX_ATTEMPTS) {
        Menu candidate;
        MenuDao dao(candidate, m_current_menu, GLOBAL_MENU_PATH);

        m_log.log(UtilLog::LogLevel::Debug,
                  "Attempting to load menu:", m_current_menu);

        // Menu file exists?
        if (!dao.fileExists()) {
            m_log.log(UtilLog::LogLevel::Warn,
                      "Menu file missing:", m_current_menu);

            std::string fallback = resolveFallbackMenu();
            if (fallback.empty()) {
                m_log.log(UtilLog::LogLevel::Error,
                          "No fallback menu available");
                assert(false);
            }

            m_current_menu = fallback;
            continue;
        }

        // Load menu
        if (!dao.loadMenu()) {
            m_failFlag = true;
        }

        // ACS check
        if (!checkMenuAcsAccess(candidate)) {
            m_log.log(UtilLog::LogLevel::Warn,
                      "Menu ACS denied:", m_current_menu);

            std::string fallback = resolveFallbackMenu();
            if (fallback.empty()) {
                m_log.log(UtilLog::LogLevel::Error,
                          "No fallback menu after ACS failure");
                assert(false);
            }

            m_current_menu = fallback;
            continue;
        }

        // SUCCESS
        m_menu_info = candidate;
        buildMenuOptionsFromAcs();

        m_log.log(UtilLog::LogLevel::Info,
                  "Menu loaded successfully:", m_menu_info.menu_name);

        return;
    }

    // TODO FIXME : change menu coming in, from current menu, now doesn't work correctly.
    // Safety net
    m_log.log(UtilLog::LogLevel::Error,
              "Menu resolution exceeded maximum attempts ", m_current_menu);
    assert(false);
}

void MenuBase::reviewMenuDefinition(const std::string &menuName, Menu &candidate) {

    // Unlike Load Definition, this is from Menu Editor, so passed in Menu name is accurate.
    m_log.log(UtilLog::LogLevel::Info, "MenuBase() - reviewMenuDefinition=", menuName);

    // Default PullDown ID, reset.
    m_active_pulldownID = 0;

    // Reset on First Load.
    m_is_active_pulldown_menu = false;

    m_log.log(UtilLog::LogLevel::Debug, "MenuBase() - readInMenuData");

    clearMenuPullDownOptions();

    constexpr int MAX_ATTEMPTS = 5;
    int attempts = 0;

    // If access issues rollback to fallback as would normally.
    while (attempts++ < MAX_ATTEMPTS) {

        m_log.log(UtilLog::LogLevel::Debug,
                  "Attempting to review menu:", menuName);

        // ACS check
        if (!checkMenuAcsAccess(candidate)) {
            m_log.log(UtilLog::LogLevel::Warn,
                      "Menu ACS denied:", menuName);

            std::string fallback = resolveFallbackMenu();
            if (fallback.empty()) {
                m_log.log(UtilLog::LogLevel::Error,
                          "No fallback menu after ACS failure");
                assert(false);
            }

            m_current_menu = fallback;
            continue;
        }

        // SUCCESS
        m_menu_info = candidate;
        buildMenuOptionsFromAcs();

        // On Success, Make sure we set the proper Loaded.
        m_current_menu = m_menu_info.menu_name;

        m_log.log(UtilLog::LogLevel::Info,
                  "Menu loaded successfully:", m_menu_info.menu_name);

        return;
    }

    // Safety net
    m_log.log(UtilLog::LogLevel::Error,
              "Menu resolution exceeded maximum attempts");
    assert(false);
}

void MenuBase::prepareMenuState(MenuLoadReason reason) {
    m_line_buffer.clear();
    m_baseState = BaseState::MENU_INPUT;
    m_use_hotkey = false;
    m_is_active = true;

    m_pulldown_reentrace_flag = false;
    m_is_active_pulldown_menu = false;
    m_active_pulldownID = 0;
    m_loaded_pulldown_options.clear();
    m_loaded_pulldown_options.shrink_to_fit();

}

bool MenuBase::shouldExecuteFirst(MenuLoadReason reason) {
    if (reason == MenuLoadReason::Initial)
        return true;

    if (reason == MenuLoadReason::Redisplay)
        return false;

    if (reason == MenuLoadReason::Jump) {
        if (m_pendingJumpMode == MenuJumpMode::SkipFirstCmd)
            return false;

        if (m_pendingJumpMode == MenuJumpMode::PreviousNoFirst)
            return false;
    }

    return true;
}

void MenuBase::enterMenu(const std::string &menuName, MenuLoadReason reason) {
    m_log.log(UtilLog::LogLevel::Info,
              "enterMenu menu={} reason={}",
              menuName, MenuLoadReasonToString(reason));

    m_pendingMenuJump = false;
    m_baseState = BaseState::MENU_INPUT;

    loadMenuDefinition(menuName);
    prepareMenuState(reason);

    if (m_menu_info.menu_options.empty()) {
        m_log.log(UtilLog::LogLevel::Error,
                  "Menu has no menu_options", m_current_menu);
        return;
    }

    if (shouldExecuteFirst(reason)) {
        // Injects and Executes First Commands.
        injectFirstCommands();

        if (m_cmdChainExecutor.isActive())
            return;
    }

    // Handle Menu Rendering Below Here, Pull Down And/Or Generic Templates.
    if (handleSpecialPulldownModes()) {
        return;
    }

    // If first command execution resulted in jumping to new menu,
    // then we want to skip display, and return for transition.
    if (m_pendingMenuJump) {
        return;
    }

    std::string raw_buffer = loadMenuScreen();
    std::string output = m_ctx.getIoSession().pipe2ansi(raw_buffer);

    setupPulldownsAndLightbars(raw_buffer, output);
    baseProcessAndDeliver(output);

    if (reason != MenuLoadReason::Redisplay) {
        executeEachCommands();
    }

    if (!m_cmdChainExecutor.isActive()) {
        output.clear();
        output = loadMenuPrompt();
        baseProcessAndDeliver(output);
    }
}

void MenuBase::reviewMenu(const std::string &menuName, Menu &candidate, MenuLoadReason reason) {
    m_log.log(UtilLog::LogLevel::Info,
              "displayMenu menu={} reason={}",
              menuName, MenuLoadReasonToString(reason));

    m_pendingMenuJump = false;
    m_baseState = BaseState::MENU_INPUT;

    reviewMenuDefinition(menuName, candidate);
    prepareMenuState(reason);

    if (m_menu_info.menu_options.empty()) {
        m_log.log(UtilLog::LogLevel::Error,
                  "Menu has no menu_options", m_current_menu);
        return;
    }

    if (shouldExecuteFirst(reason)) {
        // Injects and Executes First Commands.
        injectFirstCommands();

        if (m_cmdChainExecutor.isActive())
            return;
    }

    // Handle Menu Rendering Below Here, Pull Down And/Or Generic Templates.
    if (handleSpecialPulldownModes()) {
        return;
    }

    // If first command execution resulted in jumping to new menu,
    // then we want to skip display, and return for transition.
    if (m_pendingMenuJump) {
        return;
    }

    std::string raw_buffer = loadMenuScreen();
    std::string output = m_ctx.getIoSession().pipe2ansi(raw_buffer);

    setupPulldownsAndLightbars(raw_buffer, output);
    baseProcessAndDeliver(output);

    // Maybe we want to execute on Preview of Menu?!?? No for now.
    if (reason != MenuLoadReason::Redisplay) {
        executeEachCommands();
    }

    if (!m_cmdChainExecutor.isActive()) {
        output.clear();
        output = loadMenuPrompt();
        baseProcessAndDeliver(output);
    }
}

bool MenuBase::handleSpecialPulldownModes() {
    // N = [Yes / No] lightbar prompt
    if (m_menu_info.menu_pulldown_file.size() == 1 &&
        toupper(m_menu_info.menu_pulldown_file[0]) == 'N') {
        // Do we run firstcmds before displaying text prompt in pulldown menu?
        baseProcessAndDeliver(
            parseMenuPromptString(m_menu_info.menu_prompt)
        );

        m_is_active_pulldown_menu = true;
        return true;
    }

    // ::X logic can live here later
    return false;
}

void MenuBase::setupPulldownsAndLightbars(const std::string &raw_buffer, std::string &output) {
    if (m_menu_info.menu_pulldown_file.empty() ||
        !m_ctx.getSessionWrite().isAnsi()) {
        m_log.log(UtilLog::LogLevel::Info, "setupPulldownsAndLightbars isEmpty?",
                  m_menu_info.menu_pulldown_file.empty(),
                  "isAnsi?", m_ctx.getSessionWrite().isAnsi());

        m_is_active_pulldown_menu = false;
        return;
    }

    m_is_active_pulldown_menu = true;

    std::vector<int> pull_down_ids;
    m_loaded_pulldown_options.clear();

    for (auto &opt: m_menu_info.menu_options) {
        if (opt.pulldown_id > 0) {
            pull_down_ids.push_back(opt.pulldown_id);
            m_loaded_pulldown_options.push_back(opt);
        }
    }

    if (pull_down_ids.empty()) {
        m_is_active_pulldown_menu = false;
        return;
    }

    m_active_pulldownID =
            *std::min_element(pull_down_ids.begin(), pull_down_ids.end());

    // Hide cursor
    output += "\x1b[?25l";

    // Parse screen for pull-down anchors
    m_ctx.getScreenAnsi().parseTextToBuffer(raw_buffer);
    m_ctx.getScreenAnsi().screenBufferToString();
    m_ctx.getScreenAnsi().screenBufferParse();

    output += buildLightBars();
}

void MenuBase::redisplayMenuScreen() {
    m_log.log(UtilLog::LogLevel::Info, "MenuBase() - redisplayMenuScreen", "chain Active=",m_cmdChainExecutor.isActive());

    // If we are in a chain, supress re-display.
    if (m_cmdChainExecutor.isActive() || m_cmdChainExecutor.isWaiting())
        return;


    // Read in the Menu ANSI
    std::string buffer = loadMenuScreen();
    std::string output = m_ctx.getIoSession().pipe2ansi(buffer);

    if (m_is_active_pulldown_menu) {
        // Parse the Screen to the Screen Buffer.
        m_ctx.getScreenAnsi().parseTextToBuffer(buffer);

        // Screen to String so it can be processed.
        m_ctx.getScreenAnsi().screenBufferToString();

        // Process buffer for PullDown Codes.
        // only if we want result, ignore.., result just for testing at this time!
        std::string result = m_ctx.getScreenAnsi().screenBufferParse();

        // Now Build the Light bars with Hidden Cursor.
        std::string light_bars = "\x1b[?25l";
        light_bars += buildLightBars();

        // add and write out.
        output.append(light_bars);
    }

    // Load the Menu prompt
    // Only skip prompt

    if (!m_cmdChainExecutor.isActive()) {
        executeEachCommands();

        if (!m_cmdChainExecutor.isActive())
            output += loadMenuPrompt();
    }

    baseProcessAndDeliver(output);
}

std::string MenuBase::parseMenuPromptString(const std::string &prompt_string) {
    m_log.log(UtilLog::LogLevel::Info, "MenuBase() - parseMenuPromptString");

    // Color Sequences and NewLine
    m_ctx.getIoSession().clearAllMCIMapping();
    m_ctx.getIoSession().addMCIMapping("^R", m_ctx.getCfg().default_color_regular);
    m_ctx.getIoSession().addMCIMapping("^S", m_ctx.getCfg().default_color_stat);
    m_ctx.getIoSession().addMCIMapping("^P", m_ctx.getCfg().default_color_prompt);
    m_ctx.getIoSession().addMCIMapping("^E", m_ctx.getCfg().default_color_input);
    m_ctx.getIoSession().addMCIMapping("^V", m_ctx.getCfg().default_color_inverse);
    m_ctx.getIoSession().addMCIMapping("^X", m_ctx.getCfg().default_color_box);
    m_ctx.getIoSession().addMCIMapping("^M", "\r\n");

    std::vector<CodeMapType> code_map = m_ctx.getIoSession().pipe2promptCodeMap(prompt_string);
    std::string output(prompt_string);

    output = m_ctx.getIoSession().parseCodeMapGenerics(output, code_map);

    // Loop codes and picked out ending control code.
    bool match_found = false;

    for (unsigned int i = 0; i < code_map.size(); i++) {
        auto &map = code_map[i];

        // Control Codes are in Group 2
        if (map.m_match == 2) {
            switch (map.m_code[0]) {

                case '\\':
                    m_active_pulldownID = 1; // Default to YES
                    output = setupYesNoMenuInput(output, code_map);
                    match_found = true;
                    break;

                case '/':
                    m_active_pulldownID = 2; // Default to NO
                    output = setupYesNoMenuInput(output, code_map);
                    match_found = true;
                    break;

                case '=':
                    m_active_pulldownID = 1; // Yes/No/Quit - Default Yes
                    output = setupYesNoMenuInput(output, code_map);
                    match_found = true;
                    break;

                case '|':
                    m_active_pulldownID = 2; // Yes/No/Quit - Default No
                    output = setupYesNoMenuInput(output, code_map);
                    match_found = true;
                    break;

                case '@':
                    m_active_pulldownID = 3; // Yes/No/Quit - Default Quit
                    output = setupYesNoMenuInput(output, code_map);
                    match_found = true;
                    break;

                // Handle yes /no /quit
                default:
                    break;
            }
        }

        // Found code, return.
        if (match_found) {
            break;
        }
    }

    // Then we feed it through again to handle colors replacements.
    return output;
}

std::string MenuBase::loadMenuScreen() {
    m_log.log(UtilLog::LogLevel::Info, "MenuBase() - loadMenuScreen");

    // Check Pull down FileID
    std::string screen_data;

    // NOTES: check for themes here!!!
    // also  if (m_menu_session_data->m_is_use_ansi), if not ansi, then maybe no pull down, or light bars!
    bool use_ansi = m_ctx.getSessionWrite().isAnsi();

    if (m_menu_info.menu_pulldown_file.empty() || !use_ansi) {
        std::string screen_file = m_menu_info.menu_help_file;
        m_log.log(UtilLog::LogLevel::Debug, "MenuBase() - loadMenuScreen menu_pulldown_file.empty() or Not ANSI",
                  screen_file);

        // Load ansi by Menu Name, remove .MNU and Add .ANS, maybe .UTF for utf8 native?
        if (use_ansi) {
            screen_file.append(".ANS");
        } else {
            screen_file.append(".ASC");
        }

        // Make all screens uppercase, handle Unicode names.
        screen_file = m_ctx.getIoCommon().toUpper(screen_file);

        // if file doesn't exist, then use generic template
        if (m_ctx.getIoCommon().fileExists(screen_file)) {
            m_log.log(UtilLog::LogLevel::Debug, "MenuBase() - loadMenuScreen readinAnsi", screen_file);
            screen_data = m_ctx.getIoCommon().readAnsi(screen_file);
        } else {
            // Load and use generic template.
            // These are GENTOP. GENMID, GENBOT.ANS
            m_log.log(UtilLog::LogLevel::Debug, "MenuBase() - loadMenuScreen processGenericScreens", screen_file);
            screen_data = processGenericScreens();
        }
    } else {
        // Pull down file should have .ANS extension.
        std::string screen_file = m_menu_info.menu_pulldown_file;

        m_log.log(UtilLog::LogLevel::Info, "MenuBase() - loadMenuScreen menu_pulldown_file.empty() or Not ANSI2",
                  screen_file);

        // Screen File(s) are Uppercase.
        screen_file = m_ctx.getIoCommon().toUpper(screen_file);

        // Otherwise use the Pull down menu name from the menu.
        // if file doesn't exist, then use generic template
        if (m_ctx.getIoCommon().fileExists(screen_file)) {
            m_log.log(UtilLog::LogLevel::Info, "MenuBase() - loadMenuScreen readinAnsi2", screen_file);
            screen_data = m_ctx.getIoCommon().readAnsi(screen_file);
        } else {
            // Load and use generic template, fallback if file is missing.
            // These are GENTOP. GENMID, GENBOT.ANS
            screen_data = processGenericScreens();
            m_log.log(UtilLog::LogLevel::Info, "MenuBase() - loadMenuScreen processGenericScreens2", screen_file);
        }
    }

    m_log.log(UtilLog::LogLevel::Debug, "MenuBase() - loadMenuScreen screen_data for display.", screen_data);
    return screen_data;
}

std::string MenuBase::buildLightBars() {
    m_log.log(UtilLog::LogLevel::Info, "MenuBase() - buildLightBars");

    // Test setup and display lightbars
    std::string light_bars;
    bool active_lightbar = false;

    for (unsigned int i = 0; i < m_menu_info.menu_options.size(); i++) {
        auto &m = m_menu_info.menu_options[i];

        // Always start on Initial or first indexed light bar.
        // Might need to verify if we need to check for lowest ID, and start on that!
        if (m_active_pulldownID > 0 && m_active_pulldownID == m.pulldown_id) {
            active_lightbar = true;
        }

        if (m.pulldown_id > 0) {
            // Parse for X/Y Position and colors
            light_bars.append(m_ctx.getScreenAnsi().buildPullDownBars(m.pulldown_id, active_lightbar));
            active_lightbar = false;

            // Add the Option Description
            light_bars.append(m.name);

            // Clear and reset so we end the light bar
            light_bars.append("\x1b[0m");
        }
    }

    return light_bars;
}

bool MenuBase::executeWithAcs(const MenuOption &opt) {
    m_log.log(UtilLog::LogLevel::Info, "MenuBase() - executeWithAcs");
    AcsBase acs;

    // 1. Check Access First
    if (!acs.validateAcsString(opt.acs_string, m_ctx.getUser())) {
        m_failFlag = true;
        return false;
    }

    // 2. Parity Check: Is this a multi-command action string?
    // Example: opt.command_key = " *U", opt.command_string = "5; -/MAIN"
    // We need to split the command_string if it contains semicolons.
    if (opt.command_string.find(';') != std::string::npos) {
        std::vector<MenuOption> chain;
        std::stringstream ss(opt.command_string);
        std::string segment;

        while (std::getline(ss, segment, ';')) {
            if (segment.empty()) continue;

            MenuOption chainedOpt = opt;

            // Logic Parity: In Pascal, only the first command in a chain
            // usually kept the original OpCode. Subsequent segments
            // might be full standalone commands (e.g., "-/MAIN").
            if (segment.size() >= 2 && (segment[0] == '-' || segment[0] == '*' || segment[0] == '/')) {
                // This segment looks like a full Command Key + String pair
                chainedOpt.command_key = segment.substr(0, 2);
                chainedOpt.command_string = (segment.size() > 2) ? segment.substr(2) : "";
            } else {
                // This segment is just a new argument for the original OpCode
                chainedOpt.command_string = segment;
            }
            chain.push_back(chainedOpt);
        }

        // Start the queue-based execution
        m_cmdChainExecutor.start(chain);
        return true;
    }

    // 3. Single command execution
    m_failFlag = false;
    return executeMenuOptions(opt);
}

std::vector<std::string> MenuBase::getListOfMenuPrompts() {
    std::vector<std::filesystem::path> result_set =
            UtilDir::listCaseInsensitive(GLOBAL_MENU_PROMPT_PATH, ".yaml");

    std::vector<std::string> result_list;

    // check result set, if no menu then return gracefully.
    if (result_set.empty()) {
        m_log.log(UtilLog::LogLevel::Error, "No Menu Prompt .yaml files found!");
        return result_list;
    }

    // Sort Menu Prompt's in ascending order
    std::sort(result_set.begin(), result_set.end());

    for (auto i: result_set) {
        result_list.push_back(i.stem().string());
    }

    return result_list;
}

std::string MenuBase::getRandomMenuPrompt() {
    std::vector<std::string> result_set = getListOfMenuPrompts();

    // check result set, if no menu then return gracefully.
    if (result_set.empty()) {
        return "";
    }

    //generator initialized with seed from time.
    static std::mt19937_64 generator{static_cast<unsigned int>(std::time(nullptr))};
    int set_size = result_set.size() - 1;

    //the range is inclusive, so this produces numbers in range [0, 10), same as before
    std::uniform_int_distribution<> dist{0, set_size};

    int randomNumber = dist(generator);
    return result_set[randomNumber];
}

std::string MenuBase::loadMenuPrompt() {
    m_log.log(UtilLog::LogLevel::Info, "MenuBase() - loadMenuPrompt");

    // Display Menu Prompt if it exists, right now it's default
    // later on add users selected.  This is just a test!
    std::string prompt;
    long record_id = 0;
    int term_rows = 0;
    int node_number = 0;
    std::string prompt_name;

    record_id = m_ctx.getUser().iId;
    term_rows = m_ctx.getTelnet().getTermRows();
    node_number = m_ctx.getSessionWrite().getNodeNumber();
    prompt_name = m_ctx.getUser().sMenuPromptName;

    if (record_id != -1) {
        prompt = prompt_name;
    }

    // If users Menu Prompt is blank, then grab random menu prompt!
    if (prompt.empty()) {
        prompt = getRandomMenuPrompt();
    }

    MenuPromptDao mnu_prompt(m_menu_prompt, prompt, GLOBAL_MENU_PROMPT_PATH);
    bool is_loaded = mnu_prompt.loadMenuPrompt();

    // Don't display prompts on Pull down menu's.
    if (!m_is_active_pulldown_menu && is_loaded) {
        std::string prompt_display;
        // Used Screen Rows is not reliable across menu's, need to look into this more!
        //int screen_rows = m_ctx.getScreenAnsi().getMaxRowsUsedOnScreen();

        // For Now use defaults when Term height is 24 (Default) or 25 and greater
        // Usually menu's themselves are not going to be higher 25
        // Properly can also overwrite and have MCU position Codes in them.

        int prompt_lines = 0; // with rumors for now, till we update each commands. otherwise 0
        if (!m_menu_prompt.data_line1.empty()) ++prompt_lines;
        if (!m_menu_prompt.data_line2.empty()) ++prompt_lines;
        if (!m_menu_prompt.data_line3.empty()) ++prompt_lines;

        // Default ANSI row start: 1 above menu or terminal rows minus prompt
        //int start_row = std::max(1, term_rows - prompt_lines - 1);
        //prompt_display = "\x1b[?25h\x1b[" + std::to_string(start_row) + ";1H";


        if (term_rows == 24) {
            prompt_display = "\x1b[?25h\x1b[21;1H";
        } else if (term_rows > 24) {
            prompt_display = "\x1b[?25h\x1b[22;1H";
        }

        //prompt_display = "\x1b[?25h\x1b[" + std::to_string(screen_rows) + ";1H\r\n";
        prompt_display += getDefaultColor();

        // Insert Rumor Test here 1 line above enu prompt!
        prompt_display += "\"Mock Rumor: Oblivion/2 will live once again!\r\n";
        // Start of Menu prompt
        prompt_display += m_menu_prompt.data_line1 + "\r\n";
        prompt_display += m_menu_prompt.data_line2 + "\r\n";
        prompt_display += m_menu_prompt.data_line3;

        // Clear All Mappings
        m_ctx.getIoSession().clearAllMCIMapping();

        // Parse Prompt for Menu Title here, let pip2ansi parse standard codes.
        m_ctx.getIoSession().addMCIMapping("|MN", m_menu_info.menu_prompt);
        m_ctx.getIoSession().addMCIMapping("|TL", "1440"); // Time Left {Not Implemented Yet}

        // Get current time
        auto now = std::chrono::system_clock::now();
        std::time_t t = std::chrono::system_clock::to_time_t(now);

        // Convert to local time
        std::tm tm{};
#if defined(_WIN32)
        localtime_s(&tm, &t); // Windows
#else
        localtime_r(&t, &tm);   // Linux / macOS
#endif

        std::ostringstream oss;
        oss << std::put_time(&tm, "%m/%d/%Y %I:%M %p");

        m_ctx.getIoSession().addMCIMapping("|TM", oss.str()); // Time Now
        oss.clear();

        m_ctx.getIoSession().addMCIMapping("|NN", std::to_string(node_number));

        // Legacy Note:
        // SysOps may place %%filename.ext anywhere in the menu prompt
        // to display filename.ext from the prompts' directory. Use the
        // %MN code to display the Menu's Name in Prompt (in filename.ext).
        // Set the Prompts directory in the CONFIG.
        m_ctx.getIoSession().addMCIMapping("%MN", m_menu_info.menu_prompt);

        // Encode Prompt First prior to passing to Pipe2Ansi/CodeMapping.
        const IoEncoding encode;
        std::string encoded = encode.utf8Encode(prompt_display);
        return m_ctx.getIoSession().pipe2ansi(encoded);
    }

    prompt = "";
    if (!m_menu_info.menu_prompt.empty()) {
        m_log.log(UtilLog::LogLevel::Debug, "Use Default Prompt String in Menu.");
        prompt = "\x1b[?25h" + m_menu_info.menu_prompt;

        // Encode Prompt First prior to passing to Pipe2Ansi/CodeMapping.
        const IoEncoding encode;
        std::string encoded = encode.utf8Encode(prompt);
        return m_ctx.getIoSession().pipe2ansi(encoded);
    }

    // Otherwise Noting loads here, Pull down Menu with no prompt
    // So only light bars display.
    return prompt;
}

void MenuBase::moveToBottomAndDisplay(const std::string &prompt) {
    std::string output;
    const int screen_row = m_ctx.getScreenAnsi().getMaxRowsUsedOnScreen();

    output += getDefaultColor();
    output += "\x1b[" + std::to_string(screen_row) + ";1H\r\n";
    output += prompt;
    baseProcessAndDeliver(output);
}

std::string MenuBase::moveStringToBottom(const std::string &prompt) {
    std::string output;
    int screen_row = m_ctx.getScreenAnsi().getMaxRowsUsedOnScreen();

    output += getDefaultColor();
    output += "\x1b[" + std::to_string(screen_row) + ";1H\r\n";
    output += prompt;
    return output;
}

void MenuBase::lightbarUpdate(unsigned int previous_pulldown_id) {
    // Draw Light bars, use next item to determine next/previous id to pull.
    std::string light_bars;
    // Moved to Next Item
    // Turn off Previous Bar
    light_bars.append("\x1b[s"); // Save Cursor Position for prompt.
    light_bars.append(m_ctx.getScreenAnsi().buildPullDownBars(previous_pulldown_id, false));

    // Grab Previous
    for (unsigned int i = 0; i < m_loaded_pulldown_options.size(); i++) {
        auto &m = m_loaded_pulldown_options[i];

        if (m.pulldown_id == previous_pulldown_id) {
            light_bars.append(m.name);
            break;
        }
    }

    light_bars.append("\x1b[0m");

    // Turn on Current Bar
    light_bars.append(m_ctx.getScreenAnsi().buildPullDownBars(m_active_pulldownID, true));

    // Grab Current or new selection
    for (unsigned int i = 0; i < m_loaded_pulldown_options.size(); i++) {
        auto &m = m_loaded_pulldown_options[i];

        if (m.pulldown_id == m_active_pulldownID) {
            light_bars.append(m.name);
            break;
        }
    }

    // Clear Attributes, then move back to menu prompt position.
    light_bars.append("\x1b[0m\x1b[u");
    const std::string output = m_ctx.getIoSession().pipe2ansi(light_bars);
    baseProcessAndDeliver(output);
}

bool MenuBase::executeMenuOptions(const MenuOption &option) {
    // If Invalid then return
    if (m_execute_callback.empty() || option.command_key.size() != 2) {
        m_log.log(UtilLog::LogLevel::Error, "~MenuBase() - executeMenuOptions isEmpty or Size !=2 on Command_Key");
        m_failFlag = true;
        return false;
    }

    m_log.log(UtilLog::LogLevel::Info, "~MenuBase() - m_execute_callback.back()(command_key);", option.command_key,
              "m_execute_callback size=", m_execute_callback.size());

    m_failFlag = false;
    return m_execute_callback.back()(option);
}

bool MenuBase::handleStandardMenuInput(const std::string &input, const std::string &key) {
    // Check for wildcard command input.
    std::string::size_type idx = key.find("*", 0);

    // Catch Light bar Movement when checking wild cards.
    // Return nothing found on Movement.
    if (idx != std::string::npos && m_is_active_pulldown_menu &&
        (input == "RT_ARROW" || input == "DN_ARROW" ||
         input == "LT_ARROW" || input == "UP_ARROW")) {
        return false;
    }

    // If it exists, grab text up to *, then test against input.
    // Check for Wildcard input .. A* would be any keys starting with A
    if (idx != std::string::npos && idx != 0) {
        // Match Strings to the same size.
        std::string key_match = key.substr(0, idx);
        std::string input_match = input.substr(0, m_ctx.getIoCommon().numberOfChars(key_match));

        m_log.log(UtilLog::LogLevel::Debug, "key_match=", key_match, "input_match=", input_match);

        // Normalize and upper case for testing key input
        key_match = m_ctx.getIoCommon().toUpper(key_match);
        input_match = m_ctx.getIoCommon().toUpper(input_match);

        // If we have a match, execute
        if (key_match == input_match) {
            return true;
        }

        // Else if Pull down, and ENTER, then take wild card!
        //else if (m_is_active_pulldown_menu && input_match == "ENTER")
        //{
        //    return true;
        //}
        return false;
    }

    if (idx == 0) {
        m_log.log(UtilLog::LogLevel::Debug, "Wild Card Key * By Itself=", key);
        return true;
    }

    // TODO Review for UTF-8 and extended languages.
    std::string key_normalized = m_ctx.getIoCommon().toUpper(key);
    std::string input_normalized = m_ctx.getIoCommon().toUpper(input);

    // Handle one to one matches.
    if (input_normalized.compare(key_normalized) == 0) {
        m_log.log(UtilLog::LogLevel::Debug, "Match Found=", input_normalized);
        return true;
    }

    return false;
}

bool MenuBase::handleLightbarSelection(const std::string &input) {
    // Handle ESC and Sequences
    int executed = 0;
    int previous_id = m_active_pulldownID;

    if (input == "RT_ARROW" || input == "DN_ARROW") {
        if (m_active_pulldownID < m_ctx.getScreenAnsi().getPullDownMenuSize()) {
            ++m_active_pulldownID;
        } else {
            m_active_pulldownID = 1;
        }

        lightbarUpdate(previous_id);
        ++executed;
    } else if (input == "LT_ARROW" || input == "UP_ARROW") {
        if (m_active_pulldownID > 1) {
            --m_active_pulldownID;
        } else {
            m_active_pulldownID = static_cast<signed>(m_ctx.getScreenAnsi().getPullDownMenuSize());
        }

        lightbarUpdate(previous_id);
        ++executed;
    } else {
        // Add home end.  page etc..
        m_log.log(UtilLog::LogLevel::Debug, "lightbar ELSE!=", input);
    }

    if (executed > 0) {
        return true;
    }

    return false;
}

bool MenuBase::handlePullDownHotKeys(const MenuOption &m, const bool &is_enter, bool &stack_reassignment) {
    std::string current_menu = m_current_menu;
    int executed = 0;

    // First Check for Execute on LightBar Selection.
    // If no valid pull down id matched the active pull down ID, then it's not valid.
    if (is_enter) {
        // Process the current active pull down ID.
        // Check Pull down commands
        if (m.pulldown_id == m_active_pulldownID) {
            // Then we have a match!  Execute the Menu Command with this ID!
            m_log.log(UtilLog::LogLevel::Debug, "[ENTER] Menu Command HOTKEY Executed for=", m.menu_key);

            if (m.menu_key != "FIRSTCMD" && m.menu_key != "EACH") {
                /**
                 * Note, if command doesn't execute, next command doesn't follow!
                 */
                if (executeWithAcs(m)) {
                    // If the menu changed after executing the command
                    // then we are done, leave gracefully.
                    if (current_menu != m_current_menu || m_logoff) {
                        return false;
                    }

                    m_log.log(UtilLog::LogLevel::Debug, "set stack_reassignment = true");
                    // Now assign the m.menu_key to the input, so on next loop, we hit any stacked commands!
                    // If were in pull down menu, and the first lightbar has stacked commands, then we need
                    // to cycle through the remaining command's for stacked on light bars.
                    stack_reassignment = true;
                    ++executed;
                }
            }
        }
    } else {
        // NOT ENTER and pull down,  check hotkeys here!!
        m_log.log(UtilLog::LogLevel::Debug, "[HOTKEY] Menu Command HOTKEY Executed for=", m.menu_key);

        if (executeWithAcs(m)) {
            // If the menu changed after executing the command
            // then we are done, leave gracefully.
            if (current_menu != m_current_menu || m_logoff) {
                return false;
            }
            ++executed;
        }
        // More testing here.. executeWithAcs( ... );
    }

    if (executed > 0) {
        return true;
    }

    return false;
}

std::deque<MenuOption> MenuBase::buildEachCommands() {
    std::deque<MenuOption> commands;
    if (m_menu_info.menu_options.empty())
        return commands;


    for (const auto &cmd: m_menu_info.menu_options) {
        if (cmd.menu_key != "EACH") {
            continue;
        }
        commands.emplace_back(cmd);
    }

    return commands;
}

void MenuBase::executeEachCommands() {
    if (m_menu_info.menu_options.empty())
        return;

    for (const auto &cmd: m_menu_info.menu_options) {
        if (cmd.menu_key != "EACH") {
            continue;
        }

        // If a transition is already pending, abort EACH
        if (m_pendingMenuJump || m_logoff)
            return;

        // Reset FailFlag per Pascal behavior
        m_failFlag = false;

        // Inject this EACH command as a single-item chain
        MenuOption single = cmd;
        m_cmdChainExecutor.start(std::deque<MenuOption>{ single });

        // If chain execution triggered a menu jump or waiting state,
        // we must stop processing further EACH commands
        if (m_cmdChainExecutor.isActive() ||
            m_cmdChainExecutor.isWaiting() ||
            m_pendingMenuJump ||
            m_logoff) {
            return;
        }
    }
}

bool MenuBase::handleLightbarNavigation(const std::string &input) {
    if (m_loaded_pulldown_options.empty()) return false;

    // Standard ANSI Arrow Keys: Up [A, Down [B, Right [C, Left [D
    // We also handle Tab (\t) for parity with some Pascal systems.
    bool moveForward = (input == "\x1b[C" || input == "\x1b[B" || input == "\t");
    bool moveBackward = (input == "\x1b[D" || input == "\x1b[A");

    if (!moveForward && !moveBackward) return false;

    // Collect all unique IDs and sort them to ensure we jump in order
    std::vector<int> ids;
    for (const auto& opt : m_loaded_pulldown_options) {
        if (std::find(ids.begin(), ids.end(), opt.pulldown_id) == ids.end()) {
            ids.push_back(opt.pulldown_id);
        }
    }
    std::sort(ids.begin(), ids.end());

    // Find current position in the ID list
    auto it = std::find(ids.begin(), ids.end(), m_active_pulldownID);

    if (moveForward) {
        if (it != ids.end() && std::next(it) != ids.end()) {
            m_active_pulldownID = *std::next(it);
        } else {
            m_active_pulldownID = ids.front(); // Wrap around to first
        }
    } else if (moveBackward) {
        if (it != ids.begin() && it != ids.end()) {
            m_active_pulldownID = *std::prev(it);
        } else {
            m_active_pulldownID = ids.back(); // Wrap around to last
        }
    }

    return true;
}

bool MenuBase::processMenuOptions(const std::string &input) {
    bool is_enter = false;
    int executed = 0;
    int executedLightBarMovement = 0;

    if (input.empty()) {
        return false;
    }

    // For checking if the menu has changed from an executed option
    std::string current_menu(m_current_menu);

    // For lightbar [ENTER] Selections, stuff with menu key for stacked commands
    // On light bars so any following menu options are executed in order.
    bool stack_reassignment = false;

    // Uppercase all input to match on command/option keys
    std::string input_text = m_ctx.getIoCommon().toUpper(input);
    if (input_text.empty()) {
        return false;
    }

    // Check if ENTER was hit as a command!
    if (input_text == "ENTER") {
        m_log.log(UtilLog::LogLevel::Debug, "EXECUTE ENTER=", input_text);
        is_enter = true;

        // Push out a NewLine after ENTER Executions
        //baseProcessAndDeliver("\r\n");
        // we want a new line, but if executions, then display is
        // side stepped and pushed down one!
    }

    // Check for loaded menu commands.
    // Get Pull down menu commands, Load all from menu options (disk)
    // TODO review if light bar are pulled and if we need to loop and push each one to handleStandardInput!?!
    for (unsigned int i = 0; i < m_menu_info.menu_options.size(); i++) {
        auto &m = m_menu_info.menu_options[i];

        m_log.log(UtilLog::LogLevel::Debug, "MENU KEY=", m.menu_key, "Input=", input_text);

        // Skip all first CMD's.. where only processing input here.
        // FIRSTCMD are executed when the menu loads.
        if (m.menu_key == "FIRSTCMD" || m.menu_key == "EACH") {
            continue;
        }

        // Catch Light bars input is RT_ARROW, LT_ARROW, etc..
        if (input_text[0] == '\x1b' && input_text.size() > 2) // hmm 2?
        {
            // Remove leading ESC for cleaner comparisons.
            std::string clean_sequence = input_text;
            clean_sequence.erase(0, 1);

            // Handle Pull Down Options for Light bars only.
            if (m_is_active_pulldown_menu) {
                // First Make sure the pull down menu, doesn't have menu keys set to specific
                // Control Sequence,  If so, they are normal menu commands, execute first
                // Instead of lightbar interaction.
                m_log.log(UtilLog::LogLevel::Debug, "Pulldown Handle 1=", m.menu_key);

                if (handleStandardMenuInput(clean_sequence, m.menu_key)) {

                    // Before executing ACS command
                    std::string command_to_execute = m.menu_key;

                    if (executeWithAcs(m)) {
                        ++executed;
                    }
                } else {
                    // handle Pull Down Light bar Changes Movement Keys
                    // Should only execute once for movement! in loop of all options.
                    if (executedLightBarMovement == 0) {
                        if (handleLightbarSelection(clean_sequence)) {
                            ++executed;
                            ++executedLightBarMovement;
                        }
                    }
                }
            } else {
                // Handle Standard Input for CONTROL KEYS.
                m_log.log(UtilLog::LogLevel::Debug, "Pulldown Handle 2=", m.menu_key);

                if (handleStandardMenuInput(clean_sequence, m.menu_key)) {

                    // Before executing ACS command
                    std::string command_to_execute = m.menu_key;

                    if (executeWithAcs(m)) {
                        ++executed;
                    }
                }
            }
        }

        // Check for ESC sequence, and next/prev lightbar movement.
        else if (input_text[0] == '\x1b') {
            // Received ESC key,  check for ESC is menu here..
            ++executed;
        }

        // Check Input Keys on Both Pull down and Normal Menus
        // If the input matches the current key, or Enter is hit, then process it.
        else if (input_text.compare(m.menu_key) == 0 || (m_is_active_pulldown_menu && is_enter)) {
            // Pull down selection.
            if (m_is_active_pulldown_menu) {
                m_log.log(UtilLog::LogLevel::Debug, "handlePullDownHotKeys");

                // Handles ENTER Selection or Hotkeys Command Input.
                if (handlePullDownHotKeys(m, is_enter, stack_reassignment)) {
                    if (m_logoff) {
                        return false;
                    }

                    // If Pull down option was selected on Enter, make sure following commands
                    // With Same Menu Key are executed (stacked commands) afterwords in order.
                    if (stack_reassignment && is_enter) {
                        m_log.log(UtilLog::LogLevel::Debug, "stack_reassignment TRUE, KEY=", m.menu_key);
                        input_text.clear();
                        input_text = m.menu_key;
                        stack_reassignment = false;
                        is_enter = false;
                    }

                    ++executed;
                }
            } else {
                // They m.menu_key compared, execute it
                m_log.log(UtilLog::LogLevel::Debug, "ENTER OR HOT KEY MATCH and EXECUTE!=", m.menu_key);

                // Before executing ACS command
                std::string command_to_execute = m.menu_key;

                // Handle '&' substitution with user input
                size_t amp_idx = command_to_execute.find('&');
                if (amp_idx != std::string::npos) {
                    // Replace & with input string (after key prefix, if any)
                    command_to_execute.replace(amp_idx, 1, input_text);
                }

                if (executeWithAcs(m)) {
                    ++executed;
                }
            }
        } else {
            // Handle Standard Menu, Input Field processing.
            m_log.log(UtilLog::LogLevel::Debug, "Pulldown Handle 3=", m.menu_key);

            if (handleStandardMenuInput(input_text, m.menu_key)) {

                // Before executing ACS command
                std::string command_to_execute = m.menu_key;

                // Handle '&' substitution with user input
                size_t amp_idx = command_to_execute.find('&');
                if (amp_idx != std::string::npos) {
                    // Replace & with input string (after key prefix, if any)
                    command_to_execute.replace(amp_idx, 1, input_text);
                }

                if (executeWithAcs(m)) {
                    ++executed;
                }
            }
        }

        // If menu changed, then exit out.
        if (current_menu != m_current_menu || m_logoff) {
            return false;
        }
    }

    // Track Executed Commands, If we didn't execute anything
    // By user input_text, then clear the menu prompt input field
    if (executed > 0) {
        return true;
    }

    return false;
}

void MenuBase::handlePullDownInput(const std::string &character_buffer, const bool &is_utf8) {
    // Get hotkey and lightbar input.
    std::string result = m_ctx.getIoSession().getKeyInput(character_buffer);
    std::string input;

    if (result.empty()) {
        return;
    }
    if (result[0] == 13 || result[0] == 10) {
        // Menu Translations for ENTER
        input = "ENTER";
    } else if (result[0] == '\x1b' && result.size() > 2 && !is_utf8) {
        // ESC SEQUENCE
        input = result;
    } else if (result[0] == '\x1b' && result.size() == 1) {
        // Check Single ESC KEY
        input = "ESC";
    } else {
        // Hot Key Input.
        input = result;
    }

    // Process CommandOptions Matching the Key Input.
    // Need to check for wildcard input there with menu option.
    processMenuOptions(input);
}

void MenuBase::handleFieldInput(const std::string &character_buffer) {
    m_log.log(UtilLog::LogLevel::Info, "MenuBase() - handleFieldInput=", character_buffer);

    // Get LineInput and wait for ENTER.
    std::string key;
    std::string result = m_ctx.getIoSession().getInputField(character_buffer, key, Config::sMenuPrompt_length);

    if (m_cmdChainExecutor.isActive()) {
        if (m_cmdChainExecutor.isWaiting()) {
            // ESC was hit, make this just clear the input text, or start over!
            if (result == "aborted") {
            } else if (result.empty() || result[0] == '\n') {
                // Key == 0 on [ENTER] pressed alone. then invalid!
                // TODO, might have menu keys with ENTER, update this lateron!!
                if (key.empty()) {
                    // Return and don't do anything.
                    return;
                }

                // Process incoming String from Menu Input up to ENTER.
                // If no commands were processed, erase all prompt text
                m_cmdChainExecutor.resumeWithInput(character_buffer);
            } else {
                // Send back the single input received to show client key presses.
                // Only if return data shows a processed key returned.
                if (result != "empty") {
                    std::string output = getDefaultInputColor();
                    output.append(result);
                    baseProcessAndDeliver(output);
                }
            }
            return;
        }
    }

    // Else Normal Field Input

    // ESC was hit, make this just clear the input text, or start over!
    if (result == "aborted") {
    } else if (result.empty() || result[0] == '\n') {
        // Key == 0 on [ENTER] pressed alone. then invalid!
        // TODO, might have menu keys with ENTER, update this lateron!!
        if (key.empty()) {
            // Return and don't do anything.
            return;
        }

        // Extra test, if the menu changed, then don't clear input!
        std::string tmp_menu = m_current_menu;

        // Process incoming String from Menu Input up to ENTER.
        // If no commands were processed, erase all prompt text
        if (!processMenuOptions(key) && tmp_menu == m_current_menu) {
            // Clear Menu Field input Text, redraw prompt?
            std::string clear_input = "\x1b[0m";

            for (std::string::size_type i = m_ctx.getIoCommon().numberOfChars(key); i > 0; i--) {
                clear_input += "\x1b[D \x1b[D";
            }

            baseProcessAndDeliver(clear_input);
        }
    } else {
        // Send back the single input received to show client key presses.
        // Only if return data shows a processed key returned.
        if (result != "empty") {
            std::string output = getDefaultInputColor();
            output.append(result);
            baseProcessAndDeliver(output);
        }
    }
}

void MenuBase::menuInput(const std::string &character_buffer, const bool &is_utf8) {
    m_log.log(UtilLog::LogLevel::Info, "MenuBase() - menuInput=", character_buffer,
              "m_is_active_pulldown_menu=", m_is_active_pulldown_menu);

    // If were in lightbar mode, then we are using hotkeys.
    if (m_is_active_pulldown_menu || m_baseState == BaseState::MENU_YESNO_BAR) {

        // Handle Arrows/Home/End to change m_active_pulldownID
        if (handleLightbarNavigation(character_buffer)) {
            // Re-render only the lightbars, not the whole screen
            baseProcessAndDeliver(buildLightBars());
            return;
        }

        // Handle ENTER Hot Keys.
        if (character_buffer == "\r" || character_buffer == "\n") {
            auto it = std::find_if(m_loaded_pulldown_options.begin(),
                                   m_loaded_pulldown_options.end(),
                [this](const MenuOption& m) {
                    return m.pulldown_id == m_active_pulldownID;
                });

            // This handles the semicolon chain internally
            if (it != m_loaded_pulldown_options.end()) {
                executeWithAcs(*it);
            }
            return;
        }

        m_log.log(UtilLog::LogLevel::Debug, "MenuBase() - handlePullDownInput");
        // might not be needed anymore?
        handlePullDownInput(character_buffer, is_utf8);
    } else {
        m_log.log(UtilLog::LogLevel::Debug, "MenuBase() - handleFieldInput");
        handleFieldInput(character_buffer);
    }
}

void MenuBase::menuYesNoBarInput(const std::string &character_buffer, const bool &is_utf8) {
    handlePullDownInput(character_buffer, is_utf8);
}
