#ifndef MOD_FILE_EDITOR_HPP
#define MOD_FILE_EDITOR_HPP

#include "mod_base.hpp"

#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include <functional>

class Directory;
typedef std::shared_ptr<Directory> directory_ptr;

/**
 * @class ModFileEditor
 * @author Michael Griffin
 * @date 26/06/2018
 * @file mod_file_editor.hpp
 * @brief File Editor
 */
class ModFileEditor
    : public ModBase
{
public:
    ModFileEditor(session_ptr session_data, config_ptr config, processor_ansi_ptr ansi_process);

    virtual ~ModFileEditor() override
    {
        std::cout << "~ModFileEditor()" << std::endl;
        std::vector<std::function< void()> >().swap(m_setup_functions);
        std::vector<std::function< void(const std::string &)> >().swap(m_mod_functions);
    }

    virtual bool update(const std::string &character_buffer, const bool &) override;
    virtual bool onEnter() override;
    virtual bool onExit() override;

    // This matches the index for mod_functions.push_back
    enum
    {
        MOD_PROMPT,
        MOD_ADD,
        MOD_CHANGE,
        MOD_DELETE
    };

    // Create Prompt Constants, these are the keys for key/value lookup
    const std::string PROMPT_HEADER = "header";
    const std::string PROMPT_INPUT_TEXT = "input_text";
    const std::string PROMPT_INVALID = "invalid_input";

    /**
     * @brief Create Default Text Prompts for module
     */
    void createTextPrompts();

    /**
     * @brief Sets an individual module index.
     * @param mod_function_index
     */
    void changeModule(int mod_function_index);

private:

    // Function Input Vector.
    std::vector<std::function< void()> >                    m_setup_functions;
    std::vector<std::function< void(const std::string &)> > m_mod_functions;
    
    std::string            m_filename;
    text_prompts_dao_ptr   m_text_prompts_dao;
    directory_ptr          m_directory;

    int                    m_mod_function_index;
    int                    m_failure_attempts;
    bool                   m_is_text_prompt_exist;

    

    // Hold instance of user trying to login to the system.
    //user_ptr             m_logon_user;
};

#endif // MOD_FILE_EDITOR_HPP
