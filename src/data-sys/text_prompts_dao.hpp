#ifndef TEXT_PROMPTS_DAO_HPP
#define TEXT_PROMPTS_DAO_HPP

#include <memory>
#include <iostream>
#include <fstream>
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
class TextPromptsDao
{
public:

    static const std::string FILE_VERSION;

    TextPromptsDao(std::string path, const std::string &filename);
    ~TextPromptsDao();

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
     * @brief Retrieves Desc, Text Pair of Text Prompt from yaml file.
     * @param lookup
     * @return
     */
    M_StringPair getPrompt(const std::string &lookup);

    /**
     * @brief Testing, display all nodes in a file.
     */
    void displayAll();
    
    /**
     * @brief Testing, cahce all records to a map instad of reloading and searching.
     */
    void cacheAllTextPrompts(YAML::Node &node);

    Logging     &m_log;
    std::string  m_path;
    std::string  m_filename;
    bool         m_is_loaded;        

};

typedef std::shared_ptr<TextPromptsDao> text_prompts_dao_ptr;

#endif // TEXT_PROMPTS_DAO_HPP
