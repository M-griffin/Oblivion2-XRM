#include "state_manager.hpp"

#include <boost/locale.hpp>
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
    
    
/*
    using namespace boost::locale;
    generator gen;

    std::locale systemLocale = gen("");
    std::locale::global(systemLocale);
    std::cout.imbue(systemLocale);

    std::string character;
    bool utf_found  = false;
    int  char_value =  0;

    boundary::ssegment_index::iterator current, end;

    // Make sure state is created before accepting input.
    if(!m_the_state.empty())
    {
        std::string incoming_data = std::move(m_the_state.back()->m_session_data->m_parsed_data);

        std::cout << "incoming: " << incoming_data << std::endl;

        if(incoming_data.size() > 0)
        {
            // Were going to loop the Parsed data, and pass each
            // Charactesr in seperately like 1 keys at a time
            // Handles Multi-byte UTF-8 Sequences
            for (auto ch : incoming_data)
            {
                char_value = ch;
                if (!utf_found)
                {
                    if ((char_value & 0xC0) != 0xC0)
                        utf_found = false; // 1 Byte
                    else if ((char_value & 0xE0) == 0xC0)
                        utf_found = true;  // 2;
                    else if ((char_value & 0xF0) == 0xE0)
                        utf_found = true;  // 3;
                    else if ((char_value & 0xF8) == 0xF0)
                        utf_found = true;  // 4;
                    else
                    {
                        // Not a valid character, or in the
                        std::cout << "Exception: Invalid character input (NOT ASCII or UTF-8)!" << std::endl;
                        continue;
                    }
                }

                //charValue = 0x8A;  // Test single-byte extended ASCII
                character += char_value;
                boundary::ssegment_index index; //index(boundary::word,character.begin(),character.end());
                index.map(boundary::character,character.begin(),character.end());

                // Catch All MultiByte UTF-8 Character Sequences.
                // By default, UTF-8 can be any char > 127 ASCII
                // Depending if input encoding is CP437 etc.. we might need to intercept
                // 128-255 here on input for the 8th bit translations.
                if (utf_found)
                {
                    for(current = index.begin(), end = index.end();
                            current != end; ++current)
                    {
                        // Only Gets here in complete UTF-8 Sequence.
                        //std::cout << "utf8: " << character << std::endl;

                        m_the_state.back()->update(character, utf_found);
                        character.erase();
                    }
                }
                else
                {
                    // Single Byte 0 - 255
                    //std::cout << "single: " << character << std::endl;
                    m_the_state.back()->update(character, utf_found);
                    character.erase();
                }
            }
        }

        // After All data parsed, if there is a lone ESC, then we want to pass it!
        // if last character sent was ESC, then force a null \0 after to notifiy no more data!
        if (char_value == 27)
        {
            character.erase();
            character += '\0';
            m_the_state.back()->update(character, utf_found);
        }
    }
    */
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
