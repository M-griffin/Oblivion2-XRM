#include "mod_signup.hpp"

#include "../model/config.hpp"
#include "../model/users.hpp"
#include "../encryption.hpp"

#include <boost/regex.hpp>

#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <ctime>



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
    value[PROMPT_LOCATION]           = std::make_pair("Location", "|08|CRCity/State : |04");
    value[PROMPT_COUNTRY]            = std::make_pair("Country", "|08|CRCountry : |04");
    value[PROMPT_EMAIL]              = std::make_pair("Email Address", "|08|CREmail Address : |04");
    value[PROMPT_USER_NOTE]          = std::make_pair("User Note", "|08|CRAffiliations / User Note : |04");
    value[PROMPT_BIRTH_DATE]         = std::make_pair("Birth Date", "|08|CR[yyyy-mm-dd] Birth Date : |04");
    value[PROMPT_GENDER]             = std::make_pair("Gender", "|CR|08[m/f] Gender : |04");
    value[PROMPT_PASSWORD]           = std::make_pair("Password", "|CR|08(case sensitive) Password : |04");
    value[PROMPT_VERIFY_PASSWORD]    = std::make_pair("Verify Password", "|CR|08(case sensitive) Verify Password : |04");
    value[PROMPT_CHALLENGE_QUESTION] = std::make_pair("Forgot Password Challenge Question", "|CR|08Challenge Question : |04");
    value[PROMPT_CHALLENGE_ANSWER]   = std::make_pair("Forgot Password Challenge Answer", "|CR|08(case sensitive) Challenge Answer : |04");
    value[PROMPT_VERIFY_ANSWER]      = std::make_pair("Forgot Password Verify Answer", "|CR|08(case sensitive) Verify Answer : |04");
    value[PROMPT_YESNO_BARS]         = std::make_pair("Use YES/NO Bars", "|CR|08[y/n] Use Yes/No Bars : |04");
    value[PROMPT_USE_PAUSE]          = std::make_pair("Pause on ", "|CR|08Screen Pausing : |04");
    value[PROMPT_USE_CLEAR]          = std::make_pair("Clear Screen or Scroll ", "|CR|08Clear Screen or Scroll : |04");
    value[PROMPT_USE_ANSI_COLOR]     = std::make_pair("Use Ansi Color ", "|CR|08[y/n] Ansi Color : |04");
    value[PROMPT_BACK_SPACE]         = std::make_pair("Backspace Type", "|CR|08Backspace Key |CR[(W)indows/(T)erminal/(D)etect [ENTER] = Detect] : |04");
    // Invalid.
    value[PROMPT_TEXT_INVALID]       = std::make_pair("Invalid Text", "|CR|04Invalid entry!");
    value[PROMPT_DATE_INVALID]       = std::make_pair("Invalid Date", "|CR|04Invalid date entered!");
    value[PROMPT_PASS_INVALID]       = std::make_pair("Invalid/Non Matching Password", "|CR|04Invalid, password does not match!");
    value[PROMPT_HANDLE_INVALID]     = std::make_pair("User Name Already Exists", "|CR|04Invalid UserName, Already Exists!");
    value[PROMPT_NAME_INVALID]       = std::make_pair("Real Name Already Exists", "|CR|04Name, Already Exists, Try Adding a middle initial.");

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
        baseProcessAndDeliver(result);
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
        baseProcessAndDeliver(result);
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
    baseProcessAndDeliver(result);
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
    baseProcessAndDeliver(result);
}

/**
 * @brief Get Address
 * @return
 */
void ModSignup::setupAddress()
{
    if(m_config->use_address)
    {
        std::string result = m_session_io.parseTextPrompt(
                                 m_text_prompts_dao->getPrompt(PROMPT_ADDRESS)
                             );
        baseProcessAndDeliver(result);
    }
    else
    {
        // Move to next module.
        changeModule(m_mod_function_index+1);
    }
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
    baseProcessAndDeliver(result);
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
    baseProcessAndDeliver(result);
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
    baseProcessAndDeliver(result);
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
    baseProcessAndDeliver(result);
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
    baseProcessAndDeliver(result);
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
    baseProcessAndDeliver(result);
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
    baseProcessAndDeliver(result);
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
    baseProcessAndDeliver(result);
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
    baseProcessAndDeliver(result);
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
    baseProcessAndDeliver(result);
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
    baseProcessAndDeliver(result);
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
    baseProcessAndDeliver(result);
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
    baseProcessAndDeliver(result);
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
    baseProcessAndDeliver(result);
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
    baseProcessAndDeliver(result);
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
    baseProcessAndDeliver(result);
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
    std::string result = m_session_io.getInputField(input, key, Config::sPassword_length);
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
        if(key.compare(m_config->password_newuser) == 0)
        {
            std::cout << "Match" << key.size() << std::endl;

            // Move to next module.
            changeModule(m_mod_function_index+1);
        }
        else
        {
            std::cout << "No Match" << key.size() << std::endl;

            std::string prompt_text = m_session_io.parseTextPrompt(
                                          m_text_prompts_dao->getPrompt(PROMPT_PASS_INVALID)
                                      );
            baseProcessAndDeliver(prompt_text);

            // Invalid, Ask again, Reload Current Module
            changeModule(m_mod_function_index);
        }
    }
    else
    {
        // Send back the single input received to show client key presses.
        // Only if return data shows a processed key returned.
        if(result != "empty")
        {
            baseProcessAndDeliver(result);
        }
    }

    return true;
}

/**
 * @brief Pre Application Disclaimer
 * @return
 */
bool ModSignup::disclaimer(const std::string &input)
{
    std::cout << "disclaimer: " << input << std::endl;

    // handle input for using ansi color, hot key or ENTER after..  hmm
    std::string key = "";
    std::string result = m_session_io.getInputField(input, key, Config::sSingle_key_length);
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

        // If ENTER Default to Yes, or Single Y is hit
        if(toupper(key[0]) == 'Y' && key.size() == 1)
        {
            // Move to next module.
            changeModule(m_mod_function_index+1);
        }
        // Else check for single N for No to default to ASCII no colors.
        else if(toupper(key[0]) == 'N' && key.size() == 1)
        {
            // Disconnect or exist back to matrix!
            m_is_active = false;
        }
        else
        {
            // Display Invalid Input.
            std::string prompt_text = m_session_io.parseTextPrompt(
                                          m_text_prompts_dao->getPrompt(PROMPT_TEXT_INVALID)
                                      );

            baseProcessAndDeliver(prompt_text);

            // Invalid, Ask again, Reload Current Module
            changeModule(m_mod_function_index);
        }
    }
    else
    {
        // Send back the single input received to show client key presses.
        // Only if return data shows a processed key returned.
        if(result != "empty")
        {
            baseProcessAndDeliver(result);
        }
    }

    return true;
}


/**
 * @brief Get Handle from User
 * @return
 */
bool ModSignup::handle(const std::string &input)
{
    std::cout << "handle: " << input << std::endl;

    // handle input for using ansi color, hot key or ENTER after..  hmm
    std::string key = "";
    std::string result = m_session_io.getInputField(input, key, Config::sName_length);
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

        // Check for user name and if is already exists!
        users_dao_ptr user_data(new UsersDao(m_session_data->m_user_database));
        user_ptr search = user_data->getUserByHandle(key);

        if(search->iId == -1)
        {
            std::cout << "no match found" << std::endl;

            // Set the User Name
            m_user_record->sHandle = key;

            // Move to next module.
            changeModule(m_mod_function_index+1);
        }
        else
        {
            std::cout << "match found" << std::endl;

            // Invalid Entry, try again!
            std::string message = m_session_io.parseTextPrompt(
                                      m_text_prompts_dao->getPrompt(PROMPT_HANDLE_INVALID)
                                  );

            baseProcessAndDeliver(message);

            // Invalid, Ask again, Reload Current Module
            changeModule(m_mod_function_index);
        }
    }
    else
    {
        // Send back the single input received to show client key presses.
        // Only if return data shows a processed key returned.
        if(result != "empty")
        {
            baseProcessAndDeliver(result);
        }
    }

    return true;
}

/**
 * @brief Get Real Name
 * @return
 */
bool ModSignup::realName(const std::string &input)
{
    std::cout << "realName: " << input << std::endl;

    // realName input for using ansi color, hot key or ENTER after..  hmm
    std::string key = "";
    std::string result = m_session_io.getInputField(input, key, Config::sName_length);
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


        // Check for real name and if is already exists!
        users_dao_ptr user_data(new UsersDao(m_session_data->m_user_database));
        user_ptr search = user_data->getUserByRealName(key);

        if(search->iId == -1)
        {
            std::cout << "no match found" << std::endl;

            // Set the User Name
            m_user_record->sRealName = key;

            // Move to next module.
            changeModule(m_mod_function_index+1);
        }
        else
        {
            std::cout << "match found" << std::endl;

            // Invalid Entry, try again!
            std::string message = m_session_io.parseTextPrompt(
                                      m_text_prompts_dao->getPrompt(PROMPT_NAME_INVALID)
                                  );

            baseProcessAndDeliver(message);

            // Invalid, Ask again, Reload Current Module
            changeModule(m_mod_function_index);
        }
    }
    else
    {
        // Send back the single input received to show client key presses.
        // Only if return data shows a processed key returned.
        if(result != "empty")
        {
            baseProcessAndDeliver(result);
        }
    }

    return true;
}

/**
 * @brief Get Address
 * @return
 */
bool ModSignup::address(const std::string &input)
{
    std::cout << "address: " << input << std::endl;

    // address input, hot key or ENTER after..  hmm
    std::string key = "";
    std::string result = m_session_io.getInputField(input, key, Config::sDefault_question_length);
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

        // Set the User Name
        m_user_record->sAddress = key;

        // Move to next module.
        changeModule(m_mod_function_index+1);
    }
    else
    {
        // Send back the single input received to show client key presses.
        // Only if return data shows a processed key returned.
        if(result != "empty")
        {
            baseProcessAndDeliver(result);
        }
    }

    return true;
}

/**
 * @brief Get Location
 * @return
 */
bool ModSignup::location(const std::string &input)
{
    std::cout << "location: " << input << std::endl;

    // location input, hot key or ENTER after..  hmm
    std::string key = "";
    std::string result = m_session_io.getInputField(input, key, Config::sDefault_question_length);
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

        // Set the User Name
        m_user_record->sLocation = key;

        // Move to next module.
        changeModule(m_mod_function_index+1);
    }
    else
    {
        // Send back the single input received to show client key presses.
        // Only if return data shows a processed key returned.
        if(result != "empty")
        {
            baseProcessAndDeliver(result);
        }
    }

    return true;
}

/**
 * @brief Get Country
 * @return
 */
bool ModSignup::country(const std::string &input)
{
    std::cout << "country: " << input << std::endl;

    // country input, hot key or ENTER after..  hmm
    std::string key = "";
    std::string result = m_session_io.getInputField(input, key, Config::sDefault_question_length);
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

        // Set the User Name
        m_user_record->sCountry = key;

        // Move to next module.
        changeModule(m_mod_function_index+1);
    }
    else
    {
        // Send back the single input received to show client key presses.
        // Only if return data shows a processed key returned.
        if(result != "empty")
        {
            baseProcessAndDeliver(result);
        }
    }

    return true;
}

/**
 * @brief Get Email
 * @return
 */
bool ModSignup::email(const std::string &input)
{
    std::cout << "email: " << input << std::endl;

    // email input, hot key or ENTER after..  hmm
    std::string key = "";
    std::string result = m_session_io.getInputField(input, key, Config::sDefault_question_length);
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

        // Set the User Name
        m_user_record->sEmail = key;

        // Move to next module.
        changeModule(m_mod_function_index+1);
    }
    else
    {
        // Send back the single input received to show client key presses.
        // Only if return data shows a processed key returned.
        if(result != "empty")
        {
            baseProcessAndDeliver(result);
        }
    }

    return true;
}

/**
 * @brief Get UserNote
 * @return
 */
bool ModSignup::userNote(const std::string &input)
{
    std::cout << "userNote: " << input << std::endl;

    // userNote input, hot key or ENTER after..  hmm
    std::string key = "";
    std::string result = m_session_io.getInputField(input, key, Config::sDefault_question_length);
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

        // Set the User Name
        m_user_record->sUserNote = key;

        // Move to next module.
        changeModule(m_mod_function_index+1);
    }
    else
    {
        // Send back the single input received to show client key presses.
        // Only if return data shows a processed key returned.
        if(result != "empty")
        {
            baseProcessAndDeliver(result);
        }
    }

    return true;
}

/**
 * @brief Get Birthdate
 * @return
 */
bool ModSignup::birthday(const std::string &input)
{
    std::cout << "birthday: " << input << std::endl;

    // birthday input, hot key or ENTER after..  hmm
    std::string key = "";
    std::string result = m_session_io.getInputField(input, key, Config::sDate_length);
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

        // Validate Date Here,  ie.. 2016-01-01 format.
        boost::regex date_regex { m_config->regexp_date_validation };
        boost::smatch str_matches;

        if(boost::regex_match(key, str_matches, date_regex))
        {
            struct std::tm tm;
            std::istringstream ss(key);

            ss >> std::get_time(&tm, "%Y-%m-%d");

            // Make sure date format was parsed properly.
            if(ss.fail())
            {
                std::cout << "regex passed, ss failed!" << std::endl;
                ss.clear();

                // Invalid Entry, try again!
                std::string message = m_session_io.parseTextPrompt(
                                          m_text_prompts_dao->getPrompt(PROMPT_DATE_INVALID)
                                      );

                baseProcessAndDeliver(message);

                // Move to next module.
                changeModule(m_mod_function_index);
            }

            std::time_t time = mktime(&tm);

            // Set the User Birth Date
            m_user_record->dtBirthday = time;

            // Move to next module.
            changeModule(m_mod_function_index+1);
        }
        else
        {
            // Invalid Entry, try again!
            std::string message = m_session_io.parseTextPrompt(
                                      m_text_prompts_dao->getPrompt(PROMPT_DATE_INVALID)
                                  );

            baseProcessAndDeliver(message);

            // Move to next module.
            changeModule(m_mod_function_index);
        }
    }
    else
    {
        // Send back the single input received to show client key presses.
        // Only if return data shows a processed key returned.
        if(result != "empty")
        {
            baseProcessAndDeliver(result);
        }
    }

    return true;
}

/**
 * @brief Get Gender
 * @return
 */
bool ModSignup::gender(const std::string &input)
{
    std::cout << "gender: " << input << std::endl;

    // gender input, hot key or ENTER after..  hmm
    std::string key = "";
    std::string result = m_session_io.getInputField(input, key, Config::sSingle_key_length);
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

        // Set the User Name
        m_user_record->sGender = key;

        // Move to next module.
        changeModule(m_mod_function_index+1);
    }
    else
    {
        // Send back the single input received to show client key presses.
        // Only if return data shows a processed key returned.
        if(result != "empty")
        {
            baseProcessAndDeliver(result);
        }
    }

    return true;
}

/**
 * @brief Get Password
 * @return
 */
bool ModSignup::password(const std::string &input)
{
    std::cout << "password: " << input << std::endl;

    // password input, hot key or ENTER after..  hmm
    std::string key = "";
    bool hiddenOutput = true;
    std::string result = m_session_io.getInputField(
                             input,
                             key,
                             Config::sPassword_length,
                             "",
                             hiddenOutput
                         );

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

        // Set the Password and verify it matches on next module.
        m_security_record->sPasswordHash = key;

        // Move to next module.
        changeModule(m_mod_function_index+1);
    }
    else
    {
        // Send back the single input received to show client key presses.
        // Only if return data shows a processed key returned.
        if(result != "empty")
        {
            baseProcessAndDeliver(result);
        }
    }

    return true;
}

/**
 * @brief Verify Password
 * @return
 */
bool ModSignup::verifyPassword(const std::string &input)
{
    std::cout << "password: " << input << std::endl;

    // password input, hot key or ENTER after..  hmm
    std::string key = "";
    bool hiddenOutput = true;
    std::string result = m_session_io.getInputField(
                             input,
                             key,
                             Config::sPassword_length,
                             "",
                             hiddenOutput
                         );

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

        // compare password to previous, then encrypt if they match
        // otherwise fail back if they don't and ask again.
        if(m_security_record->sPasswordHash.compare(key) == 0)
        {
            std::string salt = m_encryption->generate_salt(m_user_record->sHandle, m_config->bbs_uuid);
            std::string password = m_encryption->generate_password(m_security_record->sPasswordHash, salt);

            if(salt.size() == 0 || password.size() == 0)
            {
                std::cout << "Error, Salt or Password were empty" << std::endl;
                assert(false);
            }

            // setup the salt and password.
            m_security_record->sSaltHash = salt;
            m_security_record->sPasswordHash = password;

            // Move to next module.
            changeModule(m_mod_function_index+1);
        }
        else
        {

            std::cout << "no match found" << std::endl;

            // Set the password back to blank
            m_security_record->sPasswordHash = "";

            // Invalid Entry, try again!
            std::string message = m_session_io.parseTextPrompt(
                                      m_text_prompts_dao->getPrompt(PROMPT_PASS_INVALID)
                                  );

            baseProcessAndDeliver(message);

            // Move to next module.
            changeModule(m_mod_function_index-1);
        }
    }
    else
    {
        // Send back the single input received to show client key presses.
        // Only if return data shows a processed key returned.
        if(result != "empty")
        {
            baseProcessAndDeliver(result);
        }
    }

    return true;
}

/**
 * @brief Set Password Reset Challenge Question
 * @return
 */
bool ModSignup::challengeQuestion(const std::string &input)
{
    std::cout << "challengeQuestion: " << input << std::endl;

    // challengeQuestion input, hot key or ENTER after..  hmm
    std::string key = "";
    std::string result = m_session_io.getInputField(input, key, Config::sPassword_length);
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

        // Set the Password and verify it matches on next module.
        m_security_record->sChallengeQuestion = key;

        // Move to next module.
        changeModule(m_mod_function_index+1);
    }
    else
    {
        // Send back the single input received to show client key presses.
        // Only if return data shows a processed key returned.
        if(result != "empty")
        {
            baseProcessAndDeliver(result);
        }
    }

    return true;
}

/**
 * @brief Set Password Reset Challenge Answer
 * @return
 */
bool ModSignup::challengeAnswer(const std::string &input)
{
    std::cout << "challengeAnswer: " << input << std::endl;

    // challengeAnswer input, hot key or ENTER after..  hmm
    std::string key = "";
    bool hiddenOutput = true;
    std::string result = m_session_io.getInputField(
                             input,
                             key,
                             Config::sPassword_length,
                             "",
                             hiddenOutput
                         );

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

        // Set the ChallengeAnswer and verify it matches on next module.
        m_security_record->sChallengeAnswerHash = key;

        // Move to next module.
        changeModule(m_mod_function_index+1);
    }
    else
    {
        // Send back the single input received to show client key presses.
        // Only if return data shows a processed key returned.
        if(result != "empty")
        {
            baseProcessAndDeliver(result);
        }
    }

    return true;
}

/**
 * @brief Set Password Reset Challenge Answer
 * @return
 */
bool ModSignup::verifyChallengeAnswer(const std::string &input)
{
    std::cout << "password: " << input << std::endl;

    // password input, hot key or ENTER after..  hmm
    std::string key = "";
    bool hiddenOutput = true;
    std::string result = m_session_io.getInputField(
                             input,
                             key,
                             Config::sPassword_length,
                             "",
                             hiddenOutput
                         );

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

        // compare password to previous, then encrypt if they match
        // otherwise fail back if they don't and ask again.
        if(m_security_record->sChallengeAnswerHash.compare(key) == 0)
        {
            std::string password = m_encryption->generate_password(
                                       m_security_record->sChallengeAnswerHash,
                                       m_security_record->sSaltHash
                                   );

            if(password.size() == 0)
            {
                std::cout << "Error, ChallengeAnswer was empty" << std::endl;
                assert(false);
            }

            // setup the salted challenge answer.
            m_security_record->sChallengeAnswerHash = password;

            // Move to next module.
            changeModule(m_mod_function_index+1);
        }
        else
        {
            std::cout << "no match found" << std::endl;

            // Set the password back to blank
            m_security_record->sChallengeAnswerHash = "";

            // Invalid Entry, try again!
            std::string message = m_session_io.parseTextPrompt(
                                      m_text_prompts_dao->getPrompt(PROMPT_PASS_INVALID)
                                  );

            baseProcessAndDeliver(message);

            // Move to next module.
            changeModule(m_mod_function_index-1);
        }
    }
    else
    {
        // Send back the single input received to show client key presses.
        // Only if return data shows a processed key returned.
        if(result != "empty")
        {
            baseProcessAndDeliver(result);
        }
    }

    return true;
}

/**
 * @brief Get Yes / No Bar Preference
 * @return
 */
bool ModSignup::yesNoBars(const std::string &input)
{
    std::cout << "yesNoBars: " << input << std::endl;

    // yesNoBars input, hot key or ENTER after..  hmm
    std::string key = "";
    std::string result = m_session_io.getInputField(input, key, Config::sSingle_key_length);
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

        // Set the User Name
        //m_user_record->bYesNoBars = key;

        // Move to next module.
        changeModule(m_mod_function_index+1);
    }
    else
    {
        // Send back the single input received to show client key presses.
        // Only if return data shows a processed key returned.
        if(result != "empty")
        {
            baseProcessAndDeliver(result);
        }
    }

    return true;
}

/**
 * @brief Get Pause Preference
 * @return
 */
bool ModSignup::doPause(const std::string &input)
{
    std::cout << "doPause: " << input << std::endl;

    // doPause input, hot key or ENTER after..  hmm
    std::string key = "";
    std::string result = m_session_io.getInputField(input, key, Config::sSingle_key_length);
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

        // Set the User Name
        //m_user_record->bDoPause = key;

        // Move to next module.
        changeModule(m_mod_function_index+1);
    }
    else
    {
        // Send back the single input received to show client key presses.
        // Only if return data shows a processed key returned.
        if(result != "empty")
        {
            baseProcessAndDeliver(result);
        }
    }

    return true;
}

/**
 * @brief Get Clear or Scroll preference
 * @return
 */
bool ModSignup::clearOrScroll(const std::string &input)
{
    std::cout << "clearOrScroll: " << input << std::endl;

    // clearOrScroll input, hot key or ENTER after..  hmm
    std::string key = "";
    std::string result = m_session_io.getInputField(input, key, Config::sSingle_key_length);
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

        // Set the User Name
        //m_user_record->bClearOrScroll = key;

        // Move to next module.
        changeModule(m_mod_function_index+1);
    }
    else
    {
        // Send back the single input received to show client key presses.
        // Only if return data shows a processed key returned.
        if(result != "empty")
        {
            baseProcessAndDeliver(result);
        }
    }

    return true;
}

/**
 * @brief Get ANSI Color preference
 * @return
 */
bool ModSignup::ansiColor(const std::string &input)
{
    std::cout << "ansiColor: " << input << std::endl;

    // ansiColor input, hot key or ENTER after..  hmm
    std::string key = "";
    std::string result = m_session_io.getInputField(input, key, Config::sSingle_key_length);
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

        // Set the User Name
        //m_user_record->bAnsi = key;

        // Move to next module.
        changeModule(m_mod_function_index+1);
    }
    else
    {
        // Send back the single input received to show client key presses.
        // Only if return data shows a processed key returned.
        if(result != "empty")
        {
            baseProcessAndDeliver(result);
        }
    }

    return true;
}

/**
 * @brief Get Backspace Preference WINDOWS/LINUX (Terminal)
 * @return
 */
bool ModSignup::backSpace(const std::string &input)
{
    std::cout << "backSpace: " << input << std::endl;

    // backSpace input, hot key or ENTER after..  hmm
    std::string key = "";
    std::string result = m_session_io.getInputField(input, key, Config::sSingle_key_length);
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

        // Set the User Name
        //m_user_record->bBackSpaceVt100 = key;

        // Move to next module.
        changeModule(m_mod_function_index+1);
    }
    else
    {
        // Send back the single input received to show client key presses.
        // Only if return data shows a processed key returned.
        if(result != "empty")
        {
            baseProcessAndDeliver(result);
        }
    }

    return true;
}
