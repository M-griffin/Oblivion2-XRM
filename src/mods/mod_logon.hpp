#ifndef MOD_LOGON_HPP
#define MOD_LOGON_HPP

#include "mod_base.hpp"

#include "../model/structures.hpp"
#include "../data/text_prompts_dao.hpp"

#include "../session_data.hpp"
#include "../session_io.hpp"

#include <vector>
#include <functional>


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
    ModLogon(session_data_ptr session_data)
        : ModBase(session_data)
        , m_session_io(session_data)
        , m_filename("mod_logon.yaml")
        , m_text_prompts_dao(new TextPromptsDao(GLOBAL_DATA_PATH, m_filename))
        , m_mod_function_index(MOD_LOGON)
        , m_failure_attempts(0)
        , m_max_failure_attempts(3)
        , m_is_text_prompt_exist(false)
    {
        std::cout << "ModLogon" << std::endl;

        // Push function pointers to the stack.

        m_setup_functions.push_back(std::bind(&ModLogon::setupLogon, this));
        m_setup_functions.push_back(std::bind(&ModLogon::setupPassword, this));
        m_setup_functions.push_back(std::bind(&ModLogon::setupPasswordChallenge, this));
        m_setup_functions.push_back(std::bind(&ModLogon::setupPasswordChange, this));

        m_mod_functions.push_back(std::bind(&ModLogon::logon, this, std::placeholders::_1));
        m_mod_functions.push_back(std::bind(&ModLogon::password, this, std::placeholders::_1));
        m_mod_functions.push_back(std::bind(&ModLogon::passwordChallenge, this, std::placeholders::_1));
        m_mod_functions.push_back(std::bind(&ModLogon::passwordChange, this, std::placeholders::_1));

        // Check of the Text Prompts exist.
        m_is_text_prompt_exist = m_text_prompts_dao->fileExists();
        if (!m_is_text_prompt_exist)
        {
            createTextPrompts();
        }

        // Loads all Text Prompts for current module
        m_text_prompts_dao->readPrompts();
    }

    virtual ~ModLogon()
    {
        std::cout << "~ModLogon" << std::endl;
        std::vector<std::function< void()> >().swap(m_setup_functions);
        std::vector<std::function< void(const std::string &)> >().swap(m_mod_functions);
    }

    virtual bool update(const std::string &character_buffer, const bool &);
    virtual bool onEnter();
    virtual bool onExit();

    // This matches the index for mod_functions.push_back
    enum
    {
        MOD_LOGON,
        MOD_PASSWORD,
        MOD_FORGOT_PASSWORD,
        MOD_CHANGE_PASSWORD,
        MOD_NEW_USER
    };

    // Create Prompt Constants, these are the keys for key/value lookup
    const std::string PROMPT_LOGON = "logon";
    const std::string PROMPT_PASSWORD = "password";

    /**
     * @brief Create Default Text Prompts for module
     */
    void createTextPrompts();

    /**
     * @brief Sets an indivdual module index.
     * @param mod_function_index
     */
    void changeModule(int mod_function_index);

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
     * @brief Validates user logon password
     * @return
     */
    void setupPassword();

    /**
     * @brief Validates user logon password challenge questions
     * @return
     */
    void setupPasswordChallenge();

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
     * @brief Validates user Logon
     * @return
     */
    bool logon(const std::string &input);

    /**
     * @brief Validates user logon password
     * @return
     */
    bool password(const std::string &input);

     /**
     * @brief Validates user logon password challenge questions
     * @return
     */
    bool passwordChallenge(const std::string &input);

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



    SessionIO            m_session_io;
    std::string          m_filename;
    text_prompts_dao_ptr m_text_prompts_dao;

    int                  m_mod_function_index;
    int                  m_failure_attempts;
    int                  m_max_failure_attempts;
    bool                 m_is_text_prompt_exist;
};

#endif // SYSTEM_STATE_HPP
