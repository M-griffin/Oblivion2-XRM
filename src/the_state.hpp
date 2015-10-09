#ifndef THE_STATE_HPP
#define THE_STATE_HPP

#include "session_data.hpp"

#include <boost/enable_shared_from_this.hpp>
#include <boost/smart_ptr/shared_ptr.hpp>

#include <iostream>
#include <string>

/**
 * @class TheState
 * @author Michael Griffin
 * @date 9/1/2015
 * @file the_state.hpp
 * @brief Virtual Class to manager Individual Interfaces
 */
class TheState
    : public boost::enable_shared_from_this<TheState>
{
public:

    virtual ~TheState()
    {
        std::cout << "~TheState." << std::endl;
    }
    virtual void update(std::string character_buffer, bool is_utf8)  = 0;
    virtual bool onEnter() = 0;
    virtual bool onExit()  = 0;
    virtual void resume() {}
    virtual std::string getStateID() const = 0;

    TheState(session_data_ptr session_data)
        : m_session_data(session_data)
        , m_is_active(false)
    { }

    // This holds session data passed to each session.
    session_data_ptr m_session_data;
    bool             m_is_active;


    // All Data is saved to this buffer, which is then
    // Checked after each stage, onEnter, Update, onExit    
};

typedef boost::shared_ptr<TheState>	state_ptr;
typedef boost::weak_ptr<TheState>	state_wptr;


#endif // THE_STATE_HPP
