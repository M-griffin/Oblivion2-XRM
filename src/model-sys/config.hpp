#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/smart_ptr/weak_ptr.hpp>

// turn off the specific warning. Boost 1_64_0 for uuid
#pragma GCC diagnostic ignored "-Wconversion-null"

#include <boost/uuid/uuid.hpp>            // uuid class
#include <boost/uuid/uuid_generators.hpp> // generators
#include <boost/uuid/uuid_io.hpp>         // streaming operators etc.

// turn the warnings back on
#pragma GCC diagnostic pop

#include <boost/lexical_cast.hpp>

#include <fstream>

#include <yaml-cpp/yaml.h>

/**
 * @class Config Object
 * @author Michael Griffin
 * @date 4/05/2016
 * @file config.hpp
 * @brief Config Model - To output serilized data to and from .yaml files.
 */
class Config
{
    
public:    

    const static std::string FILE_VERSION;

    // Static Field Lengths to keep fields standard throughout
    // The internal and external program.
    static enum {

        sName_length             = 35,
        sPassword_length         = 35,
        sDirectory_length        = 35,
        sAccess_length           = 35,
        sMenuPrompt_length       = 35,
        sDefault_color_length    = 3,
        sDefault_question_length = 35,
        sControl_string_length   = 80,
        sQwk_packet_name_length  = 10,
        sMenu_name_length        = 10,
        sSingle_key_length       = 1,
        sDate_length             = 10

    } FIELD_LENGTH;


    std::string file_version;          // File Version.

// string
    std::string bbs_name_sysop;        // SysopName,
    std::string bbs_name;              // BoardName;
    std::string bbs_uuid;              // new {unique uuid for this bbs for services etc }
    std::string password_system;       // SysPass,
    std::string password_sysop;        // System1Pass,
    std::string password_newuser;      // NewUserPass;

// int
    int port_telnet;           // new { default telnet port }
    int port_ssl;              // new { default ssl port }

// bool
    bool use_service_telnet;    // new
    bool use_service_ssl;       // new
    std::string directory_screens;     // TextDir,
    std::string directory_boards;      // BoardDir,
    std::string directory_files;       // FilesDir,
    std::string directory_data;        // DataDir,
    std::string directory_menu;        // MenuDir,
    std::string directory_work;        // WorkDir,
    std::string directory_text_files;  // TFileDir,
    std::string directory_file_mail;   // FileMailDir,
    std::string directory_doors;       // DoorDir,
    std::string directory_log;         // LogDir;
    std::string directory_scripts;     // new
    std::string directory_prompts;     // IncPromptsDir;  { This might be redone }

// int
    int points_per_megabyte;       // KForEachPoint;
    int points_earned_postcall;    // PointsBackPC;

// string - Should ASC be strings or TAGS! I'm thinking tags or groups!

    std::string access_sysop;              // SysopACS,
    std::string access_message_sysop;      // MSysopACS,
    std::string access_file_sysop;         // FSysopACS;
    std::string access_hidden_files;       // HiddenFilesACS,
    std::string access_post_anonymous;     // PostAnon,

// std::string
    std::string access_mail_attachment;    // FileMailACS,
    std::string access_top_ten;            // TopTenACS,
    std::string access_check_sysop_avail;  // AskAvailabilityACS;
    
// int    
    int invalid_password_attempts;         
    int invalid_newuser_password_attempts;

// bool
    bool use_file_points;
    bool use_postcall_ratio;
    bool use_library_ansi;        // LibraryDAAnsi,
    bool use_notify_on_logins;    // InformUsers;  { Notify when users connect / disconnect }
    bool use_notify_on_logoff;    // bool InformLogoff;
    bool use_log_chat_sessions;   // DoLogChat,
    bool use_screen_prelogin;     // UsePrelogon,
    bool use_screen_welcome;      // UseWelcome,
    bool use_matrix_login;        // SpecialLogin;
    
    // New User Prompts
    bool use_newuser_password;    // Use a NewUser Password
    bool use_disclaimer;          // Show Disclaimer
    bool use_address;             // Ask Address New User Application
    bool use_handle;              // Ask for Handle
    bool use_real_name;           // Ask for RealName
    bool use_location;            // Ask Location
    bool use_country;             // Ask Country
    bool use_email;               // Ask Email
    bool use_user_note;           // Ask User Note
    bool use_birthdate;           // Ask Birthdate
    bool use_gender;              // Ask Gender
    bool use_challenge_question;  // Ask for Challenge Question
    bool use_yesno_bars;          // Ask for yes/no bars
    bool use_pause;               // Ask for Pause
    bool use_clear_screen;        // Ask Clear or Scroll
    bool use_ansi_color;          // Ask ANSI
    bool use_backspace;           // Ask Backspace
    
    
// char
    char hidden_input_character;  // HiddenInputChar;

// bool
    bool use_auto_validate_users; // AutoValNewUsers;
    bool use_newuser_voting;      // UseNuv;
    bool use_auto_kick_unvalidated; // KickOffNotV;    { kicks off unvalidated users }

// int
    int newuser_votes_validate;  // VotesToValid,
    int newuser_votes_delete;    // VotesToDel;
    int newuser_days_to_upload;  // DaysToAddFNuv;  { Days for new users to upload a file }

//int
    int days_keep_logs;          // DaysToSaveLog;
    
// string
    std::string qwk_packet_name;         // QWKName;

// string
    std::string starting_menu_name;      // SMenu;

// bool
    bool use_message_attachments;  // AllowFileMail;

// int
    int days_keep_attachments;     // DaysToKeepFM;
    
// string { convert char or digit to pipe codes! }
    std::string default_color_regular;    // DefRegColor,
    std::string default_color_stat;       // DefStatColor,
    std::string default_color_prompt;     // DefPromptColor,
    std::string default_color_input;      // DefInputColor,
    std::string default_color_inverse;    // DefInverseColor,
    std::string default_color_box;        // DefBoxColor;

// string
    std::string default_user_flags;      // DefaultFlags;
    std::string flag_change_daily;       // DailyFlagChange,
    std::string flag_change_session; 	 // CallFlagChange;

// int
    int default_level; 			 // DefLevel,
    int default_file_level;	     // DefFLevel
    int default_message_level;   // new

// int
    int default_file_points;     // DefFilePoints,
    int default_file_ratio;      // DefUDR,
    int default_megabyte_ratio;  // DefUDKR,
    int default_megabyte_daily;  // DnKPerDay,
    int default_post_call_ratio; // DefPCR,
    int default_time_limit;      // DefTimeLimit;
    int default_user_timeout;    // NumMinsBeforeTimeOut;

// bool
    bool use_auto_validate_files; // AutoValidFiles,
    bool use_upload_checker;      // UseULChecker;

// string  { Upload Checker }
    std::string cmdline_virus_scan;        // ScanCom;
    std::string filename_bbs_ad;           // BBSAd,
    std::string filename_archive_comments; // CommentFN;
    std::string directory_bad_files;       // BadDir;

// bool
    bool use_greater_then_for_quotes;  // UseGreaterThanSign;

// Regexp String Validations
    std::string regexp_generic_validation;
    std::string regexp_generic_validation_msg;
    
    std::string regexp_handle_validation;
    std::string regexp_handle_validation_msg;
    
    std::string regexp_password_validation;
    std::string regexp_password_validation_msg;
    
    std::string regexp_date_validation;
    std::string regexp_date_validation_msg;
    
    std::string regexp_email_validation;
    std::string regexp_email_validation_msg;
    

    explicit Config()
        : file_version(FILE_VERSION)
        , bbs_name_sysop("New OBV2 XRM Sysop")
        , bbs_name("New OBV2 XRM BBS")
        , bbs_uuid("")
        , password_system("system")
        , password_sysop("sysop")
        , password_newuser("newuser")
        , port_telnet(6023)
        , port_ssl(443)
        , use_service_telnet(true)
        , use_service_ssl(false)
        , directory_screens("")
        , directory_boards("")
        , directory_files("")
        , directory_data("")
        , directory_menu("")
        , directory_work("")
        , directory_text_files("")
        , directory_file_mail("")
        , directory_doors("")
        , directory_log("")
        , directory_scripts("")
        , directory_prompts("")
        , points_per_megabyte(0)
        , points_earned_postcall(0)
        , access_sysop("s255")
        , access_message_sysop("s200")
        , access_file_sysop("s200")
        , access_hidden_files("s200")
        , access_post_anonymous("s20")
        , access_mail_attachment("s20")
        , access_top_ten("s20")
        , access_check_sysop_avail("s20")
        , invalid_password_attempts(3)
        , invalid_newuser_password_attempts(3)
        , use_file_points(false)
        , use_postcall_ratio(false)
        , use_library_ansi(true)
        , use_notify_on_logins(true)
        , use_notify_on_logoff(true)
        , use_log_chat_sessions(true)
        , use_screen_prelogin(false)
        , use_screen_welcome(false)
        , use_matrix_login(true)
        , use_newuser_password(true)
        , use_disclaimer(true)
        , use_address(true)
        , use_handle(true)
        , use_real_name(true)
        , use_location(true)
        , use_country(true)
        , use_email(true)
        , use_user_note(true)
        , use_birthdate(true)
        , use_gender(true)
        , use_challenge_question(true)
        , use_yesno_bars(true)
        , use_pause(true)
        , use_clear_screen(true)
        , use_ansi_color(true)
        , use_backspace(true)        
        , hidden_input_character('*')        
        , use_auto_validate_users(true)
        , use_newuser_voting(false)
        , use_auto_kick_unvalidated(false)        
        , newuser_votes_validate(3)
        , newuser_votes_delete(3)
        , newuser_days_to_upload(7)
        , days_keep_logs(30)                
        , qwk_packet_name("Obv2_XRM")
        , starting_menu_name("main")
        , use_message_attachments(false)
        , days_keep_attachments(30)
        , default_color_regular("|15")
        , default_color_stat("|03")
        , default_color_prompt("|11")
        , default_color_input("|11")
        , default_color_inverse("|19")
        , default_color_box("|08")  
        , default_user_flags("")
        , flag_change_daily("")
        , flag_change_session("")
        , default_level(20)
        , default_file_level(20)
        , default_message_level(20)
        , default_file_points(0)
        , default_file_ratio(0)
        , default_megabyte_ratio(0)
        , default_megabyte_daily(0)
        , default_post_call_ratio(0)
        , default_time_limit(120)
        , default_user_timeout(20)        
        , use_auto_validate_files(false)
        , use_upload_checker(false) // default to true lateron!        
        , cmdline_virus_scan("")
        , filename_bbs_ad("")
        , filename_archive_comments("")
        , directory_bad_files("")        
        , use_greater_then_for_quotes(false)        
        , regexp_generic_validation("[^\\s][\\A\\w\\s,.!@#$%^&*()]+")  // testing no starting spaces!
        , regexp_generic_validation_msg("At least one AlphaNumeric Word characters required space seperators.")        
        , regexp_handle_validation("(?=.*[a-zA-Z])(?!.*\\d).{2,}")
        , regexp_handle_validation_msg("At least two characters case insensitive no spaces.")        
        , regexp_password_validation("(?=.*[a-z])(?=.*[A-Z])(?=.*\\d).{8,}")
        , regexp_password_validation_msg("At least one lower, one upper, one digit and minimum of 8.")        
        , regexp_date_validation("(19|20)\\d\\d([- /.])(0[1-9]|1[012])\\2(0[1-9]|[12][0-9]|3[01])")
        , regexp_date_validation_msg("Must be a valid year matching UTC Format yyyy-mm-dd")        
        , regexp_email_validation("[\\w.]+[@]{1}[\\w]+[.]*[\\w]*")
        , regexp_email_validation_msg("Must be a valid email at the very lest name@domain")
    {
        // Generates an Initial Unique Board UUID when the configuration is created.
        // If someone wipes out their config, they should save this and re-enter it!
        boost::uuids::random_generator generator;
        boost::uuids::uuid uuid = generator();
        bbs_uuid = boost::lexical_cast<std::string>(uuid);
    }

    ~Config() { }

};


// YAML Overide namespace for encoding/decoding Config Class.
namespace YAML
{
    template<>
    struct convert<Config>
    {
        /**
         * @brief Overide for encoding the Config Class
         *        This is not used as it makes the map on a single line!
         *        Instead Key/Value is done in the ConfigDao on save.
         * @param rhs
         * @return
         */
        static Node encode(const Config &rhs)
        {
            Node node;
            node["file_version"] = rhs.file_version;
            node["bbs_name_sysop"] = rhs.bbs_name_sysop;
            node["bbs_name"] = rhs.bbs_name;
            node["bbs_uuid"] = rhs.bbs_uuid;
            node["password_system"] = rhs.password_system;
            node["password_sysop"] = rhs.password_sysop;
            node["password_newuser"] = rhs.password_newuser;
            node["port_telnet"] = rhs.port_telnet;
            node["port_ssl"] = rhs.port_ssl;
            node["use_service_telnet"] = rhs.use_service_telnet;
            node["use_service_ssl"] = rhs.use_service_ssl;
            node["directory_screens"] = rhs.directory_screens;
            node["directory_boards"] = rhs.directory_boards;
            node["directory_files"] = rhs.directory_files;
            node["directory_data"] = rhs.directory_data;
            node["directory_menu"] = rhs.directory_menu;
            node["directory_work"] = rhs.directory_work;
            node["directory_text_files"] = rhs.directory_text_files;
            node["directory_file_mail"] = rhs.directory_file_mail;
            node["directory_doors"] = rhs.directory_doors;
            node["directory_log"] = rhs.directory_log;
            node["directory_scripts"] = rhs.directory_scripts;
            node["directory_prompts"] = rhs.directory_prompts;
            node["points_per_megabyte"] = rhs.points_per_megabyte;
            node["points_earned_postcall"] = rhs.points_earned_postcall;
            node["access_post_anonymous"] = rhs.access_post_anonymous;
            node["access_sysop"] = rhs.access_sysop;
            node["access_message_sysop"] = rhs.access_message_sysop;
            node["access_file_sysop"] = rhs.access_file_sysop;
            node["access_hidden_files"] = rhs.access_hidden_files;
            node["access_mail_attachment"] = rhs.access_mail_attachment;
            node["access_top_ten"] = rhs.access_top_ten;
            node["access_check_sysop_avail"] = rhs.access_check_sysop_avail;
            node["invalid_password_attempts"] = rhs.invalid_password_attempts;
            node["invalid_newuser_password_attempts"] = rhs.invalid_newuser_password_attempts;
            node["use_file_points"] = rhs.use_file_points;
            node["use_postcall_ratio"] = rhs.use_postcall_ratio;
            node["use_library_ansi"] = rhs.use_library_ansi;
            node["use_notify_on_logins"] = rhs.use_notify_on_logins;
            node["use_notify_on_logoff"] = rhs.use_notify_on_logoff;
            node["use_log_chat_sessions"] = rhs.use_log_chat_sessions;
            node["use_screen_prelogin"] = rhs.use_screen_prelogin;
            node["use_screen_welcome"] = rhs.use_screen_welcome;
            node["use_matrix_login"] = rhs.use_matrix_login;
            node["use_newuser_password"] = rhs.use_newuser_password;
            node["use_disclaimer"] = rhs.use_disclaimer;                        
            node["use_handle"] = rhs.use_handle;
            node["use_real_name"] = rhs.use_real_name;
            node["use_location"] = rhs.use_location;
            node["use_country"] = rhs.use_country;
            node["use_email"] = rhs.use_email;
            node["use_user_note"] = rhs.use_user_note;
            node["use_birthdate"] = rhs.use_birthdate;
            node["use_gender"] = rhs.use_gender;
            node["use_challenge_question"] = rhs.use_challenge_question;
            node["use_yesno_bars"] = rhs.use_yesno_bars;
            node["use_pause"] = rhs.use_pause;
            node["use_clear_screen"] = rhs.use_clear_screen;
            node["use_ansi_color"] = rhs.use_ansi_color;
            node["use_backspace"] = rhs.use_backspace;                        
            node["hidden_input_character"] = rhs.hidden_input_character;
            node["use_auto_validate_users"] = rhs.use_auto_validate_users;
            node["use_newuser_voting"] = rhs.use_newuser_voting;
            node["use_auto_kick_unvalidated"] = rhs.use_auto_kick_unvalidated;
            node["newuser_votes_validate"] = rhs.newuser_votes_validate;
            node["newuser_votes_delete"] = rhs.newuser_votes_delete;
            node["newuser_days_to_upload"] = rhs.newuser_days_to_upload;
            node["days_keep_logs"] = rhs.days_keep_logs;
            node["qwk_packet_name"] = rhs.qwk_packet_name;
            node["starting_menu_name"] = rhs.starting_menu_name;
            node["use_message_attachments"] = rhs.use_message_attachments;
            node["days_keep_attachments"] = rhs.days_keep_attachments;
            node["default_color_regular"] = rhs.default_color_regular;
            node["default_color_stat"] = rhs.default_color_stat;
            node["default_color_prompt"] = rhs.default_color_prompt;
            node["default_color_input"] = rhs.default_color_input;
            node["default_color_inverse"] = rhs.default_color_inverse;
            node["default_color_box"] = rhs.default_color_box;
            node["default_user_flags"] = rhs.default_user_flags;
            node["flag_change_daily"] = rhs.flag_change_daily;
            node["flag_change_session"] = rhs.flag_change_session;
            node["default_level"] = rhs.default_level;
            node["default_file_level"] = rhs.default_file_level;
            node["default_message_level"] = rhs.default_message_level;
            node["default_file_points"] = rhs.default_file_points;
            node["default_file_ratio"] = rhs.default_file_ratio;
            node["default_megabyte_ratio"] = rhs.default_megabyte_ratio;
            node["default_megabyte_daily"] = rhs.default_megabyte_daily;
            node["default_post_call_ratio"] = rhs.default_post_call_ratio;
            node["default_time_limit"] = rhs.default_time_limit;
            node["default_user_timeout"] = rhs.default_user_timeout;
            node["use_auto_validate_files"] = rhs.use_auto_validate_files;
            node["use_upload_checker"] = rhs.use_upload_checker;
            node["cmdline_virus_scan"] = rhs.cmdline_virus_scan;
            node["filename_bbs_ad"] = rhs.filename_bbs_ad;
            node["filename_archive_comments"] = rhs.filename_archive_comments;
            node["directory_bad_files"] = rhs.directory_bad_files;
            node["use_greater_then_for_quotes"] = rhs.use_greater_then_for_quotes;
            node["regexp_generic_validation"] = rhs.regexp_generic_validation;
            node["regexp_handle_validation"] = rhs.regexp_handle_validation;
            node["regexp_password_validation"] = rhs.regexp_password_validation;
            node["regexp_date_validation"] = rhs.regexp_date_validation;
            node["regexp_email_validation"] = rhs.regexp_email_validation;            
            node["regexp_generic_validation_msg"] = rhs.regexp_generic_validation_msg;
            node["regexp_handle_validation_msg"] = rhs.regexp_handle_validation_msg;
            node["regexp_password_validation_msg"] = rhs.regexp_password_validation_msg;
            node["regexp_date_validation_msg"] = rhs.regexp_date_validation_msg;
            node["regexp_email_validation_msg"] = rhs.regexp_email_validation_msg;

            return node;
        }

        /**
         * @brief Overide for the Load from File, this read and piluaes the Config Object.
         * @param node
         * @param rhs
         * @return
         */
        static bool decode(const Node& node, Config& rhs)
        {
            rhs.file_version                    = node["file_version"].as<std::string>();
            rhs.bbs_name_sysop                  = node["bbs_name_sysop"].as<std::string>();
            rhs.bbs_name                        = node["bbs_name"].as<std::string>();
            rhs.bbs_uuid                        = node["bbs_uuid"].as<std::string>();
            rhs.password_system                 = node["password_system"].as<std::string>();
            rhs.password_sysop                  = node["password_sysop"].as<std::string>();
            rhs.password_newuser                = node["password_newuser"].as<std::string>();
            rhs.port_telnet                     = node["port_telnet"].as<int>();
            rhs.port_ssl                        = node["port_ssl"].as<int>();
            rhs.use_service_telnet              = node["use_service_telnet"].as<bool>();
            rhs.use_service_ssl                 = node["use_service_ssl"].as<bool>();
            rhs.directory_screens               = node["directory_screens"].as<std::string>();
            rhs.directory_boards                = node["directory_boards"].as<std::string>();
            rhs.directory_files                 = node["directory_files"].as<std::string>();
            rhs.directory_data                  = node["directory_data"].as<std::string>();
            rhs.directory_menu                  = node["directory_menu"].as<std::string>();
            rhs.directory_work                  = node["directory_work"].as<std::string>();
            rhs.directory_text_files            = node["directory_text_files"].as<std::string>();
            rhs.directory_file_mail             = node["directory_file_mail"].as<std::string>();
            rhs.directory_doors                 = node["directory_doors"].as<std::string>();
            rhs.directory_log                   = node["directory_log"].as<std::string>();
            rhs.directory_scripts               = node["directory_scripts"].as<std::string>();
            rhs.directory_prompts               = node["directory_prompts"].as<std::string>();
            rhs.points_per_megabyte             = node["points_per_megabyte"].as<int>();
            rhs.points_earned_postcall          = node["points_earned_postcall"].as<int>();
            rhs.access_sysop                    = node["access_sysop"].as<std::string>();
            rhs.access_message_sysop            = node["access_message_sysop"].as<std::string>();
            rhs.access_file_sysop               = node["access_file_sysop"].as<std::string>();
            rhs.access_hidden_files             = node["access_hidden_files"].as<std::string>();
            rhs.access_post_anonymous           = node["access_post_anonymous"].as<std::string>();
            rhs.access_mail_attachment          = node["access_mail_attachment"].as<std::string>();
            rhs.access_top_ten                  = node["access_top_ten"].as<std::string>();
            rhs.access_check_sysop_avail        = node["access_check_sysop_avail"].as<std::string>();            
            rhs.invalid_password_attempts       = node["invalid_password_attempts"].as<int>();
            rhs.invalid_newuser_password_attempts = node["invalid_newuser_password_attempts"].as<int>();
            rhs.use_file_points                 = node["use_file_points"].as<bool>();
            rhs.use_postcall_ratio              = node["use_postcall_ratio"].as<bool>();
            rhs.use_library_ansi                = node["use_library_ansi"].as<bool>();
            rhs.use_notify_on_logins            = node["use_notify_on_logins"].as<bool>();
            rhs.use_notify_on_logoff            = node["use_notify_on_logoff"].as<bool>();
            rhs.use_log_chat_sessions           = node["use_log_chat_sessions"].as<bool>();
            rhs.use_screen_prelogin             = node["use_screen_prelogin"].as<bool>();
            rhs.use_screen_welcome              = node["use_screen_welcome"].as<bool>();
            rhs.use_matrix_login                = node["use_matrix_login"].as<bool>();
            rhs.use_newuser_password            = node["use_newuser_password"].as<bool>();
            rhs.use_disclaimer                  = node["use_disclaimer"].as<bool>();            
            rhs.use_handle                      = node["use_handle"].as<bool>();
            rhs.use_real_name                   = node["use_real_name"].as<bool>();
            rhs.use_location                    = node["use_location"].as<bool>();
            rhs.use_country                     = node["use_country"].as<bool>();
            rhs.use_email                       = node["use_email"].as<bool>();
            rhs.use_user_note                   = node["use_user_note"].as<bool>();
            rhs.use_birthdate                   = node["use_birthdate"].as<bool>();
            rhs.use_gender                      = node["use_gender"].as<bool>();
            rhs.use_challenge_question          = node["use_challenge_question"].as<bool>();
            rhs.use_yesno_bars                  = node["use_yesno_bars"].as<bool>();
            rhs.use_pause                       = node["use_pause"].as<bool>();
            rhs.use_clear_screen                = node["use_clear_screen"].as<bool>();
            rhs.use_ansi_color                  = node["use_ansi_color"].as<bool>();
            rhs.use_backspace                   = node["use_backspace"].as<bool>();
            rhs.hidden_input_character          = node["hidden_input_character"].as<unsigned char>();
            rhs.use_auto_validate_users         = node["use_auto_validate_users"].as<bool>();
            rhs.use_newuser_voting              = node["use_newuser_voting"].as<bool>();
            rhs.use_auto_kick_unvalidated       = node["use_auto_kick_unvalidated"].as<bool>();
            rhs.newuser_votes_validate          = node["newuser_votes_validate"].as<int>();
            rhs.newuser_votes_delete            = node["newuser_votes_delete"].as<int>();
            rhs.newuser_days_to_upload          = node["newuser_days_to_upload"].as<int>();
            rhs.days_keep_logs                  = node["days_keep_logs"].as<int>();
            rhs.qwk_packet_name                 = node["qwk_packet_name"].as<std::string>();
            rhs.starting_menu_name              = node["starting_menu_name"].as<std::string>();
            rhs.use_message_attachments         = node["use_message_attachments"].as<bool>();
            rhs.days_keep_attachments           = node["days_keep_attachments"].as<int>();
            rhs.default_color_regular           = node["default_color_regular"].as<std::string>();
            rhs.default_color_stat              = node["default_color_stat"].as<std::string>();
            rhs.default_color_prompt            = node["default_color_prompt"].as<std::string>();
            rhs.default_color_input             = node["default_color_input"].as<std::string>();
            rhs.default_color_inverse           = node["default_color_inverse"].as<std::string>();
            rhs.default_color_box               = node["default_color_box"].as<std::string>();
            rhs.default_user_flags              = node["default_user_flags"].as<std::string>();
            rhs.flag_change_daily               = node["flag_change_daily"].as<std::string>();
            rhs.flag_change_session             = node["flag_change_session"].as<std::string>();
            rhs.default_level                   = node["default_level"].as<int>();
            rhs.default_file_level              = node["default_file_level"].as<int>();
            rhs.default_message_level           = node["default_message_level"].as<int>();
            rhs.default_file_points             = node["default_file_points"].as<int>();
            rhs.default_file_ratio              = node["default_file_ratio"].as<int>();
            rhs.default_megabyte_ratio          = node["default_megabyte_ratio"].as<int>();
            rhs.default_megabyte_daily          = node["default_megabyte_daily"].as<int>();
            rhs.default_post_call_ratio         = node["default_post_call_ratio"].as<int>();
            rhs.default_time_limit              = node["default_time_limit"].as<int>();
            rhs.default_user_timeout            = node["default_user_timeout"].as<int>();
            rhs.use_auto_validate_files         = node["use_auto_validate_files"].as<bool>();
            rhs.use_upload_checker              = node["use_upload_checker"].as<bool>();
            rhs.cmdline_virus_scan              = node["cmdline_virus_scan"].as<std::string>();
            rhs.filename_bbs_ad                 = node["filename_bbs_ad"].as<std::string>();
            rhs.filename_archive_comments       = node["filename_archive_comments"].as<std::string>();
            rhs.directory_bad_files             = node["directory_bad_files"].as<std::string>();
            rhs.use_greater_then_for_quotes     = node["use_greater_then_for_quotes"].as<bool>();
            rhs.regexp_generic_validation       = node["regexp_generic_validation"].as<std::string>();
            rhs.regexp_handle_validation        = node["regexp_handle_validation"].as<std::string>();
            rhs.regexp_password_validation      = node["regexp_password_validation"].as<std::string>();
            rhs.regexp_date_validation          = node["regexp_date_validation"].as<std::string>();
            rhs.regexp_email_validation         = node["regexp_email_validation"].as<std::string>();            
            rhs.regexp_generic_validation_msg   = node["regexp_generic_validation_msg"].as<std::string>();
            rhs.regexp_handle_validation_msg    = node["regexp_handle_validation_msg"].as<std::string>();
            rhs.regexp_password_validation_msg  = node["regexp_password_validation_msg"].as<std::string>();
            rhs.regexp_date_validation_msg      = node["regexp_date_validation_msg"].as<std::string>();
            rhs.regexp_email_validation_msg     = node["regexp_email_validation_msg"].as<std::string>();

            return true;
        }
    };
}

typedef boost::shared_ptr<Config> config_ptr;
typedef boost::weak_ptr<Config> config_wptr;

#endif // CONFIG_HPP
