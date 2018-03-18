#include "protocol_dao.hpp"
#include "../model-sys/protocol.hpp"

#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <mutex>
#include <cassert>

// Setup the file version for the file.
const std::string Protocols::FILE_VERSION = "1.0.0";

ProtocolDao::ProtocolDao(protocols_ptr prot, std::string path)
    : m_protocols(prot)
    , m_path(path)
    , m_filename("protocols")
{
    std::cout << "ProtocolDao" << std::endl;
}

ProtocolDao::~ProtocolDao()
{
    std::cout << "~ProtocolDao" << std::endl;
}

/**
 * @brief Helper, appends forward/backward slash to path
 * @param value
 */
void ProtocolDao::pathSeperator(std::string &value)
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
bool ProtocolDao::fileExists()
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
 * @param prot
 * @return
 */
bool ProtocolDao::saveConfig(protocols_ptr prot)
{
    std::string path = m_path;
    pathSeperator(path);    
    path.append(m_filename);
    path.append(".yaml");

    YAML::Emitter out;

    out << YAML::BeginMap;
    out << YAML::Flow;

    // Start Creating the Key/Value Output for the Config File.
    
    out << YAML::Key << "file_version" << YAML::Value << prot->file_version;
    
    // Loop and encode each menu option
    for(unsigned int i = 0; i < prot->protocols.size(); i++)
    {
        auto &opt = prot->protocols[i];
        
        out << YAML::Key << "protocols";
        out << YAML::Value << YAML::BeginMap;                        
        out << YAML::Key << "protocol_name" << YAML::Value << opt.protocol_name;
        out << YAML::Key << "protocol_type" << YAML::Value << opt.protocol_type;
        out << YAML::Key << "protocol_key" << YAML::Value << opt.protocol_key;
        out << YAML::Key << "protocol_path" << YAML::Value << opt.protocol_path;
        out << YAML::Key << "protocol_argument" << YAML::Value << opt.protocol_argument;
        out << YAML::Key << "protocol_isBatch" << YAML::Value << opt.protocol_isBatch;
        out << YAML::Key << "protocol_hasDSZLog" << YAML::Value << opt.protocol_hasDSZLog;        
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
void ProtocolDao::encode(const Protocols &rhs)
{      
    m_protocols->file_version    = rhs.file_version;    
    m_protocols->protocols       = rhs.protocols;
    
    // Now Sort All Protocols once they have been loaded.
    // Unforunatally YAML does not keep ordering in arrays properly.
    sort(
        m_protocols->protocols.begin( ), m_protocols->protocols.end( ), 
        [ ] (const Protocol& lhs, const Protocol& rhs)
    {
        return lhs.protocol_name < rhs.protocol_name;
    });
}

/**
 * @brief Loads a Configuation file into the m_protocol stub for access.
 * @return
 */
bool ProtocolDao::loadConfig()
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
        std::cout << "Protocols File Version: " << file_version << std::endl;
        if (file_version != Protocols::FILE_VERSION) {
            throw std::invalid_argument("Invalid file_version, expected: " + Protocols::FILE_VERSION);
        }
        
        // When doing node.as (all fields must be present on file)
        Protocols prot = node.as<Protocols>();

        // Moves the Loaded config to m_config shared pointer.
        encode(prot);
    }
    catch (YAML::Exception &ex)
    {
        std::cout << "YAML::LoadFile(protocols.yaml) " << ex.what() << std::endl;
        std::cout << "Most likely a required field in the config file is missing. " << std::endl;
        assert(false);
    }
    catch (std::exception &ex)
    {
        std::cout << "Unexpected YAML::LoadFile(protocols.yaml) " << ex.what() << std::endl;
        assert(false);
    }

    return true;
}

