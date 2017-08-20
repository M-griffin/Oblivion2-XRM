#ifndef FORM_SYSTEM_CONFIG_HPP
#define FORM_SYSTEM_CONFIG_HPP

#include "form_base.hpp"

#include <boost/smart_ptr/shared_ptr.hpp>

#include <yaml-cpp/yaml.h>

class Config;
typedef boost::shared_ptr<Config> config_ptr;

/**
 * @class FormSystemConfig
 * @author Michael Griffin
 * @date 31/03/2017
 * @file form_system_config.hpp
 * @brief Generates the System Configuration layout
 */
class FormSystemConfig
    : public FormBase
{
public:
    explicit FormSystemConfig(config_ptr config);
    ~FormSystemConfig();

    virtual bool onEnter() override;
    virtual bool onExit() override;
    virtual void onSave() override;    
};


// YAML Overide namespace for encoding/decoding Config Class.
namespace YAML
{
    template<>
    struct convert<config_ptr>
    {
        /**
         * @brief Overide for encoding the Config Class
         *        This is not used as it makes the map on a single line!
         *        Instead Key/Value is done in the ConfigDao on save.
         * @param rhs
         * @return
         */
        static Node encode(const config_ptr &rhs)
        {
            Node node;
            node["file_version"] = rhs->file_version;
            node["bbs_name_sysop"] = rhs->bbs_name_sysop;
            node["bbs_name"] = rhs->bbs_name;
            node["bbs_uuid"] = rhs->bbs_uuid;
            node["password_system"] = rhs->password_system;
            node["password_sysop"] = rhs->password_sysop;
            node["password_newuser"] = rhs->password_newuser;
            node["port_telnet"] = rhs->port_telnet;
            node["port_ssl"] = rhs->port_ssl;
            node["use_service_telnet"] = rhs->use_service_telnet;
            node["use_service_ssl"] = rhs->use_service_ssl;
            node["directory_screens"] = rhs->directory_screens;
            node["directory_boards"] = rhs->directory_boards;
            node["directory_files"] = rhs->directory_files;
            node["directory_data"] = rhs->directory_data;
            node["directory_menu"] = rhs->directory_menu;
            node["directory_work"] = rhs->directory_work;
            node["directory_text_files"] = rhs->directory_text_files;
            node["directory_file_mail"] = rhs->directory_file_mail;
            node["directory_doors"] = rhs->directory_doors;
            node["directory_log"] = rhs->directory_log;
            node["directory_scripts"] = rhs->directory_scripts;
            node["directory_prompts"] = rhs->directory_prompts;
            node["points_per_megabyte"] = rhs->points_per_megabyte;
            node["points_earned_postcall"] = rhs->points_earned_postcall;
            node["access_post_anonymous"] = rhs->access_post_anonymous;
            node["access_sysop"] = rhs->access_sysop;
            node["access_message_sysop"] = rhs->access_message_sysop;
            node["access_file_sysop"] = rhs->access_file_sysop;
            node["access_hidden_files"] = rhs->access_hidden_files;
            node["access_mail_attachment"] = rhs->access_mail_attachment;
            node["access_top_ten"] = rhs->access_top_ten;
            node["access_check_sysop_avail"] = rhs->access_check_sysop_avail;
            node["invalid_password_attempts"] = rhs->invalid_password_attempts;
            node["invalid_newuser_password_attempts"] = rhs->invalid_newuser_password_attempts;
            node["use_file_points"] = rhs->use_file_points;
            node["use_postcall_ratio"] = rhs->use_postcall_ratio;
            node["use_library_ansi"] = rhs->use_library_ansi;
            node["use_notify_on_logins"] = rhs->use_notify_on_logins;
            node["use_notify_on_logoff"] = rhs->use_notify_on_logoff;
            node["use_log_chat_sessions"] = rhs->use_log_chat_sessions;
            node["use_screen_prelogin"] = rhs->use_screen_prelogin;
            node["use_screen_welcome"] = rhs->use_screen_welcome;
            node["use_matrix_login"] = rhs->use_matrix_login;
            node["use_newuser_password"] = rhs->use_newuser_password;
            node["use_disclaimer"] = rhs->use_disclaimer;                        
            node["use_handle"] = rhs->use_handle;
            node["use_real_name"] = rhs->use_real_name;
            node["use_location"] = rhs->use_location;
            node["use_country"] = rhs->use_country;
            node["use_email"] = rhs->use_email;
            node["use_user_note"] = rhs->use_user_note;
            node["use_birthdate"] = rhs->use_birthdate;
            node["use_gender"] = rhs->use_gender;
            node["use_challenge_question"] = rhs->use_challenge_question;
            node["use_yesno_bars"] = rhs->use_yesno_bars;
            node["use_pause"] = rhs->use_pause;
            node["use_clear_screen"] = rhs->use_clear_screen;
            node["use_ansi_color"] = rhs->use_ansi_color;
            node["use_backspace"] = rhs->use_backspace;                        
            node["hidden_input_character"] = rhs->hidden_input_character;
            node["use_auto_validate_users"] = rhs->use_auto_validate_users;
            node["use_newuser_voting"] = rhs->use_newuser_voting;
            node["use_auto_kick_unvalidated"] = rhs->use_auto_kick_unvalidated;
            node["newuser_votes_validate"] = rhs->newuser_votes_validate;
            node["newuser_votes_delete"] = rhs->newuser_votes_delete;
            node["newuser_days_to_upload"] = rhs->newuser_days_to_upload;
            node["days_keep_logs"] = rhs->days_keep_logs;
            node["qwk_packet_name"] = rhs->qwk_packet_name;
            node["starting_menu_name"] = rhs->starting_menu_name;
            node["use_message_attachments"] = rhs->use_message_attachments;
            node["days_keep_attachments"] = rhs->days_keep_attachments;
            node["default_color_regular"] = rhs->default_color_regular;
            node["default_color_stat"] = rhs->default_color_stat;
            node["default_color_prompt"] = rhs->default_color_prompt;
            node["default_color_input"] = rhs->default_color_input;
            node["default_color_inverse"] = rhs->default_color_inverse;
            node["default_color_box"] = rhs->default_color_box;
            node["default_user_flags"] = rhs->default_user_flags;
            node["flag_change_daily"] = rhs->flag_change_daily;
            node["flag_change_session"] = rhs->flag_change_session;
            node["default_level"] = rhs->default_level;
            node["default_file_level"] = rhs->default_file_level;
            node["default_message_level"] = rhs->default_message_level;
            node["default_file_points"] = rhs->default_file_points;
            node["default_file_ratio"] = rhs->default_file_ratio;
            node["default_megabyte_ratio"] = rhs->default_megabyte_ratio;
            node["default_megabyte_daily"] = rhs->default_megabyte_daily;
            node["default_post_call_ratio"] = rhs->default_post_call_ratio;
            node["default_time_limit"] = rhs->default_time_limit;
            node["default_user_timeout"] = rhs->default_user_timeout;
            node["use_auto_validate_files"] = rhs->use_auto_validate_files;
            node["use_upload_checker"] = rhs->use_upload_checker;
            node["cmdline_virus_scan"] = rhs->cmdline_virus_scan;
            node["filename_bbs_ad"] = rhs->filename_bbs_ad;
            node["filename_archive_comments"] = rhs->filename_archive_comments;
            node["directory_bad_files"] = rhs->directory_bad_files;
            node["use_greater_then_for_quotes"] = rhs->use_greater_then_for_quotes;
            node["regexp_generic_validation"] = rhs->regexp_generic_validation;
            node["regexp_handle_validation"] = rhs->regexp_handle_validation;
            node["regexp_password_validation"] = rhs->regexp_password_validation;
            node["regexp_date_validation"] = rhs->regexp_date_validation;
            node["regexp_email_validation"] = rhs->regexp_email_validation;            
            node["regexp_generic_validation_msg"] = rhs->regexp_generic_validation_msg;
            node["regexp_handle_validation_msg"] = rhs->regexp_handle_validation_msg;
            node["regexp_password_validation_msg"] = rhs->regexp_password_validation_msg;
            node["regexp_date_validation_msg"] = rhs->regexp_date_validation_msg;
            node["regexp_email_validation_msg"] = rhs->regexp_email_validation_msg;

            return node;
        }

        /**
         * @brief Overide for the Load from File, this read and piluaes the Config Object.
         * @param node
         * @param rhs
         * @return
         */
        static bool decode(const Node& node, config_ptr rhs)
        {
            rhs.reset(new Config());
            
            rhs->file_version                    = node["file_version"].as<std::string>();
            rhs->bbs_name_sysop                  = node["bbs_name_sysop"].as<std::string>();
            rhs->bbs_name                        = node["bbs_name"].as<std::string>();
            rhs->bbs_uuid                        = node["bbs_uuid"].as<std::string>();
            rhs->password_system                 = node["password_system"].as<std::string>();
            rhs->password_sysop                  = node["password_sysop"].as<std::string>();
            rhs->password_newuser                = node["password_newuser"].as<std::string>();
            rhs->port_telnet                     = node["port_telnet"].as<int>();
            rhs->port_ssl                        = node["port_ssl"].as<int>();
            rhs->use_service_telnet              = node["use_service_telnet"].as<bool>();
            rhs->use_service_ssl                 = node["use_service_ssl"].as<bool>();
            rhs->directory_screens               = node["directory_screens"].as<std::string>();
            rhs->directory_boards                = node["directory_boards"].as<std::string>();
            rhs->directory_files                 = node["directory_files"].as<std::string>();
            rhs->directory_data                  = node["directory_data"].as<std::string>();
            rhs->directory_menu                  = node["directory_menu"].as<std::string>();
            rhs->directory_work                  = node["directory_work"].as<std::string>();
            rhs->directory_text_files            = node["directory_text_files"].as<std::string>();
            rhs->directory_file_mail             = node["directory_file_mail"].as<std::string>();
            rhs->directory_doors                 = node["directory_doors"].as<std::string>();
            rhs->directory_log                   = node["directory_log"].as<std::string>();
            rhs->directory_scripts               = node["directory_scripts"].as<std::string>();
            rhs->directory_prompts               = node["directory_prompts"].as<std::string>();
            rhs->points_per_megabyte             = node["points_per_megabyte"].as<int>();
            rhs->points_earned_postcall          = node["points_earned_postcall"].as<int>();
            rhs->access_sysop                    = node["access_sysop"].as<std::string>();
            rhs->access_message_sysop            = node["access_message_sysop"].as<std::string>();
            rhs->access_file_sysop               = node["access_file_sysop"].as<std::string>();
            rhs->access_hidden_files             = node["access_hidden_files"].as<std::string>();
            rhs->access_post_anonymous           = node["access_post_anonymous"].as<std::string>();
            rhs->access_mail_attachment          = node["access_mail_attachment"].as<std::string>();
            rhs->access_top_ten                  = node["access_top_ten"].as<std::string>();
            rhs->access_check_sysop_avail        = node["access_check_sysop_avail"].as<std::string>();            
            rhs->invalid_password_attempts       = node["invalid_password_attempts"].as<int>();
            rhs->invalid_newuser_password_attempts = node["invalid_newuser_password_attempts"].as<int>();
            rhs->use_file_points                 = node["use_file_points"].as<bool>();
            rhs->use_postcall_ratio              = node["use_postcall_ratio"].as<bool>();
            rhs->use_library_ansi                = node["use_library_ansi"].as<bool>();
            rhs->use_notify_on_logins            = node["use_notify_on_logins"].as<bool>();
            rhs->use_notify_on_logoff            = node["use_notify_on_logoff"].as<bool>();
            rhs->use_log_chat_sessions           = node["use_log_chat_sessions"].as<bool>();
            rhs->use_screen_prelogin             = node["use_screen_prelogin"].as<bool>();
            rhs->use_screen_welcome              = node["use_screen_welcome"].as<bool>();
            rhs->use_matrix_login                = node["use_matrix_login"].as<bool>();
            rhs->use_newuser_password            = node["use_newuser_password"].as<bool>();
            rhs->use_disclaimer                  = node["use_disclaimer"].as<bool>();            
            rhs->use_handle                      = node["use_handle"].as<bool>();
            rhs->use_real_name                   = node["use_real_name"].as<bool>();
            rhs->use_location                    = node["use_location"].as<bool>();
            rhs->use_country                     = node["use_country"].as<bool>();
            rhs->use_email                       = node["use_email"].as<bool>();
            rhs->use_user_note                   = node["use_user_note"].as<bool>();
            rhs->use_birthdate                   = node["use_birthdate"].as<bool>();
            rhs->use_gender                      = node["use_gender"].as<bool>();
            rhs->use_challenge_question          = node["use_challenge_question"].as<bool>();
            rhs->use_yesno_bars                  = node["use_yesno_bars"].as<bool>();
            rhs->use_pause                       = node["use_pause"].as<bool>();
            rhs->use_clear_screen                = node["use_clear_screen"].as<bool>();
            rhs->use_ansi_color                  = node["use_ansi_color"].as<bool>();
            rhs->use_backspace                   = node["use_backspace"].as<bool>();
            rhs->hidden_input_character          = node["hidden_input_character"].as<unsigned char>();
            rhs->use_auto_validate_users         = node["use_auto_validate_users"].as<bool>();
            rhs->use_newuser_voting              = node["use_newuser_voting"].as<bool>();
            rhs->use_auto_kick_unvalidated       = node["use_auto_kick_unvalidated"].as<bool>();
            rhs->newuser_votes_validate          = node["newuser_votes_validate"].as<int>();
            rhs->newuser_votes_delete            = node["newuser_votes_delete"].as<int>();
            rhs->newuser_days_to_upload          = node["newuser_days_to_upload"].as<int>();
            rhs->days_keep_logs                  = node["days_keep_logs"].as<int>();
            rhs->qwk_packet_name                 = node["qwk_packet_name"].as<std::string>();
            rhs->starting_menu_name              = node["starting_menu_name"].as<std::string>();
            rhs->use_message_attachments         = node["use_message_attachments"].as<bool>();
            rhs->days_keep_attachments           = node["days_keep_attachments"].as<int>();
            rhs->default_color_regular           = node["default_color_regular"].as<std::string>();
            rhs->default_color_stat              = node["default_color_stat"].as<std::string>();
            rhs->default_color_prompt            = node["default_color_prompt"].as<std::string>();
            rhs->default_color_input             = node["default_color_input"].as<std::string>();
            rhs->default_color_inverse           = node["default_color_inverse"].as<std::string>();
            rhs->default_color_box               = node["default_color_box"].as<std::string>();
            rhs->default_user_flags              = node["default_user_flags"].as<std::string>();
            rhs->flag_change_daily               = node["flag_change_daily"].as<std::string>();
            rhs->flag_change_session             = node["flag_change_session"].as<std::string>();
            rhs->default_level                   = node["default_level"].as<int>();
            rhs->default_file_level              = node["default_file_level"].as<int>();
            rhs->default_message_level           = node["default_message_level"].as<int>();
            rhs->default_file_points             = node["default_file_points"].as<int>();
            rhs->default_file_ratio              = node["default_file_ratio"].as<int>();
            rhs->default_megabyte_ratio          = node["default_megabyte_ratio"].as<int>();
            rhs->default_megabyte_daily          = node["default_megabyte_daily"].as<int>();
            rhs->default_post_call_ratio         = node["default_post_call_ratio"].as<int>();
            rhs->default_time_limit              = node["default_time_limit"].as<int>();
            rhs->default_user_timeout            = node["default_user_timeout"].as<int>();
            rhs->use_auto_validate_files         = node["use_auto_validate_files"].as<bool>();
            rhs->use_upload_checker              = node["use_upload_checker"].as<bool>();
            rhs->cmdline_virus_scan              = node["cmdline_virus_scan"].as<std::string>();
            rhs->filename_bbs_ad                 = node["filename_bbs_ad"].as<std::string>();
            rhs->filename_archive_comments       = node["filename_archive_comments"].as<std::string>();
            rhs->directory_bad_files             = node["directory_bad_files"].as<std::string>();
            rhs->use_greater_then_for_quotes     = node["use_greater_then_for_quotes"].as<bool>();
            rhs->regexp_generic_validation       = node["regexp_generic_validation"].as<std::string>();
            rhs->regexp_handle_validation        = node["regexp_handle_validation"].as<std::string>();
            rhs->regexp_password_validation      = node["regexp_password_validation"].as<std::string>();
            rhs->regexp_date_validation          = node["regexp_date_validation"].as<std::string>();
            rhs->regexp_email_validation         = node["regexp_email_validation"].as<std::string>();            
            rhs->regexp_generic_validation_msg   = node["regexp_generic_validation_msg"].as<std::string>();
            rhs->regexp_handle_validation_msg    = node["regexp_handle_validation_msg"].as<std::string>();
            rhs->regexp_password_validation_msg  = node["regexp_password_validation_msg"].as<std::string>();
            rhs->regexp_date_validation_msg      = node["regexp_date_validation_msg"].as<std::string>();
            rhs->regexp_email_validation_msg     = node["regexp_email_validation_msg"].as<std::string>();

            return true;
        }
    };
}


#endif // FORM_SYSTEM_CONFIG_HPP
