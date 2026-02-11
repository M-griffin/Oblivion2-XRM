#ifndef IO_MAPPING_HPP
#define IO_MAPPING_HPP

#include <string>
#include <cwchar>

#include "io_pipes_colors.hpp"
#include "util_log.hpp"
#include "tcp_session_wrapper.hpp"
#include "data-sys/yml_text_prompts.hpp"
#include "model-sys/structures.hpp"

class IoCodeMapping {

    UtilLog &m_log;
    IoPipesAndColors m_io_pipes_and_colors;

    std::map<std::string, std::string> m_mapped_codes;

    const std::regex STD_EXPRESSION {
        "([|]{1}[0-9]{2})|([|]{1}[X][Y][0-9]{4})|"
        "([|]{1}[A-Z]{1,2}[0-9]{1,2})|([|]{1}[A-Z]{2})|"
        "([%]{2}[\\w]+[.]{1}[\\w]{3})|([%]{1}[A-Z]{2})|"
        "([%]{1}[0-9]{2})"
    };

    const std::regex MID_EXPRESSION  {"([|]{1}[A-Z]{1}[0-9]{1,2})|([|]{1}[A-Z]{2})"};
    const std::regex PROMPT_EXPRESSION {"([\\^]{1}[A-Z]{1})|([\\\\/=|@*:#)(]{1}$)"};
    const std::regex FORMAT_EXPRESSION {"([[]{1}[\\w\\W]+[]]{1})|([:]{1})"};

public:

    // Types for Text Prompt formatting to file.
    typedef std::pair<std::string, std::string> M_StringPair;

    explicit IoCodeMapping()
        : m_log(UtilLog::getInstance()) {
    }

    ~IoCodeMapping() {
        m_log.log(UtilLog::LogLevel::Console, "~IoCodeMapping()");
        m_mapped_codes.clear();
    }

    IoCodeMapping(const IoCodeMapping &other) = delete;
    IoCodeMapping(IoCodeMapping &&other) = delete;
    IoCodeMapping & operator=(const IoCodeMapping &other) noexcept = delete;
    IoCodeMapping & operator=(IoCodeMapping &&other) noexcept = delete;

    std::string parseFilename(const std::string &pipe_code);
    std::string parseCodeMap(const std::string &screen, std::vector<CodeMapType> &code_map);
    std::string parseCodeMapGenerics(const std::string &screen, const std::vector<CodeMapType> &code_map);
    std::vector<CodeMapType> parseScreenBufferToCodeMap(const std::string &sequence, const std::regex &expression);
    std::vector<CodeMapType> parseToCodeMap(const std::string &sequence, const std::regex &expression);
    std::string pipe2ansi(const std::string &sequence);
    std::vector<CodeMapType> pipe2genericCodeMap(const std::string &sequence);
    std::vector<CodeMapType> pipe2promptCodeMap(const std::string &sequence);
    std::vector<CodeMapType> pipe2promptFormatCodeMap(const std::string &sequence);
    std::string pipe2promptFormat(const std::string &sequence, Config &config);
    bool checkRegex(const std::string &sequence, const std::regex &expression);
    std::string parseTextPrompt(const M_StringPair &prompt);
    void addMCIMapping(const std::string &key, const std::string &value);
    void clearAllMCIMapping();
    int getMCIMappingCount();
};

#endif
