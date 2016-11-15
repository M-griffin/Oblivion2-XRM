#include "menu_dao.hpp"
#include "../model/menu.hpp"

#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/smart_ptr/make_shared.hpp>

#include <iostream>
#include <fstream>
#include <string>

#include <mutex>

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
 * @brief Check if the file exists and we need to create a new one.
 * @return
 */
bool MenuDao::fileExists()
{
    std::string path = m_path;
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
 * @brief Creates and Saves a newly Generated Menu File.
 * @param cfg
 * @return
 */
bool MenuDao::saveMenu(menu_ptr menu)
{
    std::string path = m_path;
    path.append(m_filename);

    YAML::Emitter out;

    out << YAML::BeginMap;
    out << YAML::Flow;

    // Start Creating the Key/Value Output for the Menu File.
    out << YAML::Key << "menu_name" << YAML::Value << menu->menu_name;
    
    
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
    m_menu->menu_name = rhs.menu_name;
    
    
}

/**
 * @brief Loads a Menu file into the m_menu stub for access.
 * @return
 */
bool MenuDao::loadMenu()
{
    std::string path = m_path;
    path.append(m_filename);

    YAML::Node node;

    // Load the file into the class.
    try
    {
        // Load file fresh.
        node = YAML::LoadFile(path);
        Menu m = node.as<Menu>();

        // Moves the Loaded config to m_config shared pointer.
        encode(m);
    }
    catch (std::exception ex)
    {
        std::cout << "Exception YAML::LoadFile(" << m_filename << ") " << ex.what() << std::endl;
        assert(false);
    }

    return true;
}

