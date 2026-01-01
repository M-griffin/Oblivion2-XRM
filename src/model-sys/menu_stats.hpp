#ifndef MENU_STATS_HPP
#define MENU_STATS_HPP

#include <string>
#include <chrono>

/**
 * @class MenuStats
 * @author Michael Griffin
 * @date 05/04/2017
 * @file menu_stats.hpp
 * @brief Menu Traffic and Command Usage (Anonymous)
 */
class MenuStats {
public:
    explicit MenuStats()
        : iId(-1)
          , sMenuName("")
          , sCmdKey("")
          , dtDateTime(0) {
    }

    long iId;
    std::string sMenuName;
    std::string sCmdKey;
    std::time_t dtDateTime;
};

#endif
