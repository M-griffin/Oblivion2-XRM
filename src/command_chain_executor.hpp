#ifndef COMMAND_CHAIN_EXECUTOR_HPP
#define COMMAND_CHAIN_EXECUTOR_HPP

#include <string>
#include <vector>

#include "model-sys/menu.hpp"

class MenuSystem;

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

    void resultWithInput(const std::string &input);

    [[nodiscard]]
    bool isActive() const;

private:
    MenuSystem &m_menuSystem;
    CommandChainContext m_ctx;

    void execute();
};


#endif
