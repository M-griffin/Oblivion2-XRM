#include "form_manager.hpp"

#include "model-sys/menu.hpp"
#include "forms/form_system_config.hpp"
#include "ansi_processor.hpp"
#include "session_data.hpp"

#include <iostream>
#include <string>
#include <vector>


FormManager::FormManager(config_ptr config, session_data_ptr session)
    : m_config(config)
    , m_session_data(session)
    , m_menu_info(new Menu())
    , max_cmds_per_page(15)
    , m_current_page(1)
    , m_total_pages(1)
    , m_form_name("")
    , m_ansi_top("")
    , m_ansi_mid("")
    , m_ansi_bot("")
    , m_box_top(1)
    , m_box_bottom(24)
{
}

FormManager::~FormManager()
{
    std::cout << "~FormManager()" << std::endl;
}

/**
 * @brief Clears All Forms
 */
void FormManager::clearAllForms()
{
    if (m_form.size() > 0)
    {
        std::vector<form_ptr>().swap(m_form);
        std::vector<MenuOption>().swap(m_loaded_options);
    }
}

/**
 * @brief Exists and Shutsdown the current form
 */
void FormManager::shutdownForm()
{
    // Do module shutdown, only single modules are loaded
    // This makes it easy to allocate and kill on demand.
    m_form.back()->onExit();
    m_form.pop_back();
}

/**
 * @brief Start Up and execute current Form
 */
void FormManager::startupForm(form_ptr form)
{
    // First clear any left overs if they exist.
    clearAllForms();

    // Run the startup for the form
    form->onEnter();

    // Push to stack now the new form.
    m_form.push_back(form);
        
    // Ansi Processor used for Parsing templates and get line size(s).
    // This determines the screen template size and data we can fit between
    // Top and Bottom Templates.
    ansi_process_ptr ansi(new AnsiProcessor(
                        m_session_data->m_telnet_state->getTermRows(),
                        m_session_data->m_telnet_state->getTermCols())
                     );
    
    // Read in ANSI Templates related to the Form.
    m_ansi_top = m_common_io.readinAnsi(m_form.back()->m_ansi_top);
    m_ansi_mid = m_common_io.readinAnsi(m_form.back()->m_ansi_mid);
    m_ansi_bot = m_common_io.readinAnsi(m_form.back()->m_ansi_bot);
    
    // Calc Top Rows, get Ending Y Position
    ansi->parseAnsiScreen((char *)m_ansi_top.c_str());
    int top_rows = ansi->getYPosition();
    
    ansi->clearScreen();
    
    // Calc Bottom Rows
    ansi->parseAnsiScreen((char *)m_ansi_bot.c_str());
    int bot_rows = ansi->getMaxRowsUsedOnScreen();
    
    ansi->clearScreen();
    
    // Calculate the Box Range
    m_box_top = top_rows + 1;
    m_box_bottom = m_session_data->m_telnet_state->getTermRows() - (bot_rows + 1);
    
    // Parse and Process Mid ansi Template for menu display where.
}

/**
 * @brief Start up the Configuration Form.
 */
void FormManager::startupFormSystemConfiguration()
{
    // Allocate and Create
    form_ptr form(new FormSystemConfig(m_config));
    if (!form)
    {
        std::cout << "FormSystemConfig Allocation Error!" << std::endl;
        assert(false);
    }

    startupForm(form);
}

/**
 * @brief handles selected option for processing.
 * @param option
 */
void FormManager::processFormOption(MenuOption &option, std::string value)
{
    m_form.back()->updateNodeMapping(option, value);
}

/**
 * @brief Calculates Pages in Vector of Menu Options, pulls out options for a single page.
 * @param page
 * @param list
 */
void FormManager::buildPageOptions(std::vector<MenuOption> &page_options, int current_page)
{    
    // First Clear All Page Options
    std::vector<MenuOption>().swap(page_options);
    
    // Calcuate Box Size and Total Pages
    // Use this lateron with screen height / size to 
    // get dynamic page lengths.
    int boxsize = m_box_bottom - m_box_top;        
    int total_rows = m_loaded_options.size();
            
    m_total_pages = total_rows / boxsize;
    if (total_rows % boxsize > 0)
    {
        ++m_total_pages;
    }
    
    if (total_rows <= boxsize)
    {
        m_total_pages = 1;        
    }
    
    if (current_page < 1) 
    {
        current_page = 1;
    }
    
    // Paging is Zero Based for Array.
    int page = current_page - 1;

    // Now Grab as Records that will fit in the box
    for (int i = 1; i < boxsize+1; i++)
    {
        if (((boxsize*page)+i)-1 >= (signed)m_loaded_options.size()) 
            break;       
        page_options.push_back(m_loaded_options[((boxsize*page)+i)-1]);        
    }
}

/**
 * @brief Pulls Generated Menu Options from Form
 * @param option
 */
menu_ptr FormManager::retrieveFormOptions(int page)
{
    // Setup the Default Menu, and pull in Form Generated Options.
    m_menu_info->menu_name = m_form.back()->m_name;
    m_menu_info->menu_title = m_form.back()->m_title;
    m_menu_info->menu_pulldown_file = m_form.back()->m_pulldown_file;
           
    // Populate Menu with the Loaded Page Options.
    std::vector<MenuOption> page_options;
    buildPageOptions(page_options, page);
    m_menu_info->menu_options.swap(page_options);

    return m_menu_info;
}


/**
 * @brief Processes a TOP Template Screen
 * @param screen
 * @return
 */
std::string FormManager::processTopFormTemplate(const std::string &screen)
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
 * @brief Processes a MID Template Screen
 * @param screen
 * @return
 */
std::string FormManager::processMidFormTemplate(const std::string &screen)
{
    // Use a Local Ansi Parser for Pasrsing Menu Template with Mid.
    ansi_process_ptr ansi_process(new AnsiProcessor(
                                      m_session_data->m_telnet_state->getTermRows(),
                                      m_session_data->m_telnet_state->getTermCols()));
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
 * @brief Setup light bar string, and return default display.
 *
std::string MenuBase::setupMidOptions(const std::string &menu_prompt, std::vector<MapType> &code_map)
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
}*/

/**
 * @brief Builds the menu prompt as a question String
 * @return
 *
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
}*/

/**
 * @brief SRT, MID, END screen processing
 * @return
 */
std::string FormManager::processFormScreens()
{   
    std::string screen_output = "";

    // Add the Top section of the template
    // Do a simple MCI Code replace for title
    // |TI - Menu Title
    std::string::size_type idx = m_ansi_top.find("|TI");
    if (idx != std::string::npos)
    {
        std::cout << "parsing form code title" << std::endl;
        m_ansi_top.replace(
            idx,
            3,
            m_menu_info->menu_title
        );
    }

    screen_output += processTopFormTemplate(m_ansi_top);

    // |K? - key,  |D? - Description
    //|K1 |D1   |K2  |D2  |K3  |D3 ...
    screen_output += processMidFormTemplate(m_ansi_mid);
    screen_output += m_ansi_bot;
    return screen_output;
}
