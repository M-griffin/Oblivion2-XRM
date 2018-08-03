#ifndef MOD_USER_EDITOR_HPP
#define MOD_USER_EDITOR_HPP

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
 * @class ModUserEditor
 * @author Michael Griffin
 * @date 26/06/2018
 * @file mod_user_editor.hpp
 * @brief User Editor
 */
class ModUserEditor
    : public ModBase
{
public:
    ModUserEditor(session_data_ptr session_data, config_ptr config, ansi_process_ptr ansi_process)
        : ModBase(session_data, config, ansi_process)
        , m_session_io(session_data)
        , m_filename("mod_user_editor.yaml")
        , m_text_prompts_dao(new TextPromptsDao(GLOBAL_DATA_PATH, m_filename))
        , m_mod_setup_index(MOD_DISPLAY_USER_LIST)
        , m_mod_function_index(MOD_USER_INPUT)
        , m_mod_user_state_index(USER_CHANGE)
        , m_is_text_prompt_exist(false)
        , m_page(0)
        , m_rows_per_page(0)
        , m_current_user_id(0)
        , m_current_field(0)
        , m_wildcard_filter("")
        , m_user_array_position(0)
    {
        std::cout << "ModUserEditor" << std::endl;

        // Push function pointers to the stack.
        
        m_setup_functions.push_back(std::bind(&ModUserEditor::setupUserList, this));
        m_setup_functions.push_back(std::bind(&ModUserEditor::setupUserEditFields, this));
        m_setup_functions.push_back(std::bind(&ModUserEditor::setupUserEditExtendedFields, this));

        m_mod_functions.push_back(std::bind(&ModUserEditor::userListInput, this, std::placeholders::_1));
        m_mod_functions.push_back(std::bind(&ModUserEditor::userEditorPausedInput, this, std::placeholders::_1));
        m_mod_functions.push_back(std::bind(&ModUserEditor::userEditorUserInput, this, std::placeholders::_1));
        
        m_mod_functions.push_back(std::bind(&ModUserEditor::userEditorFieldInput, this, std::placeholders::_1));
        m_mod_functions.push_back(std::bind(&ModUserEditor::userEditorFieldHandler, this, std::placeholders::_1));
        
        m_mod_functions.push_back(std::bind(&ModUserEditor::userEditorExtendedInput, this, std::placeholders::_1));
        m_mod_functions.push_back(std::bind(&ModUserEditor::userEditorExtendedFieldHandler, this, std::placeholders::_1));
        
            
        // Check of the Text Prompts exist.
        m_is_text_prompt_exist = m_text_prompts_dao->fileExists();
        if (!m_is_text_prompt_exist)
        {
            createTextPrompts();
        }

        // Loads all Text Prompts for current module
        m_text_prompts_dao->readPrompts();
    }

    virtual ~ModUserEditor() override
    {
        std::cout << "~ModUserEditor" << std::endl;
        std::vector<std::function< void()> >().swap(m_setup_functions);
        std::vector<std::function< void(const std::string &)> >().swap(m_mod_functions);
        std::vector<user_ptr>().swap(m_users_listing);
        std::vector<user_ptr>().swap(m_loaded_user);        
    }

    virtual bool update(const std::string &character_buffer, const bool &) override;
    virtual bool onEnter() override;
    virtual bool onExit() override;

    // Setup Methods
    enum 
    {
        MOD_DISPLAY_USER_LIST   = 0,
        MOD_DISPLAY_USER_FIELDS = 1,
        MOD_DISPLAY_USER_EXTENDED_FIELDS = 2
    };

    // Input Module Index
    enum
    {
        MOD_USER_INPUT                = 0, // User List Input Parser
        MOD_PAUSE                     = 1, // Pauses on display of menus/options
        MOD_USER_NAME                 = 2, // User Name Input Handler        
        MOD_USER_FIELD_INPUT          = 3, // Selecting Individual User Fields.
        MOD_USER_FIELD                = 4, // Updates Current Field
        MOD_USER_EXTENDED_FIELD_INPUT = 5, // Selecting Individual Extended User Fields.
        MOD_USER_EXTENDED_FIELD       = 6  // Updates Current Extended Field
    };

    // Input Menu State Index
    // Used for both Menus and Options.
    enum
    {
        USER_CHANGE    = 0,
        USER_DELETE    = 1,
        USER_COPY      = 2,
        USER_FILTER    = 3
    };
    
    // Box drawing characters
    enum
    {
        BORDER_TOP_LEFT  = (char)214,  // ╓
        BORDER_BOT_LEFT  = (char)211,  // ╙
        BORDER_ROW       = (char)196,  // ─
        BORDER_TOP_RIGHT = (char)183,  // ╖
        BORDER_BOT_RIGHT = (char)189,  // ╜
        BORDER_MID_TOP   = (char)210,  // ╥
        BORDER_MID_BOT   = (char)208,  // ╨
        BORDER_MID       = (char)186   // ║
    };
    
    // Create Prompt Constants, these are the keys for key/value lookup
    const std::string PROMPT_HEADER = "header";
    const std::string PROMPT_INPUT_TEXT = "input_text";
    const std::string PROMPT_INVALID = "invalid_input";
    const std::string PROMPT_PAUSE = "pause_prompt";

    const std::string PROMPT_USER_CHANGE = "user_change";
    const std::string PROMPT_USER_DELETE = "user_delete";    
    const std::string PROMPT_USER_COPY = "user_copy";
    const std::string PROMPT_USER_FILTER = "user_filter";

    const std::string PROMPT_INVALID_USER_NOT_EXISTS = "invalid_user_doesnt_exist";
    const std::string PROMPT_USER_EDIT_HEADER = "user_editor_field_header";
    const std::string PROMPT_USER_FIELD_INPUT_TEXT = "user_editor_field_input";
    
    const std::string PROMPT_USER_FIELD_USERNAME = "user_field_username";
    const std::string PROMPT_USER_FIELD_USERLEVEL = "user_field_userlevel";
    const std::string PROMPT_USER_FIELD_REALNAME = "user_field_realname";
    const std::string PROMPT_USER_FIELD_FILELEVEL = "user_field_filelevel";
    const std::string PROMPT_USER_FIELD_EMAIL = "user_field_email";
    const std::string PROMPT_USER_FIELD_MESGLEVEL = "user_field_messagelevel";
    const std::string PROMPT_USER_FIELD_ADDRESS = "user_field_address";
    const std::string PROMPT_USER_FIELD_HACKATTEMPT = "user_field_user_hack_attempts";
    const std::string PROMPT_USER_FIELD_LOCATION = "user_field_user_location";
    const std::string PROMPT_USER_FIELD_NOTIMELIMIT = "user_field_user_no_timelimit";
    const std::string PROMPT_USER_FIELD_COUNTRY = "user_field_user_country";
    const std::string PROMPT_USER_FIELD_USEANSI = "user_field_user_use_ansi";
    const std::string PROMPT_USER_FIELD_USERNOTE = "user_field_user_usernote";
    const std::string PROMPT_USER_FIELD_BACKSPACE = "user_field_user_backspace";
    const std::string PROMPT_USER_FIELD_BIRTHDATE = "user_field_user_birthdate";
    const std::string PROMPT_USER_FIELD_WANTED = "user_field_user_wanted";
    const std::string PROMPT_USER_FIELD_FLAGS1 = "user_field_user_flags1";
    const std::string PROMPT_USER_FIELD_CLEARSCREEN = "user_field_user_clearscreen";
    const std::string PROMPT_USER_FIELD_FLAGS2 = "user_field_user_flags2";
    const std::string PROMPT_USER_FIELD_SCREENPAUSE = "user_field_user_screenpause";

    const std::string PROMPT_USER_EDIT_EXTENDED_HEADER = "user_field_extended_header";
    const std::string PROMPT_USER_EXTENDED_FIELD_INPUT_TEXT = "user_editor_extended_field_input";

    // Prompts when dispalying the user edit screen
    // Fields Being Displayed on user edit screen
    const std::string DISPLAY_USER_FIELDS_USERNAME = "display_user_field_username";
    const std::string DISPLAY_USER_FIELDS_LEVEL = "display_user_field_level";
    const std::string DISPLAY_USER_FIELDS_REALNAME = "display_user_field_realname";
    const std::string DISPLAY_USER_FIELDS_FILELEVEL = "display_user_field_filelevel";
    const std::string DISPLAY_USER_FIELDS_EMAIL = "display_user_field_email";
    const std::string DISPLAY_USER_FIELDS_MESGLEVEL = "display_user_field_messagelevel";
    const std::string DISPLAY_USER_FIELDS_ADDRESS = "display_user_field_address";
    const std::string DISPLAY_USER_FIELDS_HACK_ATTEMPT = "display_user_field_hack_attempts";
    const std::string DISPLAY_USER_FIELDS_LOCATION = "display_user_field_location";
    const std::string DISPLAY_USER_FIELDS_NO_TIMELIMIT = "display_user_field_no_timelimit";
    const std::string DISPLAY_USER_FIELDS_COUNTRY = "display_user_field_country";
    const std::string DISPLAY_USER_FIELDS_USE_ANSI = "display_user_field_use_ansi";
    const std::string DISPLAY_USER_FIELDS_USERNOTE = "display_user_field_usernote";
    const std::string DISPLAY_USER_FIELDS_USE_VTBACKSPACE = "display_user_field_backspace";
    const std::string DISPLAY_USER_FIELDS_BIRTHDATE = "display_user_field_birthdate";
    const std::string DISPLAY_USER_FIELDS_WANTED = "display_user_field_wanted";
    const std::string DISPLAY_USER_FIELDS_FLAGS1 = "display_user_field_flags1";
    const std::string DISPLAY_USER_FIELDS_CLEAR_OR_SCROLL = "display_user_field_clearscreen";
    const std::string DISPLAY_USER_FIELDS_FLAGS2 = "display_user_field_flags2";
    const std::string DISPLAY_USER_FIELDS_SCREEN_PAUSE = "display_user_field_screenpause";
    const std::string DISPLAY_USER_FIELDS_EXTENDED_DETAILS = "display_user_field_extended";
    const std::string DISPLAY_USER_FIELDS_PREVIOUS_USER = "display_user_field_previous_user";
    const std::string DISPLAY_USER_FIELDS_NEXT_USER = "display_user_field_next_user";
    const std::string DISPLAY_USER_FIELDS_QUIT_SAVE = "display_user_field_quit_save";
    const std::string DISPLAY_USER_FIELDS_QUIT_ABORT = "display_user_field_quit_abort";
    
    // User Displayed Extended Fields.
    const std::string DISPLAY_USER_EXT_FIELDS_PASSWORD = "display_user_ext_field_password";
    const std::string DISPLAY_USER_EXT_FIELDS_GENDER = "display_user_ext_field_gender";
    const std::string DISPLAY_USER_EXT_FIELDS_PASSCHANGE_DATE = "display_user_ext_field_pass_change_date";
    const std::string DISPLAY_USER_EXT_FIELDS_PASS_FORCE_CHANCE = "display_user_ext_password_force_change";
    const std::string DISPLAY_USER_EXT_FIELDS_FIRSTON_DATE = "display_user_ext_field_firston_date";
    const std::string DISPLAY_USER_EXT_FIELDS_FILEPOINTS = "display_user_ext_field_filepoints";
    const std::string DISPLAY_USER_EXT_FIELDS_EXPIRE_DATE = "display_user_ext_field_expire_date";
    const std::string DISPLAY_USER_EXT_FIELDS_POSTCALL_RATIO = "display_user_ext_field_postcall_ratio";
    const std::string DISPLAY_USER_EXT_FIELDS_TIME_LIMIT = "display_user_ext_field_time_limit";
    const std::string DISPLAY_USER_EXT_FIELDS_TIME_LEFT = "display_user_ext_field_time_left";
    const std::string DISPLAY_USER_EXT_FIELDS_NUV_YESVOTES = "display_user_ext_field_nuv_yes_votes";
    const std::string DISPLAY_USER_EXT_FIELDS_NUV_NOVOTES = "display_user_ext_field_nuv_no_votes";
    const std::string DISPLAY_USER_EXT_FIELDS_REGULAR_COLOR = "display_user_ext_field_regular_color";
    const std::string DISPLAY_USER_EXT_FIELDS_INPUT_COLOR = "display_user_ext_field_input_color";
    const std::string DISPLAY_USER_EXT_FIELDS_PROMPT_COLOR = "display_user_ext_field_prompt_color";
    const std::string DISPLAY_USER_EXT_FIELDS_BOX_COLOR = "display_user_ext_field_box_color";
    const std::string DISPLAY_USER_EXT_FIELDS_STATUS_COLOR = "display_user_ext_field_status_color";
    const std::string DISPLAY_USER_EXT_FIELDS_INVERSE_COLOR = "display_user_ext_field_inverse_color";
    const std::string DISPLAY_USER_EXT_FIELDS_QUIT_RETURN = "display_user_ext_field_quit_return";
    
    // Extended Prompts
    const std::string PROMPT_USER_EXT_FIELDS_PASSWORD = "prompt_user_ext_field_password";
    const std::string PROMPT_USER_EXT_FIELDS_GENDER = "prompt_user_ext_field_gender";
    const std::string PROMPT_USER_EXT_FIELDS_PASSCHANGE_DATE = "prompt_user_ext_field_pass_change_date";
    const std::string PROMPT_USER_EXT_FIELDS_PASS_FORCE_CHANCE = "prompt_user_ext_password_force_change";
    const std::string PROMPT_USER_EXT_FIELDS_FIRSTON_DATE = "prompt_user_ext_field_firston_date";
    const std::string PROMPT_USER_EXT_FIELDS_FILEPOINTS = "prompt_user_ext_field_filepoints";
    const std::string PROMPT_USER_EXT_FIELDS_EXPIRE_DATE = "prompt_user_ext_field_expire_date";
    const std::string PROMPT_USER_EXT_FIELDS_POSTCALL_RATIO = "prompt_user_ext_field_postcall_ratio";
    const std::string PROMPT_USER_EXT_FIELDS_TIME_LIMIT = "prompt_user_ext_field_time_limit";
    const std::string PROMPT_USER_EXT_FIELDS_TIME_LEFT = "prompt_user_ext_field_time_left";
    const std::string PROMPT_USER_EXT_FIELDS_NUV_YESVOTES = "prompt_user_ext_field_nuv_yes_votes";
    const std::string PROMPT_USER_EXT_FIELDS_NUV_NOVOTES = "prompt_user_ext_field_nuv_no_votes";
    const std::string PROMPT_USER_EXT_FIELDS_REGULAR_COLOR = "prompt_user_ext_field_regular_color";
    const std::string PROMPT_USER_EXT_FIELDS_INPUT_COLOR = "prompt_user_ext_field_input_color";
    const std::string PROMPT_USER_EXT_FIELDS_PROMPT_COLOR = "prompt_user_ext_field_prompt_color";
    const std::string PROMPT_USER_EXT_FIELDS_BOX_COLOR = "prompt_user_ext_field_box_color";
    const std::string PROMPT_USER_EXT_FIELDS_STATUS_COLOR = "prompt_user_ext_field_status_color";
    const std::string PROMPT_USER_EXT_FIELDS_INVERSE_COLOR = "prompt_user_ext_field_inverse_color";
    
    /**
     * @brief Create Default Text Prompts for module
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
    void changeMenuInputState(int mod_menu_state_index);
    
    /**
     * @brief Redisplay's the current module prompt.
     * @return
     */
    void redisplayModulePrompt();

    /**
     * @brief Toggle the Option View.
     * @return
     */
    void toggleNextOptionView();

    /**
     * @brief Pull and Display Prompts
     * @param prompt
     */
    void displayPrompt(const std::string &prompt);
    
    /**
     * @brief Pull and parse and return Display Prompts for use in interfaces
     * @param prompt
     */
    std::string getDisplayPrompt(const std::string &prompt);

    /**
     * @brief Pull and parse and return Display Prompts for use in interfaces
     * @param prompt
     */
    std::string getDisplayPromptRaw(const std::string &prompt);

    /**
     * @brief Pull and Display Prompts with MCI Code
     * @param prompt
     * @param mci_field
     */
    void displayPromptMCI(const std::string &prompt, const std::string &mci_field);

    /**
     * @brief Pull and Display Prompts with following newline
     * @param prompt
     */
    void displayPromptAndNewLine(const std::string &prompt);

    /**
     * @brief Setup for the User Editor 
     * @return
     */
    void setupUserList();

    /**
     * @brief Setup for the User Field Editor
     * @return
     */
    void setupUserEditFields();

    /**
     * @brief Setup for the User Field Editor
     * @return
     */
    void setupUserEditExtendedFields();

    /**
     * Handle Input Commands.
     */

    /**
     * @brief Handles input command for User Editor Prompt
     * @return
     */
    void userListInput(const std::string &input);
    
    /**
     * @brief Handles User Number Input, Parses Strings and checks Valid User Id
     * @param input
     */
    void userEditorUserInput(const std::string &input);
    
    /**
     * @brief handle each user separate state and what to do next on input.
     * @param does_user_exist
     * @param user_id
     */
    void handleUserInputState(bool does_user_exist, long user_id);

    /**
     * @brief Copy an Existing User Record
     * @param user_id
     */
    void copyExistingUser(long user_id);

    /**
     * @brief Delete an Existing User Record
     * @param user_id
     */
    void deleteExistingUser(long user_id);

    /**
     * @brief Check if the user exists in the current listing by String Id
     * @param user_id
     */
    bool checkUserExistsById(long user_id);

    /**
     * @brief Check if the user exists in the current listing by String Id
     * @param user_id
     */
    bool loadUserById(long user_id);

    /**
     * @brief Check if the next user exists in the current listing by Current Id
     */
    bool nextUserById();
    
    /**
     * @brief Check if the previous user exists in the current listing by Current Id
     */
    bool previousUserById();

    /**
     * @brief Handles Input (Waiting for Any Key Press)
     * @param input
     */
    void userEditorPausedInput(const std::string &input);
    
    /**
     * Display Methods
     */
     
    /**
     * @brief Displays the current page of users
     * @param input_state
     */
    void displayCurrentPage(const std::string &input_state);
    
    /**
     * @brief User Editor, Read and Modify User Preferences
     * @return
     */
    std::string displayUserList();
    
    /**
     * @brief Handles User Field Editor Command Selection
     * @param input
     */
    void userEditorFieldInput(const std::string &input);
    
    /**
     * @brief Handles User Extended Field Editor Command Selection
     * @param input
     */
    void userEditorExtendedInput(const std::string &input);
    
    /**
     * @brief Handles Extended Field Updates for User Data
     * @param input
     */
    void userEditorExtendedFieldHandler(const std::string &input);

    /**
     * @brief Handles Field Updates for User Data
     * @param input
     */
    void userEditorFieldHandler(const std::string &input);
    
    /**
     * @brief User Editor, for Displaying Menu Fields to Edit
     * @return
     */
    std::string displayUserEditScreen();
    
    /**
     * @brief User Editor, for Displaying User Extended Fields to Edit
     * @return
     */
    std::string displayUserExtendedEditScreen();
    
    /**
     * @brief Displays the current page of user items
     * @param input_state
     */
    void displayCurrentEditPage(const std::string &input_state);
    
private:

    // Function Input Vector.
    std::vector<std::function< void()> >                    m_setup_functions;
    std::vector<std::function< void(const std::string &)> > m_mod_functions;
    std::vector<user_ptr>                                   m_users_listing;
    std::vector<std::string>                                m_user_display_list;    
    std::vector<user_ptr>                                   m_loaded_user;

    SessionIO              m_session_io;
    std::string            m_filename;
    text_prompts_dao_ptr   m_text_prompts_dao;

    unsigned int           m_mod_setup_index;
    unsigned int           m_mod_function_index;
    unsigned int           m_mod_user_state_index;
    
    bool                   m_is_text_prompt_exist;
    unsigned int           m_page;
    unsigned int           m_rows_per_page;
    
    unsigned long          m_current_user_id;
    unsigned int           m_current_field;
    std::string            m_wildcard_filter;
    unsigned long          m_user_array_position;
    
    CommonIO               m_common_io;
    directory_ptr          m_directory;
    
};


#endif // MOD_USER_EDITOR_HPP
