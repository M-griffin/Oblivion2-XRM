#ifndef MOD_MENU_EDITOR_HPP
#define MOD_MENU_EDITOR_HPP

#include "mod_base.hpp"

#include <string>
#include <memory>
#include <functional>
#include <vector>

class SessionData;
typedef std::shared_ptr<SessionData> session_data_ptr;

class Directory;
typedef std::shared_ptr<Directory> directory_ptr;

/**
 * @class ModMenuEditor
 * @author Michael Griffin
 * @date 26/06/2018
 * @file mod_menu_editor.hpp
 * @brief Menu Editor
 */
class ModMenuEditor
    : public ModBase
{
public:
    ModMenuEditor(session_data_ptr session_data, config_ptr config, ansi_process_ptr ansi_process)
        : ModBase(session_data, config, ansi_process)
        , m_session_io(session_data)
        , m_filename("mod_menu_editor.yaml")
        , m_text_prompts_dao(new TextPromptsDao(GLOBAL_DATA_PATH, m_filename))
        , m_mod_function_index(MOD_PROMPT)
        , m_failure_attempts(0)
        , m_is_text_prompt_exist(false)
        , m_page(0)
        , m_rows_per_page(0)
    {
        std::cout << "ModMenuEditor" << std::endl;

        // Push function pointers to the stack.
        
        m_setup_functions.push_back(std::bind(&ModMenuEditor::setupMenuEditor, this));
        m_mod_functions.push_back(std::bind(&ModMenuEditor::menuEditorPausedInput, this, std::placeholders::_1));
        m_mod_functions.push_back(std::bind(&ModMenuEditor::menuEditorInput, this, std::placeholders::_1));
        
        
        // Check of the Text Prompts exist.
        m_is_text_prompt_exist = m_text_prompts_dao->fileExists();
        if (!m_is_text_prompt_exist)
        {
            createTextPrompts();
        }

        // Loads all Text Prompts for current module
        m_text_prompts_dao->readPrompts();
    }

    virtual ~ModMenuEditor() override
    {
        std::cout << "~ModMenuEditor" << std::endl;
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
        MOD_PAUSE,
    };

    // Create Prompt Constants, these are the keys for key/value lookup
    const std::string PROMPT_HEADER = "header";
    const std::string PROMPT_PAUSE = "pause_prompt";
    const std::string PROMPT_INPUT_TEXT = "input_text";
    const std::string PROMPT_INVALID = "invalid_input";

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
     * @brief Validates user Logon
     * @return
     */
    void setupMenuEditor();
    
    /**
     * @brief Displays the current page of menu items
     */
    void displayCurrentPage();

    /**
     * @brief Menu Editor Display, Runs through all existing menus
     */
    std::string displayMenuList();


    /**
     * Input Methods below here.
     */

    /**
     * @brief Handles Input (Waiting for Any Key Press)
     * @param character_buffer
     */
    void menuEditorPausedInput(const std::string &input);

    /**
     * @brief Handles Menu Editor Command Selection
     * @param character_buffer
     */
    void menuEditorInput(const std::string &input);

private:

    // Function Input Vector.
    std::vector<std::function< void()> >                    m_setup_functions;
    std::vector<std::function< void(const std::string &)> > m_mod_functions;
    std::vector<std::string>                                m_menu_display_list;


    SessionIO              m_session_io;
    std::string            m_filename;
    text_prompts_dao_ptr   m_text_prompts_dao;

    unsigned int           m_mod_function_index;
    int                    m_failure_attempts;
    bool                   m_is_text_prompt_exist;
    unsigned int           m_page;
    unsigned int           m_rows_per_page;

    CommonIO               m_common_io;
    directory_ptr          m_directory;

    // Hold instatnce of user trying to login to the system.
    //user_ptr             m_logon_user;
    
};

#endif // MOD_MENU_EDITOR_HPP
