#include "menu_base.hpp"

#include "data-sys/menu_dao.hpp"
#include "data-sys/menu_prompt_dao.hpp"

#include "access_condition.hpp"

#include <boost/locale.hpp>
#include <boost/lexical_cast.hpp>

// Fix for file_copy
#define BOOST_NO_CXX11_SCOPED_ENUMS
#include <boost/filesystem.hpp>
#undef BOOST_NO_CXX11_SCOPED_ENUMS

#include <cstring>
#include <string>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <algorithm>
#include <functional>
#include <random>


MenuBase::MenuBase(session_data_ptr session_data)
    : m_menu_session_data(session_data)
    , m_session_io(session_data)
    , m_config(new Config())
    , m_line_buffer("")
    , m_use_hotkey(false)
    , m_current_menu("")
    , m_previous_menu("")
    , m_fallback_menu("")
    , m_starting_menu("") // Note Update this from config!
    , m_input_index(MENU_INPUT)
    , m_menu_info(new Menu())
    , m_menu_prompt()
    , m_ansi_process(new AnsiProcessor(
                         session_data->m_telnet_state->getTermRows(),
                         session_data->m_telnet_state->getTermCols()))
    , m_active_pulldownID(0)
    , m_fail_flag(false)
    , m_pulldown_reentrace_flag(false)
    , m_use_first_command_execution(true)
    , m_logoff(false)
{
    std::cout << "MenuBase" << std::endl;
}

MenuBase::~MenuBase()
{
    std::cout << "~MenuBase" << std::endl;

    // Pop Functions off the stack.
    std::vector<std::function< void(const std::string &, const bool &is_utf8)> >().swap(m_menu_functions);
    std::vector<std::function< bool(const MenuOption &)> >().swap(m_execute_callback);

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

    m_ansi_process->clearPullDownBars();
}

/**
 * @brief Validates if user has access to menu (preLoad)
 * @return
 */
bool MenuBase::checkMenuAcsAccess(menu_ptr menu)
{
    AccessCondition acs;
    return acs.validateAcsString(
               menu->menu_acs_string,
               m_menu_session_data->m_user_record
           );
}

/**
 * @brief Validates if user has access to menu options
 * @return
 */
void MenuBase::checkMenuOptionsAcsAccess()
{
    std::vector<MenuOption>::iterator it = m_menu_info->menu_options.begin();
    std::vector<MenuOption>::iterator end = m_menu_info->menu_options.end();
    std::vector<MenuOption> new_options;
    AccessCondition acs;

    for(; it != end; it++)
    {
        if (acs.validateAcsString(
                    (*it).acs_string,
                    m_menu_session_data->m_user_record))
        {
            new_options.push_back(*it);
        }
    }

    // Swap Validated Options with Existing.
    m_menu_info->menu_options.swap(new_options);
}

/**
 * @brief Reads a Specific Menu, Info and Options
 */
void MenuBase::readInMenuData()
{
    clearMenuPullDownOptions();

    // Get Fallback menu if menu is not available.
    std::string revert = "";
    if(m_menu_info)
    {
        // Only set or override if it exists.
        if (m_menu_info->menu_fall_back.size() > 0)
        {
            m_fallback_menu = m_menu_info->menu_fall_back;
        }
        revert = m_menu_info->menu_name;
    }

    // Isolate Smart Pointer Scope, once it's leaves it's cleared.
    // For PreLoading and Testing Menu ACS String
    {
        // Pre-Load Menu, check access, if not valud, then fall back to previous.
        menu_ptr pre_load_menu(new Menu);

        // Call MenuDao to ready in .yaml file
        MenuDao mnu(pre_load_menu, m_current_menu, GLOBAL_MENU_PATH);
        if (mnu.fileExists())
        {
            // Reset the Smart Pointer on menu load.
            mnu.loadMenu();

            // Check Menu Access Acces,, if Valid, swap current with preloaded.
            if (checkMenuAcsAccess(pre_load_menu))
            {
                m_menu_info.reset();
                m_menu_info = pre_load_menu;
                checkMenuOptionsAcsAccess();
            }
        }
        else
        {
            // Fallck is if user doesn't have access.  update this lateron.
            std::cout << "Menu doesn't exist, loading fallback if exists." << std::endl;
            if (m_fallback_menu.size() > 0)
            {
                std::cout << "Loading Fallback menu " << m_fallback_menu << std::endl;
                m_current_menu = m_fallback_menu;
                return readInMenuData();
            }

            // No menu to fallback or revert
            // Assert so were not in endless loop, something wrong, fix it!
            assert(false);
        }
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

    // Testing Information pulled in.
    std::cout << "Menu Name: " << m_menu_info->menu_name << std::endl;
    std::cout << "Menu Pulldown: " << m_menu_info->menu_pulldown_file << std::endl;
    std::cout << "Menu Helpfile: " << m_menu_info->menu_help_file << std::endl;

    // Check If user has access for menu.

    // if not load fallback menu.. etc..
    std::cout << "Fallback Menu: " << m_fallback_menu << std::endl;
}

/**
 * @brief Imports Menu from Modules into the system container.
 */
void MenuBase::importMenu(menu_ptr menu_info)
{
    clearMenuPullDownOptions();
    m_menu_info = menu_info;
    m_current_menu = m_menu_info->menu_name;

    // Remove Options the users might not have access to
    // ie Sysop Commands inside of forms.
    checkMenuOptionsAcsAccess();
    
    // Now we need to process custom screens from module menu
    
    
    
}

/**
 * @brief Processes a TOP Template Screen for Menus
 * @param screen
 * @return
 */
std::string MenuBase::processTopGenericTemplate(const std::string &screen)
{
    /**
     * When we get 2J alone, it clears but leaves cursor.
     * In most cases we need to add a pre-home cursor!
     */
    std::string new_screen = screen;
    std::string::size_type index = 0;
    while(index != std::string::npos)
    {
        index = new_screen.find("\x1b[2J", index);
        if (index != std::string::npos)
        {
            new_screen.replace(index, 4, "\x1b[1;1H\x1b[2J");
            // Incriment past previous replacement.
            index += 9;
        }
    }

    return new_screen;
}

/**
 * @brief Processes a MID Template Screen for Menus
 * @param screen
 * @return
 */
std::string MenuBase::processMidGenericTemplate(const std::string &screen)
{
    // Use a Local Ansi Parser for Pasrsing Menu Template with Mid.
    ansi_process_ptr ansi_process(new AnsiProcessor(
                                      m_menu_session_data->m_telnet_state->getTermRows(),
                                      m_menu_session_data->m_telnet_state->getTermCols()));
    std::string output_screen;
    std::string new_screen = screen;

    std::string::size_type index = 0;
    while(index != std::string::npos)
    {
        index = new_screen.find("\r", index);
        if (index != std::string::npos)
        {
            new_screen.erase(index, 1);
        }
    }

    index = 0;
    while(index != std::string::npos)
    {
        index = new_screen.find("\n", index);
        if (index != std::string::npos)
        {
            new_screen.erase(index, 1);
        }
    }

    // Clear All Mappings
    m_session_io.clearAllMCIMapping();

    // Build a single code map that can be reused.
    std::vector<MapType> code_map = m_session_io.pipe2genericCodeMap(new_screen);

    // Loop the code map and determine the number of unique columns for parsing.
    int key_columns = 0;
    int des_columns = 0;
    std::string lookup = "";

    // Loop codes and get max number of columns per code.
    for(unsigned int i = 0; i < code_map.size(); i++)
    {
        auto &map = code_map[i];
        //std::cout << "Generic Code: " << map.m_code << std::endl;
        if (map.m_code[1] == 'K')
        {
            ++key_columns;
        }

        if (map.m_code[1] == 'D')
        {
            ++des_columns;
        }
    }

    // No codes found in ansi, or invalid combination exit!
    if (key_columns == 0 || key_columns != des_columns)
    {
        //std::cout << "No Generic Code Maps found." << std::endl;
        return output_screen;
    }

    // Loop all menu options, and pass the number of columns at a time.
    int column = 1;
    std::string key, value;
    std::string::size_type idx;
    for(unsigned int i = 0; i < m_menu_info->menu_options.size(); i++)
    {
        auto &m = m_menu_info->menu_options[i];
        // Skip Options that are Automatic Exection, or Stacked with no name and hidden
        if(m.menu_key == "FIRSTCMD" || m.menu_key == "EACH" ||
                m.name.size() == 0 || m.hidden)
        {
            continue;
        }

        // Build Key/Value for Menu Key
        key = "|K" + std::to_string(column);

        // Clean any wildcard from menu key.
        idx = m.menu_key.find("*");
        if (idx != std::string::npos)
        {
            value = m.menu_key.substr(0, idx);
        }
        else
        {
            value = m.menu_key;
        }
        m_session_io.addMCIMapping(key, value);

        // Build Key/Value for Menu Description
        key = "|D" + std::to_string(column);
        value = m.name;
        m_session_io.addMCIMapping(key, value);

        if (column % key_columns == 0)
        {
            // Process template menu row and all columns added.
            output_screen += m_session_io.parseCodeMapGenerics(new_screen, code_map);
            output_screen += "\x1b[D\r\n";
            column = 0;
        }
        ++column;
    }

    // Process any remaining not caught in offset.
    if (m_session_io.getMCIMappingCount() > 0)
    {
        output_screen += m_session_io.parseCodeMapGenerics(new_screen, code_map);
        output_screen += "\x1b[D\r\n";
    }

    // Clear Codemap.
    std::vector<MapType>().swap(code_map);
    ansi_process->parseAnsiScreen((char *)output_screen.c_str());

    // Return with no clear screen, since this is a mid ansi.
    return ansi_process->getScreenFromBuffer(false);
}

/**
 * @brief Generic SRT, MID, END screen processing
 * @return
 */
std::string MenuBase::processGenericScreens()
{
    std::string top_screen = m_common_io.readinAnsi("GENSRT.ANS");
    std::string mid_screen = m_common_io.readinAnsi("GENMID.ANS");
    std::string bot_screen = m_common_io.readinAnsi("GENEND.ANS");
    std::string screen_output = "";

    // Add the Top section of the template
    // Do a simple MCI Code replace for title
    // |TI - Menu Title
    std::string::size_type idx = top_screen.find("|TI");
    if (idx != std::string::npos)
    {
        std::cout << "parsing menu code title" << std::endl;
        top_screen.replace(
            idx,
            3,
            m_menu_info->menu_title
        );
    }

    screen_output += processTopGenericTemplate(top_screen);

    /**
     * According to the ANSI 3.64-1979 standard esc[;xxH should go
     * to XXth column in first row, however Oblivion/2 interprets
     * the code differently in middle repeat ansis (and only middle
     * in the repeat ansis).  Instead of going to XXth column in
     * first row, it will go to XXth column in current row, thus
     * making repeat ANSIs possible.
     */

    // |K? - key,  |D? - Description
    //|K1 |D1   |K2  |D2  |K3  |D3 ...
    screen_output += processMidGenericTemplate(mid_screen);
    screen_output += bot_screen;
    return screen_output;
}

/**
 * @brief Setup light bar string, and return default display.
 */
std::string MenuBase::setupYesNoMenuInput(const std::string &menu_prompt, std::vector<MapType> &code_map)
{
    m_input_index = MENU_YESNO_BAR;
    clearMenuPullDownOptions();

    // Then feed though and return the updated string.
    std::string prompt_string = std::move(m_session_io.parseCodeMapGenerics(menu_prompt, code_map));
    std::string display_prompt = moveStringToBottom(prompt_string);

    // Translate Pipe Coles to ESC Sequences prior to parsing to keep
    // String length calculations.
    display_prompt = m_session_io.pipe2ansi(display_prompt);

    std::string yesNoBars = getDefaultColor() + "|01";
    yesNoBars += getDefaultInputColor() + getDefaultInverseColor() + "%01\x1b[0m";
    yesNoBars += " ";
    yesNoBars += getDefaultColor() + "|02";
    yesNoBars += getDefaultInputColor() + getDefaultInverseColor() + "%02\x1b[0m";
    yesNoBars.insert(0, display_prompt);

    // Parse the Screen to the Screen Buffer.
    m_ansi_process->parseAnsiScreen((char *)yesNoBars.c_str());

    // Screen to String so it can be processed.
    m_ansi_process->screenBufferToString();

    // Process buffer for PullDown Codes.
    // only if we want result, ignore.., result just for testing at this time!
    std::string result = std::move(m_ansi_process->screenBufferParse());

    // Update Lightbars, by default they have no names for YES/NO/Continue prompts.
    for(unsigned int i = 0; i < m_menu_info->menu_options.size(); i++)
    {
        auto &m = m_menu_info->menu_options[i];

        // Default setup for Yes No with default to Yes!
        if (i == 0)
        {
            m.pulldown_id = 1;
            m.name = "  Yes  ";
        }
        else
        {
            m.pulldown_id = 2;
            m.name = "  No  ";
        }

        m_loaded_pulldown_options.push_back(m);
    }

    // Now Build the Light bars
    std::string light_bars = buildLightBars();
    display_prompt.append(light_bars);

    // Hide Cursor on lightbars
    display_prompt.append("\x1b[?25l");

    return display_prompt;
}

/**
 * @brief Gets the Default Color Sequence
 * @return
 */
std::string MenuBase::getDefaultColor()
{
    return m_session_io.pipeColors(m_menu_session_data->m_user_record->sRegColor);
}

/**
 * @brief Gets the Default Input Color Sequence
 * @return
 */
std::string MenuBase::getDefaultInputColor()
{
    return m_session_io.pipeColors(m_menu_session_data->m_user_record->sInputColor);
}

/**
 * @brief Gets the Default Inverse Color Sequence
 * @return
 */
std::string MenuBase::getDefaultInverseColor()
{
    return m_session_io.pipeColors(m_menu_session_data->m_user_record->sInverseColor);
}

/**
 * @brief Builds the menu prompt as a question String
 * @return
 */
std::string MenuBase::parseMenuPromptString(const std::string &prompt_string)
{
    // Color Sequences and NewLine
    m_session_io.clearAllMCIMapping();
    m_session_io.addMCIMapping("^R", m_config->default_color_regular);
    m_session_io.addMCIMapping("^S", m_config->default_color_stat);
    m_session_io.addMCIMapping("^P", m_config->default_color_prompt);
    m_session_io.addMCIMapping("^E", m_config->default_color_input);
    m_session_io.addMCIMapping("^V", m_config->default_color_inverse);
    m_session_io.addMCIMapping("^X", m_config->default_color_box);
    m_session_io.addMCIMapping("^M", "\r\n");

    /*
     * Notes from the Legacy Doc's.
     *
     * Side Note, looks like legacy does some extra newlines before displaying
     * these prompt, or clears screen,  double check and compare!!
     *
    N
        Writes the Name (menu prompt string) in the Prompt alone

          May Contain:

              ^R - Regular Color      ^S - Status Color
              ^P - Prompt Color       ^E - Input Color
              ^V - Inverse Color      ^X - Box Color
              ^M - Goes down a line

          Using The Following To End:
              / Yes/No Bar Prompt beginning with No
              \ Yes/No Bar Prompt beginning with Yes
              = Yes/No/Quit Bar Prompt Beginning with Yes
              | Yes/No/Quit Bar Prompt Beginning with No
              @ Yes/No/Quit Bar Prompt Beginning with Quit
              * Inputs String
              : Inputs String with a : in a different color
              # Hotkey without Echo
              ) Hotkey with Echo
              ( Sets the string equal to the Input Question
                varaible set with -I, -J, or -M

      You can use the following characters in the keys of a menu
      using a name in prompt ending with a # or ).

      All pipe codes are also applicable in this prompt.

          This command is used to make menus that ask questions,
      create hotkeyed type menus, or get input from users.

      Bx
        Does Bar selection menu with x number of columns

      R
        Uses a one-line bar menu. When creating command stacks,
        make sure to only put a description on the first option in
        the stack, otherwise your whole command stack will appear
        on this menu. Also, make sure that all options don't exceed
        80 colums, as this is a ONE line bar menu. If your options
        exceed the 80 column limit, use the bar menus.
    */

    // Depending on the CodeMap return fro the (2)nd group, which are the ending characters
    // We'll need to setup new menus on these features.
    std::vector<MapType> code_map = m_session_io.pipe2promptCodeMap(prompt_string);
    std::string output = "";

    // Loop codes and picked out ending control code.
    bool match_found = false;
    for(unsigned int i = 0; i < code_map.size(); i++)
    {
        auto &map = code_map[i];
        std::cout << "Menu Prompt Code: " << map.m_code << std::endl;

        // Control Codes are in Group 2
        if (map.m_match == 2)
        {
            switch(map.m_code[0])
            {
                case '\\':
                    m_active_pulldownID = 2; // NO Default
                    output = std::move(setupYesNoMenuInput(prompt_string, code_map));
                    match_found = true;
                    break;

                case '/':
                    m_active_pulldownID = 1; // YES Default
                    output = std::move(setupYesNoMenuInput(prompt_string, code_map));
                    match_found = true;
                    break;

                    // Handle yes /no /continue

                default:
                    break;
            }
        }

        // Found code, return.
        if (match_found)
        {
            break;
        }
    }

    // Then we feed it through again to handle colors replacements.
    return output;
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
        std::string screen_file = m_menu_info->menu_help_file;
        // Load ansi by Menu Name, remove .MNU and Add .ANS, maybe .UTF for utf8 native?
        if (m_menu_session_data->m_is_use_ansi)
        {
            screen_file.append(".ANS");
        }
        else
        {
            screen_file.append(".ASC");
        }

        // Make all screens uppercase, handle unicode names.
        screen_file = boost::locale::to_upper(screen_file);

        // if file doesn't exist, then use generic template
        if (m_common_io.fileExists(screen_file))
        {
            m_common_io.readinAnsi(screen_file, screen_data);
        }
        else
        {
            // Load and use generic template.
            // These are GENTOP. GENMID, GENBOT.ANS
            screen_data = processGenericScreens();
        }
    }
    else
    {
        // Pulldown file should have .ANS extnesion.
        std::string screen_file = m_menu_info->menu_pulldown_file;

        // Screen File(s) are Uppercase.
        screen_file = boost::locale::to_upper(screen_file);

        // Otherwise use the Pulldown menu name from the menu.
        // if file doesn't exist, then use generic template
        if (m_common_io.fileExists(screen_file))
        {
            m_common_io.readinAnsi(screen_file, screen_data);
        }
        else
        {
            // Load and use generic template, fallback if file is missing.
            // These are GENTOP. GENMID, GENBOT.ANS
            screen_data = processGenericScreens();
        }
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

    for(unsigned int i = 0; i < m_menu_info->menu_options.size(); i++)
    {
        auto &m = m_menu_info->menu_options[i];
        // Always start on Initial or first indexed lightbar.
        // Might need to verify if we need to check for lowest ID, and start on that!
        if(m_active_pulldownID > 0 && m_active_pulldownID == m.pulldown_id)
        {
            active_lightbar = true;
        }

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
    std::cout << " *** redisplay Menu Screen *** " << std::endl;
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

    // Load the Menu prompt
    output += loadMenuPrompt();
    baseProcessAndDeliver(output);
}

/**
 * @brief Execute First and Each Commands on Startup
 */
void MenuBase::executeFirstAndEachCommands()
{
    using namespace boost::locale;
    using namespace std;
    generator gen;
    locale loc=gen("");
    locale::global(loc);
    cout.imbue(loc);

    //std::cout << "m_menu_info->menu_options: " << m_menu_info->menu_options.size() << std::endl;

    // Now loop and scan for first cmd and each time
    for(unsigned int i = 0; i < m_menu_info->menu_options.size(); i++)
    {
        auto &m = m_menu_info->menu_options[i];
        // Process all First Commands or commands that should run every action.
        //std::cout << "index: " << m.index << std::endl;
        //std::cout << "menu_key: " << m.menu_key << std::endl;
        std::string new_key = boost::locale::to_upper(m.menu_key);
        m.menu_key = std::move(new_key);

        if(m.menu_key == "FIRSTCMD" || m.menu_key == "EACH")
        {
            //std::cout << "*** FOUND FIRSTCMD! EXECUTE: " << m.command_key << std::endl;
            executeMenuOptions(m);
        }
    }
}

/**
 * @brief Parse Menu Prompt Folder and pull Random Menu Prompt
 * @return
 */
std::vector<std::string> MenuBase::getListOfMenuPrompts()
{
    namespace fs = boost::filesystem;
    fs::path prompt_directory(GLOBAL_MENU_PROMPT_PATH);
    fs::directory_iterator end_iter;

    typedef std::vector<std::string> result_set_t;
    typedef std::vector<std::string>::iterator iterator;
    result_set_t result_set;
    std::vector<std::string> result_list;

    if(fs::exists(prompt_directory) && fs::is_directory(prompt_directory))
    {
        for(fs::directory_iterator dir_iter(prompt_directory); dir_iter != end_iter; ++dir_iter)
        {
            if(dir_iter->path().extension() == ".yaml")
            {
                if(fs::is_regular_file(dir_iter->status()))
                {
                    result_set.push_back(dir_iter->path().filename().string());
                    // result_set_t::value_type(fs::last_write_time( dir_iter->path() ) ) ); // *dir_iter));
                }
            }
        }
    }

    // check result set, if no menu then return gracefully.
    if(result_set.size() == 0)
    {
        std::cout << "\r\n*** No Menu Prompt .yaml files found!" << std::endl;
        return result_list;
    }

    // Sort Menu Prompt's in accending order
    std::sort(result_set.begin(), result_set.end());

    for (std::string s : result_set)
    {
        result_list.push_back(s.substr(0, s.size()-5));
    }

    return result_list;
}


/**
 * @brief Parse Menu Prompt Folder and pull Random Menu Prompt
 * @return
 */
std::string MenuBase::getRandomMenuPrompt()
{
    std::vector<std::string> result_set = getListOfMenuPrompts();

    // check result set, if no menu then return gracefully.
    if(result_set.size() == 0)
    {
        return "";
    }

    //generator initialized with seed from time.
    std::mt19937_64 generator {static_cast<unsigned int>(std::time(0))};

    int set_size = result_set.size()-1;

    //the range is inclusive, so this produces numbers in range [0, 10), same as before
    std::uniform_int_distribution<> dist {0, set_size};

    int randomNumber = dist(generator);
    return result_set[randomNumber];
}

/**
 * @brief Return Selected or Active prompt as a string.
 * @return
 */
std::string MenuBase::loadMenuPrompt()
{
    // Display Menu Prompt if it exists, right now it's default
    // lateron add users selected.  This is just a test!
    std::string prompt = "";
    std::string prompt_display = "";

    if (m_menu_session_data->m_user_record->iId != -1)
    {
        prompt = m_menu_session_data->m_user_record->sMenuPromptName;
    }

    // If users Menu Promt is blank, then grab random menu prompt!
    if (prompt.size() == 0 || prompt == "")
    {
        prompt = getRandomMenuPrompt();
    }

    // Load Menu prompt.
    m_menu_prompt.reset(new MenuPrompt());

    std::cout << "Loading Menu Prompt: " << prompt << std::endl;

    // Load YAML Menu Prompt
    MenuPromptDao mnu_prompt(m_menu_prompt, prompt, GLOBAL_MENU_PROMPT_PATH);
    bool is_loaded = mnu_prompt.loadMenuPrompt();

    // Don't display prompts on Pulldown menu's.
    if (!m_is_active_pulldown_menu && is_loaded)
    {
        //fmt.Print("\033[?25l")
        //fmt.Print("\033[?25h")

        // Default Display Cursor prompt starting point, make this configurable lateron
        prompt_display = "\x1b[?25h\x1b[22;1H";
        prompt_display += boost::lexical_cast<std::string>(m_menu_prompt->data_line1) + "\r\n";
        prompt_display += boost::lexical_cast<std::string>(m_menu_prompt->data_line2) + "\r\n";
        prompt_display += boost::lexical_cast<std::string>(m_menu_prompt->data_line3);

        // Clear All Mappings
        m_session_io.clearAllMCIMapping();

        // Parse Prompt for Menu Title here, let pip2ansi parse standard codes.
        m_session_io.addMCIMapping("|MN", m_menu_info->menu_prompt);
        m_session_io.addMCIMapping("|TL", "1440");              // Time Left {Not Implimented Yet}
        m_session_io.addMCIMapping("|TM", "Current Date/Time"); // Time Now  {Not Implimented Yet}
        m_session_io.addMCIMapping("|NN", std::to_string(m_menu_session_data->m_node_number));

        // Legacy Note:
        // SysOps may place %%filename.ext anywhere in the menu prompt
        // to display filename.ext from the prompts directory. Use the
        // %MN code to display the Menu's Name in Prompt (in filename.ext).
        // Set the Prompts directory in the CONFIG.
        m_session_io.addMCIMapping("%MN", m_menu_info->menu_prompt);

        std::string output = m_session_io.pipe2ansi(prompt_display);
        //std::cout << "prompt: " << output << std::endl;
        return output;
    }
    else
    {
        prompt = "";
        std::cout << "No Menu prompts loaded." << std::endl;
        if (m_menu_info->menu_prompt.size() > 0)
        {
            std::cout << "Use Default Prompt String in Menu." << std::endl;
            prompt = "\x1b[?25h"; // Turn on Cursor.
            prompt +=  m_session_io.pipe2ansi(m_menu_info->menu_prompt);
        }
        // Pull prompt from menu text if exists.
    }

    return prompt;
}

/**
 * @brief Move to End of Display then output
 * @param output
 */
void MenuBase::moveToBottomAndDisplay(const std::string &prompt)
{
    std::string output = "";
    int screen_row = m_ansi_process->getMaxRowsUsedOnScreen();

    output += getDefaultColor();
    output += "\x1b[" + std::to_string(screen_row) + ";1H\r\n";
    output += std::move(prompt);
    baseProcessAndDeliver(output);
}

/**
 * @brief Move to End of Display then Setup Display for String
 * @param output
 */
std::string MenuBase::moveStringToBottom(const std::string &prompt)
{
    std::string output = "";
    int screen_row = m_ansi_process->getMaxRowsUsedOnScreen();

    output += getDefaultColor();
    output += "\x1b[" + std::to_string(screen_row) + ";1H\r\n";
    output += std::move(prompt);
    return output;
}

/**
 * @brief Startup And load the Menu File
 */
void MenuBase::loadAndStartupMenu()
{
    // Check Configuration here,  if use SpecialLogin (Matrix Menu)
    // Then load it, otherwise jump to Entering UserID / P
    std::cout << "Loading Matrix Menu -  Menu Input " << std::endl;

    // 1. Make sure the Input is set to the
    m_input_index = 0; // MENU_INPUT;

    m_active_pulldownID = 0;

    // Load the Menu ,, Clears All Structs.
    loadInMenu(m_current_menu);

    // Validate menu options loaded.
    if(m_menu_info->menu_options.size() < 1)
    {
        std::cout << "Menu has no menu_options: " << m_current_menu << std::endl;
        return;
    }

    // Pulldown filename can have (2) customization
    // N = single prompt string with Y/N light bar propmpts, used in goodybye, feedback, newscan menus..
    // ::X
    // Where X is a letter in the alphabet. Randomly picks a
    // letter from A to X, and will act as if the user pressed
    // that key.  For Random Matrixes.. or menu commands!

    // First Lets imploment N with ^ color codes for local theme colors.
    if (m_menu_info->menu_pulldown_file.size() == 1 && toupper(m_menu_info->menu_pulldown_file[0]) == 'N')
    {
        baseProcessAndDeliver(parseMenuPromptString(m_menu_info->menu_prompt));
        m_is_active_pulldown_menu = true;

        // Not sure if this is allowed in legacy, but lets do it, then they can clear screen or add ansi!
        if (!m_use_first_command_execution)
        {
            // Onlu Execute Each Command
            executeEachCommands();
        }
        else
        {
            // Execute Both
            executeFirstAndEachCommands();
        }

        return;
    }


    //if (MenuInfo clear the scrren etc.. feature to add! )
    //m_menu_session_data->deliver("\x1b[2J\x1b[1;1H");

    // Finally parse the ansi screen and remove pipes
    // Read in the Menu ANSI
    std::string buffer = loadMenuScreen();

    // Output has parsed out MCI codes, translations are then appended.
    std::string output = m_session_io.pipe2ansi(buffer);

    // If we have a pulldown ansi, then setup pull down
    if(m_menu_info->menu_pulldown_file.size() != 0)
    {
        // Get Pulldown menu commands, Load all from menu options (disk)
        std::vector<int> pull_down_ids;
        for(unsigned int i = 0; i < m_menu_info->menu_options.size(); i++)
        {
            auto &m = m_menu_info->menu_options[i];
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

        // If active pull_down id's found, mark as active pulldown menu.
        if(pull_down_ids.size() > 0 && m_menu_session_data->m_is_use_ansi)
        {
            // Hide Cursor on lightbars
            output += "\x1b[?25l";

            // m_menu_info.PulldownFN
            m_is_active_pulldown_menu = true;

            // Parse the Screen to the Screen Buffer.
            m_ansi_process->parseAnsiScreen((char *)buffer.c_str());

            // Screen to String so it can be processed.
            m_ansi_process->screenBufferToString();

            // Process buffer for PullDown Codes. results for TESTING, are discarded.
            std::string result = std::move(m_ansi_process->screenBufferParse());


            std::cout << " *** push out lightbars *** " << std::endl;
            // Now Build the Light bars
            std::string light_bars = buildLightBars();

            // add and write out.
            output.append(light_bars);
        }
        else
        {
            m_is_active_pulldown_menu = false;
        }
    }
    else
    {
        m_is_active_pulldown_menu = false;
    }

    // Loads the users selected menu prompt
    output += loadMenuPrompt();
    baseProcessAndDeliver(output);

    if (!m_use_first_command_execution)
    {
        // Onlu Execute Each Command
        executeEachCommands();
    }
    else
    {
        // Execute Both
        executeFirstAndEachCommands();
    }
}

/**
 * @brief Updates current and next lightbar positions.
 */
void MenuBase::lightbarUpdate(int previous_pulldown_id)
{

    std::cout << " *** lightbar update! *** " << std::endl;
    // Draw Light bars, use next item to determine next/previous id to pull.
    std::string light_bars = "";
    // Moved to Next Item
    // Turn off Previous Bar
    light_bars.append("\x1b[s"); // Save Cursor Position for prompt.
    light_bars.append(m_ansi_process->buildPullDownBars(previous_pulldown_id, false));

    // Grab Previous
    for(unsigned int i = 0; i < m_loaded_pulldown_options.size(); i++)
    {
        auto &m = m_loaded_pulldown_options[i];
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
    for(unsigned int i = 0; i < m_loaded_pulldown_options.size(); i++)
    {
        auto &m = m_loaded_pulldown_options[i];
        if(m.pulldown_id == m_active_pulldownID)
        {
            light_bars.append(m.name);
            break;
        }
    }

    // Clear Attriutes, then move back to menu prompt position.
    light_bars.append("\x1b[0m\x1b[u");
    std::string output = std::move(m_session_io.pipe2ansi(light_bars));
    baseProcessAndDeliver(output);
}

/**
 * @brief Process Command Keys passed from menu selection
 * @param input
 */
bool MenuBase::executeMenuOptions(const MenuOption &option)
{
    // If Invalid then return
    if(m_execute_callback.size() == 0 || option.command_key.size() != 2)
    {
        return false;
    }

    // Execute Menu Option Commands per Callback
    return m_execute_callback.back()(option);
}

/**
 * @brief Handle Standard Menu Input with Wildcard input
 * @param input
 * @param key
 * @return
 */
bool MenuBase::handleStandardMenuInput(const std::string &input, const std::string &key)
{
    /**
     * There is wildcarding for menu commands:
     * If you set the Key to X*, then you can put * in the
     * Cstring and that will put what follows the X in the
     * Cstring. This is advisable for such cases as file
     * conference jumping such as J* with would do JM with a
     * Cstring of * so one could J1,J2, etc.
     *
     * Also a possibility for CString is & in which is set to the
     * input_text gotten with -I, -J, or set with -*.
     *
     *
     * Note 2, * can also be used as a wild card alone for stacked
     * commands to always run after any input.. for like return to a menu
     * on yes/ no..  yes executes then does * to return,, n just returns on *
     */

    std::cout << "STANDARD INPUT: " << input << " KEY: " << key << std::endl;

    using namespace boost::locale;
    using namespace std;
    generator gen;
    locale loc=gen("");
    locale::global(loc);
    cout.imbue(loc);

    // Check for wildcard command input.
    std::string::size_type idx;
    idx = key.find("*", 0);

    // Catch Lightbar Movement when checking wilecards.
    // Return nothing found on Movement.
    if (idx != std::string::npos && m_is_active_pulldown_menu &&
            (input == "RT_ARROW" || input == "DN_ARROW" ||
             input == "LT_ARROW" || input == "UP_ARROW"))
    {
        return false;
    }

    // If it exists, grab text up to *, then test aginst input.
    // Check for Wildcard input .. A* would be any keys starting with A
    if (idx != std::string::npos && idx != 0)
    {
        // Match Strings to the same size.
        std::string key_match = key.substr(0, idx);
        std::string input_match = input.substr(0, m_common_io.numberOfChars(key_match));

        std::cout << "key_match: " << key_match << std::endl;
        std::cout << "input_match: " << input_match << std::endl;

        // Normalize and upper case for testing key input
        key_match = boost::locale::to_upper(key_match);
        input_match = boost::locale::to_upper(input_match);

        // If we have a match, execute
        if (key_match == input_match)
        {
            return true;
        }
        // Else if Pulldown, and ENTER, then take wild card!
        //else if (m_is_active_pulldown_menu && input_match == "ENTER")
        //{
        //    return true;
        //}
        return false;
    }
    else if (idx == 0)
    {
        std::cout << "Wild Card Key * By Itself: " << key << std::endl;
        return true;
    }

    std::string key_normalized = boost::locale::to_upper(key);
    std::string input_normailized = boost::locale::to_upper(input);

    // Handle one to one matches.
    if (input_normailized.compare(key_normalized) == 0)
    {
        std::cout << "Match Found " << input_normailized << std::endl;
        return true;
    }

    return false;
}

/**
 * @brief Handle updating lightbar selections and redraw
 * @param input
 * @return
 */
bool MenuBase::handleLightbarSelection(const std::string &input)
{
    // Handle ESC and Sequences
    int executed = 0;
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
        ++executed;
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
        ++executed;
    }
    else
    {
        // Add home end.  page etc..
        std::cout << "lightbar ELSE!" << std::endl;
    }

    if (executed > 0)
    {
        return true;
    }
    return false;
}

/**
 * @brief Handle Pulldown Specific Command Processing
 * @param input_text
 * @param m
 * @param is_enter
 * @return
 */
bool MenuBase::handlePulldownHotKeys(const MenuOption &m, const bool &is_enter, bool &stack_reassignment)
{
    std::string current_menu = m_current_menu;
    int executed = 0;

    // First Check for Execute on LightBar Selection.
    // If no valid pulldown id matched the active pulldown ID, then it's not valid.
    if(is_enter)
    {
        // Process the current active pull down ID.
        // Check Pull down commands
        if(m.pulldown_id == m_active_pulldownID)
        {
            // Then we have a match!  Execute the Menu Command with this ID!
            std::cout << "[ENTER] Menu Command HOTKEY Executed for: " << m.menu_key << std::endl;

            if(m.menu_key != "FIRSTCMD" && m.menu_key != "EACH")
            {
                /**
                 * Note, if command doesn't excute, next command doesn't follow!
                 */
                if (executeMenuOptions(m))
                {
                    // If the menu changed after executing the command
                    // then we are done, leave gracefully.
                    if (current_menu != m_current_menu || m_logoff)
                    {
                        return false;
                    }

                    std::cout << "set stack_reassignment = true " << std::endl;
                    // Now assign the m.menu_key to the input, so on next loop, we hit any stacked commands!
                    // If were in pulldown menu, and the first lightbar has stacked commands, then we need
                    // to cycle through the remaining command's for stacked on lightbars.
                    stack_reassignment = true;
                    ++executed;
                }

                /*
                // Testing for Stack Reassignment on FeedBack Lightbars
                executeMenuOptions(m);
                std::cout << "set stack_reassignment = true " << std::endl;
                // Now assign the m.menu_key to the input, so on next loop, we hit any stacked commands!
                // If were in pulldown menu, and the first lightbar has stacked commands, then we need
                // to cycle through the remaining command's for stacked on lightbars.
                stack_reassignment = true;
                ++executed;
                */
            }
        }
    }
    else
    {
        // NOT ENTER and pulldown,  check hotkeys here!!
        std::cout << "[HOTKEY] Menu Command HOTKEY Executed for: " << m.menu_key << std::endl;
        if (executeMenuOptions(m))
        {
            // If the menu changed after executing the command
            // then we are done, leave gracefully.
            if (current_menu != m_current_menu || m_logoff)
            {
                return false;
            }
            ++executed;
        }
        // More testing here.. executeMenuOptions( ... );
    }

    if (executed > 0)
    {
        return true;
    }
    return false;
}

/**
 * @brief Handles Re-running EACH command re-executed after each refresh
 */
void MenuBase::executeEachCommands()
{
    // Then do not loop and execute this!
    // Get Pulldown menu commands, Load all from menu options (disk)
    for(unsigned int i = 0; i < m_menu_info->menu_options.size(); i++)
    {
        auto &m = m_menu_info->menu_options[i];
        // Process Each should onlu be done, before return to the menu, after completed
        // All if any stacked menu commands.
        if(m.menu_key == "EACH")
        {
            // Process, although should each be execure before, or after a menu command!
            // OR is each just on each load/reload of menu i think!!
            std::cout << "FOUND EACH! EXECUTE: " << m.command_key << std::endl;
            executeMenuOptions(m);
        }
    }
}

/**
 * @brief Processes Menu Commands with input.
 * @param input
 */
bool MenuBase::processMenuOptions(const std::string &input)
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
    int  executed = 0;
    int  executedLightBarMovement = 0;

    // For checking if the menu has changed from an executed option
    std::string current_menu = m_current_menu;

    // For lightbar [ENTER] Selections, stuff with menu key for stacked commands
    // On lightbars so any following menu options are executed in order.
    bool stack_reassignment = false;

    // Uppercase all input to match on comamnd/option keys
    // TODO, use boost local for upper case local!!
    std::string input_text = boost::locale::to_upper(input);

    // Check if ENTER was hit as a command!
    if(input_text == "ENTER")
    {
        std::cout << "EXECUTE ENTER!: " << input_text << std::endl;
        is_enter = true;

        // Push out a NewLine after ENTER Executions
        //baseProcessAndDeliver("\r\n");
        // we want a new line, but if execution fails, then display is
        // side stepped and pushed down one!
    }

    // Check for loaded menu commands.
    // Get Pulldown menu commands, Load all from menu options (disk)
    for(unsigned int i = 0; i < m_menu_info->menu_options.size(); i++)
    {
        auto &m = m_menu_info->menu_options[i];

        std::cout << "MENU KEY: " << m.menu_key << std::endl;
        std::cout << "Input: " << input_text << std::endl;

        // Skip all first CMD's.. where only processing input here.
        // FIRSTCMD are executed when the menu loads.
        if (m.menu_key == "FIRSTCMD")
        {
            continue;
        }

        // Catch Lightbars input is RT_ARROW, LT_ARROW, etc..
        if(input_text[0] == '\x1b' && input_text.size() > 2) // hmm 2?
        {
            // Remove leading ESC for cleaner comparisons.
            std::string clean_sequence = input_text;
            clean_sequence.erase(0,1);

            // Handle Pull Down Options for Lightbars only.
            if (m_is_active_pulldown_menu)
            {
                // First Make sure the pulldown menu, doesn't have menu keys set to specific
                // Control Sequence,  If so, they are normal menu commands, execute first
                // Instead of lightbar interaction.
                std::cout << "Handle 1 " << m.menu_key << std::endl;
                if (handleStandardMenuInput(clean_sequence, m.menu_key))
                {
                    std::cout << "STANDARD MATCH, EXECUTING " << m.menu_key << std::endl;
                    if (executeMenuOptions(m))
                    {
                        ++executed;
                    }
                }
                else
                {
                    // handle Pull Down Lightbar Changes Movement Keys
                    // Should only execute once for movement! in loop of all options.
                    if (executedLightBarMovement == 0)
                    {
                        if (handleLightbarSelection(clean_sequence))
                        {
                            ++executed;
                            ++executedLightBarMovement;
                        }
                    }
                }
            }
            else
            {
                // Handle Standard Input for CONTROL KEYS.
                std::cout << "Handle 2 " << m.menu_key << std::endl;
                if (handleStandardMenuInput(clean_sequence, m.menu_key))
                {
                    std::cout << "STANDARD MATCH, EXECUTING " << m.menu_key << std::endl;
                    if (executeMenuOptions(m))
                    {
                        ++executed;
                    }
                }
            }
        }

        // Check for ESC sequence, and next/prev lightbar movement.
        else if(input_text[0] == '\x1b')
        {
            // Received ESC key,  check for ESC is menu here..
            ++executed;
        }

        // Check Input Keys on Both Pulldown and Normal Menus
        // If the input matches the current key, or Enter is hit, then process it.
        else if(input_text.compare(m.menu_key) == 0 || (m_is_active_pulldown_menu && is_enter))
        {
            // Pulldown selection.
            if(m_is_active_pulldown_menu)
            {

                std::cout << "handlePulldownHotKeys" << std::endl;

                // Handles ENTER Selection or Hotkeys Command Input.
                if (handlePulldownHotKeys(m, is_enter, stack_reassignment))
                {

                    if (m_logoff)
                    {
                        return false;
                    }

                    // If Pulldown option was selected on Enter, make sure following commands
                    // With Same Menu Key are executed (stacked commands) afterwords in order.
                    if (stack_reassignment && is_enter)
                    {
                        std::cout << "stack_reassignment TRUE, KEY: " << m.menu_key << std::endl;
                        input_text.clear();
                        input_text = m.menu_key;
                        stack_reassignment = false;
                        is_enter = false;
                    }
                    ++executed;
                }
            }
            else
            {
                // They m.menu_key compared, execute it
                std::cout << "ENTER OR HOT KEY MATCH and EXECUTE! " << m.menu_key << std::endl;
                if (executeMenuOptions(m))
                {
                    ++executed;
                }
            }
        }
        else
        {
            // Handle Standard Menu, Input Field processing.
            std::cout << "Handle 3 " << m.menu_key << std::endl;
            if (handleStandardMenuInput(input_text, m.menu_key))
            {
                std::cout << "STANDARD MATCH, EXECUTING " << m.menu_key << std::endl;
                if (executeMenuOptions(m))
                {
                    ++executed;
                }
            }
        }

        // If menu changed, then exit out.
        if (current_menu != m_current_menu || m_logoff)
        {
            return false;
        }
    }



    // Check for Change Menu before this point, if we changed the menu
    // Then do not re-execute menu commands for previous menu
    // Each New Menu Load does handle this the first time.
    if (current_menu == m_current_menu)
    {
        executeEachCommands();
    }
    else
    {
        // Menu Changed, exit and leave startup to next menu.
        return true;
    }

    // Track Executed Commands, If we didn't execute anything
    // By user input_text, then clear the menu prompt input field
    if (executed > 0)
    {
        return true;
    }

    return false;
}

/**
 * @brief Handle Input Specific to Pull Down Menus
 * @param character_buffer
 */
void MenuBase::handlePulldownInput(const std::string &character_buffer, const bool &is_utf8)
{
    // Get hotmay and lightbar input.
    std::string result = m_session_io.getKeyInput(character_buffer);
    std::string input = "";

    if(result.size() == 0)
    {
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
    else
    {
        // Hot Key Input.
        input = result;
    }
    // Process CommandOptions Matching the Key Input.
    // Need to check for wildcard input there with menu option.
    processMenuOptions(input);
}

/**
 * @brief Handle Input Specific to Pull Down Menus
 * @param character_buffer
 */
void MenuBase::handleStandardInput(const std::string &character_buffer)
{
    // Get LineInput and wait for ENTER.
    std::string key = "";
    std::string result = m_session_io.getInputField(character_buffer, key, Config::sMenuPrompt_length);

    //std::cout << "result: " << result << std::endl;
    if(result == "aborted") // ESC was hit, make this just clear the input text, or start over!
    {
        std::cout << "ESC aborted!" << std::endl;
    }
    else if(result[0] == '\n')
    {
        // Key == 0 on [ENTER] pressed alone. then invalid!
        // TODO, might have menu keys with ENTER, update this lateron!!
        if(key.size() == 0)
        {
            // Return and don't do anything.
            return;
        }

        // Extra test, if the menu changed, then don't clear input!
        std::string tmp_menu = m_current_menu;

        // Process incoming String from Menu Input up to ENTER.
        // If no commands were processed, erase all prompt text
        if (!processMenuOptions(key) && tmp_menu == m_current_menu)
        {
            // Clear Menu Field input Text, redraw prompt?
            std::string clear_input = "\x1b[0m";
            for(int i = m_common_io.numberOfChars(key); i > 0; i--)
            {
                clear_input += "\x1b[D \x1b[D";
            }
            baseProcessAndDeliver(clear_input);
        }
    }
    else
    {
        // Send back the single input received to show client key presses.
        // Only if return data shows a processed key returned.
        if (result != "empty")
        {
            std::string output = getDefaultInputColor();
            output.append(result);
            baseProcessAndDeliver(output);
        }
    }
}

/**
 * @brief Default Menu Input Processing.
 *        Handles Processing for Loaded Menus Hotkey and Lightbars
 */
void MenuBase::menuInput(const std::string &character_buffer, const bool &is_utf8)
{
    std::cout << " *** menuInput" << std::endl;

    // If were in lightbar mode, then we are using hotkeys.
    if (m_is_active_pulldown_menu)
    {
        std::cout << "handlePulldown" << std::endl;
        handlePulldownInput(character_buffer, is_utf8);
    }
    else
    {
        std::cout << "handleStandard" << std::endl;
        handleStandardInput(character_buffer);
    }
}

/**
 * @brief Default Menu Input Processing. (HotKey and Lightbar)
 *        Handles Processing for Loaded Menus Hotkey and Lightbars
 */
void MenuBase::menuYesNoBarInput(const std::string &character_buffer, const bool &is_utf8)
{
    std::cout << "*** yesNO Menu Bar Input" << std::endl;
    handlePulldownInput(character_buffer, is_utf8);
}
