#ifndef USERS_HPP
#define USERS_HPP

#include <string>
#include <ctime>

#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/smart_ptr/weak_ptr.hpp>

class Users;
typedef boost::shared_ptr<Users> user_ptr;
typedef boost::weak_ptr<Users> user_weak_ptr;

/**
 * @class Users
 * @author Michael Griffin
 * @date 2/28/2016
 * @file users.hpp
 * @brief User Model for Database Table.
 */
class Users
{
public:

    /**
     * Variables
     */
    long iId;
    std::string sHandle;
    std::string sRealName;
    std::string sAddress;
    std::string sLocation;
    std::string sCountry;
    std::string sEmail;
    std::string sUserNote;
    std::time_t dtBirthday;

    int iLevel;
    int iFileLevel;

    int iLastFileArea;
    int iLastMessageArea;

    std::time_t dtLastCallDate;

    int iTimeLeft;
    int iTimeLimit;

    // Might remove these!
    std::string sRegColor;
    std::string sPromptColor;
    std::string sInputColor;
    std::string sInverseColor;
    std::string sStatColor;
    std::string sBoxColor;

    int iPostCallRatio;

    int iPromptSelected;
    int iMenuSelected;
    int iStatusSelected;

    bool bAnsi;
    bool bBackSpaceVt100;

    int iNuvVotesYes;
    int iNuvVotesNo;

    std::time_t dtPassChangeDate;
    std::time_t dtLastReplyDate;

    bool bScrollFL;

    bool iCallsToday;
    bool iNewLevel;
    bool iCSPassChange;
    std::string sControlFlags;

    bool bWanted;

    std::string sHeaderType;
    int iLastMesConf;
    int iLastFileConf;

    std::time_t dtExpirationDate;
    std::string sGender;

    std::time_t dtFirstOn;
    bool bYesNoBars;
    int iHackAttempts;
    bool bDoPause;
    bool bClearOrScroll;
    bool bIgnoreTimeLimit;
    bool bAllowPurge;

    // Default User ID to -1 so we know it's not active!
    Users()
        : iId(-1)
        , sHandle("")
        , sRealName("")
        , sAddress("")
        , sLocation("")
        , sCountry("")
        , sEmail("")
        , sUserNote("")
        , dtBirthday(0)
        , iLevel(10)
        , iFileLevel(10)
        , iLastFileArea(0)
        , iLastMessageArea(0)
        , dtLastCallDate(0)
        , iTimeLeft(1440)
        , iTimeLimit(1440)
        , sRegColor("|05")
        , sPromptColor("|09")
        , sInputColor("|07")
        , sInverseColor("|17")
        , sStatColor("|11")
        , sBoxColor("|03")
        , iPostCallRatio(0)
        , iPromptSelected(0)
        , iMenuSelected(0)
        , iStatusSelected(0)
        , bAnsi(true)
        , bBackSpaceVt100(false)
        , iNuvVotesYes(0)
        , iNuvVotesNo(0)
        , dtPassChangeDate(0)
        , dtLastReplyDate(0)
        , bScrollFL(true)
        , iCallsToday(0)
        , iNewLevel(10)
        , iCSPassChange(0)
        , sControlFlags("")
        , bWanted(false)
        , sHeaderType("")
        , iLastMesConf(0)
        , iLastFileConf(0)
        , dtExpirationDate(0)
        , sGender("M")
        , dtFirstOn(0)
        , bYesNoBars(true)
        , iHackAttempts(0)
        , bDoPause(true)
        , bClearOrScroll(true)
        , bIgnoreTimeLimit(false)
        , bAllowPurge(true)
    { }
    ~Users() { }

};



#endif // USERS_HPP
