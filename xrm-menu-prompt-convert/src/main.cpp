/*
 * Oblivion/2 XRM - Legacy to XRM Menu Prompt Converter (c) 2015-2017 Michael Griffin
 * This converts legacy MENUPROMPT.DAT files to new .yaml configuration file
 *
 * Compiles under MingW32/64 5.1.0 g++
 *
 * LIBS: Boost, yamlc++
 *
 */

#include "model-sys/struct_compat.hpp"
#include "model-sys/structures.hpp"

#include "data-sys/config_dao.hpp"
#include "model-sys/config.hpp"

#include "compat/prompts_compat_dao.hpp"
#include "model-sys/menu_prompt.hpp"
#include "data-sys/menu_prompt_dao.hpp"

#include "common_io.hpp"

#include <boost/lexical_cast.hpp>
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
std::string GLOBAL_MENU_PROMPT_PATH = "";
std::string GLOBAL_TEXTFILE_PATH = "";

/**
 * @class MenuConvert
 * @author Michael Griffin
 * @date 17/11/2016
 * @file main.cpp
 * @brief Handle OBV/2 Legacy to XRM Menu conversion
 */
class MenuPromptConvert : private PromptDao
{

public:
    MenuPromptConvert()
        : m_current_menu_prompt("")
    {
    }

    ~MenuPromptConvert()
    {
    }

    std::string m_current_menu_prompt;
    CommonIO m_common_io;

    MenuPromptCompat m_menu_prompt;
    std::vector<MenuPromptCompat> m_loaded_menu_prompts;

    /**
     * @brief Helper, appends forward/backward slash to path
     * @param value
     */
    void path_seperator(std::string& value);

    /**
     * @brief Convert Legacy to Backup and Create Yaml
     *
     */
    void convert_menu_prompts();

    /**
     * @brief Clears out the Loaded Menu Prompts.
     */
    void clearAllMenuPrompts();

    /**
     * @brief Reads a Specific Menu, Info and Options
     */
    void readMenuAllPrompts();

    /**
     * @brief Read all Legacy Menus, then save to backup folder
     * Then convert Legacy to YAML format.
     */
    void process_menu_prompts();
};

/**
 * @brief Helper, appends forward/backward slash to path
 * @param value
 */
void MenuPromptConvert::path_seperator(std::string& value)
{
#ifdef _WIN32
    value.append("\\");
#else
    value.append("/");
#endif
}

/**
 * @brief Convert Legacy to Backup and Create Yaml
 *
 */
void MenuPromptConvert::convert_menu_prompts()
{
    namespace fs = boost::filesystem;

    // Instance for new yaml menu format.
    menu_prompt_ptr menu_prompt(new MenuPrompt());
    menu_prompt.reset(new MenuPrompt());

    for (unsigned int i = 0; i < m_loaded_menu_prompts.size(); i++)
    {
        auto &m = m_loaded_menu_prompts[i];

        // Convert int8_t* to std::strings
        menu_prompt->name = boost::lexical_cast<std::string>(m.Name);
        menu_prompt->data_line1 = boost::lexical_cast<std::string>(m.Data[0]);
        menu_prompt->data_line2 = boost::lexical_cast<std::string>(m.Data[1]);
        menu_prompt->data_line3 = boost::lexical_cast<std::string>(m.Data[2]);

        // Save new YAML Menu
        MenuPromptDao mnu_prompt(menu_prompt, menu_prompt->name, GLOBAL_MENU_PROMPT_PATH);

        try
        {
            if(mnu_prompt.saveMenuPrompt(menu_prompt))
            {
                std::cout << "Menu Prompt Created: " << menu_prompt->name << std::endl;
            }
        }
        catch(std::exception& e)
        {
            std::cout << "Exception, unable to write .yaml menu file: " << menu_prompt->name << std::endl;
            std::cout << e.what() << std::endl;
        }
    }
}

/**
 * @brief Clears out the Loaded Menu Prompts.
 */
void MenuPromptConvert::clearAllMenuPrompts()
{
    while(m_loaded_menu_prompts.size() > 0)
    {
        m_loaded_menu_prompts.pop_back();
    }
}

/**
 * @brief Reads a Specific Menu, Info and Options
 */
void MenuPromptConvert::readMenuAllPrompts()
{
    // Clear Screen
    //m_session_data->deliver(m_session_io.pipe2ansi("|CS|07"));
    std::string filename = "MENUPROM.DAT";

    // Make sure list is cleared so we can reload if needed
    if(m_loaded_menu_prompts.size() > 0)
    {
        clearAllMenuPrompts();
    }

    // Loop each Option after Reading the Menu.
    int u = 0;
    while(recordRead(&m_menu_prompt, filename, u++))
    {
        // Convert Pascal to C Strings.
        m_common_io.PascalToCString(m_menu_prompt.Name);
        m_common_io.PascalToCString(m_menu_prompt.Data[0]);
        m_common_io.PascalToCString(m_menu_prompt.Data[1]);
        m_common_io.PascalToCString(m_menu_prompt.Data[2]);

        // Add to Vector
        m_loaded_menu_prompts.push_back(m_menu_prompt);
    }
}

/**
 * @brief Main Program Entrance.
 * @return
 */
auto main() -> int
{
    std::cout << "Oblivion/2 XRM Server - Legacy to XRM Menu Prompt Converter" << std::endl;
    std::cout << "(c) 2015-2017 Michael Griffin." << std::endl << std::endl;
    std::cout << "Important, you must run this from the root directory," << std::endl;
    std::cout << "Otherwise you can set the OBV2 environment variable." << std::endl << std::endl;

    CommonIO common;
    GLOBAL_BBS_PATH = common.getProgramPath("xrm-menu-prompt-convert");
    std::cout << "BBS HOME Directory Registered: " << std::endl << GLOBAL_BBS_PATH << std::endl;

    // Setup System Folder Paths off main BBS Path.
    GLOBAL_DATA_PATH = GLOBAL_BBS_PATH + "DATA";
    GLOBAL_MENU_PROMPT_PATH = GLOBAL_BBS_PATH + "MPROMPT";
    GLOBAL_TEXTFILE_PATH = GLOBAL_BBS_PATH + "TEXTFILE";

    // Load System Configuration
    try {
        config_ptr config(new Config());
        if(!config)
        {
            std::cout << "Unable to allocate config structure" << std::endl;
            assert(false);
        }

        // Handle to Data Access Object,  at the moment were not using directories
        // Setup in the config, everything is branched from the main path.
        // Later on we'll check config for overides only.
        ConfigDao cfg(config, GLOBAL_BBS_PATH);

        if(!cfg.fileExists())
        {
            std::cout << "Unable to locate xrm-config.yaml, you must run this from root bbs directory." << std::endl;
            exit(1);
        }
    }
    catch(std::exception& e)
    {
        std::cout << "Unable to load configuration in bbs root." << std::endl;
        exit(2);
    }

    // Menu Convert Instance.
    MenuPromptConvert convert;

    // start Conversion process
    try
    {
        convert.readMenuAllPrompts();
        convert.convert_menu_prompts();
        convert.clearAllMenuPrompts();
    }
    catch(std::exception& e)
    {
        std::cout << "Exception: Unable to process menus." << std::endl;
        std::cout << e.what() << std::endl;
        exit(3);
    }

    return 0;
}
