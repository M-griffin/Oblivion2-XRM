#ifndef FORM_MANAGER_H
#define FORM_MANAGER_H

#include "model-sys/menu.hpp"

#include <boost/smart_ptr/shared_ptr.hpp>

#include <string>
#include <vector>


/**
 * Development Notes:
 *   Generate Menu and Commands for 
 *   Custom Listbar Configuration Interfaces
 *   We will hard code for initial dev and testing
 *   Create form folder with form codes for pulling each interface.
 *   and later customize
 * 
 *   A Custom Form Menu Load method will be needed in the menu base
 *   To use the data created here instead of trying to load a menu yaml.
 * 
 *   Special "new" menu commands will be added to handle lightbar selections
 *   And input fields processing when the option is selected..
 *   Fe.. Bools    YES/NO Bars
 *        String   Input Fields         (Covers single and multi-char / digit input)
 *        String   Hidden Input Fields
 * 
 *   Special Option, color selector like legacy obv/2  with right/left arror key movement.
 *   This will need some deep thought on best implimentation.
 */

class FormBase;
typedef boost::shared_ptr<FormBase> form_ptr;

class Config;
typedef boost::shared_ptr<Config> config_ptr;

/**
 * @class FormManager
 * @author Michael Griffin
 * @date 06/11/2016
 * @file form_manager.hpp
 * @brief Handles Form Creation and Form Management
 */
class FormManager
{
public:


    FormManager(config_ptr config)
        : m_config(config)
        , m_menu_info(new Menu())
        , max_cmds_per_page(0)
        , m_current_page(1)
        , m_total_pages(1)
        , m_form_name("")
    {
    }

    ~FormManager()
    {
    }

    
    // Handle Dynamic modules being executed.
    std::vector<form_ptr> m_form;
        
        
        
    /**
     * @brief Clears All Forms
     */
    void clearAllForms();

    /**
     * @brief Exists and Shutsdown the current form
     */
    void shutdownForm();
    
    /**
     * @brief Start up the Normal Login Process.
     */
    void startupFormSystemConfiguration();

    /**
     * @brief Start Up and execute current Form
     */
    void startupForm(form_ptr form);

    config_ptr       m_config;
    menu_ptr         m_menu_info;    // Custom Menus on the Fly
    int              max_cmds_per_page;
    int              m_current_page;
    int              m_total_pages;
    std::string      m_form_name;

};

#endif // FORM_MANAGER_H
