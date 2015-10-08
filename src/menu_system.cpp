
#include "menu_system.hpp"
#include "ansi_processor.hpp"
#include "struct.hpp"
#include "common_io.hpp"
#include "communicator.hpp"

#include <boost/locale.hpp>
#include <boost/filesystem.hpp>

#include <cstring>
#include <string>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <algorithm>
#include <functional>

const std::string MenuSystem::m_menuID = "MENU_SYSTEM";

MenuSystem::MenuSystem(session_data_ptr session_data)
    : TheState(session_data)
    , m_session_io(m_session_data)
    , m_line_buffer("")
    , m_use_hotkey(false)
    , m_current_menu("")
    , m_previous_menu("")
    , m_input_index(MENU_INPUT)
    , m_menu_prompt()
    , m_menu_info()
    , m_menu_options()
    , m_ansi_process(m_session_data->m_telnet_state->getTermRows(),
                     m_session_data->m_telnet_state->getTermCols())
    , m_active_pulldownID(0)
    , m_fail_flag(false)
    , m_pulldown_reentrace_flag(false)

{
    std::cout << "MenuSystem" << std::endl;

    // Setup std::function array with available options to pass input to.
    menu_functions.push_back(std::bind(&MenuSystem::menuInput, this, std::placeholders::_1, std::placeholders::_2));
    menu_functions.push_back(std::bind(&MenuSystem::menuEditorInput, this, std::placeholders::_1, std::placeholders::_2));
}

MenuSystem::~MenuSystem()
{
    std::cout << "~MenuSystem" << std::endl;
}

/**
 * @brief Handles Updates or Data Input from Client
 */
void MenuSystem::update(std::string character_buffer, bool is_utf8)
{
    if(!m_is_active)
    {
        return;
    }

    // This simplily passed through the input to the current system fuction were at.
    menu_functions[m_input_index](character_buffer, is_utf8);
}

/**
 * @brief Startup class, setup initial screens / interface, flags etc..
 * @return
 */
bool MenuSystem::onEnter()
{
    std::cout << "OnEnter() MenuSystem\n";

/*
 *
 * possiable uses right now..  need to double check on where i really want to
 * execute this stuff from
 */

    // Access any needed global configuration values
    if (TheCommunicator::Instance()->m_config_record.UseWelcome)
    {
        // Display random welcome or default welcome.ans here.
    }

    if (TheCommunicator::Instance()->m_config_record.DailyFlagChange)
    {
        // Reset A Users Faild Flags
    }

    if (TheCommunicator::Instance()->m_config_record.SpecialLogin)
    {
        // Needed for MATRIX MENU STARTUP!
    }


    if (TheCommunicator::Instance()->m_config_record.SpecialLogin)
    {
        // Needed for MATRIX MENU STARTUP!
    }

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
 * @brief Clears out Loaded Pulldown options
 */
void MenuSystem::clearMenuPullDownOptions()
{
    while(m_loaded_pulldown_options.size() > 0)
    {
        m_loaded_pulldown_options.pop_back();
    }
}

/**
 * @brief Reads a Specific Menu, Info and Options
 */
void MenuSystem::clearMenuOptions()
{
    while(m_loaded_menu_options.size() > 0)
    {
        m_loaded_menu_options.pop_back();
    }
}

/**
 * @brief Reads a Specific Menu, Info and Options
 */
void MenuSystem::readInMenuData()
{
    MenuInfo m_menu_info;
    MenuOption m_menu_option;

    memset(&m_menu_info, 0, sizeof(MenuInfo));
    memset(&m_menu_option, 0, sizeof(MenuOption));
    clearMenuOptions();
    clearMenuPullDownOptions();

    recordReadInfo(&m_menu_info, m_current_menu, 0);

    // Convert Pascal to C Strings.
    m_common_io.PascalToCString(m_menu_info.Name);
    m_common_io.PascalToCString(m_menu_info.Password);
    m_common_io.PascalToCString(m_menu_info.FallBack);
    m_common_io.PascalToCString(m_menu_info.HelpID);
    m_common_io.PascalToCString(m_menu_info.ACS);
    m_common_io.PascalToCString(m_menu_info.NameInPrompt);
    m_common_io.PascalToCString(m_menu_info.MenuTitle);
    m_common_io.PascalToCString(m_menu_info.PulldownFN);

    /*
    std::cout << "Name: " << t.Name << std::endl;
    std::cout << "Password: " << t.Password << std::endl;
    std::cout << "FallBack: " << t.FallBack << std::endl;
    std::cout << "HelpID: " << t.HelpID << std::endl;
    std::cout << "ACS: " << t.ACS << std::endl;
    std::cout << "NameInPrompt: " << t.NameInPrompt << std::endl;
    std::cout << "MenuTitle: " << t.MenuTitle << std::endl;
    std::cout << "PulldownFN: " << t.PulldownFN << std::endl;

    std::cout << std::endl; */

    // Loop each Option after Reading the Menu.
    int u = 0;
    while(recordReadOption(&m_menu_option, m_current_menu, u++))
    {
        m_common_io.PascalToCString(m_menu_option.Acs);
        m_common_io.PascalToCString(m_menu_option.OptName);
        m_common_io.PascalToCString(m_menu_option.Keys);
        m_common_io.PascalToCString(m_menu_option.CKeys);
        m_common_io.PascalToCString(m_menu_option.CString);

        // Load into vector.
        m_loaded_menu_options.push_back(m_menu_option);

        /*
        std::cout << "OptName: " << o.OptName << std::endl;
        std::cout << "Acs: " << o.Acs << std::endl;
        std::cout << "Hidden: " << m_common_io.boolAlpha(o.Hidden) << std::endl;
        std::cout << "CKeys: " << o.CKeys << std::endl;
        std::cout << "Keys: " << o.Keys << std::endl;
        std::cout << "CString: " << o.CString << std::endl;
        std::cout << "PulldownID: " << o.PulldownID << std::endl;
        std::cout << std::endl;*/
    }
}

/**
 * @brief Load a menu handling.
 */
void MenuSystem::loadInMenu(std::string menu_name)
{
    // Assign current to previous menu, then assign new menu.
    m_previous_menu = m_current_menu;
    m_current_menu = menu_name;

    // Reset Fail flag to false
    m_fail_flag = false;

    // Read in the Current Menu
    readInMenuData();

    // Check If user has access for menu.

    //if not load fallback menu.. etc..
}


/**
 * @brief Decides which Screen is loaded then returns as string.
 */
std::string MenuSystem::loadMenuScreen()
{
    // Check Pulldown FileID
    std::string pull_down_filename = (char *)m_menu_info.PulldownFN;
    std::string screen_data = "";

    // NOTES: check for themes here!!!

    if (pull_down_filename.size() == 0)
    {
        // Load ansi by Menu Name, remove .MNU and Add .ANS
        std::string screen_file = m_current_menu.substr(0, m_current_menu.size()-4) + ".ANS";
        m_common_io.readinAnsi(screen_file, screen_data);
    }
    else
    {
        // Otherwise use the Pulldown menu name from the menu.
        m_common_io.readinAnsi(pull_down_filename, screen_data);
    }
    return screen_data;
}

std::string MenuSystem::buildLightBars()
{
    // Test setup and display lightbars
    std::string light_bars = "";
    bool active_lightbar = false;

    for(int i = 0; i < (signed)m_loaded_menu_options.size(); i++)
    {
        // Always start on Initial or first arrived lightbar.
        if(m_active_pulldownID == i && m_active_pulldownID > 0)
            active_lightbar = true;

        if(m_loaded_menu_options[i].PulldownID > 0)
        {
            // Parse for X/Y Position and colors
            //light_bars.append("      \x1b[6D"); // Notthe bset way, LETS PARSE THE ANSI, and remove %## codes.
            light_bars.append(m_ansi_process.buildPullDownBars(m_loaded_menu_options[i].PulldownID, active_lightbar));
            active_lightbar = false;

            // Add the Option Description
            light_bars.append((char *)m_loaded_menu_options[i].OptName);

            // Clear and reset so we end the lightbar
            light_bars.append("\x1b[0m");
        }
    }
    return light_bars;
}

/**
 * @brief Re parses and display current menu system.
 */
void MenuSystem::redisplayMenuScreen()
{
    // Read in the Menu ANSI
    std::string buffer = loadMenuScreen();
    std::string output = m_session_io.pipe2ansi(buffer);

    if(m_is_active_pulldown_menu)
    {
        // Parse the Screen to the Screen Buffer.
        m_ansi_process.parseAnsiScreen((char *)buffer.c_str());

        // Screen to String so it can be processed.
        m_ansi_process.screenBufferToString();

        // Process buffer for PullDown Codes.
        std::string result = std::move(m_ansi_process.screenBufferParse());  // only if we want result, ignore..

        // Now Build the Light bars
        std::string light_bars = buildLightBars();

        // add and write out.
        output.append(light_bars);
    }
    m_session_data->deliver(output);
}

/**
 * @brief Startup and Switch to Menu Input
 */
void MenuSystem::startupMenu()
{
    std::cout << "Loading Matrix Menu -  Menu Input " << std::endl;

    // 1. Make sure the Input is set to the
    m_input_index = MENU_INPUT;

    // Reset the Pulldown ID for new menus.
    /**
     *  @brief IMPORTANT,m TEHRE IS A MENU COMMAND TO OVERIDE THIS CHECK FOR IT!
     */
    m_active_pulldownID = 0;

    // Read in the Menu ANSI
    std::string buffer = loadMenuScreen();

    // Load the Menu ,, Clears All Structs.
    loadInMenu(m_current_menu);
    if(m_loaded_menu_options.size() < 1)
    {
        std::cout << "<Error> Failed to {find} and load menu: " << m_current_menu << std::endl;
        return;
    }

    // Clear!
    std::vector<MenuOption>().swap(m_loaded_pulldown_options);
    std::vector<int> pull_down_id;

    // Get Pulldown menu commands, Load all from menu options (disk)
    for(int i = 0; i < (signed)m_loaded_menu_options.size(); i++)
    {
        if(m_loaded_menu_options[i].PulldownID > 0)
        {
            // Set First ID as Active.
            m_active_pulldownID = m_loaded_menu_options[i].PulldownID;

            // Setup ID's for pull Ansi Processor
            pull_down_id.push_back(m_loaded_menu_options[i].PulldownID);

            // Get Actual Options with Descriptiosn for Lightbars.
            m_loaded_pulldown_options.push_back(m_loaded_menu_options[i]);
        }
    }

    //if (MenuInfo clear the scrren etc.. )
    //m_session_data->deliver("\x1b[2J\x1b[1;1H");

    // Finally parse the ansi screen and remove pipes
    std::string output = m_session_io.pipe2ansi(buffer);

    // If active pull_down id's found, mark as active pulldown menu.
    if(pull_down_id.size() > 0)
    {
        /**
          * @brief Set Default, however check menu command for override.
          */
        m_active_pulldownID = 1;

        m_is_active_pulldown_menu = true;

        // Parse the Screen to the Screen Buffer.
        m_ansi_process.parseAnsiScreen((char *)buffer.c_str());

        // Screen to String so it can be processed.
        m_ansi_process.screenBufferToString();

        // Process buffer for PullDown Codes. results for TESTING, are discarded.
        std::string result = std::move(m_ansi_process.screenBufferParse());

        // Now Build the Light bars
        std::string light_bars = buildLightBars();

        // add and write out.
        output.append(light_bars);
    }
    else
    {
        m_is_active_pulldown_menu = false;
    }
    m_session_data->deliver(output);



    using namespace boost::locale;
    using namespace std;
    generator gen;
    locale loc=gen("");
    // Create system default locale

    locale::global(loc);
    // Make it system global

    cout.imbue(loc);

    // Now loop and scan for first cmd and each time
    for(int i = 0; i < (signed)m_loaded_menu_options.size(); i++)
    {
        // Process all First Commands}
        std::string key = (char *)m_loaded_menu_options[i].Keys;
        key = to_upper(key);

        if(key == "FIRSTCMD")
        {
            // Process
            std::cout << "FOUND FIRSTCMD! EXECUTE: " << m_loaded_menu_options[i].CKeys << std::endl;
        }

        // Next Process EVERY Commands}
        else if(key == "EACH")
        {
            // Process
            std::cout << "FOUND EACH! EXECUTE: " << m_loaded_menu_options[i].CKeys << std::endl;
        }
    }
}

/**
 * @brief Updates current and next lightbar positions.
 */
void MenuSystem::lightbarUpdate(int previous_pulldown_id)
{
    // Draw Light bars, use next item to determine next/previous id to pull.
    std::string light_bars = "";
    // Moved to Next Item
    // Turn off Previous Bar
    light_bars.append(m_ansi_process.buildPullDownBars(previous_pulldown_id, false));

    for(auto &m : m_loaded_pulldown_options)
    {
        if(m.PulldownID == previous_pulldown_id)
        {
            light_bars.append((char *)m.OptName);
            break;
        }
    }
    light_bars.append("\x1b[0m");

    // Turn on Current Bar
    light_bars.append(m_ansi_process.buildPullDownBars(m_active_pulldownID, true));

    for(auto &m : m_loaded_pulldown_options)
    {
        if(m.PulldownID == m_active_pulldownID)
        {
            light_bars.append((char *)m.OptName);
            break;
        }
    }
    light_bars.append("\x1b[0m");

    std::string output = std::move(m_session_io.pipe2ansi(light_bars));
    m_session_data->deliver(output);
}

/**
 * @brief Processes Menu Commands with input.
 * @param input
 */
void MenuSystem::processMenuOptions(std::string &input)
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

    // Test jump to Menu Editor
    if(input[0] == 'G')
    {
        startupMenuEditor();
        return;
    }

    if(input == "ENTER")
    {
        std::cout << "EXECUTE ENTER!: " << input << std::endl;
        is_enter = true;
    }

    // Uppercase all input to match on comamnd/option keys
    input = to_upper(input);

    // Check for loaded menu commands.
    // Get Pulldown menu commands, Load all from menu options (disk)
    for(int i = 0; i < (signed)m_loaded_menu_options.size(); i++)
    {
        std::string key = (char *)m_loaded_menu_options[i].Keys;
        key = to_upper(key);

        // Next Process EVERY Commands}
        if(key == "EACH")
        {
            // Process
            std::cout << "FOUND EACH! EXECUTE: " << m_loaded_menu_options[i].CKeys << std::endl;
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
                if(m_active_pulldownID < (signed)m_ansi_process.m_pull_down_options.size())
                {
                    ++m_active_pulldownID;
                }
                else
                {
                    m_active_pulldownID = 1;
                }
                lightbarUpdate(previous_id);
            }
            else if(input == "LT_ARROW" || input == "UP_ARROW")
            {
                if(m_active_pulldownID > 1)
                {
                    --m_active_pulldownID;
                }
                else
                {
                    m_active_pulldownID = (signed)m_ansi_process.m_pull_down_options.size();
                }
                lightbarUpdate(previous_id);
            }
            else
            {
                // Add home end.  page etc..
            }
        }

        // Check for ESC sequence, and next/prev lightbar movement.
        else if(input[0] == '\x1b')
        {
            // Received ESC key

        }

        // Then check other keys.
        else if(input.compare(key) == 0 || (m_is_active_pulldown_menu && is_enter))
        {            
            // We have the command, start the work
            if(m_is_active_pulldown_menu)
            {
                // First Check for Execute on LightBar Selection.
                if(is_enter)
                {
                    // Process the current active pull down ID.
                    // Check Pull down commands
                    if(m_loaded_menu_options[i].PulldownID == m_active_pulldownID)
                    {
                        // Then we have a match!  Execute the Menu Command with this ID!
                        std::cout << "Menu Command Executed for: " << key << std::endl;

                        if(key != "FIRSTCMD" && key != "EACH")
                        {
                            input = key;
                        }
                    }

                    // Now assign the key to the input, so on next loop, we hit any stacked commands!
                }
                else
                {
                    // NOT ENTER and pulldown,  check hotkeys here!!
                    std::cout << "Menu Command HOTKEY Executed for: " << key << std::endl;
                }
            }
            else
            {
                // They key compared, execute it
                std::cout << "NORMAL HOT KEY MATCH and EXECUTE! " << key << std::endl;
            }
        }
    }
    
}

/**
 * @brief Default Menu Input Processing.
 */
void MenuSystem::menuInput(const std::string &character_buffer, bool is_utf8)
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
        // Check ESC KEY
        input = "ESC";
    }
    // Process CommandOptions Matching the Key Input.
    processMenuOptions(input);
}


/**
 * @brief Startup Menu Editor and Switch to MenuEditorInput
 */
void MenuSystem::startupMenuEditor()
{

    std::cout << "Entering MenuEditor Input " << std::endl;

    // 1. Make sure the Input is set to the
    m_input_index = MENU_EDITOR_INPUT;

    // 2. Handle any init and startup Screen Displays

    // WIP, nothing completed just yet for the startup.
}


/**
 * @brief Menu Editor, Read and Modify Menus
 */
void MenuSystem::menuEditorInput(const std::string &character_buffer, bool is_utf8)
{

    // Test jump to Matrix Menu
    if(character_buffer[0] == 'Q' && !is_utf8)
    {
        // Switch back to Default Menu System.
        m_current_menu = "MATRIX.MNU";
        startupMenu();
        return;
    }

    // Test Input field here, display leadoff when displaying the input field.
    // Leadoff in getline is just for extra padding and to add to the buffer.

    // NOTE, Getline doesn't handle ESC sequences!  it's raw!
    if(is_utf8)
        std::cout << "unicode" << std::endl;
    else
        std::cout << "ascii" << std::endl;

    /*  Hotkey input working!
    std::cout << "getKeyInput" << std::endl;
    std::string result = m_menu_io.getKeyInput(character_buffer);
    std::cout << "m_session_data->deliver(result);" << std::endl;
    m_session_data->deliver(result);
    */

    std::cout << "getInputField" << std::endl;
    std::string field = "";
    std::string result = m_session_io.getInputField(character_buffer, field);
    if(result == "aborted")
        std::cout << "aborted!" << std::endl;
    else if(result[0] == '\n')
        m_session_data->deliver(field);
    else
        m_session_data->deliver(result);

    return;
    /*

        // Only valid editor input ASCII.
        if (!is_utf8)
        {
            // If were not using hot keys, loop input untill we get ENTER
            // Then handle only the first key in the buffer.  Other "SYSTEMS"
            // Will parse for entire line for matching Command Keys.
            if (!m_use_hotkey)
            {
                // Received ENTER, grab the previous input.
                if (!(character_buffer[0] == 13))
                {
                    m_line_buffer += character_buffer[0];
                    return output_buffer;
                }
            }
            else
            {
                m_line_buffer += character_buffer[0];
            }

            switch (std::toupper(m_line_buffer[0]))
            {
                case 'A': // Add
                    output_buffer = "Enter Menu Name to Add : ";
                    break;
                case 'C': // Change/Edit
                    output_buffer = "Enter Menu Name to Change : ";
                    break;
                case 'D': // Delete
                    output_buffer = "Enter Menu to Delete : ";
                    break;
                case 'Q': // Quit
                    break;
                default : // Return
                    break;
            }
        }
         */
}

