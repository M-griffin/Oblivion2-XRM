#ifndef MENU_DAO_HPP
#define MENU_DAO_HPP

#include <string>
#include <mutex>

#include "../model-sys/menu.hpp"

class Logging;

/**
 * @class MenuDao
 * @author Michael Griffin
 * @date 15/11/2016
 * @file menu_dao.hpp
 * @brief Handles Reading and Writing a Menu from YAML Files
 */
class MenuDao {
public:
    explicit MenuDao(Menu &menu, const std::string &menu_name, const std::string &path);
    ~MenuDao() = default;

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
     * @param menu
     * @return
     */
    bool saveMenu(Menu &menu);

    /**
     * @brief Removes and Existing Menu from the directory.
     * @param menu
     * @return
     */
    bool deleteMenu();

    /**
     * @brief Loads a Menu file into the m_menu stub for access.
     * @return
     */
    bool loadMenu();

    /**
     * @brief Moves the Loaded Menu to the shared pointer.
     * @param rhs
     * @return
     */
    void encode(const Menu &rhs);

    /**
     * @brief Grab a const handle to the loaded menu.
     * @return
     */
    Menu &getMenu() {
        return m_menu;
    }

    /**
     * @brief Override the Filename
     * @param menu_name
     */
    void setFileName(const std::string &menu_name) {
        m_filename = menu_name;
    }

    /**
     * @brief Retrieve the existing Filename
     * @return
     */
    std::string getFileName() const { return m_filename; }

    Logging &m_log;
    Menu &m_menu;
    std::string m_path;
    std::string m_filename;
    std::mutex m;
};

#endif
