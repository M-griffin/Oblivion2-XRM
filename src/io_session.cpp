#include "io_session.hpp"

#include <regex>
#include <string>
#include <iostream>
#include <sstream>

#include "session_writer.hpp"
#include "io_common.hpp"
#include "io_input_handler.hpp"

#include "logging.hpp"

#include "model-sys/config.hpp"

IoSession::IoSession(SessionWriter &session, IoCommon &common)
    : m_log(Logging::getInstance())
      , m_session(session)
      , m_io_common(common)
      , m_io_input_handler(session, common) {
}

IoSession::~IoSession() {
    m_log.log(Logging::LogLevel::Console, "~IoSession()");
    m_mapped_codes.clear();
}

// Io_Input Handler
std::string IoSession::getFSEKeyInput(const std::string &character_buffer) {
    return m_io_input_handler.getFSEKeyInput(character_buffer);
}

std::string IoSession::getKeyInput(const std::string &character_buffer) {
    return m_io_input_handler.getKeyInput(character_buffer);
}

void IoSession::createInputField(std::string &field_name, int &len) {
    m_io_input_handler.createInputField(field_name, len);
}

std::string IoSession::getInputField(const std::string &character_buffer,
                                     std::string &result, int length,
                                     std::string leadoff, bool hidden) {
    return m_io_input_handler.getInputField(character_buffer,
                                            result, length,
                                            leadoff, hidden);
}

// Io_Pipes and Colors
std::string IoSession::pipeReplaceForeground(int foreground) {
    return m_pipes_and_colors.pipeReplaceForeground(foreground);
}

std::string IoSession::pipeReplaceBackground(int background) {
    return m_pipes_and_colors.pipeReplaceBackground(background);
}

std::string IoSession::pipeColors(const std::string &color_string) {
    return m_pipes_and_colors.pipeColors(color_string);
}

std::string IoSession::getDefaultColor(Config &config) {
    return m_pipes_and_colors.getDefaultColor(config);
}

std::string IoSession::getDefaultInputColor(Config &config) {
    return m_pipes_and_colors.getDefaultInputColor(config);
}

std::string IoSession::getDefaultInverseColor(Config &config) {
    return m_pipes_and_colors.getDefaultInverseColor(config);
}

std::string IoSession::getDefaultPromptColor(Config &config) {
    return m_pipes_and_colors.getDefaultPromptColor(config);
}

std::string IoSession::getDefaultStatColor(Config &config) {
    return m_pipes_and_colors.getDefaultStatColor(config);
}

std::string IoSession::getDefaultBoxColor(Config &config) {
    return m_pipes_and_colors.getDefaultBoxColor(config);
}

std::string IoSession::parsePipeWithCharsDigits(const std::string &code, int value) {
    return m_pipes_and_colors.parsePipeWithCharsDigits(code, value);
}

std::string IoSession::separatePipeWithCharsDigits(const std::string &pipe_code) {
    return m_pipes_and_colors.separatePipeWithCharsDigits(pipe_code);
}

std::string IoSession::parsePipeWithChars(const std::string &pipe_code) {
    return m_pipes_and_colors.parsePipeWithChars(pipe_code);
}

std::string IoSession::parseFormatColorsBrackets(const std::string &sequence, Config &config) {
    return m_pipes_and_colors.parseFormatColorsBrackets(sequence, config);
}

std::string IoSession::parseFormatColorsColon(const std::string &sequence, Config &config) {
    return m_pipes_and_colors.parseFormatColorsColon(sequence, config);
}


// Code Maps for Ansi Screen Parsing.

/**
 * @brief Parse Files %%Filename.ext
 * @param pipe_code
 * @return
 */
std::string IoSession::parseFilename(const std::string &pipe_code) {
    // Strip %%DF and grab the 'Filename.ext
    IoCommon common_io;
    std::string str = pipe_code.substr(4);
    std::string buffer = common_io.readAnsi(str);

    if (!buffer.empty()) {
        return pipe2ansi(buffer);
    }

    return buffer;
}

/**
 * @brief Parses Code Map and replaces screen codes with ANSI Sequences.
 * @param screen
 * @param code_map
 * @return
 */
std::string IoSession::parseCodeMap(const std::string &screen, std::vector<MapType> &code_map) {
    m_log.log(Logging::LogLevel::Debug, "[parseCodeMap]", __LINE__, __FILE__);

    std::string ansi_string(screen);

    // All Global MCI Codes likes standard screens and colors will
    // He handled here, then specific interfaces will break out below this.
    // Break out parsing on which pattern was matched.
    while (!code_map.empty()) {
        // Loop Backwards to preserve string offsets on replacement.
        MapType my_matches = code_map.back();
        code_map.pop_back();

        // Check for Custom Screen Translation Mappings
        // If these exist, they take presidency over standard codes
        if (!m_mapped_codes.empty()) {
            std::map<std::string, std::string>::iterator it;
            it = m_mapped_codes.find(my_matches.m_code);

            if (it != m_mapped_codes.end()) {
                // If found, replace mci sequence with text
                ansi_string.replace(my_matches.m_offset, my_matches.m_length, it->second);
                continue;
            }
        }

        // Handle parsing on expression match.
        switch (my_matches.m_match) {
            case 1: // Pipe w/ 2 DIGIT Colors
            {
                m_log.log(Logging::LogLevel::Info, "Pipe w/ 2 DIGIT Colors |00", "Ansi?=", m_session.isAnsi());
                std::string result = pipeColors(my_matches.m_code);

                if (!result.empty()) {
                    // Replace the Color, if not ansi then remove the color!
                    if (m_session.isAnsi()) {
                        ansi_string.replace(my_matches.m_offset, my_matches.m_length, result);
                    } else {
                        ansi_string.replace(my_matches.m_offset, my_matches.m_length, "");
                    }
                } else {
                    ansi_string.replace(my_matches.m_offset, my_matches.m_length, "   ");
                }
            }
            break;

            case 2: // Pipe w/ 2 Chars and 4 Digits // |XY0101
            {
                m_log.log(Logging::LogLevel::Info, "Pipe w/ 2 Chars and 4 Digits // |XY0101");
                // Remove for now, haven't gotten this far!
                ansi_string.replace(my_matches.m_offset, my_matches.m_length, "       ");
            }
            break;

            case 3: // Pipe w/ 1 or 2 CHARS followed by 1 or 2 DIGITS
            {
                m_log.log(Logging::LogLevel::Info,
                          "Pipe w/ 1 or 2 CHARS followed by 1 or 2 DIGITS // |A1 A22  AA2  AA33");
                std::string result = separatePipeWithCharsDigits(my_matches.m_code);

                if (!result.empty()) {
                    // Replace the string
                    ansi_string.replace(my_matches.m_offset, my_matches.m_length, result);
                }
            }
            break;

            case 4: // Pipe w/ 2 CHARS
                // This one will need replacement in the string parsing
                // Pass the original string because of |DE for delay!
            {
                m_log.log(Logging::LogLevel::Info, "Pipe w/ 2 CHARS // |AA");
                std::string result = parsePipeWithChars(my_matches.m_code);

                if (!result.empty()) {
                    // Replace the string
                    ansi_string.replace(my_matches.m_offset, my_matches.m_length, result);
                } else {
                    ansi_string.replace(my_matches.m_offset, my_matches.m_length, "   ");
                }
            }
            break;

            case 5: // %%FILENAME.EXT  get filenames for loading from string prompts
            {
                m_log.log(Logging::LogLevel::Info, "Replacing %%FILENAME.EXT codes");
                std::string result = parseFilename(my_matches.m_code);

                if (!result.empty()) {
                    ansi_string.replace(my_matches.m_offset, my_matches.m_length, result);
                } else {
                    std::string s(my_matches.m_length, ' ');
                    ansi_string.replace(my_matches.m_offset, my_matches.m_length, s);
                }
            }
            break;

            case 6: // Percent w/ 2 CHARS
            {
                m_log.log(Logging::LogLevel::Info, "Percent w/ 2 CHARS");
                // Remove for now, haven't gotten this far!
                ansi_string.replace(my_matches.m_offset, my_matches.m_length, "   ");
            }
            break;

            case 7: // Percent with 2 digits, custom codes
            {
                // Were just removing them because they are processed.
                // Now that first part of sequence |01 etc.. are processed!
                m_log.log(Logging::LogLevel::Info, "replacing %## codes");
                // Remove for now, haven't gotten this far!
                ansi_string.replace(my_matches.m_offset, my_matches.m_length, "   ");
            }
            break;

            default:
                break;
        }
    }

    // Clear Code map.
    code_map.clear();
    std::vector<MapType>().swap(code_map);

    // Clear Custom MCI Screen Translation Mappings
    clearAllMCIMapping();
    return ansi_string;
}

/**
 * @brief Parses Code Map and replaces screen codes with Generic Items.
 * @param screen
 * @param code_map
 * @return
 */
std::string IoSession::parseCodeMapGenerics(const std::string &screen, const std::vector<MapType> &code_map) {
    m_log.log(Logging::LogLevel::Debug, "[parseCodeMapGenerics]", __LINE__, __FILE__);

    std::string ansi_string(screen);

    // Make a copy so the original is not modified.
    std::vector<MapType> code_mapping;
    code_mapping.assign(code_map.begin(), code_map.end());

    // All Global MCI Codes likes standard screens and colors will
    // He handled here, then specific interfaces will break out below this.
    // Break out parsing on which pattern was matched.
    while (!code_mapping.empty()) {
        // Loop Backwards to preserve string offsets on replacement.
        MapType my_matches = code_mapping.back();
        code_mapping.pop_back();

        // Check for Custom Screen Translation Mappings
        // If these exist, they take presidency over standard codes
        if (!m_mapped_codes.empty()) {
            std::map<std::string, std::string>::iterator it;
            it = m_mapped_codes.find(my_matches.m_code);

            if (it != m_mapped_codes.end()) {
                m_log.log(Logging::LogLevel::Debug, "[parseCodeMapGenerics] gen found=", my_matches.m_code, it->second,
                          __LINE__, __FILE__);
                // If found, replace mci sequence with text
                ansi_string.replace(my_matches.m_offset, my_matches.m_length, it->second);
            } else {
                m_log.log(Logging::LogLevel::Debug, "[parseCodeMapGenerics] gen not found=", __LINE__, __FILE__);
                std::string remove_code;
                ansi_string.replace(my_matches.m_offset, my_matches.m_length, remove_code);
            }
        }
    }

    // Clear MCI And Code Mappings
    clearAllMCIMapping();
    code_mapping.clear();
    std::vector<MapType>().swap(code_mapping);
    return ansi_string;
}

/**
 * @brief Parses string and returns code mapping and positions per expression
 * @param sequence
 * @return
 */
std::vector<MapType> IoSession::parseToCodeMap(const std::string &sequence, const std::string &expression) {
    // Contains all matches found so we can iterate and replace
    // Without Multiple loops through the string.
    std::vector<MapType> code_map;

    // Make a copy that we can modify and process on.
    std::string ansi_string(sequence);

    // MCI Code Groups 1 - 7
    /*
        ([|]{1}[0-9]{2})                // |00
        ([|]{1}[X][Y][0-9]{4}           // |XY0101
        ([|]{1}[A-Z]{1,2}[0-9]{1,2})    // |A1 A22  AA2  AA33
        ([|]{1}[A-Z]{2})                // |AA
        ([%]{2}[\w]+[.]{1}[\w]{3})      // %%filename.ans
        ([%]{1}[A-Z]{2})                // %AA
        ([%]{1}[0-9]{2})                // %11
    */

    //std::cout << "exp: " << expression << std::endl;
    try {
        MapType my_matches;
        std::regex expr(expression);
        std::smatch matches;
        std::string::const_iterator start = ansi_string.begin(), end = ansi_string.end();

        // Temp for Logging
        std::string::size_type offset = 0;
        std::string::size_type length = 0;

        std::regex_constants::match_flag_type flags = std::regex_constants::match_default;

        while (std::regex_search(start, end, matches, expr, flags)) {
            // Found a match!

            std::cout << "Matched Sub '" << matches.str()
                    << "' following ' " << matches.prefix().str()
                    << "' preceding ' " << matches.suffix().str()
                    << std::endl;

            // Avoid Infinite loop and make sure the existing
            // is not the same as the next!
            if (start == matches[0].second) {
                m_log.log(Logging::LogLevel::Debug, "[parseToCodeMap] no Code Maps Found", __LINE__, __FILE__);
                break;
            }

            // Since were replacing on the fly, we need to re-scan the
            // string for next code
            start = matches[0].second;

            // Loop each match, and grab the starting position and length to replace.
            for (size_t s = 1; s < matches.size(); ++s) {
                // Make sure the Match is true! otherwise skip.
                if (matches[s].matched) {
                    offset = matches[s].first - ansi_string.begin();
                    length = matches[s].length();

                    // Test output s registers which pattern matched, 1, 2, or 3!

                    std::cout << s << " :  Matched Sub 2" << matches[s].str()
                            << " at offset " << offset
                            << " of length " << length
                            << std::endl;

                    // Add to Vector so we store each match.
                    my_matches.m_offset = matches[s].first - ansi_string.begin();
                    my_matches.m_length = matches[s].length();
                    my_matches.m_match = s;
                    my_matches.m_code = matches[s].str();

                    code_map.push_back(std::move(my_matches));
                }
            }
        }
    } catch (std::regex_error &ex) {
        m_log.log(Logging::LogLevel::Error, "[parseToCodeMap] Exception=", ex.what(), ex.code(), __LINE__, __FILE__);
    }

    return code_map;
}

/**
 * @brief Converts MCI Sequences to Ansi screen output.
 * @param sequence
 * @return
 */
std::string IoSession::pipe2ansi(const std::string &sequence) {
    std::vector<MapType> code_map = parseToCodeMap(sequence, STD_EXPRESSION);
    std::string result = parseCodeMap(sequence, code_map);
    return result;
}

/**
 * @brief Converts MCI Sequences to Code Maps for Multiple Parses of same string data
 * @param sequence
 * @return
 */
std::vector<MapType> IoSession::pipe2genericCodeMap(const std::string &sequence) {
    std::vector<MapType> code_map = parseToCodeMap(sequence, MID_EXPRESSION);
    return code_map;
}

/**
 * @brief Converts MCI Sequences to Code Maps for Prompt Strings
 * @param sequence
 * @return
 */
std::vector<MapType> IoSession::pipe2promptCodeMap(const std::string &sequence) {
    // This will handle parsing the sequence, and replacement
    std::vector<MapType> code_map = parseToCodeMap(sequence, PROMPT_EXPRESSION);
    return code_map;
}

/**
 * @brief Converts MCI Sequences to Code Maps for Prompt Formatting Strings
 * @param sequence
 * @return
 */
std::vector<MapType> IoSession::pipe2promptFormatCodeMap(const std::string &sequence) {
    // This will handle parsing the sequence, and replacement
    std::vector<MapType> code_map = parseToCodeMap(sequence, FORMAT_EXPRESSION);
    return code_map;
}



/**
 * @brief Parses unformatted prompt text and adds colors to brackets and colon's.
 * @param sequence
 * @param config
 * @return
 */
std::string IoSession::pipe2promptFormat(const std::string &sequence, Config &config) {
    std::vector<MapType> code_map = pipe2promptFormatCodeMap(sequence);
    std::string output;
    std::string key;
    std::string value;

    // Loop codes and build MCI Parsing List
    for (unsigned int i = 0; i < code_map.size(); i++) {
        auto &map = code_map[i];
        m_log.log(Logging::LogLevel::Debug, "[pipe2promptFormat] Menu Format Code=", map.m_code, __LINE__, __FILE__);

        // Control Codes are in Group 2
        switch (map.m_match) {
            case 1: // Handle [ text ] inside brackets
                // Build replacement here
                key = map.m_code;
                value = parseFormatColorsBrackets(map.m_code, config);
                addMCIMapping(key, value);
                break;

            case 2: // Handle :
                // Build replacement here
                key = map.m_code;
                value = parseFormatColorsColon(map.m_code, config);
                addMCIMapping(key, value);
                break;

            default:
                break;
        }
    }

    // Then feed though and return the updated string.
    output = parseCodeMapGenerics(sequence, code_map);
    return output;
}

/**
 * @brief Checks a String if it matches the expression passed.
 * @param sequence
 * @param expression
 * @return
 */
bool IoSession::checkRegex(const std::string &sequence, const std::string &expression) {
    std::smatch match;
    bool result = false;

    //std::cout << "exp: " << expression << std::endl;
    try {
        const std::regex regExpression(expression);
        result = std::regex_match(sequence, match, regExpression);
    } catch (std::regex_error &ex) {
        m_log.log(Logging::LogLevel::Error, "[checkRegex] Expression=", expression, "Exception=", ex.what(), ex.code(),
                  __LINE__, __FILE__);
    }

    return result;
}

/**
 * @brief Parses Text Prompt String Pair
 * @param prompt
 * @return
 */
std::string IoSession::parseTextPrompt(const M_StringPair &prompt) {
    // |PD is new for XRM, Prompt Description.
    std::string text_prompt = prompt.second;
    std::string mci_code = "|PD";

    // If Description Flag is in Prompt, then replace code with Description
    m_io_common.parseLocalMCI(text_prompt, mci_code, prompt.first);

    // Return full mci code parsing on the new string.
    return pipe2ansi(text_prompt);
}

/**
 * @brief Stores Key (MCI Code) Value (String for Replacement) in Mapping
 * @param key
 * @param value
 * @return
 */
void IoSession::addMCIMapping(const std::string &key, const std::string &value) {
    m_mapped_codes.insert(std::pair<std::string, std::string>(key, value));
}

/**
 * @brief Clears all mappings
 */
void IoSession::clearAllMCIMapping() {
    if (!m_mapped_codes.empty()) {
        m_mapped_codes.clear();
        std::map<std::string, std::string>().swap(m_mapped_codes);
    }
}

/**
 * @brief Get a Count of all Mapped MCI Codes
 * @return
 */
int IoSession::getMCIMappingCount() {
    return m_mapped_codes.size();
}
