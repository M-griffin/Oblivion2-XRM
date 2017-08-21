#ifndef PROTOCOLS_HPP
#define PROTOCOLS_HPP

#include <yaml-cpp/yaml.h>

#include <boost/smart_ptr/shared_ptr.hpp>

#include <string>
#include <vector>

/**
 * @class Protocol
 * @author Michael Griffin
 * @date 05/05/2017
 * @file protocols.hpp
 * @brief Individual Protocol Enteries
 */ 
class Protocol
{
public:

    explicit Protocol()    
        : protocol_name("")
        , protocol_type("") // up/down.
        , protocol_key("")
        , protocol_path("")
        , protocol_argument("")
        , protocol_isBatch(false)
        , protocol_hasDSZLog(false)
    {
    }
    
    Protocol(std::string name, std::string type, std::string key,
        std::string path, std::string argu, bool batch, bool dszlog)
        : protocol_name(name)
        , protocol_type(type) // Up/Download.
        , protocol_key(key)
        , protocol_path(path)
        , protocol_argument(argu)
        , protocol_isBatch(batch)
        , protocol_hasDSZLog(dszlog)
    {
    }
    ~Protocol()
    {        
    }
    
    std::string protocol_name;
    std::string protocol_type;
    std::string protocol_key;
    std::string protocol_path;
    std::string protocol_argument;
    bool protocol_isBatch;
    bool protocol_hasDSZLog;
};

/**
 * @class Protocols
 * @author Michael Griffin
 * @date 05/05/2017
 * @file protocols.hpp
 * @brief Container for All Protocols
 */
class Protocols
{
public:

    const static std::string FILE_VERSION;
        
    Protocols()
        : file_version(FILE_VERSION)
    {        
    }
    
    ~Protocols()
    {        
    }
    
    std::string file_version;
    std::vector<Protocol> protocols;            
};

// YAML Overide namespace for encoding/decoding Class.
namespace YAML
{
    template<>
    struct convert<Protocol>
    {
        /**
         * @brief Overide for encoding the Class
         *        This is not used as it makes the map on a single line!
         *        Instead Key/Value is done on save.
         * @param rhs
         * @return
         */
        static Node encode(const Protocol &rhs)
        {
            Node node;          
            node["protocol_name"] = rhs.protocol_name;
            node["protocol_type"] = rhs.protocol_type;
            node["protocol_key"] = rhs.protocol_key;
            node["protocol_path"] = rhs.protocol_path;
            node["protocol_argument"] = rhs.protocol_argument;
            node["protocol_isBatch"] = rhs.protocol_isBatch;
            node["protocol_hasDSZLog"] = rhs.protocol_hasDSZLog;
        
            return node;
        }

        /**
         * @brief Overide for the Load from File, this read Object.
         * @param node
         * @param rhs
         * @return
         */
        static bool decode(const Node& node, Protocol& rhs)
        {
            rhs.protocol_name      = node["protocol_name"].as<std::string>();
            rhs.protocol_type      = node["protocol_type"].as<std::string>();
            rhs.protocol_key       = node["protocol_key"].as<std::string>();
            rhs.protocol_path      = node["protocol_path"].as<std::string>();
            rhs.protocol_argument  = node["protocol_argument"].as<std::string>();
            rhs.protocol_isBatch   = node["protocol_isBatch"].as<bool>();
            rhs.protocol_hasDSZLog = node["protocol_hasDSZLog"].as<bool>();
 
            return true;
        }
    };
    
    
    template<>
    struct convert<Protocols>
    {
        /**
         * @brief Overide for encoding the Menu Class
         *        This is not used as it makes the map on a single line!
         *        Instead Key/Value is done on save.
         * @param rhs
         * @return
         */
        static Node encode(const Protocols &rhs)
        {
            Node node;
            node["file_version"]           = rhs.file_version;     

            // Child Nodes for storying menu options.
            Node prot;                               
                        
            // Loop and encode each menu option
            for (auto &p : rhs.protocols) 
            {
                prot["protocol_name"]      = p.protocol_name;
                prot["protocol_type"]      = p.protocol_type;
                prot["protocol_key"]       = p.protocol_key;
                prot["protocol_path"]      = p.protocol_path;
                prot["protocol_argument"]  = p.protocol_argument;
                prot["protocol_isBatch"]   = p.protocol_isBatch;
                prot["protocol_hasDSZLog"] = p.protocol_hasDSZLog;
                                
                node["protocols"].push_back(prot);
            }
            
            return node;
        }

        /**
         * @brief Overide for the Load from File, this read and piluaes the Menu Object.
         * @param node
         * @param rhs
         * @return
         */
        static bool decode(const Node& node, Protocols& rhs)
        {
            rhs.file_version = node["file_version"].as<std::string>();

            // Iterate through so we can get MAPS with the same key "access_levels".
            std::string key = "";
            for (YAML::const_iterator it = node.begin(); it != node.end(); ++it) 
            {
                key.clear();                
                key = it->first.as<std::string>();
                
                if (key == "protocols")
                {
                    rhs.protocols.push_back(it->second.as<Protocol>());
                }   
            }
                                
            return true;
        }
    };
}

typedef boost::shared_ptr<Protocols> protocols_ptr;

#endif // PROTOCOLS_HPP
