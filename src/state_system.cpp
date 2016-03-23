#include "state_system.hpp"
#include "session.hpp"
#include "menu_system.hpp"

#include <iostream>
#include <string>

const std::string StateSystem::m_menuID = "SYSTEM";

/**
 * @brief Handles Updates or Data Input from Client
 */
void StateSystem::update(const std::string &character_buffer, const bool &is_utf8)
{
    if (!m_is_active)
    {
        return;
    }

    // Need Function Array here for what interface were calling for input from!
    // Return result can be a state changer or something else lateron!  open for debate
    m_menu_state.update(character_buffer, is_utf8);
}

/**
 * @brief Startup class, setup and display initial screens / interface.
 * @return
 */
bool StateSystem::onEnter()
{
    std::cout << "OnEnter() SystemState\n";
    m_is_active = true;

    // m_menu_state.readMenuAllPrompts();  // Testing!
    return true;
}

/**
 * @brief Exit, close down, display screens to change over data.
 * @return
 */
bool StateSystem::onExit()
{
    std::cout << "OnExit() SystemState\n";
    m_is_active = false;
    return true;
}

