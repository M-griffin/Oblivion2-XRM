#include "form_manager.hpp"

#include "forms/form_base.hpp"
#include "forms/form_system_config.hpp"

#include <string>
#include <vector>

/**
 * @brief Clears All Forms
 */
void FormManager::clearAllForms()
{
    if (m_form.size() > 0)
    {
        std::vector<form_ptr>().swap(m_form);
    }
}

/**
 * @brief Exists and Shutsdown the current form
 */
void FormManager::shutdownForm()
{
    // Do module shutdown, only single modules are loaded
    // This makes it easy to allocate and kill on demand.
    m_form[0]->onExit();
    m_form.pop_back();
}

/**
 * @brief Start Up and execute current Form
 */
void FormManager::startupForm(form_ptr form)
{
    // First clear any left overs if they exist.
    clearAllForms();

    // Run the startup for the form
    form->onEnter();

    // Push to stack now the new form.
    m_form.push_back(form);
}

/**
 * @brief Start up the Configuration Form.
 */
void FormManager::startupFormSystemConfiguration()
{
    // Allocate and Create
    form_ptr form(new FormSystemConfig(m_config));
    if (!form)
    {
        std::cout << "FormSystemConfig Allocation Error!" << std::endl;
        assert(false);
    }

    startupForm(form);
}
