#ifndef TEXT_PROMPTS_DAO_HPP
#define TEXT_PROMPTS_DAO_HPP

#include <string>
#include <map>

#include <yaml-cpp/yaml.h>

class Logging;

// Types for Text Prompt formatting to file.
typedef std::pair<std::string, std::string> M_StringPair;
typedef std::map<std::string, M_StringPair> M_TextPrompt;

// Global Cache of Loaded Text Prompts
// extern M_TextPrompt m_text_prompts;
extern std::map<std::string, M_TextPrompt> TEXT_PROMPTS;

/**
 * @class TextPromptsDao
 * @author Michael Griffin
 * @date 3/24/2016
 * @file text_prompts_dao.hpp
 * @brief Serializes Text Prompts to .yaml files for Modules
 */
class TextPromptsDao {
public:
    static const std::string FILE_VERSION;

    Logging &m_log;
    std::string m_path;
    std::string m_filename;
    bool m_is_loaded;

    TextPromptsDao(const std::string &path, const std::string &filename);
    ~TextPromptsDao();

    // Disable copy semantics
    TextPromptsDao(const TextPromptsDao &) = delete;
    TextPromptsDao &operator=(const TextPromptsDao &) = delete;
    TextPromptsDao(TextPromptsDao &&other) = delete;
    TextPromptsDao &operator=(TextPromptsDao &&other) = delete;

    /**
     * @brief Check if the file exists and we need to create a new one.
     * @return
     */
    bool fileExists();

    /**
     * @brief Helper, appends forward/backward slash to path
     * @param value
     */
    void pathSeperator(std::string &value);

    /**
     * @brief Takes mapping of M_TextPrompt and writes out Key, (Value, Value).
     * @param value
     */
    void writeValue(M_TextPrompt &value);

    /**
     * @brief Read in the prompt file to the class.
     * @return
     */
    bool readPrompts();

    /**
     * @brief Retrieves Desc, Text A Pair of Text Prompt from yaml file.
     * @param lookup
     * @return
     */
    [[nodiscard]]
    M_StringPair getPrompt(const std::string &lookup) const;

    /**
     * @brief Testing, display all nodes in a file.
     */
    void displayAll() const;

    /**
     * @brief Testing, cache all records to a map instead of reloading and searching.
     */
    void cacheAllTextPrompts(YAML::Node &node) const;
};

#endif
