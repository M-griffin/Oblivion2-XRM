#ifndef MENU_HPP
#define MENU_HPP

#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/smart_ptr/weak_ptr.hpp>

#include <fstream>
#include <string>
#include <vector>
#include <iostream>

#include <yaml-cpp/yaml.h>


/**
 * @class MenuOption
 * @author Michael Griffin
 * @date 15/11/2016
 * @file menu.hpp
 * @brief XRM Menu Options Template for Yaml file format
 */
class MenuOption
{

public:

    MenuOption()
        : option_index(0)
        , option_name("New Option")
        , option_groups("")  
        , option_hidden(false)
        , option_input_key("")
        , option_cmd_key("")
        , option_cmd_string("")        
        , option_pulldown_id(0)
    { }
    
    ~MenuOption() { }
    
    int            option_index;
    std::string    option_name;
    std::string    option_groups;    
    bool           option_hidden;
    std::string    option_input_key;
    std::string    option_cmd_key;
    std::string    option_cmd_string;        
    int            option_pulldown_id;

};


/**
 * @class MenuOption
 * @author Michael Griffin
 * @date 15/11/2016
 * @file menu.hpp
 * @brief XRM Menu Options Template for Yaml file format
 */
class Menu
{

public:

    std::string file_version;
    std::string menu_name;
    std::string menu_password;
    std::string menu_fall_back;
    std::string menu_help_file;
    std::string menu_groups;
    std::string menu_prompt;
    std::string menu_title;
    std::string menu_pulldown_file;    
    
    std::vector<MenuOption> menu_options;

    Menu()
        : file_version("1.0.0")
        , menu_name("New Menu")
        , menu_password("")
        , menu_fall_back("")
        , menu_help_file("")
        , menu_groups("")
        , menu_prompt("")
        , menu_title("")
        , menu_pulldown_file("")    
    { }

    ~Menu() { }

};


// YAML Overide namespace for encoding/decoding Menu Class.
namespace YAML
{
    
    /**
     * @class convert
     * @author Blue
     * @date 16/11/2016
     * @file menu.hpp
     * @brief MenuOption convert for MenuOption Class
     */
    template<>
    struct convert<MenuOption>
    {
        /**
         * @brief Overide for encoding the Menu Class
         *        This is not used as it makes the map on a single line!
         *        Instead Key/Value is done in the ConfigDao on save.
         * @param rhs
         * @return
         */
        static Node encode(const MenuOption &rhs)
        {
            Node node;
            
            node["option_index"]        = rhs.option_index;
            node["option_name"]         = rhs.option_name;
            node["option_groups"]       = rhs.option_groups;  
            node["option_hidden"]       = rhs.option_hidden;
            node["option_input_key"]    = rhs.option_input_key;
            node["option_cmd_key"]      = rhs.option_cmd_key;
            node["option_cmd_string"]   = rhs.option_cmd_string;
            node["option_pulldown_id"]  = rhs.option_pulldown_id;
                            
            return node;
        }

        /**
         * @brief Overide for the Load from File, this read and piluaes the Menu Object.
         * @param node
         * @param rhs
         * @return
         */
        static bool decode(const Node& node, MenuOption& rhs)
        {            
            rhs.option_index       = node["option_index"].as<int>();
            rhs.option_name        = node["option_name"].as<std::string>();
            rhs.option_groups      = node["option_groups"].as<std::string>();
            rhs.option_hidden      = node["option_hidden"].as<bool>();
            rhs.option_input_key   = node["option_input_key"].as<std::string>();
            rhs.option_cmd_key     = node["option_cmd_key"].as<std::string>();
            rhs.option_cmd_string  = node["option_cmd_string"].as<std::string>();
            rhs.option_pulldown_id = node["option_pulldown_id"].as<int>();
                
            return true;
        }
    };
    
    
    /**
     * @class convert
     * @author Blue
     * @date 16/11/2016
     * @file menu.hpp
     * @brief YAML Conversion for Menu Class
     */
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
            node["file_version"] = rhs.file_version;
            node["menu_name"] = rhs.menu_name;            
            node["menu_password"] = rhs.menu_password;
            node["menu_fall_back"] = rhs.menu_fall_back;
            node["menu_help_file"] = rhs.menu_help_file;
            node["menu_groups"] = rhs.menu_groups;
            node["menu_prompt"] = rhs.menu_prompt;
            node["menu_title"] = rhs.menu_title;
            node["menu_pulldown_file"] = rhs.menu_pulldown_file;          

            // Child Nodes for storying menu options.
            Node option;                               
                        
            // Loop and encode each menu option
            for (auto &opt : rhs.menu_options) 
            {
                option["option_index"]        = opt.option_index;
                option["option_name"]         = opt.option_name;
                option["option_groups"]       = opt.option_groups;  
                option["option_hidden"]       = opt.option_hidden;
                option["option_input_key"]    = opt.option_input_key;
                option["option_cmd_key"]      = opt.option_cmd_key;
                option["option_cmd_string"]   = opt.option_cmd_string;
                option["option_pulldown_id"]  = opt.option_pulldown_id;
                                
                node["menu_option"].push_back(option);
            }

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
            rhs.file_version             = node["file_version"].as<std::string>();
            rhs.menu_name                = node["menu_name"].as<std::string>();
            rhs.menu_password            = node["menu_password"].as<std::string>();
            rhs.menu_fall_back           = node["menu_fall_back"].as<std::string>();
            rhs.menu_help_file           = node["menu_help_file"].as<std::string>();
            rhs.menu_groups              = node["menu_groups"].as<std::string>();
            rhs.menu_prompt              = node["menu_prompt"].as<std::string>();
            rhs.menu_title               = node["menu_title"].as<std::string>();
            rhs.menu_pulldown_file       = node["menu_pulldown_file"].as<std::string>();

            // Iterate through so we can get MAPS with the same key "menu_option".
            std::string key = "";
            for (YAML::const_iterator it = node.begin(); it != node.end(); ++it) {
                key.clear();                
                key = it->first.as<std::string>();
                
                if (key == "menu_option")
                {
                    rhs.menu_options.push_back(it->second.as<MenuOption>());
                }   
            }
                                
            return true;
        }
    };
}

typedef boost::shared_ptr<Menu> menu_ptr;
typedef boost::weak_ptr<Menu> menu_wptr;

#endif // MENU_HPP
