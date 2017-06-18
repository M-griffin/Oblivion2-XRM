#ifndef FORM_BASE_HPP
#define FORM_BASE_HPP

#include "../model-sys/config.hpp"
#include "../model-sys/menu.hpp"
#include "../session_io.hpp"


#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/smart_ptr/make_shared.hpp>

#include <yaml-cpp/yaml.h>

#include <iostream>
#include <string>


/**
 * @class FormBase
 * @author Michael Griffin
 * @date 31/03/2017
 * @file form_base.hpp
 * @brief Base Class for Form System
 */
class FormBase
{
public:

    virtual ~FormBase()
    {
        std::cout << "~FormBase." << std::endl;
    }

    virtual bool onEnter() = 0;
    virtual bool onExit() = 0;
    virtual void onSave() = 0;
    
    FormBase(config_ptr config, std::string name, std::string title, std::string pulldown_file,
            std::string ansi_top, std::string ansi_mid, std::string ansi_bot)
        : m_config(config)
        , m_name(name)
        , m_title(title)
        , m_pulldown_file(pulldown_file)
        , m_ansi_top(ansi_top)
        , m_ansi_mid(ansi_mid)
        , m_ansi_bot(ansi_bot)
    { }
        
    /**
     * @brief Updates the YAML Mapping Value along with Menu Option.
     * @param m
     * @param value
     */
    void updateNodeMapping(MenuOption &m, const std::string &value)
    {
        m.form_value = value;
        m_node[m.name] = value;        
    }
    
    /**
     * @brief Set the YAML::Node Mapping, this should be Smart Pointer.
     * @param data
     */
    template<typename T>
    void setNodeMappingType(T data)
    {
        m_node = data;
    }
    
    /**
     * @brief Retrieves all Mappings and return as Instance Class Pointer.
     * @param m
     * @param value
     */
    template<typename T>
    boost::shared_ptr<T> retrieveNodeMapping()
    {
        // Generate Update <T>_ptr from Node Mapping for T Save.
        T conf = m_node.as<T>();
        boost::shared_ptr<T> c = boost::make_shared<T>(conf);
        return c;
    }
            
    /**
     * @brief Clear All Menu Options
     */
    void baseClearOptions() 
    {
        std::vector<MenuOption>().swap(m_menu_options);
    }

    /**
     * @brief Gets the Default Color Sequence
     * @return 
     */
    std::string baseGetDefaultColor() 
    {
        return m_session_io.pipeColors(m_config->default_color_regular);
    }
    
    /**
     * @brief Gets the Default Input Color Sequence
     * @return 
     */
    std::string baseGetDefaultInputColor() 
    {
        return m_session_io.pipeColors(m_config->default_color_input);
    }
    
    /**
     * @brief Gets the Default Input Color Sequence
     * @return 
     */
    std::string baseGetDefaultInverseColor() 
    {
        return m_session_io.pipeColors(m_config->default_color_inverse);
    }    
    
    /**
     * @brief Pull Generated Menu Options
     * @return 
     */
    std::vector<MenuOption> baseGetFormOptions() 
    {
        return m_menu_options;
    }    
    
    /**
     * @brief Insert Generated Menu Options
     * @return 
     */
    void baseSetFormOption(MenuOption option) 
    {
        m_menu_options.push_back(option);
    }    
    
    /**
     * @brief Build Options, String
     * @param value
     */
    void baseBuildOptions(MenuOption &m, std::string value)
    {
        m.form_value = value;
        baseSetFormOption(m);
    }

    /**
     * @brief Build Options, Int to String
     * @param value
     */
    void baseBuildOptions(MenuOption &m, int value)
    {
        m.form_value = std::to_string(value);
        baseSetFormOption(m);        
    }

    /**
     * @brief Build Options, Bool to String
     * @param value
     */
    void baseBuildOptions(MenuOption &m, bool value)
    {
        m.form_value = (value) ? "true" : "false";
        baseSetFormOption(m);        
    }

    /**
     * @brief Build Options, Char to String
     * @param value
     */
    void baseBuildOptions(MenuOption &m, unsigned char value)
    {
        m.form_value = std::to_string(value);
        baseSetFormOption(m);        
    }
    
    /**
     * @brief Template for building Options List.
     * @param name
     * @param value
     */
    template <typename T>
    void setupBuildOptions(std::string name, T value) 
    {
        MenuOption opt;
        opt.name = name;
        baseBuildOptions(opt, value);
    }
                    
    config_ptr              m_config;
    std::string             m_name;
    std::string             m_title;
    std::string             m_pulldown_file;
    std::string             m_ansi_top;
    std::string             m_ansi_mid;
    std::string             m_ansi_bot;
    
    YAML::Node              m_node;
    SessionIO               m_session_io;
    
    // Holds all pulldown menu options.
    std::vector<MenuOption> m_menu_options;

};

typedef boost::shared_ptr<FormBase> form_ptr;


#endif // FORM_BASE_HPP
