#include "menu_base.hpp"

#include "data/menu_dao.hpp"

#include <boost/locale.hpp>
#include <boost/filesystem.hpp>

#include <cstring>
#include <string>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <algorithm>
#include <functional>

MenuBase::MenuBase(session_data_ptr session_data)
    : m_menu_session_data(session_data)
    , m_session_io(session_data)
    , m_config(new Config())
    , m_config_dao(new ConfigDao(m_config, GLOBAL_BBS_PATH))
    , m_line_buffer("")
    , m_use_hotkey(false)
    , m_current_menu("")
    , m_previous_menu("")
    , m_fallback_menu("")
    , m_input_index(0)
    , m_menu_prompt()
    , m_menu_info(new Menu())
    , m_ansi_process(new AnsiProcessor(
                    session_data->m_telnet_state->getTermRows(),
                    session_data->m_telnet_state->getTermCols()))
    , m_active_pulldownID(0)
    , m_fail_flag(false)
    , m_pulldown_reentrace_flag(false)
{
}

MenuBase::~MenuBase()
{
    std::cout << "~MenuBase" << std::endl;
    
    // Pop Functions off the stack.
    std::vector<std::function< void(const std::string &, const bool &is_utf8)> >().swap(m_menu_functions);
    std::vector<std::function< void(const MenuOption &)> >().swap(m_execute_callback);

    // Pop off the stack to deaallocate any active modules.
    std::vector<module_ptr>().swap(m_module);
}


/**
 * @brief Clears out Loaded Pulldown options
 */
void MenuBase::clearMenuPullDownOptions()
{
    while(m_loaded_pulldown_options.size() > 0)
    {
        m_loaded_pulldown_options.pop_back();
    }
}


/**
 * @brief Reads a Specific Menu, Info and Options
 */
void MenuBase::readInMenuData()
{
    clearMenuPullDownOptions();
    
    // Get Fallback menu if menu is not available.
    std::string fallback = "";
    std::string revert = "";
    if(m_menu_info)
    { 
        fallback = m_menu_info->menu_fall_back;
        revert = m_menu_info->menu_name;
    }

    // Reset the Smart Pointer on menu load.
    m_menu_info.reset(new Menu());

    // Call MenuDao to ready in .yaml file
    MenuDao mnu(m_menu_info, m_current_menu, GLOBAL_MENU_PATH);
    if (mnu.fileExists())
    {
        mnu.loadMenu();
    }
    else
    {
        // Fallck is if user doesn't have access.  update this lateron.
        std::cout << "Menu doesn't exist, loading fallback if exists." << std::endl;
        if (fallback.size() > 0) 
        {
            std::cout << "Loading Fallback menu " << fallback << std::endl;
            m_current_menu = fallback;
            return readInMenuData();
        }
                
        // No menu to fallback or revert
        // Assert so were not in endless loop, something wrong, fix it!
        assert(false);
    }
}


/**
 * @brief Load a menu handling.
 */
void MenuBase::loadInMenu(std::string menu_name)
{
    // Assign current to previous menu, then assign new menu.
    m_previous_menu = m_current_menu;
    m_current_menu = menu_name;

    // Reset Fail flag to false
    m_fail_flag = false;

    // Read in the Current Menu
    readInMenuData();

    std::cout << m_menu_info->menu_name << std::endl;
    std::cout << m_menu_info->menu_pulldown_file << std::endl;
    std::cout << m_menu_info->menu_help_file << std::endl;

    // Check If user has access for menu.

    // if not load fallback menu.. etc..
}


/**
 * @brief Decides which Screen is loaded then returns as string.
 */
std::string MenuBase::loadMenuScreen()
{
    std::cout << "loadMenuScreen" << std::endl;
    
    // Check Pulldown FileID
    std::string screen_data = "";

    // NOTES: check for themes here!!!
    // also  if (m_menu_session_data->m_is_use_ansi), if not ansi, then maybe no pull down, or lightbars!
    if(m_menu_info->menu_pulldown_file.size() == 0 || !m_menu_session_data->m_is_use_ansi)
    {
        std::string screen_file = m_menu_info->menu_name;
        // Load ansi by Menu Name, remove .MNU and Add .ANS, maybe .UTF for utf8 native?
        if (m_menu_session_data->m_is_use_ansi)
        {
            screen_file.append(".ANS");
        }
        else
        {
            screen_file.append(".ASC");
        }
        
        // Screen File(s) are Uppercase.
        std::transform(
            screen_file.begin(), 
            screen_file.end(), 
            screen_file.begin(), 
            ::toupper
        );
        
        std::cout << "readinAnsi1: " << screen_file << std::endl;
         
        m_common_io.readinAnsi(screen_file, screen_data);
    }
    else
    {
        // Pulldown file should have .ANS extnesion.
        std::string screen_file = m_menu_info->menu_pulldown_file;
        
        // Screen File(s) are Uppercase.
        std::transform(
            screen_file.begin(), 
            screen_file.end(), 
            screen_file.begin(), 
            ::toupper
        );
        
        std::cout << "readinAnsi2: " << screen_file << std::endl;
        
        // Otherwise use the Pulldown menu name from the menu.
        m_common_io.readinAnsi(screen_file, screen_data);
    }
    return screen_data;
}


/**
 * @brief Build Light Bars Strings for Display
 *        NOTE, need to check if codes don't exist in ansi screen, we need to skip!
 * @return
 */
std::string MenuBase::buildLightBars()
{
    // Test setup and display lightbars
    std::string light_bars = "";
    bool active_lightbar = false;

    for(auto &m : m_menu_info->menu_options)
    {
        // Always start on Initial or first indexed lightbar.
        // Might need to verify if we need to check for lowest ID, and start on that!
        if(m_active_pulldownID > 0 && m_active_pulldownID == m.pulldown_id)
            active_lightbar = true;

        if(m.pulldown_id > 0)
        {
            // Parse for X/Y Position and colors
            light_bars.append(m_ansi_process->buildPullDownBars(m.pulldown_id, active_lightbar));
            active_lightbar = false;

            // Add the Option Description
            light_bars.append(m.name);

            // Clear and reset so we end the lightbar
            light_bars.append("\x1b[0m");
        }
    }
    return light_bars;
}


/**
 * @brief Re parses and display current menu system.
 */
void MenuBase::redisplayMenuScreen()
{
    // Read in the Menu ANSI
    std::string buffer = loadMenuScreen();
    std::string output = m_session_io.pipe2ansi(buffer);

    if(m_is_active_pulldown_menu)
    {
        // Parse the Screen to the Screen Buffer.
        m_ansi_process->parseAnsiScreen((char *)buffer.c_str());

        // Screen to String so it can be processed.
        m_ansi_process->screenBufferToString();

        // Process buffer for PullDown Codes.
        // only if we want result, ignore.., result just for testing at this time!
        std::string result = std::move(m_ansi_process->screenBufferParse());

        // Now Build the Light bars
        std::string light_bars = buildLightBars();

        // add and write out.
        output.append(light_bars);
    }
    m_menu_session_data->deliver(output);
}


/**
 * @brief Startup And load the Menu File
 *        Default Menu Startup sets Input to MenuInput processing.
 */
void MenuBase::startupMenu()
{
    // Check Configuration here,  if use SpecialLogin (Matrix Menu)
    // Then load it, otherwise jump to Entering UserID / P
    std::cout << "Loading Matrix Menu -  Menu Input " << std::endl;

    // 1. Make sure the Input is set to the
    m_input_index = 0; // MENU_INPUT;

    m_active_pulldownID = 0;
    
    // Load the Menu ,, Clears All Structs.
    loadInMenu(m_current_menu);
    if(m_menu_info->menu_options.size() < 1)
    {
        std::cout << "Menu has no menu_options: " << m_current_menu << std::endl;
        return;
    }
            
    // Get Pulldown menu commands, Load all from menu options (disk)
    std::vector<int> pull_down_ids;
    for(auto &m : m_menu_info->menu_options)
    {
        if(m.pulldown_id > 0 && m_menu_session_data->m_is_use_ansi)
        {
            pull_down_ids.push_back(m.pulldown_id);

            // Get Actual Options with Descriptions for Lightbars.
            m_loaded_pulldown_options.push_back(m);
        }
    }

    // Set the lowest pulldown ID as Active
    if(pull_down_ids.size() > 0)
    {
        auto id = std::min_element(pull_down_ids.begin(), pull_down_ids.end());
        m_active_pulldownID = *id;
    }

    //if (MenuInfo clear the scrren etc.. feature to add! )
    //m_menu_session_data->deliver("\x1b[2J\x1b[1;1H");

    // Finally parse the ansi screen and remove pipes
    // Read in the Menu ANSI
    std::string buffer = loadMenuScreen();
    
    // Output has parsed out MCI codes, translations are then appended.
    std::string output = m_session_io.pipe2ansi(buffer);

    // If active pull_down id's found, mark as active pulldown menu.
    if(pull_down_ids.size() > 0 && m_menu_session_data->m_is_use_ansi)
    {
        // m_menu_info.PulldownFN

        m_is_active_pulldown_menu = true;

        // Parse the Screen to the Screen Buffer.
        m_ansi_process->parseAnsiScreen((char *)buffer.c_str());

        // Screen to String so it can be processed.
        m_ansi_process->screenBufferToString();

        // Process buffer for PullDown Codes. results for TESTING, are discarded.
        std::string result = std::move(m_ansi_process->screenBufferParse());

        // Now Build the Light bars
        std::string light_bars = buildLightBars();

        // add and write out.
        output.append(light_bars);
    }
    else
    {
        m_is_active_pulldown_menu = false;
    }
    m_menu_session_data->deliver(output);

    using namespace boost::locale;
    using namespace std;
    generator gen;
    locale loc=gen("");
    locale::global(loc);
    cout.imbue(loc);

    // Now loop and scan for first cmd and each time
    for(auto &m : m_menu_info->menu_options)
    {
        // Process all First Commands or commands that should run every action.
        std::transform(
            m.menu_key.begin(), 
            m.menu_key.end(), 
            m.menu_key.begin(), 
            ::toupper
        );

        if(m.menu_key == "FIRSTCMD" || m.menu_key == "EACH")
        {
            // Process
            std::cout << "FOUND FIRSTCMD! EXECUTE: " << m.command_key << std::endl;
            executeMenuOptions(m);
        }
    }
}


/**
 * @brief Updates current and next lightbar positions.
 */
void MenuBase::lightbarUpdate(int previous_pulldown_id)
{
    // Draw Light bars, use next item to determine next/previous id to pull.
    std::string light_bars = "";
    // Moved to Next Item
    // Turn off Previous Bar
    light_bars.append(m_ansi_process->buildPullDownBars(previous_pulldown_id, false));

    // Grab Previous
    for(auto &m : m_loaded_pulldown_options)
    {
        if(m.pulldown_id == previous_pulldown_id)
        {
            light_bars.append(m.name);
            break;
        }
    }
    light_bars.append("\x1b[0m");

    // Turn on Current Bar
    light_bars.append(m_ansi_process->buildPullDownBars(m_active_pulldownID, true));

    // Grab Current or new selection
    for(auto &m : m_loaded_pulldown_options)
    {
        if(m.pulldown_id == m_active_pulldownID)
        {
            light_bars.append(m.name);
            break;
        }
    }
    light_bars.append("\x1b[0m");

    std::string output = std::move(m_session_io.pipe2ansi(light_bars));
    m_menu_session_data->deliver(output);
}


/**
 * @brief Process Command Keys passed from menu selection
 * @param input
 */
void MenuBase::executeMenuOptions(const MenuOption &option)
{
    // If Invalid then return
    if(m_execute_callback.size() == 0 || option.command_key.size() != 2)
    {
        return;
    }

    // Execute Menu Option Commands per Callback
    m_execute_callback[0](option);    
}


/**
 * @brief Processes Menu Commands with input.
 * @param input
 */
void MenuBase::processMenuOptions(std::string &input)
{
    std::cout << "processMenuOptions: " << input << std::endl;
    // Create system default locale
    using namespace boost::locale;
    using namespace std;
    generator gen;
    locale loc=gen("");

    // Make it system global
    locale::global(loc);

    // Set as default locale for output
    cout.imbue(loc);

    bool is_enter = false;

    // Uppercase all input to match on comamnd/option keys
    input = to_upper(input);

    // Check if ENTER was hit as a command!
    if(input == "ENTER")
    {
        std::cout << "EXECUTE ENTER!: " << input << std::endl;
        is_enter = true;
    }

    // Check for loaded menu commands.
    // Get Pulldown menu commands, Load all from menu options (disk)
    for(auto &m : m_menu_info->menu_options)
    {
        // Process all First Commands or commands that should run every action.
        std::transform(
            m.menu_key.begin(), 
            m.menu_key.end(), 
            m.menu_key.begin(), 
            ::toupper
        );

        // Next Process EVERY Commands}
        if(m.menu_key == "EACH")
        {
            // Process, although should each be execure before, or after a menu command!
            // OR is each just on each load/reload of menu i think!!
            std::cout << "FOUND EACH! EXECUTE: " << m.command_key << std::endl;
            executeMenuOptions(m);
            continue;
        }

        // Check for ESC sequence, and next/prev lightbar movement.
        else if(input[0] == '\x1b' && input.size() > 2)
        {
            // Remove leading ESC for cleaner comparisons.
            input.erase(0,1);

            // Handle ESC and Sequences
            int previous_id = m_active_pulldownID;
            if(input == "RT_ARROW" || input == "DN_ARROW")
            {
                if(m_active_pulldownID < (signed)m_ansi_process->m_pull_down_options.size())
                {
                    ++m_active_pulldownID;
                }
                else
                {
                    m_active_pulldownID = 1;
                }
                lightbarUpdate(previous_id);
                continue;
            }
            else if(input == "LT_ARROW" || input == "UP_ARROW")
            {
                if(m_active_pulldownID > 1)
                {
                    --m_active_pulldownID;
                }
                else
                {
                    m_active_pulldownID = (signed)m_ansi_process->m_pull_down_options.size();
                }
                lightbarUpdate(previous_id);
                continue;
            }
            else
            {
                // Add home end.  page etc..
            }
        }

        // Check for ESC sequence, and next/prev lightbar movement.
        else if(input[0] == '\x1b')
        {
            // Received ESC key,  check for ESC is menu here..

        }

        /**
         * There is wildcarding for menu commands:
         * If you set the Key to X*, then you can put * in the
         * Cstring and that will put what follows the X in the
         * Cstring. This is advisable for such cases as file
         * conference jumping such as J* with would do JM with a
         * Cstring of * so one could J1,J2, etc.
         * 
         * Also a possibility for CString is & in which is set to the
         * input gotten with -I, -J, or set with -*.
         */
 
        // Check Input Keys on Both Pulldown and Normal Menus
        // If the input matches the current key, or Enter is hit, then process it.
        else if(input.compare(m.menu_key) == 0 || (m_is_active_pulldown_menu && is_enter))
        {
            // We have the command, start the work
            if(m_is_active_pulldown_menu)
            {
                // First Check for Execute on LightBar Selection.
                if(is_enter)
                {
                    // Process the current active pull down ID.
                    // Check Pull down commands
                    if(m.pulldown_id == m_active_pulldownID)
                    {
                        // Then we have a match!  Execute the Menu Command with this ID!
                        std::cout << "Menu Command Executed for: " << m.menu_key << std::endl;

                        if(m.menu_key != "FIRSTCMD" && m.menu_key != "EACH")
                        {
                            //input = m.menu_key; ??
                            executeMenuOptions(m);
                        }
                    }

                    // Now assign the m.menu_key to the input, so on next loop, we hit any stacked commands!
                }
                else
                {
                    // NOT ENTER and pulldown,  check hotkeys here!!
                    std::cout << "Menu Command HOTKEY Executed for: " << m.menu_key << std::endl;
                    executeMenuOptions(m);
                    // More testing here.. executeMenuOptions( ... );
                }
            }
            else
            {
                // They m.menu_key compared, execute it
                std::cout << "NORMAL HOT KEY MATCH and EXECUTE! " << m.menu_key << std::endl;
                executeMenuOptions(m);
                // More testing here.. executeMenuOptions( ... );
            }
        }
    }
}


/**
 * @brief Default Menu Input Processing.
 *        Handles Processing for Loaded Menus Hotkey and Lightbars
 */
void MenuBase::menuInput(const std::string &character_buffer, const bool &is_utf8)
{
    // Check key input, if were in a sequence get the complete sequence
    std::string result = m_session_io.getKeyInput(character_buffer);
    std::string input = character_buffer;

    if(result.size() == 0)
    {
        // Empty, possiable sequence, get keep checking!
        return;
    }
    else if(result[0] == 13 || result[0] == 10)
    {
        // Menu Translations for ENTER
        input = "ENTER";
    }
    else if(result[0] == '\x1b' && result.size() > 2 && !is_utf8)
    {
        // ESC SEQUENCE
        input = result;
    }
    else if(result[0] == '\x1b' && result.size() == 1)
    {
        // Check Single ESC KEY
        input = "ESC";
    }
    // Process CommandOptions Matching the Key Input.
    processMenuOptions(input);
}
