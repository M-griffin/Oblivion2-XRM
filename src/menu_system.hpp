#ifndef MENU_SYSTEM_HPP
#define MENU_SYSTEM_HPP

#include <unordered_map>
#include <functional>
#include <string>
#include <map>
#include <optional>

#include "model-sys/menu.hpp"
#include "menu_base.hpp"

// Mods
#include "mods/mod_logon.hpp"
#include "mods/mod_signup.hpp"
#include "mods/mod_menu_editor.hpp"

class Context;
class UtilLog;

/**
 * @class MenuSystem
 * @author Michael Griffin
 * @date 10/11/2016
 * @file menu_system.hpp
 * @brief Main Menu System State Handles Core Loop
 */
class MenuSystem :
        public MenuBase {
public:

    enum class State : uint8_t {
        MenuSystem = 0,
        ModLogon,
        ModSignup,
        ModMenuEditor,
        COUNT
    };

    std::string stateToString() const
    {
        switch (currentState)
        {
            case State::MenuSystem: return "MenuSystem";
            case State::ModLogon:   return "ModLogon";
            case State::ModSignup:  return "ModSignup";
            case State::ModMenuEditor:  return "ModMenuEditor";
            default:                return "Unknown";
        }
    }

    explicit MenuSystem(Context &ctx);
    ~MenuSystem();

    void update(const std::string &character_buffer, const bool &is_utf8);
    bool onEnter();
    bool onExit();
    bool pollTimers();

    // Dynamic Map of all Menu Option Command functions
    typedef std::function<bool(const MenuOption &option)> CommandFuncType;
    typedef std::map<char, CommandFuncType> MappedCommandFunctions;

    // Function Bind Helper
    template <typename T, typename Ret, typename... Args>
    auto bind_member(T* obj, Ret (T::*fn)(Args...)) {
        return [obj, fn](Args... args) -> Ret {
            return (obj->*fn)(std::forward<Args>(args)...);
        };
    }

    template <typename T, typename Ret, typename... Args>
    auto bind_member(const T* obj, Ret (T::*fn)(Args...) const) {
        return [obj, fn](Args... args) -> Ret {
            return (obj->*fn)(std::forward<Args>(args)...);
        };
    }

    // Module States
    std::optional<ModLogon> logonState;
    std::optional<ModSignup> signupState;
    std::optional<ModMenuEditor> menuEditorState;

    // Pointer to Current Active State
    State currentState;

    // Type aliases
    using StateHandler = std::function<void()>;
    using InputHandler = std::function<void(const std::string &)>;

    // Dispatch tables
    std::unordered_map<State, StateHandler> clearHandlers;
    std::unordered_map<State, StateHandler> createHandlers;
    std::unordered_map<State, StateHandler> pollHandlers;
    std::unordered_map<State, InputHandler> inputHandlers;

    // Holds map of Menu Option Commands for quick lookup and execution
    MappedCommandFunctions m_menu_command_functions;

    // Setup Current State or Transitions
    void setState(State newState);

    void bindStateHandlers();

    void reloadMenu();

    ChainResult executeChainedCommand(
        const MenuOption &option,
        CommandChainContext &ctx);

    bool menuOptionsControlCommands(const MenuOption &option);
    bool menuOptionsMultiNodeCommands(const MenuOption &option);
    bool menuOptionsMatrixCommands(const MenuOption &option);
    bool menuOptionsGlobalNewScanCommands(const MenuOption &option);
    void disconnectUser();
    bool menuOptionsMainMenuCommands(const MenuOption &option);
    bool menuOptionsDoorCommands(const MenuOption &option);
    bool menuOptionsSysopCommands(const MenuOption &option);
    bool menuOptionsNewUserVotingCommands(const MenuOption &option);
    bool menuOptionsConferenceEditorCommands(const MenuOption &option);
    bool menuOptionsDataAreaCommands(const MenuOption &option);
    bool menuOptionsEmailCommands(const MenuOption &option);
    bool menuOptionsFileCommands(const MenuOption &option);
    bool menuOptionsMessageCommands(const MenuOption &option);
    bool menuOptionsJoinConference(const MenuOption &option);
    bool menuOptionsQWKMailCommands(const MenuOption &option);
    bool menuOptionsTopTenListingCommands(const MenuOption &option);
    bool menuOptionsMessageBaseSponsorCommands(const MenuOption &option);
    bool menuOptionsFileBaseSponsorCommands(const MenuOption &option);
    bool menuOptionsVotingCommands(const MenuOption &option);
    bool menuOptionsColorSettingCommands(const MenuOption &option);
    bool menuOptionsCallback(const MenuOption &option);
    void setMenuBaseState(BaseState newState);
    BaseState getMenuBaseState();
    void startupExternalProcess(const std::string &cmdline);


    // -------------------------
    // Menu System
    // -------------------------
    void commitTransitions();

    void createMenuSystem();
    void clearMenuSystem();
    void pollMenuSystem();
    void inputMenuSystem(const std::string &input);

    // -------------------------
    // Logon Module
    // -------------------------
    void createLogon();
    void clearLogon();
    void pollLogon();
    void inputLogon(const std::string &input);

    // -------------------------
    // Signup Module
    // -------------------------
    void createSignup();
    void clearSignup();
    void pollSignup();
    void inputSignup(const std::string &input);

    // -------------------------
    // MenuEditor Module
    // -------------------------
    void createMenuEditor();
    void clearMenuEditor();
    void pollMenuEditor();
    void inputMenuEditor(const std::string &input);

    // Compile-time guarantees
    static constexpr size_t StateCount =
            static_cast<size_t>(State::COUNT);

    static_assert(StateCount == 4,
                  "MenuSystem: handler tables must be updated when adding states");
};

#endif
