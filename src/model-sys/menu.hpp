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

    explicit MenuOption()
        : index(0)
        , name("New Option")
        , acs_string("")  
        , hidden(false)
        , menu_key("")
        , command_key("")
        , command_string("")        
        , pulldown_id(0)
        , form_value("")
    { }
    
    ~MenuOption() { }
    
    int            index;
    std::string    name;
    std::string    acs_string;    
    bool           hidden;
    std::string    menu_key;
    std::string    command_key;
    std::string    command_string;        
    int            pulldown_id;
    std::string    form_value;

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

    static const std::string FILE_VERSION;

    std::string file_version;
    std::string menu_name;
    std::string menu_password;
    std::string menu_fall_back;
    std::string menu_help_file;
    std::string menu_acs_string;
    std::string menu_prompt;
    std::string menu_title;
    std::string menu_pulldown_file;
    bool        menu_form_menu;
    
    std::vector<MenuOption> menu_options;

    Menu()
        : file_version(FILE_VERSION)
        , menu_name("New Menu")
        , menu_password("")
        , menu_fall_back("")
        , menu_help_file("")
        , menu_acs_string("")
        , menu_prompt("")
        , menu_title("")
        , menu_pulldown_file("")
        , menu_form_menu(false)
    { }

    ~Menu() { }

};


// YAML Overide namespace for encoding/decoding Menu Class.
namespace YAML
{    
    /**
     * @class convert
     * @author Michael Griffin
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
            
            node["index"]            = rhs.index;
            node["name"]             = rhs.name;
            node["acs_string"]       = rhs.acs_string;  
            node["hidden"]           = rhs.hidden;
            node["menu_key"]         = rhs.menu_key;
            node["command_key"]      = rhs.command_key;
            node["command_string"]   = rhs.command_string;
            node["pulldown_id"]      = rhs.pulldown_id;
                           
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
            rhs.index           = node["index"].as<int>();
            rhs.name            = node["name"].as<std::string>();
            rhs.acs_string      = node["acs_string"].as<std::string>();
            rhs.hidden          = node["hidden"].as<bool>();
            rhs.menu_key        = node["menu_key"].as<std::string>();
            rhs.command_key     = node["command_key"].as<std::string>();
            rhs.command_string  = node["command_string"].as<std::string>();
            rhs.pulldown_id     = node["pulldown_id"].as<int>();
                
            return true;
        }
    };
    
    
    /**
     * @class convert
     * @author Michael Griffin
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
            node["file_version"]       = rhs.file_version;
            node["menu_name"]          = rhs.menu_name;            
            node["menu_password"]      = rhs.menu_password;
            node["menu_fall_back"]     = rhs.menu_fall_back;
            node["menu_help_file"]     = rhs.menu_help_file;
            node["menu_acs_string"]    = rhs.menu_acs_string;
            node["menu_prompt"]        = rhs.menu_prompt;
            node["menu_title"]         = rhs.menu_title;
            node["menu_pulldown_file"] = rhs.menu_pulldown_file;          

            // Child Nodes for storying menu options.
            Node option;                               
                        
            // Loop and encode each menu option
            for (auto &opt : rhs.menu_options) 
            {
                option["index"]            = opt.index;
                option["name"]             = opt.name;
                option["acs_string"]       = opt.acs_string;  
                option["hidden"]           = opt.hidden;
                option["menu_key"]         = opt.menu_key;
                option["command_key"]      = opt.command_key;
                option["command_string"]   = opt.command_string;
                option["pulldown_id"]      = opt.pulldown_id;
                                
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
            rhs.menu_acs_string          = node["menu_acs_string"].as<std::string>();
            rhs.menu_prompt              = node["menu_prompt"].as<std::string>();
            rhs.menu_title               = node["menu_title"].as<std::string>();
            rhs.menu_pulldown_file       = node["menu_pulldown_file"].as<std::string>();

            // Iterate through so we can get MAPS with the same key "menu_option".
            std::string key = "";
            for (YAML::const_iterator it = node.begin(); it != node.end(); ++it) 
            {
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
