#ifndef MOD_BASE_HPP
#define MOD_BASE_HPP

#include "../session_data.hpp"

#include <boost/smart_ptr/shared_ptr.hpp>

#include <iostream>
#include <string>

/**
 * @class ModBase
 * @author Michael Griffin
 * @date 3/17/2016
 * @file mod_base.hpp
 * @brief Base Class for Module System
 */
class ModBase
{
public:

    virtual ~ModBase()
    {
        std::cout << "~ModBase." << std::endl;
    }
    virtual bool update(const std::string &character_buffer, const bool &is_utf8) = 0;
    virtual bool onEnter() = 0;
    virtual bool onExit()  = 0;

    ModBase(session_data_ptr session_data)
        : m_session_data(session_data)
        , m_is_active(false)
    { }

    // This holds session data passed to each session.
    // In modules we'll use the weak pointer so more clarity.
    session_data_ptr  m_session_data;
    bool              m_is_active;


    // All Data is saved to this buffer, which is then
    // Checked after each stage, onEnter, Update, onExit
};

typedef boost::shared_ptr<ModBase>	module_ptr;
typedef boost::weak_ptr<ModBase>	module_wptr;


#endif // THE_STATE_HPP
