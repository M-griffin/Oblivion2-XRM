#include "mod_logon.hpp"
#include "../model-sys/config.hpp"
#include "../encryption.hpp"

#include "../model-sys/security.hpp"
#include "../model-sys/users.hpp"
#include "../data-sys/security_dao.hpp"
#include "../data-sys/users_dao.hpp"
#include "../logging.hpp"

#include <iostream>
#include <string>
#include <sstream>

/**
 * @brief Handles Updates or Data Input from Client
 * @return bool, not used anymore?!?
 */
bool ModLogon::update(const std::string &character_buffer, const bool &)
{
    // Make sure system is active, when system is done, success or fails
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

    // Process all incoming data straight to the input functions.
    m_mod_functions[m_mod_function_index](character_buffer);

    return true;
}

/**
 * @brief Startup class, setup and display initial screens / interface.
 * @return
 */
bool ModLogon::onEnter()
{
    m_is_active = true;

    // Grab ANSI Screen, display, if desired.. logon.ans maybe?
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
bool ModLogon::onExit()
{
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

    value[PROMPT_LOGON]               = std::make_pair("Logon Prompt", "|CR|CR|15[|07Enter Username, Real Name, Email or User # to Login|15] |CRlogon: ");
    value[PROMPT_USERNUMBER]          = std::make_pair("Your User Number is |OT", "|CR|15Use your user number |03|OT |15for quick logins |CR");
    value[PROMPT_PASSWORD]            = std::make_pair("Password Prompt", "password: ");
    value[PROMPT_USE_INVALID]         = std::make_pair("Invalid Entry", "|04Invalid Response! Try again.|CR");
    value[PROMPT_INVALID_USERNAME]    = std::make_pair("Invalid Username", "|04Invalid Username! Try again.|CR");
    value[PROMPT_INVALID_PASSWORD]    = std::make_pair("Invalid Password", "|04Invalid Password! Try again.|CR");

    m_text_prompts_dao->writeValue(value);
}

/**
 * @brief Sets an individual module index.
 * @param mod_function_index
 */
void ModLogon::changeModule(int mod_function_index)
{
    // Set, and Execute the Setup module.
    m_mod_function_index = mod_function_index;
    m_setup_functions[m_mod_function_index]();
}

/**
 * @brief Changes to Next module index.
 */
void ModLogon::changeNextModule()
{
    // Set, and Execute the Setup module.
    ++m_mod_function_index;
    m_setup_functions[m_mod_function_index]();
}

/**
 * @brief Changes to Previous module index.
 */
void ModLogon::changePreviousModule()
{
    // Set, and Execute the Setup module.
    --m_mod_function_index;
    m_setup_functions[m_mod_function_index]();
}

/**
 * @brief Redisplay's the current module prompt.
 * @param mod_function_index
 */
void ModLogon::redisplayModulePrompt()
{
    m_setup_functions[m_mod_function_index]();
}

/**
 * @brief Pull and Display Prompts
 * @param prompt
 */
void ModLogon::displayPrompt(const std::string &prompt)
{
    baseDisplayPrompt(prompt, m_text_prompts_dao);
}

/**
 * @brief Pull and Display Prompts with following newline
 * @param prompt
 */
void ModLogon::displayPromptAndNewLine(const std::string &prompt)
{
    baseDisplayPromptAndNewLine(prompt, m_text_prompts_dao);
}

/**
 * @brief Validates user Logon
 * @return
 */
void ModLogon::setupLogon()
{
    displayPrompt(PROMPT_LOGON);
}

/**
 * @brief Display the UserNumber on Logon.
 */
void ModLogon::displayUserNumber()
{
    M_StringPair prompt_set = m_text_prompts_dao->getPrompt(PROMPT_USERNUMBER);

    std::string mci_code = "|OT";
    std::string result = prompt_set.second;
    std::string user_number = std::to_string(m_logon_user->iId);

    m_session_io.m_common_io.parseLocalMCI(result, mci_code, user_number);
    result = m_session_io.pipe2ansi(result);
    result += "\r\n";
    baseProcessAndDeliver(result);
}

/**
 * @brief Validates user logon password
 * @return
 */
void ModLogon::setupPassword()
{
    displayPrompt(PROMPT_PASSWORD);
}

/**
 * @brief Validates user logon password challenge questions
 * @return
 */
void ModLogon::setupPasswordQuestion()
{
    displayPrompt(PROMPT_PASSWORD_QUESTION);
}

/**
 * @brief Validates user logon password challenge questions
 * @return
 */
void ModLogon::setupPasswordAnswer()
{
    displayPrompt(PROMPT_PASSWORD_ANSWER);
}

/**
 * @brief Changes user logon password
 * @return
 */
void ModLogon::setupPasswordChange()
{

}

/**
 * @brief Lookup user records Id, Handle, Name, or Email
 * @return
 */
bool ModLogon::checkUserLogon(const std::string &input)
{
    // Check for user name and if is already exists!
    users_dao_ptr user_data; //(new UsersDao(m_session_data->m_user_database));

    // Check if a Digit, if so, lookup by userId.
    if(m_session_data->m_common_io.isDigit(input))
    {
        long userId = 0;
        std::stringstream ss(input);
        ss >> userId;

        if(!ss.fail())
        {
            m_logon_user = user_data->getRecordById(userId);

            if(m_logon_user && m_logon_user->iId != -1)
            {
                ss.clear();
                return true;
            }
        }
    }

    // Pass through all (3) checks if they are all enabled.
    if(m_config->use_handle)
    {
        m_logon_user = user_data->getUserByHandle(input);

        if(m_logon_user && m_logon_user->iId != -1)
        {
            displayUserNumber();
            return true;
        }
    }

    if(m_config->use_email)
    {
        m_logon_user = user_data->getUserByEmail(input);

        if(m_logon_user && m_logon_user->iId != -1)
        {
            displayUserNumber();
            return true;
        }
    }

    if(m_config->use_real_name)
    {
        m_logon_user = user_data->getUserByRealName(input);

        if(m_logon_user && m_logon_user->iId != -1)
        {
            displayUserNumber();
            return true;
        }
    }

    return false;
}

/**
 * @brief Validates user Logon
 * @return
 */
bool ModLogon::logon(const std::string &input)
{
    Logging *log = Logging::instance();
    std::string key = "";
    std::string result = m_session_io.getInputField(input, key, Config::sName_length);

    // ESC was hit
    if(result == "aborted")
    {
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

        baseProcessDeliverNewLine();

        // Check if users enter valid identifier.
        if(checkUserLogon(key))
        {
            log->write<Logging::CONSOLE_LOG>("PROMPT_USERNAME=", m_logon_user->sHandle);
            changeNextModule();
        }
        else
        {
            log->write<Logging::ERROR_LOG>("PROMPT_INVALID_USERNAME=", key);

            displayPromptAndNewLine(PROMPT_INVALID_USERNAME);
            ++m_failure_attempts;

            // If max, then exit back to matrix.
            // NOTE Separate login/password attempts or change to login?
            if(m_failure_attempts >= m_config->invalid_password_attempts)
            {
                m_is_active = false;
                return false;
            }

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

    return false;
}

/**
 * @brief Encodes and Validates User Password
 * @param input
 * @return
 */
bool ModLogon::validate_password(const std::string &input)
{
    // No empty strings passed.
    if(input.size() == 0)
    {
        return false;
    }

    Logging *log = Logging::instance();

    // First load the secure record for the existing user.
    // Link to security dao for data access object
    security_dao_ptr security_dao; //(new SecurityDao(m_session_data->m_user_database));

    // Lookup the security table for existing hash.
    security_ptr security = security_dao->getRecordById(m_logon_user->iSecurityIndex);

    if(!security || security->iId == -1)
    {
        log->write<Logging::ERROR_LOG>("Error, Security Index on user record not available=", m_logon_user->sHandle, __FILE__, __LINE__);
        return false;
    }

    // Generate Encrypted password from incoming string.
    encrypt_ptr encryption(new Encrypt());

    if(!encryption)
    {
        log->write<Logging::ERROR_LOG>("Error, unable to allocate encryption (Password)=", m_logon_user->sHandle, __FILE__, __LINE__);
        return false;
    }

    // Compare case sensitive hash with generated hash string.
    std::string password = encryption->generate_password(input, security->sSaltHash);

    if(security->sPasswordHash.compare(password) == 0)
    {
        log->write<Logging::CONSOLE_LOG>("Password Successful=", m_logon_user->sHandle);
        return true;
    }

    log->write<Logging::ERROR_LOG>("Password Failure=", m_logon_user->sHandle);
    return false;
}


/**
 * @brief Validates user logon password
 * @return
 */
bool ModLogon::password(const std::string &input)
{
    std::string key = "";
    bool useHiddenOutput = true;
    std::string result = m_session_io.getInputField(input, key, Config::sPassword_length, "", useHiddenOutput);

    // ESC was hit
    if(result == "aborted")
    {
        return false;
    }
    else if(result[0] == '\n')
    {
        // If ENTER Default to Yes, or Single Y is hit
        if(key.size() == 0)
        {
            return false;
        }

        baseProcessDeliverNewLine();

        if(validate_password(key))
        {
            // Authorize and assign user to the session.
            m_session_data->m_is_session_authorized = true;
            m_session_data->m_user_record = m_logon_user;
            m_is_active = false;
        }
        else
        {
            displayPromptAndNewLine(PROMPT_INVALID_PASSWORD);
            ++m_failure_attempts;

            // If max, then exit back to matrix.
            if(m_failure_attempts >= m_config->invalid_password_attempts)
            {
                m_is_active = false;
                return false;
            }

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

    return false;
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
 * @brief Changes user logon password
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
