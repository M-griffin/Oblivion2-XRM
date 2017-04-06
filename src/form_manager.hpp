#ifndef FORM_MANAGER_H
#define FORM_MANAGER_H

#include "model/menu.hpp"

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


    FormManager()
        : m_menu_info(new Menu())
        , max_cmds_per_page(0)
        , m_current_page(1)
        , m_total_pages(1)
        , m_form_name("")
    {
    }

    ~FormManager()
    {
    }


    menu_ptr         m_menu_info;    // Custom Menus on the Fly
    int              max_cmds_per_page;
    int              m_current_page;
    int              m_total_pages;
    std::string      m_form_name;

};

#endif // FORM_MANAGER_H
