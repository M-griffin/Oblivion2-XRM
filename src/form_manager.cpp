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

    m_total_pages = form->m_menu_options.size() / max_cmds_per_page;
    if (form->m_menu_options.size() % max_cmds_per_page > 1)
        ++m_total_pages;
        
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
    
    // Calc Top Rows
    ansi->parseAnsiScreen((char *)m_ansi_top.c_str());
    int top_rows = ansi->getMaxRowsUsedOnScreen();
    
    // Clear to recalc the screen fresh.
    ansi->clearScreen();
    
    // Calc Bottom Rows
    ansi->parseAnsiScreen((char *)m_ansi_bot.c_str());
    int bot_rows = ansi->getMaxRowsUsedOnScreen();
    
    // Calculate the Box Range
    m_box_top = top_rows + 1;
    m_box_bottom = m_session_data->m_telnet_state->getTermRows() - (bot_rows + 1);
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
 * @brief Pulls Generated Menu Options from Form
 * @param option
 */
menu_ptr FormManager::retrieveFormOptions(int ) //page)
{
    // Setup the Default Menu, and pull in Form Generated Options.
    m_menu_info->menu_name = m_form.back()->m_name;
    m_menu_info->menu_title = m_form.back()->m_title;
    m_menu_info->menu_pulldown_file = m_form.back()->m_pulldown_file;
        
    // Need to figure out paging options?
    m_loaded_options = m_form.back()->baseGetFormOptions();

    return m_menu_info;
}

/**
 * @brief Calculates Pages in Vector of Menu Options, pulls out options for a single page.
 * @param page
 * @param list
 */
std::vector<MenuOption> FormManager::box_start(long current_page)
{    
    // Clear Existing Page Options to refresh new.
    std::vector<MenuOption>().swap(m_page_options);    
    
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
        m_page_options.push_back(m_loaded_options[((boxsize*page)+i)-1]);        
    }

    return m_page_options;
}
