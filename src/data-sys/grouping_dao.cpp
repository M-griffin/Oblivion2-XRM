#include "grouping_dao.hpp"
#include "../model-sys/grouping.hpp"
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
bool GroupingDao::doesTableExist()
{
    return baseDoesTableExist();
}

/**
 * @brief Run Setup Params for SQL Database Table.
 */
bool GroupingDao::firstTimeSetupParams()
{
    return baseFirstTimeSetupParams();
}

/**
 * @brief Create Database Table
 * @return
 */
bool GroupingDao::createTable()
{
    return baseCreateTable();
}

/**
 * @brief Drop Database
 * @return
 */
bool GroupingDao::dropTable()
{
    return baseDropTable();
}

/**
 * @brief Updates a Record in the database!
 * @param area
 * @return
 */
bool GroupingDao::updateRecord(group_ptr obj)
{
    return baseUpdateRecord(obj);
}

/**
 * @brief Inserts a New Record in the database!
 * @param area
 * @return
 */
long GroupingDao::insertRecord(group_ptr obj)
{
    return baseInsertRecord(obj);
}

/**
 * @brief Deletes a Record
 * @param areaId
 * @return
 */
bool GroupingDao::deleteRecord(long id)
{
    return baseDeleteRecord(id);
}

/**
 * @brief Retrieve Record By Id.
 * @param id
 * @return
 */
group_ptr GroupingDao::getRecordById(long id)
{
    return baseGetRecordById(id);
}

/**
 * @brief Retrieve All Records in a Table
 * @return
 */
std::vector<group_ptr> GroupingDao::getAllRecords()
{
    return baseGetAllRecords();
}

/**
 * @brief Retrieve Count of All Records in a Table
 * @return
 */
long GroupingDao::getRecordsCount()
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
void GroupingDao::pullGroupingResult(query_ptr qry, group_ptr obj)
{
    qry->getFieldByName("iId", obj->iId);
    qry->getFieldByName("iConferenceId", obj->iConferenceId);
    qry->getFieldByName("iAreaId", obj->iAreaId);
}

/**
 * @brief (Callback) for Insert Statement translates to (Column, .. ) VALUES (%d, %Q,)
 * @param qry
 * @param obj
 * @param values
 */
void GroupingDao::fillGroupingColumnValues(query_ptr qry, group_ptr obj,
        std::vector< std::pair<std::string, std::string> > &values)
{
    // values.push_back(qry->translateFieldName("iId", obj->iId));
    values.push_back(qry->translateFieldName("iConferenceId", obj->iConferenceId));
    values.push_back(qry->translateFieldName("iAreaId", obj->iAreaId));
}

/**
 * @brief (Callback) Create Record Insert Statement, returns query string
 * @param qry
 * @param obj
 * @return
 */
std::string GroupingDao::insertGroupingQryString(std::string qry, group_ptr obj)
{
    // Mprint statement to avoid injections.
    char *result = sqlite3_mprintf(qry.c_str(),
                                         obj->iConferenceId,
                                         obj->iAreaId
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
std::string GroupingDao::updateGroupingQryString(std::string qry, group_ptr obj)
{
    // Mprint statement to avoid injections.
    char *result = sqlite3_mprintf(qry.c_str(),
                                         obj->iConferenceId,
                                         obj->iAreaId,
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
 * @brief Return List of All Groupings by ConferenceId
 * @param confId
 * @return
 */
std::vector<group_ptr> GroupingDao::getAllGroupingsByConferenceId(long id)
{
    group_ptr group(new Grouping);
    std::vector<group_ptr> list;
    Logging &log = Logging::getInstance();

    // Make Sure Database Reference is Connected
    if(!m_database.isConnected())
    {
        log.write<Logging::ERROR_LOG>(m_strTableName, "Error, Database is not connected!", __LINE__, __FILE__);
        return list;
    }

    // Create Pointer and Connect Query Object to Database.
    query_ptr qry(new SQLW::Query(m_database));

    if(!qry->isConnected())
    {
        log.write<Logging::ERROR_LOG>(m_strTableName, "Error, Query has no connection to the database", __LINE__, __FILE__);
        return list;
    }

    // Build Query String
    char *result = sqlite3_mprintf("SELECT * FROM %Q WHERE iConferenceId = %ld;", m_strTableName.c_str(), id);
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
                group.reset(new Grouping);
                pullGroupingResult(qry, group);
                list.push_back(group);
            }
        }
        else
        {
            log.write<Logging::ERROR_LOG>(m_strTableName, "Error, getAllGroupingsByConferenceId Returned Rows", rows, __LINE__, __FILE__);
        }
    }
    else
    {
        log.write<Logging::ERROR_LOG>(m_strTableName, "Error, getResult()", __LINE__, __FILE__);
    }

    return list;
}
