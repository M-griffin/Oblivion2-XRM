#include "mod_message_reader.hpp"

#include <string>
#include <vector>

#include "../data-sys/text_prompts_dao.hpp"
#include "../model-sys/config.hpp"

#include "../session.hpp"
#include "../session_io.hpp"

#include "../processor_ansi.hpp"
#include "../logging.hpp"
#include "../directory.hpp"


ModMessageReader::ModMessageReader(session_ptr session_data, config_ptr config, processor_ansi_ptr ansi_process,
        common_io_ptr common_io, session_io_ptr session_io)
    : ModBase(session_data, config, ansi_process, "mod_message_reader.yaml", common_io, session_io)
    , m_text_prompts_dao(nullptr)
    , m_directory(nullptr)
    , m_mod_function_index(MOD_PROMPT)
    , m_failure_attempts(0)
    , m_is_text_prompt_exist(false)
{
    // Setup Smart Pointers
    m_text_prompts_dao = std::make_shared<TextPromptsDao>(GLOBAL_DATA_PATH, m_filename);
    m_directory = std::make_shared<Directory>();
    
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

bool ModMessageReader::update(const std::string &, const bool &)
{
    return false;
}

bool ModMessageReader::onEnter()
{
    return false;
}

bool ModMessageReader::onExit()
{
    return false;
}
    
/**
 * @brief Create Default Text Prompts for module
 */
void ModMessageReader::createTextPrompts()
{
    return;
}

/**
 * @brief Sets an individual module index.
 * @param mod_function_index
 */
void ModMessageReader::changeModule(int)
{
    return;
}