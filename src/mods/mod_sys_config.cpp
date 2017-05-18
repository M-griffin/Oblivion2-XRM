#include "mod_sys_config.hpp"

#include "../forms/form_system_config.hpp"
#include "../form_manager.hpp"
#include "../menu_base.hpp"

#include <boost/smart_ptr/shared_ptr.hpp>

ModSysConfig::ModSysConfig(session_data_ptr session_data, config_ptr config, ansi_process_ptr ansi_process)
    : ModBase(session_data, config, ansi_process)
    , m_menu(new MenuBase(session_data))
    , m_form_manager(new FormManager(config))
        
{
    
    //m_menu_functions.push_back(std::bind(&MenuSystem::formInput, this, std::placeholders::_1, std::placeholders::_2));

    //m_execute_callback.push_back(std::bind(&MenuSystem::menuOptionsCallback, this, std::placeholders::_1));
    
}


bool ModSysConfig::update(const std::string &character_buffer, const bool &)
{
    //if(!m_is_active)
    //{
    //    return;
    //}

    // This simplily passed through the input to the current system fuction were at.
    //m_menu_functions[m_input_index](character_buffer, is_utf8);
    
    return true;
}

bool ModSysConfig::onEnter() 
{

    return true;
}

bool ModSysConfig::onExit()
{
    return true;
}


/**
 * @brief Starts up Form Manager Module.
 */
void ModSysConfig::startupFormManager()
{
    // Setup the input processor
    //resetMenuInputIndex(FORM_INPUT);

    // Startup a new Form manager instance.
    m_form_manager.reset(new FormManager(m_config));
    
    if (m_form_manager)
    {
        m_form_manager->startupFormSystemConfiguration();
        
    }
}


/**
 * @brief Handles parsing input for forms
 */
void ModSysConfig::formInput(const std::string &character_buffer, const bool &is_utf8)
{
    //handlePulldownInput(character_buffer, is_utf8);    
}
