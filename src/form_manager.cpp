#include "form_manager.hpp"

#include "model-sys/menu.hpp"
#include "forms/form_system_config.hpp"

#include <iostream>
#include <string>
#include <vector>


FormManager::FormManager(config_ptr config)
        : m_config(config)
        , m_menu_info(new Menu())
        , max_cmds_per_page(15)
        , m_current_page(1)
        , m_total_pages(1)
        , m_form_name("")
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
    std::vector<MenuOption> opts = m_form.back()->baseGetFormOptions();
    
    //m_menu_info->menu_options = 
    
    return m_menu_info;
}


/**
 * TODO NOTE: Move to base lateron for all forms!
 * @brief Calculates Pages in Vector of Menu Options.
 * List = current area.
 * @param page
 * @param list
 */
std::vector<MenuOption> FormManager::box_start(unsigned long page, unsigned long list)
{
    std::vector<MenuOption> page_options;
    
    // Calcuate Box Size and Total Pages
    // Use this lateron with screen height / size to 
    // get dynamic page lengths.
    //int boxsize = Bot - Top; // Fist Get Box Size
    
    
    /*Tot = listing.size();
    TotPages = Tot / boxsize;
    if (Tot % boxsize > 0)
    {
        ++TotPages;
    }
    if (Tot <= boxsize)
        TotPages = 1;


	// This doesn't work in full screen message read?!?!?!
	// Testing, only clear the box, if total pages > 1,
	// Otherwise no need to clear since it's fresh draw.
	if (TotPages > 1)
	{
		//Now clear the box First
		for (int t = 0; t < boxsize; t++)
		{
		    sprintf(capture, "\x1b[%i;%iH\x1b[K", (Top)+t, 1);
		    _editbox += capture;
		}
	}

    // Now Grab as many lines as will fit in the box
    for (int i = 1; i < boxsize+1; i++)
    {
        if (((boxsize*Page)+i)-1 >= (signed)listing.size()) break;

        // If Area has new message rotate output to new lightbars.
        if (listing[((boxsize*Page)+i)-1].isnew)
        {
            if ((signed)list+1 == (boxsize*Page)+i)
            {
                // Current Area
                current_selection = Top+i-1; // Get current place in box to display.
                sprintf(capture, "\x1b[%i;%iH%s", Top+i-1, 1, (char *)listing[((boxsize*Page)+i)-1].ansi_4.c_str());
            }
            else
            {
                sprintf(capture, "\x1b[%i;%iH%s", Top+i-1, 1, (char *)listing[((boxsize*Page)+i)-1].ansi_3.c_str());
            }
        }
        else
        {
            if ((signed)list+1 == (boxsize*Page)+i)
            {
                // Current Area
                current_selection = Top+i-1; // Get current place in box to display.
                sprintf(capture, "\x1b[%i;%iH%s", Top+i-1, 1, (char *)listing[((boxsize*Page)+i)-1].ansi_2.c_str());
            }
            else
            {
                sprintf(capture, "\x1b[%i;%iH%s", Top+i-1, 1, (char *)listing[((boxsize*Page)+i)-1].ansi_1.c_str());
            }
        }

        _editbox += capture;
    }

    // Write out Box.
    sprintf(capture, "\x1b[%i;%iH", Row+Top-1, 1);
    _editbox += capture;
    pipe2ansi((char *)_editbox.c_str());*/

}
