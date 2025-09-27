#ifndef MENU_PROMPT_DAO_HPP
#define MENU_PROMPT_DAO_HPP

#include <memory>
#include <string>
#include <mutex>

class Logging;

class MenuPrompt;
typedef std::shared_ptr<MenuPrompt> menu_prompt_ptr;


/**
 * @class MenuPromptDao
 * @author Michael Griffin
 * @date 05/03/2017
 * @file menu_dao.hpp
 * @brief Handles Reading and Writing a Menu Prompt from YAML Files
 */
class MenuPromptDao
{
public:

    MenuPromptDao(menu_prompt_ptr menu_prompt, 
                  const std::string &menu_prompt_name, 
                  const std::string &path);
    ~MenuPromptDao();

    /**
     * @brief Helper, appends forward/backward slash to path
     * @param value
     */
    void pathSeperator(std::string &value);

    /**
     * @brief Check if the file exists and we need to create a new one.
     * @return
     */
    bool fileExists();

    /**
     * @brief Creates and Saves a newly Generated Menu File.
     * @param m_menu_prompt
     * @return
     */
    bool saveMenuPrompt(menu_prompt_ptr m_menu_prompt);

    /**
     * @brief Loads a Menu file into the m_menu stub for access.
     * @return
     */
    bool loadMenuPrompt();

    /**
     * @brief Moves the Loaded Menu to the shared pointer.
     * @param rhs
     * @return
     */
    void encode(const MenuPrompt &rhs);

    /**
     * @brief Grab a const handle to the loaded menu_prompt.
     * @return
     */
    menu_prompt_ptr getMenuPrompt() const
    {
        if(m_menu_prompt)
        {
            return m_menu_prompt;
        }

		return nullptr;
    }

    Logging         &m_log;
    menu_prompt_ptr  m_menu_prompt;
    std::string      m_path;
    std::string      m_filename;
    std::mutex       m;

};

typedef std::shared_ptr<MenuPromptDao> menu_prompt_dao_ptr;

#endif // MENU_PROMPT_DAO_HPP
