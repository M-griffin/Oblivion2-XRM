#include "mod_logon.hpp"
#include "../model/config.hpp"

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
        std::cout << "logon() !m_is_active" << std::endl;
        return false;
    }

    // Return True when were keeping module active / else false;
    if(character_buffer.size() == 0)
    {
        std::cout << "logon() !character_buffer size 0" << std::endl;
        return true;
    }

    // Process all incoming data stright to the input functions.
    m_mod_functions[m_mod_function_index](character_buffer);

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

    // Execute the initial setup index.
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

    value[PROMPT_LOGON]          = std::make_pair("Logon Prompt", "|CR|08Lo|07gon|08: |04");
    value[PROMPT_PASSWORD]       = std::make_pair("Password Prompt", "|CR|08pa|07ssword|08: |04");
    value[PROMPT_USE_INVALID]    = std::make_pair("Invalid Entry", "|CR|12Invalid Response! Try again.");

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
    std::string result = m_session_io.parseTextPrompt(
                             m_text_prompts_dao->getPrompt(PROMPT_LOGON)
                         );
    baseProcessAndDeliver(result);
}


/**
 * @brief Validates user logon password
 * @return
 */
void ModLogon::setupPassword()
{
    std::cout << "setupPassword()" << std::endl;
    std::string result = m_session_io.parseTextPrompt(
                             m_text_prompts_dao->getPrompt(PROMPT_PASSWORD)
                         );
    baseProcessAndDeliver(result);
}

/**
 * @brief Validates user logon password challenge questions
 * @return
 */
void ModLogon::setupPasswordQuestion()
{
    std::cout << "setupPasswordQuestion()" << std::endl;
    std::string result = m_session_io.parseTextPrompt(
                             m_text_prompts_dao->getPrompt(PROMPT_PASSWORD_QUESTION)
                         );
    baseProcessAndDeliver(result);
}

/**
 * @brief Validates user logon password challenge questions
 * @return
 */
void ModLogon::setupPasswordAnswer()
{
    std::cout << "setupPasswordAnswer()" << std::endl;
    std::string result = m_session_io.parseTextPrompt(
                             m_text_prompts_dao->getPrompt(PROMPT_PASSWORD_ANSWER)
                         );
    baseProcessAndDeliver(result);
}

/**
 * @brief Chanes user logon password
 * @return
 */
void ModLogon::setupPasswordChange()
{

}


/**
 * @brief Validates user Logon
 * @return
 */
bool ModLogon::logon(const std::string &input)
{
    std::cout << "logon: " << input << std::endl;

    // handle input for using ansi color, hot key or ENTER after..  hmm
    std::string key = "";
    std::string result = m_session_io.getInputField(input, key, Config::sName_length);

    if(result == "aborted") // ESC was hit, make this just clear the input text, or start over!
    {
        std::cout << "aborted!" << std::endl;
    }
    else if(result[0] == '\n')
    {
        // If ENTER Default to Yes, or Single Y is hit
        if(key.size() > 0)
        {
            // Validate User name here...

            // If user matches then change mod to ask for password!

            // Change to next Module
            changeModule(MOD_PASSWORD);
        }
        else
        {
            std::string result = m_session_io.parseTextPrompt(
                                     m_text_prompts_dao->getPrompt(PROMPT_USE_INVALID)
                                 );
            baseProcessAndDeliver(result);

            // Invalid, Ask again
            setupLogon();
        }
    }
    else
    {
        // Send back the single input received to show client key presses.
        // Only if return data shows a processed key returned.
        if (result != "empty") {
            baseProcessAndDeliver(result);
        }
    }

    return m_is_authorized;
}


/**
 * @brief Validates user logon password
 * @return
 */
bool ModLogon::password(const std::string &input)
{
   std::cout << "password: " << input << std::endl;

    // handle input for using ansi color, hot key or ENTER after..  hmm
    std::string key = "";
    std::string result = m_session_io.getInputField(input, key, Config::sPassword_length);
    if(result == "aborted") // ESC was hit, make this just clear the input text, or start over!
    {
        std::cout << "aborted!" << std::endl;
    }
    else if(result[0] == '\n')
    {
        // If ENTER Default to Yes, or Single Y is hit
        if(key.size() > 0)
        {            
            // Validate User password here...

            // If passowrd is valid, then marked authorized.


            // If success, set authorized true, and return.
            // or m_session_data->m_is_session_authorized = true;
            m_is_authorized = true;
            m_is_active = false;
        }
        else
        {
            std::string result = m_session_io.parseTextPrompt(
                                     m_text_prompts_dao->getPrompt(PROMPT_USE_INVALID)
                                 );
            baseProcessAndDeliver(result);

            // Invalid, Ask again
            setupLogon();
        }
    }
    else
    {
        // Send back the single input received to show client key presses.
        // Only if return data shows a processed key returned.
        if (result != "empty") {
            baseProcessAndDeliver(result);
        }
    }

    // If successful login, we'll check the return result.
    return m_is_authorized;
}

/**
 * @brief Gets Password Challenge Question Response
 * @return
 */
bool ModLogon::passwordQuestion(const std::string &input)
{
    bool result = false;
    if(input.size() != 0)
    {

    }

    return result;
}

/**
 * @brief Gets Password Challenge Answer Response
 * @return
 */
bool ModLogon::passwordAnswer(const std::string &input)
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
