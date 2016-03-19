#ifndef MENU_MANAGER_HPP
#define MENU_MANAGER_HPP

#include "the_state.hpp"

#include <boost/enable_shared_from_this.hpp>

#include <iostream>
#include <string>
#include <vector>

/**
 * @class StateMachine
 * @author Michael Griffin
 * @date 9/1/2015
 * @file menu_manager.hpp
 * @brief State Manager to load the Menu System and pass session data connections.
 */
class MenuManager
{
public:
    MenuManager()
        : m_is_state_changed(false)
    {
        std::cout << "MenuManager Created" << std::endl;
    }

    ~MenuManager()
    {
        std::cout << "~MenuManager" << std::endl;
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

typedef boost::shared_ptr<MenuManager>	menu_manager_ptr;

#endif
