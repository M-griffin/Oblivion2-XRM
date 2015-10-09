#ifndef STATE_MACHINE_HPP
#define STATE_MACHINE_HPP

#include "the_state.hpp"

#include <boost/enable_shared_from_this.hpp>
#include <boost/smart_ptr/shared_ptr.hpp>

#include <iostream>
#include <string>
#include <vector>

/**
 * @class StateMachine
 * @author Michael Griffin
 * @date 9/1/2015
 * @file state_machine.hpp
 * @brief State Machine, Manages Interfaces and Switching
 */
class StateMachine
    : public boost::enable_shared_from_this<StateMachine>
{
public:
    StateMachine()
        : m_is_state_changed(false)
    {
        std::cout << "StateMachine Created" << std::endl;
    }

    ~StateMachine()
    {
        std::cout << "~StateMachine" << std::endl;
        if(!m_the_state.empty())
        {
            m_the_state.back()->onExit();
            while(m_the_state.size() > 0)
            {
                m_the_state.pop_back();
            }
            m_the_state.clear();
        }
    }
    
    void update();
    void pushState(state_ptr &the_state);
    void changeState(state_ptr &the_state);
    void popState();
    void clean();

    // List of Active States per session.
    std::vector<state_ptr>& getTermStates()
    {
        return m_the_state;
    }

private:

    bool                   m_is_state_changed;
    std::vector<state_ptr> m_the_state;

};

typedef boost::shared_ptr<StateMachine>	state_machine_ptr;

#endif
