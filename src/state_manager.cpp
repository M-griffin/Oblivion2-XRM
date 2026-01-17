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
    m_log.log(Logging::LogLevel::Debug, "StateManager() handleInput=", input);

    std::string result = std::string(input);
    std::size_t id1 = 0;

    // Convert CR\LF to LF!
    do {
        id1 = result.find("\r\n", 0);

        if (id1 != std::string::npos) {
            result.erase(id1, 1);
            id1 = result.find("\r\n", 0);
        }
    } while (id1 != std::string::npos);

    // PUSH ONE CHARACTER AT A TIME
    for (char ch : result) {
        std::string oneChar(1, ch);
        inputHandlers.at(currentState)(oneChar);
    }

    //inputHandlers.at(currentState)(result);
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

    // Menu System
    clearHandlers.emplace(State::MenuSystem,
                          [this]() { clearMenuSystem(); });

    createHandlers.emplace(State::MenuSystem,
                           [this]() { createMenuSystem(); });

    pollHandlers.emplace(State::MenuSystem,
                         [this]() { pollMenuSystem(); });

    inputHandlers.emplace(State::MenuSystem,
                          [this](const std::string &input) {
                              inputMenuSystem(input);
                          });

    // Runtime guarantee (debug)
    assert(clearHandlers.size() == StateCount);
    assert(createHandlers.size() == StateCount);
    assert(pollHandlers.size() == StateCount);
    assert(inputHandlers.size() == StateCount);
}

// Timers, if they exist, roll up from TCPSession
void StateManager::pollTimers() {
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
        preLogonState->onEnter();
    } catch (std::exception &ex) {
        std::cout << "createPreLogon Exception: " << ex.what() << std::endl;
        throw;
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

        if (!preLogonState->m_is_active) {
            m_log.log(Logging::LogLevel::Console, "StateManager() preLogonState is Inactive");
            setState(State::MenuSystem);
        }
    }
}

// -------------------------
// Menu System (Startup for Matrix, then Core System)
// -------------------------

void StateManager::createMenuSystem() {

    m_log.log(Logging::LogLevel::Console, "StateManager() createMenuSystem");

    // Make Sure we cover any unexpected errors in Creating the Module.
    try {
        menuSystemState.emplace(m_ctx);
        menuSystemState->onEnter();

    } catch (std::exception &ex) {
        std::cout << "createMenuSystem Exception: " << ex.what() << std::endl;
        throw;
    }

    std::cout << "MenuSystem created\n";
}

void StateManager::clearMenuSystem() {
    if (menuSystemState) {
        menuSystemState->onExit();
        menuSystemState = std::experimental::nullopt; // .reset();
    }
    std::cout << "MenuSystem cleared\n";
}

void StateManager::pollMenuSystem() {
    if (menuSystemState) {
        menuSystemState->pollMenuSystem();
    }
}

void StateManager::inputMenuSystem(const std::string &input) {

    m_log.log(Logging::LogLevel::Console, "StateManager() inputMenuSystem");
    if (menuSystemState) {
        menuSystemState->update(input, false);

        if (!menuSystemState->m_is_active) {
            m_log.log(Logging::LogLevel::Console, "StateManager() MenuSystem is Inactive");
            // setState(State::ModPreLogon);
            // After here, it's a system logoff usually, but maybe we'll swtich to a chat state.
            // but wouldn't want to shutdown and remove menu system!!
        }
    }
}
