/*
 * Oblivion/2 XRM - Legacy to XRM Menu Converter (c) 2015-2016 Michael Griffin
 * This converts all legacy .mnu files to new .yaml configuration files
 *
 * Compiles under MingW32/64 5.1.0 g++
 *
 * LIBS: Boost, yamlc++
 *
 */

#include "model/struct_compat.hpp"
#include "model/structures.hpp"

#include "data/config_dao.hpp"
#include "model/config.hpp"

#include "compat/menu_compat_dao.hpp"
#include "model/menu.hpp"
#include "data/menu_dao.hpp"

#include "common_io.hpp"

#include <boost/locale.hpp>

// Fix for file_copy
#define BOOST_NO_CXX11_SCOPED_ENUMS
#include <boost/filesystem.hpp>
#undef BOOST_NO_CXX11_SCOPED_ENUMS

#include <cstdlib>
#include <iostream>
#include <algorithm>
#include <exception>

#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/smart_ptr/weak_ptr.hpp>

std::string GLOBAL_BBS_PATH = "";
std::string GLOBAL_DATA_PATH = "";
std::string GLOBAL_MENU_PATH = "";
std::string GLOBAL_TEXTFILE_PATH = "";

/**
 * @class MenuConvert
 * @author Michael Griffin
 * @date 17/11/2016
 * @file main.cpp
 * @brief Handle OBV/2 Legacy to XRM Menu conversion
 */
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
    void path_seperator(std::string &value);

    /**
     * @brief Reads a Specific Menu, Info and Options
     */
    void clear_menu_options();

    /**
     * @brief Reads a Specific Menu, Info and Options
     */
    void load_menu();

    /**
     * @brief Convert Legacy to Backup and Create Yaml
     *
     */
    void convert_menu();

    /**
    * @brief Copy Legacy to Backup and Create Yaml
    *
    */
    bool backup_menu();

    /**
     * @brief Read all Legacy Menus, then save to backup folder
     * Then convert Legacy to YAML format.
     */
    void process_menu();

};

/**
 * @brief Helper, appends forward/backward slash to path
 * @param value
 */
void MenuConvert::path_seperator(std::string &value)
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
void MenuConvert::clear_menu_options()
{
    while(m_loaded_menu_options.size() > 0)
    {
        m_loaded_menu_options.pop_back();
    }
}

/**
 * @brief Reads a Specific Menu, Info and Options
 */
void MenuConvert::load_menu()
{
    MenuCompatOption menu_option;

    memset(&m_menu_info, 0, sizeof(MenuCompatInfo));
    memset(&menu_option, 0, sizeof(MenuCompatOption));
    clear_menu_options();

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
    while(recordReadOption(&menu_option, m_current_menu, u++))
    {
        m_common_io.PascalToCString(menu_option.Acs);
        m_common_io.PascalToCString(menu_option.OptName);
        m_common_io.PascalToCString(menu_option.Keys);
        m_common_io.PascalToCString(menu_option.CKeys);
        m_common_io.PascalToCString(menu_option.CString);

        // Load into vector.
        m_loaded_menu_options.push_back(menu_option);

        memset(&menu_option, 0, sizeof(MenuCompatOption));
    }
}

/**
 * @brief Convert Legacy to Backup and Create Yaml
 *
 */
void MenuConvert::convert_menu()
{
    namespace fs = boost::filesystem;

    // Instatnce for new yaml menu format.
    menu_ptr menu(new Menu());

    // Convert int8_t* to std::strings
    menu->menu_name          = boost::lexical_cast<std::string>(m_menu_info.Name);
    menu->menu_password      = boost::lexical_cast<std::string>(m_menu_info.Password);
    menu->menu_fall_back     = boost::lexical_cast<std::string>(m_menu_info.FallBack);
    menu->menu_help_file     = boost::lexical_cast<std::string>(m_menu_info.HelpID);
    menu->menu_groups        = boost::lexical_cast<std::string>(m_menu_info.ACS);
    menu->menu_prompt        = boost::lexical_cast<std::string>(m_menu_info.NameInPrompt);
    menu->menu_title         = boost::lexical_cast<std::string>(m_menu_info.MenuTitle);
    menu->menu_pulldown_file = boost::lexical_cast<std::string>(m_menu_info.PulldownFN);

    MenuOption option;

    int index = 0;
    for (auto &opt : m_loaded_menu_options)
    {
        option.index           = index++;
        option.name            = boost::lexical_cast<std::string>(opt.OptName);
        option.groups          = boost::lexical_cast<std::string>(opt.Acs);
        option.hidden          = boost::lexical_cast<bool>(opt.Hidden);
        option.menu_key        = boost::lexical_cast<std::string>(opt.Keys);
        option.command_key     = boost::lexical_cast<std::string>(opt.CKeys);
        option.command_string  = boost::lexical_cast<std::string>(opt.CString);
        option.pulldown_id     = boost::lexical_cast<int>(opt.PulldownID);

        menu->menu_options.push_back(option);
    }

    // Strip .MNU from Menu filename and lower case it.
    // Might want to change to boost locale to_lower
    std::string core_menu_name = m_current_menu.substr(0, m_current_menu.size() - 4);
    std::transform(
        core_menu_name.begin(),
        core_menu_name.end(),
        core_menu_name.begin(),
        ::tolower
    );

    // Save new YAML Menu
    MenuDao mnu(menu, core_menu_name, GLOBAL_MENU_PATH);

    try
    {
        // On success, remove legacy .MNU File
        if (mnu.saveMenu(menu))
        {
            std::string legacy_menu = GLOBAL_MENU_PATH;
            pathSeperator(legacy_menu);
            legacy_menu.append(m_current_menu);
            remove(legacy_menu.c_str());
        }
    }
    catch (std::exception &e)
    {
        std::cout << "Exception, unable to write .yaml menu file" << std::endl;
        std::cout << e.what() << std::endl;
    }
}


/**
 * @brief Backup Legacy to Backup and Create Yaml
 *
 */
bool MenuConvert::backup_menu()
{
    namespace fs = boost::filesystem;

    bool result = true;

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
    boost::system::error_code ec;
    try
    {
        fs::copy_file(menu_source_path, menu_dest_path, fs::copy_option::overwrite_if_exists);
    }
    catch (std::exception &e)
    {
        std::cout << "erorr, unable to backup: " << m_current_menu << std::endl;
        std::cout << "Excepton: " << e.what() << std::endl;
        result = false;
    }

    return result;
}

/**
 * @brief Read all Legacy Menus and create backup folder.
 *
 */
void MenuConvert::process_menu()
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

    // check result set, if no menu then return gracefully.
    if (result_set.size() == 0)
    {
        std::cout << "\r\n*** No Legacy .MNU files found to convert at this time." << std::endl;
        return;
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

        // Only convert menus that are backed up.
        if (backup_menu())
        {
            load_menu();
            convert_menu();
        }
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
    GLOBAL_BBS_PATH = common.getProgramPath("xrm-menu-convert");
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
            exit(1);
        }
    }
    catch (std::exception &e)
    {
        std::cout << "Unable to load configuration in bbs root." << std::endl;
        exit(2);
    }


    // Menu Convert Instance.
    MenuConvert convert;

    // start Conversion process
    try {
        convert.process_menu();
    }
    catch (std::exception &e)
    {
        std::cout << "Exception: Unable to process menus." << std::endl;
        std::cout << e.what() << std::endl;
        exit(3);
    }

    return 0;
}
