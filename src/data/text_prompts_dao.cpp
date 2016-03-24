#include "text_prompts_dao.hpp"

#include <yaml-cpp/yaml.h>

#include <iostream>
#include <fstream>
#include <string>

TextPromptsDao::TextPromptsDao(std::string path, std::string filename)
    : m_path(path)
    , m_filename(filename)
{
}

TextPromptsDao::~TextPromptsDao()
{
}

/**
 * @brief Helper, appends forward/backward slash to path
 * @param value
 */
void TextPromptsDao::pathSeperator(std::string &value)
{
#ifdef _WIN32
    value.append("\\");
#else
    value.append("/");
#endif
}

/**
 * @brief Takes mapping of M_TextPrompt and writes out Key, (Value, Value).
 * @param value
 */
void TextPromptsDao::writeValue(M_TextPrompt &value)
{
    std::string path = m_path;
    pathSeperator(path);
    path.append(m_filename);

    std::ofstream ofs(path);
    if (!ofs.is_open())
    {
        std::cout << "Error, unable to write to: " << path << std::endl;
        return;
    }

    YAML::Emitter out;
    YAML::Node node;

    out << YAML::BeginMap;
    out << YAML::Flow;

    for(auto it = value.begin(); it != value.end(); ++it)
    {
        out << YAML::Key << (*it).first;
        out << YAML::Value << YAML::BeginSeq << (*it).second.first << (*it).second.second << YAML::EndSeq;
    }

    out << YAML::EndMap;

    // Write out and save the file.
    ofs << out.c_str();
    ofs.close();
}

/**
 * @brief Retrieves Desc, Text Pair of Text Prompt from yaml file.
 * @param lookup
 * @return
 */
M_StringPair TextPromptsDao::getPrompt(std::string &lookup)
{
    std::string path = m_path;
    pathSeperator(path);
    path.append(m_filename);

    YAML::Node node = YAML::LoadFile(path);
    M_StringPair temp;

    std::string key = "";
    for (YAML::const_iterator it = node.begin(); it != node.end(); ++it)
    {
        key = it->first.as<std::string>();

        // Check key for selected prompt to pull
        if (key.compare(lookup) == 0)
        {
            M_StringPair value = it->second.as<M_StringPair>();
            return value;
        }
    }

    return temp;
}

/**
 * @brief Testing, display all nodes in a file.
 */
void TextPromptsDao::displayAll()
{
    std::string path = m_path;
    pathSeperator(path);
    path.append(m_filename);

    YAML::Node node = YAML::LoadFile(path);
    M_StringPair temp;

    for (YAML::const_iterator it = node.begin(); it != node.end(); ++it)
    {
        std::string key = it->first.as<std::string>();                // <- key
        M_StringPair value = it->second.as<M_StringPair>();

        std::cout << key << " -> " << value.first << ", " << value.second << std::endl;
    }
}
