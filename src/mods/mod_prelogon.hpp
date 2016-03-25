#ifndef MOD_PRELOGON_HPP
#define MOD_PRELOGON_HPP

#include "mod_base.hpp"

#include "../model/structures.hpp"
#include "../data/text_prompts_dao.hpp"

#include "../session_data.hpp"
#include "../session_io.hpp"

#include <vector>
#include <functional>


/**
 * @class ModPreLogin
 * @author Michael Griffin
 * @date 3/17/2016
 * @file mod_logon.hpp
 * @brief System PreLogin Module
 */
class ModPreLogon
    : public ModBase
{
public:
    ModPreLogon(session_data_ptr session_data)
        : ModBase(session_data)
        , m_session_io(session_data)
        , m_filename("mod_prelogon.yaml")
        , m_text_prompts_dao(new TextPromptsDao(GLOBAL_DATA_PATH, m_filename))
        , m_mod_function_index(MOD_DETECT_EMULATION)
        , m_is_text_prompt_exist(false)
        , m_input_buffer("")
        , m_x_position(0)
        , m_y_position(0)
    {
        std::cout << "ModPreLogon" << std::endl;

        // Push function pointers to the stack.

        m_setup_functions.push_back(std::bind(&ModPreLogon::setupEmulationDetection, this));

        m_mod_functions.push_back(std::bind(&ModPreLogon::emulationDetection, this, std::placeholders::_1));

        // Check of the Text Prompts exist.
        m_is_text_prompt_exist = m_text_prompts_dao->fileExists();
        if (!m_is_text_prompt_exist)
        {
            createTextPrompts();
        }

        // Loads all Text Prompts for current module
        m_text_prompts_dao->readPrompts();
    }

    virtual ~ModPreLogon()
    {
        std::cout << "~ModPreLogon" << std::endl;
        std::vector<std::function< void()> >().swap(m_setup_functions);
        std::vector<std::function< void(const std::string &)> >().swap(m_mod_functions);
    }

    virtual bool update(const std::string &character_buffer, const bool &);
    virtual bool onEnter();
    virtual bool onExit();

    // This matches the index for mod_functions.push_back
    enum
    {
        MOD_DETECT_EMULATION
    };

    // Create Prompt Constants, these are the keys for key/value lookup
    const std::string PROMPT_DETECT_EMULATION = "detect_emu";
    const std::string PROMPT_DETECTED_ANSI = "detected_ansi";
    const std::string PROMPT_DETECTED_NONE = "detected_none";

    const std::string PROMPT_USE_ANSI = "use_ansi";

    const std::string PROMPT_USE_CP437 = "use_cp437";
    const std::string PROMPT_USE_UTF8 = "use_utf8";
    const std::string PROMPT_CODEPAGE_CP437 = "codepage_437";
    const std::string PROMPT_CODEPAGE_UTF8 = "codepage_utf8";


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
    void setupEmulationDetection();

    

private:

    /**
     * @brief Pre Logon Sequence
     * @return
     */
    bool emulationDetection(const std::string &input);

   

    // Function Input Vector.
    std::vector<std::function< void()> >                    m_setup_functions;
    std::vector<std::function< void(const std::string &)> > m_mod_functions;



    SessionIO            m_session_io;
    std::string          m_filename;
    text_prompts_dao_ptr m_text_prompts_dao;

    int                  m_mod_function_index;   
    bool                 m_is_text_prompt_exist;
    std::string          m_input_buffer;
    int                  m_x_position;
    int                  m_y_position;
};

#endif // SYSTEM_STATE_HPP
