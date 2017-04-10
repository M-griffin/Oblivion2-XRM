#include "access_level_dao.hpp"
#include "../model-sys/access_levels.hpp"

#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/smart_ptr/make_shared.hpp>

#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <mutex>


// Setup the file version for the file.
const std::string AccessLevels::FILE_VERSION = "1.0.0";


AccessLevelDao::AccessLevelDao(access_level_ptr acl, std::string path)
    : m_access_level(acl)
    , m_path(path)
    , m_filename("access_levels")
{
    std::cout << "AccessLevelDao" << std::endl;
}

AccessLevelDao::~AccessLevelDao()
{
    std::cout << "~AccessLevelDao" << std::endl;
}

/**
 * @brief Helper, appends forward/backward slash to path
 * @param value
 */
void AccessLevelDao::pathSeperator(std::string &value)
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
bool AccessLevelDao::fileExists()
{
    std::string path = m_path;
    pathSeperator(path);    
    path.append(m_filename);
    path.append(".yaml");

    std::ifstream ifs(path);
    if (!ifs.is_open())
    {
        return false;
    }
    ifs.close();
    return true;
}


/**
 * @brief Creates and Saves a newly Generated Configuration File.
 * @param acl
 * @return
 */
bool AccessLevelDao::saveConfig(access_level_ptr acl)
{
    std::string path = m_path;
    pathSeperator(path);    
    path.append(m_filename);
    path.append(".yaml");

    YAML::Emitter out;

    out << YAML::BeginMap;
    out << YAML::Flow;

    // Start Creating the Key/Value Output for the Config File.
    
    out << YAML::Key << "file_version" << YAML::Value << acl->file_version;
    
    // Loop and encode each menu option
    for(unsigned int i = 0; i < acl->access_levels.size(); i++)
    {
        auto &opt = acl->access_levels[i];
        
        out << YAML::Key << "access_levels";
        out << YAML::Value << YAML::BeginMap;
                
        out << YAML::Key << "access_level" << YAML::Value << opt.access_level;
        out << YAML::Key << "access_name" << YAML::Value << opt.access_name;
        
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
 * @brief Moves the Loaded config to the shared pointer.
 *        This can probably be redone lateron with copy constructors..
 * @param rhs
 * @return
 */
void AccessLevelDao::encode(const AccessLevels &rhs)
{      
    m_access_level->file_version        = rhs.file_version;    
    m_access_level->access_levels       = rhs.access_levels;
    
    // Now Sort All AccessLevels once they have been loaded.
    // Unforunatally YAML does not keep ordering in arrays properly.
    sort(
        m_access_level->access_levels.begin( ), m_access_level->access_levels.end( ), 
        [ ] (const Level& lhs, const Level& rhs)
    {
        return lhs.access_level < rhs.access_level;
    });
}

/**
 * @brief Loads a Configuation file into the m_config stub for access.
 * @return
 */
bool AccessLevelDao::loadConfig()
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
        std::cout << "AccessLevels File Version: " << file_version << std::endl;
        if (file_version != AccessLevels::FILE_VERSION) {
            throw std::invalid_argument("Invalid file_version, expected: " + AccessLevels::FILE_VERSION);
        }
        
        // When doing node.as (all fields must be present on file)
        AccessLevels acl = node.as<AccessLevels>();

        // Moves the Loaded config to m_config shared pointer.
        encode(acl);
    }
    catch (YAML::Exception &ex)
    {
        std::cout << "YAML::LoadFile(access_level.yaml) " << ex.what() << std::endl;
        std::cout << "Most likely a required field in the config file is missing. " << std::endl;
        assert(false);
    }
    catch (std::exception &ex)
    {
        std::cout << "Unexpected YAML::LoadFile(access_level.yaml) " << ex.what() << std::endl;
        assert(false);
    }

    return true;
}

