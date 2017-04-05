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

    value[PROMPT_NUP]                = std::make_pair("New User Password", "|CR|CRNew User Password: ");
    value[PROMPT_DISCLAIMER]         = std::make_pair("Disclaimer", "%%DFDISCLAIMER.ANS |CR[y/n] Disclaimer Agree: ");
    value[PROMPT_HANDLE]             = std::make_pair("User Handle", "Enter Handle: ");
    value[PROMPT_USER_NUMBER]        = std::make_pair("Your User Numer |OT", "Your user number is |OT.");
    value[PROMPT_REAL_NAME]          = std::make_pair("Real Name", "Enter Real Name: ");
    value[PROMPT_ADDRESS]            = std::make_pair("Address", "Address: ");
    value[PROMPT_LOCATION]           = std::make_pair("Location", "City/State: ");
    value[PROMPT_COUNTRY]            = std::make_pair("Country", "Country: ");
    value[PROMPT_EMAIL]              = std::make_pair("Email Address", "Email Address: ");
    value[PROMPT_USER_NOTE]          = std::make_pair("User Note", "Affiliations / User Note: ");
    value[PROMPT_BIRTH_DATE]         = std::make_pair("Birth Date", "[yyyy-mm-dd] Birth Date: ");
    value[PROMPT_GENDER]             = std::make_pair("Gender", "[m/f] Gender: ");
    value[PROMPT_PASSWORD]           = std::make_pair("Password", "(case sensitive) Password: ");
    value[PROMPT_VERIFY_PASSWORD]    = std::make_pair("Verify Password", "(case sensitive) Verify Password: ");
    value[PROMPT_CHALLENGE_QUESTION] = std::make_pair("Forgot Password Challenge Question", "Challenge Question: ");
    value[PROMPT_CHALLENGE_ANSWER]   = std::make_pair("Forgot Password Challenge Answer", "(case sensitive) Challenge Answer: ");
    value[PROMPT_VERIFY_ANSWER]      = std::make_pair("Forgot Password Verify Answer", "(case sensitive) Verify Answer: ");
    value[PROMPT_YESNO_BARS]         = std::make_pair("Use YES/NO Bars", "[y/ENTER or n] Use Yes/No Bars: ");
    value[PROMPT_USE_PAUSE]          = std::make_pair("Enable Screen Pausing", "[y/ENTER or n] Use Screen Pausing: ");
    value[PROMPT_USE_CLEAR]          = std::make_pair("Clear Screen or Scroll", "[y/ENTER or n] Use Clear Screen or Scrolling: ");
    value[PROMPT_USE_ANSI_COLOR]     = std::make_pair("Use Ansi Colors", "[y/ENTER or n] Use Ansi Colors: ");
    value[PROMPT_BACK_SPACE]         = std::make_pair("Backspace Sequence", "[W]indows/[T]erminal/[ENTER] to Set Backspace Key: ");
    value[PROMPT_VERIFY_SAVE]        = std::make_pair("Verify All Data", "[y/n] Verify and Save user record: ");

    // Invalid.
    value[PROMPT_TEXT_INVALID]       = std::make_pair("Invalid Text", "|04Invalid entry!|CR");
    value[PROMPT_DATE_INVALID]       = std::make_pair("Invalid Date", "|04Invalid date entered!|CR");
    value[PROMPT_PASS_INVALID]       = std::make_pair("Invalid/Non Matching Password", "|04Invalid, password does not match!|CR");
    value[PROMPT_HANDLE_INVALID]     = std::make_pair("User Name Already Exists", "|04Invalid UserName, Already Exists!|CR");
    value[PROMPT_NAME_INVALID]       = std::make_pair("Real Name Already Exists", "|04Name, Already Exists, Try Adding a middle initial.|CR");
    value[PROMPT_EMAIL_INVALID]      = std::make_pair("Email Already Exists", "|04Email, Already Exists, Try another adress or check if your have an account.|CR");

    // Confirmation of Save
    value[PROMPT_SAVED]              = std::make_pair("User Record Saved", "|CR|03User Record Saved Successfully.");
    value[PROMPT_NOT_SAVED]          = std::make_pair("User Record Not Saved", "|CR|04Error, User Record Not Saved!");

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
 * @brief Changes to Next module index.
 */
void ModSignup::changeNextModule()
{
    // Set, and Execute the Setup module.
    ++m_mod_function_index;
    m_setup_functions[m_mod_function_index]();
}

/**
 * @brief Changes to Previous module index.
 */
void ModSignup::changePreviousModule()
{
    // Set, and Execute the Setup module.
    --m_mod_function_index;
    m_setup_functions[m_mod_function_index]();
}

/**
 * @brief Redisplay's the current module prompt.
 * @param mod_function_index
 */
void ModSignup::redisplayModulePrompt()
{
    m_setup_functions[m_mod_function_index]();
}

/**
 * @brief Pull and Display Prompts
 * @param prompt
 */
void ModSignup::displayPrompt(const std::string &prompt)
{
    baseDisplayPrompt(prompt, m_text_prompts_dao);
}

/**
 * @brief Pull and Display Prompts with following newline
 * @param prompt
 */
void ModSignup::displayPromptAndNewLine(const std::string &prompt)
{
    baseDisplayPromptAndNewLine(prompt, m_text_prompts_dao);
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
        changeNextModule();
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
        changeNextModule();
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
        changeNextModule();
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
        changeNextModule();
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
        changeNextModule();
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
        changeNextModule();
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
        changeNextModule();
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
        changeNextModule();
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
        changeNextModule();
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
        changeNextModule();
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
        changeNextModule();
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
        changeNextModule();
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
        changeNextModule();
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
        changeNextModule();
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
        changeNextModule();
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
        changeNextModule();
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
        changeNextModule();
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
        changeNextModule();
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
        changeNextModule();
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
    std::string key = "";
    bool useHiddenOutput = true;
    std::string result = m_session_io.getInputField(input, key, Config::sPassword_length, "", useHiddenOutput);
    
    // ESC was hit
    if(result == "aborted")
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
            return false;
        }

        baseProcessDeliverNewLine();

        // Pull in and test aginst new user password.
        if(key.compare(m_config->password_newuser) == 0)
        {
            std::cout << "Match" << key.size() << std::endl;
            changeNextModule();
        }
        else
        {
            std::cout << "No Match" << key.size() << std::endl;
            ++m_newuser_password_attempts;
            displayPromptAndNewLine(PROMPT_PASS_INVALID);
            redisplayModulePrompt();
        }
    }
    else
    {
        // Send back the single input received to show client key presses.
        // Only if return data shows a processed key returned.
        if(result != "empty")
        {
            baseProcessDeliverInput(result);
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
    
    // ESC was hit
    if(result == "aborted") 
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
            return false;
        }

        baseProcessDeliverNewLine();

        // If ENTER Default to Yes, or Single Y is hit
        if(toupper(key[0]) == 'Y' && key.size() == 1)
        {
            changeNextModule();
        }
        // Else check for single N for No to default to ASCII no colors.
        else if(toupper(key[0]) == 'N' && key.size() == 1)
        {
            // Disconnect or exist back to matrix!
            m_is_active = false;
        }
        else
        {
            displayPromptAndNewLine(PROMPT_TEXT_INVALID);
            redisplayModulePrompt();
        }
    }
    else
    {
        // Send back the single input received to show client key presses.
        // Only if return data shows a processed key returned.
        if(result != "empty")
        {
            baseProcessDeliverInput(result);
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
    std::string key = "";
    std::string result = m_session_io.getInputField(input, key, Config::sName_length);
    
    // ESC was hit
    if(result == "aborted")
    {
        std::cout << "aborted!" << std::endl;
        m_is_active = false;
        return false;
    }
    else if(result[0] == '\n')
    {
        // Key == 0 on [ENTER] pressed alone. then invalid!
        if(key.size() == 0)
        {
            return false;
        }

        // Check for user name and if is already exists!
        users_dao_ptr user_data(new UsersDao(m_session_data->m_user_database));
        user_ptr search = user_data->getUserByHandle(key);
        
        baseProcessDeliverNewLine();

        if(!search || search->iId == -1)
        {
            std::cout << "no match found" << std::endl;
            m_user_record->sHandle = key;
            changeNextModule();
        }
        else
        {
            std::cout << "match found" << std::endl;
            displayPromptAndNewLine(PROMPT_HANDLE_INVALID);
            redisplayModulePrompt();
        }
    }
    else
    {
        // Send back the single input received to show client key presses.
        // Only if return data shows a processed key returned.
        if(result != "empty")
        {
            baseProcessDeliverInput(result);
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
    std::string key = "";
    std::string result = m_session_io.getInputField(input, key, Config::sName_length);
    
    // ESC was hit
    if(result == "aborted") 
    {
        std::cout << "aborted!" << std::endl;
        // exit, and return
        m_is_active = false;
        return false;
    }
    else if(result[0] == '\n')
    {
        if(key.size() == 0)
        {
            return false;
        }

        // Check for real name and if is already exists!
        users_dao_ptr user_data(new UsersDao(m_session_data->m_user_database));
        user_ptr search = user_data->getUserByRealName(key);

        baseProcessDeliverNewLine();

        if(!search || search->iId == -1)
        {
            std::cout << "no match found" << std::endl;
            m_user_record->sRealName = key;
            changeNextModule();
        }
        else
        {
            std::cout << "match found" << std::endl;
            displayPromptAndNewLine(PROMPT_NAME_INVALID);
            redisplayModulePrompt();
        }
    }
    else
    {
        // Send back the single input received to show client key presses.
        // Only if return data shows a processed key returned.
        if(result != "empty")
        {
            baseProcessDeliverInput(result);
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
    std::string key = "";
    std::string result = m_session_io.getInputField(input, key, Config::sDefault_question_length);
    
    // ESC was hit
    if(result == "aborted") 
    {
        std::cout << "aborted!" << std::endl;
        m_is_active = false;
        return false;
    }
    else if(result[0] == '\n')
    {
        // Key == 0 on [ENTER] pressed alone. then invalid!
        if(key.size() == 0)
        {
            return false;
        }
        
        baseProcessDeliverNewLine();
        m_user_record->sAddress = key;
        changeNextModule();
    }
    else
    {
        // Send back the single input received to show client key presses.
        // Only if return data shows a processed key returned.
        if(result != "empty")
        {
            baseProcessDeliverInput(result);
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
    std::string key = "";
    std::string result = m_session_io.getInputField(input, key, Config::sDefault_question_length);
    
    // ESC was hit
    if(result == "aborted") 
    {
        std::cout << "aborted!" << std::endl;
        m_is_active = false;
        return false;
    }
    else if(result[0] == '\n')
    {
        // Key == 0 on [ENTER] pressed alone. then invalid!
        if(key.size() == 0)
        {
            return false;
        }

        baseProcessDeliverNewLine();
        m_user_record->sLocation = key;
        changeNextModule();
    }
    else
    {
        // Send back the single input received to show client key presses.
        // Only if return data shows a processed key returned.
        if(result != "empty")
        {
            baseProcessDeliverInput(result);
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
    std::string key = "";
    std::string result = m_session_io.getInputField(input, key, Config::sDefault_question_length);
    
    // ESC was hit
    if(result == "aborted") 
    {
        std::cout << "aborted!" << std::endl;
        m_is_active = false;
        return false;
    }
    else if(result[0] == '\n')
    {
        // Key == 0 on [ENTER] pressed alone. then invalid!
        if(key.size() == 0)
        {
            return false;
        }

        baseProcessDeliverNewLine();
        m_user_record->sCountry = key;
        changeNextModule();
    }
    else
    {
        // Send back the single input received to show client key presses.
        // Only if return data shows a processed key returned.
        if(result != "empty")
        {
            baseProcessDeliverInput(result);
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
    std::string key = "";
    std::string result = m_session_io.getInputField(input, key, Config::sDefault_question_length);
    
    // ESC was hit
    if(result == "aborted") 
    {
        std::cout << "aborted!" << std::endl;
        m_is_active = false;
        return false;
    }
    else if(result[0] == '\n')
    {
        // Key == 0 on [ENTER] pressed alone. then invalid!
        if(key.size() == 0)
        {
            return false;
        }

        baseProcessDeliverNewLine();

        // Test if email already exists.
        users_dao_ptr user_data(new UsersDao(m_session_data->m_user_database));
        user_ptr search = user_data->getUserByEmail(key);        

        if(!search || search->iId == -1)
        {
            std::cout << "no match found" << std::endl;
            m_user_record->sEmail = key;
            changeNextModule();
        }
        else
        {
            std::cout << "match found" << std::endl;
            displayPromptAndNewLine(PROMPT_EMAIL_INVALID);
            redisplayModulePrompt();
        }
    }
    else
    {
        // Send back the single input received to show client key presses.
        // Only if return data shows a processed key returned.
        if(result != "empty")
        {
            baseProcessDeliverInput(result);
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
    std::string key = "";
    std::string result = m_session_io.getInputField(input, key, Config::sDefault_question_length);
    
    // ESC was hit
    if(result == "aborted")
    {
        std::cout << "aborted!" << std::endl;
        m_is_active = false;
        return false;
    }
    else if(result[0] == '\n')
    {
        // Key == 0 on [ENTER] pressed alone. then invalid!
        if(key.size() == 0)
        {
            return false;
        }

        baseProcessDeliverNewLine();
        m_user_record->sUserNote = key;
        changeNextModule();
    }
    else
    {
        // Send back the single input received to show client key presses.
        // Only if return data shows a processed key returned.
        if(result != "empty")
        {
            baseProcessDeliverInput(result);
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
    std::string key = "";
    std::string result = m_session_io.getInputField(input, key, Config::sDate_length);
    
    // ESC was hit
    if(result == "aborted") 
    {
        std::cout << "aborted!" << std::endl;
        m_is_active = false;
        return false;
    }
    else if(result[0] == '\n')
    {
        // Key == 0 on [ENTER] pressed alone. then invalid!
        if(key.size() == 0)
        {
            return false;
        }

        baseProcessDeliverNewLine();

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
            if(ss.fail())
            {
                std::cout << "regex passed, ss failed!" << std::endl;
                ss.clear();
                displayPromptAndNewLine(PROMPT_DATE_INVALID);
                redisplayModulePrompt();
                return true;
            }

            std::time_t const time = mktime(&tm);
            m_user_record->dtBirthday = time;
            changeNextModule();
        }
        else
        {
            displayPromptAndNewLine(PROMPT_DATE_INVALID);
            redisplayModulePrompt();
        }
    }
    else
    {
        // Send back the single input received to show client key presses.
        // Only if return data shows a processed key returned.
        if(result != "empty")
        {
            baseProcessDeliverInput(result);
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
    std::string key = "";
    std::string result = m_session_io.getInputField(input, key, Config::sSingle_key_length);
    
    // ESC was hit
    if(result == "aborted") 
    {
        std::cout << "aborted!" << std::endl;
        m_is_active = false;
        return false;
    }
    else if(result[0] == '\n')
    {
        // Key == 0 on [ENTER] pressed alone. then invalid!
        if(key.size() == 0)
        {
            return false;
        }
        
        baseProcessDeliverNewLine();
        
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
            displayPromptAndNewLine(PROMPT_TEXT_INVALID);
            redisplayModulePrompt();
            return true;
        }
        
        changeNextModule();
    }
    else
    {
        // Send back the single input received to show client key presses.
        // Only if return data shows a processed key returned.
        if(result != "empty")
        {
            baseProcessDeliverInput(result);
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
    std::string key = "";
    bool useHiddenOutput = true;
    std::string result = m_session_io.getInputField(
                             input,
                             key,
                             Config::sPassword_length,
                             "",
                             useHiddenOutput
                         );

    // ESC was hit
    if(result == "aborted")
    {
        std::cout << "aborted!" << std::endl;
        m_is_active = false;
        return false;
    }
    else if(result[0] == '\n')
    {
        // Key == 0 on [ENTER] pressed alone. then invalid!
        if(key.size() == 0)
        {
            return false;
        }

        baseProcessDeliverNewLine();
        m_security_record->sPasswordHash = key;
        changeNextModule();
    }
    else
    {
        // Send back the single input received to show client key presses.
        // Only if return data shows a processed key returned.
        if(result != "empty")
        {
            baseProcessDeliverInput(result);
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
    std::string key = "";
    bool useHiddenOutput = true;
    std::string result = m_session_io.getInputField(
                             input,
                             key,
                             Config::sPassword_length,
                             "",
                             useHiddenOutput
                         );

    // ESC was hit
    if(result == "aborted")
    {
        std::cout << "aborted!" << std::endl;
        m_is_active = false;
        return false;
    }
    else if(result[0] == '\n')
    {
        // Key == 0 on [ENTER] pressed alone. then invalid!
        if(key.size() == 0)
        {
            return false;
        }

        baseProcessDeliverNewLine();

        // compare password to previous, then encrypt if they match
        // otherwise fail back if they don't and ask again.
        if(m_security_record->sPasswordHash.compare(key) == 0)
        {
            // Load pointer to encrypt methods.
            encrypt_ptr encryption(new Encrypt());
            if (!encryption)
            {
                std::cout << "Error, unable to allocate encryption" << std::endl;
                displayPromptAndNewLine(PROMPT_PASS_INVALID);
                changePreviousModule();
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
            changeNextModule();
        }
        else
        {
            std::cout << "no match found" << std::endl;
            m_security_record->sPasswordHash = "";
            displayPromptAndNewLine(PROMPT_PASS_INVALID);
            changePreviousModule();
        }
    }
    else
    {
        // Send back the single input received to show client key presses.
        // Only if return data shows a processed key returned.
        if(result != "empty")
        {
            baseProcessDeliverInput(result);
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
    std::string key = "";
    std::string result = m_session_io.getInputField(input, key, Config::sPassword_length);
    
    // ESC was hit
    if(result == "aborted") 
    {
        std::cout << "aborted!" << std::endl;
        m_is_active = false;
        return false;
    }
    else if(result[0] == '\n')
    {
        // Key == 0 on [ENTER] pressed alone. then invalid!
        if(key.size() == 0)
        {
            return false;
        }
        
        baseProcessDeliverNewLine();

        // Set the Password and verify it matches on next module.
        m_security_record->sChallengeQuestion = key;
        changeNextModule();
    }
    else
    {
        // Send back the single input received to show client key presses.
        // Only if return data shows a processed key returned.
        if(result != "empty")
        {
            baseProcessDeliverInput(result);
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
    std::string key = "";
    bool useHiddenOutput = true;
    std::string result = m_session_io.getInputField(
                             input,
                             key,
                             Config::sPassword_length,
                             "",
                             useHiddenOutput
                         );

    // ESC was hit
    if(result == "aborted") 
    {
        std::cout << "aborted!" << std::endl;
        m_is_active = false;
        return false;
    }
    else if(result[0] == '\n')
    {
        // Key == 0 on [ENTER] pressed alone. then invalid!
        if(key.size() == 0)
        {
            return false;
        }
        
        baseProcessDeliverNewLine();

        m_security_record->sChallengeAnswerHash = key;
        changeNextModule();
    }
    else
    {
        // Send back the single input received to show client key presses.
        // Only if return data shows a processed key returned.
        if(result != "empty")
        {
            baseProcessDeliverInput(result);
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
    std::string key = "";
    bool useHiddenOutput = true;
    std::string result = m_session_io.getInputField(
                             input,
                             key,
                             Config::sPassword_length,
                             "",
                             useHiddenOutput
                         );

    // ESC was hit
    if(result == "aborted") 
    {
        std::cout << "aborted!" << std::endl;
        m_is_active = false;
        return false;
    }
    else if(result[0] == '\n')
    {
        // Key == 0 on [ENTER] pressed alone. then invalid!
        if(key.size() == 0)
        {
            return false;
        }
        
        baseProcessDeliverNewLine();

        // compare password to previous, then encrypt if they match
        // otherwise fail back if they don't and ask again.
        if(m_security_record->sChallengeAnswerHash.compare(key) == 0)
        {
            encrypt_ptr encryption(new Encrypt());
            if (!encryption)
            {
                std::cout << "Error, unable to allocate encryption" << std::endl;
                m_security_record->sChallengeAnswerHash = "";
                displayPromptAndNewLine(PROMPT_PASS_INVALID);
                changePreviousModule();
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

            m_security_record->sChallengeAnswerHash = password;
            changeNextModule();
        }
        else
        {
            std::cout << "no match found" << std::endl;
            m_security_record->sChallengeAnswerHash = "";
            displayPromptAndNewLine(PROMPT_PASS_INVALID);
            changePreviousModule();
        }
    }
    else
    {
        // Send back the single input received to show client key presses.
        // Only if return data shows a processed key returned.
        if(result != "empty")
        {
            baseProcessDeliverInput(result);
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
    std::string key = "";
    std::string result = m_session_io.getInputField(input, key, Config::sSingle_key_length);
    
    // ESC was hit
    if(result == "aborted") 
    {
        std::cout << "aborted!" << std::endl;
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
                std::string yes_prompt = "Yes";
                baseProcessDeliverInput(yes_prompt);
            }

            m_user_record->bYesNoBars = true;
            baseProcessDeliverNewLine();
        }
        // Else check for single N for No to default to ASCII no colors.
        else if(toupper(key[0]) == 'N' && key.size() == 1)
        {
            m_user_record->bYesNoBars = false;
            baseProcessDeliverNewLine();
        }
        else
        {
            baseProcessDeliverNewLine();
            displayPromptAndNewLine(PROMPT_TEXT_INVALID);
            redisplayModulePrompt();
            return true;
        }

        changeNextModule();
    }
    else
    {
        // Send back the single input received to show client key presses.
        // Only if return data shows a processed key returned.
        if(result != "empty")
        {
            baseProcessDeliverInput(result);
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
    std::string key = "";
    std::string result = m_session_io.getInputField(input, key, Config::sSingle_key_length);
    
    // ESC was hit
    if(result == "aborted") 
    {
        std::cout << "aborted!" << std::endl;
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
                std::string yes_prompt = "Yes";
                baseProcessDeliverInput(yes_prompt);
            }

            m_user_record->bDoPause = true;
            baseProcessDeliverNewLine();
        }
        // Else check for single N for No to default to ASCII no colors.
        else if(toupper(key[0]) == 'N' && key.size() == 1)
        {
            m_user_record->bDoPause = false;
            baseProcessDeliverNewLine();
        }
        else
        {
            baseProcessDeliverNewLine();
            displayPromptAndNewLine(PROMPT_TEXT_INVALID);
            redisplayModulePrompt();
            return true;
        }

        changeNextModule();
    }
    else
    {
        // Send back the single input received to show client key presses.
        // Only if return data shows a processed key returned.
        if(result != "empty")
        {
            baseProcessDeliverInput(result);
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
    std::string key = "";
    std::string result = m_session_io.getInputField(input, key, Config::sSingle_key_length);
    
    // ESC was hit
    if(result == "aborted") 
    {
        std::cout << "aborted!" << std::endl;
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
                std::string yes_prompt = "Yes";
                baseProcessDeliverInput(yes_prompt);
            }

            m_user_record->bClearOrScroll = true;
            baseProcessDeliverNewLine();
        }
        // Else check for single N for No to default to ASCII no colors.
        else if(toupper(key[0]) == 'N' && key.size() == 1)
        {
            m_user_record->bClearOrScroll = false;
            baseProcessDeliverNewLine();
        }
        else
        {
            baseProcessDeliverNewLine();
            displayPromptAndNewLine(PROMPT_TEXT_INVALID);
            redisplayModulePrompt();
            return true;
        }

        changeNextModule();
    }
    else
    {
        // Send back the single input received to show client key presses.
        // Only if return data shows a processed key returned.
        if(result != "empty")
        {
            baseProcessDeliverInput(result);
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
    std::string key = "";
    std::string result = m_session_io.getInputField(input, key, Config::sSingle_key_length);
    
    // ESC was hit
    if(result == "aborted") 
    {
        std::cout << "aborted!" << std::endl;
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
                std::string yes_prompt = "Yes";
                baseProcessDeliverInput(yes_prompt);
            }

            m_user_record->bAnsi = true;
            baseProcessDeliverNewLine();
        }
        // Else check for single N for No to default to ASCII no colors.
        else if(toupper(key[0]) == 'N' && key.size() == 1)
        {
            m_user_record->bAnsi = false;
            baseProcessDeliverNewLine();
        }
        else
        {
            baseProcessDeliverNewLine();
            displayPromptAndNewLine(PROMPT_TEXT_INVALID);
            redisplayModulePrompt();
            return true;
        }

        changeNextModule();
    }
    else
    {
        // Send back the single input received to show client key presses.
        // Only if return data shows a processed key returned.
        if(result != "empty")
        {
            baseProcessDeliverInput(result);
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
    std::string key = "";
    std::string result = m_session_io.getInputField(input, key, Config::sSingle_key_length);

    // ESC was hit
    if(result == "aborted") 
    {
        std::cout << "aborted!" << std::endl;
        m_is_active = false;
        return false;
    }
    else if(result[0] == '\n')
    {
        // If ENTER Default to Yes, or Single Y is hit
        if(key.size() == 0)
        {
            baseProcessDeliverNewLine();
            // Key == 0 on [ENTER] pressed alone.
            std::string detection_prompt = "- Detection WIP, Select W or T.";
            baseProcessAndDeliverNewLine(detection_prompt);

            // Not Implimented YET
            // Start Backspace Detection here!
            redisplayModulePrompt();
            return true;

        }
        else if(toupper(key[0]) == 'W' && key.size() == 1)
        {
            m_user_record->bBackSpaceVt100 = false;
            baseProcessDeliverNewLine();
        }
        // Else check for single N for No to default to ASCII no colors.
        else if(toupper(key[0]) == 'T' && key.size() == 1)
        {
            m_user_record->bBackSpaceVt100 = true;
            baseProcessDeliverNewLine();
        }
        else
        {
            baseProcessDeliverNewLine();
            displayPromptAndNewLine(PROMPT_TEXT_INVALID);
            redisplayModulePrompt();
            return true;
        }

        changeNextModule();
    }
    else
    {
        // Send back the single input received to show client key presses.
        // Only if return data shows a processed key returned.
        if(result != "empty")
        {
            baseProcessDeliverInput(result);
        }
    }

    return true;
}


/**
 * @brief Save New User Record
 */
void ModSignup::saveNewUserRecord()
{
    // StartUp Data Access Objects for SQL
    users_dao_ptr user_dao(new UsersDao(m_session_data->m_user_database));
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
    
    // Set Default User Theme colors, pull from system defaults.
    m_user_record->sRegColor = m_config->default_color_regular;
    m_user_record->sPromptColor = m_config->default_color_prompt;
    m_user_record->sInputColor = m_config->default_color_input;
    m_user_record->sInverseColor = m_config->default_color_inverse;
    m_user_record->sStatColor = m_config->default_color_stat;
    m_user_record->sBoxColor = m_config->default_color_box;

    long userIndex = user_dao->insertUserRecord(m_user_record);
    if (userIndex < 0)
    {
        std::cout << "Error, unable to insert new user record." << std::endl;

        // Remove Secutiry Record if unable to create user record.
        if (!security_dao->deleteSecurityRecord(securityIndex))
        {
            std::cout << "Error, unable to remove secutiry record." << std::endl;
        }
        
        baseProcessDeliverNewLine();
        displayPromptAndNewLine(PROMPT_NOT_SAVED);
        m_is_active = false;
        return;
    }

    // Completed Successfully
    baseProcessDeliverNewLine();
    displayPromptAndNewLine(PROMPT_SAVED);

    m_is_active = false;
    return;
}


/**
 * @brief Confirm and save user record.
 * @return
 */
bool ModSignup::verifyAndSave(const std::string &input)
{
    std::cout << "verifyAndSave: " << input << std::endl;
    std::string key = "";
    std::string result = m_session_io.getInputField(input, key, Config::sSingle_key_length);
    
    // ESC was hit
    if(result == "aborted") 
    {
        std::cout << "aborted!" << std::endl;
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
                std::string yes_prompt = "Yes";
                baseProcessDeliverInput(yes_prompt);
            }

            baseProcessDeliverNewLine();

            saveNewUserRecord();
            return false;
        }
        // Else check for single N for No to default to ASCII no colors.
        else if(toupper(key[0]) == 'N' && key.size() == 1)
        {
            // Abort and Return;
            m_is_active = false;
            baseProcessDeliverNewLine();
            return false;
        }
        else
        {
            baseProcessDeliverNewLine();
            displayPromptAndNewLine(PROMPT_TEXT_INVALID);
            redisplayModulePrompt();
            return true;
        }
    }
    else
    {
        // Send back the single input received to show client key presses.
        // Only if return data shows a processed key returned.
        if(result != "empty")
        {
            baseProcessDeliverInput(result);
        }
    }

    return true;
}
