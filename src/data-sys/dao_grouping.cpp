#include "dao_grouping.hpp"

#include <string>
#include <sqlite3.h>

#include "../model-sys/grouping.hpp"
#include "../util_log.hpp"

#include "libSqliteWrapped.h"


/**
 * @brief Check If Database Table Exists.
 * @return
 */
bool GroupingDao::doesTableExist() {
    return baseDoesTableExist();
}

/**
 * @brief Run Setup Params for SQL Database Table.
 */
bool GroupingDao::firstTimeSetupParams() {
    return baseFirstTimeSetupParams();
}

/**
 * @brief Create Database Table
 * @return
 */
bool GroupingDao::createTable() {
    return baseCreateTable();
}

/**
 * @brief Drop Database
 * @return
 */
bool GroupingDao::dropTable() {
    return baseDropTable();
}

/**
 * @brief Updates a Record in the database!
 * @param obj
 * @return
 */
bool GroupingDao::updateRecord(Grouping &obj) {
    return baseUpdateRecord(obj);
}

/**
 * @brief Inserts a New Record in the database!
 * @param obj
 * @return
 */
long GroupingDao::insertRecord(Grouping &obj) {
    return baseInsertRecord(obj);
}

/**
 * @brief Deletes a Record
 * @param id
 * @return
 */
bool GroupingDao::deleteRecord(const long id) {
    return baseDeleteRecord(id);
}

/**
 * @brief Retrieve Record By Id.
 * @param id
 * @return
 */
Grouping GroupingDao::getRecordById(const long id) {
    return baseGetRecordById(id);
}

/**
 * @brief Retrieve All Records in a Table
 * @return
 */
std::vector<Grouping> GroupingDao::getAllRecords() {
    return baseGetAllRecords();
}

/**
 * @brief Retrieve Count of All Records in a Table
 * @return
 */
long GroupingDao::getRecordsCount() {
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
void GroupingDao::pullGroupingResult(Query &qry, Grouping &obj) {
    qry.getFieldByName("iId", obj.iId);
    qry.getFieldByName("iConferenceId", obj.iConferenceId);
    qry.getFieldByName("iAreaId", obj.iAreaId);
}

/**
 * @brief (Callback) for Insert Statement translates to (Column, .. ) VALUES (%d, %Q,)
 * @param qry
 * @param obj
 * @param values
 */
void GroupingDao::fillGroupingColumnValues(Query &qry, Grouping &obj,
                                           std::vector<std::pair<std::string, std::string> > &values) {
    // values.push_back(qry.translateFieldName("iId", obj.iId));
    values.push_back(qry.translateFieldName("iConferenceId", obj.iConferenceId));
    values.push_back(qry.translateFieldName("iAreaId", obj.iAreaId));
}

/**
 * @brief (Callback) Create Record Insert Statement, returns query string
 * @param qry
 * @param obj
 * @return
 */
std::string GroupingDao::insertGroupingQryString(std::string qry, Grouping &obj) {
    // Mprint statement to avoid injections.
    char *result = sqlite3_mprintf(qry.c_str(),
                                   obj.iConferenceId,
                                   obj.iAreaId
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
std::string GroupingDao::updateGroupingQryString(std::string qry, Grouping &obj) {
    // Mprint statement to avoid injections.
    char *result = sqlite3_mprintf(qry.c_str(),
                                   obj.iConferenceId,
                                   obj.iAreaId,
                                   obj.iId
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
std::vector<Grouping> GroupingDao::getAllGroupingsByConferenceId(long id) {
    std::vector<Grouping> list;
    UtilLog &log = UtilLog::getInstance();

    // Make Sure Database Reference is Connected
    if (!m_database.isConnected()) {
        log.log(UtilLog::LogLevel::Info, m_strTableName, "Error, Database is not connected!", __LINE__, __FILE__);
        return list;
    }

    // Create Pointer and Connect Query Object to Database.
    Query qry(m_database);

    if (!qry.isConnected()) {
        log.log(UtilLog::LogLevel::Info, m_strTableName, "Error, Query has no connection to the database", __LINE__,
                                      __FILE__);
        return list;
    }

    // Build Query String
    char *result = sqlite3_mprintf("SELECT * FROM %Q WHERE iConferenceId = %ld;", m_strTableName.c_str(), id);
    const std::string queryString(result);
    sqlite3_free(result);

    // Execute Query.
    if (qry.getResult(queryString)) {
        const long rows = qry.getNumRows();

        if (rows > 0) {
            while (qry.fetchRow()) {
                Grouping group;
                pullGroupingResult(qry, group);
                list.push_back(group);
            }
        } else {
            log.log(UtilLog::LogLevel::Info, m_strTableName, "Error, getAllGroupingsByConferenceId Returned Rows", rows,
                                          __LINE__, __FILE__);
        }
    } else {
        log.log(UtilLog::LogLevel::Info, m_strTableName, "Error, getResult()", __LINE__, __FILE__);
    }

    return list;
}
