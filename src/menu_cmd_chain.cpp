#include <string>
#include <vector>

#include "menu_cmd_chain.hpp"

#include "model-sys/menu.hpp"
#include "menu_system.hpp"


CommandChainExecutor::CommandChainExecutor(MenuSystem &ms)
    : m_menuSystem(ms) {
}

void CommandChainExecutor::start(std::deque<MenuOption> chain) {
    clear();
    m_ctx.executionCounter = 0;
    for (auto &opt: chain)
        m_ctx.queue.push_back(std::move(opt));

    execute();
}

void CommandChainExecutor::start(std::vector<MenuOption> chain) {
    clear();
    m_ctx.executionCounter = 0;
    for (auto &opt: chain)
        m_ctx.queue.push_back(std::move(opt));

    execute();
}

void CommandChainExecutor::resumeWithInput(const std::string &input) {


    // Parse input here for ENTER or end of field input though.
    // Get LineInput and wait for ENTER.
    std::string key;
    std::string result = m_menuSystem.m_ctx.getIoSession().getInputField(
        input, key, Config::sMenuPrompt_length);

    // ESC was hit abort the input and resume.
    if (result == "aborted") {
        m_ctx.lastInput = "";
        m_ctx.waitingForInput = false;
        m_ctx.suppressPrompt = false;

        // complete remaining chain.
        execute();
        return;
    }

    if (result.empty() || result[0] == '\n') {
        // Key == 0 on [ENTER] pressed alone. then invalid!

        // Actual Input completed from FieldInput, Passback to execution.
        m_ctx.waitingForInput = false;
        m_ctx.suppressPrompt = false;
        m_ctx.lastInput = key;
        execute();
        return;
    }

    // Send back the single input received to show client key presses.
    // Only if return data shows a processed key returned.
    if (result != "empty") {
        std::string output = m_menuSystem.getDefaultInputColor();
        output.append(result);
        m_menuSystem.baseProcessAndDeliver(output);
    }
}

bool CommandChainExecutor::isActive() const {
    return !m_ctx.queue.empty();
}

bool CommandChainExecutor::isWaiting() const {
    return m_ctx.waitingForInput;
}

void CommandChainExecutor::injectFront(const MenuOption &opt) {
    m_ctx.queue.push_front(opt);
}

void CommandChainExecutor::injectBack(const MenuOption &opt) {
    m_ctx.queue.push_back(opt);
}

void CommandChainExecutor::clear() {
    m_ctx = CommandChainContext{};
}

CommandChainContext &CommandChainExecutor::context() {
    return m_ctx;
}

void CommandChainExecutor::execute() {
    while (!m_ctx.queue.empty()) {
        if (m_ctx.executionCounter++ > MAX_CHAIN_EXECUTIONS) {
            // Safety guard
            clear();
            return;
        }

        m_menuSystem.m_failFlag = false;
        MenuOption opt = m_ctx.queue.front();
        m_ctx.queue.pop_front();

        ChainResult result =
                m_menuSystem.executeChainedCommand(opt, m_ctx);

        if (m_menuSystem.m_pendingMenuJump) {
            clear();
            return;
        }

        switch (result) {
            case ChainResult::Continue:
                break;

            case ChainResult::WaitingForInput:
                m_ctx.waitingForInput = true;
                m_ctx.suppressPrompt = true;
                return;

            case ChainResult::AbortChain:
                clear();
                return;

            case ChainResult::ReloadMenu:
                //m_menuSystem.reloadMenu();
                //break;
                clear();          // stop chain immediately
                return;           // exit execute()

            case ChainResult::ExitSystem:
                clear();
                m_menuSystem.disconnectUser();
                return;
        }
    }
}
