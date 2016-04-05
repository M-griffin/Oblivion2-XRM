#include "config_dao.hpp"
#include "../model/config.hpp"

#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/smart_ptr/make_shared.hpp>

#include <iostream>
#include <fstream>
#include <string>

#include <mutex>

ConfigDao::ConfigDao(config_ptr config, std::string path)
    : m_config(config)
    , m_path(path)
    , m_filename("xrm_config.yaml")
{
    std::cout << "ConfigDAO" << std::endl;
}

ConfigDao::~ConfigDao()
{
    std::cout << "~ConfigDAO" << std::endl;
}


/**
 * @brief Check if the file exists and we need to create a new one.
 * @return
 */
bool ConfigDao::fileExists()
{
    std::string path = m_path;
    path.append(m_filename);

    std::ifstream ifs(path);
    if (!ifs.is_open())
    {
        return false;
    }
    ifs.close();
    return true;
}


/**
 * @brief Creates and Saves a newly Generated Configuration File.
 * @param cfg
 * @return
 */
bool ConfigDao::saveConfig(config_ptr cfg)
{
    std::string path = m_path;
    path.append(m_filename);

    YAML::Emitter out;

    out << YAML::BeginMap;
    out << YAML::Flow;

    // Start Creating the Key/Value Output for the Config File.
    out << YAML::Key << "bbs_name_sysop" << YAML::Value << cfg->bbs_name_sysop;
    out << YAML::Key << "bbs_name" << YAML::Value << cfg->bbs_name;
    out << YAML::Key << "bbs_uuid" << YAML::Value << cfg->bbs_uuid;
    out << YAML::Key << "password_system" << YAML::Value << cfg->password_system;
    out << YAML::Key << "password_sysop" << YAML::Value << cfg->password_sysop;
    out << YAML::Key << "password_newuser" << YAML::Value << cfg->password_newuser;
    out << YAML::Key << "port_telnet" << YAML::Value << cfg->port_telnet;
    out << YAML::Key << "port_ssl" << YAML::Value << cfg->port_ssl;
    out << YAML::Key << "use_service_telnet" << YAML::Value << cfg->use_service_telnet;
    out << YAML::Key << "use_service_ssl" << YAML::Value << cfg->use_service_ssl;
    out << YAML::Key << "directory_screens" << YAML::Value << cfg->directory_screens;
    out << YAML::Key << "directory_boards" << YAML::Value << cfg->directory_boards;
    out << YAML::Key << "directory_files" << YAML::Value << cfg->directory_files;
    out << YAML::Key << "directory_data" << YAML::Value << cfg->directory_data;
    out << YAML::Key << "directory_menu" << YAML::Value << cfg->directory_menu;
    out << YAML::Key << "directory_work" << YAML::Value << cfg->directory_work;
    out << YAML::Key << "directory_text_files" << YAML::Value << cfg->directory_text_files;
    out << YAML::Key << "directory_file_mail" << YAML::Value << cfg->directory_file_mail;
    out << YAML::Key << "directory_doors" << YAML::Value << cfg->directory_doors;
    out << YAML::Key << "directory_log" << YAML::Value << cfg->directory_log;
    out << YAML::Key << "directory_scripts" << YAML::Value << cfg->directory_scripts;
    out << YAML::Key << "directory_prompts" << YAML::Value << cfg->directory_prompts;
    out << YAML::Key << "points_per_kilobyte" << YAML::Value << cfg->points_per_kilobyte;
    out << YAML::Key << "points_earned_postcall" << YAML::Value << cfg->points_earned_postcall;
    out << YAML::Key << "access_post_anonymous" << YAML::Value << cfg->access_post_anonymous;
    out << YAML::Key << "access_sysop" << YAML::Value << cfg->access_sysop;
    out << YAML::Key << "access_message_sysop" << YAML::Value << cfg->access_message_sysop;
    out << YAML::Key << "access_file_sysop" << YAML::Value << cfg->access_file_sysop;
    out << YAML::Key << "access_hidden_files" << YAML::Value << cfg->access_hidden_files;
    out << YAML::Key << "access_mail_attachment" << YAML::Value << cfg->access_mail_attachment;
    out << YAML::Key << "access_top_ten" << YAML::Value << cfg->access_top_ten;
    out << YAML::Key << "access_check_sysop_avail" << YAML::Value << cfg->access_check_sysop_avail;
    out << YAML::Key << "use_file_points" << YAML::Value << cfg->use_file_points;
    out << YAML::Key << "use_postcall_ratio" << YAML::Value << cfg->use_postcall_ratio;
    out << YAML::Key << "use_library_ansi" << YAML::Value << cfg->use_library_ansi;
    out << YAML::Key << "use_notify_on_logins" << YAML::Value << cfg->use_notify_on_logins;
    out << YAML::Key << "use_notify_on_logoff" << YAML::Value << cfg->use_notify_on_logoff;
    out << YAML::Key << "use_log_chat_sessions" << YAML::Value << cfg->use_log_chat_sessions;
    out << YAML::Key << "use_screen_prelogin" << YAML::Value << cfg->use_screen_prelogin;
    out << YAML::Key << "use_screen_welcome" << YAML::Value << cfg->use_screen_welcome;
    out << YAML::Key << "use_matrix_login" << YAML::Value << cfg->use_matrix_login;
    out << YAML::Key << "hidden_input_character" << YAML::Value << cfg->hidden_input_character;
    out << YAML::Key << "use_auto_validate_users" << YAML::Value << cfg->use_auto_validate_users;
    out << YAML::Key << "use_newuser_voting" << YAML::Value << cfg->use_newuser_voting;
    out << YAML::Key << "use_auto_kick_unvalidated" << YAML::Value << cfg->use_auto_kick_unvalidated;
    out << YAML::Key << "newuser_votes_validate" << YAML::Value << cfg->newuser_votes_validate;
    out << YAML::Key << "newuser_votes_delete" << YAML::Value << cfg->newuser_votes_delete;
    out << YAML::Key << "newuser_days_to_upload" << YAML::Value << cfg->newuser_days_to_upload;
    out << YAML::Key << "days_keep_logs" << YAML::Value << cfg->days_keep_logs;
    out << YAML::Key << "qwk_packet_name" << YAML::Value << cfg->qwk_packet_name;
    out << YAML::Key << "starting_menu_name" << YAML::Value << cfg->starting_menu_name;
    out << YAML::Key << "use_message_attachments" << YAML::Value << cfg->use_message_attachments;
    out << YAML::Key << "days_keep_attachments" << YAML::Value << cfg->days_keep_attachments;
    out << YAML::Key << "default_color_regular" << YAML::Value << cfg->default_color_regular;
    out << YAML::Key << "default_color_stat" << YAML::Value << cfg->default_color_stat;
    out << YAML::Key << "default_color_propmpt" << YAML::Value << cfg->default_color_propmpt;
    out << YAML::Key << "default_color_input" << YAML::Value << cfg->default_color_input;
    out << YAML::Key << "default_color_inverse" << YAML::Value << cfg->default_color_inverse;
    out << YAML::Key << "default_color_box" << YAML::Value << cfg->default_color_box;
    out << YAML::Key << "default_user_flags" << YAML::Value << cfg->default_user_flags;
    out << YAML::Key << "flag_change_daily" << YAML::Value << cfg->flag_change_daily;
    out << YAML::Key << "flag_change_session" << YAML::Value << cfg->flag_change_session;
    out << YAML::Key << "default_level" << YAML::Value << cfg->default_level;
    out << YAML::Key << "default_file_level" << YAML::Value << cfg->default_file_level;
    out << YAML::Key << "default_message_level" << YAML::Value << cfg->default_message_level;
    out << YAML::Key << "default_file_points" << YAML::Value << cfg->default_file_points;
    out << YAML::Key << "default_file_ratio" << YAML::Value << cfg->default_file_ratio;
    out << YAML::Key << "default_kilobyte_ratio" << YAML::Value << cfg->default_kilobyte_ratio;
    out << YAML::Key << "default_kilobyte_daily" << YAML::Value << cfg->default_kilobyte_daily;
    out << YAML::Key << "default_post_call_ratio" << YAML::Value << cfg->default_post_call_ratio;
    out << YAML::Key << "default_time_limit" << YAML::Value << cfg->default_time_limit;
    out << YAML::Key << "default_user_timeout" << YAML::Value << cfg->default_user_timeout;
    out << YAML::Key << "use_auto_validate_files" << YAML::Value << cfg->use_auto_validate_files;
    out << YAML::Key << "use_upload_checker" << YAML::Value << cfg->use_upload_checker;
    out << YAML::Key << "cmdline_virus_scan" << YAML::Value << cfg->cmdline_virus_scan;
    out << YAML::Key << "filename_bbs_ad" << YAML::Value << cfg->filename_bbs_ad;
    out << YAML::Key << "filename_archive_comments" << YAML::Value << cfg->filename_archive_comments;
    out << YAML::Key << "directory_bad_files" << YAML::Value << cfg->directory_bad_files;
    out << YAML::Key << "use_greater_then_for_quotes" << YAML::Value << cfg->use_greater_then_for_quotes;

    out << YAML::EndMap;


    // Setup file to Write out File.
    std::ofstream ofs(path);
    if (!ofs.is_open())
    {
        std::cout << "Error, unable to write to: " << path << std::endl;
        return false;
    }

    ofs << out.c_str();
    ofs.close();

    return true;
}


/**
 * @brief Loads a Configuation file into the m_config stub for access.
 * @return
 */
bool ConfigDao::loadConfig()
{
    std::string path = m_path;
    path.append(m_filename);

    YAML::Node node;

    // Load the file into the class.
    try
    {
        // Load file fresh.
        node = YAML::LoadFile(path);
        Config c = node.as<Config>();
        m_config = boost::make_shared<Config>(c);
    }
    catch (std::exception ex)
    {
        std::cout << "Exception YAML::LoadFile(xrm-config.yaml) " << ex.what() << std::endl;
        assert(false);
    }

    return true;
}

