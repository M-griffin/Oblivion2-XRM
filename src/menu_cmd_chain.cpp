#include <string>
#include <vector>

#include "model-sys/menu.hpp"

#include "menu_system.hpp"


CommandChainExecutor::CommandChainExecutor(MenuSystem &ms)
    : m_menuSystem(ms) {
}

void CommandChainExecutor::start(std::vector<MenuOption> chain) {
    m_ctx = CommandChainContext{};
    m_ctx.chain = std::move(chain);
    execute();
}

void CommandChainExecutor::resumeWithInput(const std::string &input) {
    CHAIN_TRACE(
        "[CHAIN] input='%s'",
        input.c_str());

    m_ctx.inputBuffer = input;
    m_ctx.waitingForInput = false;
}

bool CommandChainExecutor::isActive() const {
    bool isRunning = m_ctx.index < m_ctx.chain.size();
    if (!isRunning) {
        CHAIN_TRACE(
            "[CHAIN] Completed - TotalCommands=%d",
            m_ctx.chain.size());
    }
    return isRunning;
}

void CommandChainExecutor::execute() {
    CHAIN_TRACE(
        "[CHAIN] Started - TotalCommands=%d",
        m_ctx.chain.size());

    while (m_ctx.index < m_ctx.chain.size()) {
        auto &option = m_ctx.chain[m_ctx.index];

        CHAIN_TRACE(
            "[CHAIN] index=%zu key=%s fail=%d waiting=%d",
            m_ctx.index,
            option.command_key.c_str(),
            m_ctx.failFlag,
            m_ctx.waitingForInput);

        ChainResult result = m_menuSystem.executeChainedCommand(option, m_ctx);

        if (result == ChainResult::WaitingForInput) {
            // Returned Result we are jumping into input now.
            // Switch state to Chained Input for Field Input etc..
            // - Menu cmd for Field, Lightbar, Y/N Bars??S?S
            return;
        }

        ++m_ctx.index;
    }
}
