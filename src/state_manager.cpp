#include "state_manager.hpp"

#include <iostream>
#include <experimental/optional>

#include "model-sys/context.hpp"

StateManager::StateManager(Context &ctx)
    : m_log(Logging::getInstance())
      , m_ctx(ctx)
      , currentState(State::ModPreLogon) {
    m_log.log(Logging::LogLevel::Console, "StateManager()");
    bindStateHandlers();
}

StateManager::~StateManager() {
    m_log.log(Logging::LogLevel::Console, "~StateManager()");
    clearHandlers.at(currentState)();
}

// -------------------------
// State Control
// -------------------------

void StateManager::setState(State newState) {
    clearHandlers.at(currentState)();
    currentState = newState;
    createHandlers.at(currentState)();
}

void StateManager::handleInput(const std::string &input) {
    m_log.log(Logging::LogLevel::Console, "StateManager() handleInput");
    inputHandlers.at(currentState)(input);
}

// -------------------------
// Binding
// -------------------------

void StateManager::bindStateHandlers() {
    clearHandlers.clear();
    createHandlers.clear();
    inputHandlers.clear();
    pollHandlers.clear();

    // PreLogon
    clearHandlers.emplace(State::ModPreLogon,
                          [this]() { clearPreLogon(); });

    createHandlers.emplace(State::ModPreLogon,
                           [this]() { createPreLogon(); });

    pollHandlers.emplace(State::ModPreLogon,
                         [this]() { pollPreLogon(); });

    inputHandlers.emplace(State::ModPreLogon,
                          [this](const std::string &input) {
                              inputPreLogon(input);
                          });

    // LoggedIn
    clearHandlers.emplace(State::LoggedIn,
                          [this]() { clearLoggedIn(); });

    createHandlers.emplace(State::LoggedIn,
                           [this]() { createLoggedIn(); });

    pollHandlers.emplace(State::LoggedIn,
                         [this]() { pollLoggedIn(); });

    inputHandlers.emplace(State::LoggedIn,
                          [this](const std::string &input) {
                              inputLoggedIn(input);
                          });

    // Runtime guarantee (debug)
    assert(clearHandlers.size() == StateCount);
    assert(createHandlers.size() == StateCount);
    assert(pollHandlers.size() == StateCount);
    assert(inputHandlers.size() == StateCount);
}

// Timers, if they exist, roll up from TCPSession
void StateManager::pollTimers() {
    //m_log.log(Logging::LogLevel::Console, "TCP Session Poll Timers");

    // Check if module has an active state timer that needs an update.
    //m_escTimer.isTriggered();
    pollHandlers.at(currentState)();
}

// -------------------------
// PreLogon
// -------------------------

void StateManager::createPreLogon() {
    m_log.log(Logging::LogLevel::Console, "StateManager() createPreLogon");

    // Make Sure we cover any unexpected errors in Creating the Module.
    try {
        preLogonState.emplace(m_ctx);

        m_log.log(Logging::LogLevel::Console, "StateManager() PreLogonState->onEnter()");
        preLogonState->onEnter();

        m_log.log(Logging::LogLevel::Console, "StateManager() PreLogon Created!");
    } catch (std::exception &ex) {
        std::cout << "createPreLogon Exception: " << ex.what() << std::endl;
    }
}

void StateManager::clearPreLogon() {
    if (preLogonState) {
        preLogonState->onExit();
        preLogonState = std::experimental::nullopt; // .reset();
    }
    std::cout << "PreLogon cleared\n";
}

void StateManager::pollPreLogon() {
    if (preLogonState) {
        preLogonState->pollTimers();
    }
}

void StateManager::inputPreLogon(const std::string &input) {
    m_log.log(Logging::LogLevel::Console, "StateManager() inputPreLogon");
    if (preLogonState) {
        preLogonState->update(input, false);
        m_log.log(Logging::LogLevel::Console, "StateManager() inputPreLogon Completed");
    }
}

// -------------------------
// LoggedIn
// -------------------------

void StateManager::createLoggedIn() {
    loggedInState.emplace();
    std::cout << "LoggedIn created\n";
}

void StateManager::clearLoggedIn() {
    loggedInState = std::experimental::nullopt; // .reset();
    std::cout << "LoggedIn cleared\n";
}

void StateManager::pollLoggedIn() {
    if (loggedInState) {

    }
}

void StateManager::inputLoggedIn(const std::string &input) {
    std::cout << "LoggedIn input: " << input << "\n";
}
