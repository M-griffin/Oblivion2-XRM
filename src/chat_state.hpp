#ifndef CHAT_STATE_HPP
#define CHAT_STATE_HPP

#include "struct_compat.hpp"
#include "the_state.hpp"

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
 * @class ChatState
 * @author Michael Griffin
 * @date 9/1/2015
 * @file chat_state.hpp
 * @brief Chat Interface State.
 */
class ChatState
    : public TheState
{
public:

    ChatState(session_data_ptr session_data)
        : TheState(session_data)
        , m_next_state(0)
    { }

    virtual ~ChatState()
    {
        std::cout << "~ChatState" << std::endl;
    }

    virtual void update(std::string string_buffer, bool is_utf8);
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

};

#endif // CHAT_STATE_HPP
