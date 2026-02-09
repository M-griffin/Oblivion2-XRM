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

enum class ChainResult {
    Continue,
    WaitingForInput,
    Done
};

struct CommandChainContext {
    std::vector<MenuOption> chain;
    size_t index = 0;

    bool waitingForInput = false;
    bool failFlag = false;

    std::string inputBuffer;
    std::string inputExpected;
};

class CommandChainExecutor {
public:
    explicit CommandChainExecutor(MenuSystem &ms);
    ~CommandChainExecutor() = default;

    void start(std::vector<MenuOption> chain);

    void resumeWithInput(const std::string &input);

    [[nodiscard]]
    bool isActive() const;

private:
    MenuSystem &m_menuSystem;
    CommandChainContext m_ctx;

    void execute();
};


#endif
