#ifndef MESSAGE_AREA_HPP
#define MESSAGE_AREA_HPP

#include <boost/smart_ptr/shared_ptr.hpp>

class MessageArea;
typedef boost::shared_ptr<MessageArea> message_area_ptr;

/**
 * @class MessageArea
 * @author Michael Griffin
 * @date 03/05/2017
 * @file message_area.hpp
 * @brief Message Area Model
 */
class MessageArea
{
public:
        
    long iId;
    std::string sName;
    std::string sType;
    std::string sACS;
    long iSortOrder;
    
    MessageArea()
        : iId(-1)
        , sType("")
        , sACS("")
        , iSortOrder(-1)
    { }
    
    ~MessageArea()
    { }

};

#endif // MESSAGE_AREA_HPP

