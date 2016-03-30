#ifndef MOD_SIGNUP_HPP
#define MOD_SIGNUP_HPP

#include "mod_base.hpp"

#include "../model/structures.hpp"
#include "../data/text_prompts_dao.hpp"

#include "../session_data.hpp"
#include "../session_io.hpp"

#include <vector>
#include <functional>

/**
 * @class ModSignup
 * @author Michael Griffin
 * @date 3/28/2016
 * @file mod_signup.hpp
 * @brief New User Application Module
 */
class ModSignup
    : public ModBase
{
public:
    ModSignup(session_data_ptr session_data)
        : ModBase(session_data)
        , m_session_io(session_data)
        , m_filename("mod_signup.yaml")
        , m_text_prompts_dao(new TextPromptsDao(GLOBAL_DATA_PATH, m_filename))
        , m_mod_function_index(MOD_SIGNUP)
        , m_is_text_prompt_exist(false)
    {
        std::cout << "ModSignup" << std::endl;

        // Push function pointers to the stack.
        m_setup_functions.push_back(std::bind(&ModSignup::setupSignup, this));

        m_mod_functions.push_back(std::bind(&ModSignup::signup, this, std::placeholders::_1));

        // Check of the Text Prompts exist.
        m_is_text_prompt_exist = m_text_prompts_dao->fileExists();
        if (!m_is_text_prompt_exist)
        {
            createTextPrompts();
        }

        // Loads all Text Prompts for current module
        m_text_prompts_dao->readPrompts();
    }

    virtual ~ModSignup()
    {
        std::cout << "~ModSignup" << std::endl;
        std::vector<std::function< void()> >().swap(m_setup_functions);
        std::vector<std::function< void(const std::string &)> >().swap(m_mod_functions);
    }

    virtual bool update(const std::string &character_buffer, const bool &);
    virtual bool onEnter();
    virtual bool onExit();

    // This matches the index for mod_functions.push_back
    enum
    {
        MOD_SIGNUP
    };

    // Create Prompt Constants, these are the keys for key/value lookup
    const std::string PROMPT_HANDLE = "ask_handle";

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
    void setupSignup();

private:

    /**
     * @brief Pre Logon Sequence
     * @return
     */
    bool signup(const std::string &input);

    // Function Input Vector.
    std::vector<std::function< void()> >                    m_setup_functions;
    std::vector<std::function< void(const std::string &)> > m_mod_functions;

    SessionIO            m_session_io;
    std::string          m_filename;
    text_prompts_dao_ptr m_text_prompts_dao;

    int                  m_mod_function_index;
    bool                 m_is_text_prompt_exist;
};

#endif // MOD_SIGNUP_HPP
