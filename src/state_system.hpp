#ifndef SYSTEM_STATE_HPP
#define SYSTEM_STATE_HPP

#include "state_base.hpp"
#include "menu_system.hpp"

/*
 * NOTES,  Each State is like a module or subsystem.
 * Each subsystem can have it own state machine on what is being
 * executed.
 *
 * Data Passed through update, is the received data from client.
 * Data return from update, is the output after input has processed.
 * This keeps the system running asynchronously, we should never
 * be looking and waiting for data in these modules.
 */

/**
 * @class SystemState
 * @author Michael Griffin
 * @date 9/1/2015
 * @file state_system.hpp
 * @brief System Interface Binds the MenuSystem And the SessionData.
 */
class StateSystem
    : public StateBase
{
public:
    StateSystem(session_data_ptr session_data)
        : StateBase(session_data)
        , m_next_state(0)
        , m_menu_state(session_data)
    {
        std::cout << "StateSystem" << std::endl;
    }

    virtual ~StateSystem()
    {
        std::cout << "~StateSystem" << std::endl;
    }

    virtual void update(const std::string &character_buffer, const bool &is_utf8);
    virtual bool onEnter();
    virtual bool onExit();

    virtual std::string getStateID() const
    {
        return m_menuID;
    }
    virtual int getNextState() const
    {
        return m_next_state;
    }

private:

    int m_next_state;
    static const std::string m_menuID;

    // Core Menu System
    MenuSystem m_menu_state;
};

#endif // SYSTEM_STATE_HPP
