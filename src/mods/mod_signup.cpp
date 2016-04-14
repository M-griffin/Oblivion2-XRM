#include "mod_signup.hpp"

#include "../model/config.hpp"

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

    value[PROMPT_NUP]                = std::make_pair("New User Password", "|08|CR|CRNew User Password: |04");
    value[PROMPT_DISCLAIMER]         = std::make_pair("Disclaimer", "|08|CRDisclaimer Text here.. |CR|CR[y/n] Disclaimer Agree : |04");
    value[PROMPT_HANDLE]             = std::make_pair("User Handle", "|08|CREnter Handle : |04");
    value[PROMPT_REAL_NAME]          = std::make_pair("Real Name", "|08|CREnter Real Name : |04");
    value[PROMPT_ADDRESS]            = std::make_pair("Address", "|08|CRAddress : |04");
    value[PROMPT_LOCATION]           = std::make_pair("Location", "|08|CRLocation : |04");
    value[PROMPT_COUNTRY]            = std::make_pair("Country", "|08|CRCountry : |04");
    value[PROMPT_EMAIL]              = std::make_pair("Email Address", "|08|CREmail Address : |04");
    value[PROMPT_USER_NOTE]          = std::make_pair("User Note", "|08|CRAffiliations / User Note : |04");
    value[PROMPT_BIRTH_DATE]         = std::make_pair("Birth Date", "|08|CR[yyyy-mm-dd] Birth Date : |04");
    value[PROMPT_GENDER]             = std::make_pair("Gender", "|CR|08[m/f] Gender : |04");
    value[PROMPT_PASSWORD]           = std::make_pair("Password", "|CR|08(case sensitive) Password : |04");
    value[PROMPT_VERIFY_PASSWORD]    = std::make_pair("Verify Password", "|CR|08(case sensitive) Verify Password : |04");
    value[PROMPT_CHALLENGE_QUESTION] = std::make_pair("Forgot Password Challenge Question", "|CR|08Challenge Question : |04");
    value[PROMPT_CHALLENGE_ANSWER]   = std::make_pair("Forgot Password Challenge Answer", "|CR|08Challenge Answer : |04");
    value[PROMPT_VERIFY_ANSWER]      = std::make_pair("Forgot Password Verify Answer", "|CR|08Verify Answer : |04");
    value[PROMPT_YESNO_BARS]         = std::make_pair("Use YES/NO Bars", "|CR|08[y/n] Use Yes/No Bars : |04");
    value[PROMPT_USE_PAUSE]          = std::make_pair("Pause on ", "|CR|08Screen Pausing : |04");
    value[PROMPT_USE_CLEAR]          = std::make_pair("Clear Screen or Scroll ", "|CR|08Clear Screen or Scroll : |04");
    value[PROMPT_USE_ANSI_COLOR]     = std::make_pair("Use Ansi Color ", "|CR|08[y/n] Ansi Color : |04");
    value[PROMPT_BACK_SPACE]         = std::make_pair("Backspace Type", "|CR|08Backspace Key |CR[(W)indows/(T)erminal/(D)etect [ENTER] = Detect] : |04");

    // Invalid.
    value[PROMPT_TEXT_INVALID]     = std::make_pair("Invalid Text", "|CR|08[y/n] Ansi Color : |04");
    value[PROMPT_DATE_INVALID]     = std::make_pair("Invalid Date", "|CR|08[y/n] Ansi Color : |04");
    value[PROMPT_PASS_INVALID]     = std::make_pair("Invalid/Non Matching Password", "|CR|08[y/n] Ansi Color : |04");

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
    // Check if New User Password is enabled
    if(m_config->use_newuser_password)
    {
        std::string result = m_session_io.parseTextPrompt(
                                 m_text_prompts_dao->getPrompt(PROMPT_NUP)
                             );
        m_session_data->deliver(result);
    }
    else
    {
        // Move to next module.
        changeModule(m_mod_function_index+1);
    }
}

/**
 * @brief Pre Application Disclaimer
 * @return
 */
void ModSignup::setupDisclaimer()
{
    if(m_config->use_disclaimer)
    {
        std::string result = m_session_io.parseTextPrompt(
                                 m_text_prompts_dao->getPrompt(PROMPT_DISCLAIMER)
                             );
        m_session_data->deliver(result);
    }
    else
    {
        // Move to next module.
        changeModule(m_mod_function_index+1);
    }
}

/**
 * @brief Get Handle from User
 * @return
 */
void ModSignup::setupHandle()
{
    std::string result = m_session_io.parseTextPrompt(
                             m_text_prompts_dao->getPrompt(PROMPT_HANDLE)
                         );
    m_session_data->deliver(result);
}

/**
 * @brief Get Real Name
 * @return
 */
void ModSignup::setupRealName()
{
    std::string result = m_session_io.parseTextPrompt(
                             m_text_prompts_dao->getPrompt(PROMPT_REAL_NAME)
                         );
    m_session_data->deliver(result);
}

/**
 * @brief Get Address
 * @return
 */
void ModSignup::setupAddress()
{
    std::string result = m_session_io.parseTextPrompt(
                             m_text_prompts_dao->getPrompt(PROMPT_ADDRESS)
                         );
    m_session_data->deliver(result);
}

/**
 * @brief Get Location
 * @return
 */
void ModSignup::setupLocation()
{
    std::string result = m_session_io.parseTextPrompt(
                             m_text_prompts_dao->getPrompt(PROMPT_LOCATION)
                         );
    m_session_data->deliver(result);
}

/**
 * @brief Get Country
 * @return
 */
void ModSignup::setupCountry()
{
    std::string result = m_session_io.parseTextPrompt(
                             m_text_prompts_dao->getPrompt(PROMPT_COUNTRY)
                         );
    m_session_data->deliver(result);
}

/**
 * @brief Get Email
 * @return
 */
void ModSignup::setupEmail()
{
    std::string result = m_session_io.parseTextPrompt(
                             m_text_prompts_dao->getPrompt(PROMPT_EMAIL)
                         );
    m_session_data->deliver(result);
}

/**
 * @brief Get UserNote
 * @return
 */
void ModSignup::setupUserNote()
{
    std::string result = m_session_io.parseTextPrompt(
                             m_text_prompts_dao->getPrompt(PROMPT_USER_NOTE)
                         );
    m_session_data->deliver(result);
}

/**
 * @brief Get Birthdate
 * @return
 */
void ModSignup::setupBirthday()
{
    std::string result = m_session_io.parseTextPrompt(
                             m_text_prompts_dao->getPrompt(PROMPT_BIRTH_DATE)
                         );
    m_session_data->deliver(result);
}

/**
 * @brief Get Gender
 * @return
 */
void ModSignup::setupGender()
{
    std::string result = m_session_io.parseTextPrompt(
                             m_text_prompts_dao->getPrompt(PROMPT_GENDER)
                         );
    m_session_data->deliver(result);
}

/**
 * @brief Get Password
 * @return
 */
void ModSignup::setupPassword()
{
    std::string result = m_session_io.parseTextPrompt(
                             m_text_prompts_dao->getPrompt(PROMPT_PASSWORD)
                         );
    m_session_data->deliver(result);
}

/**
 * @brief Verify Password
 * @return
 */
void ModSignup::setupVerifyPassword()
{
    std::string result = m_session_io.parseTextPrompt(
                             m_text_prompts_dao->getPrompt(PROMPT_VERIFY_PASSWORD)
                         );
    m_session_data->deliver(result);
}

/**
 * @brief Set Password Reset Challenge Question
 * @return
 */
void ModSignup::setupChallengeQuestion()
{

    std::string result = m_session_io.parseTextPrompt(
                             m_text_prompts_dao->getPrompt(PROMPT_CHALLENGE_QUESTION)
                         );
    m_session_data->deliver(result);
}

/**
 * @brief Set Password Reset Challenge Answer
 * @return
 */
void ModSignup::setupChallengeAnswer()
{
    std::string result = m_session_io.parseTextPrompt(
                             m_text_prompts_dao->getPrompt(PROMPT_CHALLENGE_ANSWER)
                         );
    m_session_data->deliver(result);
}

/**
 * @brief Set Password Reset Challenge Answer
 * @return
 */
void ModSignup::setupVerifyChallengeAnswer()
{
    std::string result = m_session_io.parseTextPrompt(
                             m_text_prompts_dao->getPrompt(PROMPT_VERIFY_ANSWER)
                         );
    m_session_data->deliver(result);
}

/**
 * @brief Get Yes / No Bar Preference
 * @return
 */
void ModSignup::setupYesNoBars()
{
    std::string result = m_session_io.parseTextPrompt(
                             m_text_prompts_dao->getPrompt(PROMPT_YESNO_BARS)
                         );
    m_session_data->deliver(result);
}

/**
 * @brief Get Pause Preference
 * @return
 */
void ModSignup::setupDoPause()
{
    std::string result = m_session_io.parseTextPrompt(
                             m_text_prompts_dao->getPrompt(PROMPT_USE_PAUSE)
                         );
    m_session_data->deliver(result);
}

/**
 * @brief Get Clear or Scroll preference
 * @return
 */
void ModSignup::setupClearOrScroll()
{

    std::string result = m_session_io.parseTextPrompt(
                             m_text_prompts_dao->getPrompt(PROMPT_USE_CLEAR)
                         );
    m_session_data->deliver(result);
}

/**
 * @brief Get ANSI Color preference
 * @return
 */
void ModSignup::setupAnsiColor()
{
    std::string result = m_session_io.parseTextPrompt(
                             m_text_prompts_dao->getPrompt(PROMPT_USE_ANSI_COLOR)
                         );
    m_session_data->deliver(result);
}

/**
 * @brief Get Backspace Preference WINDOWS/LINUX (Terminal)
 * @return
 */
void ModSignup::setupBackSpace()
{
    std::string result = m_session_io.parseTextPrompt(
                             m_text_prompts_dao->getPrompt(PROMPT_BACK_SPACE)
                         );
    m_session_data->deliver(result);
}


/**
 * @brief Check for New User Password
 * @return
 */
bool ModSignup::newUserPassword(const std::string &input)
{
    std::cout << "newUserPassword: " << input << std::endl;

    // handle input for using ansi color, hot key or ENTER after..  hmm
    std::string key = "";
    std::string result = m_session_io.getInputField(input, key);
    if(result == "aborted") // ESC was hit, make this just clear the input text, or start over!
    {
        std::cout << "aborted!" << std::endl;
        // exit, and return
        m_is_active = false;
        return false;
    }
    else if(result[0] == '\n')
    {
        // Key == 0 on [ENTER] pressed alone. then invalid!
        if(key.size() == 0)
        {
            // Return and don't do anything.
            return false;
        }

        // Pull in and test aginst new user password.
        if(key.compare("TeSt") == 0)
        {
            std::cout << "Match" << key.size() << std::endl;
        }
        else
        {
            std::cout << "No Match" << key.size() << std::endl;

            std::string prompt_text = m_session_io.parseTextPrompt(
                                          m_text_prompts_dao->getPrompt(PROMPT_TEXT_INVALID)
                                      );
            m_session_data->deliver(prompt_text);

            // Invalid, Ask again
            setupNewUserPassword();
        }
    }
    else
    {
        // Send back the single input received to show client key presses.
        m_session_data->deliver(input);
    }

    return true;
}

/**
 * @brief Pre Application Disclaimer
 * @return
 */
bool ModSignup::disclaimer(const std::string &) //input)
{
    bool result = false;
    return result;
}

/**
 * @brief Get Handle from User
 * @return
 */
bool ModSignup::handle(const std::string &) //input)
{
    bool result = false;
    return result;
}

/**
 * @brief Get Real Name
 * @return
 */
bool ModSignup::realName(const std::string &) //input)
{
    bool result = false;
    return result;
}

/**
 * @brief Get Address
 * @return
 */
bool ModSignup::address(const std::string &) //input)
{
    bool result = false;
    return result;
}

/**
 * @brief Get Location
 * @return
 */
bool ModSignup::location(const std::string &) //input)
{
    bool result = false;
    return result;
}

/**
 * @brief Get Country
 * @return
 */
bool ModSignup::country(const std::string &) //input)
{
    bool result = false;
    return result;
}

/**
 * @brief Get Email
 * @return
 */
bool ModSignup::email(const std::string &) //input)
{
    bool result = false;
    return result;
}

/**
 * @brief Get UserNote
 * @return
 */
bool ModSignup::userNote(const std::string &) //input)
{
    bool result = false;
    return result;
}

/**
 * @brief Get Birthdate
 * @return
 */
bool ModSignup::birthday(const std::string &) //input)
{
    bool result = false;
    return result;
}

/**
 * @brief Get Gender
 * @return
 */
bool ModSignup::gender(const std::string &) //input)
{
    bool result = false;
    return result;
}

/**
 * @brief Get Password
 * @return
 */
bool ModSignup::password(const std::string &) //input)
{
    bool result = false;
    return result;
}

/**
 * @brief Verify Password
 * @return
 */
bool ModSignup::verifyPassword(const std::string &) //input)
{
    bool result = false;
    return result;
}

/**
 * @brief Set Password Reset Challenge Question
 * @return
 */
bool ModSignup::challengeQuestion(const std::string &) //input)
{
    bool result = false;
    return result;
}

/**
 * @brief Set Password Reset Challenge Answer
 * @return
 */
bool ModSignup::challengeAnswer(const std::string &) //input)
{
    bool result = false;
    return result;
}

/**
 * @brief Set Password Reset Challenge Answer
 * @return
 */
bool ModSignup::verifyChallengeAnswer(const std::string &) //input)
{
    bool result = false;
    return result;
}

/**
 * @brief Get Yes / No Bar Preference
 * @return
 */
bool ModSignup::yesNoBars(const std::string &) //input)
{
    bool result = false;
    return result;
}

/**
 * @brief Get Pause Preference
 * @return
 */
bool ModSignup::doPause(const std::string &) //input)
{
    bool result = false;
    return result;
}

/**
 * @brief Get Clear or Scroll preference
 * @return
 */
bool ModSignup::clearOrScroll(const std::string &) //input)
{
    bool result = false;
    return result;
}

/**
 * @brief Get ANSI Color preference
 * @return
 */
bool ModSignup::ansiColor(const std::string &) //input)
{
    bool result = false;
    return result;
}

/**
 * @brief Get Backspace Preference WINDOWS/LINUX (Terminal)
 * @return
 */
bool ModSignup::backSpace(const std::string &) //input)
{
    bool result = false;
    return result;
}
