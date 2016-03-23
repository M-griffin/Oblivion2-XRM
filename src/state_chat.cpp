#include "state_chat.hpp"
#include "session.hpp"

#include <iostream>
#include <string>

const std::string ChatState::m_menuID = "LOGIN";

/**
 * @brief Handles Updates or Data Input from Client
 */
void ChatState::update(std::string character_buffer, bool is_utf8)
{
    if (!m_is_active)
    {
        return;
    }

    std::cout << "CHAT_STATE Single-Key: " << character_buffer << " | UTF-8: " << is_utf8 << std::endl;
}

/**
 * @brief Startup class, setup and display initial screens / interface.
 * @return
 */
bool ChatState::onEnter()
{
    std::cout << "OnEnter() ChatState\n";
    m_is_active = true;
    return true;
}

/**
 * @brief Exit, close down, display screens to change over data.
 * @return
 */
bool ChatState::onExit()
{
    std::cout << "OnExit() ChatState\n";
    m_is_active = false;
    return true;
}

