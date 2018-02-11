#include "menu_prompt_dao.hpp"
#include "../model-sys/menu_prompt.hpp"

#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <mutex>


// Setup the file version for the config file.
const std::string MenuPrompt::FILE_VERSION = "1.0.0";

MenuPromptDao::MenuPromptDao(menu_prompt_ptr menu_prompt, std::string menu_prompt_name, std::string path)
    : m_menu_prompt(menu_prompt)
    , m_path(path)
    , m_filename(menu_prompt_name)
{
    std::cout << "MenuPromptDao" << std::endl;
}

MenuPromptDao::~MenuPromptDao()
{
    std::cout << "~MenuPromptDao" << std::endl;
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

    std::cout << "menu_prompt_path: " << path << std::endl;

    std::ifstream ifs(path);
    if (!ifs.is_open())
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
    if (!ofs.is_open())
    {
        std::cout << "Error, unable to write to: " << path << std::endl;
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
    std::string path = m_path;
    pathSeperator(path);
    path.append(m_filename);
    path.append(".yaml");

    YAML::Node node;

    // Load the file into the class.
    try
    {
        // Load file fresh.
        node = YAML::LoadFile(path);

        // Testing Is on nodes always throws exceptions.
        if (node.size() == 0)
        {
            std::cout << "Not Found: " << path << std::endl;
            return false; //File Not Found?
        }

        std::string file_version = node["file_version"].as<std::string>();

        // Validate File Version
        std::cout << "MenuPrompt File Version: " << file_version << std::endl;
        if (file_version != MenuPrompt::FILE_VERSION)
        {
            throw std::invalid_argument("Invalid file_version, expected: " + MenuPrompt::FILE_VERSION);
        }

        MenuPrompt m = node.as<MenuPrompt>();

        // Moves the Loaded config to m_config shared pointer.
        encode(m);
    }
    catch (YAML::Exception &ex)
    {
        std::cout << "Exception YAML::LoadFile(" << m_filename << ".yaml) " << ex.what() << std::endl;
        std::cout << "Most likely a required field in the menu file is missing. " << std::endl;
        assert(false);
    }
    catch (std::exception &ex)
    {
        std::cout << "Unexpected YAML::LoadFile(" << m_filename << ".yaml) " << ex.what() << std::endl;
        assert(false);
    }

    return true;
}
