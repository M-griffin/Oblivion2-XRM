#include "mod_signup.hpp"

#include <iostream>
#include <string>

/**
 * @brief Handles Updates or Data Input from Client
 * @return bool, not used anymore?!?
 */
bool ModSignup::update(const std::string &character_buffer, const bool &)
{
    // Make sure system is active, when system is done, success or failes
    // We change this is inactive to single the login process is completed.
    if(!m_is_active)
    {
        std::cout << "ModSignup() !m_is_active" << std::endl;
        return false;
    }

    // Return True when were keeping module active / else false;
    if(character_buffer.size() == 0)
    {
        std::cout << "ModSignup() !character_buffer size 0" << std::endl;
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
bool ModSignup::onEnter()
{
    std::cout << "OnEnter() ModSignup\n";
    m_is_active = true;

    // Check if NUP is active,  if not, move to disclaimer!


    // Execute the initial setup index.
    m_setup_functions[m_mod_function_index]();

    return true;
}

/**
 * @brief Exit, close down, display screens to change over data.
 * @return
 */
bool ModSignup::onExit()
{
    std::cout << "OnExit() ModSignup\n";
    m_is_active = false;
    return true;
}

/**
 * @brief Create Default Text Prompts for module
 */
void ModSignup::createTextPrompts()
{
    // Create Mapping to pass for file creation (default values)
    M_TextPrompt value;

    value[PROMPT_HANDLE]          = std::make_pair("User Handle", "|08Handle: |04");

    m_text_prompts_dao->writeValue(value);
}


/**
 * @brief Sets an indivdual module index.
 * @param mod_function_index
 */
void ModSignup::changeModule(int mod_function_index)
{
    // Set, and Execute the Setup module.
    m_mod_function_index = mod_function_index;
    m_setup_functions[m_mod_function_index]();
}


/**
 * @brief Check for New User Password
 * @return
 */
void ModSignup::setupNewUserPassword()
{

}

/**
 * @brief Pre Application Disclaimer
 * @return
 */
void ModSignup::setupDisclaimer()
{

}

/**
 * @brief Get Handle from User
 * @return
 */
void ModSignup::setupHandle()
{

}

/**
 * @brief Get Real Name
 * @return
 */
void ModSignup::setupRealName()
{

}

/**
 * @brief Get Address
 * @return
 */
void ModSignup::setupAddress()
{

}

/**
 * @brief Get Location
 * @return
 */
void ModSignup::setupLocation()
{

}

/**
 * @brief Get Country
 * @return
 */
void ModSignup::setupCountry()
{

}

/**
 * @brief Get Email
 * @return
 */
void ModSignup::setupEmail()
{

}

/**
 * @brief Get UserNote
 * @return
 */
void ModSignup::setupUserNote()
{

}

/**
 * @brief Get Birthdate
 * @return
 */
void ModSignup::setupBirthday()
{

}

/**
 * @brief Get Gender
 * @return
 */
void ModSignup::setupGender()
{

}

/**
 * @brief Get Password
 * @return
 */
void ModSignup::setupPassword()
{

}

/**
 * @brief Verify Password
 * @return
 */
void ModSignup::setupVerifyPassword()
{

}

/**
 * @brief Set Password Reset Challenge Question
 * @return
 */
void ModSignup::setupChallengeQuestion()
{

}

/**
 * @brief Set Password Reset Challenge Answer
 * @return
 */
void ModSignup::setupChallengeAnswer()
{

}

/**
 * @brief Set Password Reset Challenge Answer
 * @return
 */
void ModSignup::setupVerifyChallengeAnswer()
{

}

/**
 * @brief Get Yes / No Bar Preference
 * @return
 */
void ModSignup::setupYesNoBars()
{

}

/**
 * @brief Get Pause Preference
 * @return
 */
void ModSignup::setupDoPause()
{

}

/**
 * @brief Get Clear or Scroll preference
 * @return
 */
void ModSignup::setupClearOrScroll()
{

}

/**
 * @brief Get ANSI Color preference
 * @return
 */
void ModSignup::setupAnsiColor()
{

}

/**
 * @brief Get Backspace Preference WINDOWS/LINUX (Terminal)
 * @return
 */
void ModSignup::setupBackSpace()
{

}


/**
 * @brief Check for New User Password
 * @return
 */
bool ModSignup::newUserPassword(const std::string &input)
{
    bool result = false;
    return result;
}

/**
 * @brief Pre Application Disclaimer
 * @return
 */
bool ModSignup::disclaimer(const std::string &input)
{
    bool result = false;
    return result;
}

/**
 * @brief Get Handle from User
 * @return
 */
bool ModSignup::handle(const std::string &input)
{
    bool result = false;
    return result;
}

/**
 * @brief Get Real Name
 * @return
 */
bool ModSignup::realName(const std::string &input)
{
    bool result = false;
    return result;
}

/**
 * @brief Get Address
 * @return
 */
bool ModSignup::address(const std::string &input)
{
    bool result = false;
    return result;
}

/**
 * @brief Get Location
 * @return
 */
bool ModSignup::location(const std::string &input)
{
    bool result = false;
    return result;
}

/**
 * @brief Get Country
 * @return
 */
bool ModSignup::country(const std::string &input)
{
    bool result = false;
    return result;
}

/**
 * @brief Get Email
 * @return
 */
bool ModSignup::email(const std::string &input)
{
    bool result = false;
    return result;
}

/**
 * @brief Get UserNote
 * @return
 */
bool ModSignup::userNote(const std::string &input)
{
    bool result = false;
    return result;
}

/**
 * @brief Get Birthdate
 * @return
 */
bool ModSignup::birthday(const std::string &input)
{
    bool result = false;
    return result;
}

/**
 * @brief Get Gender
 * @return
 */
bool ModSignup::gender(const std::string &input)
{
    bool result = false;
    return result;
}

/**
 * @brief Get Password
 * @return
 */
bool ModSignup::password(const std::string &input)
{
    bool result = false;
    return result;
}

/**
 * @brief Verify Password
 * @return
 */
bool ModSignup::verifyPassword(const std::string &input)
{
    bool result = false;
    return result;
}

/**
 * @brief Set Password Reset Challenge Question
 * @return
 */
bool ModSignup::challengeQuestion(const std::string &input)
{
    bool result = false;
    return result;
}

/**
 * @brief Set Password Reset Challenge Answer
 * @return
 */
bool ModSignup::challengeAnswer(const std::string &input)
{
    bool result = false;
    return result;
}

/**
 * @brief Set Password Reset Challenge Answer
 * @return
 */
bool ModSignup::verifyChallengeAnswer(const std::string &input)
{
    bool result = false;
    return result;
}

/**
 * @brief Get Yes / No Bar Preference
 * @return
 */
bool ModSignup::yesNoBars(const std::string &input)
{
    bool result = false;
    return result;
}

/**
 * @brief Get Pause Preference
 * @return
 */
bool ModSignup::doPause(const std::string &input)
{
    bool result = false;
    return result;
}

/**
 * @brief Get Clear or Scroll preference
 * @return
 */
bool ModSignup::clearOrScroll(const std::string &input)
{
    bool result = false;
    return result;
}

/**
 * @brief Get ANSI Color preference
 * @return
 */
bool ModSignup::ansiColor(const std::string &input)
{
    bool result = false;
    return result;
}

/**
 * @brief Get Backspace Preference WINDOWS/LINUX (Terminal)
 * @return
 */
bool ModSignup::backSpace(const std::string &input)
{
    bool result = false;
    return result;
}
