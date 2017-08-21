#include "mod_sys_config.hpp"

#include "../model-sys/menu.hpp"
#include "../forms/form_system_config.hpp"
#include "../form_manager.hpp"
#include "../menu_base.hpp"


#include <boost/smart_ptr/shared_ptr.hpp>

ModSysConfig::ModSysConfig(session_data_ptr session_data, config_ptr config, ansi_process_ptr ansi_process)
    : ModBase(session_data, config, ansi_process)
    , m_current_page(0)
    , m_menu(new MenuBase(session_data))
    , m_form_manager(new FormManager(config, session_data))
{
    // Setup the Callback to Menu Option Exection to this base class.
    m_menu->m_execute_callback.push_back(std::bind(&ModSysConfig::menuOptionsCallback, this, std::placeholders::_1));
}


/**
 * @brief Update Mehtod, Receives Input Passed Through
 * @param character_buffer
 * @param is_utf8
 * @return 
 */
bool ModSysConfig::update(const std::string &character_buffer, const bool &is_utf8)
{
    // Were calling single input method, we don't need dynamic input call backs here.
    m_menu->menuInput(character_buffer, is_utf8);
    return true;
}

/**
 * @brief Executes on Module Load
 * @return 
 */
bool ModSysConfig::onEnter() 
{
    startupFormManager();
    return true;
}

/**
 * @brief Executes on Module Exit
 * @return 
 */
bool ModSysConfig::onExit()
{
    return true;
}

/**
 * @brief Process Command Keys passed from menu selection (Callback)
 * @param option
 */
bool ModSysConfig::menuOptionsCallback(const MenuOption &option)
{
    std::cout << "ModSysConfig::menuOptionsCallback!" << std::endl;
    /* Run through the case and switch over the new interface.
    std::string mnuOption = option.CKeys;
    std::string mnuString = option.CString;
    std::string mnuAccess = option.Acs; */

    // If Invalid then return
    if(option.command_key.size() != 2)
    {
        return false;
    }

    return false;
}

/**
 * @brief Starts up Form Manager Module.
 */
void ModSysConfig::startupFormManager()
{
    // Startup a new Form manager instance.
    m_form_manager.reset(new FormManager(m_config, m_session_data));    
    if (m_form_manager)
    {
        m_form_manager->startupFormSystemConfiguration();
        menu_ptr menu = m_form_manager->retrieveFormOptions(m_current_page);
        
        // Load the Form Menu Options into the Menu System.
        m_menu->importMenu(menu);
    }
}

