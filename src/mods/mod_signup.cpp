#include "mod_signup.hpp"

#include "../model/config.hpp"
#include "../model/users.hpp"
#include "../encryption.hpp"

// DataBase
#include "../data/security_dao.hpp"
#include "../data/users_dao.hpp"

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
    std::string prompt = "\x1b[?25h"; // Turn on Cursor.
    baseProcessAndDeliver(prompt);


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
    value[PROMPT_HANDLE]             = std::make_pair("User Handle", "|08|CR|CREnter Handle : |04");
    value[PROMPT_REAL_NAME]          = std::make_pair("Real Name", "|08|CREnter Real Name : |04");
    value[PROMPT_ADDRESS]            = std::make_pair("Address", "|08|CRAddress : |04");
    value[PROMPT_LOCATION]           = std::make_pair("Location", "|08|CRCity/State : |04");
    value[PROMPT_COUNTRY]            = std::make_pair("Country", "|08|CRCountry : |04");
    value[PROMPT_EMAIL]              = std::make_pair("Email Address", "|08|CREmail Address : |04");
    value[PROMPT_USER_NOTE]          = std::make_pair("User Note", "|08|CRAffiliations / User Note : |04");
    value[PROMPT_BIRTH_DATE]         = std::make_pair("Birth Date", "|08|CR[yyyy-mm-dd] Birth Date : |04");
    value[PROMPT_GENDER]             = std::make_pair("Gender", "|CR|08[m/f] Gender : |04");
    value[PROMPT_PASSWORD]           = std::make_pair("Password", "|CR|CR|08(case sensitive) Password : |04");
    value[PROMPT_VERIFY_PASSWORD]    = std::make_pair("Verify Password", "|CR|08(case sensitive) Verify Password : |04");
    value[PROMPT_CHALLENGE_QUESTION] = std::make_pair("Forgot Password Challenge Question", "|CR|CR|08Challenge Question : |04");
    value[PROMPT_CHALLENGE_ANSWER]   = std::make_pair("Forgot Password Challenge Answer", "|CR|08(case sensitive) Challenge Answer : |04");
    value[PROMPT_VERIFY_ANSWER]      = std::make_pair("Forgot Password Verify Answer", "|CR|08(case sensitive) Verify Answer : |04");
    value[PROMPT_YESNO_BARS]         = std::make_pair("Use YES/NO Bars", "|CR|CR|08[y/n] Use Yes/No Bars [ENTER] = Yes: |04");
    value[PROMPT_USE_PAUSE]          = std::make_pair("Pause on ", "|CR|CR|08Screen Pausing [ENTER] = Yes: |04");
    value[PROMPT_USE_CLEAR]          = std::make_pair("Clear Screen or Scroll ", "|CR|CR|08Clear Screen or Scroll [ENTER] = Yes: |04");
    value[PROMPT_USE_ANSI_COLOR]     = std::make_pair("Use Ansi Color ", "|CR|CR|08[y/n] Ansi Color [ENTER] = Yes: |04");
    value[PROMPT_BACK_SPACE]         = std::make_pair("Backspace Sequence", "|CR|CR|08Backspace Key |CR[(W)indows/(T)erminal/(D)etect [ENTER] = Detect] : |04");
    value[PROMPT_VERIFY_SAVE]        = std::make_pair("Verify All Data", "|CR|CR|08[y/n] Verify and Save user record [ENTER] = Yes: |04");


    // Invalid.
    value[PROMPT_TEXT_INVALID]       = std::make_pair("Invalid Text", "|CR|04Invalid entry!|CR");
    value[PROMPT_DATE_INVALID]       = std::make_pair("Invalid Date", "|CR|04Invalid date entered!|CR");
    value[PROMPT_PASS_INVALID]       = std::make_pair("Invalid/Non Matching Password", "|CR|04Invalid, password does not match!|CR");
    value[PROMPT_HANDLE_INVALID]     = std::make_pair("User Name Already Exists", "|CR|04Invalid UserName, Already Exists!|CR");
    value[PROMPT_NAME_INVALID]       = std::make_pair("Real Name Already Exists", "|CR|04Name, Already Exists, Try Adding a middle initial.|CR");
    value[PROMPT_EMAIL_INVALID]      = std::make_pair("Email Already Exists", "|CR|04Email, Already Exists, Try another adress or check if your have an account.|CR");

    // Confirmation of Save
    value[PROMPT_SAVED]              = std::make_pair("User Record Saved", "|CR|10User Record Saved Successfully.|CR");
    value[PROMPT_NOT_SAVED]          = std::make_pair("User Record Not Saved", "|CR|04Error, User Record Not Saved!|CR");

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
 * @brief Pull and Display Prompts
 * @param prompt
 */
void ModSignup::displayPrompt(const std::string &prompt)
{
    std::string result = m_session_io.parseTextPrompt(
                                 m_text_prompts_dao->getPrompt(prompt)
                             );

    baseProcessAndDeliver(result);
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
        // Kick back to Matrix if we exceed the number of attempts.
        if (m_newuser_password_attempts >= m_config->invalid_newuser_password_attempts)
        {
            m_is_active = false;
            return;
        }
        
        displayPrompt(PROMPT_NUP);
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
        displayPrompt(PROMPT_DISCLAIMER);
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
    if(m_config->use_handle)
    {
        displayPrompt(PROMPT_HANDLE);        
    }
    else
    {
        // Move to next module.
        changeModule(m_mod_function_index+1);
    }
        
}

/**
 * @brief Get Real Name
 * @return
 */
void ModSignup::setupRealName()
{
    if(m_config->use_real_name)
    {
        displayPrompt(PROMPT_REAL_NAME);        
    }
    else
    {
        // Move to next module.
        changeModule(m_mod_function_index+1);
    }
}

/**
 * @brief Get Address
 * @return
 */
void ModSignup::setupAddress()
{
    if(m_config->use_address)
    {
        displayPrompt(PROMPT_ADDRESS);
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
    if(m_config->use_location)
    {
        displayPrompt(PROMPT_LOCATION);
    }
    else
    {
        // Move to next module.
        changeModule(m_mod_function_index+1);
    }
}

/**
 * @brief Get Country
 * @return
 */
void ModSignup::setupCountry()
{
    if(m_config->use_country)
    {
        displayPrompt(PROMPT_COUNTRY);
    }
    else
    {
        // Move to next module.
        changeModule(m_mod_function_index+1);
    }
}

/**
 * @brief Get Email
 * @return
 */
void ModSignup::setupEmail()
{
    if(m_config->use_email)
    {
        displayPrompt(PROMPT_EMAIL);
    }
    else
    {
        // Move to next module.
        changeModule(m_mod_function_index+1);
    }
}

/**
 * @brief Get UserNote
 * @return
 */
void ModSignup::setupUserNote()
{
    if(m_config->use_user_note)
    {
        displayPrompt(PROMPT_USER_NOTE);
    }
    else
    {
        // Move to next module.
        changeModule(m_mod_function_index+1);
    }
}

/**
 * @brief Get Birthdate
 * @return
 */
void ModSignup::setupBirthday()
{
    if(m_config->use_birthdate)
    {
        displayPrompt(PROMPT_BIRTH_DATE);
    }
    else
    {
        // Move to next module.
        changeModule(m_mod_function_index+1);
    }
}

/**
 * @brief Get Gender
 * @return
 */
void ModSignup::setupGender()
{
    if(m_config->use_gender)
    {
        displayPrompt(PROMPT_GENDER);
    }
    else
    {
        // Move to next module.
        changeModule(m_mod_function_index+1);
    }    
}

/**
 * @brief Get Password
 * @return
 */
void ModSignup::setupPassword()
{
    displayPrompt(PROMPT_PASSWORD);
}

/**
 * @brief Verify Password
 * @return
 */
void ModSignup::setupVerifyPassword()
{
    displayPrompt(PROMPT_VERIFY_PASSWORD);
}

/**
 * @brief Set Password Reset Challenge Question
 * @return
 */
void ModSignup::setupChallengeQuestion()
{
    if(m_config->use_challenge_question)
    {
        displayPrompt(PROMPT_CHALLENGE_QUESTION);        
    }
    else
    {
        // Move to next module.
        changeModule(m_mod_function_index+1);
    }   
}

/**
 * @brief Set Password Reset Challenge Answer
 * @return
 */
void ModSignup::setupChallengeAnswer()
{
    if(m_config->use_challenge_question)
    {
        displayPrompt(PROMPT_CHALLENGE_ANSWER);        
    }
    else
    {
        // Move to next module.
        changeModule(m_mod_function_index+1);
    }   
        
}

/**
 * @brief Set Password Reset Challenge Answer
 * @return
 */
void ModSignup::setupVerifyChallengeAnswer()
{
    if(m_config->use_challenge_question)
    {
        displayPrompt(PROMPT_VERIFY_ANSWER);        
    }
    else
    {
        // Move to next module.
        changeModule(m_mod_function_index+1);
    }   
}

/**
 * @brief Get Yes / No Bar Preference
 * @return
 */
void ModSignup::setupYesNoBars()
{
    if(m_config->use_yesno_bars)
    {
        displayPrompt(PROMPT_YESNO_BARS);        
    }
    else
    {
        // Move to next module.
        changeModule(m_mod_function_index+1);
    }  
}

/**
 * @brief Get Pause Preference
 * @return
 */
void ModSignup::setupDoPause()
{
    if(m_config->use_pause)
    {
        displayPrompt(PROMPT_USE_PAUSE);        
    }
    else
    {
        // Move to next module.
        changeModule(m_mod_function_index+1);
    }  
}

/**
 * @brief Get Clear or Scroll preference
 * @return
 */
void ModSignup::setupClearOrScroll()
{
    if(m_config->use_clear_screen)
    {
        displayPrompt(PROMPT_USE_CLEAR);        
    }
    else
    {
        // Move to next module.
        changeModule(m_mod_function_index+1);
    }  
}

/**
 * @brief Get ANSI Color preference
 * @return
 */
void ModSignup::setupAnsiColor()
{
    if(m_config->use_ansi_color)
    {
        displayPrompt(PROMPT_USE_ANSI_COLOR);        
    }
    else
    {
        // Move to next module.
        changeModule(m_mod_function_index+1);
    }  
}

/**
 * @brief Get Backspace Preference WINDOWS/LINUX (Terminal)
 * @return
 */
void ModSignup::setupBackSpace()
{
    if(m_config->use_backspace)
    {
        displayPrompt(PROMPT_BACK_SPACE);        
    }
    else
    {
        // Move to next module.
        changeModule(m_mod_function_index+1);
    }  
}


/**
 * @brief Confirm and save user record.
 * @return
 */
void ModSignup::setupVerifyAndSave()
{
    displayPrompt(PROMPT_VERIFY_SAVE);
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
    bool hiddenOutput = true;
    std::string result = m_session_io.getInputField(input, key, Config::sPassword_length, "", hiddenOutput);
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
            ++m_newuser_password_attempts;
            
            displayPrompt(PROMPT_PASS_INVALID);

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
            displayPrompt(PROMPT_TEXT_INVALID);

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

        if(!search || search->iId == -1)
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
            displayPrompt(PROMPT_HANDLE_INVALID);

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

        if(!search || search->iId == -1)
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
            displayPrompt(PROMPT_NAME_INVALID);

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


        // Test if email already exists.
        users_dao_ptr user_data(new UsersDao(m_session_data->m_user_database));
        user_ptr search = user_data->getUserByEmail(key);

        if(!search || search->iId == -1)
        {
            std::cout << "no match found" << std::endl;

            // Set the User Name
            m_user_record->sEmail = key;

            // Move to next module.
            changeModule(m_mod_function_index+1);
        }
        else
        {
            std::cout << "match found" << std::endl;

            // Invalid Entry, try again!
            displayPrompt(PROMPT_EMAIL_INVALID);

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
            // Append Time For Date.
            key += " 00:00:00";
            struct std::tm tm;
            std::istringstream ss(key);

            ss >> std::get_time(&tm, "%Y-%m-%d %H:%M:%S");

            // Make sure date format was parsed properly.
            if(ss.fail())
            {
                std::cout << "regex passed, ss failed!" << std::endl;
                ss.clear();

                // Invalid Entry, try again!
                displayPrompt(PROMPT_DATE_INVALID);

                // Move to next module.
                changeModule(m_mod_function_index);
            }

            std::time_t const time = mktime(&tm);

            // Set the User Birth Date
            m_user_record->dtBirthday = time;

            // Move to next module.
            changeModule(m_mod_function_index+1);
        }
        else
        {
            // Invalid Entry, try again!
            displayPrompt(PROMPT_DATE_INVALID);

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

        // If ENTER Default to Yes, or Single Y is hit
        if((toupper(key[0]) == 'M' && key.size() == 1))
        {
            m_user_record->sGender = key;
        }
        // Else check for single N for No to default to ASCII no colors.
        else if(toupper(key[0]) == 'F' && key.size() == 1)
        {
            m_user_record->sGender = key;
        }
        else
        {
            // Invalid Entry, try again!
            displayPrompt(PROMPT_TEXT_INVALID);

            // Ask Again And Redisplay.
            changeModule(m_mod_function_index);
            return true;
        }

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
            // Load pointer to encrypt methods.
            encrypt_ptr encryption(new Encrypt());
            if (!encryption) 
            {
                std::cout << "Error, unable to allocate encryption" << std::endl;
                
                // Invalid Entry, display try again!
                displayPrompt(PROMPT_PASS_INVALID);

                // Move to next module.
                changeModule(m_mod_function_index-1);
                return false;
            }
        
            std::string salt = encryption->generate_salt(m_user_record->sHandle, m_config->bbs_uuid);
            std::string password = encryption->generate_password(m_security_record->sPasswordHash, salt);

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
            displayPrompt(PROMPT_PASS_INVALID);

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
            encrypt_ptr encryption(new Encrypt());
            if (!encryption) 
            {
                std::cout << "Error, unable to allocate encryption" << std::endl;
                // Set the password back to blank
                m_security_record->sChallengeAnswerHash = "";

                // Invalid Entry, try again!
                displayPrompt(PROMPT_PASS_INVALID);
                
                // Move to next module.
                changeModule(m_mod_function_index-1);
                return true;
            }
            
            // Generate Password Hash
            std::string password = encryption->generate_password(
                                       m_security_record->sChallengeAnswerHash,
                                       m_security_record->sSaltHash
                                   );

            if(password.size() == 0)
            {
                // Shouldn't have any empty fields.
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
            displayPrompt(PROMPT_PASS_INVALID);
            
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
        // If ENTER Default to Yes, or Single Y is hit
        if(key.size() == 0 || (toupper(key[0]) == 'Y' && key.size() == 1))
        {
            // Key == 0 on [ENTER] pressed alone.
            if(key.size() == 0)
            {
                // If ENTER, then display Yes as key press.
                baseProcessAndDeliver("Yes");
            }

            m_user_record->bYesNoBars = true;
        }
        // Else check for single N for No to default to ASCII no colors.
        else if(toupper(key[0]) == 'N' && key.size() == 1)
        {
            m_user_record->bYesNoBars = false;
        }
        else
        {
            // Invalid Entry, try again!
            displayPrompt(PROMPT_TEXT_INVALID);

            // Ask Again And Redisplay.
            changeModule(m_mod_function_index);
            return true;
        }

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
        // If ENTER Default to Yes, or Single Y is hit
        if(key.size() == 0 || (toupper(key[0]) == 'Y' && key.size() == 1))
        {
            // Key == 0 on [ENTER] pressed alone.
            if(key.size() == 0)
            {
                // If ENTER, then display Yes as key press.
                baseProcessAndDeliver("Yes");
            }

            m_user_record->bDoPause = true;
        }
        // Else check for single N for No to default to ASCII no colors.
        else if(toupper(key[0]) == 'N' && key.size() == 1)
        {
            m_user_record->bDoPause = false;
        }
        else
        {
            // Invalid Entry, try again!
            displayPrompt(PROMPT_TEXT_INVALID);

            // Ask Again And Redisplay.
            changeModule(m_mod_function_index);
            return true;
        }

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
        // If ENTER Default to Yes, or Single Y is hit
        if(key.size() == 0 || (toupper(key[0]) == 'Y' && key.size() == 1))
        {
            // Key == 0 on [ENTER] pressed alone.
            if(key.size() == 0)
            {
                // If ENTER, then display Yes as key press.
                baseProcessAndDeliver("Yes");
            }

            m_user_record->bClearOrScroll = true;
        }
        // Else check for single N for No to default to ASCII no colors.
        else if(toupper(key[0]) == 'N' && key.size() == 1)
        {
            m_user_record->bClearOrScroll = false;
        }
        else
        {
            // Invalid Entry, try again!
            displayPrompt(PROMPT_TEXT_INVALID);

            // Ask Again And Redisplay.
            changeModule(m_mod_function_index);
            return true;
        }

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
        // If ENTER Default to Yes, or Single Y is hit
        if(key.size() == 0 || (toupper(key[0]) == 'Y' && key.size() == 1))
        {
            // Key == 0 on [ENTER] pressed alone.
            if(key.size() == 0)
            {
                // If ENTER, then display Yes as key press.
                baseProcessAndDeliver("Yes");
            }

            m_user_record->bAnsi = true;
        }
        // Else check for single N for No to default to ASCII no colors.
        else if(toupper(key[0]) == 'N' && key.size() == 1)
        {
            m_user_record->bAnsi = false;
        }
        else
        {
            // Invalid Entry, try again!
            displayPrompt(PROMPT_TEXT_INVALID);

            // Ask Again And Redisplay.
            changeModule(m_mod_function_index);
            return true;
        }

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
       // If ENTER Default to Yes, or Single Y is hit
        if(key.size() == 0)
        {
            // Key == 0 on [ENTER] pressed alone.
            // If ENTER, then display Yes as key press.
            baseProcessAndDeliver("Detection WIP, Select W or T.");

            // Not Implimented YET
            // Start Backspace Detection here!
            changeModule(m_mod_function_index);
            return true;

        }
        else if(toupper(key[0]) == 'W' && key.size() == 1)
        {
            // Use Default Windows Termnal Backspace
            m_user_record->bBackSpaceVt100 = false;
        }
        // Else check for single N for No to default to ASCII no colors.
        else if(toupper(key[0]) == 'T' && key.size() == 1)
        {
            // Use Degault Linux Terminal Backspace
            m_user_record->bBackSpaceVt100 = true;
        }
        else
        {
            // Invalid Entry, try again!
            displayPrompt(PROMPT_TEXT_INVALID);
            
            // Ask Again And Redisplay.
            changeModule(m_mod_function_index);
            return true;
        }

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
 * @brief Save New User Record
 */
void ModSignup::saveNewUserRecord()
{
    // Link to users dao for data access object
    users_dao_ptr user_dao(new UsersDao(m_session_data->m_user_database));

    // Link to security dao for data access object
    security_dao_ptr security_dao(new SecurityDao(m_session_data->m_user_database));

    // Save New Security Record, index is then inserted into user record
    long securityIndex = security_dao->insertSecurityRecord(m_security_record);
    if (securityIndex < 0)
    {
        std::cout << "Error, unable to insert new user record." << std::endl;

        displayPrompt(PROMPT_NOT_SAVED);

        m_is_active = false;
        return;
    }

    // Save New User Record
    m_user_record->iSecurityIndex = securityIndex;

    //time_t const t = (time_t)time(0);
    std::time_t tt = 0;
    std::time_t const result = std::time(&tt);

    m_user_record->dtFirstOn = result;
    m_user_record->dtPassChangeDate = result;
    m_user_record->dtLastCallDate = result;

    long userIndex = user_dao->insertUserRecord(m_user_record);
    if (userIndex < 0)
    {
        std::cout << "Error, unable to insert new user record." << std::endl;

        // Remove Secutiry Record if unable to create user record.
        if (!security_dao->deleteSecurityRecord(securityIndex))
        {
            std::cout << "Error, unable to remove secutiry record." << std::endl;
        }

        displayPrompt(PROMPT_NOT_SAVED);

        m_is_active = false;
        return;
    }

    // Completed Successfully
    displayPrompt(PROMPT_SAVED);


    m_is_active = false;
    return;
}


/**
 * @brief Confirm and save user record.
 * @return
 */
bool ModSignup::verifyAndSave(const std::string &input) {

    std::cout << "verifyAndSave: " << input << std::endl;

    // verifyAndSave input, hot key or ENTER after..  hmm
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
        // If ENTER Default to Yes, or Single Y is hit
        if(key.size() == 0 || (toupper(key[0]) == 'Y' && key.size() == 1))
        {
            // Key == 0 on [ENTER] pressed alone.
            if(key.size() == 0)
            {
                // If ENTER, then display Yes as key press.
                baseProcessAndDeliver("Yes");
            }

            // Save The User Record
            saveNewUserRecord();

            return false;
        }
        // Else check for single N for No to default to ASCII no colors.
        else if(toupper(key[0]) == 'N' && key.size() == 1)
        {
            // Abort and Return;
            m_is_active = false;
            return false;
        }
        else
        {
            // Invalid Entry, try again!
            displayPrompt(PROMPT_TEXT_INVALID);

            // Ask Again And Redisplay.
            changeModule(m_mod_function_index);
            return true;
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
