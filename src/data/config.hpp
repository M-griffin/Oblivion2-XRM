#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <boost/config.hpp>
#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/smart_ptr/weak_ptr.hpp>
#include <boost/lexical_cast.hpp>

#include <boost/uuid/uuid.hpp>            // uuid class
#include <boost/uuid/uuid_generators.hpp> // generators
#include <boost/uuid/uuid_io.hpp>         // streaming operators etc.

#include <boost/serialization/serialization.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/shared_ptr.hpp>
#include <boost/serialization/level.hpp>
#include <boost/serialization/tracking.hpp>

#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>
#include <fstream>


/**
 * @class Config Object
 * @author Michael Griffin
 * @date 2/21/2016
 * @file config.hpp
 * @brief Config Model - To output serilized data to XML.
 */
class Config
{

protected:
    friend class boost::serialization::access;

    template<class Archive>
    void serialize(
        Archive & ar,
        const unsigned int // version
    )
    {
        ar & BOOST_SERIALIZATION_NVP(bbs_name_sysop);
        ar & BOOST_SERIALIZATION_NVP(bbs_name);
        ar & BOOST_SERIALIZATION_NVP(bbs_uuid);
        ar & BOOST_SERIALIZATION_NVP(password_system);
        ar & BOOST_SERIALIZATION_NVP(password_sysop);
        ar & BOOST_SERIALIZATION_NVP(password_newuser);
        ar & BOOST_SERIALIZATION_NVP(port_telnet);
        ar & BOOST_SERIALIZATION_NVP(port_ssl);
        ar & BOOST_SERIALIZATION_NVP(use_service_telnet);
        ar & BOOST_SERIALIZATION_NVP(use_service_ssl);
        ar & BOOST_SERIALIZATION_NVP(directory_bbs);
        ar & BOOST_SERIALIZATION_NVP(directory_screens);
        ar & BOOST_SERIALIZATION_NVP(directory_boards);
        ar & BOOST_SERIALIZATION_NVP(directory_files);
        ar & BOOST_SERIALIZATION_NVP(directory_data);
        ar & BOOST_SERIALIZATION_NVP(directory_menu);
        ar & BOOST_SERIALIZATION_NVP(directory_work);
        ar & BOOST_SERIALIZATION_NVP(directory_text_files);
        ar & BOOST_SERIALIZATION_NVP(directory_file_mail);
        ar & BOOST_SERIALIZATION_NVP(directory_doors);
        ar & BOOST_SERIALIZATION_NVP(directory_log);
        ar & BOOST_SERIALIZATION_NVP(directory_scripts);
        ar & BOOST_SERIALIZATION_NVP(directory_prompts);
        ar & BOOST_SERIALIZATION_NVP(points_per_kilobyte);
        ar & BOOST_SERIALIZATION_NVP(points_earned_postcall);
        ar & BOOST_SERIALIZATION_NVP(access_post_anonymous);
        ar & BOOST_SERIALIZATION_NVP(access_sysop);
        ar & BOOST_SERIALIZATION_NVP(access_message_sysop);
        ar & BOOST_SERIALIZATION_NVP(access_file_sysop);
        ar & BOOST_SERIALIZATION_NVP(access_hidden_files);
        ar & BOOST_SERIALIZATION_NVP(access_mail_attachment);
        ar & BOOST_SERIALIZATION_NVP(access_top_ten);
        ar & BOOST_SERIALIZATION_NVP(access_check_sysop_avail);
        ar & BOOST_SERIALIZATION_NVP(use_file_points);
        ar & BOOST_SERIALIZATION_NVP(use_postcall_ratio);
        ar & BOOST_SERIALIZATION_NVP(use_library_ansi);
        ar & BOOST_SERIALIZATION_NVP(use_notify_on_logins);
        ar & BOOST_SERIALIZATION_NVP(use_notify_on_logoff);
        ar & BOOST_SERIALIZATION_NVP(use_log_chat_sessions);
        ar & BOOST_SERIALIZATION_NVP(use_screen_prelogin);
        ar & BOOST_SERIALIZATION_NVP(use_screen_welcome);
        ar & BOOST_SERIALIZATION_NVP(use_matrix_login);
        ar & BOOST_SERIALIZATION_NVP(hidden_input_character);
        ar & BOOST_SERIALIZATION_NVP(use_auto_validate_users);
        ar & BOOST_SERIALIZATION_NVP(use_newuser_voting);
        ar & BOOST_SERIALIZATION_NVP(use_auto_kick_unvalidated);
        ar & BOOST_SERIALIZATION_NVP(newuser_votes_validate);
        ar & BOOST_SERIALIZATION_NVP(newuser_votes_delete);
        ar & BOOST_SERIALIZATION_NVP(newuser_days_to_upload);
        ar & BOOST_SERIALIZATION_NVP(days_keep_logs);
        ar & BOOST_SERIALIZATION_NVP(qwk_packet_name);
        ar & BOOST_SERIALIZATION_NVP(starting_menu_name);
        ar & BOOST_SERIALIZATION_NVP(use_message_attachments);
        ar & BOOST_SERIALIZATION_NVP(days_keep_attachments);
        ar & BOOST_SERIALIZATION_NVP(default_color_regular);
        ar & BOOST_SERIALIZATION_NVP(default_color_stat);
        ar & BOOST_SERIALIZATION_NVP(default_color_propmpt);
        ar & BOOST_SERIALIZATION_NVP(default_color_input);
        ar & BOOST_SERIALIZATION_NVP(default_color_inverse);
        ar & BOOST_SERIALIZATION_NVP(default_color_box);
        ar & BOOST_SERIALIZATION_NVP(default_user_flags);
        ar & BOOST_SERIALIZATION_NVP(flag_change_daily);
        ar & BOOST_SERIALIZATION_NVP(flag_change_session);
        ar & BOOST_SERIALIZATION_NVP(default_level);
        ar & BOOST_SERIALIZATION_NVP(default_file_level);
        ar & BOOST_SERIALIZATION_NVP(default_message_level);
        ar & BOOST_SERIALIZATION_NVP(default_file_points);
        ar & BOOST_SERIALIZATION_NVP(default_file_ratio);
        ar & BOOST_SERIALIZATION_NVP(default_kilobyte_ratio);
        ar & BOOST_SERIALIZATION_NVP(default_kilobyte_daily);
        ar & BOOST_SERIALIZATION_NVP(default_post_call_ratio);
        ar & BOOST_SERIALIZATION_NVP(default_time_limit);
        ar & BOOST_SERIALIZATION_NVP(default_user_timeout);
        ar & BOOST_SERIALIZATION_NVP(use_auto_validate_files);
        ar & BOOST_SERIALIZATION_NVP(use_upload_checker);
        ar & BOOST_SERIALIZATION_NVP(cmdline_virus_scan);
        ar & BOOST_SERIALIZATION_NVP(filename_bbs_ad);
        ar & BOOST_SERIALIZATION_NVP(filename_archive_comments);
        ar & BOOST_SERIALIZATION_NVP(directory_bad_files);
        ar & BOOST_SERIALIZATION_NVP(use_greater_then_for_quotes);
    }

    template<class Archive, class T>
    void serialize(
        Archive & ar,
        T &t,
        const unsigned int //version
    )
    {
        ar & BOOST_SERIALIZATION_NVP(t.bbs_name_sysop);
        ar & BOOST_SERIALIZATION_NVP(t.bbs_name);
        ar & BOOST_SERIALIZATION_NVP(t.bbs_uuid);
        ar & BOOST_SERIALIZATION_NVP(t.password_system);
        ar & BOOST_SERIALIZATION_NVP(t.password_sysop);
        ar & BOOST_SERIALIZATION_NVP(t.password_newuser);
        ar & BOOST_SERIALIZATION_NVP(t.port_telnet);
        ar & BOOST_SERIALIZATION_NVP(t.port_ssl);
        ar & BOOST_SERIALIZATION_NVP(t.use_service_telnet);
        ar & BOOST_SERIALIZATION_NVP(t.use_service_ssl);
        ar & BOOST_SERIALIZATION_NVP(t.directory_bbs);
        ar & BOOST_SERIALIZATION_NVP(t.directory_screens);
        ar & BOOST_SERIALIZATION_NVP(t.directory_boards);
        ar & BOOST_SERIALIZATION_NVP(t.directory_files);
        ar & BOOST_SERIALIZATION_NVP(t.directory_data);
        ar & BOOST_SERIALIZATION_NVP(t.directory_menu);
        ar & BOOST_SERIALIZATION_NVP(t.directory_work);
        ar & BOOST_SERIALIZATION_NVP(t.directory_text_files);
        ar & BOOST_SERIALIZATION_NVP(t.directory_file_mail);
        ar & BOOST_SERIALIZATION_NVP(t.directory_doors);
        ar & BOOST_SERIALIZATION_NVP(t.directory_log);
        ar & BOOST_SERIALIZATION_NVP(t.directory_scripts);
        ar & BOOST_SERIALIZATION_NVP(t.directory_prompts);
        ar & BOOST_SERIALIZATION_NVP(t.points_per_kilobyte);
        ar & BOOST_SERIALIZATION_NVP(t.points_earned_postcall);
        ar & BOOST_SERIALIZATION_NVP(t.access_post_anonymous);
        ar & BOOST_SERIALIZATION_NVP(t.access_sysop);
        ar & BOOST_SERIALIZATION_NVP(t.access_message_sysop);
        ar & BOOST_SERIALIZATION_NVP(t.access_file_sysop);
        ar & BOOST_SERIALIZATION_NVP(t.access_hidden_files);
        ar & BOOST_SERIALIZATION_NVP(t.access_mail_attachment);
        ar & BOOST_SERIALIZATION_NVP(t.access_top_ten);
        ar & BOOST_SERIALIZATION_NVP(t.access_check_sysop_avail);
        ar & BOOST_SERIALIZATION_NVP(t.use_file_points);
        ar & BOOST_SERIALIZATION_NVP(t.use_postcall_ratio);
        ar & BOOST_SERIALIZATION_NVP(t.use_library_ansi);
        ar & BOOST_SERIALIZATION_NVP(t.use_notify_on_logins);
        ar & BOOST_SERIALIZATION_NVP(t.use_notify_on_logoff);
        ar & BOOST_SERIALIZATION_NVP(t.use_log_chat_sessions);
        ar & BOOST_SERIALIZATION_NVP(t.use_screen_prelogin);
        ar & BOOST_SERIALIZATION_NVP(t.use_screen_welcome);
        ar & BOOST_SERIALIZATION_NVP(t.use_matrix_login);
        ar & BOOST_SERIALIZATION_NVP(t.hidden_input_character);
        ar & BOOST_SERIALIZATION_NVP(t.use_auto_validate_users);
        ar & BOOST_SERIALIZATION_NVP(t.use_newuser_voting);
        ar & BOOST_SERIALIZATION_NVP(t.use_auto_kick_unvalidated);
        ar & BOOST_SERIALIZATION_NVP(t.newuser_votes_validate);
        ar & BOOST_SERIALIZATION_NVP(t.newuser_votes_delete);
        ar & BOOST_SERIALIZATION_NVP(t.newuser_days_to_upload);
        ar & BOOST_SERIALIZATION_NVP(t.days_keep_logs);
        ar & BOOST_SERIALIZATION_NVP(t.qwk_packet_name);
        ar & BOOST_SERIALIZATION_NVP(t.starting_menu_name);
        ar & BOOST_SERIALIZATION_NVP(t.use_message_attachments);
        ar & BOOST_SERIALIZATION_NVP(t.days_keep_attachments);
        ar & BOOST_SERIALIZATION_NVP(t.default_color_regular);
        ar & BOOST_SERIALIZATION_NVP(t.default_color_stat);
        ar & BOOST_SERIALIZATION_NVP(t.default_color_propmpt);
        ar & BOOST_SERIALIZATION_NVP(t.default_color_input);
        ar & BOOST_SERIALIZATION_NVP(t.default_color_inverse);
        ar & BOOST_SERIALIZATION_NVP(t.default_color_box);
        ar & BOOST_SERIALIZATION_NVP(t.default_user_flags);
        ar & BOOST_SERIALIZATION_NVP(t.flag_change_daily);
        ar & BOOST_SERIALIZATION_NVP(t.flag_change_session);
        ar & BOOST_SERIALIZATION_NVP(t.default_level);
        ar & BOOST_SERIALIZATION_NVP(t.default_file_level);
        ar & BOOST_SERIALIZATION_NVP(t.default_message_level);
        ar & BOOST_SERIALIZATION_NVP(t.default_file_points);
        ar & BOOST_SERIALIZATION_NVP(t.default_file_ratio);
        ar & BOOST_SERIALIZATION_NVP(t.default_kilobyte_ratio);
        ar & BOOST_SERIALIZATION_NVP(t.default_kilobyte_daily);
        ar & BOOST_SERIALIZATION_NVP(t.default_post_call_ratio);
        ar & BOOST_SERIALIZATION_NVP(t.default_time_limit);
        ar & BOOST_SERIALIZATION_NVP(t.default_user_timeout);
        ar & BOOST_SERIALIZATION_NVP(t.use_auto_validate_files);
        ar & BOOST_SERIALIZATION_NVP(t.use_upload_checker);
        ar & BOOST_SERIALIZATION_NVP(t.cmdline_virus_scan);
        ar & BOOST_SERIALIZATION_NVP(t.filename_bbs_ad);
        ar & BOOST_SERIALIZATION_NVP(t.filename_archive_comments);
        ar & BOOST_SERIALIZATION_NVP(t.directory_bad_files);
        ar & BOOST_SERIALIZATION_NVP(t.use_greater_then_for_quotes);
    }

public:

    void SetAccessCheckSysopAvail(const std::string& access_check_sysop_avail)
    {
        this->access_check_sysop_avail = access_check_sysop_avail;
    }
    void SetAccessFileSysop(const std::string& access_file_sysop)
    {
        this->access_file_sysop = access_file_sysop;
    }
    void SetAccessHiddenFiles(const std::string& access_hidden_files)
    {
        this->access_hidden_files = access_hidden_files;
    }
    void SetAccessMailAttachment(const std::string& access_mail_attachment)
    {
        this->access_mail_attachment = access_mail_attachment;
    }
    void SetAccessMessageSysop(const std::string& access_message_sysop)
    {
        this->access_message_sysop = access_message_sysop;
    }
    void SetAccessPostAnonymous(const std::string& access_post_anonymous)
    {
        this->access_post_anonymous = access_post_anonymous;
    }
    void SetAccessSysop(const std::string& access_sysop)
    {
        this->access_sysop = access_sysop;
    }
    void SetAccessTopTen(const std::string& access_top_ten)
    {
        this->access_top_ten = access_top_ten;
    }
    void SetBbsName(const std::string& bbs_name)
    {
        this->bbs_name = bbs_name;
    }
    void SetBbsNameSysop(const std::string& bbs_name_sysop)
    {
        this->bbs_name_sysop = bbs_name_sysop;
    }
    void SetBbsUuid(const std::string& bbs_uuid)
    {
        this->bbs_uuid = bbs_uuid;
    }
    void SetCmdlineVirusScan(const std::string& cmdline_virus_scan)
    {
        this->cmdline_virus_scan = cmdline_virus_scan;
    }
    void SetDaysKeepAttachments(int days_keep_attachments)
    {
        this->days_keep_attachments = days_keep_attachments;
    }
    void SetDaysKeepLogs(int days_keep_logs)
    {
        this->days_keep_logs = days_keep_logs;
    }
    void SetDefaultColorBox(const std::string& default_color_box)
    {
        this->default_color_box = default_color_box;
    }
    void SetDefaultColorInput(const std::string& default_color_input)
    {
        this->default_color_input = default_color_input;
    }
    void SetDefaultColorInverse(const std::string& default_color_inverse)
    {
        this->default_color_inverse = default_color_inverse;
    }
    void SetDefaultColorPropmpt(const std::string& default_color_propmpt)
    {
        this->default_color_propmpt = default_color_propmpt;
    }
    void SetDefaultColorRegular(const std::string& default_color_regular)
    {
        this->default_color_regular = default_color_regular;
    }
    void SetDefaultColorStat(const std::string& default_color_stat)
    {
        this->default_color_stat = default_color_stat;
    }
    void SetDefaultFileLevel(int default_file_level)
    {
        this->default_file_level = default_file_level;
    }
    void SetDefaultFilePoints(int default_file_points)
    {
        this->default_file_points = default_file_points;
    }
    void SetDefaultFileRatio(int default_file_ratio)
    {
        this->default_file_ratio = default_file_ratio;
    }
    void SetDefaultKilobyteDaily(int default_kilobyte_daily)
    {
        this->default_kilobyte_daily = default_kilobyte_daily;
    }
    void SetDefaultKilobyteRatio(int default_kilobyte_ratio)
    {
        this->default_kilobyte_ratio = default_kilobyte_ratio;
    }
    void SetDefaultLevel(int default_level)
    {
        this->default_level = default_level;
    }
    void SetDefaultMessageLevel(int default_message_level)
    {
        this->default_message_level = default_message_level;
    }
    void SetDefaultPostCallRatio(int default_post_call_ratio)
    {
        this->default_post_call_ratio = default_post_call_ratio;
    }
    void SetDefaultTimeLimit(int default_time_limit)
    {
        this->default_time_limit = default_time_limit;
    }
    void SetDefaultUserFlags(const std::string& default_user_flags)
    {
        this->default_user_flags = default_user_flags;
    }
    void SetDefaultUserTimeout(int default_user_timeout)
    {
        this->default_user_timeout = default_user_timeout;
    }
    void SetDirectoryBadFiles(const std::string& directory_bad_files)
    {
        this->directory_bad_files = directory_bad_files;
    }
    void SetDirectoryBbs(const std::string& directory_bbs)
    {
        this->directory_bbs = directory_bbs;
    }
    void SetDirectoryBoards(const std::string& directory_boards)
    {
        this->directory_boards = directory_boards;
    }
    void SetDirectoryData(const std::string& directory_data)
    {
        this->directory_data = directory_data;
    }
    void SetDirectoryDoors(const std::string& directory_doors)
    {
        this->directory_doors = directory_doors;
    }
    void SetDirectoryFileMail(const std::string& directory_file_mail)
    {
        this->directory_file_mail = directory_file_mail;
    }
    void SetDirectoryFiles(const std::string& directory_files)
    {
        this->directory_files = directory_files;
    }
    void SetDirectoryLog(const std::string& directory_log)
    {
        this->directory_log = directory_log;
    }
    void SetDirectoryMenu(const std::string& directory_menu)
    {
        this->directory_menu = directory_menu;
    }
    void SetDirectoryPrompts(const std::string& directory_prompts)
    {
        this->directory_prompts = directory_prompts;
    }
    void SetDirectoryScreens(const std::string& directory_screens)
    {
        this->directory_screens = directory_screens;
    }
    void SetDirectoryScripts(const std::string& directory_scripts)
    {
        this->directory_scripts = directory_scripts;
    }
    void SetDirectoryTextFiles(const std::string& directory_text_files)
    {
        this->directory_text_files = directory_text_files;
    }
    void SetDirectoryWork(const std::string& directory_work)
    {
        this->directory_work = directory_work;
    }
    void SetFilenameArchiveComments(const std::string& filename_archive_comments)
    {
        this->filename_archive_comments = filename_archive_comments;
    }
    void SetFilenameBbsAd(const std::string& filename_bbs_ad)
    {
        this->filename_bbs_ad = filename_bbs_ad;
    }
    void SetFlagChangeDaily(const std::string& flag_change_daily)
    {
        this->flag_change_daily = flag_change_daily;
    }
    void SetFlagChangeSession(const std::string& flag_change_session)
    {
        this->flag_change_session = flag_change_session;
    }
    void SetHiddenInputCharacter(char hidden_input_character)
    {
        this->hidden_input_character = hidden_input_character;
    }
    void SetNewuserDaysToUpload(int newuser_days_to_upload)
    {
        this->newuser_days_to_upload = newuser_days_to_upload;
    }
    void SetNewuserVotesDelete(int newuser_votes_delete)
    {
        this->newuser_votes_delete = newuser_votes_delete;
    }
    void SetNewuserVotesValidate(int newuser_votes_validate)
    {
        this->newuser_votes_validate = newuser_votes_validate;
    }
    void SetPasswordNewuser(const std::string& password_newuser)
    {
        this->password_newuser = password_newuser;
    }
    void SetPasswordSysop(const std::string& password_sysop)
    {
        this->password_sysop = password_sysop;
    }
    void SetPasswordSystem(const std::string& password_system)
    {
        this->password_system = password_system;
    }
    void SetPointsEarnedPostcall(int points_earned_postcall)
    {
        this->points_earned_postcall = points_earned_postcall;
    }
    void SetPointsPerKilobyte(int points_per_kilobyte)
    {
        this->points_per_kilobyte = points_per_kilobyte;
    }
    void SetPortSsl(int port_ssl)
    {
        this->port_ssl = port_ssl;
    }
    void SetPortTelnet(int port_telnet)
    {
        this->port_telnet = port_telnet;
    }
    void SetQwkPacketName(const std::string& qwk_packet_name)
    {
        this->qwk_packet_name = qwk_packet_name;
    }
    void SetStartingMenuName(const std::string& starting_menu_name)
    {
        this->starting_menu_name = starting_menu_name;
    }
    void SetUseAutoKickUnvalidated(bool use_auto_kick_unvalidated)
    {
        this->use_auto_kick_unvalidated = use_auto_kick_unvalidated;
    }
    void SetUseAutoValidateFiles(bool use_auto_validate_files)
    {
        this->use_auto_validate_files = use_auto_validate_files;
    }
    void SetUseAutoValidateUsers(bool use_auto_validate_users)
    {
        this->use_auto_validate_users = use_auto_validate_users;
    }
    void SetUseFilePoints(bool use_file_points)
    {
        this->use_file_points = use_file_points;
    }
    void SetUseGreaterThenForQuotes(bool use_greater_then_for_quotes)
    {
        this->use_greater_then_for_quotes = use_greater_then_for_quotes;
    }
    void SetUseLibraryAnsi(bool use_library_ansi)
    {
        this->use_library_ansi = use_library_ansi;
    }
    void SetUseLogChatSessions(bool use_log_chat_sessions)
    {
        this->use_log_chat_sessions = use_log_chat_sessions;
    }
    void SetUseMatrixLogin(bool use_matrix_login)
    {
        this->use_matrix_login = use_matrix_login;
    }
    void SetUseMessageAttachments(bool use_message_attachments)
    {
        this->use_message_attachments = use_message_attachments;
    }
    void SetUseNewuserVoting(bool use_newuser_voting)
    {
        this->use_newuser_voting = use_newuser_voting;
    }
    void SetUseNotifyOnLogins(bool use_notify_on_logins)
    {
        this->use_notify_on_logins = use_notify_on_logins;
    }
    void SetUseNotifyOnLogoff(bool use_notify_on_logoff)
    {
        this->use_notify_on_logoff = use_notify_on_logoff;
    }
    void SetUsePostcallRatio(bool use_postcall_ratio)
    {
        this->use_postcall_ratio = use_postcall_ratio;
    }
    void SetUseScreenPrelogin(bool use_screen_prelogin)
    {
        this->use_screen_prelogin = use_screen_prelogin;
    }
    void SetUseScreenWelcome(bool use_screen_welcome)
    {
        this->use_screen_welcome = use_screen_welcome;
    }
    void SetUseServiceSsl(bool use_service_ssl)
    {
        this->use_service_ssl = use_service_ssl;
    }
    void SetUseServiceTelnet(bool use_service_telnet)
    {
        this->use_service_telnet = use_service_telnet;
    }
    void SetUseUploadChecker(bool use_upload_checker)
    {
        this->use_upload_checker = use_upload_checker;
    }
    const std::string& GetAccessCheckSysopAvail() const
    {
        return access_check_sysop_avail;
    }
    const std::string& GetAccessFileSysop() const
    {
        return access_file_sysop;
    }
    const std::string& GetAccessHiddenFiles() const
    {
        return access_hidden_files;
    }
    const std::string& GetAccessMailAttachment() const
    {
        return access_mail_attachment;
    }
    const std::string& GetAccessMessageSysop() const
    {
        return access_message_sysop;
    }
    const std::string& GetAccessPostAnonymous() const
    {
        return access_post_anonymous;
    }
    const std::string& GetAccessSysop() const
    {
        return access_sysop;
    }
    const std::string& GetAccessTopTen() const
    {
        return access_top_ten;
    }
    const std::string& GetBbsName() const
    {
        return bbs_name;
    }
    const std::string& GetBbsNameSysop() const
    {
        return bbs_name_sysop;
    }
    const std::string& GetBbsUuid() const
    {
        return bbs_uuid;
    }
    const std::string& GetCmdlineVirusScan() const
    {
        return cmdline_virus_scan;
    }
    int GetDaysKeepAttachments() const
    {
        return days_keep_attachments;
    }
    int GetDaysKeepLogs() const
    {
        return days_keep_logs;
    }
    const std::string& GetDefaultColorBox() const
    {
        return default_color_box;
    }
    const std::string& GetDefaultColorInput() const
    {
        return default_color_input;
    }
    const std::string& GetDefaultColorInverse() const
    {
        return default_color_inverse;
    }
    const std::string& GetDefaultColorPropmpt() const
    {
        return default_color_propmpt;
    }
    const std::string& GetDefaultColorRegular() const
    {
        return default_color_regular;
    }
    const std::string& GetDefaultColorStat() const
    {
        return default_color_stat;
    }
    int GetDefaultFileLevel() const
    {
        return default_file_level;
    }
    int GetDefaultFilePoints() const
    {
        return default_file_points;
    }
    int GetDefaultFileRatio() const
    {
        return default_file_ratio;
    }
    int GetDefaultKilobyteDaily() const
    {
        return default_kilobyte_daily;
    }
    int GetDefaultKilobyteRatio() const
    {
        return default_kilobyte_ratio;
    }
    int GetDefaultLevel() const
    {
        return default_level;
    }
    int GetDefaultMessageLevel() const
    {
        return default_message_level;
    }
    int GetDefaultPostCallRatio() const
    {
        return default_post_call_ratio;
    }
    int GetDefaultTimeLimit() const
    {
        return default_time_limit;
    }
    const std::string& GetDefaultUserFlags() const
    {
        return default_user_flags;
    }
    int GetDefaultUserTimeout() const
    {
        return default_user_timeout;
    }
    const std::string& GetDirectoryBadFiles() const
    {
        return directory_bad_files;
    }
    const std::string& GetDirectoryBbs() const
    {
        return directory_bbs;
    }
    const std::string& GetDirectoryBoards() const
    {
        return directory_boards;
    }
    const std::string& GetDirectoryData() const
    {
        return directory_data;
    }
    const std::string& GetDirectoryDoors() const
    {
        return directory_doors;
    }
    const std::string& GetDirectoryFileMail() const
    {
        return directory_file_mail;
    }
    const std::string& GetDirectoryFiles() const
    {
        return directory_files;
    }
    const std::string& GetDirectoryLog() const
    {
        return directory_log;
    }
    const std::string& GetDirectoryMenu() const
    {
        return directory_menu;
    }
    const std::string& GetDirectoryPrompts() const
    {
        return directory_prompts;
    }
    const std::string& GetDirectoryScreens() const
    {
        return directory_screens;
    }
    const std::string& GetDirectoryScripts() const
    {
        return directory_scripts;
    }
    const std::string& GetDirectoryTextFiles() const
    {
        return directory_text_files;
    }
    const std::string& GetDirectoryWork() const
    {
        return directory_work;
    }
    const std::string& GetFilenameArchiveComments() const
    {
        return filename_archive_comments;
    }
    const std::string& GetFilenameBbsAd() const
    {
        return filename_bbs_ad;
    }
    const std::string& GetFlagChangeDaily() const
    {
        return flag_change_daily;
    }
    const std::string& GetFlagChangeSession() const
    {
        return flag_change_session;
    }
    char GetHiddenInputCharacter() const
    {
        return hidden_input_character;
    }
    int GetNewuserDaysToUpload() const
    {
        return newuser_days_to_upload;
    }
    int GetNewuserVotesDelete() const
    {
        return newuser_votes_delete;
    }
    int GetNewuserVotesValidate() const
    {
        return newuser_votes_validate;
    }
    const std::string& GetPasswordNewuser() const
    {
        return password_newuser;
    }
    const std::string& GetPasswordSysop() const
    {
        return password_sysop;
    }
    const std::string& GetPasswordSystem() const
    {
        return password_system;
    }
    int GetPointsEarnedPostcall() const
    {
        return points_earned_postcall;
    }
    int GetPointsPerKilobyte() const
    {
        return points_per_kilobyte;
    }
    int GetPortSsl() const
    {
        return port_ssl;
    }
    int GetPortTelnet() const
    {
        return port_telnet;
    }
    const std::string& GetQwkPacketName() const
    {
        return qwk_packet_name;
    }
    const std::string& GetStartingMenuName() const
    {
        return starting_menu_name;
    }
    bool IsUseAutoKickUnvalidated() const
    {
        return use_auto_kick_unvalidated;
    }
    bool IsUseAutoValidateFiles() const
    {
        return use_auto_validate_files;
    }
    bool IsUseAutoValidateUsers() const
    {
        return use_auto_validate_users;
    }
    bool IsUseFilePoints() const
    {
        return use_file_points;
    }
    bool IsUseGreaterThenForQuotes() const
    {
        return use_greater_then_for_quotes;
    }
    bool IsUseLibraryAnsi() const
    {
        return use_library_ansi;
    }
    bool IsUseLogChatSessions() const
    {
        return use_log_chat_sessions;
    }
    bool IsUseMatrixLogin() const
    {
        return use_matrix_login;
    }
    bool IsUseMessageAttachments() const
    {
        return use_message_attachments;
    }
    bool IsUseNewuserVoting() const
    {
        return use_newuser_voting;
    }
    bool IsUseNotifyOnLogins() const
    {
        return use_notify_on_logins;
    }
    bool IsUseNotifyOnLogoff() const
    {
        return use_notify_on_logoff;
    }
    bool IsUsePostcallRatio() const
    {
        return use_postcall_ratio;
    }
    bool IsUseScreenPrelogin() const
    {
        return use_screen_prelogin;
    }
    bool IsUseScreenWelcome() const
    {
        return use_screen_welcome;
    }
    bool IsUseServiceSsl() const
    {
        return use_service_ssl;
    }
    bool IsUseServiceTelnet() const
    {
        return use_service_telnet;
    }
    bool IsUseUploadChecker() const
    {
        return use_upload_checker;
    }

    // string
    std::string bbs_name_sysop;        // SysopName,
    std::string bbs_name;              // BoardName;
    std::string bbs_uuid;              // new {unique uuid for this bbs for services etc }
    std::string password_system;         // SysPass,
    std::string password_sysop;           // System1Pass,
    std::string password_newuser;         // NewUserPass;

    // int
    int port_telnet;           // new { default telnet port }
    int port_ssl;              // new { default ssl port }

    // bool
    bool use_service_telnet;    // new
    bool use_service_ssl;       // new
    std::string directory_bbs;         // BBSDir,
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
    int points_per_kilobyte;       // KForEachPoint;
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
    std::string default_color_propmpt;    // DefPromptColor,
    std::string default_color_input;      // DefInputColor,
    std::string default_color_inverse;    // DefInverseColor,
    std::string default_color_box;        // DefBoxColor;

    // string
    std::string default_user_flags;      // DefaultFlags;
    std::string flag_change_daily;       // DailyFlagChange,
    std::string flag_change_session; 	// CallFlagChange;

    // int
    int default_level; 			 // DefLevel,
    int default_file_level;	     // DefFLevel
    int default_message_level;    // new

    // int
    int default_file_points;     // DefFilePoints,
    int default_file_ratio;      // DefUDR,
    int default_kilobyte_ratio;  // DefUDKR,
    int default_kilobyte_daily;  // DnKPerDay,
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

    Config()
        : bbs_name_sysop("New OBV2 XRM Sysop")
        , bbs_name("New OBV2 XRM BBS")
        , bbs_uuid("")
        , password_system("")
        , password_sysop("")
        , password_newuser("")
        , port_telnet(23)
        , port_ssl(443)
        , use_service_telnet(true)
        , use_service_ssl(false)
        , directory_bbs("")
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
        , points_per_kilobyte(0)
        , points_earned_postcall(0)
        , access_sysop("s255")
        , access_message_sysop("s200")
        , access_file_sysop("s200")
        , access_hidden_files("s200")
        , access_post_anonymous("s20")
        , access_mail_attachment("s20")
        , access_top_ten("s20")
        , access_check_sysop_avail("s20")
        , use_file_points(false)        // NEW
        , use_postcall_ratio(false)     // NEW
        , use_library_ansi(true)
        , use_notify_on_logins(true)
        , use_notify_on_logoff(true)
        , use_log_chat_sessions(true)
        , use_screen_prelogin(false)
        , use_screen_welcome(false)
        , use_matrix_login(false)
        , hidden_input_character('*')
        , use_auto_validate_users(false)
        , use_newuser_voting(false)
        , use_auto_kick_unvalidated(false)
        , newuser_votes_validate(3)
        , newuser_votes_delete(3)
        , newuser_days_to_upload(7)
        , days_keep_logs(30)
        , qwk_packet_name("Obv2_XRM")
        , starting_menu_name("top")
        , use_message_attachments(false)
        , days_keep_attachments(30)
        , default_color_regular("|09")
        , default_color_stat("|03")
        , default_color_propmpt("|01")
        , default_color_input("|15")
        , default_color_inverse("|17")
        , default_color_box("|11")
        , default_user_flags("")
        , flag_change_daily("")
        , flag_change_session("")
        , default_level(20)
        , default_file_level(20)
        , default_message_level(20)
        , default_file_points(0)
        , default_file_ratio(0)
        , default_kilobyte_ratio(0)
        , default_kilobyte_daily(0)
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

    {
        // Generate an Initial Board UUID
        boost::uuids::random_generator generator;
        boost::uuids::uuid uuid = generator();
        bbs_uuid = boost::lexical_cast<std::string>(uuid);
    }
    ~Config() { }

};


typedef boost::shared_ptr<Config> config_ptr;
typedef boost::weak_ptr<Config>   config_weak_ptr;

//BOOST_CLASS_VERSION(Config, 1)
//BOOST_CLASS_IS_WRAPPER(Config)
//BOOST_SERIALIZATION_BASE_OBJECT_NVP(Config)

//BOOST_CLASS_EXPORT(Config)
//BOOST_SERIALIZATION_SHARED_PTR (Config)
//BOOST_SERIALIZATION_BASE_OBJECT_NVP(Config)
// elminate serialization overhead at the cost of
// never being able to increase the version.
//BOOST_CLASS_IMPLEMENTATION(Config, boost::serialization::object_serializable)

// eliminate object tracking (even if serialized through a pointer)
// at the risk of a programming error creating duplicate objects.
//BOOST_CLASS_TRACKING(Config, boost::serialization::track_never)


#endif // CONFIG_HPP
