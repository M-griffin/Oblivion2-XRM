#ifndef MENU_DAO_HPP
#define MENU_DAO_HPP

#include <boost/smart_ptr/shared_ptr.hpp>

#include <string>
#include <mutex>


struct Menu;
typedef boost::shared_ptr<Menu> menu_ptr;

 
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
    }
   
    menu_ptr    m_menu;
    std::string m_path;
    std::string m_filename;
    std::mutex  m;

};

typedef boost::shared_ptr<MenuDao> menu_dao_ptr;

#endif // MENU_DAO_HPP
