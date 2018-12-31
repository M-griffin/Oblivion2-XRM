#include "file_area_dao.hpp"

#include "../model-sys/file_area.hpp"
#include "../logging.hpp"

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
bool FileAreaDao::doesTableExist()
{
    return baseDoesTableExist();
}

/**
 * @brief Run Setup Params for SQL Database Table.
 */
bool FileAreaDao::firstTimeSetupParams()
{
    return baseFirstTimeSetupParams();
}

/**
 * @brief Create Database Table
 * @return
 */
bool FileAreaDao::createTable()
{
    return baseCreateTable();
}

/**
 * @brief Drop Database
 * @return
 */
bool FileAreaDao::dropTable()
{
    return baseDropTable();
}

/**
 * @brief Updates a Record in the database!
 * @param area
 * @return
 */
bool FileAreaDao::updateRecord(file_area_ptr obj)
{
    return baseUpdateRecord(obj);
}

/**
 * @brief Inserts a New Record in the database!
 * @param area
 * @return
 */
long FileAreaDao::insertRecord(file_area_ptr obj)
{
    return baseInsertRecord(obj);
}

/**
 * @brief Deletes a Record
 * @param areaId
 * @return
 */
bool FileAreaDao::deleteRecord(long id)
{
    return baseDeleteRecord(id);
}

/**
 * @brief Retrieve Record By Id.
 * @param id
 * @return
 */
file_area_ptr FileAreaDao::getRecordById(long id)
{
    return baseGetRecordById(id);
}

/**
 * @brief Retrieve All Records in a Table
 * @return
 */
std::vector<file_area_ptr> FileAreaDao::getAllRecords()
{
    return baseGetAllRecords();
}

/**
 * @brief Retrieve Count of All Records in a Table
 * @return
 */
long FileAreaDao::getRecordsCount()
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
void FileAreaDao::pullFileAreaResult(query_ptr qry, file_area_ptr obj)
{
    qry->getFieldByName("iId", obj->iId);
    qry->getFieldByName("sName", obj->sName);
    qry->getFieldByName("sAcsAccess", obj->sAcsAccess);
    qry->getFieldByName("sAcsUpload", obj->sAcsUpload);
    qry->getFieldByName("sAcsDownload", obj->sAcsDownload);
    qry->getFieldByName("sAcsList", obj->sAcsList);
    qry->getFieldByName("sSponsor", obj->sSponsor);
    qry->getFieldByName("iSecurityIndex", obj->iSecurityIndex);
    qry->getFieldByName("sLinkname", obj->sLinkname);
    qry->getFieldByName("sSort", obj->sSort);
    qry->getFieldByName("iMultiplier", obj->iMultiplier);
    qry->getFieldByName("bFreeArea", obj->bFreeArea);
    qry->getFieldByName("iSortOrder", obj->iSortOrder);
}

/**
 * @brief (Callback) for Insert Statement translates to (Column, .. ) VALUES (%d, %Q,)
 * @param qry
 * @param obj
 * @param values
 */
void FileAreaDao::fillFileAreaColumnValues(query_ptr qry, file_area_ptr obj,
        std::vector< std::pair<std::string, std::string> > &values)
{
    // values.push_back(qry->translateFieldName("iId", conf->iId));
    values.push_back(qry->translateFieldName("sName", obj->sName));
    values.push_back(qry->translateFieldName("sAcsAccess", obj->sAcsAccess));
    values.push_back(qry->translateFieldName("sAcsUpload", obj->sAcsUpload));
    values.push_back(qry->translateFieldName("sAcsDownload", obj->sAcsDownload));
    values.push_back(qry->translateFieldName("sAcsList", obj->sAcsList));
    values.push_back(qry->translateFieldName("sSponsor", obj->sSponsor));
    values.push_back(qry->translateFieldName("iSecurityIndex", obj->iSecurityIndex));
    values.push_back(qry->translateFieldName("sLinkname", obj->sLinkname));
    values.push_back(qry->translateFieldName("sSort", obj->sSort));
    values.push_back(qry->translateFieldName("iMultiplier", obj->iMultiplier));
    values.push_back(qry->translateFieldName("bFreeArea", obj->bFreeArea));
    values.push_back(qry->translateFieldName("iSortOrder", obj->iSortOrder));
}

/**
 * @brief (Callback) Create Record Insert Statement, returns query string
 * @param qry
 * @param obj
 * @return
 */
std::string FileAreaDao::insertFileAreaQryString(std::string qry, file_area_ptr obj)
{
    // Mprint statement to avoid injections.
    std::string result = sqlite3_mprintf(qry.c_str(),
                                         obj->sName.c_str(),
                                         obj->sAcsAccess.c_str(),
                                         obj->sAcsUpload.c_str(),
                                         obj->sAcsDownload.c_str(),
                                         obj->sAcsList.c_str(),
                                         obj->sSponsor.c_str(),
                                         obj->iSecurityIndex,
                                         obj->sLinkname.c_str(),
                                         obj->sSort.c_str(),
                                         obj->iMultiplier,
                                         obj->bFreeArea,
                                         obj->iSortOrder
                                        );

    return result;
}

/**
 * @brief (CallBack) Update Existing Record.
 * @param qry
 * @param obj
 * @return
 */
std::string FileAreaDao::updateFileAreaQryString(std::string qry, file_area_ptr obj)
{
    // Mprint statement to avoid injections.
    std::string result = sqlite3_mprintf(qry.c_str(),
                                         obj->sName.c_str(),
                                         obj->sAcsAccess.c_str(),
                                         obj->sAcsUpload.c_str(),
                                         obj->sAcsDownload.c_str(),
                                         obj->sAcsList.c_str(),
                                         obj->sSponsor.c_str(),
                                         obj->iSecurityIndex,
                                         obj->sLinkname.c_str(),
                                         obj->sSort.c_str(),
                                         obj->iMultiplier,
                                         obj->bFreeArea,
                                         obj->iSortOrder,
                                         obj->iId
                                        );

    return result;
}


/**
 * One Off Methods SQL Queries not included in the BaseDao
 * (Below This Point)
 */


/**
 * @brief Return List of All FileArea by ConferenceId
 * @param areas
 * @return
 */
std::vector<file_area_ptr> FileAreaDao::getAllFileAreasByConference(long id)
{
    Logging *log = Logging::instance();
    file_area_ptr area(new FileArea);
    std::vector<file_area_ptr> list;

    // Make Sure Database Reference is Connected
    if(!m_database.isConnected())
    {
        log->xrmLog<Logging::ERROR_LOG>("Error, Database is not connected!", m_strTableName, __LINE__, __FILE__);
        return list;
    }

    // Create Pointer and Connect Query Object to Database.
    query_ptr qry(new SQLW::Query(m_database));

    if(!qry->isConnected())
    {
        log->xrmLog<Logging::ERROR_LOG>("Error, Query has no connection to the database", m_strTableName, __LINE__, __FILE__);
        return list;
    }

    // Build Query String
    std::string queryString = sqlite3_mprintf("SELECT a.* FROM %Q a, Grouping g WHERE g.iConferenceId = %ld AND a.iID = g.iFileAreaId;", m_strTableName.c_str(), id);

    // Execute Query.
    if(qry->getResult(queryString))
    {
        long rows = qry->getNumRows();

        if(rows > 0)
        {
            while(qry->fetchRow())
            {
                area.reset(new FileArea);
                pullFileAreaResult(qry, area);
                list.push_back(area);
            }
        }
        else
        {
            log->xrmLog<Logging::ERROR_LOG>("Error, getAllFileAreasByConference Returned Rows=", rows, m_strTableName, __LINE__, __FILE__);
        }
    }
    else
    {
        log->xrmLog<Logging::ERROR_LOG>("Error, getResult()", m_strTableName, __LINE__, __FILE__);
    }

    return list;
}
