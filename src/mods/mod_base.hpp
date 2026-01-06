#ifndef MOD_BASE_HPP
#define MOD_BASE_HPP

#include <string>
#include <vector>

struct Context;
class Logging;
class TextPromptsDao;

/**
 * @class ModBase
 * @author Michael Griffin
 * @date 3/17/2016
 * @file mod_base.hpp
 * @brief Base Class for Module System
 */
class ModBase {
    std::string m_filename;
    bool m_is_active;

public:
    Logging &m_log;
    Context &m_ctx;

    ModBase(Context ctx, std::string &filename);
    ~ModBase() = default;

    // Disable copy semantics
    ModBase(const ModBase &) = delete;
    ModBase &operator=(const ModBase &) = delete;

    // Move constructor
    ModBase(ModBase &&other) noexcept;
    ModBase &operator=(ModBase &&other) noexcept;

    const bool DISCONNECT_USER = true;

    // Box drawing characters
    enum {
        M_BORDER_TOP_LEFT = static_cast<char>(214), // ╓
        M_BORDER_BOT_LEFT = static_cast<char>(211), // ╙
        M_BORDER_ROW = static_cast<char>(196), // ─
        M_BORDER_TOP_RIGHT = static_cast<char>(183), // ╖
        M_BORDER_BOT_RIGHT = static_cast<char>(189), // ╜
        M_BORDER_MID_TOP = static_cast<char>(210), // ╥
        M_BORDER_MID_BOT = static_cast<char>(208), // ╨
        M_BORDER_MID = static_cast<char>(186) // ║
    };

    /**
     * @brief Translate Box Chars to UTF-8
     * @param enum_value
     */
    std::string baseGetEncodedBoxChar(int enum_value) const;

    /**
     * @brief Translate Box Chars to UTF-8 with Default box Color
     * @param enum_value
     */
    std::string baseGetEncodedBoxCharAndColor(int enum_value) const;

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
    std::string baseGetDefaultColor() const;

    /**
     * @brief Gets the Default Input Color Sequence
     * @return
     */
    std::string baseGetDefaultInputColor() const;

    /**
     * @brief Gets the Default Input Color Sequence
     * @return
     */
    std::string baseGetDefaultInverseColor() const;

    /**
     * @brief Gets the Default Box Color Sequence
     * @return
     */
    std::string baseGetDefaultBoxColor() const;

    /**
     * @brief Gets the Default Prompt Color Sequence
     * @return
     */
    std::string baseGetDefaultPromptColor() const;

    /**
     * @brief Gets the Default Stat Color Sequence
     * @return
     */
    std::string baseGetDefaultStatColor() const;

    /**
     * @brief Method for Adding outgoing text data to ANSI processor
     *        Then delivering the data to the client
     * @param data
     */
    void baseProcessAndDeliver(std::string &data) const;

    /**
     * @brief Method for Adding outgoing text data to ANSI processor
     *        Then delivering the data to the client, Then Disconnect
     * @param data
     */
    void baseProcessAndDeliverThenDisconnect(std::string &data) const;

    /**
     * @brief Deliver Output followed with New Line.
     * @param data
     */
    void baseProcessAndDeliverNewLine(std::string &data) const;

    /**
     * @brief Deliver NewLine for [ENTER] On Prompts.
     */
    void baseProcessDeliverNewLine() const;

    /**
     * @brief Deliver Input for prompts (No Coloring Extras)
     */
    void baseProcessDeliverInput(std::string &data) const;

    /**
    * @brief Deliver Input for prompts Then Disconnect (No Coloring Extras)
    */
    void baseProcessDeliverInputAndDisconnect(std::string &data) const;

    /**
     * @brief Pull and Display Prompts
     * @param prompt
     * @param m_text_dao
     * @param is_disconnect
     */
    void baseDisplayPrompt(const std::string &prompt, TextPromptsDao &m_text_dao, bool is_disconnect = false) const;

    /**
     * @brief Pull and Return Display Prompt
     * @param prompt
     * @param m_text_dao
     */
    std::string baseGetDisplayPrompt(const std::string &prompt, TextPromptsDao &m_text_dao) const;

    /**
     * @brief Pull and Return Raw Display Prompts
     * @param prompt
     * @param m_text_dao
     */
    std::string baseGetDisplayPromptRaw(const std::string &prompt, TextPromptsDao &m_text_dao) const;

    /**
     * @brief Pull and Return Raw Display Prompts Parse Pipe Codes to ANSI
     * @param prompt
     * @param m_text_dao
     */
    std::string baseGetDisplayPromptPipeToAnsi(const std::string &prompt, TextPromptsDao &m_text_dao) const;

    /**
     * @brief Pull and Display Prompts, Replace MCI Code |OT
     * @param prompt
     * @param m_text_dao
     * @param mci_field
     */
    void baseDisplayPromptMCI(const std::string &prompt, TextPromptsDao &m_text_dao,
                              const std::string &mci_field) const;

    /**
     * @brief Pull and Display Prompt with a following new line for info messages.
     * @param prompt
     * @param m_text_dao
     */
    void baseDisplayPromptAndNewLine(const std::string &prompt, TextPromptsDao &m_text_dao) const;

    /**
     * @brief Move to End of Display then output
     * @param prompt
     */
    void moveToBottomAndDisplay(const std::string &prompt) const;


    /**
     * @brief Move to End of Display then Setup Display for String
     * @param prompt
     */
    std::string moveStringToBottom(const std::string &prompt) const;

    /**
     * Determine if the current module is active or has been shutdown
     * @return
     */
    bool isModuleActive() const;

    /**
     * First Time Module Setup
     * @return
     */
    void setModuleActive();

    /**
     * Deactivate Module for Cleanup.
     * @return
     */
    void setModuleInActive();
};

#endif
