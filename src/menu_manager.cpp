#include "menu_manager.hpp"

#include <boost/locale.hpp>
#include <string>

/**
 * @brief Removed the Current State from the session
 */
void MenuManager::clean()
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
 */
void MenuManager::update()
{
    using namespace boost::locale;
    generator gen;

    std::locale systemLocale = gen("");
    std::locale::global(systemLocale);
    std::cout.imbue(systemLocale);

    std::string character;
    bool utf_found  = false;
    int  char_value =  0;

    boundary::ssegment_index::iterator current, end;

    std::string incoming_data = std::move(m_the_state.back()->m_session_data->m_parsed_data);
    if(!m_the_state.empty() && incoming_data.size() > 0)
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
                    //std::cout << "Exception: Invalid character input (NOT ASCII or UTF-8)!" << std::endl;
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
                    m_the_state.back()->update(character, utf_found);
                    character.erase();
                }
            }
            else
            {
                // Single Byte 0 - 255
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

/**
 * @brief Appends a new state then run the onEnter()
 */
void MenuManager::pushState(state_ptr &the_state)
{
    m_the_state.push_back(the_state);
    m_the_state.back()->onEnter();
}

/**
 * @brief Runs onExit(), Then Removes the Last Added State.
 */
void MenuManager::popState()
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
void MenuManager::changeState(state_ptr &the_state)
{
    if(!m_the_state.empty())
    {
        if(m_the_state.back()->getStateID() == the_state->getStateID())
        {
            return; // do nothing
        }
        m_the_state.back()->onExit();

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
