#ifndef THE_STATE_HPP
#define THE_STATE_HPP

#include <memory>
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
    }
    virtual void update(const std::string &character_buffer, const bool &is_utf8)  = 0;
    virtual bool onEnter() = 0;
    virtual bool onExit()  = 0;
    virtual void resume() {}
    virtual std::string getStateID() const = 0;

    explicit StateBase()
        : m_is_active(false)
    { }

    // This holds session data passed to each session.
    bool m_is_active;

};

typedef std::shared_ptr<StateBase> state_ptr;
typedef std::weak_ptr<StateBase>   state_wptr;


#endif // THE_STATE_HPP
