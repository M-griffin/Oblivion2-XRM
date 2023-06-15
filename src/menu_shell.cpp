#include "menu_shell.hpp"
#include "menu_base.hpp"

#include "logging.hpp"

#include <locale>
#include <string>
#include <vector>
#include <functional>
#include <cassert>


MenuShell::MenuShell(session_ptr session_data)
    : StateBase(session_data)
    , MenuBase(session_data)
    , m_log(Logging::getInstance())
{
    std::cout << "MenuShell Startup()" << std::endl;
}

MenuShell::~MenuShell()
{
    std::cout << "~MenuShell()" << std::endl;
}

/**
 * @brief Handles Updates or Data Input from Client
 */
void MenuShell::update(const std::string &character_buffer, const bool &)//is_utf8)
{
    if(!m_is_active)
    {
        std::cout << "MenuShell: m_is_active: " << m_is_active << std::endl;
        return;
    }

    std::cout << "MenuShell: Data Received: " << character_buffer << std::endl;
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
