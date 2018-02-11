#include "state_manager.hpp"

#include <utf-cpp/utf8.h>
#include <string>

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
 * @brief Parses Incoming Strings, Sepeates into single character
 * strings of single ASCII charactrs or UTF-8 multi-byte sequence
 * 
 * Each Code point is a individual UTF8 Character
 * which is copied from code_point to char[5] array.
 */
void StateManager::update()
{
    int char_count = 0;
    std::string new_string_builder = "";
    bool utf_found = false;
    
    if(!m_the_state.empty())
    {
        std::string incoming_data = std::move(m_the_state.back()->m_session_data->m_parsed_data);

        if(incoming_data.size() > 0)
        {
            std::string::iterator it = incoming_data.begin();
            std::string::iterator line_end = incoming_data.end();    
            
            int length = utf8::distance(it, line_end);
            while (it != line_end) {                
                utf_found = false;
                uint32_t code_point = utf8::next(it, line_end);
                
                std::cout << "ut: " << *it << std::endl;
                std::cout << "code_point: " << code_point << std::endl;
                
                //std::cout << "append" << std::endl;
                // This convert the uint32_t code point to char array
                // So each sequence can be writen as seperate byte.
                unsigned char character[5] = {0};
                utf8::append(code_point, character);
                new_string_builder += (char *)character;
                
                // NOTE Not really used at this time,  might just remove!
                if (strlen((const char *)character) > 1 || code_point > 512)
                    utf_found = true;
                
                //std::cout << "char_count: " << char_count << " " << code_point << std::endl;        
                ++char_count;
                
                // End of Sequences or single ESC's.
                if (length == char_count && character[0] == 27)
                {
                    new_string_builder.erase();
                    new_string_builder += '\0';
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
 * @brief Main Entertrance for Adding, Switching to new States.
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
        std::cout << "Deleteing Current MenuSystem!: " << m_the_state.size() << std::endl;
        m_the_state.pop_back();

        // Clear the Memory!
        std::cout << "Clearing Memory of MenuSystem!: " << m_the_state.size() << std::endl;
        std::vector<state_ptr>().swap(m_the_state);
    }

    // initialize it
    the_state->onEnter();

    // push back our new state
    m_the_state.push_back(the_state);
}
