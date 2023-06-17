#ifndef MOD_LOGON_HPP
#define MOD_LOGON_HPP

#include "mod_base.hpp"

#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include <functional>

class Users;
typedef std::shared_ptr<Users> user_ptr;


/**
 * @class ModLogin
 * @author Michael Griffin
 * @date 3/17/2016
 * @file mod_logon.hpp
 * @brief System Login Module
 */
class ModLogon
    : public ModBase
{
public:
    ModLogon(session_ptr session_data, config_ptr config, processor_ansi_ptr ansi_process);

    virtual ~ModLogon() override
    {
        std::cout << "~ModLogon()" << std::endl;
        std::vector<std::function< void()> >().swap(m_setup_functions);
        std::vector<std::function< void(const std::string &)> >().swap(m_mod_functions);
    }

    virtual bool update(const std::string &character_buffer, const bool &) override;
    virtual bool onEnter() override;
    virtual bool onExit() override;

    // This matches the index for mod_functions.push_back
    enum
    {
        MOD_LOGON,
        MOD_PASSWORD,
        MOD_PASSWORD_QUESTION,
        MOD_PASSWORD_ANSWER,
        MOD_CHANGE_PASSWORD,
        MOD_NEW_USER
    };

    // Create Prompt Constants, these are the keys for key/value lookup
    const std::string PROMPT_LOGON = "logon";
    const std::string PROMPT_USERNUMBER = "user_number";
    const std::string PROMPT_PASSWORD = "password";
    const std::string PROMPT_PASSWORD_QUESTION = "password_question";
    const std::string PROMPT_PASSWORD_ANSWER = "password_answer";
    const std::string PROMPT_USE_INVALID = "invalid";
    const std::string PROMPT_INVALID_USERNAME = "invalid_username";
    const std::string PROMPT_INVALID_PASSWORD = "invalid_password";

    /**
     * @brief Create Default Text Prompts for module
     */
    void createTextPrompts();

    /**
     * @brief Sets an individual module index.
     * @param mod_function_index
     */
    void changeModule(int mod_function_index);


    /**
     * @brief Changes to Next module index.
     */
    void changeNextModule();

    /**
     * @brief Changes to Previous module index.
     */
    void changePreviousModule();

    /**
     * @brief Redisplay's the current module prompt.
     * @param mod_function_index
     */
    void redisplayModulePrompt();

    /**
     * @brief Pull and Display Prompts
     * @param prompt
     */
    void displayPrompt(const std::string &prompt);

    /**
     * @brief Pull and Display Prompts with following newline
     * @param prompt
     */
    void displayPromptAndNewLine(const std::string &prompt);

    /**
     * @brief Pre Logon Sequence
     * @return
     */
    void setupPreLogon();

    /**
     * @brief Validates user Logon
     * @return
     */
    void setupLogon();

    /**
     * @brief Display the UserNumber on Logon.
     */
    void displayUserNumber();

    /**
     * @brief Validates user logon password
     * @return
     */
    void setupPassword();

    /**
     * @brief Set the Password Challenge Question
     * @return
     */
    void setupPasswordQuestion();

    /**
     * @brief Set the Password Challenge Answer
     * @return
     */
    void setupPasswordAnswer();

    /**
     * @brief Changes user logon password
     * @return
     */
    void setupPasswordChange();

    /**
     * @brief Creates New User Account
     * @return
     */
    void setupNewUserApplication();

private:

    /**
     * @brief Pre Logon Sequence
     * @return
     */
    bool preLogon(const std::string &input);

    /**
     * @brief Lookup user records Handle, Name, or Email
     * @return
     */
    bool checkUserLogon(const std::string &input);

    /**
     * @brief Validates user Logon
     * @return
     */
    bool logon(const std::string &input);

    /**
     * @brief Encodes and Validates User Password
     * @param input
     * @return
     */
    bool validate_password(const std::string &input);

    /**
     * @brief Validates user logon password
     * @return
     */
    bool password(const std::string &input);

    /**
     * @brief Gets Password Challenge Question Response
     * @return
     */
    bool passwordQuestion(const std::string &input);

    /**
     * @brief Gets Password Challenge Answer Response
     * @return
     */
    bool passwordAnswer(const std::string &input);

    /**
     * @brief Changes user logon password
     * @return
     */
    bool passwordChange(const std::string &input);

    /**
     * @brief Creation of new user account
     * @return
     */
    bool newUserApplication(const std::string &input);

    // Function Input Vector.
    std::vector<std::function< void()> >                    m_setup_functions;
    std::vector<std::function< void(const std::string &)> > m_mod_functions;


    session_io_ptr         m_session_io;
    std::string            m_filename;
    text_prompts_dao_ptr   m_text_prompts_dao;

    int                    m_mod_function_index;
    int                    m_failure_attempts;
    bool                   m_is_text_prompt_exist;

    // Hold instance of user trying to login to the system.
    user_ptr               m_logon_user;
};

#endif // MOD_LOGON_HPP
