#ifndef MENU_HPP
#define MENU_HPP

#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/smart_ptr/weak_ptr.hpp>
#include <boost/lexical_cast.hpp>

#include <fstream>

#include <yaml-cpp/yaml.h>

/**
 * @class Menu
 * @author Michael Griffin
 * @date 15/11/2016
 * @file menu.hpp
 * @brief XRM Menu Template for Yaml file format
 */
class Menu
{
    
public:
    

// string
    std::string menu_name;
    
    

    Menu()
        : menu_name("Default Menu")
        
    { }

    ~Menu() { }

};


// YAML Overide namespace for encoding/decoding Menu Class.
namespace YAML
{
    template<>
    struct convert<Menu>
    {
        /**
         * @brief Overide for encoding the Menu Class
         *        This is not used as it makes the map on a single line!
         *        Instead Key/Value is done in the ConfigDao on save.
         * @param rhs
         * @return
         */
        static Node encode(const Menu &rhs)
        {
            Node node;
            node["menu_name"] = rhs.menu_name;
            

            return node;
        }

        /**
         * @brief Overide for the Load from File, this read and piluaes the Menu Object.
         * @param node
         * @param rhs
         * @return
         */
        static bool decode(const Node& node, Menu& rhs)
        {
            rhs.menu_name                         = node["menu_name"].as<std::string>();           
            
            
            //rhs.bbs_name_sysop                  = node["bbs_name_sysop"].as<std::string>();           
            //rhs.port_telnet                     = node["port_telnet"].as<int>();
            //rhs.use_service_telnet              = node["use_service_telnet"].as<bool>();
            //rhs.use_service_ssl                 = node["use_service_ssl"].as<bool>();
            //rhs.hidden_input_character          = node["hidden_input_character"].as<unsigned char>();
            

            return true;
        }
    };
}

typedef boost::shared_ptr<Menu> menu_ptr;
typedef boost::weak_ptr<Menu> menu_wptr;

#endif // MENU_HPP
