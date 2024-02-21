#include "menu_shell.hpp"
#include "menu_base.hpp"

#include <locale>
#include <string>
#include <vector>
#include <functional>
#include <cassert>


#include "logging.hpp"

MenuShell::MenuShell(session_ptr session_data)
    : StateBase(session_data)
    , MenuBase(session_data)
    , m_log(Logging::getInstance())
{
}

MenuShell::~MenuShell()
{
    m_log.write<Logging::DEBUG_LOG>("~MenuShell()");
}

/**
 * @brief Handles Updates or Data Input from Client
 */
void MenuShell::update(const std::string &character_buffer, const bool &)//is_utf8)
{
    if(!m_is_active)
    {
        return;
    }
    
    m_log.write<Logging::DEBUG_LOG>("character_buffer", character_buffer);
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
