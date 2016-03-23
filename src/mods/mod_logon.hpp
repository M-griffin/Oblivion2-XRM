#ifndef MOD_LOGON_HPP
#define MOD_LOGON_HPP

#include "mod_base.hpp"

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
        , m_mod_function_index(MOD_LOGON)
        , m_failure_attempts(0)
        , m_max_failure_attempts(3)
    {
        std::cout << "ModLogon" << std::endl;

        // Push function pointers to the stack.
        m_setup_functions.push_back(std::bind(&ModLogon::setupLogon, this));
        m_setup_functions.push_back(std::bind(&ModLogon::setupPassword, this));
        m_setup_functions.push_back(std::bind(&ModLogon::setupPasswordChallenge, this));
        m_setup_functions.push_back(std::bind(&ModLogon::setupPasswordChange, this));
        m_setup_functions.push_back(std::bind(&ModLogon::setupNewUserApplication, this));

        m_mod_functions.push_back(std::bind(&ModLogon::logon, this, std::placeholders::_1));
        m_mod_functions.push_back(std::bind(&ModLogon::password, this, std::placeholders::_1));
        m_mod_functions.push_back(std::bind(&ModLogon::passwordChallenge, this, std::placeholders::_1));
        m_mod_functions.push_back(std::bind(&ModLogon::passwordChange, this, std::placeholders::_1));
        m_mod_functions.push_back(std::bind(&ModLogon::newUserApplication, this, std::placeholders::_1));
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

    /**
     * @brief Sets an indivdual module index.
     * @param mod_function_index
     */
    void changeModule(int mod_function_index);

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

    SessionIO   m_session_io;

    int         m_mod_function_index;
    int         m_failure_attempts;
    int         m_max_failure_attempts;

    

};

#endif // SYSTEM_STATE_HPP
