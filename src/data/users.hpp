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
    bool bVt100;

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
    bool bIgnoreTimeLimit;
    bool bAllowPurge;

    void SetBAllowPurge(bool bAllowPurge)
    {
        this->bAllowPurge = bAllowPurge;
    }
    void SetBAnsi(bool bAnsi)
    {
        this->bAnsi = bAnsi;
    }
    void SetBDoPause(bool bDoPause)
    {
        this->bDoPause = bDoPause;
    }
    void SetBIgnoreTimeLimit(bool bIgnoreTimeLimit)
    {
        this->bIgnoreTimeLimit = bIgnoreTimeLimit;
    }
    void SetBScrollFL(bool bScrollFL)
    {
        this->bScrollFL = bScrollFL;
    }
    void SetBVt100(bool bVt100)
    {
        this->bVt100 = bVt100;
    }
    void SetBWanted(bool bWanted)
    {
        this->bWanted = bWanted;
    }
    void SetBYesNoBars(bool bYesNoBars)
    {
        this->bYesNoBars = bYesNoBars;
    }
    void SetICSPassChange(bool iCSPassChange)
    {
        this->iCSPassChange = iCSPassChange;
    }
    void SetICallsToday(bool iCallsToday)
    {
        this->iCallsToday = iCallsToday;
    }
    void SetIFileLevel(int iFileLevel)
    {
        this->iFileLevel = iFileLevel;
    }
    void SetIHackAttempts(int iHackAttempts)
    {
        this->iHackAttempts = iHackAttempts;
    }
    void SetIId(long iId)
    {
        this->iId = iId;
    }
    void SetILastFileArea(int iLastFileArea)
    {
        this->iLastFileArea = iLastFileArea;
    }
    void SetILastFileConf(int iLastFileConf)
    {
        this->iLastFileConf = iLastFileConf;
    }
    void SetILastMesConf(int iLastMesConf)
    {
        this->iLastMesConf = iLastMesConf;
    }
    void SetILastMessageArea(int iLastMessageArea)
    {
        this->iLastMessageArea = iLastMessageArea;
    }
    void SetILevel(int iLevel)
    {
        this->iLevel = iLevel;
    }
    void SetIMenuSelected(int iMenuSelected)
    {
        this->iMenuSelected = iMenuSelected;
    }
    void SetINewLevel(bool iNewLevel)
    {
        this->iNewLevel = iNewLevel;
    }
    void SetINuvVotesNo(int iNuvVotesNo)
    {
        this->iNuvVotesNo = iNuvVotesNo;
    }
    void SetINuvVotesYes(int iNuvVotesYes)
    {
        this->iNuvVotesYes = iNuvVotesYes;
    }
    void SetIPostCallRatio(int iPostCallRatio)
    {
        this->iPostCallRatio = iPostCallRatio;
    }
    void SetIPromptSelected(int iPromptSelected)
    {
        this->iPromptSelected = iPromptSelected;
    }
    void SetIStatusSelected(int iStatusSelected)
    {
        this->iStatusSelected = iStatusSelected;
    }
    void SetITimeLeft(int iTimeLeft)
    {
        this->iTimeLeft = iTimeLeft;
    }
    void SetITimeLimit(int iTimeLimit)
    {
        this->iTimeLimit = iTimeLimit;
    }
    void SetSAddress(const std::string& sAddress)
    {
        this->sAddress = sAddress;
    }
    void SetSBoxColor(const std::string& sBoxColor)
    {
        this->sBoxColor = sBoxColor;
    }
    void SetSControlFlags(const std::string& sControlFlags)
    {
        this->sControlFlags = sControlFlags;
    }
    void SetSCountry(const std::string& sCountry)
    {
        this->sCountry = sCountry;
    }
    void SetSEmail(const std::string& sEmail)
    {
        this->sEmail = sEmail;
    }
    void SetSGender(const std::string& sGender)
    {
        this->sGender = sGender;
    }
    void SetSHandle(const std::string& sHandle)
    {
        this->sHandle = sHandle;
    }
    void SetSHeaderType(const std::string& sHeaderType)
    {
        this->sHeaderType = sHeaderType;
    }
    void SetSInputColor(const std::string& sInputColor)
    {
        this->sInputColor = sInputColor;
    }
    void SetSInverseColor(const std::string& sInverseColor)
    {
        this->sInverseColor = sInverseColor;
    }
    void SetSLocation(const std::string& sLocation)
    {
        this->sLocation = sLocation;
    }
    void SetSPromptColor(const std::string& sPromptColor)
    {
        this->sPromptColor = sPromptColor;
    }
    void SetSRealName(const std::string& sRealName)
    {
        this->sRealName = sRealName;
    }
    void SetSRegColor(const std::string& sRegColor)
    {
        this->sRegColor = sRegColor;
    }
    void SetSStatColor(const std::string& sStatColor)
    {
        this->sStatColor = sStatColor;
    }
    void SetSUserNote(const std::string& sUserNote)
    {
        this->sUserNote = sUserNote;
    }
    bool IsBAllowPurge() const
    {
        return bAllowPurge;
    }
    bool IsBAnsi() const
    {
        return bAnsi;
    }
    bool IsBDoPause() const
    {
        return bDoPause;
    }
    bool IsBIgnoreTimeLimit() const
    {
        return bIgnoreTimeLimit;
    }
    bool IsBScrollFL() const
    {
        return bScrollFL;
    }
    bool IsBVt100() const
    {
        return bVt100;
    }
    bool IsBWanted() const
    {
        return bWanted;
    }
    bool IsBYesNoBars() const
    {
        return bYesNoBars;
    }
    bool IsICSPassChange() const
    {
        return iCSPassChange;
    }
    bool IsICallsToday() const
    {
        return iCallsToday;
    }
    int GetIFileLevel() const
    {
        return iFileLevel;
    }
    int GetIHackAttempts() const
    {
        return iHackAttempts;
    }
    long GetIId() const
    {
        return iId;
    }
    int GetILastFileArea() const
    {
        return iLastFileArea;
    }
    int GetILastFileConf() const
    {
        return iLastFileConf;
    }
    int GetILastMesConf() const
    {
        return iLastMesConf;
    }
    int GetILastMessageArea() const
    {
        return iLastMessageArea;
    }
    int GetILevel() const
    {
        return iLevel;
    }
    int GetIMenuSelected() const
    {
        return iMenuSelected;
    }
    bool IsINewLevel() const
    {
        return iNewLevel;
    }
    int GetINuvVotesNo() const
    {
        return iNuvVotesNo;
    }
    int GetINuvVotesYes() const
    {
        return iNuvVotesYes;
    }
    int GetIPostCallRatio() const
    {
        return iPostCallRatio;
    }
    int GetIPromptSelected() const
    {
        return iPromptSelected;
    }
    int GetIStatusSelected() const
    {
        return iStatusSelected;
    }
    int GetITimeLeft() const
    {
        return iTimeLeft;
    }
    int GetITimeLimit() const
    {
        return iTimeLimit;
    }
    const std::string& GetSAddress() const
    {
        return sAddress;
    }
    const std::string& GetSBoxColor() const
    {
        return sBoxColor;
    }
    const std::string& GetSControlFlags() const
    {
        return sControlFlags;
    }
    const std::string& GetSCountry() const
    {
        return sCountry;
    }
    const std::string& GetSEmail() const
    {
        return sEmail;
    }
    const std::string& GetSGender() const
    {
        return sGender;
    }
    const std::string& GetSHandle() const
    {
        return sHandle;
    }
    const std::string& GetSHeaderType() const
    {
        return sHeaderType;
    }
    const std::string& GetSInputColor() const
    {
        return sInputColor;
    }
    const std::string& GetSInverseColor() const
    {
        return sInverseColor;
    }
    const std::string& GetSLocation() const
    {
        return sLocation;
    }
    const std::string& GetSPromptColor() const
    {
        return sPromptColor;
    }
    const std::string& GetSRealName() const
    {
        return sRealName;
    }
    const std::string& GetSRegColor() const
    {
        return sRegColor;
    }
    const std::string& GetSStatColor() const
    {
        return sStatColor;
    }
    const std::string& GetSUserNote() const
    {
        return sUserNote;
    }

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
        , bVt100(false)
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
        , bIgnoreTimeLimit(false)
        , bAllowPurge(true)
    { }
    ~Users() { }

};



#endif // USERS_HPP
