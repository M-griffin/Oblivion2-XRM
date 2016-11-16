#include "config_dao.hpp"
#include "../model/config.hpp"

#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/smart_ptr/make_shared.hpp>

#include <iostream>
#include <fstream>
#include <string>

#include <mutex>


// Setup the file version for the config file.
const std::string Config::FILE_VERSION = "1.0.0";


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
    
    out << YAML::Key << "file_version" << YAML::Value << cfg->file_version;
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
    out << YAML::Key << "use_newuser_password" << YAML::Value << cfg->use_newuser_password;
    out << YAML::Key << "use_disclaimer" << YAML::Value << cfg->use_disclaimer;        
    out << YAML::Key << "use_address" << YAML::Value << cfg->use_address;
    out << YAML::Key << "use_handle" << YAML::Value << cfg->use_handle;
    out << YAML::Key << "use_real_name" << YAML::Value << cfg->use_real_name;
    out << YAML::Key << "use_location" << YAML::Value << cfg->use_location;
    out << YAML::Key << "use_country" << YAML::Value << cfg->use_country;
    out << YAML::Key << "use_email" << YAML::Value << cfg->use_email;
    out << YAML::Key << "use_user_note" << YAML::Value << cfg->use_user_note;
    out << YAML::Key << "use_birthdate" << YAML::Value << cfg->use_birthdate;
    out << YAML::Key << "use_gender" << YAML::Value << cfg->use_gender;
    out << YAML::Key << "use_challenge_question" << YAML::Value << cfg->use_challenge_question;
    out << YAML::Key << "use_yesno_bars" << YAML::Value << cfg->use_yesno_bars;
    out << YAML::Key << "use_pause" << YAML::Value << cfg->use_pause;
    out << YAML::Key << "use_clear_screen" << YAML::Value << cfg->use_clear_screen;
    out << YAML::Key << "use_ansi_color" << YAML::Value << cfg->use_ansi_color;
    out << YAML::Key << "use_backspace" << YAML::Value << cfg->use_backspace;        
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
    out << YAML::Key << "regexp_generic_validation" << YAML::Value << cfg->regexp_generic_validation;
    out << YAML::Key << "regexp_generic_validation_msg" << YAML::Value << cfg->regexp_generic_validation_msg;
    out << YAML::Key << "regexp_handle_validation" << YAML::Value << cfg->regexp_handle_validation;
    out << YAML::Key << "regexp_handle_validation_msg" << YAML::Value << cfg->regexp_handle_validation_msg;
    out << YAML::Key << "regexp_password_validation" << YAML::Value << cfg->regexp_password_validation;
    out << YAML::Key << "regexp_password_validation_msg" << YAML::Value << cfg->regexp_password_validation_msg;
    out << YAML::Key << "regexp_date_validation" << YAML::Value << cfg->regexp_date_validation;
    out << YAML::Key << "regexp_date_validation_msg" << YAML::Value << cfg->regexp_date_validation_msg;
    out << YAML::Key << "regexp_email_validation" << YAML::Value << cfg->regexp_email_validation;    
    out << YAML::Key << "regexp_email_validation_msg" << YAML::Value << cfg->regexp_email_validation_msg;
    
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
 * @brief Moves the Loaded config to the shared pointer.
 *        This can probably be redone lateron with copy constructors..
 * @param rhs
 * @return
 */
void ConfigDao::encode(const Config &rhs)
{      
    m_config->file_version = rhs.file_version;
    m_config->bbs_name_sysop = rhs.bbs_name_sysop;
    m_config->bbs_name = rhs.bbs_name;
    m_config->bbs_uuid = rhs.bbs_uuid;
    m_config->password_system = rhs.password_system;
    m_config->password_sysop = rhs.password_sysop;
    m_config->password_newuser = rhs.password_newuser;
    m_config->port_telnet = rhs.port_telnet;
    m_config->port_ssl = rhs.port_ssl;
    m_config->use_service_telnet = rhs.use_service_telnet;
    m_config->use_service_ssl = rhs.use_service_ssl;
    m_config->directory_screens = rhs.directory_screens;
    m_config->directory_boards = rhs.directory_boards;
    m_config->directory_files = rhs.directory_files;
    m_config->directory_data = rhs.directory_data;
    m_config->directory_menu = rhs.directory_menu;
    m_config->directory_work = rhs.directory_work;
    m_config->directory_text_files = rhs.directory_text_files;
    m_config->directory_file_mail = rhs.directory_file_mail;
    m_config->directory_doors = rhs.directory_doors;
    m_config->directory_log = rhs.directory_log;
    m_config->directory_scripts = rhs.directory_scripts;
    m_config->directory_prompts = rhs.directory_prompts;
    m_config->points_per_kilobyte = rhs.points_per_kilobyte;
    m_config->points_earned_postcall = rhs.points_earned_postcall;
    m_config->access_post_anonymous = rhs.access_post_anonymous;
    m_config->access_sysop = rhs.access_sysop;
    m_config->access_message_sysop = rhs.access_message_sysop;
    m_config->access_file_sysop = rhs.access_file_sysop;
    m_config->access_hidden_files = rhs.access_hidden_files;
    m_config->access_mail_attachment = rhs.access_mail_attachment;
    m_config->access_top_ten = rhs.access_top_ten;
    m_config->access_check_sysop_avail = rhs.access_check_sysop_avail;
    m_config->use_file_points = rhs.use_file_points;
    m_config->use_postcall_ratio = rhs.use_postcall_ratio;
    m_config->use_library_ansi = rhs.use_library_ansi;
    m_config->use_notify_on_logins = rhs.use_notify_on_logins;
    m_config->use_notify_on_logoff = rhs.use_notify_on_logoff;
    m_config->use_log_chat_sessions = rhs.use_log_chat_sessions;
    m_config->use_screen_prelogin = rhs.use_screen_prelogin;
    m_config->use_screen_welcome = rhs.use_screen_welcome;
    m_config->use_matrix_login = rhs.use_matrix_login;
    m_config->use_newuser_password = rhs.use_newuser_password;
    m_config->use_disclaimer = rhs.use_disclaimer;    
    m_config->use_address = rhs.use_address;
    m_config->use_handle = rhs.use_handle;
    m_config->use_real_name = rhs.use_real_name;
    m_config->use_location = rhs.use_location;
    m_config->use_country = rhs.use_country;
    m_config->use_email = rhs.use_email;
    m_config->use_user_note = rhs.use_user_note;
    m_config->use_birthdate = rhs.use_birthdate;
    m_config->use_gender = rhs.use_gender;
    m_config->use_challenge_question = rhs.use_challenge_question;
    m_config->use_yesno_bars = rhs.use_yesno_bars;
    m_config->use_pause = rhs.use_pause;
    m_config->use_clear_screen = rhs.use_clear_screen;
    m_config->use_ansi_color = rhs.use_ansi_color;
    m_config->use_backspace = rhs.use_backspace;    
    m_config->hidden_input_character = rhs.hidden_input_character;
    m_config->use_auto_validate_users = rhs.use_auto_validate_users;
    m_config->use_newuser_voting = rhs.use_newuser_voting;
    m_config->use_auto_kick_unvalidated = rhs.use_auto_kick_unvalidated;
    m_config->newuser_votes_validate = rhs.newuser_votes_validate;
    m_config->newuser_votes_delete = rhs.newuser_votes_delete;
    m_config->newuser_days_to_upload = rhs.newuser_days_to_upload;
    m_config->days_keep_logs = rhs.days_keep_logs;
    m_config->qwk_packet_name = rhs.qwk_packet_name;
    m_config->starting_menu_name = rhs.starting_menu_name;
    m_config->use_message_attachments = rhs.use_message_attachments;
    m_config->days_keep_attachments = rhs.days_keep_attachments;
    m_config->default_color_regular = rhs.default_color_regular;
    m_config->default_color_stat = rhs.default_color_stat;
    m_config->default_color_propmpt = rhs.default_color_propmpt;
    m_config->default_color_input = rhs.default_color_input;
    m_config->default_color_inverse = rhs.default_color_inverse;
    m_config->default_color_box = rhs.default_color_box;
    m_config->default_user_flags = rhs.default_user_flags;
    m_config->flag_change_daily = rhs.flag_change_daily;
    m_config->flag_change_session = rhs.flag_change_session;
    m_config->default_level = rhs.default_level;
    m_config->default_file_level = rhs.default_file_level;
    m_config->default_message_level = rhs.default_message_level;
    m_config->default_file_points = rhs.default_file_points;
    m_config->default_file_ratio = rhs.default_file_ratio;
    m_config->default_kilobyte_ratio = rhs.default_kilobyte_ratio;
    m_config->default_kilobyte_daily = rhs.default_kilobyte_daily;
    m_config->default_post_call_ratio = rhs.default_post_call_ratio;
    m_config->default_time_limit = rhs.default_time_limit;
    m_config->default_user_timeout = rhs.default_user_timeout;
    m_config->use_auto_validate_files = rhs.use_auto_validate_files;
    m_config->use_upload_checker = rhs.use_upload_checker;
    m_config->cmdline_virus_scan = rhs.cmdline_virus_scan;
    m_config->filename_bbs_ad = rhs.filename_bbs_ad;
    m_config->filename_archive_comments = rhs.filename_archive_comments;
    m_config->directory_bad_files = rhs.directory_bad_files;
    m_config->use_greater_then_for_quotes = rhs.use_greater_then_for_quotes;    
    m_config->regexp_generic_validation = rhs.regexp_generic_validation;
    m_config->regexp_generic_validation_msg = rhs.regexp_generic_validation_msg;
    m_config->regexp_handle_validation = rhs.regexp_handle_validation;
    m_config->regexp_handle_validation_msg = rhs.regexp_handle_validation_msg;
    m_config->regexp_password_validation = rhs.regexp_password_validation;
    m_config->regexp_password_validation_msg = rhs.regexp_password_validation_msg;
    m_config->regexp_date_validation = rhs.regexp_date_validation;
    m_config->regexp_date_validation_msg = rhs.regexp_date_validation_msg;
    m_config->regexp_email_validation = rhs.regexp_email_validation;    
    m_config->regexp_email_validation_msg = rhs.regexp_email_validation_msg;
    
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
        
        // Testing Is on nodes always throws exceptions.
        if (node.size() == 0) 
        {
            return false; //File Not Found?
        }
        
        std::string file_version = node["file_version"].as<std::string>();
        
        // Validate File Version
        std::cout << "Config File Version: " << file_version << std::endl;
        if (file_version != Config::FILE_VERSION) {
            throw std::invalid_argument("Invalid file_version, expected: " + Config::FILE_VERSION);
        }
        
        // When doing node.as (all fields must be present on file)
        Config c = node.as<Config>();

        // Moves the Loaded config to m_config shared pointer.
        encode(c);
    }
    catch (YAML::Exception &ex)
    {
        std::cout << "YAML::LoadFile(xrm-config.yaml) " << ex.what() << std::endl;
        std::cout << "Most likely a required field in the config file is missing. " << std::endl;
        assert(false);
    }
    catch (std::exception &ex)
    {
        std::cout << "Unexpected YAML::LoadFile(xrm-config.yaml) " << ex.what() << std::endl;
        assert(false);
    }

    return true;
}

