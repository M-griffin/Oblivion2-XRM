#include "message_area_dao.hpp"

#include "../model-sys/message_area.hpp"
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
bool MessageAreaDao::doesTableExist()
{
    return baseDoesTableExist();
}

/**
 * @brief Run Setup Params for SQL Database Table.
 */
bool MessageAreaDao::firstTimeSetupParams()
{
    return baseFirstTimeSetupParams();
}

/**
 * @brief Create Database Table
 * @return
 */
bool MessageAreaDao::createTable()
{
    return baseCreateTable();
}

/**
 * @brief Drop Database
 * @return
 */
bool MessageAreaDao::dropTable()
{
    return baseDropTable();
}

/**
 * @brief Updates a Record in the database!
 * @param area
 * @return
 */
bool MessageAreaDao::updateRecord(message_area_ptr obj)
{
    return baseUpdateRecord(obj);
}

/**
 * @brief Inserts a New Record in the database!
 * @param area
 * @return
 */
long MessageAreaDao::insertRecord(message_area_ptr obj)
{
    return baseInsertRecord(obj);
}

/**
 * @brief Deletes a Record
 * @param areaId
 * @return
 */
bool MessageAreaDao::deleteRecord(long id)
{
    return baseDeleteRecord(id);
}

/**
 * @brief Retrieve Record By Id.
 * @param id
 * @return
 */
message_area_ptr MessageAreaDao::getRecordById(long id)
{
    return baseGetRecordById(id);
}

/**
 * @brief Retrieve All Records in a Table
 * @return
 */
std::vector<message_area_ptr> MessageAreaDao::getAllRecords()
{
    return baseGetAllRecords();
}

/**
 * @brief Retrieve Count of All Records in a Table
 * @return
 */
long MessageAreaDao::getRecordsCount()
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
void MessageAreaDao::pullMessageAreaResult(query_ptr qry, message_area_ptr obj)
{
    qry->getFieldByName("iId", obj->iId);
    qry->getFieldByName("sName", obj->sName);
    qry->getFieldByName("sAcsAccess", obj->sAcsAccess);
    qry->getFieldByName("sAcsAccess", obj->sAcsAccess);
    qry->getFieldByName("sAcsPost", obj->sAcsPost);
    qry->getFieldByName("bAnonymous", obj->bAnonymous);
    qry->getFieldByName("sSponsor", obj->sSponsor);
    qry->getFieldByName("sOriginLine", obj->sOriginLine);
    qry->getFieldByName("sFidoPath", obj->sFidoPath);
    qry->getFieldByName("iNetworkId", obj->iNetworkId);
    qry->getFieldByName("sQwkName", obj->sQwkName);
    qry->getFieldByName("iMaxMessages", obj->iMaxMessages);
    qry->getFieldByName("bRealName", obj->bRealName);
    qry->getFieldByName("sLinkname", obj->sLinkname);
    qry->getFieldByName("bRequired", obj->bRequired);
    qry->getFieldByName("bPrivate", obj->bPrivate);
    qry->getFieldByName("bNetmail", obj->bNetmail);
    qry->getFieldByName("iSortOrder", obj->iSortOrder);
}

/**
 * @brief (Callback) for Insert Statement translates to (Column, .. ) VALUES (%d, %Q,)
 * @param qry
 * @param obj
 * @param values
 */
void MessageAreaDao::fillMessageAreaColumnValues(query_ptr qry, message_area_ptr obj,
        std::vector< std::pair<std::string, std::string> > &values)
{
    // values.push_back(qry->translateFieldName("iId", conf->iId));
    values.push_back(qry->translateFieldName("sName", obj->sName));
    values.push_back(qry->translateFieldName("sAcsAccess", obj->sAcsAccess));
    values.push_back(qry->translateFieldName("sAcsPost", obj->sAcsPost));
    values.push_back(qry->translateFieldName("bAnonymous", obj->bAnonymous));
    values.push_back(qry->translateFieldName("sSponsor", obj->sSponsor));
    values.push_back(qry->translateFieldName("sOriginLine", obj->sOriginLine));
    values.push_back(qry->translateFieldName("sFidoPath", obj->sFidoPath));
    values.push_back(qry->translateFieldName("iNetworkId", obj->iNetworkId));
    values.push_back(qry->translateFieldName("sQwkName", obj->sQwkName));
    values.push_back(qry->translateFieldName("iMaxMessages", obj->iMaxMessages));
    values.push_back(qry->translateFieldName("bRealName", obj->bRealName));
    values.push_back(qry->translateFieldName("sLinkname", obj->sLinkname));
    values.push_back(qry->translateFieldName("bRequired", obj->bRequired));
    values.push_back(qry->translateFieldName("bPrivate", obj->bPrivate));
    values.push_back(qry->translateFieldName("bNetmail", obj->bNetmail));
    values.push_back(qry->translateFieldName("iSortOrder", obj->iSortOrder));
}

/**
 * @brief (Callback) Create Record Insert Statement, returns query string
 * @param qry
 * @param obj
 * @return
 */
std::string MessageAreaDao::insertMessageAreaQryString(std::string qry, message_area_ptr obj)
{
    // Mprint statement to avoid injections.
    char *result = sqlite3_mprintf(qry.c_str(),
                                         obj->sName.c_str(),
                                         obj->sAcsAccess.c_str(),
                                         obj->sAcsPost.c_str(),
                                         obj->bAnonymous,
                                         obj->sSponsor.c_str(),
                                         obj->sOriginLine.c_str(),
                                         obj->sFidoPath.c_str(),
                                         obj->iNetworkId,
                                         obj->sQwkName.c_str(),
                                         obj->iMaxMessages,
                                         obj->bRealName,
                                         obj->sLinkname.c_str(),
                                         obj->bRequired,
                                         obj->bPrivate,
                                         obj->bNetmail,
                                         obj->iSortOrder
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
std::string MessageAreaDao::updateMessageAreaQryString(std::string qry, message_area_ptr obj)
{
    // Mprint statement to avoid injections.
    char *result = sqlite3_mprintf(qry.c_str(),
                                         obj->sName.c_str(),
                                         obj->sAcsAccess.c_str(),
                                         obj->sAcsPost.c_str(),
                                         obj->bAnonymous,
                                         obj->sSponsor.c_str(),
                                         obj->sOriginLine.c_str(),
                                         obj->sFidoPath.c_str(),
                                         obj->iNetworkId,
                                         obj->sQwkName.c_str(),
                                         obj->iMaxMessages,
                                         obj->bRealName,
                                         obj->sLinkname.c_str(),
                                         obj->bRequired,
                                         obj->bPrivate,
                                         obj->bNetmail,
                                         obj->iSortOrder,
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
 * @brief Return List of All MessageArea by ConferenceId
 * @param areas
 * @return
 */
std::vector<message_area_ptr> MessageAreaDao::getAllMessageAreasByConference(long id)
{
    Logging &log = Logging::getInstance();
    message_area_ptr area(new MessageArea);
    std::vector<message_area_ptr> list;

    // Make Sure Database Reference is Connected
    if(!m_database.isConnected())
    {
        log.write<Logging::ERROR_LOG>("Error, Database is not connected!", m_strTableName, __FILE__, __LINE__);
        return list;
    }

    // Create Pointer and Connect Query Object to Database.
    query_ptr qry(new SQLW::Query(m_database));

    if(!qry->isConnected())
    {
        log.write<Logging::ERROR_LOG>("Error, Query has no connection to the database", m_strTableName, __FILE__, __LINE__);
        return list;
    }

    // Build Query String
    char *result = sqlite3_mprintf("SELECT a.* FROM %Q a, Grouping g WHERE g.iConferenceId = %ld AND a.iID = g.iMsgAreaId;", m_strTableName.c_str(), id);
    std::string queryString(result);
    sqlite3_free(result);

    // Execute Query.
    if(qry->getResult(queryString))
    {
        long rows = qry->getNumRows();

        if(rows > 0)
        {
            while(qry->fetchRow())
            {
                area.reset(new MessageArea);
                pullMessageAreaResult(qry, area);
                list.push_back(area);
            }
        }
        else
        {
            log.write<Logging::ERROR_LOG>("Error, getAllMessageAreasByConference Returned Rows", rows, m_strTableName, __FILE__, __LINE__);
        }
    }
    else
    {
        log.write<Logging::ERROR_LOG>("Error, getResult()", m_strTableName, __FILE__, __LINE__);
    }

    return list;
}
