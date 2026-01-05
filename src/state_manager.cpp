#include "state_manager.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <functional>

#include "logging.hpp"

StateManager::StateManager()
    : m_log(Logging::getInstance())
    , currentState(State::PreLogon) {
    bindStateHandlers();
    createHandlers[currentState]();
}

StateManager::~StateManager() {
    m_log.write<Logging::CONSOLE_LOG>("~StateManager()");
}


// Copy Constructor
StateManager::StateManager(const StateManager& other)
    : m_log(Logging::getInstance())
    , preLogonState(other.preLogonState),
      loggedInState(other.loggedInState),
      currentState(other.currentState) {
    bindStateHandlers(); // MUST rebind
}

StateManager& StateManager::operator=(const StateManager& other)
{
    if (this != &other)
    {
        preLogonState = other.preLogonState;
        loggedInState = other.loggedInState;
        currentState  = other.currentState;

        bindStateHandlers(); // MUST rebind
    }
    return *this;
}

// Move Constructor
StateManager::StateManager(StateManager&& other) noexcept
    : m_log(Logging::getInstance()),
      preLogonState(std::move(other.preLogonState)),
      loggedInState(std::move(other.loggedInState)),
      currentState(other.currentState) {
    bindStateHandlers(); // MUST rebind
}

StateManager& StateManager::operator=(StateManager&& other) noexcept
{
    if (this != &other)
    {
        preLogonState = std::move(other.preLogonState);
        loggedInState = std::move(other.loggedInState);
        currentState  = other.currentState;

        bindStateHandlers(); // MUST rebind
    }
    return *this;
}


void StateManager::setState(State newState) {
    clearHandlers[currentState]();
    currentState = newState;
    createHandlers[currentState]();
}

void StateManager::handleInput(const std::string &input) {
    inputHandlers[currentState](input);
}

// Binding
void StateManager::bindStateHandlers() {
    clearHandlers[State::PreLogon] =
            std::bind(&StateManager::clearPreLogon, this);
    createHandlers[State::PreLogon] =
            std::bind(&StateManager::createPreLogon, this);
    inputHandlers[State::PreLogon] =
            std::bind(&StateManager::inputPreLogon, this, std::placeholders::_1);

    clearHandlers[State::LoggedIn] =
            std::bind(&StateManager::clearLoggedIn, this);
    createHandlers[State::LoggedIn] =
            std::bind(&StateManager::createLoggedIn, this);
    inputHandlers[State::LoggedIn] =
            std::bind(&StateManager::inputLoggedIn, this, std::placeholders::_1);
}

// PreLogon
void StateManager::createPreLogon() {
    preLogonState.push_back({});
    std::cout << "PreLogon created\n";
}

void StateManager::clearPreLogon() {
    preLogonState.clear();
    std::cout << "PreLogon cleared\n";
}

void StateManager::inputPreLogon(const std::string &input) {
    preLogonState.back().username = input;
    std::cout << "Username set: " << input << "\n";

    // Example transition
    setState(State::LoggedIn);
}

// -------------------------
// LoggedIn
// -------------------------
void StateManager::createLoggedIn() {
    loggedInState.push_back({42});
    std::cout << "LoggedIn created\n";
}

void StateManager::clearLoggedIn() {
    loggedInState.clear();
    std::cout << "LoggedIn cleared\n";
}

void StateManager::inputLoggedIn(const std::string &input) {
    std::cout << "LoggedIn input: " << input << "\n";
}

/**

#include "StateManager.h"

int main()
{
StateManager sm;

sm.handleInput("Alice");
sm.handleInput("do_something");

return 0;
}
*/