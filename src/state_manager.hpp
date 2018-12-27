#ifndef STATE_MANAGER_HPP
#define STATE_MANAGER_HPP

#include "state_base.hpp"

#include <memory>
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
    {
    }

    ~StateManager()
    {
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

    template<typename octet_type>
    inline uint8_t mask8(octet_type oc)
    {
        return static_cast<uint8_t>(0xff & oc);
    }

    // List of Active States per session.
    std::vector<state_ptr>& getTermStates()
    {
        return m_the_state;
    }

private:

    std::vector<state_ptr> m_the_state;

};

typedef std::shared_ptr<StateManager>	state_manager_ptr;

#endif
