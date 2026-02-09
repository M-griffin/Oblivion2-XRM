#ifndef IO_PIPES_COLORS_HPP
#define IO_PIPES_COLORS_HPP

#include <string>

#include "util_log.hpp"

class Config;

class IoPipesAndColors {

    UtilLog &m_log;

public:
    explicit IoPipesAndColors()
        : m_log(UtilLog::getInstance()) {
    }

    ~IoPipesAndColors() {
        m_log.log(UtilLog::LogLevel::Console, "~IoPipesAndColors()");
    }

    IoPipesAndColors(const IoPipesAndColors &other) = default;
    IoPipesAndColors(IoPipesAndColors &&other) = default;
    IoPipesAndColors & operator=(const IoPipesAndColors &other) noexcept = delete;
    IoPipesAndColors & operator=(IoPipesAndColors &&other) noexcept = delete;

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
    std::string parseFormatColorsBrackets(const std::string &sequence, Config &config);
    std::string parseFormatColorsColon(const std::string &sequence, Config &config);
};

#endif
