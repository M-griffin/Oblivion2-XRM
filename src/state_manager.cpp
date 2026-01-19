#include "state_manager.hpp"

#include <iostream>
#include <experimental/optional>
#include <utf8.h>

#include "model-sys/context.hpp"

StateManager::StateManager(Context &ctx)
    : m_log(Logging::getInstance())
      , m_ctx(ctx)
      , currentState(State::ModPreLogon) {
    m_log.log(Logging::LogLevel::Console, "StateManager()");
    bindStateHandlers();

    // Setup Inactivity timer, so no sessions can stay active indefinite.
    // Make Configuration String / Prompt Later On
    m_inactivityTimer.start(std::chrono::minutes(10), [this]() {
        m_ctx.getSessionWrite().send("\r\nInactive for 10 minutes, disconnecting.");
        m_ctx.getSessionWrite().hangup();
    });
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

    if (!m_ctx.sessionWriter->isActive()) {
        return;
    }

    // ReStart Inactivity timer on Input
    m_inactivityTimer.start(std::chrono::minutes(10), [this]() {
        // Make Configuration String / Prompt Later On
        m_ctx.getSessionWrite().send("\r\nInactive for 10 minutes, disconnecting.");
        m_ctx.getSessionWrite().hangup();
    });

    // Normalize CRLF → LF (ASCII-safe)
    std::string normalized = input;
    size_t pos;
    while ((pos = normalized.find("\r\n")) != std::string::npos) {
        normalized.erase(pos, 1);
    }

    // Split into UTF-8 glyphs
    auto it = normalized.begin();
    auto end = normalized.end();

    while (it != end) {
        auto glyph_start = it;

        try {
            utf8::next(it, end);
        } catch (utf8::exception &) {
            // Drop invalid byte and resync
            ++it;
            continue;
        }

        std::string glyph(glyph_start, it);
        inputHandlers.at(currentState)(glyph);
    }
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
    clearHandlers.emplace(State::ModPreLogon, [this]() { clearPreLogon(); });
    createHandlers.emplace(State::ModPreLogon, [this]() { createPreLogon(); });
    pollHandlers.emplace(State::ModPreLogon, [this]() { pollPreLogon(); });
    inputHandlers.emplace(State::ModPreLogon,
                          [this](const std::string &input) {
                              inputPreLogon(input);
                          });

    // Menu System
    clearHandlers.emplace(State::MenuSystem, [this]() { clearMenuSystem(); });
    createHandlers.emplace(State::MenuSystem, [this]() { createMenuSystem(); });
    pollHandlers.emplace(State::MenuSystem, [this]() { pollMenuSystem(); });
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
    if (!m_ctx.getSessionWrite().isActive()) {
        return;
    }

    // Check for Inactivity
    m_inactivityTimer.isTriggered();

    // Check for other Active Timers, DownStream.
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
        if (!preLogonState->m_is_active) {
            m_log.log(Logging::LogLevel::Console, "StateManager() preLogonState is Completed.");
            setState(State::MenuSystem);
            return;
        }

        preLogonState->update(input, false);
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
        if (!menuSystemState->m_is_active) {
            m_log.log(Logging::LogLevel::Console, "StateManager() MenuSystem is Inactive");
            return;
        }

        menuSystemState->update(input, false);
    }
}
