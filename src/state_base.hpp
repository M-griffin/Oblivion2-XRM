#ifndef THE_STATE_HPP
#define THE_STATE_HPP

#include "session_data.hpp"

#include <boost/smart_ptr/shared_ptr.hpp>

#include <iostream>
#include <string>

/**
 * @class StateBase
 * @author Michael Griffin
 * @date 9/1/2015
 * @file state_base.hpp
 * @brief Virtual Class to manager Individual Interfaces
 */
class StateBase
{
public:

    virtual ~StateBase()
    {
        std::cout << "~StateBase." << std::endl;
    }
    virtual void update(const std::string &character_buffer, const bool &is_utf8)  = 0;
    virtual bool onEnter() = 0;
    virtual bool onExit()  = 0;
    virtual void resume() {}
    virtual std::string getStateID() const = 0;

    explicit StateBase(session_data_ptr session_data)
        : m_session_data(session_data)
        , m_is_active(false)
    { }

    // This holds session data passed to each session.
    session_data_ptr m_session_data;
    bool             m_is_active;


    // All Data is saved to this buffer, which is then
    // Checked after each stage, onEnter, Update, onExit    
};

typedef boost::shared_ptr<StateBase> state_ptr;
typedef boost::weak_ptr<StateBase>	 state_wptr;


#endif // THE_STATE_HPP
