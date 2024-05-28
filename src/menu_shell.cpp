#include "menu_shell.hpp"

#include <locale>
#include <string>
#include <vector>
#include <functional>
#include <cassert>

#include "menu_base.hpp"
#include "logging.hpp"
#include "session.hpp"

MenuShell::MenuShell(session_ptr session_data)
    : StateBase(session_data)
    , MenuBase(session_data)
    , m_log(Logging::getInstance())
{
    m_log.write<Logging::CONSOLE_LOG>("MenuShell()");
}

MenuShell::~MenuShell()
{
    m_log.write<Logging::CONSOLE_LOG>("~MenuShell()");
}

/**
 * @brief Handles Updates or Data Input from Client
 */
void MenuShell::update(const std::string &character_buffer)//is_utf8)
{
    if(!m_is_active)
    {
        return;
    }
    
    m_log.write<Logging::CONSOLE_LOG>("character_buffer", character_buffer);
    
    baseProcessAndDeliver(character_buffer);
    if (character_buffer[0] == 'g' || character_buffer[0] == 'G') 
    {
        // Disconnect the fricking user here!@! 
    }
}

/**
 * @brief Startup class, setup initial screens / interface, flags etc..
 *        This is only called when switch to the state, not for menu instances.
 * @return
 */
bool MenuShell::onEnter()
{
    // Startup the Prelogon sequence
    /* TODO  bombing Here!
    startupModulePreLogon();
    m_is_active = true;
    return true;
    */
    m_logoff = true;
    
    session_ptr session = m_session_data.lock();
    session->logoff();
            
    m_is_active = true;
    
    return true;
}

/**
 * @brief Exit, close down, display screens to change over data.
 * @return
 */
bool MenuShell::onExit()
{
    m_is_active = false;
    return true;
}
