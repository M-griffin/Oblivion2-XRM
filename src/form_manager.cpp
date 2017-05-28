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
    m_menu_info->menu_options = m_form.back()->baseGetFormOptions();

    return m_menu_info;
}
