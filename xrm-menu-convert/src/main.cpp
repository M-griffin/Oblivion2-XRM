/*
 * Oblivion/2 XRM - Legacy to XRM Menu Converter (c) 2015-2016 Michael Griffin
 * This converts all legacy .mnu files to new .yaml configuration files
 *
 * Compiles under MingW32/64 5.1.0 g++
 *
 * LIBS:
 *
 */

#include "model/struct_compat.hpp"
#include "model/structures.hpp"

#include "model/config.hpp"
#include "data/config_dao.hpp"

#include "compat/menu_compat_dao.hpp"
#include "model/menu.hpp"
#include "data/menu_dao.hpp"

#include "common_io.hpp"

#include <boost/locale.hpp>
#include <boost/filesystem.hpp>

#include <cstdlib>
#include <iostream>

#include <exception>

#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/smart_ptr/weak_ptr.hpp>

std::string GLOBAL_BBS_PATH = "";
std::string GLOBAL_DATA_PATH = "";
std::string GLOBAL_MENU_PATH = "";
std::string GLOBAL_TEXTFILE_PATH = "";


class MenuConvert
    : private MenuCompatDao
{ 
 
public:

    MenuConvert()
        : m_current_menu("")
    { }
    
    ~MenuConvert() { }
    
    std::string                   m_current_menu;
    CommonIO                      m_common_io;
    MenuCompatInfo                m_menu_info;
    std::vector<MenuCompatOption> m_loaded_menu_options;


    /**
     * @brief Helper, appends forward/backward slash to path
     * @param value
     */
    void pathSeperator(std::string &value);
  
    /**
     * @brief Reads a Specific Menu, Info and Options
     */
    void clearMenuOptions();

    /**
     * @brief Reads a Specific Menu, Info and Options
     */
    void readInMenuData();

    /**
     * @brief Copy Legacy to Backup and Create Yaml
     * 
     */
    void copy_menus();
     
     /**
     * @brief Copy Legacy to Backup and Create Yaml
     * 
     */
    void backup_menus();

    /**
     * @brief Read all Legacy Menus, then save to backup folder
     * Then convert Legacy to YAML format.
     */
    void process_menus();
  
};


/**
 * @brief Helper, appends forward/backward slash to path
 * @param value
 */
void MenuConvert::pathSeperator(std::string &value)
{
#ifdef _WIN32
    value.append("\\");
#else
    value.append("/");
#endif
}


/**
 * @brief Reads a Specific Menu, Info and Options
 */
void MenuConvert::clearMenuOptions()
{
    while(m_loaded_menu_options.size() > 0)
    {
        m_loaded_menu_options.pop_back();
    }
}


/**
 * @brief Reads a Specific Menu, Info and Options
 */
void MenuConvert::readInMenuData()
{
    MenuCompatInfo m_menu_info;
    MenuCompatOption m_menu_option;

    memset(&m_menu_info, 0, sizeof(MenuCompatInfo));
    memset(&m_menu_option, 0, sizeof(MenuCompatOption));
    clearMenuOptions();
    //clearMenuPullDownOptions();

    recordReadInfo(&m_menu_info, m_current_menu, 0);

    // Convert Pascal to C Strings.
    m_common_io.PascalToCString(m_menu_info.Name);
    m_common_io.PascalToCString(m_menu_info.Password);
    m_common_io.PascalToCString(m_menu_info.FallBack);
    m_common_io.PascalToCString(m_menu_info.HelpID);
    m_common_io.PascalToCString(m_menu_info.ACS);
    m_common_io.PascalToCString(m_menu_info.NameInPrompt);
    m_common_io.PascalToCString(m_menu_info.MenuTitle);
    m_common_io.PascalToCString(m_menu_info.PulldownFN);

    // Loop each Option after Reading the Menu.
    int u = 0;
    while(recordReadOption(&m_menu_option, m_current_menu, u++))
    {
        m_common_io.PascalToCString(m_menu_option.Acs);
        m_common_io.PascalToCString(m_menu_option.OptName);
        m_common_io.PascalToCString(m_menu_option.Keys);
        m_common_io.PascalToCString(m_menu_option.CKeys);
        m_common_io.PascalToCString(m_menu_option.CString);

        // Load into vector.
        m_loaded_menu_options.push_back(m_menu_option);
    }
}


/**
 * @brief Backup Legacy to Backup and Create Yaml
 * 
 */
void MenuConvert::backup_menus() 
{   
    namespace fs = boost::filesystem;
    
    std::string source = GLOBAL_MENU_PATH;
    std::string dest   = GLOBAL_MENU_PATH;
    
    pathSeperator(dest);
    dest.append("backup");    

    std::string source_menu = source;
    pathSeperator(source_menu);
    
    std::string dest_menu = dest;
    pathSeperator(dest_menu);
    
    source_menu.append(m_current_menu);   
    dest_menu.append(m_current_menu);    
      
    fs::path menu_source_path(source_menu);
    fs::path menu_dest_path(dest_menu);
  
    std::cout << "processing menu: " << m_current_menu << std::endl;
    
    // Loop through and process, move to backup folder, then generate yaml.
    fs::copy_file(menu_source_path, menu_dest_path, fs::copy_option::overwrite_if_exists);
    
}


/**
 * @brief Read all Legacy Menus and create backup folder. 
 * 
 */
void MenuConvert::process_menus() 
{
    namespace fs = boost::filesystem;
    fs::path menu_directory(GLOBAL_MENU_PATH);   // Add to menu path from config!
    fs::directory_iterator end_iter;

    typedef std::vector<std::string> result_set_t;
    typedef std::vector<std::string>::iterator iterator;
    result_set_t result_set;

    if(fs::exists(menu_directory) && fs::is_directory(menu_directory))
    {
        for(fs::directory_iterator dir_iter(menu_directory); dir_iter != end_iter; ++dir_iter)
        {
            if(dir_iter->path().extension() == ".MNU")
            {
                if(fs::is_regular_file(dir_iter->status()))
                {
                    result_set.push_back(dir_iter->path().filename().string());
                    //result_set_t::value_type(fs::last_write_time( dir_iter->path() ) ) ); // *dir_iter));
                }
            }
        }
    }
    // Sort Menu's in accending order
    std::sort(result_set.begin(), result_set.end());
    
    
    // Setup and Create Backup Directory
    std::string backup_directory = GLOBAL_MENU_PATH;
    pathSeperator(backup_directory);
    backup_directory.append("backup");
    
    fs::path menu_backup_directory(backup_directory);
    
    // Check if backup exists, if not create it.
    if(!fs::exists(menu_backup_directory) || !fs::is_directory(menu_backup_directory))
    {
        std::cout << "Backup folder: " << menu_backup_directory << std::endl;
        if (boost::filesystem::create_directory(menu_backup_directory))
        {
            std::cout << "Backup folder created." << std::endl;    
        }
        else 
        {
            std::cout << "Unable to create Backup directory. Check Permissions." << std::endl;
            return;
        }
    }
        
    // Loop each menu
    for (std::string s : result_set)
    {
        m_current_menu = s;
        backup_menus();
    }    
}

/**
 * @brief Main Program Entrance.
 * @return
 */
auto main() -> int
{
    std::cout << "Oblivion/2 XRM Server - Legacy to XRM Menu Converter" << std::endl;
    std::cout << "(c) 2015-2016 Michael Griffin." << std::endl << std::endl;
    std::cout << "Important, you must run this from the root directory," << std::endl;
    std::cout << "Otherwise you can set the OBV2 environment variable." << std::endl 
              << std::endl;
    
    CommonIO common;
    GLOBAL_BBS_PATH = common.getProgramPath();
    std::cout << "BBS HOME Directory Registered: " 
              << std::endl << GLOBAL_BBS_PATH << std::endl;

    // Setup System Folder Paths off main BBS Path.
    GLOBAL_DATA_PATH = GLOBAL_BBS_PATH + "DATA";
    GLOBAL_MENU_PATH = GLOBAL_BBS_PATH + "MENU";
    GLOBAL_TEXTFILE_PATH = GLOBAL_BBS_PATH + "TEXTFILE";

    // Load System Configuration
    try {
        config_ptr config(new Config());
        if (!config)
        {
            std::cout << "Unable to allocate config structure" << std::endl;
            assert(false);
        }

        // Handle to Data Access Object,  at the moment were not using directories
        // Setup in the config, everything is branched from the main path.
        // Later on we'll check config for overides only.
        ConfigDao cfg(config, GLOBAL_BBS_PATH);

        if (!cfg.fileExists())
        {
            std::cout << "Unable to locate xrm-config.yaml, you must run this from root bbs directory." << std::endl;
            assert(false);
        }
    }
    catch (std::exception &e)
    {
        std::cout << "Unable to load configuration in bbs root." << std::endl;
    }
    
    
    // Menu Convert Instance.
    MenuConvert convert;
    
    // start Conversion process
    try {
        convert.process_menus();        
    }
    catch (std::exception &e)
    {
        std::cout << "Exception: Unable to process menus." << std::endl;
        std::cout << e.what() << std::endl;
    }
    
    
/*
    // Start Menu Conversion Process
    {
        // Testing Menu
        menu_ptr menu(new Menu());

        menu->menu_name = "matrix";
        menu->menu_password = "password";
        menu->menu_fall_back = "fallback";
        menu->menu_help_file = "helpfile";
        menu->menu_groups = "groups";
        menu->menu_prompt = "prompt";
        menu->menu_title = "title";
        menu->menu_pulldown_file = "pulldown";

        MenuOption option;

        option.option_index = 1;
        option.option_name = "name 1";
        option.option_groups = "group1";
        option.option_hidden = true;
        option.option_input_key = "ikey1";
        option.option_cmd_key = "ckey1";
        option.option_cmd_string = "cstr1";
        option.option_pulldown_id = 1;

        menu->menu_options.push_back(option);

        option.option_index = 2;
        option.option_name = "name 2";
        option.option_groups = "group2";
        option.option_hidden = false;
        option.option_input_key = "ikey2";
        option.option_cmd_key = "ckey2";
        option.option_cmd_string = "cstr2";
        option.option_pulldown_id = 2;

        menu->menu_options.push_back(option);
             
        MenuDao mnu(menu, "matrix", GLOBAL_MENU_PATH);


        //mnu.saveMenu(menu);

        mnu.loadMenu();

        std::cout << menu->menu_name << std::endl;
        std::cout << menu->menu_password << std::endl;
        std::cout << menu->menu_fall_back << std::endl;
        std::cout << menu->menu_help_file << std::endl;
        std::cout << menu->menu_groups << std::endl;
        std::cout << menu->menu_prompt << std::endl;
        std::cout << menu->menu_title << std::endl;
        std::cout << menu->menu_pulldown_file << std::endl << std::endl;

        std::cout << "options size: " << menu->menu_options.size() << std::endl;
        for (auto &o : menu->menu_options)
        {
            std::cout << o.option_index << std::endl;
            std::cout << o.option_name << std::endl;
            std::cout << o.option_groups << std::endl;
            std::cout << o.option_hidden << std::endl;
            std::cout << o.option_input_key << std::endl;
            std::cout << o.option_cmd_key << std::endl;
            std::cout << o.option_cmd_string << std::endl;
            std::cout << o.option_pulldown_id << std::endl << std::endl;
        }

    }
*/
    return 0;
}
