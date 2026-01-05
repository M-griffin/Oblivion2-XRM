#ifndef STATE_MANAGER_HPP
#define STATE_MANAGER_HPP

#include <vector>
#include <unordered_map>
#include <functional>
#include <string>

#include "logging.hpp"

class StateManager {
public:
    enum class State {
        PreLogon,
        LoggedIn
    };

    struct PreLogon {
        std::string username;
    };

    struct LoggedIn {
        int userId;
    };

    StateManager();

    StateManager(const StateManager& other);
    StateManager& operator=(const StateManager& other);

    StateManager(StateManager&& other) noexcept;
    StateManager& operator=(StateManager&& other) noexcept;

    ~StateManager();

    void setState(State newState);

    void handleInput(const std::string &input);

private:

    Logging &m_log;

    // -------------------------
    // Type aliases
    // -------------------------
    using StateHandler = std::function<void()>;
    using InputHandler = std::function<void(const std::string &)>;

    // State Modules
    std::vector<PreLogon> preLogonState;
    std::vector<LoggedIn> loggedInState;

    State currentState;

    // Dispatch tables
    std::unordered_map<State, StateHandler> clearHandlers;
    std::unordered_map<State, StateHandler> createHandlers;
    std::unordered_map<State, InputHandler> inputHandlers;

    // Binding
    void bindStateHandlers();

    // PreLogon state handlers
    void createPreLogon();

    void clearPreLogon();

    void inputPreLogon(const std::string &input);

    // LoggedIn state handlers
    void createLoggedIn();

    void clearLoggedIn();

    void inputLoggedIn(const std::string &input);
};

#endif
