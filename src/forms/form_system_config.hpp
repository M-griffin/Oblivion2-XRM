#ifndef FORM_SYSTEM_CONFIG_HPP
#define FORM_SYSTEM_CONFIG_HPP

#include "form_base.hpp"

class Config;
typedef boost::shared_ptr<Config> config_ptr;

/**
 * @class FormSystemConfig
 * @author Michael Griffin
 * @date 31/03/2017
 * @file form_system_config.hpp
 * @brief Generates the System Configuration layout
 */
class FormSystemConfig
    : public FormBase
{
public:
    FormSystemConfig(config_ptr config)
        : FormBase(config)
    {
    }

    ~FormSystemConfig()
    {
    }

    virtual bool onEnter();
    virtual bool onExit();
    

};

#endif // FORM_SYSTEM_CONFIG_HPP
