#ifndef MENU_DAO_HPP
#define MENU_DAO_HPP

#include <memory>
#include <string>
#include <mutex>


class Menu;
typedef std::shared_ptr<Menu> menu_ptr;

 
 /**
  * @class MenuDao
  * @author Michael Griffin
  * @date 15/11/2016
  * @file menu_dao.hpp
  * @brief Handles Reading and Writting a Menu from YAML Files
  */
class MenuDao
{
public:

    MenuDao(menu_ptr menu, std::string menu_name, std::string path);
    ~MenuDao();

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
    bool saveMenu(menu_ptr menu);
    
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
    menu_ptr getMenu() const
    {
        if(m_menu)
        {
            return m_menu;
        }
		
		return nullptr;
    }
   
    menu_ptr    m_menu;
    std::string m_path;
    std::string m_filename;
    std::mutex  m;

};

typedef std::shared_ptr<MenuDao> menu_dao_ptr;

#endif // MENU_DAO_HPP
