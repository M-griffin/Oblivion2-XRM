#ifndef MOD_MESSAGE_EDITOR_HPP
#define MOD_MESSAGE_EDITOR_HPP

#include "mod_base.hpp"

#include <iostream>
#include <string>
#include <memory>
#include <functional>
#include <vector>

class ProcessorText;
typedef std::shared_ptr<ProcessorText> processor_text_ptr;

/**
 * @class ModMessageEditor
 * @author Michael Griffin
 * @date 26/06/2018
 * @file mod_message_editor.hpp
 * @brief Message Editor
 */
class ModMessageEditor
    : public ModBase
{
public:
    ModMessageEditor(session_ptr session_data, config_ptr config, processor_ansi_ptr ansi_process);

    virtual ~ModMessageEditor() override
    {
        std::cout << "~ModMessageEditor()" << std::endl;
        std::vector<std::function< void()> >().swap(m_setup_functions);
        std::vector<std::function< void(const std::string &)> >().swap(m_mod_functions);
    }

    virtual bool update(const std::string &character_buffer, const bool &) override;
    virtual bool onEnter() override;
    virtual bool onExit() override;

    // Setup Methods
    enum
    {
        // Most likely add extra here for asking TOPIC, TAGS, TO etc..
        MOD_DISPLAY_EDITOR      = 0
    };

    // Input Menu State Index
    // Used for both Menus and Options.
    enum
    {
        // Most likely add more for options and titles, tags etc..
        MOD_FSE_INPUT    = 0
    };

    // This matches the index for mod_functions.push_back
    enum
    {
        MOD_PROMPT,
        MOD_ADD,
        MOD_CHANGE,
        MOD_DELETE
    };

    // Create Prompt Constants, these are the keys for key/value lookup
    const std::string DEFAULT_TEXT_COLORS = "default_text_colors";
    const std::string BACKSPACE_TEXT_COLORS = "backspace_text_colors";
    const std::string SELECTED_TEXT_COLORS = "selected_text_colors";

    // Not Used yet.
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
     * @brief Scrub CR LF from Screen Templates
     * @param screen
     */
    void scrubNewLinesChars(std::string &screen);

    /**
     * @brief Processes a TOP Template Screen
     * @param ansi_process
     * @param screen
     * @return
     */
    std::string processTopTemplate(processor_ansi_ptr ansi_process, const std::string &screen);

    /**
     * @brief Processes a MID Template Screen
     * @param ansi_process
     * @param screen
     * @return
     */
    std::string processMidTemplate(processor_ansi_ptr ansi_process, const std::string &screen);

    /**
     * @brief Processes a Bottom Template Screen
     * @param ansi_process
     * @param screen
     * @return
     */
    std::string processBottomTemplate(processor_ansi_ptr ansi_process, const std::string &screen);

    /**
     * @brief Setup for the Message Editor
     * @return
     */
    void setupEditor();

    /**
     * @brief General Input for Message Editor
     * @return
     */
    void editorInput(const std::string &input);

    /**
     * @brief Move to the Cursor to the Buffer Position
     */
    std::string moveCursorToPosition();

    /**
     * @brief Scroll Screen the Text Up
     */
    std::string scrollTextBoxUp(std::string &output);

    /**
     * @brief Scroll Screen the Text Down
     */
    std::string scrollTextBoxDown(std::string &output);

    /**
     * @brief Handle Backspaces
     * @param output
     */
    void handleBackSpace(std::string &output);

    /**
     * @brief Handle Deletes Inline
     * @param output
     */
    void handleDelete(std::string &output);

    /**
     * @brief Handle New Lines
     * @param output
     */
    void handleNewLines(std::string &output);

    /**
     * @brief Process Text Input for Editor
     * @return
     */
    void processTextInput(std::string result, std::string input);

    /**
     * @brief Process Escaped and Control Input Keys for Editor
     * @return
     */
    void processEscapedInput(std::string result, std::string input);


private:

    // Function Input Vector.
    std::vector<std::function< void()> >                    m_setup_functions;
    std::vector<std::function< void(const std::string &)> > m_mod_functions;

    std::string            m_filename;
    text_prompts_dao_ptr   m_text_prompts_dao;
    processor_text_ptr     m_text_process;

    int                    m_mod_function_index;
    int                    m_mod_setup_index;
    int                    m_mod_user_state_index;
    int                    m_failure_attempts;
    bool                   m_is_text_prompt_exist;
    int                    m_text_box_top;
    int                    m_text_box_bottom;
    int                    m_text_box_left;
    int                    m_text_box_right;
    int                    m_text_box_height;
    int                    m_text_box_width;

};

#endif // MOD_MESSAGE_EDITOR_HPP
