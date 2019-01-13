#ifndef MOD_SYS_CONFIG_HPP
#define MOD_SYS_CONFIG_HPP

#include "mod_base.hpp"

#include "../session_data.hpp"
#include "../session_io.hpp"

#include <memory>
#include <vector>
#include <functional>


class MenuOption;
class MenuBase;
typedef std::shared_ptr<MenuBase> menu_base_ptr;

class FormManager;
typedef std::shared_ptr<FormManager> form_manager_ptr;

class Config;
typedef std::shared_ptr<Config> config_ptr;

class ProcessorAnsi;
typedef std::shared_ptr<ProcessorAnsi> processor_ansi_ptr;


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

    ModFormManager(session_data_ptr session_data, config_ptr config, processor_ansi_ptr ansi_process);

    virtual ~ModFormManager() override
    { }

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
    form_manager_ptr   m_form_manager;
    SessionIO          m_session_io;

};

#endif // MOD_SYS_CONFIG_HPP
