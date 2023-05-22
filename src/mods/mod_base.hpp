#ifndef MOD_BASE_HPP
#define MOD_BASE_HPP

#include <memory>
#include <string>
#include <vector>

// Header Type Definitions
class Logging;

class Session;
typedef std::shared_ptr<Session> session_ptr;

class SessionIO;
typedef std::shared_ptr<SessionIO> session_io_ptr;

class ProcessorAnsi;
typedef std::shared_ptr<ProcessorAnsi> processor_ansi_ptr;

class Config;
typedef std::shared_ptr<Config> config_ptr;

class TextPromptsDao;
typedef std::shared_ptr<TextPromptsDao> text_prompts_dao_ptr;

/**
 * @class ModBase
 * @author Michael Griffin
 * @date 3/17/2016
 * @file mod_base.hpp
 * @brief Base Class for Module System
 */
class ModBase
{
public:

    virtual ~ModBase();    
    virtual bool update(const std::string &character_buffer, const bool &is_utf8) = 0;
    virtual bool onEnter() = 0;
    virtual bool onExit()  = 0;

    ModBase(session_ptr session_data, config_ptr config, processor_ansi_ptr ansi_process);       

    // Box drawing characters
    enum
    {
        M_BORDER_TOP_LEFT  = (char)214,  // ╓
        M_BORDER_BOT_LEFT  = (char)211,  // ╙
        M_BORDER_ROW       = (char)196,  // ─
        M_BORDER_TOP_RIGHT = (char)183,  // ╖
        M_BORDER_BOT_RIGHT = (char)189,  // ╜
        M_BORDER_MID_TOP   = (char)210,  // ╥
        M_BORDER_MID_BOT   = (char)208,  // ╨
        M_BORDER_MID       = (char)186   // ║
    };

    /**
     * @brief Translate Box Chars to UTF-8
     * @param enum_value
     */
    std::string baseGetEncodedBoxChar(int enum_value);

    /**
     * @brief Translate Box Chars to UTF-8 with Default box Color
     * @param enum_value
     */
    std::string baseGetEncodedBoxCharAndColor(int enum_value);


    /**
     * @brief Generic Border Screen used buy several interfaces for display.
     * @param result_set
     * @param total_rows
     * @param max_cols
     * @return
     */
    std::string baseCreateBorderedDisplay(std::vector<std::string> result_set, int total_rows, int max_cols);

    /**
     * @brief Transform Strings to Uppercase with Locale
     * @param value
     */
    void baseTransformToUpper(std::string &value);

    /**
     * @brief Transform Strings to Lowercase with Locale
     * @param value
     */
    void baseTransformToLower(std::string &value);

    /**
     * @brief Gets the Default Color Sequence
     * @return
     */
    std::string baseGetDefaultColor();

    /**
     * @brief Gets the Default Input Color Sequence
     * @return
     */
    std::string baseGetDefaultInputColor();

    /**
     * @brief Gets the Default Input Color Sequence
     * @return
     */
    std::string baseGetDefaultInverseColor();

    /**
     * @brief Gets the Default Box Color Sequence
     * @return
     */
    std::string baseGetDefaultBoxColor();

    /**
     * @brief Gets the Default Prompt Color Sequence
     * @return
     */
    std::string baseGetDefaultPromptColor();

    /**
     * @brief Gets the Default Stat Color Sequence
     * @return
     */
    std::string baseGetDefaultStatColor();

    /**
     * @brief Method for Adding outgoing text data to ANSI processor
     *        Then delivering the data to the client
     * @param data
     */
    void baseProcessAndDeliver(std::string &data);

    /**
     * @brief Deliver Output followed with New Line.
     * @param data
     */
    void baseProcessAndDeliverNewLine(std::string &data);

    /**
     * @brief Deliver NewLine for [ENTER] On Prompts.
     */
    void baseProcessDeliverNewLine();

    /**
     * @brief Deliver Input for prompts (No Coloring Extras)
     */
    void baseProcessDeliverInput(std::string &data);

    /**
     * @brief Pull and Display Prompts
     * @param prompt
     */
    void baseDisplayPrompt(const std::string &prompt, text_prompts_dao_ptr m_text_dao);

    /**
     * @brief Pull and Return Display Prompt
     * @param prompt
     */
    std::string baseGetDisplayPrompt(const std::string &prompt, text_prompts_dao_ptr m_text_dao);

    /**
     * @brief Pull and Return Raw Display Prompts
     * @param prompt
     */
    std::string baseGetDisplayPromptRaw(const std::string &prompt, text_prompts_dao_ptr m_text_dao);

    /**
     * @brief Pull and Display Prompts, Replace MCI Code |OT
     * @param prompt
     */
    void baseDisplayPromptMCI(const std::string &prompt, text_prompts_dao_ptr m_text_dao, std::string mci_field);

    /**
     * @brief Pull and Display Prompt with a following new line for info messages.
     * @param prompt
     */
    void baseDisplayPromptAndNewLine(const std::string &prompt, text_prompts_dao_ptr m_text_dao);

    /**
     * @brief Move to End of Display then output
     * @param output
     */
    void moveToBottomAndDisplay(const std::string &prompt);
    

    /**
     * @brief Move to End of Display then Setup Display for String
     * @param output
     */
    std::string moveStringToBottom(const std::string &prompt);

    // This holds session data passed to each session.
    // In modules we'll use the weak pointer so more clarity.
    Logging            &m_log;
    session_ptr         m_session_data;
    config_ptr          m_config;
    session_io_ptr      m_session_io;
    processor_ansi_ptr  m_ansi_process;
    bool                m_is_active;


    // All Data is saved to this buffer, which is then
    // Checked after each stage, onEnter, Update, onExit
};

typedef std::shared_ptr<ModBase> module_ptr;


#endif // MOD_BASE_HPP
