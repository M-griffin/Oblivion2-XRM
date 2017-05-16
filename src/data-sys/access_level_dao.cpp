#include "access_level_dao.hpp"
#include "../model-sys/users.hpp"

#include "libSqliteWrapped.h"
#include <sqlite3.h>

#include <iostream>
#include <string>


/**
 * Base Dao Calls for generic Object Data Calls
 * (Below This Point)
 */
 
 
/**
 * @brief Check If Database Table Exists.
 * @return
 */
bool AccessLevelDao::doesTableExist()
{
    return baseDoesTableExist();
}

/**
 * @brief Run Setup Params for SQL Database Table.
 */
bool AccessLevelDao::firstTimeSetupParams()
{
    return baseFirstTimeSetupParams();
}

/**
 * @brief Create Database Table
 * @return
 */
bool AccessLevelDao::createTable()
{
    return baseCreateTable();
}

/**
 * @brief Drop Database
 * @return
 */
bool AccessLevelDao::dropTable()
{
    return baseDropTable();
}

/**
 * @brief Updates a Record in the database!
 * @param area
 * @return
 */
bool AccessLevelDao::updateRecord(access_level_ptr obj)
{
    return baseUpdateRecord(obj);
}

/**
 * @brief Inserts a New Record in the database!
 * @param area
 * @return
 */
long AccessLevelDao::insertRecord(access_level_ptr obj)
{
    return baseInsertRecord(obj);
}

/**
 * @brief Deletes a Record
 * @param areaId
 * @return
 */
bool AccessLevelDao::deleteRecord(long id)
{
    return baseDeleteRecord(id);
}

/**
 * @brief Retrieve Record By Id.
 * @param id
 * @return 
 */ 
access_level_ptr AccessLevelDao::getRecordById(long id)
{
    return baseGetRecordById(id);
}

/**
 * @brief Retrieve All Records in a Table
 * @return
 */
std::vector<access_level_ptr> AccessLevelDao::getAllRecords()
{
    return baseGetAllRecords();
}

/**
 * @brief Retrieve Count of All Records in a Table
 * @return
 */
long AccessLevelDao::getRecordsCount()
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
void AccessLevelDao::pullAccessLevelResult(query_ptr qry, access_level_ptr obj)
{
    qry->getFieldByName("iId", obj->iId);
    qry->getFieldByName("sName", obj->sName);
    qry->getFieldByName("sStartMenu", obj->sStartMenu);
    qry->getFieldByName("iLevel", obj->iLevel);
    qry->getFieldByName("iFileLevel", obj->iFileLevel);
    qry->getFieldByName("iMessageLevel", obj->iMessageLevel);
    qry->getFieldByName("iPostCallRatio", obj->iPostCallRatio);
    qry->getFieldByName("iFileRatio", obj->iFileRatio);
    qry->getFieldByName("iTimeLimit", obj->iTimeLimit);
    qry->getFieldByName("iCallLimit", obj->iCallLimit);
    qry->getFieldByName("iDownloads", obj->iDownloads);
    qry->getFieldByName("iDownloadMB", obj->iDownloadMB);
    qry->getFieldByName("iARFlags1", obj->iARFlags1);
    qry->getFieldByName("iARFlags2", obj->iARFlags2);
}

/**
 * @brief (Callback) for Insert Statement translates to (Column, .. ) VALUES (%d, %Q,)
 * @param qry
 * @param obj
 * @param values
 */ 
void AccessLevelDao::fillAccessLevelColumnValues(query_ptr qry, access_level_ptr obj, std::vector< std::pair<std::string, std::string> > &values)
{
    // values.push_back(qry->translateFieldName("iId", obj->iId));
    values.push_back(qry->translateFieldName("sName", obj->sName));
    values.push_back(qry->translateFieldName("sStartMenu", obj->sStartMenu));
    values.push_back(qry->translateFieldName("iLevel", obj->iLevel));
    values.push_back(qry->translateFieldName("iFileLevel", obj->iFileLevel));
    values.push_back(qry->translateFieldName("iMessageLevel", obj->iMessageLevel));
    values.push_back(qry->translateFieldName("iPostCallRatio", obj->iPostCallRatio));
    values.push_back(qry->translateFieldName("iFileRatio", obj->iFileRatio));
    values.push_back(qry->translateFieldName("iTimeLimit", obj->iTimeLimit));
    values.push_back(qry->translateFieldName("iCallLimit", obj->iCallLimit));
    values.push_back(qry->translateFieldName("iDownloads", obj->iDownloads));
    values.push_back(qry->translateFieldName("iDownloadMB", obj->iDownloadMB));
    values.push_back(qry->translateFieldName("iARFlags1", obj->iARFlags1));
    values.push_back(qry->translateFieldName("iARFlags2", obj->iARFlags2));

    
}

/**
 * @brief (Callback) Create Record Insert Statement, returns query string 
 * @param qry
 * @param obj
 * @return 
 */
std::string AccessLevelDao::insertAccessLevelQryString(std::string qry, access_level_ptr obj)
{    
    // Mprint statement to avoid injections.
    std::string result = sqlite3_mprintf(qry.c_str(),
        obj->sName.c_str(),
        obj->sStartMenu.c_str(),
        obj->iLevel,
        obj->iFileLevel,
        obj->iMessageLevel,
        obj->iPostCallRatio,
        obj->iFileRatio,
        obj->iTimeLimit,
        obj->iCallLimit,
        obj->iDownloads,
        obj->iDownloadMB,
        obj->iARFlags1,
        obj->iARFlags2
    );

    return result;
}

/**
 * @brief (CallBack) Update Existing Record. 
 * @param qry
 * @param obj
 * @return 
 */
std::string AccessLevelDao::updateAccessLevelQryString(std::string qry, access_level_ptr obj)
{
    // Mprint statement to avoid injections.
    std::string result = sqlite3_mprintf(qry.c_str(),
        obj->sName.c_str(),
        obj->sStartMenu.c_str(),
        obj->iLevel,
        obj->iFileLevel,
        obj->iMessageLevel,
        obj->iPostCallRatio,
        obj->iFileRatio,
        obj->iTimeLimit,
        obj->iCallLimit,
        obj->iDownloads,
        obj->iDownloadMB,
        obj->iARFlags1,
        obj->iARFlags2,
        obj->iId
    );

    return result;
}


/**
 * One Off Methods SQL Queries not included in the BaseDao
 * (Below This Point)
 */
 
