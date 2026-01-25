#include "io_input_handler.hpp"

#include <utility>
#include <cstddef>
#include <utf8.h>
#include <cwchar>

#include "model-sys/cell.hpp"

#include "logging.hpp"
#include "encoding.hpp"
#include "io_common.hpp"


/**
 * @brief Single Key Input For Full Screen Editor or Esc Sequences
 * @param character_buffer
 * @return
 */
std::string IoInputHandler::getFSEKeyInput(const std::string &character_buffer) {
    std::string input = m_io_common.parseInput(character_buffer);

    if (input.empty()) {
        // No Data received, could be in mid-ESC sequence
        // Return for next key.
        m_log.log(Logging::LogLevel::Debug, "getKeyInput Mid Escape");
        return "";
    }

    if (input[0] == '\x1b') {
        std::string escape_sequence = m_io_common.getFSEEscapeSequence();

        m_log.log(Logging::LogLevel::Debug, "FSE escape_sequence=", escape_sequence);

        if (escape_sequence.empty()) {
            m_log.log(Logging::LogLevel::Debug, "getKeyInput Single Escape");
            return "\x1b";
        } else {
            m_log.log(Logging::LogLevel::Debug, "getKeyInput Translated Escape Sequence=", escape_sequence);
            return (escape_sequence.insert(0, "\x1b"));
        }
    }

    m_log.log(Logging::LogLevel::Debug, "getKeyInput Normal Input=", input);
    return input;
}

/**
 * @brief Single Key Input or Esc Sequence
 * @param character_buffer
 * @return
 */
std::string IoInputHandler::getKeyInput(const std::string &character_buffer) {
    std::string input = m_io_common.parseInput(character_buffer);

    if (input.empty()) {
        // No Data received, could be in mid ESC sequence
        // Return for next key.
        m_log.log(Logging::LogLevel::Debug, "getKeyInput Mid Escape");
        return "";
    }

    if (input[0] == '\x1b') {
        std::string escape_sequence = m_io_common.getEscapeSequence();

        if (escape_sequence.empty()) {
            m_log.log(Logging::LogLevel::Debug, "getKeyInput Single Escape");
            return "\x1b";
        } else {
            m_log.log(Logging::LogLevel::Debug, "getKeyInput Translated Escape Sequence=", escape_sequence);
            return (escape_sequence.insert(0, "\x1b"));
        }
    }

    m_log.log(Logging::LogLevel::Debug, "getKeyInput Normal Input=", input);
    return input;
}

/**
 * @brief Generates an input field with ANSI color background
 * @param field_name {Is the Field label, precedes the field like 'Mail From: '}
 * @param len
 */
void IoInputHandler::createInputField(std::string &field_name, int &len) {
    std::string repeat;
    char formatted[1024] = {0};
    char sTmp[3] = {0};
    char sTmp2[3] = {0};

    // Parse for Input String Modifiers
    std::string::size_type stringSize = 0;
    std::string::size_type position = 0;


    char INPUT_COLOR[255] = {0};
    bool isColorOverRide = false; //found input color

    stringSize = field_name.size() - 1;

    if (len == 0) {
        return;
    }

    m_log.log(Logging::LogLevel::Info, "m_session.getUseAnsi()=", m_session.isAnsi());

    // Format Input Field, if color is enabled, otherwise just add Field Name like "Login: "
    if (!m_session.isAnsi()) {
        sprintf(formatted, "%s", (char *) field_name.c_str()); // Field Name
        field_name = formatted;
        return;
    }

    // Override Field Input Length for Input Field.
    position = field_name.find("|FL", 0);

    if (position != std::string::npos) {
        // Make sure we don't go past the bounds
        if (position + 4 <= stringSize) {
            // (Unit Test Notes)
            // Need to Test if idDigit!  And only if, both are
            // Then we cut these out and erase!,  Otherwise
            // We only remove the |IN pipe sequence.
            if (isdigit(field_name[position + 3]) && isdigit(field_name[position + 4])) {
                std::string::size_type tempLength = 0;
                sTmp[0] = field_name[position + 3];
                sTmp[1] = field_name[position + 4];
                field_name.erase(position, 5);
                tempLength = atoi(sTmp);

                if (static_cast<signed>(tempLength) > 0 && static_cast<signed>(tempLength) <= len) {
                    len = tempLength;
                } else {
                    m_log.log(Logging::LogLevel::Error, "createInputField() Incorrect |FL field length=", tempLength,
                              "cannot exceed max size=", len);
                }
            } else {
                field_name.erase(position, 3);
            }
        }
    }

    // Override Foreground/Background Input Field Colors
    // This is now for OBV/2 - Not in Legacy.
    position = field_name.find("|FB", 0);
    m_log.log(Logging::LogLevel::Debug, "createInputField() |FB position=", position, "compare=", position + 4,
              stringSize);

    if (position != std::string::npos) {
        // (Unit Test Notes)
        // Need to Test if isDigit!  And only if, both are
        // Then we cut these out and erase!,  Otherwise
        // We only remove the |FB pipe sequence.

        memset(&sTmp, 0, 3);
        memset(&sTmp2, 0, 3);

        // Make sure we don't go past the bounds
        if (position + 6 <= stringSize) {
            if (isdigit(field_name[position + 3]) && isdigit(field_name[position + 4]) &&
                isdigit(field_name[position + 5]) && isdigit(field_name[position + 6])) {
                sTmp[0] = field_name[position + 3]; // Foreground 00-15
                sTmp[1] = field_name[position + 4];
                sTmp2[0] = field_name[position + 5]; // Background 16-23
                sTmp2[1] = field_name[position + 6];
                field_name.erase(position, 7);
                sprintf(INPUT_COLOR, "|%s|%s", sTmp, sTmp2);
                isColorOverRide = true;
            } else {
                field_name.erase(position, 3);
            }
        }
    }

    // Pad len amount of spaces.
    if (len > 0) {
        repeat.insert(0, len, ' ');
    }

    // Set Default Input Color if none was passed.
    if (!isColorOverRide) {
        // Make this a theme color to set a variable...
        sprintf(INPUT_COLOR, "|00|19"); // make theme
    }

    // Format the input field
    sprintf(formatted, "%s%s%s\x1b[%iD",
            const_cast<char *>(field_name.c_str()), // Field Name
            INPUT_COLOR, // Field Fg,Bg Color
            repeat.c_str(), // Padding length of Field
            len + 1); // Move back to starting position of field.

    field_name = formatted;
}

/**
 * @brief Input Field, Returns output for keypresses, field stored in result on ENTER.
 * @param character_buffer
 * @param result
 * @param length
 * @param leadoff
 * @param hidden
 * @return
 */
std::string IoInputHandler::getInputField(const std::string &character_buffer,
                                     std::string &result,
                                     int length,
                                     std::string leadoff,
                                     bool hidden) {
    // Set up the lead off, if it's first time, then print it out
    // Other if empty or follow-up calls to input field skip it!
    static bool is_leadoff = true;

    if (leadoff.empty()) {
        is_leadoff = false;
    }

    // Display Lead off of field.. ie.. 'Mail to: Mercyful Name'
    // put the name in the field.
    if (is_leadoff) {
        m_session.send(leadoff);
        is_leadoff = false;
    }

    std::string string_data = m_io_common.getLine(character_buffer, length, leadoff, hidden);

    if (static_cast<signed>(string_data.size()) > 0) {
        // Check for ESC for Abort!
        if (string_data[0] == 27 && string_data.size() == 1) {
            std::string esc_sequence = m_io_common.getEscapeSequence();

            if (esc_sequence.empty() && character_buffer[0] == '\0') {
                is_leadoff = true; // Reset for next run
                esc_sequence.erase();
                string_data.erase();
                return "aborted";
            }
        }
        // Check for Completed Field Entry
        else if ((string_data[0] == '\n' && string_data.size() == 1) || character_buffer[0] == '\n') {
            result = m_io_common.getInputBuffer();
            string_data.erase();
            is_leadoff = true; // Reset for next run
            return "\n";
        }
        // Updates on Keypresses.
        else {
            m_log.log(Logging::LogLevel::Debug, "getInputField() result=", result, "string_data=", string_data);
            return string_data;
        }
    }

    m_log.log(Logging::LogLevel::Debug, "getInputField() result empty");
    return "";
}