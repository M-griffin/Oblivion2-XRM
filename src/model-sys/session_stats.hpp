#ifndef SESSION_STATS_HPP
#define SESSION_STATS_HPP

#include <string>

/**
 * @class SessionStats
 * @author Michael Griffin
 * @date 15/04/2017
 * @file session_stats.hpp
 * @brief Logs Stats Usage Per Session Connection
 */
class SessionStats {
public:
    long iId;
    long iUserId;
    std::string sSessionType;
    std::string sCodePage;
    std::string sTerminal;
    std::string sIPAddress;
    int iTermWidth;
    int iTermHeight;
    long dtStartDate;
    long dtEndDate;
    int iInvalidAttempts;
    bool bLogonSuccess;
    bool bNewUser;
    bool bHungup;
    int iMsgRead;
    int iMsgPost;
    long iFilesUl;
    long iFilesDl;
    long iFilesUlMb;
    long iFilesDlMb;

    explicit SessionStats()
        : iId(-1)
          , iUserId(-1)
          , sSessionType("Telnet")
          , sCodePage("cp437")
          , sTerminal("ansi")
          , sIPAddress("localhost")
          , iTermWidth(0)
          , iTermHeight(0)
          , dtStartDate(0)
          , dtEndDate(0)
          , iInvalidAttempts(0)
          , bLogonSuccess(false)
          , bNewUser(false)
          , bHungup(false)
          , iMsgRead(0)
          , iMsgPost(0)
          , iFilesUl(0)
          , iFilesDl(0)
          , iFilesUlMb(0)
          , iFilesDlMb(0) {
    }

    ~SessionStats() = default;
};

#endif
