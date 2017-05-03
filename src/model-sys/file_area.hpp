#ifndef FILE_AREA_HPP
#define FILE_AREA_HPP

#include <boost/smart_ptr/shared_ptr.hpp>

class FileArea;
typedef boost::shared_ptr<FileArea> file_area_ptr;

/**
 * @class FileArea
 * @author Michael Griffin
 * @date 03/05/2017
 * @file file_area.hpp
 * @brief File Area Model
 */
class FileArea
{
public:
        
    long iId;
    std::string sName;
    std::string sType;
    std::string sACS;
    long iSortOrder;
    
    FileArea()
        : iId(-1)
        , sType("")
        , sACS("")
        , iSortOrder(-1)
    { }
    
    ~FileArea()
    { }

};

#endif // FILE_AREA_HPP

