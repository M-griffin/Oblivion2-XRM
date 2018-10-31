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
        LEVEL_ADD       = 0,
        LEVEL_CHANGE    = 1,
        LEVEL_DELETE    = 2,
        LEVEL_COPY_FROM = 3,
        LEVEL_COPY_TO   = 4
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
    const std::string PROMPT_LEVEL_COPY_FROM = "level_copy_from";
    const std::string PROMPT_LEVEL_COPY_TO = "level_copy_to";
    const std::string PROMPT_INVALID_LEVEL_EXISTS = "invalid_level_exists";
    const std::string PROMPT_INVALID_LEVEL_NOT_EXISTS = "invalid_level_doesnt_exist";
    const std::string PROMPT_LEVEL_FIELD_INPUT_TEXT = "level_field_input_text";

    // Level Field Edit Prompts
    const std::string PROMPT_LEVEL_FIELD_NAME = "prompt_level_field_name";
    const std::string PROMPT_LEVEL_FIELD_START_MENU = "prompt_level_field_start_menu";
    const std::string PROMPT_LEVEL_FIELD_FILE_LEVEL = "prompt_level_field_file_level";
    const std::string PROMPT_LEVEL_FIELD_MESG_LEVEL = "prompt_level_field_mesg_level";
    const std::string PROMPT_LEVEL_FIELD_POST_CALL_RATIO = "prompt_level_field_pc_ratio";
    const std::string PROMPT_LEVEL_FIELD_FILE_RATIO = "prompt_level_file_ratio";
    const std::string PROMPT_LEVEL_FIELD_TIME_LIMIT = "prompt_level_field_time_limit";
    const std::string PROMPT_LEVEL_FIELD_CALL_LIMIT = "prompt_level_field_call_limit";
    const std::string PROMPT_LEVEL_FIELD_DOWNLOADS = "prompt_level_field_downloads";
    const std::string PROMPT_LEVEL_FIELD_DOWNLOAD_MB = "prompt_level_field_downloads_mb";
    const std::string PROMPT_LEVEL_FIELD_ARFLAGS1 = "prompt_level_fields_arflags1";
    const std::string PROMPT_LEVEL_FIELD_ARFLAGS2 = "prompt_level_fields_arflags2";
    const std::string PROMPT_LEVEL_BOOL_POST_CALL_RATIO = "prompt_level_bool_pc_ratio";
    const std::string PROMPT_LEVEL_BOOL_FILE_RATIO = "prompt_level_bool_file_ratio";
    const std::string PROMPT_LEVEL_BOOL_TIME_LIMIT = "prompt_level_bool_time_limit";
    const std::string PROMPT_LEVEL_BOOL_CALL_LIMIT = "prompt_level_bool_call_limit";
    const std::string PROMPT_LEVEL_BOOL_DOWNLOADS = "prompt_level_bool_downloads";
    const std::string PROMPT_LEVEL_BOOL_DOWNLOAD_MB = "prompt_level_bool_download_mb";

    // Level Field Display for screen
    const std::string DISPLAY_LEVEL_FIELDS_BORDER_ROW_COLOR = "display_level_field_row_color";

    const std::string DISPLAY_LEVEL_FIELD_NAME = "display_level_field_name";
    const std::string DISPLAY_LEVEL_FIELD_START_MENU = "display_level_field_start_menu";
    const std::string DISPLAY_LEVEL_FIELD_FILE_LEVEL = "display_level_field_file_level";
    const std::string DISPLAY_LEVEL_FIELD_MESG_LEVEL = "display_level_field_mesg_level";
    const std::string DISPLAY_LEVEL_FIELD_POST_CALL_RATIO = "display_level_field_pc_ratio";
    const std::string DISPLAY_LEVEL_FIELD_FILE_RATIO = "display_level_file_ratio";
    const std::string DISPLAY_LEVEL_FIELD_TIME_LIMIT = "display_level_field_time_limit";
    const std::string DISPLAY_LEVEL_FIELD_CALL_LIMIT = "display_level_field_call_limit";
    const std::string DISPLAY_LEVEL_FIELD_DOWNLOADS = "display_level_field_downloads";
    const std::string DISPLAY_LEVEL_FIELD_DOWNLOAD_MB = "display_level_field_downloads_mb";
    const std::string DISPLAY_LEVEL_FIELD_ARFLAGS1 = "display_level_fields_arflags1";
    const std::string DISPLAY_LEVEL_FIELD_ARFLAGS2 = "display_level_fields_arflags2";
    const std::string DISPLAY_LEVEL_BOOL_POST_CALL_RATIO = "display_level_bool_pc_ratio";
    const std::string DISPLAY_LEVEL_BOOL_FILE_RATIO = "display_level_bool_file_ratio";
    const std::string DISPLAY_LEVEL_BOOL_TIME_LIMIT = "display_level_bool_time_limit";
    const std::string DISPLAY_LEVEL_BOOL_CALL_LIMIT = "display_level_bool_call_limit";
    const std::string DISPLAY_LEVEL_BOOL_DOWNLOADS = "display_level_bool_downloads";
    const std::string DISPLAY_LEVEL_BOOL_DOWNLOAD_MB = "display_level_bool_download_mb";

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
     * @brief Retrieve Current Level
     */
    access_level_ptr getCurrentLevel();

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
