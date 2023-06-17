#include "mod_file_lister.hpp"


#include "../data-sys/text_prompts_dao.hpp"
#include "../model-sys/config.hpp"

#include "../session.hpp"
#include "../session_io.hpp"
#include "../processor_ansi.hpp"
#include "../encoding.hpp"
#include "../logging.hpp"
#include "../directory.hpp"

ModFileLister::ModFileLister(session_ptr session_data, config_ptr config, processor_ansi_ptr ansi_process)
    : ModBase(session_data, config, ansi_process)
    , m_filename("mod_file_lister.yaml")
    , m_text_prompts_dao(new TextPromptsDao(GLOBAL_DATA_PATH, m_filename))
    , m_directory(new Directory())
    , m_mod_function_index(MOD_PROMPT)
    , m_failure_attempts(0)
    , m_is_text_prompt_exist(false)
{
    // Push function pointers to the stack.

    //m_setup_functions.push_back(std::bind(&ModMenuEditor::setupLogon, this));

    //m_mod_functions.push_back(std::bind(&ModMenuEditor::logon, this, std::placeholders::_1));


    // Check of the Text Prompts exist.
    m_is_text_prompt_exist = m_text_prompts_dao->fileExists();

    if(!m_is_text_prompt_exist)
    {
        createTextPrompts();
    }

    // Loads all Text Prompts for current module
    m_text_prompts_dao->readPrompts();
}

bool ModFileLister::update(const std::string &, const bool &)
{
    return false;
}

bool ModFileLister::onEnter()
{
    return false;
}

bool ModFileLister::onExit()
{
    return false;
}
    
/**
 * @brief Create Default Text Prompts for module
 */
void ModFileLister::createTextPrompts()
{
    return;
}

/**
 * @brief Sets an individual module index.
 * @param mod_function_index
 */
void ModFileLister::changeModule(int)
{
    return;
}