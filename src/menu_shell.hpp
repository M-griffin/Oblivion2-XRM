#ifndef MENU_SHELL_HPP
#define MENU_SHELL_HPP

#include "state_base.hpp"
#include "menu_base.hpp"

#include "model-sys/struct_compat.hpp"

#include <memory>
#include <string>
#include <vector>


class Logging;

class Session;
typedef std::shared_ptr<Session> session_ptr;


/**
 * @class MenuSystem
 * @author Michael Griffin
 * @date 10/11/2016
 * @file menu_system.hpp
 * @brief Main Menu System State Handles Core Loop
 */
class MenuShell
    : public StateBase
    , public MenuBase
{
public:
    explicit MenuShell(session_ptr session_data);
    ~MenuShell();

    virtual void update(const std::string &character_buffer, const bool &is_utf8) override;
    virtual bool onEnter() override;
    virtual bool onExit() override;

    virtual std::string getStateID() const override
    {
        return "Test";
    }

    Logging                  &m_log;
    static const std::string  m_stateID;
    std::vector<std::string>  m_system_fallback;


};

#endif // MENU_SHELL_HPP
