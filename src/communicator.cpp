#include "communicator.hpp"

#include "model-sys/structures.hpp"
#include "model-sys/struct_compat.hpp"

#include <iostream>
#include <string>
#include <cstring>

/**
 * @brief  Global Singleton Instance, needed to initalize the class.
 */
Communicator* Communicator::m_global_instance = nullptr;

/**
 * @brief Default Constructors required.
 * @return
 */
Communicator::Communicator()
    : m_filename("mod_global.yaml")
    , m_text_prompts_dao(new TextPromptsDao(GLOBAL_DATA_PATH, m_filename))
    , m_is_text_prompt_exist(false)
    , m_active(true)
{
    std::cout << "Communicator" << std::endl;

    // Check of the Text Prompts exist.
    m_is_text_prompt_exist = m_text_prompts_dao->fileExists();
    if(!m_is_text_prompt_exist)
    {
        createTextPrompts();
    }

    // Loads all Text Prompts for current module
    m_text_prompts_dao->readPrompts();
}

Communicator::~Communicator()
{
    std::cout << "~Communicator" << std::endl;
}
