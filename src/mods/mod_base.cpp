#include "mod_base.hpp"

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

#include "../data-sys/text_prompts_dao.hpp"
#include "../model-sys/config.hpp"

#include "../session.hpp"
#include "../session_io.hpp"
#include "../processor_ansi.hpp"
#include "../encoding.hpp"
#include "../logging.hpp"
#include "../common_io.hpp"
#include "../tcp_session.hpp"

ModBase::ModBase(TCPSession &session_data, Config &config, ProcessorAnsi &ansi_process, std::string &filename,
                 CommonIO &common_io, SessionIO &session_io)
    : m_filename(filename)
      , m_is_active(false)
      , m_log(Logging::getInstance())
      , m_session_data(session_data)
      , m_config(config)
      , m_ansi_process(ansi_process)
      , m_common_io(common_io)
      , m_session_io(session_io) {
    // Setup All Mods for Proper Node Logging by Session.
    m_log.setUserInfo(session_data.getSession().getNodeNumber());
}

ModBase::ModBase(ModBase &&other) noexcept
    : m_filename(std::move(other.m_filename))
      , m_is_active(other.m_is_active)
      , m_log(other.m_log)
      , m_session_data(other.m_session_data)
      , m_config(other.m_config)
      , m_ansi_process(other.m_ansi_process)
      , m_common_io(other.m_common_io)
      , m_session_io(other.m_session_io) {
    // Nothing else to do, references are bound to the same objects as 'other'
}

ModBase &ModBase::operator=(ModBase &&other) noexcept {
    if (this != &other) {
        m_filename = std::move(other.m_filename);
        m_is_active = other.m_is_active;
    }
    return *this;
}


/**
 * @brief Translate Box Chars to UTF-8
 * @param enum_value
 */
std::string ModBase::baseGetEncodedBoxChar(const int enum_value) const {
    const auto char_value = std::string(1, static_cast<char>(enum_value));

    // TODO Quick Hack, update later on
    const Encoding encode;
    return encode.utf8Encode(char_value);
}

/**
 * @brief Translate Box Chars to UTF-8 with Default box Color
 * @param enum_value
 */
std::string ModBase::baseGetEncodedBoxCharAndColor(const int enum_value) const {
    const auto char_value = std::string(1, static_cast<char>(enum_value));

    // TODO Quick Hack, update later on
    const Encoding encode;
    return baseGetDefaultBoxColor() + encode.utf8Encode(char_value);
}

/**
 * @brief Generic Border Screen used buy several interfaces for display.
 * @param result_set
 * @param total_rows
 * @param max_cols
 * @return
 */
std::string ModBase::baseCreateBorderedDisplay(std::vector<std::string> result_set, const int total_rows,
                                               const int max_cols) {
    // Vector or Menus, Loop through
    auto i = result_set.begin();
    std::string buffer = "";

    for (int rows = 0; rows < total_rows; rows++) {
        buffer += "  "; // 3 Leading spaces per row.

        for (int cols = 0; cols < max_cols; cols++) {
            // Top Row
            if (rows == 0 && cols == 0) {
                buffer += baseGetEncodedBoxCharAndColor(M_BORDER_TOP_LEFT);
            } else if (rows == 0 && cols == max_cols - 1) {
                buffer += baseGetEncodedBoxCharAndColor(M_BORDER_TOP_RIGHT);
            } else if (rows == 0 && cols % (max_cols - 1) == 0) {
                buffer += baseGetEncodedBoxCharAndColor(M_BORDER_MID_TOP);
            } else if (rows == 0) {
                buffer += baseGetEncodedBoxCharAndColor(M_BORDER_ROW);
            }

            // Bottom Row
            else if (rows == total_rows - 1 && cols == 0) {
                buffer += baseGetEncodedBoxCharAndColor(M_BORDER_BOT_LEFT);
            } else if (rows == total_rows - 1 && cols == max_cols - 1) {
                buffer += baseGetEncodedBoxCharAndColor(M_BORDER_BOT_RIGHT);
            } else if (rows == total_rows - 1 && cols % (max_cols - 1) == 0) {
                buffer += baseGetEncodedBoxCharAndColor(M_BORDER_MID_BOT);
            } else if (rows == total_rows - 1) {
                buffer += baseGetEncodedBoxCharAndColor(M_BORDER_ROW);
            } else if (cols % (max_cols - 1) == 0) {
                buffer += baseGetEncodedBoxCharAndColor(M_BORDER_MID);
            } else {
                // Here we insert the Menu name and pad through to 8 characters.
                if (cols == 1) {
                    if (i != result_set.end()) {
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

    return buffer;
}

/**
 * @brief Transform Strings to Uppercase with Locale
 * @param value
 */
void ModBase::baseTransformToUpper(std::string &value) {
    const auto stringToUpper = std::bind1st(
        std::mem_fun(
            &std::ctype<char>::toupper),
        &std::use_facet<std::ctype<char> >(std::locale()));

    transform(value.begin(), value.end(), value.begin(), stringToUpper);
}

/**
 * @brief Transform Strings to Lowercase with Locale
 * @param value
 */
void ModBase::baseTransformToLower(std::string &value) {
    const auto stringToLower = std::bind1st(
        std::mem_fun(
            &std::ctype<char>::tolower),
        &std::use_facet<std::ctype<char> >(std::locale()));

    transform(value.begin(), value.end(), value.begin(), stringToLower);
}

/**
 * @brief Gets the Default Color Sequence
 * @return
 */
std::string ModBase::baseGetDefaultColor() const {
    return m_session_io.pipeColors(m_config.default_color_regular);
}

/**
 * @brief Gets the Default Input Color Sequence
 * @return
 */
std::string ModBase::baseGetDefaultInputColor() const {
    return m_session_io.pipeColors(m_config.default_color_input);
}

/**
 * @brief Gets the Default Input Color Sequence
 * @return
 */
std::string ModBase::baseGetDefaultInverseColor() const {
    return m_session_io.pipeColors(m_config.default_color_inverse);
}

/**
 * @brief Gets the Default Box Color Sequence
 * @return
 */
std::string ModBase::baseGetDefaultBoxColor() const {
    return m_session_io.pipeColors(m_config.default_color_box);
}

/**
 * @brief Gets the Default Prompt Color Sequence
 * @return
 */
std::string ModBase::baseGetDefaultPromptColor() const {
    return m_session_io.pipeColors(m_config.default_color_prompt);
}

/**
 * @brief Gets the Default Stat Color Sequence
 * @return
 */
std::string ModBase::baseGetDefaultStatColor() const {
    return m_session_io.pipeColors(m_config.default_color_stat);
}

/**
 * @brief Method for Adding outgoing text data to ANSI processor
 *        Then delivering the data to the client
 * @param data
 */
void ModBase::baseProcessAndDeliver(std::string &data) const {
    // Clear out attributes on new strings no bleeding of colors.
    std::string output = "\x1b[0m" + baseGetDefaultColor();
    output += data;
    m_ansi_process.parseTextToBuffer(const_cast<char *>(output.c_str()));
    output += baseGetDefaultInputColor();
    m_session_data.getSession().send(output);
}

/**
 * @brief Method for Adding outgoing text data to ANSI processor
 *        Then delivering the data to the client, Then Disconnect
 * @param data
 */
void ModBase::baseProcessAndDeliverThenDisconnect(std::string &data) const {
    // Clear out attributes on new strings no bleeding of colors.
    std::string output = "\x1b[0m" + baseGetDefaultColor();
    output += data;
    m_ansi_process.parseTextToBuffer(const_cast<char *>(output.c_str()));
    output += baseGetDefaultInputColor();
    m_session_data.getSession().send(output, DISCONNECT_USER);
}

/**
 * @brief Deliver Output followed with New Line.
 * @param data
 */
void ModBase::baseProcessAndDeliverNewLine(std::string &data) const {
    data += "\r\n";
    baseProcessAndDeliver(data);
}

/**
 * @brief Deliver NewLine for [ENTER] On Prompts.
 */
void ModBase::baseProcessDeliverNewLine() const {
    std::string data = "\r\n";
    baseProcessAndDeliver(data);
}

/**
 * @brief Deliver Input for prompts (No Coloring Extras)
 */
void ModBase::baseProcessDeliverInput(std::string &data) const {
    m_ansi_process.parseTextToBuffer(const_cast<char *>(data.c_str()));
    m_session_data.getSession().send(data);
}

/**
 * @brief Deliver Input for prompts Then Disconnect (No Coloring Extras)
 */
void ModBase::baseProcessDeliverInputAndDisconnect(std::string &data) const {
    m_ansi_process.parseTextToBuffer(const_cast<char *>(data.c_str()));
    m_session_data.getSession().send(data, DISCONNECT_USER);
}

/**
 * @brief Pull and Display Prompts
 * @param prompt
 * @param m_text_dao
 * @param is_disconnect
 */
void ModBase::baseDisplayPrompt(const std::string &prompt, TextPromptsDao &m_text_dao, const bool is_disconnect) const {
    // Set Default String Color, Can be overridden with pipe colors in text prompt.
    std::string result = baseGetDefaultColor();

    // Parse Prompt for Input Color And Position Override.
    // If found, the colors of the MCI Codes should be used as the default color.
    M_StringPair prompt_set = m_text_dao.getPrompt(prompt);
    const std::string::size_type idx = prompt_set.second.find("%IN", 0);

    result += m_session_io.parseTextPrompt(prompt_set);

    // Not found, set default input color
    if (idx == std::string::npos) {
        result += baseGetDefaultInputColor();
    }

    if (is_disconnect) {
        baseProcessAndDeliverThenDisconnect(result);
    } else {
        baseProcessAndDeliver(result);
    }
}

/**
 * @brief Pull and Return Display Prompt
 * @param prompt
 * @param m_text_dao
 */
std::string ModBase::baseGetDisplayPrompt(const std::string &prompt, TextPromptsDao &m_text_dao) const {
    // Set Default String Color, Can be overridden with pipe colors in text prompt.
    std::string result = baseGetDefaultColor();

    // Parse Prompt for Input Color And Position Override.
    // If found, the colors of the MCI Codes should be used as the default color.
    M_StringPair prompt_set = m_text_dao.getPrompt(prompt);
    const std::string::size_type idx = prompt_set.second.find("%IN", 0);

    result += m_session_io.parseTextPrompt(prompt_set);

    // Not found, set default input color
    if (idx == std::string::npos) {
        result += baseGetDefaultInputColor();
    }

    return result;
}

/**
 * @brief Pull and Return Raw Display Prompts
 * @param prompt
 * @param m_text_dao
 */
std::string ModBase::baseGetDisplayPromptRaw(const std::string &prompt, TextPromptsDao &m_text_dao) const {
    // Parse Prompt for Input Color And Position Override.
    // If found, the colors of the MCI Codes should be used as the default color.
    M_StringPair prompt_set = m_text_dao.getPrompt(prompt);
    return prompt_set.second;
}

/**
 * @brief Pull and Return Raw Display Prompts Parse Pipe Codes to ANSI
 * @param prompt
 * @param m_text_dao
 */
std::string ModBase::baseGetDisplayPromptPipeToAnsi(const std::string &prompt, TextPromptsDao &m_text_dao) const {
    // Parse Prompt for Input Color And Position Override.
    // If found, the colors of the MCI Codes should be used as the default color.
    M_StringPair prompt_set = m_text_dao.getPrompt(prompt);
    return m_session_io.pipeColors(prompt_set.second);
}

/**
 * @brief Pull and Display Prompts, Replace MCI Code |OT
 * @param prompt
 * @param m_text_dao
 * @param mci_field
 */
void ModBase::baseDisplayPromptMCI(const std::string &prompt, TextPromptsDao &m_text_dao,
                                   const std::string &mci_field) const {
    // Set Default String Color, Can be overridden with pipe colors in text prompt.
    std::string result = baseGetDefaultColor();

    // Parse Prompt for Input Color And Position Override.
    // If found, the colors of the MCI Codes should be used as the default color.
    M_StringPair prompt_set = m_text_dao.getPrompt(prompt);
    const std::string::size_type idx = prompt_set.second.find("%IN", 0);

    // Parse and replace the MCI Code with the field value
    const std::string mci_code = "|OT";
    m_common_io.parseLocalMCI(prompt_set.second, mci_code, mci_field);

    // Does pipe2ansi for colors etc..
    result += m_session_io.parseTextPrompt(prompt_set);

    // Not found, set default input color
    if (idx == std::string::npos) {
        result += baseGetDefaultInputColor();
    }

    baseProcessAndDeliver(result);
}

/**
 * @brief Pull and Display Prompt with a following new line for info messages.
 * @param prompt
 * @param m_text_dao
 */
void ModBase::baseDisplayPromptAndNewLine(const std::string &prompt, TextPromptsDao &m_text_dao) const {
    // Set Default String Color, Can be overridden with pipe colors in text prompt.
    std::string result = baseGetDefaultColor();

    // Parse Prompt for Input Color And Position Override.
    // If found, the colors of the MCI Codes should be used as the default color.
    M_StringPair prompt_set = m_text_dao.getPrompt(prompt);
    const std::string::size_type idx = prompt_set.second.find("%IN", 0);

    result += m_session_io.parseTextPrompt(prompt_set);

    // Not found, set default input color
    if (idx == std::string::npos) {
        result += baseGetDefaultInputColor();
    }

    // Add New Line.
    result += "\r\n";

    baseProcessAndDeliver(result);
}

/**
 * @brief Move to End of Display then output
 * @param prompt
 */
void ModBase::moveToBottomAndDisplay(const std::string &prompt) const {
    std::string output = "";
    const int screen_row = m_ansi_process.getMaxRowsUsedOnScreen();

    output += baseGetDefaultColor();
    output += "\x1b[" + std::to_string(screen_row) + ";1H\r\n";
    output += prompt;
    baseProcessAndDeliver(output);
}

/**
 * @brief Move to End of Display then Setup Display for String
 * @param prompt
 */
std::string ModBase::moveStringToBottom(const std::string &prompt) const {
    std::string output = "";
    const int screen_row = m_ansi_process.getMaxRowsUsedOnScreen();

    output += baseGetDefaultColor();
    output += "\x1b[" + std::to_string(screen_row) + ";1H\r\n";
    output += prompt;
    return output;
}

/**
 * Determine if the current module is active or has been shutdown
 * @return
 */
bool ModBase::isModuleActive() const {
    return m_is_active;
}

/**
 * First Time Module Setup
 * @return
 */
void ModBase::setModuleActive() {
    m_is_active = true;
}

/**
 * Deactivate Module for Cleanup.
 * @return
 */
void ModBase::setModuleInActive() {
    m_is_active = false;
}
