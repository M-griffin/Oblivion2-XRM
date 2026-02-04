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

void CommandChainExecutor::resultWithInput(const std::string &input) {
    m_ctx.inputBuffer = input;
    m_ctx.waitingForInput = false;
}

bool CommandChainExecutor::isActive() const {
    return m_ctx.index < m_ctx.chain.size();
}

void CommandChainExecutor::execute() {
    while (m_ctx.index < m_ctx.chain.size()) {
        auto &option = m_ctx.chain[m_ctx.index];

        ChainResult result = m_menuSystem.executeChainedCommand(option, m_ctx);

        if (result == ChainResult::WaitingForInput) {
            return;
        }

        ++m_ctx.index;
    }
}
