#include "menu_base.hpp"

#include <stdint.h>
#include <string>
#include <vector>

/**
 * @Brief This Is an Extension of the Menu System to include but branch off the editor
 */


/**
 * @brief Clears out the Loaded Menu Prompts.
 */
void MenuBase::clearAllMenuPrompts()
{
    while(m_loaded_menu_prompts.size() > 0)
    {
        m_loaded_menu_prompts.pop_back();
    }
}

/**
 * @brief Reads a Specific Menu, Info and Options
 */
void MenuBase::readMenuAllPrompts()
{
    // Clear Scren
    //m_session_data->deliver(m_session_io.pipe2ansi("|CS|07"));
    std::string filename = "MENUPROM.DAT";

    // Make sure list is cleared so we can reload if needed
    if(m_loaded_menu_prompts.size() > 0)
        clearAllMenuPrompts();

    // Loop each Option after Reading the Menu.
    int u = 0;
    while(recordReadInfo(&m_menu_prompt, filename, u++))
    {
        // Convert Pascal to C Strings.
        m_common_io.PascalToCString(m_menu_prompt.Name);
        m_common_io.PascalToCString(m_menu_prompt.Data[0]);
        m_common_io.PascalToCString(m_menu_prompt.Data[1]);
        m_common_io.PascalToCString(m_menu_prompt.Data[2]);

        // Add to Vector
        m_loaded_menu_prompts.push_back(m_menu_prompt);

        // Just extra testing to display the results!!
        /*
        std::cout << "Name: " << m_menu_prompt.Name << std::endl;
        std::cout << "Data: " << m_menu_prompt.Data[0] << std::endl;
        std::cout << "Data: " << m_menu_prompt.Data[1] << std::endl;
        std::cout << "Data: " << m_menu_prompt.Data[2] << std::endl;

        if (strcmp((char *)m_menu_prompt.Name, "The Kind King Prompt") == 0)
        {
            m_session_data->deliver("\r\n");
            m_session_data->deliver("\r\n");
            m_session_data->deliver("\r\n");
        }

        if (strcmp((char *)m_menu_prompt.Name, "FusioN Prompt #1") == 0)
        {
            m_session_data->deliver("\r\n");
        }

        m_session_data->deliver(m_session_io.pipe2ansi("|X30 |08<|07- |15 "));
        m_session_data->deliver(m_session_io.pipe2ansi((char *)m_menu_prompt.Name));
        m_session_data->deliver(m_session_io.pipe2ansi("|X01"));

        if (strcmp((char *)m_menu_prompt.Name, "FusioN Prompt #1") == 0)
        {
            m_session_data->deliver(m_session_io.pipe2ansi("|U1"));
        }

        m_session_data->deliver(m_session_io.pipe2ansi((char *)m_menu_prompt.Data[0]));
        m_session_data->deliver("\r\n");
        m_session_data->deliver(m_session_io.pipe2ansi((char *)m_menu_prompt.Data[1]));
        m_session_data->deliver("\r\n");
        m_session_data->deliver(m_session_io.pipe2ansi((char *)m_menu_prompt.Data[2]));
        m_session_data->deliver("\r\n");

        if (strcmp((char *)m_menu_prompt.Name, "The Kind King Prompt") == 0)
        {
            m_session_data->deliver("\r\n");
        }

        std::cout << std::endl;
        */
    }
}
