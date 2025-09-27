#include "access_level_dao.hpp"

#include <iostream>
#include <string>

#include <sqlite3.h>

#include "../model-sys/access_level.hpp"

#include "../logging.hpp"
#include "libSqliteWrapped.h"


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
    qry->getFieldByName("bPostCallRatio", obj->bPostCallRatio);
    qry->getFieldByName("bFileRatio", obj->bFileRatio);
    qry->getFieldByName("bTimeLimit", obj->bTimeLimit);
    qry->getFieldByName("bCallLimit", obj->bCallLimit);
    qry->getFieldByName("bDownloads", obj->bDownloads);
    qry->getFieldByName("bDownloadMB", obj->bDownloadMB);
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
    values.push_back(qry->translateFieldName("bPostCallRatio", obj->bPostCallRatio));
    values.push_back(qry->translateFieldName("bFileRatio", obj->bFileRatio));
    values.push_back(qry->translateFieldName("bTimeLimit", obj->bTimeLimit));
    values.push_back(qry->translateFieldName("bCallLimit", obj->bCallLimit));
    values.push_back(qry->translateFieldName("bDownloads", obj->bDownloads));
    values.push_back(qry->translateFieldName("bDownloadMB", obj->bDownloadMB));
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
    char *result = sqlite3_mprintf(qry.c_str(),
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
                                         obj->bPostCallRatio,
                                         obj->bFileRatio,
                                         obj->bTimeLimit,
                                         obj->bCallLimit,
                                         obj->bDownloads,
                                         obj->bDownloadMB
                                        );

    std::string queryString(result);
    sqlite3_free(result);
    return queryString;
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
    char *result = sqlite3_mprintf(qry.c_str(),
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
                                         obj->bPostCallRatio,
                                         obj->bFileRatio,
                                         obj->bTimeLimit,
                                         obj->bCallLimit,
                                         obj->bDownloads,
                                         obj->bDownloadMB,
                                         obj->iId
                                        );

    std::string queryString(result);
    sqlite3_free(result);
    return queryString;
}


/**
 * One Off Methods SQL Queries not included in the BaseDao
 * (Below This Point)
 */

/**
 * @brief Return AccessLevel Record by Level.
 * @return
 */
access_level_ptr AccessLevelDao::getAccessLevelByLevel(long access_level)
{
    Logging &log = Logging::getInstance();
    access_level_ptr level = std::make_shared<AccessLevel>();

    // Make Sure Database Reference is Connected
    if(!m_database.isConnected())
    {
        log.write<Logging::ERROR_LOG>("Error, Database is not connected!", m_strTableName, __LINE__, __FILE__);
        return level;
    }

    // Create Pointer and Connect Query Object to Database.
    query_ptr qry = std::make_shared<SQLW::Query>(m_database);

    if(!qry->isConnected())
    {
        log.write<Logging::ERROR_LOG>("Error, Query has no connection to the database", m_strTableName, __LINE__, __FILE__);
        return level;
    }

    // Build Query String
    char *result = sqlite3_mprintf("SELECT * FROM %Q WHERE iLevel = %d;",
                              m_strTableName.c_str(), access_level);
    std::string queryString(result);
    sqlite3_free(result);

    // create a test3 table
    if(qry->getResult(queryString))
    {
        long rows = qry->getNumRows();

        if(rows > 0)
        {
            qry->fetchRow();
            pullAccessLevelResult(qry, level);
        }
        else
        {
            log.write<Logging::ERROR_LOG>("Error, getAccessLevelByLevel Returned Rows=", rows, m_strTableName, __LINE__, __FILE__);
        }
    }
    else
    {
        log.write<Logging::ERROR_LOG>("Error, getResult()", m_strTableName, __LINE__, __FILE__);
    }

    return level;
}
