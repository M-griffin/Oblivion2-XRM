#include "mod_logon.hpp"
#include "../model/config.hpp"
#include "../encryption.hpp"

#include "../model/security.hpp"
#include "../model/users.hpp"
#include "../data/security_dao.hpp"
#include "../data/users_dao.hpp"

#include <iostream>
#include <string>
#include <sstream>

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

    value[PROMPT_LOGON]               = std::make_pair("Logon Prompt", "|CR|08Lo|07gon|08: |04");
    value[PROMPT_PASSWORD]            = std::make_pair("Password Prompt", "|CR|08pa|07ssword|08: |04");
    value[PROMPT_USE_INVALID]         = std::make_pair("Invalid Entry", "|CR|12Invalid Response! Try again.");
    value[PROMPT_INVALID_USERNAME]    = std::make_pair("Invalid Username", "|CR|12Invalid Username! Try again.");
    value[PROMPT_INVALID_PASSWORD]    = std::make_pair("Invalid Passowrd", "|CR|12Invalid Passowrd! Try again.");

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
 * @brief Pull and Display Prompts
 * @param prompt
 */
void ModLogon::displayPrompt(const std::string &prompt)
{
    std::string result = m_session_io.parseTextPrompt(
                                 m_text_prompts_dao->getPrompt(prompt)
                             );

    baseProcessAndDeliver(result);
}

/**
 * @brief Validates user Logon
 * @return
 */
void ModLogon::setupLogon()
{
    std::cout << "setupLogon()" << std::endl;
    displayPrompt(PROMPT_LOGON);
}


/**
 * @brief Validates user logon password
 * @return
 */
void ModLogon::setupPassword()
{
    std::cout << "setupPassword()" << std::endl;
    displayPrompt(PROMPT_PASSWORD);
}

/**
 * @brief Validates user logon password challenge questions
 * @return
 */
void ModLogon::setupPasswordQuestion()
{
    std::cout << "setupPasswordQuestion()" << std::endl;
    displayPrompt(PROMPT_PASSWORD_QUESTION);
}

/**
 * @brief Validates user logon password challenge questions
 * @return
 */
void ModLogon::setupPasswordAnswer()
{
    std::cout << "setupPasswordAnswer()" << std::endl;
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
    users_dao_ptr user_data(new UsersDao(m_session_data->m_user_database));
            
    // Check if a Digit, if so, lookup by userId.
    if (m_session_data->m_common_io.isDigit(input))
    {
        long userId = 0;
        std::stringstream ss(input);
        ss >> userId;
        
        if (!ss.fail())
        {
            m_logon_user = user_data->getUserById(userId);
            if(!m_logon_user || m_logon_user->iId != -1) 
            {
                return true;
            }
        }
    }
                        
    // Pass through all (3) checks if they are all enabled.
    if (m_config->use_handle)
    {
        m_logon_user = user_data->getUserByHandle(input);
        if(!m_logon_user || m_logon_user->iId != -1) 
        {
            return true;
        }
    }

    if (m_config->use_email)  
    {
        m_logon_user = user_data->getUserByEmail(input);
        if(!m_logon_user || m_logon_user->iId != -1) 
        {
            return true;
        }
    }

    if (m_config->use_real_name)  
    {
        m_logon_user = user_data->getUserByRealName(input);
        if(!m_logon_user || m_logon_user->iId != -1) 
        {
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
        // Key == 0 on [ENTER] pressed alone. then invalid!
        if(key.size() == 0)
        {
            // Return and don't do anything.
            return false;
        }
                
        // Check if users enter valid identifier.
        if (checkUserLogon(key))
        {            
            // Match Found, ask for password
            std::cout << "match found" << std::endl;
            
            changeModule(m_mod_function_index + 1);                        
        }
        else
        {
            // Invalid Entry, try again!            
            std::cout << "no match found" << std::endl;
            displayPrompt(PROMPT_INVALID_USERNAME);        
            
            changeModule(m_mod_function_index);            
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
 * @brief Encodes and Validates User Password
 * @param input
 * @return 
 */
bool ModLogon::validate_password(const std::string &input)
{   
    // No empty strings passed.
    if (input.size() == 0) 
    {
        return false;
    }
    
    // First load the secure record for the exsting user.
    // Link to security dao for data access object
    security_dao_ptr security_dao(new SecurityDao(m_session_data->m_user_database));
    
    // Lookup the secutiry table for existing hash.
    security_ptr security = security_dao->getSecurityById(m_logon_user->iSecurityIndex);
    if (!security || security->iId == -1)
    {
        return false;
    }
    
    // Generate Encrypted password from incoming string.
    encrypt_ptr encryption(new Encrypt());
    if (!encryption) 
    {
        std::cout << "Error, unable to allocate encryption" << std::endl;
        return false;
    }
    
    std::string password = encryption->generate_password(input, security->sSaltHash);
    
    // Compare case sensitive hash with generated hash string.
    if(security->sPasswordHash.compare(password) == 0)
    {
        // Success!
        return true;
    }
    
    return false;
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
        if(key.size() == 0)
        {
            return false;
        }         
        
        if (validate_password(key))
        {
            // If success, set authorized true, and return.
            // or m_session_data->m_is_session_authorized = true;
            m_is_authorized = true;
            m_is_active = false;
        }
        else
        {
            // Invalid Entry, try again!            
            std::cout << "no match found" << std::endl;
            displayPrompt(PROMPT_INVALID_PASSWORD);        
            
            changeModule(m_mod_function_index);
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
