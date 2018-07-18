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
        , m_mod_setup_index(MOD_DISPLAY_MENU)
        , m_mod_function_index(MOD_PROMPT)
        , m_mod_menu_state_index(MENU_ADD)
        , m_failure_attempts(0)
        , m_is_text_prompt_exist(false)
        , m_page(0)
        , m_rows_per_page(0)
    {
        std::cout << "ModMenuEditor" << std::endl;

        // Setup Modules
        m_setup_functions.push_back(std::bind(&ModMenuEditor::setupMenuEditor, this));
        
        // Input or Method Modules that handle incoming input per state.
        m_mod_functions.push_back(std::bind(&ModMenuEditor::menuEditorInput, this, std::placeholders::_1));
        m_mod_functions.push_back(std::bind(&ModMenuEditor::menuEditorPausedInput, this, std::placeholders::_1));
        m_mod_functions.push_back(std::bind(&ModMenuEditor::menuEditorMenuNameInput, this, std::placeholders::_1));
                
        
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

    // Setup Module Index
    enum
    {
        MOD_DISPLAY_MENU = 0
    };
    
    // Input Module Index
    enum
    {
        MOD_PROMPT    = 0,
        MOD_PAUSE     = 1,
        MOD_MENU_NAME = 2
    };
    
    // Input Menu State Index
    enum
    {
        MENU_ADD    = 0,
        MENU_CHANGE = 1,
        MENU_DELETE = 2,
        MENU_COPY_FROM = 3,
        MENU_COPY_TO = 4
    };

    // Create Prompt Constants, these are the keys for key/value lookup
    const std::string PROMPT_HEADER = "header";
    const std::string PROMPT_PAUSE = "pause_prompt";
    const std::string PROMPT_INPUT_TEXT = "input_text";
    const std::string PROMPT_INVALID = "invalid_input";    
    const std::string PROMPT_MENU_ADD = "menu_add";
    const std::string PROMPT_MENU_DELETE = "menu_delete";
    const std::string PROMPT_MENU_CHANGE = "menu_edit";    
    const std::string PROMPT_MENU_COPY_FROM = "menu_copy_from";
    const std::string PROMPT_MENU_COPY_TO = "menu_copy_to";
    

    /**
     * @brief Create Default Text Prompts for module
     */
    void createTextPrompts();
    
    /**
     * @brief Sets an indivdual input module index.
     * @param mod_function_index
     */
    void changeInputModule(int mod_function_index);

    /**
     * @brief Sets an indivdual setup method module index.
     * @param mod_function_index
     */
    void changeSetupModule(int mod_function_index);
    
    /**
     * @brief Sets an indivdual Menu Input State Add/Change/Delete
     * @param mod_menu_state_index
     */
    void changeMenuInputState(int mod_menu_state_index);
    
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
     * @brief Pull and Display Prompts with following newline
     * @param prompt
     */
    void displayPromptAndNewLine(const std::string &prompt);

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
     * @brief Check if the menu exists in the current listing
     * @param menu_name
     * @return 
     */
    bool checkMenuExists(std::string menu_name);

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
    
    /**
     * @brief Handles Menu Name Input for Add/Change/Delete Methods calls.
     * @param input
     */
    void menuEditorMenuNameInput(const std::string &input);

private:

    // Function Input Vector.
    std::vector<std::function< void()> >                    m_setup_functions;
    std::vector<std::function< void(const std::string &)> > m_mod_functions;
    std::vector<std::string>                                m_menu_display_list;


    SessionIO              m_session_io;
    std::string            m_filename;
    text_prompts_dao_ptr   m_text_prompts_dao;

    unsigned int           m_mod_setup_index;
    unsigned int           m_mod_function_index;
    unsigned int           m_mod_menu_state_index;
    
    int                    m_failure_attempts;
    bool                   m_is_text_prompt_exist;
    unsigned int           m_page;
    unsigned int           m_rows_per_page;
    bool                   m_active;

    CommonIO               m_common_io;
    directory_ptr          m_directory;
   
};

#endif // MOD_MENU_EDITOR_HPP
