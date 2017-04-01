#include "form_system_config.hpp"

#include "../model/config.hpp"


FormSystemConfig::FormSystemConfig(config_ptr config)
    : FormBase(config)
{
}

FormSystemConfig::~FormSystemConfig()
{
}

bool FormSystemConfig::onEnter() 
{
 
    return true;
}


bool FormSystemConfig::onExit()
{
    
    return true;
}
