#include "text_prompts_dao.hpp"

#include "logging.hpp"

#include <yaml-cpp/yaml.h>

#include <iostream>
#include <fstream>
#include <string>
#include <exception>
#include <cassert>

const std::string TextPromptsDao::FILE_VERSION = "1.0.1";
static bool is_version_displayed = false;

TextPromptsDao::TextPromptsDao(std::string path, const std::string &filename)
    : m_log(Logging::getInstance())
    , m_path(path)
    , m_filename(filename)
    , m_is_loaded(false)
{
    m_log.write<Logging::CONSOLE_LOG>("TextPromptsDao path=", m_path, "filename=", m_filename);
}

TextPromptsDao::~TextPromptsDao()
{
    m_is_loaded = false;    
    m_text_prompts.clear();    
    std::map<std::string, std::pair<std::string, std::string> >().swap(m_text_prompts);
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

    if(!ifs.is_open())
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

    if(!ofs.is_open())
    {
        m_log.write<Logging::ERROR_LOG>("Error, unable to write to=", path);
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

    // Write out and save the file directly, using yaml's write() encodes UTF-8 automatically.
    // This can Goof up ANSI Characters and Text Prompts in CP437.
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
        // Load file fresh.
        YAML::Node node = YAML::LoadFile(path);

        if(node.size() == 0)
        {
            m_log.write<Logging::ERROR_LOG>("TextPromptsDao Node size == 0", path);
            node.reset();
            return false;
        }

        std::string file_version = node["file_version"].as<std::string>();

        // Validate File Version
        if(!is_version_displayed)
        {
            m_log.write<Logging::CONSOLE_LOG>("Text Prompt File Version=", file_version);
            is_version_displayed = true;
        }

        if(file_version != TextPromptsDao::FILE_VERSION)
        {
            m_log.write<Logging::ERROR_LOG>("Text Prompt Invalid File Version=", file_version, "Expected=", TextPromptsDao::FILE_VERSION);
            return(false);
        }
        
        m_is_loaded = true;
        
        // Testing pre-loading methods, expand this lateron for a glocal static cache instance.
        // These prompts don't change, no reason to keep reloading them for each class and session instance separately.
        // Not a priority, but groundwork is all set for this, they need a map by filename to keep separate files loaded.
        cacheAllTextPrompts(node);
        node.reset();
    }
    catch(std::exception &ex)
    {
        m_log.write<Logging::ERROR_LOG>("Exception YAML::readPrompts=", m_filename, ex.what());
        return(false);
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

    if(!m_is_loaded || (m_text_prompts.size() == 0))
    {
        return temp;
    }
    
    return m_text_prompts[lookup];
}

/**
 * @brief Testing, display all nodes loaded
 */
void TextPromptsDao::displayAll()
{
    
    for(M_TextPrompt::const_iterator it = m_text_prompts.begin(); it != m_text_prompts.end(); ++it)
    {            
        std::string key = it->first;
        M_StringPair value = it->second;

        std::cout << "Cache: " << key << " -> " << key << ", " << value.first << ", " << value.second << std::endl;
    }
}

/**
 * @brief Testing, cahce all records to a map instad of reloading and searching.
 */
void TextPromptsDao::cacheAllTextPrompts(YAML::Node &node)
{
    try
    {
        for(YAML::const_iterator it = node.begin(); it != node.end(); ++it)
        {            
            // The first Entry on All Text Prompts is the File Version which doesn't have a key/Value Pair, and is just a string value.
            // Exclude This when pre-loading all propmpts.
            if (it->first.as<std::string>() != "file_version")
            {            
                std::string key = it->first.as<std::string>();
                M_StringPair value = it->second.as<M_StringPair>();
                m_text_prompts[key] = value;
            }
        }        
    }
    catch(std::exception &ex)
    {
        m_log.write<Logging::ERROR_LOG>("Exception cacheAllTextPrompts YAML::readPrompts=", m_filename, ex.what());
        throw ex;
    }
}

