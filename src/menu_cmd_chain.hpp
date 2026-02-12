#ifndef COMMAND_CHAIN_EXECUTOR_HPP
#define COMMAND_CHAIN_EXECUTOR_HPP

#include <string>
#include <vector>

#include "model-sys/menu.hpp"
#include "util_log.hpp"

class MenuSystem;

#ifdef DEBUG_MENU_CHAIN
#define CHAIN_TRACE(...) Logging::getInstance().log(Logging::LogLevel::Info, __VA_ARGS__)
#else
#define CHAIN_TRACE(...)
#endif

/*
enum class ChainResult {
    Continue,
    WaitingForInput,
    Done
};*/

/*
struct CommandChainContext {
    std::vector<MenuOption> chain;
    size_t index = 0;

    bool waitingForInput = false;
    bool failFlag = false;
    bool suppressPrompt = false;

    std::string inputBuffer;
    std::string inputExpected;
    std::string wildcardBuffer; // captured from *
    std::string lastInput; // for &
};*/

enum class ChainResult {
    Continue,          // move to next command
    WaitingForInput,   // pause chain, await user input
    AbortChain,        // stop chain, return to menu
    ReloadMenu,        // stop chain, reload menu
    ExitSystem         // logoff / disconnect
};

struct CommandChainContext {
    std::vector<MenuOption> chain;   // immutable command list
    size_t index = 0;                // current execution index

    std::string wildcardBuffer;      // replaces ExecContext::wildcardBuffer
    std::string lastInput;            // replaces ExecContext::lastInput

    bool waitingForInput = false;     // replaces implicit pause semantics
    bool suppressPrompt = false;      // carried forward explicitly
    bool failFlag = false;            // replaces scattered fail logic
    bool skip_on_fail = false;
    bool abort_on_fail = false;
};

class MenuSystem;

class CommandChainExecutor {
public:
    explicit CommandChainExecutor(MenuSystem &ms);

    void start(std::vector<MenuOption> chain);
    void resumeWithInput(const std::string &input);

    bool isActive() const;
    bool isWaiting() const;

    void execute();

    CommandChainContext &context();

private:
    MenuSystem &m_menuSystem;
    CommandChainContext m_ctx;
};


#endif
