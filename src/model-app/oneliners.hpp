#ifndef ONELINERS_HPP
#define ONELINERS_HPP

#include <string>
#include <chrono>

/**
 * @class Oneliners
 * @author Michael Griffin
 * @date 05/05/2017
 * @file oneliners.hpp
 * @brief OneLiners Model
 */
class Oneliners {
public:
    explicit Oneliners()
        : iId(-1)
          , iUserId(-1)
          , sText("")
          , sUserName("")
          , sUserInitials("")
          , dtDatePosted(0) {
    }

    ~Oneliners() = default;

    long iId;
    long iUserId;
    std::string sText;
    std::string sUserName;
    std::string sUserInitials;
    std::time_t dtDatePosted;
};

#endif
