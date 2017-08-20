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

/*
          Area Name           - The name of the current file area.
          Access ACS          - ACS required to enter the file area.
          Upload ACS          - ACS required to upload to the area.
          Download ACS        - ACS required to download from the area.
          List ACS            - ACS required to list files in the area.
          Sponsor             - Pseudo SysOp of the file area. User
                                gets extended functions.
          Password            - Password required to enter file area.
          Area filename       - Filename (w/o extension) that the BBS
                                will use to store file information for
                                the current area.
          Path                - Path to the files for this area.
          Default sort        - Sort type to use when sorting the file
                                areas. (example: F=filename,
                                E=extension etc..)
          Multiplier          - When you set the points of a file, that
                                value is multiplied by the number you
                                put here. This is used to make
                                downloading from one area cost more
                                than downloading from others. If you
                                don't want to use this option, set it
                                to 1.
          Free Area           - If this is set to yes, all files in the
                                area are marked as free and no credit
                                is deducted for their download.
*/
        
    long        iId;
    std::string sName;
    std::string sAcsAccess;
    std::string sAcsUpload;
    std::string sAcsDownload;
    std::string sAcsList;
    std::string sSponsor;
    long        iSecurityIndex;
    std::string sLinkname;
    std::string sSort;
    int         iMultiplier;
    bool        bFreeArea;
    long        iSortOrder;
    
    explicit FileArea()
        : iId(-1)
        , sName("")
        , sAcsAccess("")
        , sAcsUpload("")
        , sAcsDownload("")
        , sAcsList("")
        , sSponsor("")
        , iSecurityIndex(-1)
        , sLinkname("")
        , sSort("F")
        , iMultiplier(0)
        , bFreeArea(true)
        , iSortOrder(-1)
    { }
    
    ~FileArea()
    { }

};

#endif // FILE_AREA_HPP

