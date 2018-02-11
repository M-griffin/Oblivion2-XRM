#ifndef TEXT_PROMPTS_DAO_HPP
#define TEXT_PROMPTS_DAO_HPP

#include <yaml-cpp/yaml.h>

#include <memory>
#include <iostream>
#include <fstream>
#include <string>
#include <map>


// Types for Text Prompt formatting to file.
typedef std::map<std::string, std::pair<std::string, std::string> > M_TextPrompt;
typedef std::pair<std::string, std::string> M_StringPair;

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

    TextPromptsDao(std::string path, std::string filename);
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


    std::string m_path;
    std::string m_filename;
    bool        m_is_loaded;

    YAML::Node  m_node;

};

typedef std::shared_ptr<TextPromptsDao> text_prompts_dao_ptr;

#endif // TEXT_PROMPTS_DAO_HPP
