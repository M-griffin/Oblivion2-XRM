#ifndef MOD_FORM_MANAGER_HPP
#define MOD_FORM_MANAGER_HPP

#include "mod_base.hpp"

#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include <functional>


class MenuOption;

class MenuBase;
typedef std::shared_ptr<MenuBase> menu_base_ptr;

//class FormManager;
//typedef std::shared_ptr<FormManager> form_manager_ptr;


/**
 * @class ModFormManager
 * @author Michael Griffin
 * @date 18/05/2017
 * @file mod_form_manager.hpp
 * @brief Form Module
 */
class ModFormManager
    : public ModBase
{
public:
    ModFormManager(session_ptr session_data, config_ptr config, processor_ansi_ptr ansi_process,
        common_io_ptr common_io, session_io_ptr session_io);

    virtual ~ModFormManager() override
    { 
        // std::cout << "~ModFormManager()" << std::endl;
    }

    virtual bool update(const std::string &character_buffer, const bool &) override;
    virtual bool onEnter() override;
    virtual bool onExit() override;

    /**
     * @brief Process Command Keys passed from menu selection (Callback)
     * @param option
     */
    bool menuOptionsCallback(const MenuOption &option);

    /**
     * @brief Starts up Form Manager Module.
     */
    void startupFormManager();

    int                m_current_page;
    menu_base_ptr      m_menu;
    //form_manager_ptr   m_form_manager;

};

#endif // MOD_FORM_MANAGER_HPP
