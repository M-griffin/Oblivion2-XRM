#ifndef FORM_MANAGER_H
#define FORM_MANAGER_H

#include <boost/smart_ptr/shared_ptr.hpp>

#include <string>
#include <vector>

#include "common_io.hpp"
#include "session_io.hpp"

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

class Menu;
typedef boost::shared_ptr<Menu> menu_ptr;

class SessionData;
typedef boost::shared_ptr<SessionData> session_data_ptr;

class MenuOption;

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

    FormManager(config_ptr config, session_data_ptr session_data);
    ~FormManager();


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

    /**
     * @brief handles selected options for processing.
     * @param option
     */
    void processFormOption(MenuOption &option, std::string value);

    /**
     * @brief Calculates Pages in Vector of Menu Options.
     * @param current_page
     * @param list
     */
    void buildPageOptions(std::vector<MenuOption> &page_options, int current_page);
    
    /**
     * @brief Pulls Generate Menu Options from Form
     * @param option
     */
    menu_ptr retrieveFormOptions(int current_page);
        
    /**
     * @brief Processes a TOP Template Screen
     * @param screen
     * @return
     */
    std::string processTopFormTemplate(const std::string &screen);

    /**
     * @brief Processes a MID Template Screen
     * @param screen
     * @return
     */
    std::string processMidFormTemplate(const std::string &screen);

    /**
     * @brief SRT, MID, END screen processing
     * @return
     */
    std::string processFormScreens();


    config_ptr       m_config;
    session_data_ptr m_session_data;
    menu_ptr         m_menu_info;
    int              max_cmds_per_page;
    int              m_current_page;
    int              m_total_pages;
    std::string      m_form_name;
    std::string      m_ansi_top;
    std::string      m_ansi_mid;
    std::string      m_ansi_bot;
    int              m_box_top;
    int              m_box_bottom;
        
    std::vector<MenuOption> m_loaded_options;
    CommonIO         m_common_io;
    SessionIO        m_session_io;
};


typedef boost::shared_ptr<FormManager> form_manager_ptr;

#endif // FORM_MANAGER_H
