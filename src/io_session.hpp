#ifndef SESSION_IO_HPP
#define SESSION_IO_HPP

#include <vector>
#include <string>
#include <utility>

#include "io_common.hpp"
#include "io_input_handler.hpp"
#include "io_pipes_colors.hpp"
#include "io_code_mapping.hpp"

#include "model-sys/config.hpp"
#include "model-sys/structures.hpp"

#include "session_writer.hpp"
#include "logging.hpp"


/**
 * @class IoSession
 * @author Michael Griffin
 * @date 9/21/2015
 * @file io_session.hpp
 * @brief Higher Level I/O specific to Menu Interfaces
 */
class IoSession {
    Logging &m_log;
    SessionWriter &m_session;
    IoCommon &m_io_common;

    IoInputHandler m_io_input_handler;
    IoPipesAndColors m_pipes_and_colors;
    IoCodeMapping m_io_code_mapping;

public:

    // Types for Text Prompt formatting to file.
    typedef std::pair<std::string, std::string> M_StringPair;

    IoSession(SessionWriter &session, IoCommon &common)
    : m_log(Logging::getInstance())
      , m_session(session)
      , m_io_common(common)
      , m_io_input_handler(session, common)
      , m_io_code_mapping(session, common) {
    }

    ~IoSession() {
        m_log.log(Logging::LogLevel::Console, "~IoSession()");
    }

    // Copy constructors
    IoSession &operator=(IoSession &) = delete;
    IoSession(const IoSession &) = delete;

    // Move Constructors
    IoSession(IoSession &&other) = delete;
    IoSession &operator=(IoSession &&other) = delete;

    // M_Io_Input_Handler
    std::string getFSEKeyInput(const std::string &character_buffer);
    std::string getKeyInput(const std::string &character_buffer);
    void createInputField(std::string &field_name, int &len);
    std::string getInputField(const std::string &character_buffer, // Input.
                              std::string &result, // Returned at [ENTER]
                              int length = 30, // Default 30 Bytes
                              std::string leadoff = "", // Default None
                              bool hidden = false); // Default Not hidden

    std::string pipeReplaceForeground(int foreground);
    std::string pipeReplaceBackground(int background);
    std::string pipeColors(const std::string &color_string);
    std::string getDefaultColor(Config &config);
    std::string getDefaultInputColor(Config &config);
    std::string getDefaultInverseColor(Config &config);
    std::string getDefaultPromptColor(Config &config);
    std::string getDefaultStatColor(Config &config);
    std::string getDefaultBoxColor(Config &config);
    std::string parsePipeWithCharsDigits(const std::string &code, int value);
    std::string separatePipeWithCharsDigits(const std::string &pipe_code);
    std::string parsePipeWithChars(const std::string &pipe_code);

    std::string parseFilename(const std::string &pipe_code);
    std::string parseCodeMap(const std::string &screen, std::vector<MapType> &code_map);
    std::string parseCodeMapGenerics(const std::string &screen, const std::vector<MapType> &code_map);
    std::vector<MapType> parseToCodeMap(const std::string &sequence, const std::regex &expression);
    std::string pipe2ansi(const std::string &sequence);
    std::vector<MapType> pipe2genericCodeMap(const std::string &sequence);
    std::vector<MapType> pipe2promptCodeMap(const std::string &sequence);
    std::vector<MapType> pipe2promptFormatCodeMap(const std::string &sequence);
    std::string parseFormatColorsBrackets(const std::string &sequence, Config &config);
    std::string parseFormatColorsColon(const std::string &sequence, Config &config);
    std::string pipe2promptFormat(const std::string &sequence, Config &config);
    bool checkRegex(const std::string &sequence, const std::regex &expression);
    std::string parseTextPrompt(const M_StringPair &prompt);
    void addMCIMapping(const std::string &key, const std::string &value);
    void clearAllMCIMapping();
    int getMCIMappingCount();
};

#endif
