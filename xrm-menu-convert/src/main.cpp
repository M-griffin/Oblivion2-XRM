/*
 * Oblivion/2 XRM - Legacy to XRM Menu Converter (c) 2015-2016 Michael Griffin
 * This converts all legacy .mnu files to new .yaml configuration files
 *
 * Compiles under MingW32/64 5.1.0 g++
 *
 * LIBS:
 *
 */

#include "model/config.hpp"
#include "data/config_dao.hpp"

#include "model/menu.hpp"
#include "data/menu_dao.hpp"

#include "common_io.hpp"


#include <cstdlib>
#include <iostream>

#include <exception>

#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/smart_ptr/weak_ptr.hpp>

// Temp!
std::string GLOBAL_BBS_PATH = "";
std::string GLOBAL_DATA_PATH = "";
std::string GLOBAL_MENU_PATH = "";
std::string GLOBAL_TEXTFILE_PATH = "";


/**
 * @brief Main Program Entrance.
 * @return
 */
auto main() -> int
{
    std::cout << "Oblivion/2 XRM Server - Legacy to XRM Menu Converter (c) 2015-2016 Michael Griffin." << std::endl;
    std::cout << "Import, you must run this from the root bbs directory." << std::endl;

    CommonIO common;
    GLOBAL_BBS_PATH = common.getProgramPath();
    std::cout << "BBS HOME Directory Registered: " << GLOBAL_BBS_PATH << std::endl;

    // Setup System Folder Paths off main BBS Path.
    GLOBAL_DATA_PATH = GLOBAL_BBS_PATH + "DATA";
    GLOBAL_MENU_PATH = GLOBAL_BBS_PATH + "MENU";
    GLOBAL_TEXTFILE_PATH = GLOBAL_BBS_PATH + "TEXTFILE";

    // Load System Configuration
    {
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

    // Start Menu Conversion Process
    {
        // Testing Menu
        menu_ptr menu(new Menu());

        /*
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
             */
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





    return 0;
}
