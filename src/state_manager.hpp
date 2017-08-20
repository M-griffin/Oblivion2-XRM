#ifndef STATE_MANAGER_HPP
#define STATE_MANAGER_HPP

#include "state_base.hpp"

#include <boost/enable_shared_from_this.hpp>

#include <iostream>
#include <string>
#include <vector>

/**
 * @class StateManager
 * @author Michael Griffin
 * @date 9/1/2015
 * @file state_manager.hpp
 * @brief State Manager to load the Menu System and pass session data connections.
 */
class StateManager
{
public:
    explicit StateManager()
        : m_is_state_changed(false)
    {
        std::cout << "StateManager Created" << std::endl;
    }

    ~StateManager()
    {
        std::cout << "~StateManager" << std::endl;
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

typedef boost::shared_ptr<StateManager>	state_manager_ptr;

#endif
