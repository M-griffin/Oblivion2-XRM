#include "io_pipes_colors.hpp"

#include "model-sys/config.hpp"

/**
 * @brief Parses for ANSI Foreground Colors (Helper for pipe2ansi)
 * @param foreground
 */
std::string IoPipesAndColors::pipeReplaceForeground(int foreground) {
    std::string escape_sequence;

    switch (foreground) {
        case 0:
            escape_sequence = "\x1b[0;30m";
            break;
        case 1:
            escape_sequence = "\x1b[0;34m";
            break;
        case 2:
            escape_sequence = "\x1b[0;32m";
            break;
        case 3:
            escape_sequence = "\x1b[0;36m";
            break;
        case 4:
            escape_sequence = "\x1b[0;31m";
            break;
        case 5:
            escape_sequence = "\x1b[0;35m";
            break;
        case 6:
            escape_sequence = "\x1b[0;33m";
            break;
        case 7:
            escape_sequence = "\x1b[0;37m";
            break;
        case 8:
            escape_sequence = "\x1b[1;30m";
            break;
        case 9:
            escape_sequence = "\x1b[1;34m";
            break;
        case 10:
            escape_sequence = "\x1b[1;32m";
            break;
        case 11:
            escape_sequence = "\x1b[1;36m";
            break;
        case 12:
            escape_sequence = "\x1b[1;31m";
            break;
        case 13:
            escape_sequence = "\x1b[1;35m";
            break;
        case 14:
            escape_sequence = "\x1b[1;33m";
            break;
        case 15:
            escape_sequence = "\x1b[1;37m";
            break;
        default:
            break;
    }

    return escape_sequence;
}

/**
 * @brief Parses for ANSI Background Colors (Helper for pipe2ansi)
 * @param background
 */
std::string IoPipesAndColors::pipeReplaceBackground(int background) {
    std::string escape_sequence;

    switch (background) {
        case 16:
            escape_sequence = "\x1b[40m";
            break;
        case 17:
            escape_sequence = "\x1b[44m";
            break;
        case 18:
            escape_sequence = "\x1b[42m";
            break;
        case 19:
            escape_sequence = "\x1b[46m";
            break;
        case 20:
            escape_sequence = "\x1b[41m";
            break;
        case 21:
            escape_sequence = "\x1b[45m";
            break;
        case 22:
            escape_sequence = "\x1b[43m";
            break;
        case 23:
            escape_sequence = "\x1b[47m";
            break;
        // Default to none.
        case 24:
            escape_sequence = "\x1b[0m";
            break;
        default:
            break;
    }

    return escape_sequence;
}

/**
 * @brief Parsed Pipe Colors Codes to ANSI ESC Sequence.
 * @param color_string
 * @return
 */
std::string IoPipesAndColors::pipeColors(const std::string &color_string) {
    // Skip PIPE and grab next two digits.
    std::string str = color_string.substr(1);
    std::string esc_sequence;

    // String to Int
    std::istringstream ss(str);
    int color_index = 0;
    ss >> color_index;

    if (ss.fail()) {
        ss.clear();
        ss.ignore();
        return "";
    }

    ss.clear();

    // Foreground Colors
    if (color_index >= 0 && color_index < 16) {
        m_log.log(Logging::LogLevel::Debug, "foreground color_index=", color_index);
        esc_sequence = pipeReplaceForeground(color_index);
        m_log.log(Logging::LogLevel::Debug, "foreground esc_sequence=", esc_sequence);
        return esc_sequence;
    }
    // Background Colors
    if (color_index >= 16 && color_index < 24) {
        m_log.log(Logging::LogLevel::Debug, "background color_index=", color_index);
        esc_sequence = pipeReplaceBackground(color_index);
        m_log.log(Logging::LogLevel::Debug, "background esc_sequence=", esc_sequence);
        return esc_sequence;
    }

    m_log.log(Logging::LogLevel::Debug, "else esc_sequence=", esc_sequence);

    return esc_sequence;
}


/**
 * @brief Gets the Default System Color Sequence
 * @return
 */
std::string IoPipesAndColors::getDefaultColor(Config &config) {
    return pipeColors(config.default_color_regular);
}

/**
 * @brief Gets the Default System Input Color Sequence
 * @return
 */
std::string IoPipesAndColors::getDefaultInputColor(Config &config) {
    return pipeColors(config.default_color_input);
}

/**
 * @brief Gets the Default System Inverse Color Sequence
 * @return
 */
std::string IoPipesAndColors::getDefaultInverseColor(Config &config) {
    return pipeColors(config.default_color_inverse);
}

/**
 * @brief Gets the Default System Prompt Color Sequence
 * @return
 */
std::string IoPipesAndColors::getDefaultPromptColor(Config &config) {
    return pipeColors(config.default_color_prompt);
}

/**
 * @brief Gets the Default System stat Color Sequence
 * @return
 */
std::string IoPipesAndColors::getDefaultStatColor(Config &config) {
    return pipeColors(config.default_color_stat);
}

/**
 * @brief Gets the Default System box Color Sequence
 * @return
 */
std::string IoPipesAndColors::getDefaultBoxColor(Config &config) {
    return pipeColors(config.default_color_box);
}

/**
 * @brief Parsed Pipe Codes with 1 or 2 Digits (Handle OBV/2 Legacy Movements)
 * @param code
 * @param value
 * @return
 */
std::string IoPipesAndColors::parsePipeWithCharsDigits(const std::string &code, int value) {
    std::string sequence;

    // Check Single letter Sequences
    if (code.size() == 1) {
        switch (code[0]) {
            case 'U': // Up
                if (value == 0 || value == 1)
                    sequence = "\x1b[A";
                else
                    sequence = "\x1b[" + std::to_string(value) + "A";

                break;

            case 'D': // Down
                if (value == 0 || value == 1)
                    sequence = "\x1b[B";
                else
                    sequence = "\x1b[" + std::to_string(value) + "B";

                break;

            case 'F': // Forward
                if (value == 0 || value == 1)
                    sequence = "\x1b[C";
                else
                    sequence = "\x1b[" + std::to_string(value) + "C";

                break;

            case 'B': // Backwards
                if (value == 0 || value == 1)
                    sequence = "\x1b[D";
                else
                    sequence = "\x1b[" + std::to_string(value) + "D";

                break;

            case 'X': // Absolute X Position
                if (value == 0 || value == 1)
                    sequence = "\x1b[G";
                else
                    sequence = "\x1b[" + std::to_string(value) + "G";

                break;

            default:
                break;
        }
    }
    // Double Letter Sequences
    else {
        // Updates, is this still needed? test lateron.
    }

    return sequence;
}

/**
 * @brief Separate Pipe Codes with 1 or 2 Digits
 * @param pipe_code
 * @return
 */
std::string IoPipesAndColors::separatePipeWithCharsDigits(const std::string &pipe_code) {
    // Strip PIPE and grab the Sequences
    std::string str = pipe_code.substr(1);
    int alpha_chars = 0;

    // Figure out if were dealing with 1 or 2 Digit Character Codes.
    if (isdigit(str[1])) // |Xxx ?
    {
        alpha_chars = 1;
    }
    // We have 2 Characters, now test for 1 or 2 digits.
    else // |XXxx ?
    {
        alpha_chars = 2;
    }

    // Split Alpha from Numeric so we can process it!
    std::string str_alpha = str.substr(0, alpha_chars);
    str.erase(0, alpha_chars);

    // String to Int for numeric remainder for switch/case.
    // Also removes leading 0 if not needed!
    std::istringstream ss(str);
    int pipe_index = 0;
    ss >> pipe_index;

    if (ss.fail()) {
        return "";
    }

    // Parse and return result.
    return (parsePipeWithCharsDigits(str_alpha, pipe_index));
}

/**
 * @brief Parse Pipe Codes with no Following Digits
 * @param pipe_code
 * @return
 */
std::string IoPipesAndColors::parsePipeWithChars(const std::string &pipe_code) {
    // Strip PIPE and grab the Sequences
    std::string esc_sequence;

    // Make this more dynamic?!?

    switch (pipe_code[1]) {
        // Most likely will break these out later bye pipe[1] and send [2] to other functions
        // To Keep small and compact!
        case 'C':
            switch (pipe_code[2]) {
                case 'S': // Clear Screen / Home Cursor
                    esc_sequence = "\x1b[2J\x1b[1;1H";
                    break;

                case 'R': // Carriage Return / NewLine
                    esc_sequence = "\r\n";
                    break;

                default:
                    break;
            }

            break;

        case 'D':
            switch (pipe_code[2]) {
                case 'S': // Clear Screen / Home Cursor
                    esc_sequence = "\x1b[2J\x1b[1;1H";
                    break;

                case 'R': // Carriage Return / NewLine
                    esc_sequence = "\r\n";
                    break;

                default:
                    break;
            }

            break;

        default:
            break;
    }

    return esc_sequence;
}

/**
 * @brief Colorizes Brackets and Text between brackets to system theme colors
 * @param sequence
 * @return
 */
std::string IoPipesAndColors::parseFormatColorsBrackets(const std::string &sequence, Config &config) {
    // Mocked up for now
    std::string output = sequence;
    output += getDefaultPromptColor(config);
    return output;
}

/**
 * @brief Colorizes Colons to system theme colors
 * @param sequence
 * @return
 */
std::string IoPipesAndColors::parseFormatColorsColon(const std::string &sequence, Config &config) {
    // Mocked up for now
    std::string output = sequence;
    output += getDefaultPromptColor(config);
    return output;
}