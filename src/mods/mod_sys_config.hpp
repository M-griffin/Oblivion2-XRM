#ifndef MOD_SYS_CONFIG_HPP
#define MOD_SYS_CONFIG_HPP

#include "mod_base.hpp"

#include "../session_data.hpp"
#include "../session_io.hpp"

#include <boost/smart_ptr/shared_ptr.hpp>

#include <vector>
#include <functional>


class MenuOption;
class MenuBase;
typedef boost::shared_ptr<MenuBase> menu_base_ptr;

class FormManager;
typedef boost::shared_ptr<FormManager> form_manager_ptr;

class Config;
typedef boost::shared_ptr<Config> config_ptr;

class AnsiProcessor;
typedef boost::shared_ptr<AnsiProcessor> ansi_process_ptr;


/**
 * @class ModSysConfig
 * @author Michael Griffin
 * @date 18/05/2017
 * @file mod_sys_config.hpp
 * @brief System Config Form Module
 */
class ModSysConfig
    : public ModBase
{
public:

    ModSysConfig(session_data_ptr session_data, config_ptr config, ansi_process_ptr ansi_process);
        
    virtual ~ModSysConfig() override
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
