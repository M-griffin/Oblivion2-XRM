#include <string>
#include <vector>

#include "model-sys/menu.hpp"

#include "menu_system.hpp"


CommandChainExecutor::CommandChainExecutor(MenuSystem &ms)
    : m_menuSystem(ms) {}

void CommandChainExecutor::start(std::vector<MenuOption> chain) {
    m_ctx = CommandChainContext{};
    m_ctx.chain = std::move(chain);
    execute();
}

void CommandChainExecutor::resumeWithInput(const std::string &input) {
    m_ctx.lastInput = input;
    m_ctx.waitingForInput = false;
    execute();
}

bool CommandChainExecutor::isActive() const {
    return m_ctx.index < m_ctx.chain.size();
}

bool CommandChainExecutor::isWaiting() const {
    return m_ctx.waitingForInput;
}

CommandChainContext &CommandChainExecutor::context() {
    return m_ctx;
}

void CommandChainExecutor::execute() {
    while (m_ctx.index < m_ctx.chain.size()) {
        auto &opt = m_ctx.chain[m_ctx.index];

        // Skip if previous failure and this option opts out
        if (m_ctx.failFlag && m_ctx.skip_on_fail) {
            ++m_ctx.index;
            continue;
        }

        ChainResult result =
            m_menuSystem.executeChainedCommand(opt, m_ctx);

        if (result == ChainResult::WaitingForInput) {
            m_ctx.waitingForInput = true;
            m_ctx.suppressPrompt = true;
            return;
        }

        if (result == ChainResult::AbortChain ||
            result == ChainResult::ReloadMenu ||
            result == ChainResult::ExitSystem) {
            return;
            }

        ++m_ctx.index;
    }
}

