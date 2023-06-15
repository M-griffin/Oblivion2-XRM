#ifndef ACCESS_LEVELS_HPP
#define ACCESS_LEVELS_HPP

#include <memory>
#include <string>
#include <cstdint>


/**
 * @class AccessLevel
 * @author Michael Griffin
 * @date 05/04/2017
 * @file access_level.hpp
 * @brief Security Access Levels
 */
class AccessLevel
{
public:

    explicit AccessLevel()
        : iId(-1)
        , sName("New Access Level")
        , sStartMenu("top")
        , iLevel(0)
        , iFileLevel(0)
        , iMessageLevel(0)
        , iPostCallRatio(0)
        , iFileRatio(0)
        , iTimeLimit(0)
        , iCallLimit(0)
        , iDownloads(0)
        , iDownloadMB(0)
        , iARFlags1(0)
        , iARFlags2(0)
          // Flags
        , bPostCallRatio(false)
        , bFileRatio(false)
        , bTimeLimit(false)
        , bCallLimit(false)
        , bDownloads(false)
        , bDownloadMB(false)
    {
    }

    long iId;
    std::string sName;
    std::string sStartMenu;
    int iLevel;
    int iFileLevel;
    int iMessageLevel;
    int iPostCallRatio;
    int iFileRatio;
    int iTimeLimit;
    int iCallLimit;
    int iDownloads;
    long iDownloadMB;
    uint32_t iARFlags1;
    uint32_t iARFlags2;
    bool bPostCallRatio;
    bool bFileRatio;
    bool bTimeLimit;
    bool bCallLimit;
    bool bDownloads;
    bool bDownloadMB;


};


typedef std::shared_ptr<AccessLevel> access_level_ptr;

#endif // ACCESS_LEVELS_HPP
