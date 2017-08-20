#ifndef MENU_PROMPT_HPP
#define MENU_PROMPT_HPP

#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/smart_ptr/weak_ptr.hpp>

#include <fstream>
#include <string>
#include <vector>
#include <iostream>

#include <yaml-cpp/yaml.h>


/**
 * @class MenuPrompt
 * @author Michael Griffin
 * @date 05/03/2017
 * @file menu_prompt.hpp
 * @brief XRM Menu Prompt Template for Yaml file format
 */
class MenuPrompt
{

public:

    static const std::string FILE_VERSION;

    std::string file_version;
    std::string name;
    std::string data_line1;
    std::string data_line2;
    std::string data_line3;

    explicit MenuPrompt()
        : file_version(FILE_VERSION)
        , name("Menu Prompt Name")
        , data_line1("")
        , data_line2("")
        , data_line3("")
    { }

    ~MenuPrompt() { }
};


// YAML Overide namespace for encoding/decoding Menu Class.
namespace YAML
{
/**
 * @class convert
 * @author Michael Griffin
 * @date 16/11/2016
 * @file menu.hpp
 * @brief MenuPrompt convert for MenuPrompt Class
 */
template<>
struct convert<MenuPrompt>
{
    /**
     * @brief Overide for encoding the Menu Class
     *        This is not used as it makes the map on a single line!
     *        Instead Key/Value is done in the ConfigDao on save.
     * @param rhs
     * @return
     */
    static Node encode(const MenuPrompt &rhs)
    {
        Node node;
        node["file_version"]     = rhs.file_version;
        node["name"]             = rhs.name;
        node["data_line1"]       = rhs.data_line1;
        node["data_line2"]       = rhs.data_line2;
        node["data_line3"]       = rhs.data_line3;

        return node;
    }

    /**
     * @brief Overide for the Load from File, this read and piluaes the Menu Object.
     * @param node
     * @param rhs
     * @return
     */
    static bool decode(const Node& node, MenuPrompt& rhs)
    {
        rhs.file_version    = node["file_version"] .as<std::string>();
        rhs.name            = node["name"].as<std::string>();
        rhs.data_line1      = node["data_line1"].as<std::string>();
        rhs.data_line2      = node["data_line2"].as<std::string>();
        rhs.data_line3      = node["data_line3"].as<std::string>();

        return true;
    }
};
}

typedef boost::shared_ptr<MenuPrompt> menu_prompt_ptr;
typedef boost::weak_ptr<MenuPrompt> menu_prompt_wptr;

#endif // MENU_HPP
