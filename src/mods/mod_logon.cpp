#include "mod_logon.hpp"

#include <iostream>
#include <string>


/**
 * @brief Handles Updates or Data Input from Client
 * @return Bool {False = Module is completed, True Means still processing}
 */
bool ModLogon::update(const std::string &character_buffer, const bool &)
{
    // Make sure system is active.
    if (!m_is_active)
    {
        return false;
    }

    if (character_buffer.size() == 0)
    {
        return true;
    }
    
    // Handle Input processing here

     /*  Hotkey input working!
    std::cout << "getKeyInput" << std::endl;
    std::string result = m_menu_io.getKeyInput(character_buffer);
    std::cout << "m_session_data->deliver(result);" << std::endl;
    m_session_data->deliver(result);
    */

    // std::cout << "getInputField" << std::endl;
    /*
    std::string input = "";
    std::string result = m_session_io.getInputField(character_buffer, input);
    if(result == "aborted")
    {
        std::cout << "aborted!" << std::endl;
    }
    else if(result[0] == '\n')
    {
        // Send back the entire string.  TESTING
        // This should then be processed becasue ENTER was hit.
        m_session_data->deliver(input);
    }
    else
    {
        // Send back the single input received TESTING
        m_session_data->deliver(result);
    }*/

    return true;
}

/**
 * @brief Startup class, setup and display initial screens / interface.
 * @return
 */
bool ModLogon::onEnter()
{
    std::cout << "OnEnter() ModLogin\n";
    m_is_active = true;

    return true;
}

/**
 * @brief Exit, close down, display screens to change over data.
 * @return
 */
bool ModLogon::onExit()
{
    std::cout << "OnExit() ModLogin\n";
    m_is_active = false;
    return true;
}

