#include "oneliners_dao.hpp"
#include "../model-app/oneliners.hpp"
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
bool OnelinerDao::doesTableExist()
{
    return baseDoesTableExist();
}

/**
 * @brief Run Setup Params for SQL Database Table.
 */
bool OnelinerDao::firstTimeSetupParams()
{
    return baseFirstTimeSetupParams();
}

/**
 * @brief Create Database Table
 * @return
 */
bool OnelinerDao::createTable()
{
    return baseCreateTable();
}

/**
 * @brief Drop Database
 * @return
 */
bool OnelinerDao::dropTable()
{
    return baseDropTable();
}

/**
 * @brief Updates a Record in the database!
 * @param area
 * @return
 */
bool OnelinerDao::updateRecord(oneliner_ptr obj)
{
    return baseUpdateRecord(obj);
}

/**
 * @brief Inserts a New Record in the database!
 * @param area
 * @return
 */
long OnelinerDao::insertRecord(oneliner_ptr obj)
{
    return baseInsertRecord(obj);
}

/**
 * @brief Deletes a Record
 * @param areaId
 * @return
 */
bool OnelinerDao::deleteRecord(long id)
{
    return baseDeleteRecord(id);
}

/**
 * @brief Retrieve Record By Id.
 * @param id
 * @return
 */
oneliner_ptr OnelinerDao::getRecordById(long id)
{
    return baseGetRecordById(id);
}

/**
 * @brief Retrieve All Records in a Table
 * @return
 */
std::vector<oneliner_ptr> OnelinerDao::getAllRecords()
{
    return baseGetAllRecords();
}

/**
 * @brief Retrieve Count of All Records in a Table
 * @return
 */
long OnelinerDao::getRecordsCount()
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
void OnelinerDao::pullOnelinerResult(query_ptr qry, oneliner_ptr obj)
{
    qry->getFieldByName("iId", obj->iId);
    qry->getFieldByName("iUserId", obj->iUserId);
    qry->getFieldByName("sText", obj->sText);
    qry->getFieldByName("sUserName", obj->sUserName);
    qry->getFieldByName("sUserInitials", obj->sUserInitials);
    qry->getFieldByName("dtDatePosted", obj->dtDatePosted);
}

/**
 * @brief (Callback) for Insert Statement translates to (Column, .. ) VALUES (%d, %Q,)
 * @param qry
 * @param obj
 * @param values
 */
void OnelinerDao::fillOnelinerColumnValues(query_ptr qry, oneliner_ptr obj,
        std::vector< std::pair<std::string, std::string> > &values)
{
    // values.push_back(qry->translateFieldName("iId", conf->iId));
    values.push_back(qry->translateFieldName("iUserId", obj->iUserId));
    values.push_back(qry->translateFieldName("sText", obj->sText));
    values.push_back(qry->translateFieldName("sUserName", obj->sUserName));
    values.push_back(qry->translateFieldName("sUserInitials", obj->sUserInitials));
    values.push_back(qry->translateFieldName("dtDatePosted", obj->dtDatePosted));
}

/**
 * @brief (Callback) Create Record Insert Statement, returns query string
 * @param qry
 * @param obj
 * @return
 */
std::string OnelinerDao::insertOnelinerQryString(std::string qry, oneliner_ptr obj)
{
    // Mprint statement to avoid injections.
    std::string result = sqlite3_mprintf(qry.c_str(),
                                         obj->iUserId,
                                         obj->sText.c_str(),
                                         obj->sUserName.c_str(),
                                         obj->sUserInitials.c_str(),
                                         obj->dtDatePosted
                                        );

    return result;
}

/**
 * @brief (CallBack) Update Existing Record.
 * @param qry
 * @param obj
 * @return
 */
std::string OnelinerDao::updateOnelinerQryString(std::string qry, oneliner_ptr obj)
{
    // Mprint statement to avoid injections.
    std::string result = sqlite3_mprintf(qry.c_str(),
                                         obj->iUserId,
                                         obj->sText.c_str(),
                                         obj->sUserName.c_str(),
                                         obj->sUserInitials.c_str(),
                                         obj->dtDatePosted,
                                         obj->iId
                                        );

    return result;
}


/**
 * One Off Methods SQL Queries not included in the BaseDao
 * (Below This Point)
 */


/**
 * @brief Return All Records By User ID.
 * @return
 */
std::vector<oneliner_ptr> OnelinerDao::getAllOnelinersByUserId(long userId)
{
    Logging *log = Logging::instance();
    oneliner_ptr obj(new Oneliners);
    std::vector<oneliner_ptr> list;

    // Make Sure Database Reference is Connected
    if(!m_database.isConnected())
    {
        log->xrmLog<Logging::ERROR_LOG>("Error, Database is not connected!", __FILE__, __LINE__);
        return list;
    }

    // Create Pointer and Connect Query Object to Database.
    query_ptr qry(new SQLW::Query(m_database));

    if(!qry->isConnected())
    {
        log->xrmLog<Logging::ERROR_LOG>("Error, Query has no connection to the database", __FILE__, __LINE__);
        return list;
    }

    // Build Query String
    std::string queryString = sqlite3_mprintf("SELECT * FROM %Q WHERE iUserId = %ld;", m_strTableName.c_str(), userId);

    // Execute Query.
    if(qry->getResult(queryString))
    {
        long rows = qry->getNumRows();

        if(rows > 0)
        {
            while(qry->fetchRow())
            {
                obj.reset(new Oneliners);
                pullOnelinerResult(qry, obj);
                list.push_back(obj);
            }
        }
        else
        {
            log->xrmLog<Logging::ERROR_LOG>("Error, getAllOneliners Returned Rows=", rows, __FILE__, __LINE__);
        }
    }
    else
    {
        log->xrmLog<Logging::ERROR_LOG>("Error, getResult()", __FILE__, __LINE__);
    }

    return list;
}
