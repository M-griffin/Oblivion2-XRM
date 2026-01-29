#include "io_session.hpp"

#include <regex>
#include <string>

#include "io_input_handler.hpp"
#include "io_pipes_colors.hpp"
#include "io_code_mapping.hpp"

#include "model-sys/config.hpp"

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

std::string IoSession::parseFilename(const std::string &pipe_code) {
    return m_io_code_mapping.parseFilename(pipe_code);
}

std::string IoSession::parseCodeMap(
        const std::string &screen, std::vector<CodeMapType> &code_map) {
    return m_io_code_mapping.parseCodeMap(screen, code_map);
}

std::string IoSession::parseCodeMapGenerics(
        const std::string &screen, const std::vector<CodeMapType> &code_map) {
    return m_io_code_mapping.parseCodeMapGenerics(screen, code_map);
}

std::vector<CodeMapType> IoSession::parseScreenBufferToCodeMap(
        const std::string &sequence, const std::regex &expression) {
    return m_io_code_mapping.parseScreenBufferToCodeMap(sequence, expression);
}

std::vector<CodeMapType> IoSession::parseToCodeMap(
        const std::string &sequence, const std::regex &expression) {
    return m_io_code_mapping.parseToCodeMap(sequence, expression);
}

std::string IoSession::pipe2ansi(const std::string &sequence) {
    return m_io_code_mapping.pipe2ansi(sequence);
}

std::vector<CodeMapType> IoSession::pipe2genericCodeMap(const std::string &sequence) {
    return m_io_code_mapping.pipe2genericCodeMap(sequence);
}

std::vector<CodeMapType> IoSession::pipe2promptCodeMap(const std::string &sequence) {
    return m_io_code_mapping.pipe2promptCodeMap(sequence);
}

std::vector<CodeMapType> IoSession::pipe2promptFormatCodeMap(const std::string &sequence) {
    return m_io_code_mapping.pipe2promptFormatCodeMap(sequence);
}

std::string IoSession::pipe2promptFormat(const std::string &sequence, Config &config) {
    return m_io_code_mapping.pipe2promptFormat(sequence, config);
}

bool IoSession::checkRegex(const std::string &sequence, const std::regex &expression) {
    return m_io_code_mapping.checkRegex(sequence, expression);
}

std::string IoSession::parseTextPrompt(const M_StringPair &prompt) {
    return m_io_code_mapping.parseTextPrompt(prompt);
}

void IoSession::addMCIMapping(const std::string &key, const std::string &value) {
    m_io_code_mapping.addMCIMapping(key, value);
}

void IoSession::clearAllMCIMapping() {
    m_io_code_mapping.clearAllMCIMapping();
}

int IoSession::getMCIMappingCount() {
    return m_io_code_mapping.getMCIMappingCount();
}
