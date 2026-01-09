#ifndef STATE_MANAGER_HPP
#define STATE_MANAGER_HPP

#include <unordered_map>
#include <functional>
#include <string>

#include <experimental/optional>

#include "model-sys/context.hpp"
#include "mods/mod_prelogon.hpp"
#include "logging.hpp"

class StateManager {
public:
    enum class State : uint8_t {
        ModPreLogon = 0,
        LoggedIn,
        COUNT
    };

    struct LoggedIn {
        int userId = -1;
    };

    explicit StateManager(Context &ctx);

    ~StateManager();

    // Non-copyable (states are runtime-owned)
    StateManager(const StateManager &) = delete;

    StateManager &operator=(const StateManager &) = delete;

    // Movable
    StateManager(StateManager &&) = delete;

    StateManager &operator=(StateManager &&) = delete;

    void setState(State newState);

    void handleInput(const std::string &input);

    // Initial State Setup should be exposed.
    void createPreLogon();

    // Forwarding Deadline Timer Polls to the State.
    void pollTimers();

    // Core members
    Logging &m_log;
    Context &m_ctx;

private:
    // State storage (SINGLE ownership)
    std::experimental::optional<ModPreLogon> preLogonState;
    std::experimental::optional<LoggedIn> loggedInState;

    State currentState;

    // Type aliases
    using StateHandler = std::function<void()>;
    using InputHandler = std::function<void(const std::string &)>;

    // Dispatch tables
    std::unordered_map<State, StateHandler> clearHandlers;
    std::unordered_map<State, StateHandler> createHandlers;
    std::unordered_map<State, StateHandler> pollHandlers;
    std::unordered_map<State, InputHandler> inputHandlers;


    // Binding
    void bindStateHandlers();

    // PreLogon
    void clearPreLogon();

    void pollPreLogon();

    void inputPreLogon(const std::string &input);

    // LoggedIn
    void createLoggedIn();

    void clearLoggedIn();

    void pollLoggedIn();

    void inputLoggedIn(const std::string &input);

    // Compile-time guarantees
    static constexpr size_t StateCount =
            static_cast<size_t>(State::COUNT);

    static_assert(StateCount == 2,
                  "StateManager: handler tables must be updated when adding states");
};

#endif
