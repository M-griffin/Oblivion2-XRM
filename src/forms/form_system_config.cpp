#include "form_system_config.hpp"

#include "../model-sys/config.hpp"
#include "../model-sys/menu.hpp"

#include <string>

FormSystemConfig::FormSystemConfig(config_ptr config)
    : FormBase(
        config, 
        "sysconfig",              // name
        "System Config",          // title
        "form_config",            // pulldown
        "SYSCNFSRT.ANS",          // ANSI Templates.
        "SYSCNFMID.ANS",
        "SYSCNFEND.ANS"
    )
{
}

FormSystemConfig::~FormSystemConfig()
{
}

/**
 * @brief Setup for Form Menu Options
 * @return 
 */
bool FormSystemConfig::onEnter() 
{       
    // Setup the YAML Noide with base data and type.
    setNodeMappingType(m_config);
    
    setupBuildOptions("bbs_name_sysop", m_config->bbs_name_sysop);
    setupBuildOptions("bbs_name", m_config->bbs_name);
    setupBuildOptions("bbs_uuid", m_config->bbs_uuid);
    setupBuildOptions("password_system", m_config->password_system);
    setupBuildOptions("password_sysop", m_config->password_sysop);
    setupBuildOptions("password_newuser", m_config->password_newuser);
    setupBuildOptions("port_telnet", m_config->port_telnet);
    setupBuildOptions("use_service_telnet", m_config->use_service_telnet);
    setupBuildOptions("port_ssl", m_config->port_ssl);    
    setupBuildOptions("use_service_ssl", m_config->use_service_ssl);       

    setupBuildOptions("directory_screens", m_config->directory_screens);
    setupBuildOptions("directory_boards", m_config->directory_boards);
    setupBuildOptions("directory_files", m_config->directory_files);
    setupBuildOptions("directory_data", m_config->directory_data);
    setupBuildOptions("directory_menu", m_config->directory_menu);
    setupBuildOptions("directory_work", m_config->directory_work);
    setupBuildOptions("directory_text_files", m_config->directory_text_files);
    setupBuildOptions("directory_file_mail", m_config->directory_file_mail);
    setupBuildOptions("directory_doors", m_config->directory_doors);
    setupBuildOptions("directory_log", m_config->directory_log);
    setupBuildOptions("directory_scripts", m_config->directory_scripts);
    setupBuildOptions("directory_prompts", m_config->directory_prompts);
    
    setupBuildOptions("points_per_megabyte", m_config->points_per_megabyte);
    setupBuildOptions("points_earned_postcall", m_config->points_earned_postcall);
    setupBuildOptions("access_sysop", m_config->access_sysop);
    setupBuildOptions("access_message_sysop", m_config->access_message_sysop);
    setupBuildOptions("access_file_sysop", m_config->access_file_sysop);
    setupBuildOptions("access_hidden_files", m_config->access_hidden_files);
    setupBuildOptions("access_post_anonymous", m_config->access_post_anonymous); 
    setupBuildOptions("access_mail_attachment", m_config->access_mail_attachment); 
    setupBuildOptions("access_top_ten", m_config->access_top_ten); 
    setupBuildOptions("access_check_sysop_avail", m_config->access_check_sysop_avail); 
    
    setupBuildOptions("invalid_password_attempts", m_config->invalid_password_attempts); 
    setupBuildOptions("invalid_newuser_password_attempts", m_config->invalid_newuser_password_attempts); 
    
    setupBuildOptions("use_file_points", m_config->use_file_points); 
    setupBuildOptions("use_postcall_ratio", m_config->use_postcall_ratio); 
    setupBuildOptions("use_library_ansi", m_config->use_library_ansi); 
    setupBuildOptions("use_notify_on_logins", m_config->use_notify_on_logins); 
    setupBuildOptions("use_notify_on_logoff", m_config->use_notify_on_logoff); 
    setupBuildOptions("use_log_chat_sessions", m_config->use_log_chat_sessions); 
    setupBuildOptions("use_screen_prelogin", m_config->use_screen_prelogin); 
    setupBuildOptions("use_screen_welcome", m_config->use_screen_welcome); 
    setupBuildOptions("use_matrix_login", m_config->use_matrix_login); 
    
    setupBuildOptions("use_newuser_password", m_config->use_newuser_password); 
    setupBuildOptions("use_disclaimer", m_config->use_disclaimer); 
    setupBuildOptions("use_address", m_config->use_address); 
    setupBuildOptions("use_handle", m_config->use_handle); 
    setupBuildOptions("use_real_name", m_config->use_real_name); 
    setupBuildOptions("use_location", m_config->use_location); 
    setupBuildOptions("use_country", m_config->use_country);     
    setupBuildOptions("use_email", m_config->use_email);     
    setupBuildOptions("use_user_note", m_config->use_user_note); 
    setupBuildOptions("use_birthdate", m_config->use_birthdate); 
    setupBuildOptions("use_gender", m_config->use_gender); 
    setupBuildOptions("use_challenge_question", m_config->use_challenge_question); 
    setupBuildOptions("use_yesno_bars", m_config->use_yesno_bars); 
    setupBuildOptions("use_pause", m_config->use_pause);    
    setupBuildOptions("use_clear_screen", m_config->use_clear_screen);
    setupBuildOptions("use_ansi_color", m_config->use_ansi_color);
    setupBuildOptions("use_backspace", m_config->use_backspace);
    setupBuildOptions("hidden_input_character", m_config->hidden_input_character);
    setupBuildOptions("use_auto_validate_users", m_config->use_auto_validate_users);
    setupBuildOptions("use_auto_kick_unvalidated", m_config->use_auto_kick_unvalidated);
    setupBuildOptions("newuser_votes_validate", m_config->newuser_votes_validate);
    setupBuildOptions("newuser_votes_delete", m_config->newuser_votes_delete);
    setupBuildOptions("newuser_days_to_upload", m_config->newuser_days_to_upload);
    setupBuildOptions("days_keep_logs", m_config->days_keep_logs);
    setupBuildOptions("qwk_packet_name", m_config->qwk_packet_name);
    setupBuildOptions("starting_menu_name", m_config->starting_menu_name);
    setupBuildOptions("use_message_attachments", m_config->use_message_attachments);
    
    setupBuildOptions("default_color_regular", m_config->default_color_regular);
    setupBuildOptions("default_color_stat", m_config->default_color_stat);
    setupBuildOptions("default_color_prompt", m_config->default_color_prompt);
    setupBuildOptions("default_color_input", m_config->default_color_input);
    setupBuildOptions("default_color_inverse", m_config->default_color_inverse);
    setupBuildOptions("default_color_box", m_config->default_color_box);
    setupBuildOptions("default_user_flags", m_config->default_user_flags);
    setupBuildOptions("flag_change_daily", m_config->flag_change_daily);
    setupBuildOptions("flag_change_session", m_config->flag_change_session);
    setupBuildOptions("default_level", m_config->default_level);
    setupBuildOptions("default_file_level", m_config->default_file_level);
    setupBuildOptions("default_message_level", m_config->default_message_level);    
    setupBuildOptions("default_file_points", m_config->default_file_points);
    setupBuildOptions("default_file_ratio", m_config->default_file_ratio);
    setupBuildOptions("default_megabyte_ratio", m_config->default_megabyte_ratio);
    setupBuildOptions("default_megabyte_daily", m_config->default_megabyte_daily);
    setupBuildOptions("default_post_call_ratio", m_config->default_post_call_ratio);
    setupBuildOptions("default_time_limit", m_config->default_time_limit);
    setupBuildOptions("default_user_timeout", m_config->default_user_timeout);
    setupBuildOptions("use_auto_validate_files", m_config->use_auto_validate_files);
    setupBuildOptions("use_upload_checker", m_config->use_upload_checker);
    setupBuildOptions("cmdline_virus_scan", m_config->cmdline_virus_scan);
    
    setupBuildOptions("filename_bbs_ad", m_config->filename_bbs_ad);
    setupBuildOptions("filename_archive_comments", m_config->filename_archive_comments);
    setupBuildOptions("directory_bad_files", m_config->directory_bad_files);
    setupBuildOptions("use_greater_then_for_quotes", m_config->use_greater_then_for_quotes);
    setupBuildOptions("regexp_generic_validation", m_config->regexp_generic_validation);
    setupBuildOptions("regexp_generic_validation_msg", m_config->regexp_generic_validation_msg);
    setupBuildOptions("regexp_handle_validation", m_config->regexp_handle_validation);
    setupBuildOptions("regexp_handle_validation_msg", m_config->regexp_handle_validation_msg);
    setupBuildOptions("regexp_password_validation", m_config->regexp_password_validation);
    setupBuildOptions("regexp_password_validation_msg", m_config->regexp_password_validation_msg);
    setupBuildOptions("regexp_date_validation", m_config->regexp_date_validation);
    setupBuildOptions("regexp_date_validation_msg", m_config->regexp_date_validation_msg);
    setupBuildOptions("regexp_email_validation", m_config->regexp_email_validation);
    setupBuildOptions("regexp_email_validation_msg", m_config->regexp_email_validation_msg);
    return true;
}

/**
 * @brief Cleanup for Form Menu Options
 * @return 
 */
bool FormSystemConfig::onExit()
{
    baseClearOptions();
    return true;
}

/**
 * @brief Saves Configuration Record to File
 */
void FormSystemConfig::onSave()
{
    
}


