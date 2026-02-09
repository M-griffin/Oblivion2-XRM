#ifndef MENU_PROMPT_DAO_HPP
#define MENU_PROMPT_DAO_HPP

#include <string>
#include <mutex>

#include "../model-sys/menu_prompt.hpp"

class UtilLog;


/**
 * @class MenuPromptDao
 * @author Michael Griffin
 * @date 05/03/2017
 * @file menu_dao.hpp
 * @brief Handles Reading and Writing a Menu Prompt from YAML Files
 */
class MenuPromptDao {

    UtilLog &m_log;
    MenuPrompt &m_menu_prompt;
    std::string m_path;
    std::string m_filename;
    std::mutex m;

public:
    MenuPromptDao(MenuPrompt &menu_prompt,
                  std::string &menu_prompt_name,
                  std::string &path);

    ~MenuPromptDao() = default;

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
    bool saveMenuPrompt(MenuPrompt &m_menu_prompt);

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
    [[nodiscard]]
    MenuPrompt &getMenuPrompt() const {
        return m_menu_prompt;
    }

};

#endif
