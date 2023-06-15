#include "conference_dao.hpp"
#include "../model-sys/conference.hpp"
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
bool ConferenceDao::doesTableExist()
{
    return baseDoesTableExist();
}

/**
 * @brief Run Setup Params for SQL Database Table.
 */
bool ConferenceDao::firstTimeSetupParams()
{
    return baseFirstTimeSetupParams();
}

/**
 * @brief Create Database Table
 * @return
 */
bool ConferenceDao::createTable()
{
    return baseCreateTable();
}

/**
 * @brief Drop Database
 * @return
 */
bool ConferenceDao::dropTable()
{
    return baseDropTable();
}

/**
 * @brief Updates a Record in the database!
 * @param area
 * @return
 */
bool ConferenceDao::updateRecord(conference_ptr obj)
{
    return baseUpdateRecord(obj);
}

/**
 * @brief Inserts a New Record in the database!
 * @param area
 * @return
 */
long ConferenceDao::insertRecord(conference_ptr obj)
{
    return baseInsertRecord(obj);
}

/**
 * @brief Deletes a Record
 * @param areaId
 * @return
 */
bool ConferenceDao::deleteRecord(long id)
{
    return baseDeleteRecord(id);
}

/**
 * @brief Retrieve Record By Id.
 * @param id
 * @return
 */
conference_ptr ConferenceDao::getRecordById(long id)
{
    return baseGetRecordById(id);
}

/**
 * @brief Retrieve All Records in a Table
 * @return
 */
std::vector<conference_ptr> ConferenceDao::getAllRecords()
{
    return baseGetAllRecords();
}

/**
 * @brief Retrieve Count of All Records in a Table
 * @return
 */
long ConferenceDao::getRecordsCount()
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
void ConferenceDao::pullConferenceResult(query_ptr qry, conference_ptr obj)
{
    qry->getFieldByName("iId", obj->iId);
    qry->getFieldByName("sName", obj->sName);
    qry->getFieldByName("sType", obj->sType);
    qry->getFieldByName("sACS", obj->sACS);
    qry->getFieldByName("iSortOrder", obj->iSortOrder);
}

/**
 * @brief (Callback) for Insert Statement translates to (Column, .. ) VALUES (%d, %Q,)
 * @param qry
 * @param obj
 * @param values
 */
void ConferenceDao::fillConferenceColumnValues(query_ptr qry, conference_ptr obj,
        std::vector< std::pair<std::string, std::string> > &values)
{
    // values.push_back(qry->translateFieldName("iId", obj->iId));
    values.push_back(qry->translateFieldName("sName", obj->sName));
    values.push_back(qry->translateFieldName("sType", obj->sType));
    values.push_back(qry->translateFieldName("sACS", obj->sACS));
    values.push_back(qry->translateFieldName("iSortOrder", obj->iSortOrder));
}

/**
 * @brief (Callback) Create Record Insert Statement, returns query string
 * @param qry
 * @param obj
 * @return
 */
std::string ConferenceDao::insertConferenceQryString(std::string qry, conference_ptr obj)
{
    // Mprint statement to avoid injections.
    std::string result = sqlite3_mprintf(qry.c_str(),
                                         obj->sName.c_str(),
                                         obj->sType.c_str(),
                                         obj->sACS.c_str(),
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
std::string ConferenceDao::updateConferenceQryString(std::string qry, conference_ptr obj)
{
    // Mprint statement to avoid injections.
    std::string result = sqlite3_mprintf(qry.c_str(),
                                         obj->sName.c_str(),
                                         obj->sType.c_str(),
                                         obj->sACS.c_str(),
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
 * @brief Return List of All Conference by Type
 * @param type
 * @return
 */
std::vector<conference_ptr> ConferenceDao::getAllConferencesByType(std::string type)
{
    Logging &log = Logging::getInstance();
    conference_ptr conf(new Conference);
    std::vector<conference_ptr> list;

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
    std::string queryString = sqlite3_mprintf("SELECT * FROM %Q WHERE sType like %Q;", m_strTableName.c_str(), type.c_str());

    // Execute Query.
    if(qry->getResult(queryString))
    {
        long rows = qry->getNumRows();

        if(rows > 0)
        {
            while(qry->fetchRow())
            {
                conf.reset(new Conference);
                pullConferenceResult(qry, conf);
                list.push_back(conf);
            }
        }
        else
        {
            log.write<Logging::ERROR_LOG>("Error, getAllConferencesByType Returned Rows=", rows, m_strTableName, __FILE__, __LINE__);
        }
    }
    else
    {
        log.write<Logging::ERROR_LOG>("Error, getResult()", m_strTableName, __FILE__, __LINE__);
    }

    return list;
}

/**
 * @brief Return Count or Number of Existing Conferences by Type
 * @param type
 * @return
 */
long ConferenceDao::getConferencesCountByType(std::string type)
{
    Logging &log = Logging::getInstance();
    conference_ptr conf(new Conference);
    std::vector<conference_ptr> list;

    // Make Sure Database Reference is Connected
    if(!m_database.isConnected())
    {
        log.write<Logging::ERROR_LOG>("Error, Database is not connected!", m_strTableName, __FILE__, __LINE__);
        return list.size();
    }

    // Create Pointer and Connect Query Object to Database.
    query_ptr qry(new SQLW::Query(m_database));

    if(!qry->isConnected())
    {
        log.write<Logging::ERROR_LOG>("Error, Query has no connection to the database", m_strTableName, __FILE__, __LINE__);
        return list.size();
    }

    // Build Query String
    std::string queryString = sqlite3_mprintf("SELECT * FROM %Q WHERE sType like %Q;", m_strTableName.c_str(), type.c_str());

    // Execute Query.
    if(qry->getResult(queryString))
    {
        long rows = qry->getNumRows();

        if(rows > 0)
        {
            while(qry->fetchRow())
            {
                conf.reset(new Conference);
                pullConferenceResult(qry, conf);
                list.push_back(conf);
            }
        }
        else
        {
            log.write<Logging::ERROR_LOG>("Error, getConferenceCount By Type Returned Rows=", rows, m_strTableName, __FILE__, __LINE__);
        }
    }
    else
    {
        log.write<Logging::ERROR_LOG>("Error, getResult()", m_strTableName, __FILE__, __LINE__);
    }

    return list.size();
}
