#ifndef ACCESS_LEVELS_HPP
#define ACCESS_LEVELS_HPP

#include <yaml-cpp/yaml.h>

#include <string>
#include <vector>


/**
 * @class Level
 * @author Michael Griffin
 * @date 05/04/2017
 * @file access_levels.hpp
 * @brief Individual Access Levels
 */
class Level
{
public:

    Level()    
        : access_level(0)
        , access_name("")
    {
    }
    
    Level(int level, std::string name)    
        : access_level(level)
        , access_name(name)
    {
    }
    ~Level()
    {        
    }
    
    int         access_level;
    std::string access_name;
};

/**
 * @class AccessLevels
 * @author Michael Griffin
 * @date 05/04/2017
 * @file access_levels.hpp
 * @brief Container for All Access Levels
 */
class AccessLevels
{
public:

    const static std::string FILE_VERSION;
        
    AccessLevels()
        : file_version(FILE_VERSION)
    {        
    }
    
    ~AccessLevels()
    {        
    }
    
    std::string file_version;
    std::vector<Level> access_levels;            
};

// YAML Overide namespace for encoding/decoding Config Class.
namespace YAML
{
    template<>
    struct convert<Level>
    {
        /**
         * @brief Overide for encoding the Config Class
         *        This is not used as it makes the map on a single line!
         *        Instead Key/Value is done in the ConfigDao on save.
         * @param rhs
         * @return
         */
        static Node encode(const Level &rhs)
        {
            Node node;
            node["access_level"] = rhs.access_level;
            node["access_name"] = rhs.access_name;
      
            return node;
        }

        /**
         * @brief Overide for the Load from File, this read and piluaes the Config Object.
         * @param node
         * @param rhs
         * @return
         */
        static bool decode(const Node& node, Level& rhs)
        {
            rhs.access_level                 = node["access_level"].as<int>();
            rhs.access_name                  = node["access_name"].as<std::string>();          
            return true;
        }
    };
    
    
    template<>
    struct convert<AccessLevels>
    {
        /**
         * @brief Overide for encoding the Menu Class
         *        This is not used as it makes the map on a single line!
         *        Instead Key/Value is done in the ConfigDao on save.
         * @param rhs
         * @return
         */
        static Node encode(const AccessLevels &rhs)
        {
            Node node;
            node["file_version"]       = rhs.file_version;     

            // Child Nodes for storying menu options.
            Node level;                               
                        
            // Loop and encode each menu option
            for (auto &acl : rhs.access_levels) 
            {
                level["access_level"]            = acl.access_level;
                level["access_name"]             = acl.access_name;
                                
                node["access_levels"].push_back(level);
            }
            
            return node;
        }

        /**
         * @brief Overide for the Load from File, this read and piluaes the Menu Object.
         * @param node
         * @param rhs
         * @return
         */
        static bool decode(const Node& node, AccessLevels& rhs)
        {
            rhs.file_version             = node["file_version"].as<std::string>();

            // Iterate through so we can get MAPS with the same key "access_levels".
            std::string key = "";
            for (YAML::const_iterator it = node.begin(); it != node.end(); ++it) 
            {
                key.clear();                
                key = it->first.as<std::string>();
                
                if (key == "access_levels")
                {
                    rhs.access_levels.push_back(it->second.as<Level>());
                }   
            }
                                
            return true;
        }
    };
}

typedef boost::shared_ptr<AccessLevels> access_level_ptr;

#endif // ACCESS_LEVELS_HPP
