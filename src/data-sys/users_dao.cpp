#include "users_dao.hpp"
#include "../model-sys/users.hpp"

#include "libSqliteWrapped.h"
#include <sqlite3.h>

#include <iostream>
#include <string>
#include <algorithm>

/**
 * Base Dao Calls for generic Object Data Calls
 * (Below This Point)
 */
 
 
/**
 * @brief Check If Database Table Exists.
 * @return
 */
bool UsersDao::doesTableExist()
{
    return baseDoesTableExist();
}

/**
 * @brief Run Setup Params for SQL Database Table.
 */
bool UsersDao::firstTimeSetupParams()
{
    return baseFirstTimeSetupParams();
}

/**
 * @brief Create Database Table
 * @return
 */
bool UsersDao::createTable()
{
    return baseCreateTable();
}

/**
 * @brief Drop Database
 * @return
 */
bool UsersDao::dropTable()
{
    return baseDropTable();
}

/**
 * @brief Updates a Record in the database!
 * @param area
 * @return
 */
bool UsersDao::updateRecord(user_ptr obj)
{
    return baseUpdateRecord(obj);
}

/**
 * @brief Inserts a New Record in the database!
 * @param area
 * @return
 */
long UsersDao::insertRecord(user_ptr obj)
{
    return baseInsertRecord(obj);
}

/**
 * @brief Deletes a Record
 * @param id
 * @return
 */
bool UsersDao::deleteRecord(long id)
{
    return baseDeleteRecord(id);
}

/**
 * @brief Retrieve Record By Id.
 * @param id
 * @return 
 */ 
user_ptr UsersDao::getRecordById(long id)
{
    return baseGetRecordById(id);
}

/**
 * @brief Retrieve All Records in a Table
 * @return
 */
std::vector<user_ptr> UsersDao::getAllRecords()
{
    return baseGetAllRecords();
}

/**
 * @brief Retrieve Count of All Records in a Table
 * @return
 */
long UsersDao::getRecordsCount()
{
    return baseGetRecordsCount();
}


/**
 * Base Dao Call Back for Object Specific Data Mappings
 * (Below This Point)
 */
 

/**
 * @brief (CallBack) Pulls results by FieldNames into their Class Variables. 
 * @param qry
 * @param obj
 */
void UsersDao::pullUsersResult(query_ptr qry, user_ptr obj)
{
    qry->getFieldByName("iId", obj->iId);
    qry->getFieldByName("sHandle", obj->sHandle);
    qry->getFieldByName("sRealName", obj->sRealName);
    qry->getFieldByName("sAddress", obj->sAddress);
    qry->getFieldByName("sLocation", obj->sLocation);
    qry->getFieldByName("sCountry", obj->sCountry);
    qry->getFieldByName("sEmail", obj->sEmail);
    qry->getFieldByName("sUserNote", obj->sUserNote);
    qry->getFieldByName("dtBirthday", obj->dtBirthday);
    qry->getFieldByName("iLevel", obj->iLevel);
    qry->getFieldByName("iFileLevel", obj->iFileLevel);
    qry->getFieldByName("iMessageLevel", obj->iMessageLevel);
    qry->getFieldByName("iLastFileArea", obj->iLastFileArea);
    qry->getFieldByName("iLastMessageArea", obj->iLastMessageArea);
    qry->getFieldByName("iTimeLeft", obj->iTimeLeft);
    qry->getFieldByName("iTimeLimit", obj->iTimeLimit);
    qry->getFieldByName("sRegColor", obj->sRegColor);
    qry->getFieldByName("sPromptColor", obj->sPromptColor);
    qry->getFieldByName("sInputColor", obj->sInputColor);
    qry->getFieldByName("sInverseColor", obj->sInverseColor);
    qry->getFieldByName("sStatColor", obj->sStatColor);
    qry->getFieldByName("sBoxColor", obj->sBoxColor);
    qry->getFieldByName("iFilePoints", obj->iFilePoints);    
    qry->getFieldByName("iPostCallRatio", obj->iPostCallRatio);
    qry->getFieldByName("sMenuPromptName", obj->sMenuPromptName);
    qry->getFieldByName("iMenuSelected", obj->iMenuSelected);
    qry->getFieldByName("iStatusSelected", obj->iStatusSelected);
    qry->getFieldByName("bAnsi", obj->bAnsi);
    qry->getFieldByName("bBackSpaceVt100", obj->bBackSpaceVt100);
    qry->getFieldByName("iNuvVotesYes", obj->iNuvVotesYes);
    qry->getFieldByName("iNuvVotesNo", obj->iNuvVotesNo);
    qry->getFieldByName("dtPassChangeDate", obj->dtPassChangeDate);
    qry->getFieldByName("dtLastReplyDate", obj->dtLastReplyDate);
    qry->getFieldByName("bScrollFL", obj->bAnsi);
    qry->getFieldByName("iCSPassChange", obj->iCSPassChange);
    qry->getFieldByName("iControlFlags1", obj->iControlFlags1);
    qry->getFieldByName("iControlFlags2", obj->iControlFlags2);
    qry->getFieldByName("bWanted", obj->bWanted);
    qry->getFieldByName("sHeaderType", obj->sHeaderType);
    qry->getFieldByName("iLastMesConf", obj->iLastMesConf);
    qry->getFieldByName("iLastFileConf", obj->iLastFileConf);
    qry->getFieldByName("dtExpirationDate", obj->dtExpirationDate);
    qry->getFieldByName("sGender", obj->sGender);
    qry->getFieldByName("dtFirstOn", obj->dtFirstOn);
    qry->getFieldByName("bYesNoBars", obj->bYesNoBars);
    qry->getFieldByName("iHackAttempts", obj->iHackAttempts);
    qry->getFieldByName("bDoPause", obj->bDoPause);
    qry->getFieldByName("bClearOrScroll", obj->bClearOrScroll);
    qry->getFieldByName("bIgnoreTimeLimit", obj->bIgnoreTimeLimit);
    qry->getFieldByName("bAllowPurge", obj->bAllowPurge);
    qry->getFieldByName("iSecurityIndex", obj->iSecurityIndex);
}

/**
 * @brief (Callback) for Insert Statement translates to (Column, .. ) VALUES (%d, %Q,)
 * @param qry
 * @param obj
 * @param values
 */ 
void UsersDao::fillUsersColumnValues(query_ptr qry, user_ptr obj, std::vector< std::pair<std::string, std::string> > &values)
{
    // values.push_back(qry->translateFieldName("iId", obj->iId));
    values.push_back(qry->translateFieldName("sHandle", obj->sHandle));
    values.push_back(qry->translateFieldName("sRealName", obj->sRealName));
    values.push_back(qry->translateFieldName("sAddress", obj->sAddress));
    values.push_back(qry->translateFieldName("sLocation", obj->sLocation));
    values.push_back(qry->translateFieldName("sCountry", obj->sCountry));
    values.push_back(qry->translateFieldName("sEmail", obj->sEmail));
    values.push_back(qry->translateFieldName("sUserNote", obj->sUserNote));
    values.push_back(qry->translateFieldName("dtBirthday", obj->dtBirthday));
    values.push_back(qry->translateFieldName("iLevel", obj->iLevel));
    values.push_back(qry->translateFieldName("iFileLevel", obj->iFileLevel));
    values.push_back(qry->translateFieldName("iMessageLevel", obj->iMessageLevel));
    values.push_back(qry->translateFieldName("iLastFileArea", obj->iLastFileArea));
    values.push_back(qry->translateFieldName("iLastMessageArea", obj->iLastMessageArea));
    values.push_back(qry->translateFieldName("iTimeLeft", obj->iTimeLeft));
    values.push_back(qry->translateFieldName("iTimeLimit", obj->iTimeLimit));
    values.push_back(qry->translateFieldName("sRegColor", obj->sRegColor));
    values.push_back(qry->translateFieldName("sPromptColor", obj->sPromptColor));
    values.push_back(qry->translateFieldName("sInputColor", obj->sInputColor));
    values.push_back(qry->translateFieldName("sInverseColor", obj->sInverseColor));
    values.push_back(qry->translateFieldName("sStatColor", obj->sStatColor));
    values.push_back(qry->translateFieldName("sBoxColor", obj->sBoxColor));    
    values.push_back(qry->translateFieldName("iFilePoints", obj->iFilePoints));
    values.push_back(qry->translateFieldName("iPostCallRatio", obj->iPostCallRatio));
    values.push_back(qry->translateFieldName("sMenuPromptName", obj->sMenuPromptName));
    values.push_back(qry->translateFieldName("iMenuSelected", obj->iMenuSelected));
    values.push_back(qry->translateFieldName("iStatusSelected", obj->iStatusSelected));
    values.push_back(qry->translateFieldName("bAnsi", obj->bAnsi));
    values.push_back(qry->translateFieldName("bBackSpaceVt100", obj->bBackSpaceVt100));
    values.push_back(qry->translateFieldName("iNuvVotesYes", obj->iNuvVotesYes));
    values.push_back(qry->translateFieldName("iNuvVotesNo", obj->iNuvVotesNo));
    values.push_back(qry->translateFieldName("dtPassChangeDate", obj->dtPassChangeDate));
    values.push_back(qry->translateFieldName("dtLastReplyDate", obj->dtLastReplyDate));
    values.push_back(qry->translateFieldName("bScrollFL", obj->bAnsi));
    values.push_back(qry->translateFieldName("iCSPassChange", obj->iCSPassChange));
    values.push_back(qry->translateFieldName("iControlFlags1", obj->iControlFlags1));
    values.push_back(qry->translateFieldName("iControlFlags2", obj->iControlFlags2));
    values.push_back(qry->translateFieldName("bWanted", obj->bWanted));
    values.push_back(qry->translateFieldName("sHeaderType", obj->sHeaderType));
    values.push_back(qry->translateFieldName("iLastMesConf", obj->iLastMesConf));
    values.push_back(qry->translateFieldName("iLastFileConf", obj->iLastFileConf));
    values.push_back(qry->translateFieldName("dtExpirationDate", obj->dtExpirationDate));
    values.push_back(qry->translateFieldName("sGender", obj->sGender));
    values.push_back(qry->translateFieldName("dtFirstOn", obj->dtFirstOn));
    values.push_back(qry->translateFieldName("bYesNoBars", obj->bYesNoBars));
    values.push_back(qry->translateFieldName("iHackAttempts", obj->iHackAttempts));
    values.push_back(qry->translateFieldName("bDoPause", obj->bDoPause));
    values.push_back(qry->translateFieldName("bClearOrScroll", obj->bClearOrScroll));
    values.push_back(qry->translateFieldName("bIgnoreTimeLimit", obj->bIgnoreTimeLimit));
    values.push_back(qry->translateFieldName("bAllowPurge", obj->bAllowPurge));
    values.push_back(qry->translateFieldName("iSecurityIndex", obj->iSecurityIndex));
}

/**
 * @brief (Callback) Create Record Insert Statement, returns query string 
 * @param qry
 * @param obj
 * @return 
 */
std::string UsersDao::insertUsersQryString(std::string qry, user_ptr obj)
{    
    // Mprint statement to avoid injections.
    std::string result = sqlite3_mprintf(qry.c_str(),
        obj->sHandle.c_str(),
        obj->sRealName.c_str(),
        obj->sAddress.c_str(),
        obj->sLocation.c_str(),
        obj->sCountry.c_str(),
        obj->sEmail.c_str(),
        obj->sUserNote.c_str(),
        obj->dtBirthday,
        obj->iLevel,
        obj->iFileLevel,
        obj->iMessageLevel,
        obj->iLastFileArea,
        obj->iLastMessageArea,
        obj->iTimeLeft,
        obj->iTimeLimit,
        obj->sRegColor.c_str(),
        obj->sPromptColor.c_str(),
        obj->sInputColor.c_str(),
        obj->sInverseColor.c_str(),
        obj->sStatColor.c_str(),
        obj->sBoxColor.c_str(),
        obj->iFilePoints,
        obj->iPostCallRatio,
        obj->sMenuPromptName.c_str(),
        obj->iMenuSelected,
        obj->iStatusSelected,
        obj->bAnsi,
        obj->bBackSpaceVt100,
        obj->iNuvVotesYes,
        obj->iNuvVotesNo,
        obj->dtPassChangeDate,
        obj->dtLastReplyDate,
        obj->bAnsi,
        obj->iCSPassChange,
        obj->iControlFlags1,
        obj->iControlFlags2,
        obj->bWanted,
        obj->sHeaderType.c_str(),
        obj->iLastMesConf,
        obj->iLastFileConf,
        obj->dtExpirationDate,
        obj->sGender.c_str(),
        obj->dtFirstOn,
        obj->bYesNoBars,
        obj->iHackAttempts,
        obj->bDoPause,
        obj->bClearOrScroll,
        obj->bIgnoreTimeLimit,
        obj->bAllowPurge,
        obj->iSecurityIndex
    );

    return result;
}

/**
 * @brief (CallBack) Update Existing Record. 
 * @param qry
 * @param obj
 * @return 
 */
std::string UsersDao::updateUsersQryString(std::string qry, user_ptr obj)
{
    // Mprint statement to avoid injections.
    std::string result = sqlite3_mprintf(qry.c_str(),
        obj->sHandle.c_str(),
        obj->sRealName.c_str(),
        obj->sAddress.c_str(),
        obj->sLocation.c_str(),
        obj->sCountry.c_str(),
        obj->sEmail.c_str(),
        obj->sUserNote.c_str(),
        obj->dtBirthday,
        obj->iLevel,
        obj->iFileLevel,
        obj->iMessageLevel,
        obj->iLastFileArea,
        obj->iLastMessageArea,
        obj->iTimeLeft,
        obj->iTimeLimit,
        obj->sRegColor.c_str(),
        obj->sPromptColor.c_str(),
        obj->sInputColor.c_str(),
        obj->sInverseColor.c_str(),
        obj->sStatColor.c_str(),
        obj->sBoxColor.c_str(),
        obj->iFilePoints,
        obj->iPostCallRatio,
        obj->sMenuPromptName.c_str(),
        obj->iMenuSelected,
        obj->iStatusSelected,
        obj->bAnsi,
        obj->bBackSpaceVt100,
        obj->iNuvVotesYes,
        obj->iNuvVotesNo,
        obj->dtPassChangeDate,
        obj->dtLastReplyDate,
        obj->bAnsi,
        obj->iCSPassChange,
        obj->iControlFlags1,
        obj->iControlFlags2,
        obj->bWanted,
        obj->sHeaderType.c_str(),
        obj->iLastMesConf,
        obj->iLastFileConf,
        obj->dtExpirationDate,
        obj->sGender.c_str(),
        obj->dtFirstOn,
        obj->bYesNoBars,
        obj->iHackAttempts,
        obj->bDoPause,
        obj->bClearOrScroll,
        obj->bIgnoreTimeLimit,
        obj->bAllowPurge,
        obj->iSecurityIndex,
        obj->iId
    );

    return result;
}


/**
 * One Off Methods SQL Queries not included in the BaseDao
 * (Below This Point)
 */
 
 
/**
 * @brief Return User Record By Handle.
 * @return
 */
user_ptr UsersDao::getUserByHandle(std::string name)
{
    user_ptr user(new Users);

    // Make Sure Database Reference is Connected
    if (!m_database.isConnected())
    {
        std::cout << "Error, Database is not connected!" << std::endl;
        return user;
    }

    // Create Pointer and Connect Query Object to Database.
    query_ptr qry(new SQLW::Query(m_database));
    if (!qry->isConnected())
    {
        std::cout << "Error, Query has no connection to the database" << std::endl;
        return user;
    }

    // Build Query String
    std::string queryString = sqlite3_mprintf("SELECT * FROM %Q WHERE sHandle = %Q; COLLATE NOCASE;",
        m_strTableName.c_str(), name.c_str());

    // Execute Query.
    if (qry->getResult(queryString))
    {
        long rows = qry->getNumRows();
        if (rows > 0)
        {
            qry->fetchRow();
            pullUsersResult(qry, user);
        }
        else
        {
            std::cout << "Error, getUserByHandle Returned Rows: " << rows << std::endl;
        }
    }
    else
    {
        std::cout << "Error, getResult()" << std::endl;
    }

    return user;
}

/**
 * @brief Return User Record By FullName.
 * @return
 */
user_ptr UsersDao::getUserByRealName(std::string name)
{
    user_ptr user(new Users);

    // Make Sure Database Reference is Connected
    if (!m_database.isConnected())
    {
        std::cout << "Error, Database is not connected!" << std::endl;
        return user;
    }

    // Create Pointer and Connect Query Object to Database.
    query_ptr qry(new SQLW::Query(m_database));
    if (!qry->isConnected())
    {
        std::cout << "Error, Query has no connection to the database" << std::endl;
        return user;
    }

    // Build Query String
    std::string queryString = sqlite3_mprintf("SELECT * FROM %Q WHERE sRealName = %Q COLLATE NOCASE;",
        m_strTableName.c_str(), name.c_str());

    // Execute Query.
    if (qry->getResult(queryString))
    {
        long rows = qry->getNumRows();
        if (rows > 0)
        {
            qry->fetchRow();
            pullUsersResult(qry, user);
        }
        else
        {
            std::cout << "Error, getUserByRealName Returned Rows: " << rows << std::endl;
        }
    }
    else
    {
        std::cout << "Error, getResult()" << std::endl;
    }
    return user;
}

/**
 * @brief Return User Record By Email.
 * @return
 */
user_ptr UsersDao::getUserByEmail(std::string email)
{
    user_ptr user(new Users);

    // Make Sure Database Reference is Connected
    if (!m_database.isConnected())
    {
        std::cout << "Error, Database is not connected!" << std::endl;
        return user;
    }

    // Create Pointer and Connect Query Object to Database.
    query_ptr qry(new SQLW::Query(m_database));
    if (!qry->isConnected())
    {
        std::cout << "Error, Query has no connection to the database" << std::endl;
        return user;
    }

    // Build Query String
    std::string queryString = sqlite3_mprintf("SELECT * FROM %Q WHERE sEmail = %Q COLLATE NOCASE;",
        m_strTableName.c_str(), email.c_str());

    // create a test3 table
    if (qry->getResult(queryString))
    {
        long rows = qry->getNumRows();
        if (rows > 0)
        {
            qry->fetchRow();
            pullUsersResult(qry, user);
        }
        else
        {
            std::cout << "Error, getUserByEmail Returned Rows: " << rows << std::endl;
        }
    }
    else
    {
        std::cout << "Error, getResult()" << std::endl;
    }
 
    return user;
}

/**
 * @brief Return User Records By WildCard (filtered Searches)
 * @return
 */
std::vector<user_ptr> UsersDao::getUsersByWildcard(std::string filter)
{
    user_ptr obj(new Users());
    std::vector<user_ptr> list;

    // Make Sure Database Reference is Connected
    if (!m_database.isConnected())
    {
        std::cout << "Error, Database is not connected!" << std::endl;
        return list;
    }

    // Create Pointer and Connect Query Object to Database.
    query_ptr qry(new SQLW::Query(m_database));
    if (!qry->isConnected())
    {
        std::cout << "Error, Query has no connection to the database" << std::endl;
        return list;
    }

    // Replace * with %
    std::replace( filter.begin(), filter.end(), '*', '%');

    // Build Query String
    std::string queryString = sqlite3_mprintf("SELECT * FROM %Q WHERE sHandle like %Q ORDER BY sHandle COLLATE NOCASE asc;",
        m_strTableName.c_str(), filter.c_str());

    // Execute Query.
    if (qry->getResult(queryString))
    {
        long rows = qry->getNumRows();
        if (rows > 0)
        {
            while(qry->fetchRow())
            {
                obj.reset(new Users());
                pullUsersResult(qry, obj);
                list.push_back(obj);
            }
        }
        else
        {
            std::cout << "Error, getUsersByWildcard Returned Rows: " << rows << std::endl;
        }
    }
    else
    {
        std::cout << "Error, getResult()" << std::endl;
    }

    return list;    
}
