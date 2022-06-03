#include "menu_prompt_dao.hpp"
#include "../model-sys/menu_prompt.hpp"
#include "../logging.hpp"

#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <mutex>
#include <cassert>

// Setup the file version for the config file.
const std::string MenuPrompt::FILE_VERSION = "1.0.0";

MenuPromptDao::MenuPromptDao(menu_prompt_ptr menu_prompt, 
                             const std::string &menu_prompt_name, 
                             const std::string &path)
    : m_menu_prompt(menu_prompt)
    , m_path(path)
    , m_filename(menu_prompt_name)
{
}

MenuPromptDao::~MenuPromptDao()
{
}

/**
 * @brief Helper, appends forward/backward slash to path
 * @param value
 */
void MenuPromptDao::pathSeperator(std::string &value)
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
bool MenuPromptDao::fileExists()
{
    std::string path = m_path;
    pathSeperator(path);
    path.append(m_filename);
    path.append(".yaml");

    std::ifstream ifs(path);

    if(!ifs.is_open())
    {
        return false;
    }

    ifs.close();
    return true;
}

/**
 * @brief Creates and Saves a newly Generated Menu Prompt File.
 * @param cfg
 * @return
 */
bool MenuPromptDao::saveMenuPrompt(menu_prompt_ptr menu_prompt)
{
    /**
     * Note YAML always outputs UTF-8, so for menu prompts
     * We need to keep the CP437 char set, so we write out
     * Yaml format manually so no conversion is done.
     */
    std::string path = m_path;
    pathSeperator(path);
    path.append(m_filename);
    path.append(".yaml");

    // Setup file to Write out File.
    std::ofstream ofs(path);

    if(!ofs.is_open())
    {
        Logging *log = Logging::instance();
        log->write<Logging::ERROR_LOG>("Error, unable to write menu prompt=", path, __FILE__, __LINE__);
        return false;
    }

    ofs << "file_version: \"" << menu_prompt->file_version << "\"" << std::endl;
    ofs << "name: \"" << menu_prompt->name << "\"" << std::endl;
    ofs << "data_line1: \"" << menu_prompt->data_line1 << "\"" << std::endl;
    ofs << "data_line2: \"" << menu_prompt->data_line2 << "\"" << std::endl;
    ofs << "data_line3: \"" << menu_prompt->data_line3 << "\"" << std::endl;

    ofs.close();

    return true;
}

/**
 * @brief Moves the Loaded menu prompt to the shared pointer.
 *        This can probably be redone lateron with copy constructors..
 * @param rhs
 * @return
 */
void MenuPromptDao::encode(const MenuPrompt &rhs)
{
    m_menu_prompt->file_version    = rhs.file_version;
    m_menu_prompt->name            = rhs.name;
    m_menu_prompt->data_line1      = rhs.data_line1;
    m_menu_prompt->data_line2      = rhs.data_line2;
    m_menu_prompt->data_line3      = rhs.data_line3;
}

/**
 * @brief Loads a Menu Prompt file into the m_menu_prompt stub for access.
 * @return
 */
bool MenuPromptDao::loadMenuPrompt()
{
    Logging *log = Logging::instance();
    std::string path = m_path;
    pathSeperator(path);
    path.append(m_filename);
    path.append(".yaml");

    YAML::Node node;

    // Load the file into the class.
    try
    {
        node = YAML::LoadFile(path);

        if(node.size() == 0)
        {
            log->write<Logging::ERROR_LOG>("YAML Node not found=", path, __FILE__, __LINE__);
            return false; //File Not Found?
        }

        std::string file_version = node["file_version"].as<std::string>();

        // Validate File Version
        log->write<Logging::DEBUG_LOG>("MenuPrompt File Version=", file_version);

        if(file_version != MenuPrompt::FILE_VERSION)
        {
            log->write<Logging::ERROR_LOG>("MenuPrompt File Version=", file_version,
                                            "Expected=", MenuPrompt::FILE_VERSION);
            return false;
        }

        MenuPrompt m = node.as<MenuPrompt>();
        encode(m);
    }
    catch(YAML::Exception &ex)
    {
        log->write<Logging::ERROR_LOG>("YAML::LoadFile=", m_filename, "Exception=", ex.what(), __LINE__, __FILE__);
        return false;
    }
    catch(std::exception &ex)
    {
        log->write<Logging::ERROR_LOG>("Unexpected YAML::LoadFile=", m_filename, "Exception=", ex.what(), __LINE__, __FILE__);
        return false;
    }

    return true;
}
