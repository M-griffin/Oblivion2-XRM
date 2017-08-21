#ifndef BBS_LIST_HPP
#define BBS_LIST_HPP

#include <boost/smart_ptr/shared_ptr.hpp>

class BBSList;
typedef boost::shared_ptr<BBSList> bbs_list_ptr;

/**
 * @class BBSList
 * @author Michael Griffin
 * @date 03/05/2017
 * @file bbs_list.hpp
 * @brief Bullentin Board Listing
 */
class BBSList
{
public:
  
    long        iId;
    std::string sName;
    std::string sType;
    std::string sAddress;
    std::string sSysop;
    
    
    explicit BBSList()
        : iId(-1)
        , sName("")
        , sType("")
        , sAddress("")
        , sSysop("")
    { }
    
    ~BBSList()
    { }

};

#endif // NETWORK_HPP

