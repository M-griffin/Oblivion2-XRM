#ifndef MENU_SHELL_HPP
#define MENU_SHELL_HPP

#include <string>
#include <vector>

#include "menu_base.hpp"

class Logging;
class Session;


/**
 * @class MenuSystem
 * @author Michael Griffin
 * @date 10/11/2016
 * @file menu_system.hpp
 * @brief Main Menu System State Handles Core Loop
 */
class MenuShell
    : public MenuBase
{
public:
    explicit MenuShell(Session &session);
    ~MenuShell();

    Logging                  &m_log;
    std::vector<std::string>  m_system_fallback;
};

#endif // MENU_SHELL_HPP
