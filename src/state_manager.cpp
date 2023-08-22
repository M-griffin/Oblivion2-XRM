#include "state_manager.hpp"

#include "state_base.hpp"
#include "session.hpp"
#include "logging.hpp"
#include "utf-cpp/utf8.h"

#include <cstring>
#include <string>

StateManager::StateManager()
    : m_log(Logging::getInstance())
{ }
    
StateManager::~StateManager()
{
    m_log.write<Logging::DEBUG_LOG>("~StateManager()");
    
    if(!m_the_state.empty())
    {
        m_the_state.back()->onExit();

        while(m_the_state.size() > 0)
        {
            m_the_state.pop_back();
        }

        m_the_state.clear();
    }
}
    
/**
 * @brief Removed the Current State from the session
 */
void StateManager::clean()
{
    if(!m_the_state.empty())
    {
        m_the_state.back()->onExit();

        while(m_the_state.size() > 0)
        {
            m_the_state.pop_back();
        }

        m_the_state.clear();
    }
}

/**
 * @brief Parses Incoming Strings, Separates into single character
 * strings of single ASCII characters or UTF-8 multi-byte sequence
 *
 * Each Code point is a individual UTF8 Character
 * which is copied from code_point to char[5] array.
 */
void StateManager::update()
{   
    std::string new_string_builder = "";
    bool utf_found = false;

    if(!m_the_state.empty())
    {
        std::string incoming_data = "";
        try
        {
            incoming_data = std::move(m_the_state.back()->m_session_data->m_parsed_data);            
        }
        catch(std::exception &ex)
        {
            m_log.write<Logging::ERROR_LOG>("StateManager() Exception=", ex.what(), __LINE__, __FILE__);
        }

        if(incoming_data.size() > 0)
        {
            std::string::iterator it = incoming_data.begin();
            std::string::iterator line_end = incoming_data.end();

            /**
             * This loops a string, each next should be a single character code point.
             */
            while(it != line_end)
            {
                int byte_value = static_cast<int>((uint8_t)*it);

                if(byte_value < 128)
                {
                    utf_found = false;
                    *it++;

                    // Only if ESC and next char is empty
                    // Then we want to pass both to register single ESC hit
                    if(byte_value == '\x1b' && *it == '\0')
                    {
                        new_string_builder += '\x1b';
                        m_the_state.back()->update(new_string_builder, utf_found);

                        new_string_builder = '\0';
                        m_the_state.back()->update(new_string_builder, utf_found);
                        new_string_builder.erase();
                        continue;
                    }                    

                    new_string_builder += std::string(1, byte_value);
                }
                else
                {
                    try
                    {
                        // Only gets here on multi-byte sequences.
                        uint32_t code_point = utf8::next(it, line_end);
                        unsigned char character[5] = {0, 0, 0, 0, 0};
                        utf8::append(code_point, character);

                        for(int i = 0; i < 5; i++)
                        {
                            if(character[i] != 0)
                            {
                                new_string_builder += std::string(1, character[i]);
                            }
                        }

                        utf_found = true;
                    }
                    catch(utf8::exception &ex)
                    {
                        // This is filling up the log on Invalid Characters, have to find out what is pushing in here.
                        m_log.write<Logging::ERROR_LOG>("Utf8 Parsing Exception=", *it, ex.what(), __LINE__, __FILE__);
                    }
                }

                m_the_state.back()->update(new_string_builder, utf_found);
                new_string_builder.erase();
            }
        }
    }
}

/**
 * @brief Appends a new state then run the onEnter()
 */
void StateManager::pushState(state_ptr &the_state)
{
    m_the_state.push_back(the_state);
    m_the_state.back()->onEnter();
}

/**
 * @brief Runs onExit(), Then Removes the Last Added State.
 */
void StateManager::popState()
{
    if(!m_the_state.empty())
    {
        m_the_state.back()->onExit();
        m_the_state.pop_back();
    }

    m_the_state.back()->resume();
}

/**
 * @brief Main Entrance for Adding, Switching to new States.
 */
void StateManager::changeState(state_ptr &the_state)
{
    if(!m_the_state.empty())
    {
        if(m_the_state.back()->getStateID() == the_state->getStateID())
        {
            return; // do nothing
        }

        m_the_state.back()->onExit();

        // Rework this lateron,  lets allow multiple states,, the most recent state will be active
        // Allowing the main state to keep all information!
        m_the_state.pop_back();

        // Clear the Memory!
        std::vector<state_ptr>().swap(m_the_state);
    }

    // initialize it
    the_state->onEnter();

    // push back our new state
    m_the_state.push_back(the_state);
}
