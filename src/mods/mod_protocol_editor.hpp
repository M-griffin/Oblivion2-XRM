#ifndef MOD_PROTOCOL_EDITOR_HPP
#define MOD_PROTOCOL_EDITOR_HPP

#include "mod_base.hpp"

#include <iostream>
#include <string>
#include <memory>
#include <functional>
#include <vector>

#include "model-sys/structures.hpp"

class Directory;
typedef std::shared_ptr<Directory> directory_ptr;

/**
 * @class ModProtocolEditor
 * @author Michael Griffin
 * @date 26/06/2018
 * @file mod_protocol_editor.hpp
 * @brief Protocol Editor
 */
class ModProtocolEditor
    : public ModBase
{
public:
    ModProtocolEditor(session_ptr session_data, config_ptr config, processor_ansi_ptr ansi_process,
        common_io_ptr common_io, session_io_ptr session_io);

    virtual ~ModProtocolEditor() override
    {
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

    /**
     * @brief Menu Editor Display, Runs through all existing menus
     */
    std::string displayOfMenus();


private:

    // Function Input Vector.
    std::vector<std::function< void()> >                    m_setup_functions;
    std::vector<std::function< void(const std::string &)> > m_mod_functions;

    text_prompts_dao_ptr   m_text_prompts_dao;
    directory_ptr          m_directory;

    int                    m_mod_function_index;
    int                    m_failure_attempts;
    bool                   m_is_text_prompt_exist;

    // Hold instance of user trying to login to the system.
    //user_ptr             m_logon_user;
};

#endif // MOD_PROTOCOL_EDITOR_HPP
