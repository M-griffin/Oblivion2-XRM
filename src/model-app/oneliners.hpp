#ifndef ONELINERS_HPP
#define ONELINERS_HPP

#include <boost/smart_ptr/shared_ptr.hpp>
#include <ctime>


class Oneliners;
typedef boost::shared_ptr<Oneliners> oneliner_ptr;

/**
 * @class Oneliners
 * @author Michael Griffin
 * @date 05/05/2017
 * @file oneliners.hpp
 * @brief OneLiners Model
 */
class Oneliners
{
public:
    explicit Oneliners() 
        : iId(-1)
        , iUserId(-1)
        , sText("")
        , sUserName("")
        , sUserInitials("")
        , dtDatePosted(0)
    {}
    
    ~Oneliners()
    {}

    long iId;
    long iUserId;        
    std::string sText;
    std::string sUserName;
    std::string sUserInitials;
    std::time_t dtDatePosted;

};


#endif // ONELINERS_HPP
