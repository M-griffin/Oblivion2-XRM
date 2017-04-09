#ifndef FORM_BASE_HPP
#define FORM_BASE_HPP

#include "../model/config.hpp"
#include "../model/menu.hpp"
#include "../session_io.hpp"

#include <boost/smart_ptr/shared_ptr.hpp>

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
    virtual bool onExit()  = 0;
    
    // Holds all pulldown menu options.
    std::vector<MenuOption> m_menu_options;

    FormBase(config_ptr config)
        : m_config(config)
    { }

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
        m.form_value = (value) ? "Yes" : "No";
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
                    
    config_ptr        m_config;        
    SessionIO         m_session_io;

};

typedef boost::shared_ptr<FormBase> form_ptr;


#endif // FORM_BASE_HPP
