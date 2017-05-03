#ifndef CONFERENCE_HPP
#define CONFERENCE_HPP

#include <boost/smart_ptr/shared_ptr.hpp>

class Conference;
typedef boost::shared_ptr<Conference> conference_ptr;

/**
 * @class Conference
 * @author Michael Griffin
 * @date 03/05/2017
 * @file conference.hpp
 * @brief Conference, Types Message, File, and/or Global
 */
class Conference
{
public:
        
    long iId;
    std::string sName;
    std::string sType;
    std::string sACS;
    long iSortOrder;
    
    Conference()
        : iId(-1)
        , sType("")
        , sACS("")
        , iSortOrder(-1)
    { }
    
    ~Conference()
    { }

};

#endif // CONFERENCE_HPP

