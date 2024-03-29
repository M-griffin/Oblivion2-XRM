#include "mod_form_manager.hpp"

#include "model-sys/config.hpp"
#include "model-sys/menu.hpp"

#include "../forms/form_system_config.hpp"
//#include "../form_manager.hpp"

#include "../processor_ansi.hpp"
#include "../session.hpp"
#include "../logging.hpp"
#include "../menu_base.hpp"


ModFormManager::ModFormManager(session_ptr session_data, config_ptr config, processor_ansi_ptr ansi_process,
        common_io_ptr common_io, session_io_ptr session_io)
    : ModBase(session_data, config, ansi_process, nullptr, common_io, session_io)
    , m_current_page(0)
    , m_menu(nullptr)
//    , m_form_manager(new FormManager(config, session_data))
{
    // Setup Smart Pointers
    m_menu = std::make_shared<MenuBase>(session_data);
    
    // Setup the Callback to Menu Option Execution to this base class.
    m_menu->m_execute_callback.push_back(std::bind(&ModFormManager::menuOptionsCallback, this, std::placeholders::_1));
}


/**
 * @brief Update Method, Receives Input Passed Through
 * @param character_buffer
 * @param is_utf8
 * @return
 */
bool ModFormManager::update(const std::string &character_buffer, const bool &is_utf8)
{
    // Were calling single input method, we don't need dynamic input call backs here.
    m_menu->menuInput(character_buffer, is_utf8);
    return true;
}

/**
 * @brief Executes on Module Load
 * @return
 */
bool ModFormManager::onEnter()
{
    startupFormManager();
    return true;
}

/**
 * @brief Executes on Module Exit
 * @return
 */
bool ModFormManager::onExit()
{
    return true;
}

/**
 * @brief Process Command Keys passed from menu selection (Callback)
 * @param option
 */
bool ModFormManager::menuOptionsCallback(const MenuOption &option)
{
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
void ModFormManager::startupFormManager()
{
    /* TODO FIXME
    // Startup a new Form manager instance.
    m_form_manager.reset(new FormManager(m_config, m_session_data));

    if(m_form_manager)
    {
        m_form_manager->startupFormSystemConfiguration();
        menu_ptr menu = m_form_manager->retrieveFormOptions(m_current_page);

        // Load the Form Menu Options into the Menu System.
        m_menu->importMenu(menu);
    }
    */
}
