#ifndef MENU_SYSTEM_HPP
#define MENU_SYSTEM_HPP

#include "state_base.hpp"
#include "session_io.hpp"
#include "common_io.hpp"
#include "session_data.hpp"
#include "ansi_processor.hpp"

#include "compat/menu_dao.hpp"
#include "model/struct_compat.hpp"
#include "mods/mod_base.hpp"

#include <stdint.h>
#include <string>
#include <vector>
#include <functional>


class MenuSystem
    : public StateBase
    , public MenuDao
{
public:
    MenuSystem(session_data_ptr session_data);
    ~MenuSystem();

    virtual void update(const std::string &character_buffer, const bool &is_utf8);
    virtual bool onEnter();
    virtual bool onExit();

    virtual std::string getStateID() const
    {
        return m_menuID;
    }

    // This hold non-hotkey text passed through.
    // If Hotkeys are turn off, we append and loop this until we hit a CRLF. or ENTER
    //session_data_ptr m_session_data;     // Handles to the session for i/o. {in TheState Base Class!}
    SessionIO        m_session_io;         // SessionIO for Output parsing and MCI Codes etc.
    CommonIO         m_common_io;          // CommonIO
    std::string      m_line_buffer;        // Buffer used for menu system and reading field data.
    bool             m_use_hotkey;         // Toggle for Single Hotkey or GetLine input. - Not used yet!
    std::string      m_current_menu;       // Name of current menu loaded.
    std::string      m_previous_menu;      // Name of Previous Menu for Gosub
    std::string      m_fallback_menu;      // Fallback, this can set as a Global Fall Back and changed via menu command
    int              m_input_index;        // Menu Input Index, for Forwarding to current function.

    // Temp
    MenuPrompt       m_menu_prompt;        // Hold the currently loaded menu prompt.
    MenuInfo         m_menu_info;          // Menu Info
    MenuOption       m_menu_options;       // Menu Commands

    AnsiProcessor    m_ansi_process;       // Instance for AnsiProcess Methods
    int              m_active_pulldownID;  // Active Lightbar Position.

    // Flags
    bool             m_fail_flag;                   // If menu or Option fails, kick off the fail flag.
    bool             m_pulldown_reentrace_flag;     // If menu or Option fails, kick off the fail flag.
    bool             m_is_active_pulldown_menu;     // If menu has active lightbars to display.

    //int m_next_state;
    static const std::string m_menuID;

    // This matches the index for menu_functions.push_back
    enum
    {
        MENU_INPUT,
        MENU_EDITOR_INPUT,
        MODULE_PRELOGON_INPUT,
        MODULE_INPUT
    };

    // Holds all menu options/commands.
    std::vector<MenuOption> m_loaded_menu_options;

    // Holds all menu prompts.
    std::vector<MenuPrompt> m_loaded_menu_prompts;

    // Holds all pulldown menu options.
    std::vector<MenuOption> m_loaded_pulldown_options;

    // Function Input Vector.
    std::vector<std::function< void(const std::string &, const bool &is_utf8)> > m_menu_functions;

    // Handle to modules being executed.
    std::vector<module_ptr> m_module;

   /**
    * @brief Reads a Specific Menu, Info and Options { Called From readInMenuData() }
    */
    void clearMenuOptions();

   /**
    * @brief Clears out Loaded Pulldown options { Called From readInMenuData() }
    */
    void clearMenuPullDownOptions();


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
     * @brief Loads or reloads and parses the ansi screen
     */
    std::string loadMenuScreen();

    /**
     * @brief Load the Menu into the system container.
     */
    void loadInMenu(std::string menu_name);

    /**
     * @brief Build string of lightbar matrices
     */
    std::string buildLightBars();

    /**
     * @brief De parse and display current menu system.
     */
    void redisplayMenuScreen();


    // Menu System will be a (2) Function system 1st setups up and displays
    // The Second handles all I/O for the menu options, this is dynamic since
    // Input is passed through to it, then the function returns for the next input
    // Since ere async, nothing stays inside it always returns.
    void startupMenu();

    /**
     * @brief Menu Editor, Runs through all existing menus
     */
    std::string displayMenus();

    /**
     * @brief Updates current and next lightbar positions.
     */
    void lightbarUpdate(int previous_pulldown_id);

    /**
     * @brief Process Command Keys passed from menu selection
     * @param input
     */
    void executeMenuOptions(MenuOption &option);

    void processMenuOptions(std::string &input);
    void menuInput(const std::string &character_buffer, const bool &is_utf8);

    void startupMenuEditor();
    void menuEditorInput(const std::string &character_buffer, const bool &);

    // Each system will have it's own module that is allocated and pushed to the
    // m_module container to easily push and pop from the stack.

    /**
     * @brief Starts up Logon Module
     */
    void startupModulePreLogon();

    /**
     * @brief Starts up Logon Module
     */
    void startupModuleLogon();

    /**
     * @brief Handles parsing input for current module.
     */
    void modulePreLogonInput(const std::string &character_buffer, const bool &is_utf8);

    /**
     * @brief Handles parsing input for current module.
     */
    void moduleInput(const std::string &character_buffer, const bool &is_utf8);

};

#endif // MENU_SYSTEM_HPP
