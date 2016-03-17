#ifndef MOD_LOGON_HPP
#define MOD_LOGON_HPP

#include "mod_base.hpp"

/**
 * @class ModLogin
 * @author Michael Griffin
 * @date 3/17/2016
 * @file mod_logon.hpp
 * @brief System Login Module
 */
class ModLogon
    : public ModBase
{
public:
    ModLogon(session_data_ptr session_data)
        : ModBase(session_data)
    {
        std::cout << "ModLogon" << std::endl;
    }

    virtual ~ModLogon()
    {
        std::cout << "~ModLogon" << std::endl;
    }

    virtual bool update(const std::string &character_buffer, const bool &);
    virtual bool onEnter();
    virtual bool onExit();

private:


};

#endif // SYSTEM_STATE_HPP
