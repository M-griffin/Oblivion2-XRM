#include "menu_dao.hpp"
#include "../model-sys/menu.hpp"

#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <mutex>

// Setup the file version for the config file.
const std::string Menu::FILE_VERSION = "1.0.1";

MenuDao::MenuDao(menu_ptr menu, std::string menu_name, std::string path)
    : m_menu(menu)
    , m_path(path)
    , m_filename(menu_name)
{
    std::cout << "MenuDao" << std::endl;
}

MenuDao::~MenuDao()
{
    std::cout << "~MenuDao" << std::endl;
}


/**
 * @brief Helper, appends forward/backward slash to path
 * @param value
 */
void MenuDao::pathSeperator(std::string &value)
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
bool MenuDao::fileExists()
{
    std::string path = m_path;
    pathSeperator(path);    
    path.append(m_filename);
    path.append(".yaml");
    
    std::cout << "menu_path: " << path << std::endl;

    std::ifstream ifs(path);
    if (!ifs.is_open())
    {
        return false;
    }
    ifs.close();
    return true;
}


/**
 * @brief Creates and Saves a newly Generated Menu File.
 * @param cfg
 * @return
 */
bool MenuDao::saveMenu(menu_ptr menu)
{
    std::string path = m_path;
    pathSeperator(path);
    path.append(m_filename);
    path.append(".yaml");

    YAML::Emitter out;

    out << YAML::BeginMap;
    out << YAML::Flow;

    // Start Creating the Key/Value Output for the Menu File.    
    out << YAML::Key << "file_version" << YAML::Value << menu->file_version;
    out << YAML::Key << "menu_name" << YAML::Value << menu->menu_name;
    out << YAML::Key << "menu_password" << YAML::Value << menu->menu_password;
    out << YAML::Key << "menu_fall_back" << YAML::Value << menu->menu_fall_back;
    out << YAML::Key << "menu_help_file" << YAML::Value << menu->menu_help_file;
    out << YAML::Key << "menu_acs_string" << YAML::Value << menu->menu_acs_string;
    out << YAML::Key << "menu_prompt" << YAML::Value << menu->menu_prompt;
    out << YAML::Key << "menu_title" << YAML::Value << menu->menu_title;
    out << YAML::Key << "menu_pulldown_file" << YAML::Value << menu->menu_pulldown_file;

    // Loop and encode each menu option
    for(unsigned int i = 0; i < menu->menu_options.size(); i++)
    {
        auto &opt = menu->menu_options[i];
        
        out << YAML::Key << "menu_option";
        out << YAML::Value << YAML::BeginMap;
                
        out << YAML::Key << "index" << YAML::Value << opt.index;
        out << YAML::Key << "name" << YAML::Value << opt.name;
        out << YAML::Key << "acs_string" << YAML::Value << opt.acs_string;  
        out << YAML::Key << "hidden" << YAML::Value << opt.hidden;
        out << YAML::Key << "menu_key" << YAML::Value << opt.menu_key;
        out << YAML::Key << "command_key" << YAML::Value << opt.command_key;
        out << YAML::Key << "command_string" << YAML::Value << opt.command_string;
        out << YAML::Key << "pulldown_id" << YAML::Value << opt.pulldown_id;
        
        out << YAML::EndMap;
    }
            
    out << YAML::EndMap;


    // Setup file to Write out File.
    std::ofstream ofs(path);
    if (!ofs.is_open())
    {
        std::cout << "Error, unable to write to: " << path << std::endl;
        return false;
    }

    ofs << out.c_str();
    ofs.close();

    return true;
}


/**
 * @brief Moves the Loaded menu to the shared pointer.
 *        This can probably be redone lateron with copy constructors..
 * @param rhs
 * @return
 */
void MenuDao::encode(const Menu &rhs)
{   
    m_menu->file_version        = rhs.file_version;
    m_menu->menu_name           = rhs.menu_name;
    m_menu->menu_password       = rhs.menu_password;
    m_menu->menu_fall_back      = rhs.menu_fall_back;
    m_menu->menu_help_file      = rhs.menu_help_file;
    m_menu->menu_acs_string     = rhs.menu_acs_string;
    m_menu->menu_prompt         = rhs.menu_prompt;
    m_menu->menu_title          = rhs.menu_title;
    m_menu->menu_pulldown_file  = rhs.menu_pulldown_file;
    m_menu->menu_options        = rhs.menu_options;
    
    // Now Sort All Menu Options once they have been loaded.
    // Unforunatally YAML does not keep ordering properly.
    sort(
        m_menu->menu_options.begin( ), m_menu->menu_options.end( ), 
        [ ] (const MenuOption& lhs, const MenuOption& rhs)
    {
        return lhs.index < rhs.index;
    });

}

/**
 * @brief Loads a Menu file into the m_menu stub for access.
 * @return
 */
bool MenuDao::loadMenu()
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
            return false; //File Not Found?
        }
        
        std::string file_version = node["file_version"].as<std::string>();
        
        // Validate File Version
        std::cout << "Menu File Version: " << file_version << std::endl;
        if (file_version != Menu::FILE_VERSION) {
            throw std::invalid_argument("Invalid file_version, expected: " + Menu::FILE_VERSION);
        }
        
        Menu m = node.as<Menu>();

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

