#include "mod_user_editor.hpp"

#include "../model-sys/security.hpp"
#include "../model-sys/users.hpp"
#include "../data-sys/security_dao.hpp"
#include "../data-sys/users_dao.hpp"
#include "../encryption.hpp"
#include "../access_condition.hpp"

#include <string>
#include <vector>
#include <cassert>
#include <regex>
 
/**
 * @brief Handles Updates or Data Input from Client
 * @return
 */
bool ModUserEditor::update(const std::string &character_buffer, const bool &)
{
    // Make sure system is active, when system is done, success or failes
    // We change this is inactive to single the login process is completed.
    if(!m_is_active)
    {
        std::cout << "ModUserEditor() !m_is_active" << std::endl;
        return false;
    }

    // Return True when were keeping module active / else false;
    if(character_buffer.size() == 0)
    {
        std::cout << "ModUserEditor() !character_buffer size 0" << std::endl;
        return true;
    }

    // Process all incoming data stright to the input functions.
    m_mod_functions[m_mod_function_index](character_buffer);

    return true;
}

/**
 * @brief Start-up class, setup and display initial screens / interface.
 * @return
 */
bool ModUserEditor::onEnter()
{
    std::cout << "OnEnter() ModUserEditor\n";
    m_is_active = true;

    // Grab ANSI Screen, display, if desired.. logon.ans maybe?
    std::string prompt = "\x1b[?25h"; // Turn on Cursor.
    baseProcessAndDeliver(prompt);

    // Execute the initial setup index.
    m_setup_functions[m_mod_setup_index]();
        
    return true;
}

/**
 * @brief Exit, close down, display screens to change over data.
 * @return
 */
bool ModUserEditor::onExit()
{
    std::cout << "OnExit() ModUserEditor\n";
    m_is_active = false;
    return true;
}

/**
 * @brief Create Default Text Prompts for module
 */
void ModUserEditor::createTextPrompts()
{
    // Create Mapping to pass for file creation (default values)
    M_TextPrompt value;

    value[PROMPT_HEADER]                  = std::make_pair("User Editor Header", "|CS|CR|03--- |15[|03Oblivion/2 XRM |07// |11User Editor|15] |03--- |11Filtering View : |15|OT |CR");
    value[PROMPT_INPUT_TEXT]              = std::make_pair("User Editor Prompt", "|CR|03E|15/dit User |03D|15/elete User |03C|15/opy User |03F|15/ilter Users |03Q|15/uit : ");
    value[PROMPT_PAUSE]                   = std::make_pair("Pause Prompt", "|CR |03- |15Hit any key to continue or (|03a|15)bort listing |03-|15 |CR");

    value[PROMPT_USER_CHANGE]             = std::make_pair("User Number To Change", "|CR|15Enter user number to |11EDIT|15 : ");
    value[PROMPT_USER_DELETE]             = std::make_pair("User Number To Delete", "|CR|15Enter user number to |11DELETE|15 : ");
    value[PROMPT_USER_COPY]               = std::make_pair("User Number To Copy", "|CR|15Enter user number to |11COPY|15 : ");
    value[PROMPT_USER_FILTER]             = std::make_pair("Wildcard (Example 'me*' lists names starting with 'me')", "|CR|03%   |15|PD|CR|11!   |15Enter wildcard to |11FILTER|15 by, Leave empty for |11ALL|15 : ");

    value[PROMPT_INVALID_USER_NOT_EXISTS] = std::make_pair("Invalid User Doesn't Exist", "|CR|04Invalid, User doesn't exist.|CR");
    value[PROMPT_USER_EDIT_HEADER]        = std::make_pair("User Fields Editor Header |OT User ID", "|CS|CR|03--- |15[|03Oblivion/2 XRM |07// |11User Editor|15] |03--- |11User ID : |15|OT |CR");
    value[PROMPT_USER_FIELD_INPUT_TEXT]   = std::make_pair("User Editor Command", "|CR|15User Editor C|07om|08mand |15: |07");

    // Prompts when editing a user field.
    value[PROMPT_USER_FIELD_USERNAME]     = std::make_pair("User Name is the Handle or users Logon ID", "|CR|03%   |15|PD|CR|11!   |03(|11A|03) |15User Name   : ");
    value[PROMPT_USER_FIELD_USERLEVEL]    = std::make_pair("User Level is for access control (0 - 255, 255 being Sysop Access)", "|CR|03%   |15|PD|CR|11!   |03(|11M|03) |15User Level     : ");

    value[PROMPT_USER_FIELD_REALNAME]     = std::make_pair("Real Name is users actual First and Last name", "|CR|03%   |15|PD|CR|11!   |03(|11B|03) |15Real Name   : ");
    value[PROMPT_USER_FIELD_FILELEVEL]    = std::make_pair("File Level is for access control (0 - 255, 255 being Sysop Access)", "|CR|03%   |15|PD|CR|11!   |03(|11N|03) |15File Level     : ");
    
    value[PROMPT_USER_FIELD_EMAIL]        = std::make_pair("Users Email Address", "|CR|03%   |15|PD|CR|11!   |03(|11C|03) |15Email       : ");
    value[PROMPT_USER_FIELD_MESGLEVEL]    = std::make_pair("Message Level is for access control (0 - 255, 255 being Sysop Access)", "|CR|03%   |15|PD|CR|11!   |03(|11O|03) |15Mesg Level     : ");
    
    value[PROMPT_USER_FIELD_ADDRESS]      = std::make_pair("Users Home Address (Usually Not Required)", "|CR|03%   |15|PD|CR|11!   |03(|11D|03) |15Address     : ");
    value[PROMPT_USER_FIELD_HACKATTEMPT]  = std::make_pair("Keeps track of Invalid Logon attempts since last logon", "|CR|03%   |15|PD|CR|11!   |03(|11P|03) |15Hack Attempts  : ");
    
    value[PROMPT_USER_FIELD_LOCATION]     = std::make_pair("Users City/State/Provence", "|CR|03%   |15|PD|CR|11!   |03(|11E|03) |15Location    : ");
    value[PROMPT_USER_FIELD_NOTIMELIMIT]  = std::make_pair("Disable daily time limit for a user", "|CR|03%   |15|PD|CR|11!   |03(|11R|03) |15No Time Limit  |07(|15T|07/|15F|07)|15 : ");

    value[PROMPT_USER_FIELD_COUNTRY]      = std::make_pair("Users Country Code", "|CR|03%   |15|PD|CR|11!   |03(|11F|03) |15Country     : ");
    value[PROMPT_USER_FIELD_USEANSI]      = std::make_pair("Enable ANSI Graphics, Otherwise strip ESC color codes", "|CR|03%   |15|PD|CR|11!   |03(|11S|03) |15Use ANSI  |07(|15T|07/|15F|07)|15 : ");
        
    value[PROMPT_USER_FIELD_USERNOTE]     = std::make_pair("User Note, Groups, Affiliations, or Tag line", "|CR|03%   |15|PD|CR|11!   |03(|11G|03) |15User Note   : ");
    value[PROMPT_USER_FIELD_BACKSPACE]    = std::make_pair("Enable Backspace translation for VT100 terminals", "|CR|03%   |15|PD|CR|11!   |03(|11T|03) |15VT100 BackSpace |07(|15T|07/|15F|07)|15 : ");
    
    value[PROMPT_USER_FIELD_BIRTHDATE]    = std::make_pair("User Date of Birth", "|CR|03%   |15|PD|CR|11!   |03(|11H|03) |15Birth Date  : ");
    value[PROMPT_USER_FIELD_WANTED]       = std::make_pair("Pages the sysop when the user logs on", "|CR|03%   |15|PD|CR|11!   |03(|11U|03) |15User Wanted |07(|15T|07/|15F|07)|15 : ");
           
    value[PROMPT_USER_FIELD_FLAGS1]       = std::make_pair("Access Restriction Flags Group 1 - Type Letters to Add/Remove/Toggle", "|CR|03%   |15|PD|CR|11!   |03(|11I|03) |15User Flags1 : ");
    value[PROMPT_USER_FIELD_CLEARSCREEN]  = std::make_pair("Clears User Screen when 'True', Otherwise scrolls when 'False'", "|CR|03%   |15|PD|CR|11!   |03(|11V|03) |15Clear or Scroll |07(|15T|07/|15F|07)|15 : ");

    value[PROMPT_USER_FIELD_FLAGS2]       = std::make_pair("Access Restriction Flags Group 2 - Type Letters to Add/Remove/Toggle", "|CR|03%   |15|PD|CR|11!   |03(|11J|03) |15User Flags2 : ");
    value[PROMPT_USER_FIELD_SCREENPAUSE]  = std::make_pair("Clears User Screen when 'True', Otherwise scrolls when 'False'", "|CR|03%   |15|PD|CR|11!   |03(|11W|03) |15Screen Pause  |07(|15T|07/|15F|07)|15 : ");

    value[PROMPT_USER_EDIT_EXTENDED_HEADER] = std::make_pair("User Extended Fields Editor Header |OT User ID", "|CS|CR|03--- |15[|03Oblivion/2 XRM |07// |11User Extended Editor|15] |03--- |11User ID : |15|OT |CR");
    value[PROMPT_USER_EXTENDED_FIELD_INPUT_TEXT] = std::make_pair("User Editor Extended Command", "|CR|15User Extended Editor C|07om|08mand |15: |07");

    // Fields Being Displayed on user edit screen
    value[DISPLAY_USER_FIELDS_USERNAME]           = std::make_pair("User Name", " |03(|11A|03) |15User Name   : ");
    value[DISPLAY_USER_FIELDS_LEVEL]              = std::make_pair("Level", " |03(|11M|03) |15User Level     : ");        
    value[DISPLAY_USER_FIELDS_REALNAME]           = std::make_pair("Real Name"," |03(|11B|03) |15Real Name   : ");
    value[DISPLAY_USER_FIELDS_FILELEVEL]          = std::make_pair("File Level", " |03(|11N|03) |15File Level     : ");
    value[DISPLAY_USER_FIELDS_EMAIL]              = std::make_pair("Email Address", " |03(|11C|03) |15Eamil       : ");
    value[DISPLAY_USER_FIELDS_MESGLEVEL]          = std::make_pair("Message Level", " |03(|11O|03) |15Mesg Level     : ");
    value[DISPLAY_USER_FIELDS_ADDRESS]            = std::make_pair("Address", " |03(|11D|03) |15Address     : ");
    value[DISPLAY_USER_FIELDS_HACK_ATTEMPT]       = std::make_pair("Number of Invalid Logons", " |03(|11P|03) |15Hack Attempts  : ");
    value[DISPLAY_USER_FIELDS_LOCATION]           = std::make_pair("Location", " |03(|11E|03) |15Location    : ");
    value[DISPLAY_USER_FIELDS_NO_TIMELIMIT]       = std::make_pair("No Time Limit", " |03(|11R|03) |15No Time Limit  : ");    
    value[DISPLAY_USER_FIELDS_COUNTRY]            = std::make_pair("Country Code", " |03(|11F|03) |15Country     : ");
    value[DISPLAY_USER_FIELDS_USE_ANSI]           = std::make_pair("Use ANSI Graphics", " |03(|11S|03) |15Use ANSI       : ");   
    value[DISPLAY_USER_FIELDS_USERNOTE]           = std::make_pair("User Note"," |03(|11G|03) |15User Note   : ");
    value[DISPLAY_USER_FIELDS_USE_VTBACKSPACE]    = std::make_pair("Use VT100 Backspace"," |03(|11T|03) |15VT100 BackSpace: ");    
    value[DISPLAY_USER_FIELDS_BIRTHDATE]          = std::make_pair("Date of Birth", " |03(|11H|03) |15Birth Date  : ");
    value[DISPLAY_USER_FIELDS_WANTED]             = std::make_pair("Page Sysop When user Logs in", " |03(|11U|03) |15User Wanted    : ");    
    value[DISPLAY_USER_FIELDS_FLAGS1]             = std::make_pair("AR User Flags 1", " |03(|11I|03) |15User Flags1 : ");
    value[DISPLAY_USER_FIELDS_CLEAR_OR_SCROLL]    = std::make_pair("Clear or Scroll Screen to Clear", " |03(|11V|03) |15Clear or Scroll: ");    
    value[DISPLAY_USER_FIELDS_FLAGS2]             = std::make_pair("AR User Flags 2", " |03(|11J|03) |15User Flags2 : ");
    value[DISPLAY_USER_FIELDS_SCREEN_PAUSE]       = std::make_pair("Pause screen when text scrolls screen height", " |03(|11W|03) |15Screen Pause   : ");    
    value[DISPLAY_USER_FIELDS_EXTENDED_DETAILS]   = std::make_pair("Shows Extended User Details", " |03(|11-|03) |15Extended User Details");    
    value[DISPLAY_USER_FIELDS_PREVIOUS_USER]      = std::make_pair("Move to Previous User", " |03(|11[|03) |15Previous User        ");
    value[DISPLAY_USER_FIELDS_NEXT_USER]          = std::make_pair("Move to Next User", " |03(|11]|03) |15Next User            ");    
    value[DISPLAY_USER_FIELDS_QUIT_SAVE]          = std::make_pair("Quit and Save", " |03(|11Q|03) |15Quit & Save          ");
    value[DISPLAY_USER_FIELDS_QUIT_ABORT]         = std::make_pair("Exit without Saving", " |03(|11X|03) |15Exit without Saving  ");
    
    // Extended User Field Display
    value[DISPLAY_USER_EXT_FIELDS_PASSWORD]           = std::make_pair("Password", " |03(|11A|03) |15Password         : ");
    value[DISPLAY_USER_EXT_FIELDS_GENDER]             = std::make_pair("Gender", " |03(|11M|03) |15User Gender    : ");    
    value[DISPLAY_USER_EXT_FIELDS_PASSCHANGE_DATE]    = std::make_pair("Password Last Change Date", " |03(|11B|03) |15Pass Change Date : ");
    value[DISPLAY_USER_EXT_FIELDS_PASS_FORCE_CHANCE]  = std::make_pair("Days to Force Password Change", " |03(|11N|03) |15Force Pass Chng: ");    
    value[DISPLAY_USER_EXT_FIELDS_FIRSTON_DATE]       = std::make_pair("Sign-up Date", " |03(|11C|03) |15First On Date    : ");
    value[DISPLAY_USER_EXT_FIELDS_FILEPOINTS]         = std::make_pair("File Points", " |03(|11O|03) |15File Points    : ");    
    value[DISPLAY_USER_EXT_FIELDS_EXPIRE_DATE]        = std::make_pair("Expiration Date", " |03(|11D|03) |15Expiration Date  : ");
    value[DISPLAY_USER_EXT_FIELDS_POSTCALL_RATIO]     = std::make_pair("Post/Call Ratio", " |03(|11P|03) |15Post Call Ratio: ");    
    value[DISPLAY_USER_EXT_FIELDS_TIME_LIMIT]         = std::make_pair("Time Limit in Minutes Per Day", " |03(|11E|03) |15Daily Time Limit : ");
    value[DISPLAY_USER_EXT_FIELDS_TIME_LEFT]          = std::make_pair("Time Left Today in Minutes", " |03(|11Q|03) |15Time Left Today: ");    
    value[DISPLAY_USER_EXT_FIELDS_NUV_YESVOTES]       = std::make_pair("New User Voting - Yes Votes", " |03(|11F|03) |15NUV Yes Votes    : ");
    value[DISPLAY_USER_EXT_FIELDS_NUV_NOVOTES]        = std::make_pair("New User Voting - No Votes", " |03(|11R|03) |15NUV No Votes   : ");    
    value[DISPLAY_USER_EXT_FIELDS_REGULAR_COLOR]      = std::make_pair("Regular Color", " |03(|11G|03) |15Regular Color    : ");
    value[DISPLAY_USER_EXT_FIELDS_INPUT_COLOR]        = std::make_pair("Input Color", " |03(|11S|03) |15Input Color    : ");    
    value[DISPLAY_USER_EXT_FIELDS_PROMPT_COLOR]       = std::make_pair("Prompt Color", " |03(|11H|03) |15Prompt Color     : ");
    value[DISPLAY_USER_EXT_FIELDS_BOX_COLOR]          = std::make_pair("Box Color", " |03(|11T|03) |15Box Color      : ");
    value[DISPLAY_USER_EXT_FIELDS_STATUS_COLOR]       = std::make_pair("Status Color", " |03(|11I|03) |15Status Color     : ");
    value[DISPLAY_USER_EXT_FIELDS_INVERSE_COLOR]      = std::make_pair("Inverse Color", " |03(|11U|03) |15Inverse Color  : ");
    value[DISPLAY_USER_EXT_FIELDS_QUIT_RETURN]        = std::make_pair("Quit and Return", " |03(|11Q|03) |15Quit & Return        ");
    
    value[PROMPT_USER_EXT_FIELDS_PASSWORD]           = std::make_pair("Password", "|CR|03%   |15|PD|CR|11!   |03(|11A|03) |15Password         : ");
    value[PROMPT_USER_EXT_FIELDS_GENDER]             = std::make_pair("Gender", "|CR|03%   |15|PD|CR|11!   |03(|11M|03) |15User Gender    : ");    
    value[PROMPT_USER_EXT_FIELDS_PASSCHANGE_DATE]    = std::make_pair("Password Last Change Date", "|CR|03%   |15|PD|CR|11!   |03(|11B|03) |15Pass Change Date : ");
    value[PROMPT_USER_EXT_FIELDS_PASS_FORCE_CHANCE]  = std::make_pair("Days to Force Password Change", "|CR|03%   |15|PD|CR|11!   |03(|11N|03) |15Force Pass Chng: ");    
    value[PROMPT_USER_EXT_FIELDS_FIRSTON_DATE]       = std::make_pair("Sign-up Date", "|CR|03%   |15|PD|CR|11!   |03(|11C|03) |15First On Date    : ");
    value[PROMPT_USER_EXT_FIELDS_FILEPOINTS]         = std::make_pair("File Points", "|CR|03%   |15|PD|CR|11!   |03(|11O|03) |15File Points    : ");    
    value[PROMPT_USER_EXT_FIELDS_EXPIRE_DATE]        = std::make_pair("Expiration Date", "|CR|03%   |15|PD|CR|11!   |03(|11D|03) |15Expiration Date  : ");
    value[PROMPT_USER_EXT_FIELDS_POSTCALL_RATIO]     = std::make_pair("Post/Call Ratio", "|CR|03%   |15|PD|CR|11!   |03(|11P|03) |15Post Call Ratio: ");    
    value[PROMPT_USER_EXT_FIELDS_TIME_LIMIT]         = std::make_pair("Time Limit in Minutes Per Day", "|CR|03%   |15|PD|CR|11!   |03(|11E|03) |15Daily Time Limit : ");
    value[PROMPT_USER_EXT_FIELDS_TIME_LEFT]          = std::make_pair("Time Left Today in Minutes", "|CR|03%   |15|PD|CR|11!   |03(|11Q|03) |15Time Left Today: ");    
    value[PROMPT_USER_EXT_FIELDS_NUV_YESVOTES]       = std::make_pair("New User Voting - Yes Votes", "|CR|03%   |15|PD|CR|11!   |03(|11F|03) |15NUV Yes Votes    : ");
    value[PROMPT_USER_EXT_FIELDS_NUV_NOVOTES]        = std::make_pair("New User Voting - No Votes", "|CR|03%   |15|PD|CR|11!   |03(|11R|03) |15NUV No Votes   : ");    
    value[PROMPT_USER_EXT_FIELDS_REGULAR_COLOR]      = std::make_pair("Regular Color", "|CR|03%   |15|PD|CR|11!   |03(|11G|03) |15Regular Color    : ");
    value[PROMPT_USER_EXT_FIELDS_INPUT_COLOR]        = std::make_pair("Input Color", "|CR|03%   |15|PD|CR|11!   |03(|11S|03) |15Input Color    : ");    
    value[PROMPT_USER_EXT_FIELDS_PROMPT_COLOR]       = std::make_pair("Prompt Color", "|CR|03%   |15|PD|CR|11!   |03(|11H|03) |15Prompt Color     : ");
    value[PROMPT_USER_EXT_FIELDS_BOX_COLOR]          = std::make_pair("Box Color", "|CR|03%   |15|PD|CR|11!   |03(|11T|03) |15Box Color      : ");
    value[PROMPT_USER_EXT_FIELDS_STATUS_COLOR]       = std::make_pair("Status Color", "|CR|03%   |15|PD|CR|11!   |03(|11I|03) |15Status Color     : ");
    value[PROMPT_USER_EXT_FIELDS_INVERSE_COLOR]      = std::make_pair("Inverse Color", "|CR|03%   |15|PD|CR|11!   |03(|11U|03) |15Inverse Color  : ");

    m_text_prompts_dao->writeValue(value);
}

/**
 * @brief Sets an individual input module index.
 * @param mod_function_index
 */
void ModUserEditor::changeInputModule(int mod_function_index)
{
    // Setup input module
    m_mod_function_index = mod_function_index;
}

/**
 * @brief Sets an individual setup method module index.
 * @param mod_function_index
 */
void ModUserEditor::changeSetupModule(int mod_function_index)
{
    // Set, and Execute the Setup module.
    m_mod_setup_index = mod_function_index;
    m_setup_functions[m_mod_setup_index]();
}

/**
 * @brief Sets an individual Menu Input State Add/Change/Delete
 * @param mod_menu_state_index
 */
void ModUserEditor::changeMenuInputState(int mod_menu_state_index)
{
    m_mod_user_state_index = mod_menu_state_index;
}

/**
 * @brief Redisplay's the current module prompt.
 * @return
 */
void ModUserEditor::redisplayModulePrompt()
{
    m_setup_functions[m_mod_setup_index]();
}
   
/**
 * @brief Pull and Display Prompts
 * @param prompt
 */
void ModUserEditor::displayPrompt(const std::string &prompt)
{
    baseDisplayPrompt(prompt, m_text_prompts_dao);
}

/**
 * @brief Pull and parse and return Display Prompts for use in interfaces
 * @param prompt
 */
std::string ModUserEditor::getDisplayPrompt(const std::string &prompt)
{
    return baseGetDisplayPrompt(prompt, m_text_prompts_dao);
}

/**
 * @brief Pull and parse and return Display Prompts for use in interfaces
 * @param prompt
 */
std::string ModUserEditor::getDisplayPromptRaw(const std::string &prompt)
{
    return baseGetDisplayPromptRaw(prompt, m_text_prompts_dao);
}

/**
 * @brief Pull and Display Prompts with MCI Code
 * @param prompt
 * @param mci_field
 */
void ModUserEditor::displayPromptMCI(const std::string &prompt, const std::string &mci_field)
{
    baseDisplayPromptMCI(prompt, m_text_prompts_dao, mci_field);
}

/**
 * @brief Pull and Display Prompts with following newline
 * @param prompt
 */
void ModUserEditor::displayPromptAndNewLine(const std::string &prompt)
{
    baseDisplayPromptAndNewLine(prompt, m_text_prompts_dao);
}

/**
 * @brief Setup for the User Editor 
 * @return
 */
void ModUserEditor::setupUserList() 
{
    // Display if were filtering or not
    std::string string_filter = "ALL";
    if (m_wildcard_filter.size() > 0)
    {
        string_filter = m_wildcard_filter;
    }
    
    baseTransformToUpper(string_filter);    
    displayPromptMCI(PROMPT_HEADER, string_filter);
    
    // Build a list of screen lines for the menu display
    // So we know when to pause in large listing, or use pagenation.
    std::string user_display_output = displayUserList();    
    
    if (m_user_display_list.size() == 0) 
    {
        // Clear Out list if anything already exists.
        std::vector<std::string>().swap(m_user_display_list);
    }
    
    m_user_display_list = m_common_io.splitString(user_display_output, '\n');           
    m_page = 0;        
    displayCurrentPage(PROMPT_INPUT_TEXT);
}

/**
 * @brief Setup for the User Field Editor
 * @return
 */
void ModUserEditor::setupUserEditFields() 
{    
    // Build a list of screen lines for the menu display
    // So we know when to pause in large listing, or use pagenation.
    std::string user_display_output = displayUserEditScreen();   
    if (user_display_output.size() == 0)
        return;
    
    std::string user_count_display = std::to_string(m_current_user_id);
    user_count_display.append(baseGetDefaultStatColor() + " Record " + std::to_string(m_user_array_position+1) + 
        " of " + std::to_string(m_users_listing.size()));
    displayPromptMCI(PROMPT_USER_EDIT_HEADER, user_count_display);
    
    if (m_user_display_list.size() == 0) 
    {
        // Clear Out list if anything already exists.
        std::vector<std::string>().swap(m_user_display_list);
    }
    
    m_user_display_list = m_common_io.splitString(user_display_output, '\n');           
    m_page = 0;        
    displayCurrentEditPage(PROMPT_USER_FIELD_INPUT_TEXT);
}

/**
 * @brief Setup for the User Field Editor
 * @return
 */
void ModUserEditor::setupUserEditExtendedFields() 
{    
    // Build a list of screen lines for the menu display
    // So we know when to pause in large listing, or use pagenation.
    std::string user_display_output = displayUserExtendedEditScreen();   
    if (user_display_output.size() == 0)
        return;
    
    std::string user_count_display = std::to_string(m_current_user_id);
    user_count_display.append(baseGetDefaultStatColor() + " Record " + std::to_string(m_user_array_position+1) + 
        " of " + std::to_string(m_users_listing.size()));
    displayPromptMCI(PROMPT_USER_EDIT_EXTENDED_HEADER, user_count_display);
    
    if (m_user_display_list.size() == 0) 
    {
        // Clear Out list if anything already exists.
        std::vector<std::string>().swap(m_user_display_list);
    }
    
    m_user_display_list = m_common_io.splitString(user_display_output, '\n');           
    m_page = 0;        
    displayCurrentEditPage(PROMPT_USER_EXTENDED_FIELD_INPUT_TEXT);
}

/**
 * @brief Handles input command for User Editor Prompt
 * @return
 */
void ModUserEditor::userListInput(const std::string &input) 
{
    std::string key = "";
    std::string result = m_session_io.getInputField(input, key, Config::sSingle_key_length);

    // ESC was hit
    if(result == "aborted") 
    {
        std::cout << "aborted!" << std::endl;        
        return;
    }
    else if(result[0] == '\n')
    {            
        // Key == 0 on [ENTER] pressed alone. then invalid!
        if(key.size() == 0)
        {
            // Return and don't do anything.
            return;
        }
                
        baseProcessDeliverNewLine();
        
        std::string output_buffer = m_config->default_color_regular;
        switch (toupper(key[0]))
        {               
            case 'E': // Change/Edit
                changeMenuInputState(USER_CHANGE);
                displayPrompt(PROMPT_USER_CHANGE);
                changeInputModule(MOD_USER_NAME);
                break;
                
            case 'D': // Delete
                changeMenuInputState(USER_DELETE);
                displayPrompt(PROMPT_USER_DELETE);
                changeInputModule(MOD_USER_NAME);
                break;
                
            case 'C': // Copy
                changeMenuInputState(USER_COPY);
                displayPrompt(PROMPT_USER_COPY);
                changeInputModule(MOD_USER_NAME);
                break;
                
            case 'F': // Filter
                changeMenuInputState(USER_FILTER);
                displayPrompt(PROMPT_USER_FILTER);
                changeInputModule(MOD_USER_NAME);
                break;
                
            case 'Q': // Quit
                // Reload fall back, or gosub to last menu!
                m_is_active = false;
                return;
                
            default:
                redisplayModulePrompt();
                break;
        }                
    }
    else
    {
        // Send back the single input received to show client key presses.
        // Only if return data shows a processed key returned.
        if (result != "empty") 
        {
            baseProcessDeliverInput(result);
        }
    }
}

/**
 * @brief Handles User Number Input, Parses Strings and checks Valid User Id
 * @param input
 */
void ModUserEditor::userEditorUserInput(const std::string &input)
{
    std::string key = "";
    std::string result = m_session_io.getInputField(input, key, Config::sName_length);

    // ESC was hit
    if(result == "aborted") 
    {
        std::cout << "aborted!" << std::endl;
        changeInputModule(MOD_USER_INPUT);
        redisplayModulePrompt();
        return;
    }
    else if(result[0] == '\n')
    {            
        // Allow Empty for Filter only!!
        // If were updating the filter, catch
        // Adjust user list and return
        if (m_mod_user_state_index == USER_FILTER)
        {
            m_wildcard_filter = key;
            changeInputModule(MOD_USER_INPUT);
            redisplayModulePrompt();
            return;
        }
        
        // Key == 0 on [ENTER] pressed alone. then invalid!
        if(key.size() == 0)
        {
            changeInputModule(MOD_USER_INPUT);
            redisplayModulePrompt();
            return;
        }
                
        baseProcessDeliverNewLine();  

        long user_id = -1;
        std::stringstream ss(key);
        ss >> user_id;

        // check for Invalid Index.
        if (ss.fail() || user_id < 0)
        {
            ss.clear();
            user_id = -1;
        }              
        
        if (user_id != -1 && checkUserExistsById(user_id))
        {            
            handleUserInputState(true, user_id);
        }
        else 
        {            
            handleUserInputState(false, user_id);
        }
    }
    else
    {
        // Send back the single input received to show client key presses.
        // Only if return data shows a processed key returned.
        if (result != "empty") 
        {
            baseProcessDeliverInput(result);
        }
    }
}

/**
 * @brief handle each user separate state and what to do next on input.
 * @param does_user_exist
 * @param user_id
 */
void ModUserEditor::handleUserInputState(bool does_user_exist, long user_id) 
{    
    switch (m_mod_user_state_index)
    {           
        case USER_CHANGE:
            if (does_user_exist)
            {
                // Move to new Default setup for Options vs Menus.
                // Also set the curent menu for the system to load
                // to pull the commands from.
                m_current_user_id = user_id;
                changeInputModule(MOD_USER_FIELD_INPUT);
                changeSetupModule(MOD_DISPLAY_USER_FIELDS);
            }
            else
            {
                // Error, can't remove a menu that doesn't exist!
                displayPrompt(PROMPT_INVALID_USER_NOT_EXISTS);
                displayPrompt(PROMPT_INPUT_TEXT);
                changeInputModule(MOD_USER_INPUT);
            }            
            break;
            
        case USER_DELETE:
            if (does_user_exist)
            {
                deleteExistingUser(user_id);
                changeInputModule(MOD_USER_INPUT);
                redisplayModulePrompt();
            }
            else
            {
                // Error, can't remove a menu that doesn't exist!
                displayPrompt(PROMPT_INVALID_USER_NOT_EXISTS);
                displayPrompt(PROMPT_INPUT_TEXT);
                changeInputModule(MOD_USER_INPUT);
            }
            break;
            
        case USER_COPY:
            // Copies and Adds a new record to back of the list.
            if (does_user_exist) 
            {
                copyExistingUser(user_id);
                changeInputModule(MOD_USER_INPUT);
                redisplayModulePrompt();
            }
            else 
            {
                // Error, can't remove a menu that doesn't exist!
                displayPrompt(PROMPT_INVALID_USER_NOT_EXISTS);
                displayPrompt(PROMPT_INPUT_TEXT);
                changeInputModule(MOD_USER_INPUT);
            }
            break;
    }        
}

/**
 * @brief Copy an Existing User Record
 * @param user_id
 */
void ModUserEditor::copyExistingUser(long user_id)
{
    users_dao_ptr user_data(new UsersDao(m_session_data->m_user_database));
    security_dao_ptr security_dao(new SecurityDao(m_session_data->m_user_database));
    
    user_ptr lookup_user = user_data->getRecordById(user_id);
    
    // Default Id when not found is -1
    if (lookup_user->iId == -1)
        return;
    
    // Setup for default password and challenge questions.   
    encrypt_ptr encryption(new Encrypt());
    std::string salt = encryption->generate_salt(lookup_user->sHandle, m_config->bbs_uuid);
    std::string password = encryption->generate_password(m_config->password_default_user, salt);

    if(salt.size() == 0 || password.size() == 0)
    {
        std::cout << "Error, Salt or Password were empty" << std::endl;
        assert(false);
    }
    
    security_ptr security_record(new Security());
    security_record->sChallengeAnswerHash = password;    
    security_record->sPasswordHash = password;
    security_record->sSaltHash = salt;
    
    long securityIndex = security_dao->insertRecord(security_record);
    if (securityIndex < 0)
    {
        std::cout << "Error, unable to insert new user record." << std::endl;
        return;
    }
    
    // Save New User Record
    lookup_user->iSecurityIndex = securityIndex;
    
    // Loop existing user records and make sure if we copy, that name is unique
    bool found = true;
    long id = 1;
    std::string user_name = "New User";
    while(found) 
    {
        user_ptr check_user = user_data->getUserByRealName(user_name + std::to_string(id));
        if (check_user->iId == -1)
        {
            // Not found, were good to go
            found = false;
            lookup_user->sRealName = user_name + std::to_string(id);
            lookup_user->sHandle = user_name + std::to_string(id);
            lookup_user->sEmail = user_name + std::to_string(id) + "@xrm.com";
            
            long userIndex = user_data->insertRecord(lookup_user);
            if (userIndex < 0)
            {
                // If user Index is not created, then remove the secutiry record.
                security_dao->deleteRecord(securityIndex);
            }
        }
        ++id;
    }
}

/**
 * @brief Delete an Existing User Record
 * @param user_id
 */
void ModUserEditor::deleteExistingUser(long user_id)
{
    // Should Cascade and remove Security Record also.
    users_dao_ptr user_data(new UsersDao(m_session_data->m_user_database));
    user_data->deleteRecord(user_id);
}

/**
 * @brief Check if the user exists in the current listing by String Id
 * @param user_id
 */
bool ModUserEditor::checkUserExistsById(long user_id)
{    
    users_dao_ptr user_data(new UsersDao(m_session_data->m_user_database));
    user_ptr lookup_user = user_data->getRecordById(user_id);
    
    // Default Id when not found is -1
    if (lookup_user->iId == -1)
        return false;
        
    return true;
}

/**
 * @brief Check if the user exists in the current listing by String Id
 * @param user_id
 */
bool ModUserEditor::loadUserById(long user_id)
{        
    // If record is already loaded then leave it.
    if (m_loaded_user.size() > 0)
    {
        std::cout << " *** user already loaded!!" << std::endl;
        return true;
    }
    
    for (unsigned int i = 0; i < m_users_listing.size(); i++)
    {
        if (m_users_listing[i]->iId == user_id)
        {
            m_user_array_position = i;
            m_loaded_user.push_back(m_users_listing[i]); 
            return true;            
        }
    }
    
    return false;
}

/**
 * @brief Check if the next user exists in the current listing by Current Id
 */
bool ModUserEditor::nextUserById()
{            
    if (m_user_array_position+1 < m_users_listing.size())
    {
        if (m_loaded_user.size() > 0)
            std::vector<user_ptr>().swap(m_loaded_user);
            
        ++m_user_array_position;
        m_loaded_user.push_back(m_users_listing[m_user_array_position]);
        m_current_user_id = m_users_listing[m_user_array_position]->iId;
        
        return true;
    }    
    return false;
}

/**
 * @brief Check if the previous user exists in the current listing by Current Id
 */
bool ModUserEditor::previousUserById()
{            
    if (m_user_array_position > 0)
    {
        if (m_loaded_user.size() > 0)
            std::vector<user_ptr>().swap(m_loaded_user);
            
        --m_user_array_position;
        m_loaded_user.push_back(m_users_listing[m_user_array_position]); 
        m_current_user_id = m_users_listing[m_user_array_position]->iId;
        return true;
    }    
    return false;
}

/**
 * @brief Handles Input (Waiting for Any Key Press)
 * @param input
 */
void ModUserEditor::userEditorPausedInput(const std::string &input)
{
    std::string current_state_input;    
    unsigned int current_module_input;    
    switch(m_mod_setup_index)
    {
        case MOD_DISPLAY_USER_LIST:
            current_state_input = PROMPT_INPUT_TEXT;
            current_module_input = MOD_USER_INPUT;
            break;
    }    
    
    // Check for abort on pause for next.
    if (input.size() == 1 && std::toupper(input[0]) == 'A')
    {
        displayPrompt(current_state_input);
        changeInputModule(current_module_input);
        return;
    }
    
    // Any input coming in here is valid
    // Increment to next page, then display remaining results.
    ++m_page;
    displayCurrentPage(current_state_input);
}

/**
 * @brief Displays the current page of users
 * @param input_state
 */
void ModUserEditor::displayCurrentPage(const std::string &input_state) 
{    
    // calculate the rows_per_page.
    unsigned int rows_used = m_ansi_process->getMaxRowsUsedOnScreen();
    unsigned int max_rows = m_ansi_process->getMaxLines();
    
    if (m_page > 0)
        rows_used -= (m_ansi_process->m_number_lines - 2);
        
    m_rows_per_page = max_rows - (rows_used + 2);
    
    bool displayed_all_rows = true;
    for (unsigned int i = (m_page*(m_rows_per_page-2)); i < m_user_display_list.size(); i++) 
    {
        std::string display_line = m_session_io.pipe2ansi(m_user_display_list[i]);
        display_line.append("\r\n");
        baseProcessAndDeliver(display_line);
        
        if (i >= (m_page*m_rows_per_page) + m_rows_per_page)
        {
            // We've displayed the max amount of rows per the currnet
            // screen break out and wait for prompt or next page.
            displayed_all_rows = false;
            break;
        }
    }
    
    // Default Page Input Method
    unsigned int current_module_input;
    switch(m_mod_setup_index)
    {
        case MOD_DISPLAY_USER_LIST:
            current_module_input = MOD_USER_INPUT;
            break;
                       
        default:
            std::cout << "Error, forgot to add new STATE index displayCurrentPage!!";
            return;
    }  
    
    // If we displayed all rows, then display propmpt, otherwise
    // Ask to hit anykey for next page.
    if (displayed_all_rows)
    {
        // Reset Page back to Zero for next display.
        m_page = 0;
        displayPrompt(input_state);
        changeInputModule(current_module_input);
    }
    else 
    {
        displayPrompt(PROMPT_PAUSE);
        changeInputModule(MOD_PAUSE);
    }
}

/**
 * @brief User Editor, Read and Modify User Preferences
 * @return
 */
std::string ModUserEditor::displayUserList()
{  
    users_dao_ptr user_data(new UsersDao(m_session_data->m_user_database));
        
    // Clear All Users
    if (m_users_listing.size() > 0)
        std::vector<user_ptr>().swap(m_users_listing);
    
    if (m_wildcard_filter.size() > 0)
    {
        m_users_listing = user_data->getUsersByWildcard(m_wildcard_filter);
    }
    else
    {
        m_users_listing = user_data->getAllRecords();
    }
    
    // If no records, add message to user.
    std::vector<std::string> result_set;  
    if (m_users_listing.size() == 0)
    {
        result_set.push_back(baseGetDefaultStatColor() + m_common_io.rightPadding("No Records Found!", 24));
    }
   
     // Build a string list of individual menu options, then loop to fit as many per screen!    
    for(unsigned int i = 0; i < m_users_listing.size(); i++)
    {  
        std::string option_string = m_common_io.rightPadding(std::to_string(m_users_listing[i]->iId), 5);      
        if (m_users_listing[i]->sHandle.size() == 0)
            option_string.append(baseGetDefaultStatColor() + m_common_io.rightPadding(m_users_listing[i]->sRealName, 19));
        else
            option_string.append(baseGetDefaultStatColor() + m_common_io.rightPadding(m_users_listing[i]->sHandle, 19));
            
        result_set.push_back(option_string);
    }

    // iterate through and print out
    int total_rows = result_set.size() / 3;
    int remainder = result_set.size() % 3;

    // Add for Header and Footer Row!
    total_rows += 2;
    if(remainder > 0)
        ++total_rows;

    // Could re-calc this on screen width lateron.
    int max_cols = 76;

    // Vector or Menus, Loop through
    std::vector<std::string>::iterator i = result_set.begin();
    std::string buffer = "";
    
    for(int rows = 0; rows < total_rows; rows++)
    {
        buffer += "  "; // 3 Leading spaces per row.
        for(int cols = 0; cols < max_cols; cols++)
        {
            // Top Row
            if(rows == 0 && cols == 0)
            {
                buffer += baseGetDefaultBoxColor();
                buffer += BORDER_TOP_LEFT;
            }
            else if(rows == 0 && cols == max_cols-1)
            {
                buffer += baseGetDefaultBoxColor();
                buffer += BORDER_TOP_RIGHT;
            }
            else if(rows == 0 && cols % 25 == 0)
            {
                buffer += baseGetDefaultBoxColor();
                buffer += BORDER_MID_TOP;
            } 
            else if(rows == 0)
            {
                buffer += baseGetDefaultBoxColor();
                buffer += BORDER_ROW;
            }

            // Bottom Row
            else if(rows == total_rows-1 && cols == 0)
            {
                buffer += baseGetDefaultBoxColor();
                buffer += BORDER_BOT_LEFT;
            }
            else if(rows == total_rows-1 && cols == max_cols-1)
            {
                buffer += baseGetDefaultBoxColor();
                buffer += BORDER_BOT_RIGHT;
            }
            else if(rows == total_rows-1 && cols % 25 == 0)
            {
                buffer += baseGetDefaultBoxColor();
                buffer += BORDER_MID_BOT;
            } 
            else if(rows == total_rows-1)
            {
                buffer += baseGetDefaultBoxColor();
                buffer += BORDER_ROW;
            }
            else if(cols % 25 == 0)
            {
                buffer += baseGetDefaultBoxColor();
                buffer += BORDER_MID;
            }
            else
            {
                // Here we insert the Menu name and pad through to 8 characters.
                if(cols % 26 == 0 || cols == 1)
                {
                    if(i != result_set.end())
                    {
                        buffer += baseGetDefaultInputColor();
                        buffer += *i;
                        ++i;
                    }
                    else
                    {
                        // Empty, 24 Spaces default menu name size.
                        buffer += "                        ";
                    }
                }
            }
        }
        
        // Were going to split on \n, which will get replaced lateron
        // with \r\n for full carriage returns.        
        buffer += "\n";
    }
    
    return (buffer);    
}

/**
 * @brief Handles User Field Editor Command Selection
 * @param input
 */
void ModUserEditor::userEditorFieldInput(const std::string &input)
{
    // Provide Hotkeys only for switching to next/previous options
    switch(input[0]) 
    {        
        // TODO, update current users m_loaded_user -> m_users_list array with update also.
        // So when we change users we keep the changes made!
        // search m_users_listing for previous / next record.        
        case '[': // previous user
            previousUserById();
            redisplayModulePrompt();
            return;
                
        case ']': // next user
            nextUserById();
            redisplayModulePrompt();
            return;
    }
    
    std::string key = "";
    std::string result = m_session_io.getInputField(input, key, Config::sSingle_key_length);

    // ESC was hit
    if(result == "aborted") 
    {
        std::cout << "aborted!" << std::endl;
        return;
    }
    else if(result[0] == '\n')
    {            
        // Key == 0 on [ENTER] pressed alone. then invalid!
        if(key.size() == 0)
        {
            // Return and don't do anything.
            return;
        }
                
        baseProcessDeliverNewLine();
        m_current_field = toupper(key[0]);
        
        std::string output_buffer = m_config->default_color_regular;
        switch (m_current_field)
        {
            case 'A': // User Name
                changeInputModule(MOD_USER_FIELD);
                displayPrompt(PROMPT_USER_FIELD_USERNAME);
                m_session_io.getInputField("", key, Config::sName_length, m_loaded_user.back()->sHandle);
                break;
                
            case 'M': // User Level
                changeInputModule(MOD_USER_FIELD);
                displayPrompt(PROMPT_USER_FIELD_USERLEVEL);
                m_session_io.getInputField("", key, Config::sName_length, std::to_string(m_loaded_user.back()->iLevel));
                break;
        
            case 'B': // User Real Name
                changeInputModule(MOD_USER_FIELD);
                displayPrompt(PROMPT_USER_FIELD_REALNAME);
                m_session_io.getInputField("", key, Config::sName_length, m_loaded_user.back()->sRealName);
                break;
            
            case 'N': // User File Level
                changeInputModule(MOD_USER_FIELD);
                displayPrompt(PROMPT_USER_FIELD_FILELEVEL);
                m_session_io.getInputField("", key, Config::sName_length, std::to_string(m_loaded_user.back()->iFileLevel));
                break;
               
            case 'C': // User Email
                changeInputModule(MOD_USER_FIELD);
                displayPrompt(PROMPT_USER_FIELD_EMAIL);
                m_session_io.getInputField("", key, Config::sName_length, m_loaded_user.back()->sEmail);
                break;
                
            case 'O': // User Message Level
                changeInputModule(MOD_USER_FIELD);
                displayPrompt(PROMPT_USER_FIELD_MESGLEVEL);
                m_session_io.getInputField("", key, Config::sName_length, std::to_string(m_loaded_user.back()->iMessageLevel));
                break;
            
            case 'D': // User Address
                changeInputModule(MOD_USER_FIELD);
                displayPrompt(PROMPT_USER_FIELD_ADDRESS);
                m_session_io.getInputField("", key, Config::sName_length, m_loaded_user.back()->sAddress);
                break;
                
            case 'P': // Numkber Hack Attempts
                changeInputModule(MOD_USER_FIELD);
                displayPrompt(PROMPT_USER_FIELD_HACKATTEMPT);
                m_session_io.getInputField("", key, Config::sName_length, std::to_string(m_loaded_user.back()->iHackAttempts));
                break;
           
            case 'E': // User Location
                changeInputModule(MOD_USER_FIELD);
                displayPrompt(PROMPT_USER_FIELD_LOCATION);
                m_session_io.getInputField("", key, Config::sName_length, m_loaded_user.back()->sLocation);
                break;
                
            case 'R': // Ignore Time Limit
            {
                changeInputModule(MOD_USER_FIELD);
                displayPrompt(PROMPT_USER_FIELD_NOTIMELIMIT);
                // Setup pre-population to display only T or F instead of True / False
                std::string bool_value = "";
                bool_value += m_common_io.boolAlpha(m_loaded_user.back()->bIgnoreTimeLimit).at(0);
                m_session_io.getInputField("", key, Config::sName_length, bool_value);
                break;
            }
            case 'F': // User Country
                changeInputModule(MOD_USER_FIELD);
                displayPrompt(PROMPT_USER_FIELD_COUNTRY);
                m_session_io.getInputField("", key, Config::sName_length, m_loaded_user.back()->sCountry);
                break;
                
            case 'S': // Use ANSI Graphics
            {
                changeInputModule(MOD_USER_FIELD);
                displayPrompt(PROMPT_USER_FIELD_USEANSI);
                // Setup pre-population to display only T or F instead of True / False
                std::string bool_value = "";
                bool_value += m_common_io.boolAlpha(m_loaded_user.back()->bAnsi).at(0);
                m_session_io.getInputField("", key, Config::sName_length, bool_value);
                break;
            }
            case 'G': // User Note
                changeInputModule(MOD_USER_FIELD);
                displayPrompt(PROMPT_USER_FIELD_USERNOTE);
                m_session_io.getInputField("", key, Config::sName_length, m_loaded_user.back()->sUserNote);
                break;
                
            case 'T': // Use VT100 Backspace
            {
                changeInputModule(MOD_USER_FIELD);
                displayPrompt(PROMPT_USER_FIELD_BACKSPACE);
                // Setup pre-population to display only T or F instead of True / False
                std::string bool_value = "";
                bool_value += m_common_io.boolAlpha(m_loaded_user.back()->bBackSpaceVt100).at(0);
                m_session_io.getInputField("", key, Config::sName_length, bool_value);
                break;
            }            
            case 'H': // User Birth Date
                changeInputModule(MOD_USER_FIELD);
                displayPrompt(PROMPT_USER_FIELD_BIRTHDATE);
                m_session_io.getInputField("", key, Config::sName_length, m_common_io.standardDateToString(m_loaded_user.back()->dtBirthday));
                break;
                
            case 'U': // User Wanted
            {
                changeInputModule(MOD_USER_FIELD);
                displayPrompt(PROMPT_USER_FIELD_WANTED);
                // Setup pre-population to display only T or F instead of True / False
                std::string bool_value = "";
                bool_value += m_common_io.boolAlpha(m_loaded_user.back()->bWanted).at(0);
                m_session_io.getInputField("", key, Config::sName_length, bool_value);
                break;
            }
            case 'I': // Access Restriction Flags 1, Type Letter to Add / Remove only.  
                // No leadoff data, just input single letters to add or remove
                changeInputModule(MOD_USER_FIELD);
                displayPrompt(PROMPT_USER_FIELD_FLAGS1);
                m_session_io.getInputField("", key, Config::sName_length);
                break;

            case 'V': // Clear Screen or Scroll Screen
            {
                changeInputModule(MOD_USER_FIELD);
                displayPrompt(PROMPT_USER_FIELD_CLEARSCREEN);
                // Setup pre-population to display only T or F instead of True / False
                std::string bool_value = "";
                bool_value += m_common_io.boolAlpha(m_loaded_user.back()->bClearOrScroll).at(0);
                m_session_io.getInputField("", key, Config::sName_length, bool_value);
                break;
            }
            case 'J': // Access Restriction Flags 2, Type Letter to Add / Remove only.  
                // No leadoff data, just input single letters to add or remove 
                changeInputModule(MOD_USER_FIELD);
                displayPrompt(PROMPT_USER_FIELD_FLAGS2);
                m_session_io.getInputField("", key, Config::sName_length);
                break;

            case 'W': // Do Screen Pause
            {
                changeInputModule(MOD_USER_FIELD);
                displayPrompt(PROMPT_USER_FIELD_SCREENPAUSE);
                // Setup pre-population to display only T or F instead of True / False
                std::string bool_value = "";
                bool_value += m_common_io.boolAlpha(m_loaded_user.back()->bDoPause).at(0);
                m_session_io.getInputField("", key, Config::sName_length, bool_value);
                break;        
            }   
            case 'Q': // Quit
                // TODO, save user, then update m_users_list array with update also.
                //saveMenuChanges();
                std::vector<user_ptr>().swap(m_loaded_user);
                
                // Reload fall back, or gosub to Menu Editor Main
                changeInputModule(MOD_USER_INPUT);
                changeSetupModule(MOD_DISPLAY_USER_LIST);                
                return;
                
            case 'X': // Exit without Saving
                std::vector<user_ptr>().swap(m_loaded_user);
                
                // Reload fall back, or gosub to Menu Editor Main
                changeInputModule(MOD_USER_INPUT);
                changeSetupModule(MOD_DISPLAY_USER_LIST);                
                return;
                
            case '-': // Extended User Information
                changeInputModule(MOD_USER_EXTENDED_FIELD_INPUT);
                changeSetupModule(MOD_DISPLAY_USER_EXTENDED_FIELDS);
                break;
            
            default:
                redisplayModulePrompt();
                break;
        }
    }
    else
    {
        // Send back the single input received to show client key presses.
        // Only if return data shows a processed key returned.
        if (result != "empty") 
        {
            baseProcessDeliverInput(result);
        }
    }
}   

/**
 * @brief Handles User Extended Field Editor Command Selection
 * @param input
 */
void ModUserEditor::userEditorExtendedInput(const std::string &input)
{    
    std::string key = "";
    std::string result = m_session_io.getInputField(input, key, Config::sSingle_key_length);

    // ESC was hit
    if(result == "aborted") 
    {
        std::cout << "aborted!" << std::endl;
        return;
    }
    else if(result[0] == '\n')
    {            
        // Key == 0 on [ENTER] pressed alone. then invalid!
        if(key.size() == 0)
        {
            // Return and don't do anything.
            return;
        }
                
        baseProcessDeliverNewLine();        
        m_current_field = toupper(key[0]);
        
        std::string output_buffer = m_config->default_color_regular;
        switch (m_current_field)
        {            
            case 'A': // Password
                changeInputModule(MOD_USER_EXTENDED_FIELD);
                displayPrompt(PROMPT_USER_EXT_FIELDS_PASSWORD);
                m_session_io.getInputField("", key, Config::sName_length);
                break;
                
            case 'M': // Gender
                changeInputModule(MOD_USER_EXTENDED_FIELD);
                displayPrompt(PROMPT_USER_EXT_FIELDS_GENDER);
                m_session_io.getInputField("", key, Config::sName_length, m_loaded_user.back()->sGender);
                break;
                
            case 'B': // Password Last change date
                changeInputModule(MOD_USER_EXTENDED_FIELD);
                displayPrompt(PROMPT_USER_EXT_FIELDS_PASSCHANGE_DATE);
                m_session_io.getInputField("", key, Config::sName_length, m_common_io.standardDateToString(m_loaded_user.back()->dtFirstOn));
                break;
                
            case 'N': // Days to Force Password change
                changeInputModule(MOD_USER_EXTENDED_FIELD);
                displayPrompt(PROMPT_USER_EXT_FIELDS_PASS_FORCE_CHANCE);
                m_session_io.getInputField("", key, Config::sName_length, std::to_string(m_loaded_user.back()->iCSPassChange));
                break;
                
            case 'C': // Signup Date / First On
                changeInputModule(MOD_USER_EXTENDED_FIELD);
                displayPrompt(PROMPT_USER_EXT_FIELDS_FIRSTON_DATE);
                m_session_io.getInputField("", key, Config::sName_length, m_common_io.standardDateToString(m_loaded_user.back()->dtPassChangeDate));
                break;
                
            case 'O': // File Points
                changeInputModule(MOD_USER_EXTENDED_FIELD);
                displayPrompt(PROMPT_USER_EXT_FIELDS_FILEPOINTS);
                m_session_io.getInputField("", key, Config::sName_length, std::to_string(m_loaded_user.back()->iFilePoints));
                break;
                
            case 'D': // Expiratrion Date
                changeInputModule(MOD_USER_EXTENDED_FIELD);
                displayPrompt(PROMPT_USER_EXT_FIELDS_EXPIRE_DATE);
                m_session_io.getInputField("", key, Config::sName_length, m_common_io.standardDateToString(m_loaded_user.back()->dtExpirationDate));
                break;
                
            case 'P': // Post/Call Ratio
                changeInputModule(MOD_USER_EXTENDED_FIELD);
                displayPrompt(PROMPT_USER_EXT_FIELDS_POSTCALL_RATIO);
                m_session_io.getInputField("", key, Config::sName_length, std::to_string(m_loaded_user.back()->iPostCallRatio));
                break;
                
            case 'E': // Time Limit
                changeInputModule(MOD_USER_EXTENDED_FIELD);
                displayPrompt(PROMPT_USER_EXT_FIELDS_TIME_LIMIT);
                m_session_io.getInputField("", key, Config::sName_length, std::to_string(m_loaded_user.back()->iTimeLimit));
                break;
                
            case 'R': // Time Left Today
                changeInputModule(MOD_USER_EXTENDED_FIELD);
                displayPrompt(PROMPT_USER_EXT_FIELDS_TIME_LEFT);
                m_session_io.getInputField("", key, Config::sName_length, std::to_string(m_loaded_user.back()->iTimeLeft));
                break;
        
            case 'F': // New User Voting (YES) 
                changeInputModule(MOD_USER_EXTENDED_FIELD);
                displayPrompt(PROMPT_USER_EXT_FIELDS_NUV_YESVOTES);
                m_session_io.getInputField("", key, Config::sName_length, std::to_string(m_loaded_user.back()->iNuvVotesYes));
                break;
                
            case 'S': // New User Voting (NO)
                changeInputModule(MOD_USER_EXTENDED_FIELD);
                displayPrompt(PROMPT_USER_EXT_FIELDS_NUV_NOVOTES);
                m_session_io.getInputField("", key, Config::sName_length, std::to_string(m_loaded_user.back()->iNuvVotesNo));
                break;
                
            case 'G': // Regular Color
                changeInputModule(MOD_USER_EXTENDED_FIELD);
                displayPrompt(PROMPT_USER_EXT_FIELDS_REGULAR_COLOR);
                m_session_io.getInputField("", key, Config::sName_length, m_loaded_user.back()->sRegColor);
                break;
                
            case 'T': // Input Color
                changeInputModule(MOD_USER_EXTENDED_FIELD);
                displayPrompt(PROMPT_USER_EXT_FIELDS_INPUT_COLOR);
                m_session_io.getInputField("", key, Config::sName_length, m_loaded_user.back()->sInputColor);
                break;
              
            case 'H': // Prompt Color
                changeInputModule(MOD_USER_EXTENDED_FIELD);
                displayPrompt(PROMPT_USER_EXT_FIELDS_PROMPT_COLOR);
                m_session_io.getInputField("", key, Config::sName_length, m_loaded_user.back()->sPromptColor);
                break;
                
            case 'U': // Box Color
                changeInputModule(MOD_USER_EXTENDED_FIELD);
                displayPrompt(PROMPT_USER_EXT_FIELDS_BOX_COLOR);
                m_session_io.getInputField("", key, Config::sName_length, m_loaded_user.back()->sBoxColor);
                break;
    
            case 'I': // Prompt Color
                changeInputModule(MOD_USER_EXTENDED_FIELD);
                displayPrompt(PROMPT_USER_EXT_FIELDS_STATUS_COLOR);                
                m_session_io.getInputField("", key, Config::sName_length, m_loaded_user.back()->sStatColor);
                break;
                
            case 'V': // Box Color
                changeInputModule(MOD_USER_EXTENDED_FIELD);
                displayPrompt(PROMPT_USER_EXT_FIELDS_INVERSE_COLOR);
                m_session_io.getInputField("", key, Config::sName_length, m_loaded_user.back()->sInverseColor);
                break;


            case 'Q': // Quit
                // Return to previous screen
                changeInputModule(MOD_USER_FIELD_INPUT);
                changeSetupModule(MOD_DISPLAY_USER_FIELDS);                
                return;
                                            
            default:
                redisplayModulePrompt();
                break;
        }
    }
    else
    {
        // Send back the single input received to show client key presses.
        // Only if return data shows a processed key returned.
        if (result != "empty") 
        {
            baseProcessDeliverInput(result);
        }
    }
}

/**
 * @brief Handles Field Updates for User Data
 * @param input
 */
void ModUserEditor::userEditorFieldHandler(const std::string &input)
{
    std::string key = "";
    std::string result = m_session_io.getInputField(input, key, Config::sName_length);

    // ESC was hit
    if(result == "aborted") 
    {
        std::cout << "aborted!" << std::endl;
        changeInputModule(MOD_USER_FIELD_INPUT);
        changeSetupModule(MOD_DISPLAY_USER_FIELDS);
        return;
    }
    else if(result[0] == '\n')
    {            
        baseProcessDeliverNewLine();

        // Handle the assigned input received for field
        switch(m_current_field)
        {
            case 'A': // User Name
                m_loaded_user.back()->sHandle = key;
                break;                     
                
            case 'M': // User Level
                m_loaded_user.back()->iLevel = m_common_io.stringToInt(key);
                break;
        
            case 'B': // User Real Name
                m_loaded_user.back()->sRealName = key;
                break;
            
            case 'N': // User File Level
                m_loaded_user.back()->iFileLevel = m_common_io.stringToInt(key);
                break;
               
            case 'C': // User Email
                m_loaded_user.back()->sEmail = key;
                break;
                
            case 'O': // User Message Level                
                m_loaded_user.back()->iMessageLevel = m_common_io.stringToInt(key);
                break;
            
            case 'D': // User Address
                m_loaded_user.back()->sAddress = key;
                break;
                
            case 'P': // Numkber Hack Attempts
                m_loaded_user.back()->iHackAttempts = m_common_io.stringToInt(key);
                break;
           
            case 'E': // User Location
                m_loaded_user.back()->sLocation = key;
                break;
                
            case 'R': // Ignore Time Limit                
                if (m_common_io.stringToBool(key) != -1)
                    m_loaded_user.back()->bIgnoreTimeLimit = m_common_io.stringToBool(key);
                break;

            case 'F': // User Country
                m_loaded_user.back()->sCountry = key;
                break;
                
            case 'S': // Use ANSI Graphics
                if (m_common_io.stringToBool(key) != -1)
                    m_loaded_user.back()->bAnsi = m_common_io.stringToBool(key);
                break;

            case 'G': // User Note
                m_loaded_user.back()->sUserNote = key;
                break;
                
            case 'T': // Use VT100 Backspace
                if (m_common_io.stringToBool(key) != -1)
                    m_loaded_user.back()->bBackSpaceVt100 = m_common_io.stringToBool(key);
                break;

            case 'H': // User Birth Date
            {
                // Make sure Date Format is valid
                std::regex date_regex { m_config->regexp_date_validation };
                std::smatch str_matches;

                // If invalid display message, but for now ignore changes
                if(std::regex_match(key, str_matches, date_regex))
                {
                    m_loaded_user.back()->dtBirthday = m_common_io.stringToStandardDate(key);
                }
                break;
            }    
            case 'U': // User Wanted
                if (m_common_io.stringToBool(key) != -1)
                    m_loaded_user.back()->bWanted = m_common_io.stringToBool(key);
                break;

            case 'I': // Access Restriction Flags 1, Will loop though and toggle each letter.
            {
                AccessCondition acs;
                for (char c : key)
                    acs.setFlagToggle(c, true, m_loaded_user.back());
                break;
            }
            case 'V': // Clear Screen or Scroll Screen
                if (m_common_io.stringToBool(key) != -1)
                    m_loaded_user.back()->bClearOrScroll = m_common_io.stringToBool(key);
                break;

            case 'J': // Access Restriction Flags 2, Type Letter to Add / Remove only.  
            {
                AccessCondition acs;
                for (char c : key)
                    acs.setFlagToggle(c, false, m_loaded_user.back());
                break;
            }
            case 'W': // Pause
                if (m_common_io.stringToBool(key) != -1)
                    m_loaded_user.back()->bDoPause = m_common_io.stringToBool(key);
                break;                                         
        }
                
        changeInputModule(MOD_USER_FIELD_INPUT);
        changeSetupModule(MOD_DISPLAY_USER_FIELDS);
    }
    else
    {
        // Send back the single input received to show client key presses.
        // Only if return data shows a processed key returned.
        if (result != "empty") 
        {
            baseProcessDeliverInput(result);
        }
    }
}

/**
 * @brief Updates an existing password index.
 * @param key_value
 */
void ModUserEditor::updateExistingPassword(std::string key_value)
{
    // Setup for default password and challenge questions.
    encrypt_ptr encryption(new Encrypt());
    std::string salt = encryption->generate_salt(m_loaded_user.back()->sHandle, m_config->bbs_uuid);
    std::string password = encryption->generate_password(key_value, salt);

    if(salt.size() == 0 || password.size() == 0)
    {
        std::cout << "Error, Salt or Password were empty" << std::endl;
        assert(false);
    }
    
    security_dao_ptr security_dao(new SecurityDao(m_session_data->m_user_database));
    security_ptr security_record(new Security());
    
    security_record = security_dao->getRecordById(m_loaded_user.back()->iSecurityIndex);    
    security_record->sPasswordHash = password;
    security_record->sSaltHash = salt;
        
    if (!security_dao->updateRecord(security_record))
    {
        std::cout << "Error, unable to update password hash." << std::endl;
        return;
    }
}
   
/**
 * @brief Handles Extended Field Updates for User Data
 * @param input
 */
void ModUserEditor::userEditorExtendedFieldHandler(const std::string &input)
{
    std::string key = "";
    std::string result = m_session_io.getInputField(input, key, Config::sName_length);

    // ESC was hit
    if(result == "aborted") 
    {
        std::cout << "aborted!" << std::endl;
        changeInputModule(MOD_USER_FIELD_INPUT);
        changeSetupModule(MOD_DISPLAY_USER_FIELDS);
        return;
    }
    else if(result[0] == '\n')
    {            
        baseProcessDeliverNewLine();

        // Handle the assigned input received for field
        switch(m_current_field)
        {
            case 'A': // Password
                updateExistingPassword(key);
                break;
                
            case 'M': // Gender
                baseTransformToUpper(key);
                m_loaded_user.back()->sGender = key;
                break;
                
            case 'B': // Password Last change date
            {
                // Make sure Date Format is valid
                std::regex date_regex { m_config->regexp_date_validation };
                std::smatch str_matches;

                // If invalid display message, but for now ignore changes
                if(std::regex_match(key, str_matches, date_regex))
                {
                    m_loaded_user.back()->dtFirstOn = m_common_io.stringToStandardDate(key);
                }
                break;
            }
            case 'N': // Days to Force Password change
                m_loaded_user.back()->iCSPassChange = m_common_io.stringToInt(key);
                break;
                
            case 'C': // Signup Date / First On
            {
                // Make sure Date Format is valid
                std::regex date_regex { m_config->regexp_date_validation };
                std::smatch str_matches;

                // If invalid display message, but for now ignore changes
                if(std::regex_match(key, str_matches, date_regex))
                {
                    m_loaded_user.back()->dtPassChangeDate = m_common_io.stringToStandardDate(key);
                }
                break;
            }
            case 'O': // File Points
                m_loaded_user.back()->iFilePoints = m_common_io.stringToInt(key);
                break;
                
            case 'D': // Expiratrion Date
            {
                // Make sure Date Format is valid
                std::regex date_regex { m_config->regexp_date_validation };
                std::smatch str_matches;

                // If invalid display message, but for now ignore changes
                if(std::regex_match(key, str_matches, date_regex))
                {
                    m_loaded_user.back()->dtExpirationDate = m_common_io.stringToStandardDate(key);
                }
                break;
            }
            case 'P': // Post/Call Ratio
                m_loaded_user.back()->iPostCallRatio = m_common_io.stringToInt(key);
                break;
                
            case 'E': // Time Limit
                m_loaded_user.back()->iTimeLimit = m_common_io.stringToInt(key);
                break;
                
            case 'R': // Time Left Today
                m_loaded_user.back()->iTimeLeft = m_common_io.stringToInt(key);
                break;
        
            case 'F': // New User Voting (YES) 
                m_loaded_user.back()->iNuvVotesYes = m_common_io.stringToInt(key);
                break;
                
            case 'S': // New User Voting (NO)
                m_loaded_user.back()->iNuvVotesNo = m_common_io.stringToInt(key);
                break;
                
            case 'G': // Regular Color
                m_loaded_user.back()->sRegColor = key;
                break;
                
            case 'T': // Input Color
                m_loaded_user.back()->sInputColor = key;
                break;
              
            case 'H': // Prompt Color
                m_loaded_user.back()->sPromptColor = key;
                break;
                
            case 'U': // Box Color
                m_loaded_user.back()->sBoxColor = key;
                break;
    
            case 'I': // Prompt Color
                m_loaded_user.back()->sStatColor = key;
                break;
                
            case 'V': // Box Color
                m_loaded_user.back()->sInverseColor = key;
                break;                                                            
        }
                
        changeInputModule(MOD_USER_FIELD_INPUT);
        changeSetupModule(MOD_DISPLAY_USER_FIELDS);
    }
    else
    {
        // Send back the single input received to show client key presses.
        // Only if return data shows a processed key returned.
        if (result != "empty") 
        {
            baseProcessDeliverInput(result);
        }
    }
}

/**
 * @brief User Editor, for Displaying User Fields to Edit
 * @return
 */
std::string ModUserEditor::displayUserEditScreen()
{
    // Create Menu Pointer then load the menu into it.
    if (!loadUserById(m_current_user_id) || m_loaded_user.back()->iId == -1)
    {
        // Error, drop back to User Editor User Listing
        std::vector<user_ptr>().swap(m_loaded_user);
        changeInputModule(MOD_USER_FIELD_INPUT);
        changeSetupModule(MOD_DISPLAY_USER_LIST);        
        return "";
    }
    
    // Build a string list of individual menu options, then loop to fit as many per screen!
    std::vector<std::string> result_set;

    AccessCondition acs;
    user_ptr usr = m_loaded_user.back();
            
    result_set.push_back(m_common_io.rightPadding(getDisplayPromptRaw(DISPLAY_USER_FIELDS_USERNAME) + baseGetDefaultStatColor() + usr->sHandle, 64) + 
        m_common_io.rightPadding(getDisplayPromptRaw(DISPLAY_USER_FIELDS_LEVEL) + baseGetDefaultStatColor() + std::to_string(usr->iLevel), 48));
        
    result_set.push_back(m_common_io.rightPadding(getDisplayPromptRaw(DISPLAY_USER_FIELDS_REALNAME) + baseGetDefaultStatColor() + usr->sRealName, 64) + 
        m_common_io.rightPadding(getDisplayPromptRaw(DISPLAY_USER_FIELDS_FILELEVEL) + baseGetDefaultStatColor() + std::to_string(usr->iFileLevel), 48));
        
    result_set.push_back(m_common_io.rightPadding(getDisplayPromptRaw(DISPLAY_USER_FIELDS_EMAIL) + baseGetDefaultStatColor() + usr->sEmail, 64) + 
        m_common_io.rightPadding(getDisplayPromptRaw(DISPLAY_USER_FIELDS_MESGLEVEL) + baseGetDefaultStatColor() + std::to_string(usr->iMessageLevel), 48));
        
    result_set.push_back(m_common_io.rightPadding(getDisplayPromptRaw(DISPLAY_USER_FIELDS_ADDRESS) + baseGetDefaultStatColor() + usr->sAddress, 64) + 
        m_common_io.rightPadding(getDisplayPromptRaw(DISPLAY_USER_FIELDS_HACK_ATTEMPT) + baseGetDefaultStatColor() + std::to_string(usr->iHackAttempts), 48));
        
    result_set.push_back(m_common_io.rightPadding(getDisplayPromptRaw(DISPLAY_USER_FIELDS_LOCATION) + baseGetDefaultStatColor() + usr->sLocation, 64) + 
        m_common_io.rightPadding(getDisplayPromptRaw(DISPLAY_USER_FIELDS_NO_TIMELIMIT) + baseGetDefaultStatColor() + m_common_io.boolAlpha(usr->bIgnoreTimeLimit), 48));
        
    result_set.push_back(m_common_io.rightPadding(getDisplayPromptRaw(DISPLAY_USER_FIELDS_COUNTRY) + baseGetDefaultStatColor() + usr->sCountry, 64) + 
        m_common_io.rightPadding(getDisplayPromptRaw(DISPLAY_USER_FIELDS_USE_ANSI) + baseGetDefaultStatColor() + m_common_io.boolAlpha(usr->bAnsi), 48));
        
    result_set.push_back(m_common_io.rightPadding(getDisplayPromptRaw(DISPLAY_USER_FIELDS_USERNOTE) + baseGetDefaultStatColor() + usr->sUserNote, 64) + 
        m_common_io.rightPadding(getDisplayPromptRaw(DISPLAY_USER_FIELDS_USE_VTBACKSPACE) + baseGetDefaultStatColor() + m_common_io.boolAlpha(usr->bBackSpaceVt100), 48));
           
    result_set.push_back(m_common_io.rightPadding(getDisplayPromptRaw(DISPLAY_USER_FIELDS_BIRTHDATE) + baseGetDefaultStatColor() + m_common_io.standardDateToString(usr->dtBirthday), 64) + 
        m_common_io.rightPadding(getDisplayPromptRaw(DISPLAY_USER_FIELDS_WANTED) + baseGetDefaultStatColor() + m_common_io.boolAlpha(usr->bWanted), 48));
        
    result_set.push_back(m_common_io.rightPadding(getDisplayPromptRaw(DISPLAY_USER_FIELDS_FLAGS1) + baseGetDefaultStatColor() + acs.getAccessConditionFlagStringFromBits(usr->iControlFlags1), 64) +
        m_common_io.rightPadding(getDisplayPromptRaw(DISPLAY_USER_FIELDS_CLEAR_OR_SCROLL) + baseGetDefaultStatColor() + m_common_io.boolAlpha(usr->bClearOrScroll), 48));
        
    result_set.push_back(m_common_io.rightPadding(getDisplayPromptRaw(DISPLAY_USER_FIELDS_FLAGS2) + baseGetDefaultStatColor() + acs.getAccessConditionFlagStringFromBits(usr->iControlFlags2), 64) +
        m_common_io.rightPadding(getDisplayPromptRaw(DISPLAY_USER_FIELDS_SCREEN_PAUSE) + baseGetDefaultStatColor() + m_common_io.boolAlpha(usr->bDoPause), 48));
                        
    result_set.push_back(baseGetDefaultPromptColor() + " " + std::string(72, BORDER_ROW) + " ");
    result_set.push_back(getDisplayPromptRaw(DISPLAY_USER_FIELDS_EXTENDED_DETAILS) + m_common_io.rightPadding("", 48));
    result_set.push_back(getDisplayPromptRaw(DISPLAY_USER_FIELDS_PREVIOUS_USER) + m_common_io.rightPadding("", 48));
    result_set.push_back(getDisplayPromptRaw(DISPLAY_USER_FIELDS_NEXT_USER) + m_common_io.rightPadding("", 48));    
    result_set.push_back(baseGetDefaultPromptColor() + " " + std::string(72, BORDER_ROW) + " ");
    result_set.push_back(getDisplayPromptRaw(DISPLAY_USER_FIELDS_QUIT_SAVE) + m_common_io.rightPadding("", 48));
    result_set.push_back(getDisplayPromptRaw(DISPLAY_USER_FIELDS_QUIT_ABORT) + m_common_io.rightPadding("", 48));

    // iterate through and print out
    int total_rows = result_set.size();
    total_rows += 2;

    // Could re-calc this on screen width lateron.
    int max_cols = 76;

    // Vector or Menus, Loop through
    std::vector<std::string>::iterator i = result_set.begin();
    std::string buffer = "";
    
    for(int rows = 0; rows < total_rows; rows++)
    {
        buffer += "  "; // 3 Leading spaces per row.
        for(int cols = 0; cols < max_cols; cols++)
        {
            // Top Row
            if(rows == 0 && cols == 0)
            {
                buffer += baseGetDefaultBoxColor();
                buffer += BORDER_TOP_LEFT;
            }
            else if(rows == 0 && cols == max_cols-1)
            {
                buffer += baseGetDefaultBoxColor();
                buffer += BORDER_TOP_RIGHT;
            }
            else if(rows == 0 && cols % 75 == 0)
            {
                buffer += baseGetDefaultBoxColor();
                buffer += BORDER_MID_TOP;
            } 
            else if(rows == 0)
            {
                buffer += baseGetDefaultBoxColor();
                buffer += BORDER_ROW;
            }

            // Bottom Row
            else if(rows == total_rows-1 && cols == 0)
            {
                buffer += baseGetDefaultBoxColor();
                buffer += BORDER_BOT_LEFT;
            }
            else if(rows == total_rows-1 && cols == max_cols-1)
            {
                buffer += baseGetDefaultBoxColor();
                buffer += BORDER_BOT_RIGHT;
            }
            else if(rows == total_rows-1 && cols % 75 == 0)
            {
                buffer += baseGetDefaultBoxColor();
                buffer += BORDER_MID_BOT;
            } 
            else if(rows == total_rows-1)
            {
                buffer += baseGetDefaultBoxColor();
                buffer += BORDER_ROW;
            }
            else if(cols % 75 == 0)
            {
                buffer += baseGetDefaultBoxColor();
                buffer += BORDER_MID;
            }
            else
            {
                // Here we insert the Menu name and pad through to 8 characters.
                if(cols == 1)
                {
                    if(i != result_set.end())
                    {
                        buffer += *i;
                        ++i;
                    }
                }
            }
        }
        
        // Were going to split on \n, which will get replaced lateron
        // with \r\n for full carriage returns.        
        buffer += "\n";
    }
    
    return (buffer);    
}

/**
 * @brief User Editor, for Displaying User Extended Fields to Edit
 * @return
 */
std::string ModUserEditor::displayUserExtendedEditScreen()
{
    // Create Menu Pointer then load the menu into it.
    if (!loadUserById(m_current_user_id) || m_loaded_user.back()->iId == -1)
    {
        // Error, drop back to User Editor User Listing
        std::vector<user_ptr>().swap(m_loaded_user);
        changeInputModule(MOD_USER_FIELD_INPUT);
        changeSetupModule(MOD_DISPLAY_USER_LIST);        
        return "";
    }
    
    // Build a string list of individual menu options, then loop to fit as many per screen!
    std::vector<std::string> result_set;

    AccessCondition acs;
    user_ptr usr = m_loaded_user.back();
        
    // Transform Gender to UpperCase
    std::string gender_string = usr->sGender;
    baseTransformToUpper(gender_string);
            
    // Initial Configuration fields, not all interfaces are implemented so some things 
    // will be left off for now.
    result_set.push_back(m_common_io.rightPadding(getDisplayPromptRaw(DISPLAY_USER_EXT_FIELDS_PASSWORD) + baseGetDefaultStatColor() + "****** Masked ", 64) + 
        m_common_io.rightPadding(getDisplayPromptRaw(DISPLAY_USER_EXT_FIELDS_GENDER) + baseGetDefaultStatColor() + gender_string, 48));
        
    result_set.push_back(m_common_io.rightPadding(getDisplayPromptRaw(DISPLAY_USER_EXT_FIELDS_PASSCHANGE_DATE) + baseGetDefaultStatColor() + m_common_io.standardDateToString(usr->dtPassChangeDate), 64) + 
        m_common_io.rightPadding(getDisplayPromptRaw(DISPLAY_USER_EXT_FIELDS_PASS_FORCE_CHANCE) + baseGetDefaultStatColor() + std::to_string(usr->iCSPassChange), 48));
               
    result_set.push_back(m_common_io.rightPadding(getDisplayPromptRaw(DISPLAY_USER_EXT_FIELDS_FIRSTON_DATE) + baseGetDefaultStatColor() + m_common_io.standardDateToString(usr->dtFirstOn), 64) + 
        m_common_io.rightPadding(getDisplayPromptRaw(DISPLAY_USER_EXT_FIELDS_FILEPOINTS) + baseGetDefaultStatColor() + std::to_string(usr->iFilePoints), 48));
        
    // If Expiration Date Set to Minimun, then set as default None
    std::string expiration_date_string = "None";
    if (m_common_io.standardDateToString(usr->dtExpirationDate) != "1969-12-31")
        expiration_date_string = m_common_io.standardDateToString(usr->dtExpirationDate);
        
    result_set.push_back(m_common_io.rightPadding(getDisplayPromptRaw(DISPLAY_USER_EXT_FIELDS_EXPIRE_DATE) + baseGetDefaultStatColor() + expiration_date_string, 64) + 
        m_common_io.rightPadding(getDisplayPromptRaw(DISPLAY_USER_EXT_FIELDS_POSTCALL_RATIO) + baseGetDefaultStatColor() + std::to_string(usr->iPostCallRatio), 48));
    
    result_set.push_back(m_common_io.rightPadding(getDisplayPromptRaw(DISPLAY_USER_EXT_FIELDS_TIME_LIMIT) + baseGetDefaultStatColor() + std::to_string(usr->iTimeLimit), 64) + 
        m_common_io.rightPadding(getDisplayPromptRaw(DISPLAY_USER_EXT_FIELDS_TIME_LEFT) + baseGetDefaultStatColor() + std::to_string(usr->iTimeLeft), 48));
        
    result_set.push_back(m_common_io.rightPadding(getDisplayPromptRaw(DISPLAY_USER_EXT_FIELDS_NUV_YESVOTES) + baseGetDefaultStatColor() + std::to_string(usr->iNuvVotesYes), 64) + 
        m_common_io.rightPadding(getDisplayPromptRaw(DISPLAY_USER_EXT_FIELDS_NUV_NOVOTES) + baseGetDefaultStatColor() + std::to_string(usr->iNuvVotesNo), 48));        
        
    result_set.push_back(m_common_io.rightPadding(getDisplayPromptRaw(DISPLAY_USER_EXT_FIELDS_REGULAR_COLOR) + usr->sRegColor + "***|16", 63) + 
        m_common_io.rightPadding(getDisplayPromptRaw(DISPLAY_USER_EXT_FIELDS_INPUT_COLOR) + usr->sInputColor + "***|16", 47));
        
    result_set.push_back(m_common_io.rightPadding(getDisplayPromptRaw(DISPLAY_USER_EXT_FIELDS_PROMPT_COLOR) + usr->sPromptColor + "***|16", 63) + 
        m_common_io.rightPadding(getDisplayPromptRaw(DISPLAY_USER_EXT_FIELDS_BOX_COLOR) + usr->sBoxColor + "***|16", 47));
        
    result_set.push_back(m_common_io.rightPadding(getDisplayPromptRaw(DISPLAY_USER_EXT_FIELDS_STATUS_COLOR) + usr->sStatColor + "***|16", 63) + 
        m_common_io.rightPadding(getDisplayPromptRaw(DISPLAY_USER_EXT_FIELDS_INVERSE_COLOR) + usr->sInverseColor + "***|16" , 47));
        
    result_set.push_back(baseGetDefaultPromptColor() + " " + std::string(72, BORDER_ROW) + " ");
    result_set.push_back(getDisplayPromptRaw(DISPLAY_USER_EXT_FIELDS_QUIT_RETURN) + m_common_io.rightPadding("", 48));
      
    // iterate through and print out
    int total_rows = result_set.size();
    total_rows += 2;

    // Could re-calc this on screen width lateron.
    int max_cols = 76;

    // Vector or Menus, Loop through
    std::vector<std::string>::iterator i = result_set.begin();
    std::string buffer = "";
    
    for(int rows = 0; rows < total_rows; rows++)
    {
        buffer += "  "; // 3 Leading spaces per row.
        for(int cols = 0; cols < max_cols; cols++)
        {
            // Top Row
            if(rows == 0 && cols == 0)
            {
                buffer += baseGetDefaultBoxColor();
                buffer += BORDER_TOP_LEFT;
            }
            else if(rows == 0 && cols == max_cols-1)
            {
                buffer += baseGetDefaultBoxColor();
                buffer += BORDER_TOP_RIGHT;
            }
            else if(rows == 0 && cols % 75 == 0)
            {
                buffer += baseGetDefaultBoxColor();
                buffer += BORDER_MID_TOP;
            } 
            else if(rows == 0)
            {
                buffer += baseGetDefaultBoxColor();
                buffer += BORDER_ROW;
            }

            // Bottom Row
            else if(rows == total_rows-1 && cols == 0)
            {
                buffer += baseGetDefaultBoxColor();
                buffer += BORDER_BOT_LEFT;
            }
            else if(rows == total_rows-1 && cols == max_cols-1)
            {
                buffer += baseGetDefaultBoxColor();
                buffer += BORDER_BOT_RIGHT;
            }
            else if(rows == total_rows-1 && cols % 75 == 0)
            {
                buffer += baseGetDefaultBoxColor();
                buffer += BORDER_MID_BOT;
            } 
            else if(rows == total_rows-1)
            {
                buffer += baseGetDefaultBoxColor();
                buffer += BORDER_ROW;
            }
            else if(cols % 75 == 0)
            {
                buffer += baseGetDefaultBoxColor();
                buffer += BORDER_MID;
            }
            else
            {
                // Here we insert the Menu name and pad through to 8 characters.
                if(cols == 1)
                {
                    if(i != result_set.end())
                    {
                        buffer += *i;
                        ++i;
                    }
                }
            }
        }
        
        // Were going to split on \n, which will get replaced lateron
        // with \r\n for full carriage returns.        
        buffer += "\n";
    }
    
    return (buffer);    
}


/**
 * @brief Displays the current page of user items
 * @param input_state
 */
void ModUserEditor::displayCurrentEditPage(const std::string &input_state) 
{
    for (unsigned int i = 0; i < m_user_display_list.size(); i++) 
    {
        std::string display_line = m_session_io.pipe2ansi(m_user_display_list[i]);
        display_line.append("\r\n");
        baseProcessAndDeliver(display_line);        
    }
    
    // Default Page Input Method
    unsigned int current_module_input;
    switch(m_mod_setup_index)
    {
        case MOD_DISPLAY_USER_FIELDS:
            current_module_input = MOD_USER_FIELD_INPUT;
            break;  
            
        case MOD_DISPLAY_USER_EXTENDED_FIELDS:
            current_module_input = MOD_USER_EXTENDED_FIELD_INPUT;
            break;  

        default:
            std::cout << "Error, forgot to add new STATE index displayCurrentEditPage!!";
            return;
    }  
    
    displayPrompt(input_state); // prompt for edit
    changeInputModule(current_module_input); // switch to the input module for field edits.
}
