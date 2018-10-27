#ifndef MOD_LEVEL_EDITOR_HPP
#define MOD_LEVEL_EDITOR_HPP

#include "mod_base.hpp"

#include <functional>
#include <memory>
#include <string>
#include <vector>

class AccessLevel;
typedef std::shared_ptr<AccessLevel> access_level_ptr;

class SessionData;
typedef std::shared_ptr<SessionData> session_data_ptr;

/**
 * @class ModLevelEditor
 * @author Michael Griffin
 * @date 15/08/2018
 * @file mod_level_editor.hpp
 * @brief Level Editor
 */
class ModLevelEditor : public ModBase
{
public:
    ModLevelEditor(session_data_ptr session_data, config_ptr config, ansi_process_ptr ansi_process)
        : ModBase(session_data, config, ansi_process)
        , m_session_io(session_data)
        , m_filename("mod_level_editor.yaml")
        , m_text_prompts_dao(new TextPromptsDao(GLOBAL_DATA_PATH, m_filename))
        , m_mod_setup_index(MOD_DISPLAY_LEVEL)
        , m_mod_function_index(MOD_LEVEL_INPUT)
        , m_mod_level_state_index(LEVEL_ADD)
        , m_is_text_prompt_exist(false)
        , m_page(0)
        , m_rows_per_page(0)
        , m_current_level(0)
        , m_current_field(0)
    {
        std::cout << "ModLevelEditor" << std::endl;

        // Setup Modules
        m_setup_functions.push_back(std::bind(&ModLevelEditor::setupLevelEditor, this));
        m_setup_functions.push_back(std::bind(&ModLevelEditor::setupLevelEditFields, this));

        // Input or Method Modules that handle incoming input per state.
        m_mod_functions.push_back(std::bind(&ModLevelEditor::levelEditorInput, this, std::placeholders::_1));
        m_mod_functions.push_back(std::bind(&ModLevelEditor::levelEditorPausedInput, this, std::placeholders::_1));
        m_mod_functions.push_back(std::bind(&ModLevelEditor::levelEditorLevelInput, this, std::placeholders::_1));

        // Menu Field Input Commands
        m_mod_functions.push_back(std::bind(&ModLevelEditor::levelEditorLevelFieldInput, this, std::placeholders::_1));
        m_mod_functions.push_back(std::bind(&ModLevelEditor::levelEditorLevelFieldHandler, this, std::placeholders::_1));

        // Check of the Text Prompts exist.
        m_is_text_prompt_exist = m_text_prompts_dao->fileExists();

        if(!m_is_text_prompt_exist)
        {
            createTextPrompts();
        }

        // Loads all Text Prompts for current module
        m_text_prompts_dao->readPrompts();
    }

    virtual ~ModLevelEditor() override
    {
        std::cout << "~ModLevelEditor" << std::endl;
        std::vector<std::function<void()>>().swap(m_setup_functions);
        std::vector<std::function<void(const std::string&)>>().swap(m_mod_functions);
        std::vector<access_level_ptr>().swap(m_loaded_levels);
    }

    virtual bool update(const std::string& character_buffer, const bool&) override;
    virtual bool onEnter() override;
    virtual bool onExit() override;

    // Setup Module Index
    enum
    {
        MOD_DISPLAY_LEVEL = 0,
        MOD_DISPLAY_LEVEL_EDIT = 1
    };

    // Input Module Index
    enum
    {
        MOD_LEVEL_INPUT       = 0, // Level Parser
        MOD_PAUSE             = 1, // Pauses on display of menus/options
        MOD_LEVEL_NAME        = 2, // Level Name Handler
        MOD_LEVEL_FIELD_INPUT = 3, // Level Field Parser
        MOD_LEVEL_FIELD       = 4  // Level Field Handler
    };

    // Input Level State Index
    enum
    {
        LEVEL_ADD    = 0,
        LEVEL_CHANGE = 1,
        LEVEL_DELETE = 2
    };

    // Box drawing characters
    enum
    {
        BORDER_TOP_LEFT  = (char)214, // ╓
        BORDER_BOT_LEFT  = (char)211, // ╙
        BORDER_ROW       = (char)196, // ─
        BORDER_TOP_RIGHT = (char)183, // ╖
        BORDER_BOT_RIGHT = (char)189, // ╜
        BORDER_MID_TOP   = (char)210, // ╥
        BORDER_MID_BOT   = (char)208, // ╨
        BORDER_MID       = (char)186  // ║
    };

    // Create Prompt Constants, these are the keys for key/value lookup
    const std::string PROMPT_HEADER = "level_header";
    const std::string PROMPT_LEVEL_EDIT_HEADER = "level_field_edit_header";
    const std::string PROMPT_PAUSE = "pause_prompt";
    const std::string PROMPT_INPUT_TEXT = "level_input_text";
    const std::string PROMPT_OPTION_INPUT_TEXT = "option_input_text";
    const std::string PROMPT_INVALID = "invalid_input";

    const std::string PROMPT_LEVEL_ADD = "level_add";
    const std::string PROMPT_LEVEL_DELETE = "level_delete";
    const std::string PROMPT_LEVEL_CHANGE = "level_edit";
    const std::string PROMPT_INVALID_LEVEL_EXISTS = "invalid_level_exists";
    const std::string PROMPT_INVALID_LEVEL_NOT_EXISTS = "invalid_level_doesnt_exist";

    // Level Field Edit Prompts
    const std::string PROMPT_LEVEL_FIELD_INPUT_TEXT = "level_field_input_text";
    const std::string PROMPT_LEVEL_FIELD_TITLE = "level_field_title";
    const std::string PROMPT_LEVEL_FIELD_PASSWORD = "level_field_password";
    const std::string PROMPT_LEVEL_FIELD_FALLBACK = "level_field_fallback";
    const std::string PROMPT_LEVEL_FIELD_HELP_ID = "level_field_help_id";
    const std::string PROMPT_LEVEL_FIELD_NAME = "level_field_name";
    const std::string PROMPT_LEVEL_FIELD_PULLDOWN = "level_field_pulldown";

    // Level Field Display for screen
    const std::string DISPLAY_LEVEL_FIELDS_VERSION_ID = "display_level_field_version_id";
    const std::string DISPLAY_LEVEL_FIELDS_BORDER_ROW_COLOR = "display_level_field_row_color";
    const std::string DISPLAY_LEVEL_FIELDS_TITLE = "display_level_field_title";
    const std::string DISPLAY_LEVEL_FIELDS_PASSWORD = "display_level_field_passoword";
    const std::string DISPLAY_LEVEL_FIELDS_FALLBACK = "display_level_field_fallback";
    const std::string DISPLAY_LEVEL_FIELDS_HELP_ID = "display_level_field_help_id";
    const std::string DISPLAY_LEVEL_FIELDS_NAME = "display_level_field_name";
    const std::string DISPLAY_LEVEL_FIELDS_PULLDOWN_FILE = "display_level_field_pulldown_file";
    const std::string DISPLAY_LEVEL_FIELDS_VIEW_GENERIC = "display_level_field_view_generic";
    const std::string DISPLAY_LEVEL_FIELDS_EDIT_OPTIONS = "display_level_field_edit_options";
    const std::string DISPLAY_LEVEL_FIELDS_QUIT_SAVE = "display_level_field_save";
    const std::string DISPLAY_LEVEL_FIELDS_QUIT_ABORT = "display_level_field_abort";

    /**
     * @brief Create Default Text Prompts for module
     * @return
     */
    void createTextPrompts();

    /**
     * @brief Sets an individual input module index.
     * @param mod_function_index
     */
    void changeInputModule(int mod_function_index);

    /**
     * @brief Sets an individual setup method module index.
     * @param mod_function_index
     */
    void changeSetupModule(int mod_function_index);

    /**
     * @brief Sets an individual Menu Input State Add/Change/Delete
     * @param mod_menu_state_index
     */
    void changeLevelInputState(int mod_menu_state_index);

    /**
     * @brief Redisplay's the current module prompt.
     * @return
     */
    void redisplayModulePrompt();

    /**
     * @brief Pull and Display Prompts
     * @param prompt
     */
    void displayPrompt(const std::string& prompt);

    /**
     * @brief Pull and parse and return Display Prompts for use in interfaces
     * @param prompt
     */
    std::string getDisplayPrompt(const std::string& prompt);

    /**
     * @brief Pull and parse and return Display Prompts for use in interfaces
     * @param prompt
     */
    std::string getDisplayPromptRaw(const std::string& prompt);

    /**
     * @brief Pull and Display Prompts with MCI Code
     * @param prompt
     * @param mci_field
     */
    void displayPromptMCI(const std::string& prompt, const std::string& mci_field);

    /**
     * @brief Pull and Display Prompts with following newline
     * @param prompt
     */
    void displayPromptAndNewLine(const std::string& prompt);

    /**
     * @brief Validates user Logon
     * @return
     */
    void setupLevelEditor();

    /**
     * @brief Setup for the Menu Editor
     * @return
     */
    void setupLevelEditFields();

    /**
     * @brief Displays the current page of menu items
     * @param input_state
     */
    void displayCurrentPage(const std::string& input_state);

    /**
     * @brief Displays the current page of menu items
     * @param input_state
     */
    void displayCurrentEditPage(const std::string& input_state);

    /**
     * @brief Check if the level exists in the current listing
     * @param level_code
     * @return
     */
    bool checkLevelExistsByLevel(int level_code);

    /**
     * @brief Menu Editor Display, Runs through all existing menus
     * @return
     */
    std::string displayLevelList();

    /**
     * @brief Menu Editor, for Displaying Menu Fields to Edit
     * @return
     */
    std::string displayLevelEditScreen();

    /**
     * Input Methods below here.
     */

    /**
     * @brief Handles Input (Waiting for Any Key Press)
     * @param input
     */
    void levelEditorPausedInput(const std::string& input);

    /**
     * @brief Handles Input (Waiting for Any Key Press) View Generic Menu
     * @param input
     */
    void levelEditorDisplayPause(const std::string& input);

    /**
     * @brief Handles Menu Editor Command Selection
     * @param input
     */
    void levelEditorInput(const std::string& input);

    /**
     * @brief Handles Menu Field Editor Command Selection
     * @param input
     */
    void levelEditorLevelFieldInput(const std::string& input);

    /**
     * @brief Handles Field Updates for Menu Data
     * @param input
     */
    void levelEditorLevelFieldHandler(const std::string& input);

    /**
     * @brief Handles Level Input, Parses Strings and checks Valid Levels
     * @param input
     */
    void levelEditorLevelInput(const std::string& input);

    /**
     * @brief handle each level separate state and what to do next on input.
     * @param does_menu_exist
     * @param level_code
     */
    void handleLevelInputState(bool does_level_exist, int level_code);

    /**
     * @brief Create a new empty Level
     * @param level_code
     */
    void createNewLevel(int level_code);

    /**
     * @brief Delete an existing Level
     * @param level_code
     */
    void deleteExistingLevel(int level_code);

    /**
     * @brief Create a new empty Menu
     * @param level_code
     */
    void copyExistingLevel(int level_code);

    /**
     * @brief Save Menu Changes
     * @return
     */
    void saveLevelChanges();

private:
    // Function Input Vector.
    std::vector<std::function<void()>>                   m_setup_functions;
    std::vector<std::function<void(const std::string&)>> m_mod_functions;
    std::vector<std::string>                             m_level_display_list;
    std::vector<access_level_ptr>                        m_loaded_levels;

    SessionIO            m_session_io;
    std::string          m_filename;
    text_prompts_dao_ptr m_text_prompts_dao;

    unsigned int         m_mod_setup_index;
    unsigned int         m_mod_function_index;
    unsigned int         m_mod_level_state_index;

    bool                 m_is_text_prompt_exist;
    unsigned int         m_page;
    unsigned int         m_rows_per_page;
    int                  m_current_level;
    unsigned int         m_current_field;

    CommonIO             m_common_io;
};

#endif // MOD_LEVEL_EDITOR_HPP
