#ifndef COMMAND_CHAIN_EXECUTOR_HPP
#define COMMAND_CHAIN_EXECUTOR_HPP

#include <string>
#include <deque>
#include <vector>

#include "model-sys/menu.hpp"
#include "util_log.hpp"

#ifdef DEBUG_MENU_CHAIN
#define CHAIN_TRACE(...) UtilLog::getInstance().log(UtilLog::LogLevel::Info, __VA_ARGS__)
#else
#define CHAIN_TRACE(...)
#endif

class MenuSystem;

enum class ChainResult {
    Continue,
    WaitingForInput,
    AbortChain,
    ReloadMenu,
    ExitSystem
};

struct CommandChainContext {
    std::deque<MenuOption> queue;  // dynamic injection queue

    std::string wildcardBuffer;
    std::string lastInput;

    bool waitingForInput = false;
    bool suppressPrompt = false;

    size_t executionCounter = 0;   // infinite loop guard
};

class CommandChainExecutor {
public:
    explicit CommandChainExecutor(MenuSystem &ms);

    void start(std::deque<MenuOption> chain);
    void start(std::vector<MenuOption> chain);
    void resumeWithInput(const std::string &input);

    bool isActive() const;
    bool isWaiting() const;

    void injectFront(const MenuOption &opt);
    void injectBack(const MenuOption &opt);

    void clear();

    CommandChainContext &context();

    void execute();

private:

    MenuSystem &m_menuSystem;
    CommandChainContext m_ctx;

    static constexpr size_t MAX_CHAIN_EXECUTIONS = 1024;
};

#endif
