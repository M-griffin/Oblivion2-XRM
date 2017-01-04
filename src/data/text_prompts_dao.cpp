#include "text_prompts_dao.hpp"

#include <yaml-cpp/yaml.h>

#include <iostream>
#include <fstream>
#include <string>
#include <exception>

const std::string TextPromptsDao::FILE_VERSION = "1.0.1";

TextPromptsDao::TextPromptsDao(std::string path, std::string filename)
    : m_path(path)
    , m_filename(filename)
    , m_is_loaded(false)
{
    std::cout << "TextPromptsDao" << std::endl;
}

TextPromptsDao::~TextPromptsDao()
{
    std::cout << "~TextPromptsDao" << std::endl;
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
 * @brief Check if the file exists and we need to create a new one.
 * @return
 */
bool TextPromptsDao::fileExists()
{
    std::string path = m_path;
    pathSeperator(path);
    path.append(m_filename);

    std::ifstream ifs(path);
    if (!ifs.is_open())
    {
        return false;
    }
    ifs.close();
    return true;
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

    // Clear and start fresh
    m_is_loaded = false;    

    out << YAML::BeginMap;
    out << YAML::Flow;

    out << YAML::Key << "file_version" << YAML::Value << FILE_VERSION;

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
 * @brief Read in the prompt file to the class.
 * @return
 */
bool TextPromptsDao::readPrompts()
{
    std::string path = m_path;
    pathSeperator(path);
    path.append(m_filename);

    // Load the file into the class.
    try
    {
        // Clear and Existing Nodes first beload loading.
        if (m_node.size() > 0)
        {
            m_node.reset();
        }

        // Load file fresh.
        m_node = YAML::LoadFile(path);  

        if (m_node.size() == 0) 
        {
            return false; //File Not Found?
        }      
        
        std::string file_version = m_node["file_version"].as<std::string>();
        
        // Validate File Version
        std::cout << "Text Prompt File Version: " << file_version << std::endl;
        if (file_version != TextPromptsDao::FILE_VERSION) {
            throw std::invalid_argument("Invalid file_version, expected: " + TextPromptsDao::FILE_VERSION);
        }
                
        m_is_loaded = true;
    }
    catch (std::exception &ex)
    {
        std::cout << "Exception YAML::readPrompts(" << m_filename << ") " << ex.what() << std::endl;
        assert(false);
    }
    return m_is_loaded;
}


/**
 * @brief Retrieves Desc, Text Pair of Text Prompt from yaml file.
 * @param lookup
 * @return
 */
M_StringPair TextPromptsDao::getPrompt(const std::string &lookup)
{
    M_StringPair temp;

    if (!m_is_loaded)
    {
        return temp;
    }

    std::string key = "";
    for (YAML::const_iterator it = m_node.begin(); it != m_node.end(); ++it)
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
 * @brief Testing, display all nodes loaded
 */
void TextPromptsDao::displayAll()
{
    for (YAML::const_iterator it = m_node.begin(); it != m_node.end(); ++it)
    {
        std::string key = it->first.as<std::string>();
        M_StringPair value = it->second.as<M_StringPair>();

        std::cout << key << " -> " << value.first << ", " << value.second << std::endl;
    }
}
