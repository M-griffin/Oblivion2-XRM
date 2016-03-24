#include "mod_logon.hpp"

#include <iostream>
#include <string>


/**
 * @brief Handles Updates or Data Input from Client
 * @return bool, not used anymore?!?
 */
bool ModLogon::update(const std::string &character_buffer, const bool &)
{
    // Make sure system is active, when system is done, success or failes
    // We change this is inactive to single the login process is completed.
    if(!m_is_active)
    {
        return false;
    }

    // Return True when were keeping module active / else false;
    if(character_buffer.size() == 0)
    {
        return true;
    }

    // std::cout << "getInputField" << std::endl;

    std::string input = "";
    std::string result = m_session_io.getInputField(character_buffer, input);
    if(result == "aborted") // ESC was hit, make this just clear the input text, or start over!
    {
        std::cout << "aborted!" << std::endl;
    }
    else if(result[0] == '\n')
    {
        // Send back the entire string.  TESTING
        // This should then be processed becasue ENTER was hit.
        //m_session_data->deliver(input);
        m_session_data->deliver(result);

        // Process the completed input for the string.
        m_mod_functions[m_mod_function_index](input);
    }
    else
    {
        // Send back the single input received TESTING
        m_session_data->deliver(result);
    }

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

    // Grab ANSI Screen, display, if desired.. logon.ans maybe?

    // Execure the initial setup index.
    m_setup_functions[m_mod_function_index]();

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

/**
 * @brief Create Default Text Prompts for module
 */
void ModLogon::createTextPrompts()
{

    // Create Mapping to pass for file creation (default values)
    M_TextPrompt value;
    value[PROMPT_LOGON]    = std::make_pair("Displayed for Logon Prompt", "|15Logon: ");
    value[PROMPT_PASSWORD] = std::make_pair("Displayed for Password Prompt", "|15password: ");

    m_text_prompts_dao->writeValue(value);
}


/**
 * @brief Sets an indivdual module index.
 * @param mod_function_index
 */
void ModLogon::changeModule(int mod_function_index)
{
    // Set, and Execute the Setup module.
    m_mod_function_index = mod_function_index;
    m_setup_functions[m_mod_function_index]();
}


/**
 * @brief Validates user Logon
 * @return
 */
void ModLogon::setupLogon()
{

    std::cout << "setupLogon()" << std::endl;

    // Testing.
    m_text_prompts_dao->displayAll();

    // Test display

    M_StringPair result = m_text_prompts_dao->getPrompt(PROMPT_PASSWORD);

    std::cout << "TEST: " << result.first << ", " << result.second << std::endl;

}


/**
 * @brief Validates user logon password
 * @return
 */
void ModLogon::setupPassword()
{

}

/**
 * @brief Validates user logon password challenge questions
 * @return
 */
void ModLogon::setupPasswordChallenge()
{

}

/**
 * @brief Chanes user logon password
 * @return
 */
void ModLogon::setupPasswordChange()
{

}

/**
 * @brief Creates New User Account
 * @return
 */
void ModLogon::setupNewUserApplication()
{



}


/**
 * @brief Validates user Logon
 * @return
 */
bool ModLogon::logon(const std::string &input)
{
    bool result = false;
    if(input.size() != 0)
    {

    }

    return result;
}


/**
 * @brief Validates user logon password
 * @return
 */
bool ModLogon::password(const std::string &input)
{
    bool result = false;
    if(input.size() != 0)
    {

    }

    return result;
}

/**
 * @brief Validates user logon password challenge questions
 * @return
 */
bool ModLogon::passwordChallenge(const std::string &input)
{
    bool result = false;
    if(input.size() != 0)
    {

    }

    return result;
}

/**
 * @brief Chanes user logon password
 * @return
 */
bool ModLogon::passwordChange(const std::string &input)
{
    bool result = false;
    if(input.size() != 0)
    {

    }

    return result;
}

/**
 * @brief Creation of new user account
 * @return
 */
bool ModLogon::newUserApplication(const std::string &input)
{
    bool result = false;
    if(input.size() != 0)
    {

    }

    return result;
}
