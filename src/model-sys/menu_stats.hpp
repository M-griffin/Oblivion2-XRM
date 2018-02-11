#ifndef MENU_STATS_HPP
#define MENU_STATS_HPP

#include <memory>
#include <string>
#include <ctime>


/**
 * @class MenuStats
 * @author Michael Griffin
 * @date 05/04/2017
 * @file menu_stats.hpp
 * @brief Menu Traffic and Command Usage (Anonymous)
 */
class MenuStats
{
public:

    explicit MenuStats()    
        : iId(-1)
        , sMenuName("")
        , sCmdKey("")
        , dtDateTime(0)
    {
    }
    
    long iId;
    std::string sMenuName;
    std::string sCmdKey;
    std::time_t dtDateTime;

};


typedef std::shared_ptr<MenuStats> menu_stats_ptr;

#endif // MENU_STATS_HPP
