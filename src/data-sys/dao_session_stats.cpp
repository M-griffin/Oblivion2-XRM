#include "dao_session_stats.hpp"

#include <iostream>
#include <string>

#include <sqlite3.h>

#include "../model-sys/session_stats.hpp"
#include "../util_log.hpp"

#include "libSqliteWrapped.h"

/**
 * Base Dao Calls for generic Object Data Calls
 * (Below This Point)
 */


/**
 * @brief Check If Database Table Exists.
 * @return
 */
bool SessionStatsDao::doesTableExist() {
    return baseDoesTableExist();
}

/**
 * @brief Run Setup Params for SQL Database Table.
 */
bool SessionStatsDao::firstTimeSetupParams() {
    return baseFirstTimeSetupParams();
}

/**
 * @brief Create Database Table
 * @return
 */
bool SessionStatsDao::createTable() {
    return baseCreateTable();
}

/**
 * @brief Drop Database
 * @return
 */
bool SessionStatsDao::dropTable() {
    return baseDropTable();
}

/**
 * @brief Updates a Record in the database!
 * @param obj
 * @return
 */
bool SessionStatsDao::updateRecord(SessionStats &obj) {
    return baseUpdateRecord(obj);
}

/**
 * @brief Inserts a New Record in the database!
 * @param obj
 * @return
 */
long SessionStatsDao::insertRecord(SessionStats &obj) {
    return baseInsertRecord(obj);
}

/**
 * @brief Deletes a Record
 * @param id
 * @return
 */
bool SessionStatsDao::deleteRecord(long id) {
    return baseDeleteRecord(id);
}

/**
 * @brief Retrieve Record By Id.
 * @param id
 * @return
 */
SessionStats SessionStatsDao::getRecordById(long id) {
    return baseGetRecordById(id);
}

/**
 * @brief Retrieve All Records in a Table
 * @return
 */
std::vector<SessionStats> SessionStatsDao::getAllRecords() {
    return baseGetAllRecords();
}

/**
 * @brief Retrieve Count of All Records in a Table
 * @return
 */
long SessionStatsDao::getRecordsCount() {
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
void SessionStatsDao::pullSessionStatsResult(Query &qry, SessionStats &obj) {
    qry.getFieldByName("iId", obj.iId);
    qry.getFieldByName("iUserId", obj.iUserId);
    qry.getFieldByName("sSessionType", obj.sSessionType);
    qry.getFieldByName("sCodePage", obj.sCodePage);
    qry.getFieldByName("sTerminal", obj.sTerminal);
    qry.getFieldByName("sIPAddress", obj.sIPAddress);
    qry.getFieldByName("iTermWidth", obj.iTermWidth);
    qry.getFieldByName("iTermHeight", obj.iTermHeight);
    qry.getFieldByName("dtStartDate", obj.dtStartDate);
    qry.getFieldByName("dtEndDate", obj.dtEndDate);
    qry.getFieldByName("iInvalidAttempts", obj.iInvalidAttempts);
    qry.getFieldByName("bNewUser", obj.bNewUser);
    qry.getFieldByName("bLogonSuccess", obj.bLogonSuccess);
    qry.getFieldByName("bHungup", obj.bHungup);
    qry.getFieldByName("iMsgRead", obj.iMsgRead);
    qry.getFieldByName("iMsgPost", obj.iMsgPost);
    qry.getFieldByName("iFilesUl", obj.iFilesUl);
    qry.getFieldByName("iFilesDl", obj.iFilesDl);
    qry.getFieldByName("iFilesUlMb", obj.iFilesUlMb);
    qry.getFieldByName("iFilesDlMb", obj.iFilesDlMb);
}

/**
 * @brief (Callback) for Insert Statement translates to (Column, .. ) VALUES (%d, %Q,)
 * @param qry
 * @param obj
 * @param values
 */
void SessionStatsDao::fillSessionStatsColumnValues(Query &qry, SessionStats &obj,
                                                   std::vector<std::pair<std::string, std::string> > &values) {
    //values.push_back(qry.translateFieldName("iId", obj.iId));
    values.push_back(qry.translateFieldName("iUserId", obj.iUserId));
    values.push_back(qry.translateFieldName("sSessionType", obj.sSessionType));
    values.push_back(qry.translateFieldName("sCodePage", obj.sCodePage));
    values.push_back(qry.translateFieldName("sTerminal", obj.sTerminal));
    values.push_back(qry.translateFieldName("sIPAddress", obj.sIPAddress));
    values.push_back(qry.translateFieldName("iTermWidth", obj.iTermWidth));
    values.push_back(qry.translateFieldName("iTermHeight", obj.iTermHeight));
    values.push_back(qry.translateFieldName("dtStartDate", obj.dtStartDate));
    values.push_back(qry.translateFieldName("dtEndDate", obj.dtEndDate));
    values.push_back(qry.translateFieldName("iInvalidAttempts", obj.iInvalidAttempts));
    values.push_back(qry.translateFieldName("bNewUser", obj.bNewUser));
    values.push_back(qry.translateFieldName("bLogonSuccess", obj.bLogonSuccess));
    values.push_back(qry.translateFieldName("bHungup", obj.bHungup));
    values.push_back(qry.translateFieldName("iMsgRead", obj.iMsgRead));
    values.push_back(qry.translateFieldName("iMsgPost", obj.iMsgPost));
    values.push_back(qry.translateFieldName("iFilesUl", obj.iFilesUl));
    values.push_back(qry.translateFieldName("iFilesDl", obj.iFilesDl));
    values.push_back(qry.translateFieldName("iFilesUlMb", obj.iFilesUlMb));
    values.push_back(qry.translateFieldName("iFilesDlMb", obj.iFilesDlMb));
}

/**
 * @brief (Callback) Create Record Insert Statement, returns query string
 * @param qry
 * @param obj
 * @return
 */
std::string SessionStatsDao::insertSessionStatsQryString(std::string qry, SessionStats &obj) {
    // Mprint statement to avoid injections.
    char *result = sqlite3_mprintf(qry.c_str(),
                                   obj.iUserId,
                                   obj.sSessionType.c_str(),
                                   obj.sCodePage.c_str(),
                                   obj.sTerminal.c_str(),
                                   obj.sIPAddress.c_str(),
                                   obj.iTermWidth,
                                   obj.iTermHeight,
                                   obj.dtStartDate,
                                   obj.dtEndDate,
                                   obj.iInvalidAttempts,
                                   obj.bNewUser,
                                   obj.bLogonSuccess,
                                   obj.bHungup,
                                   obj.iMsgRead,
                                   obj.iMsgPost,
                                   obj.iFilesUl,
                                   obj.iFilesDl,
                                   obj.iFilesUlMb,
                                   obj.iFilesDlMb
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
std::string SessionStatsDao::updateSessionStatsQryString(std::string qry, SessionStats &obj) {
    // Mprint statement to avoid injections.
    char *result = sqlite3_mprintf(qry.c_str(),
                                   obj.iUserId,
                                   obj.sSessionType.c_str(),
                                   obj.sCodePage.c_str(),
                                   obj.sTerminal.c_str(),
                                   obj.sIPAddress.c_str(),
                                   obj.iTermWidth,
                                   obj.iTermHeight,
                                   obj.dtStartDate,
                                   obj.dtEndDate,
                                   obj.iInvalidAttempts,
                                   obj.bNewUser,
                                   obj.bLogonSuccess,
                                   obj.bHungup,
                                   obj.iMsgRead,
                                   obj.iMsgPost,
                                   obj.iFilesUl,
                                   obj.iFilesDl,
                                   obj.iFilesUlMb,
                                   obj.iFilesDlMb,
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
 * @brief Return List of All Users
 * @return
 */
std::vector<SessionStats> SessionStatsDao::getAllStatsPerUser(long userId) {
    UtilLog &log = UtilLog::getInstance();
    std::vector<SessionStats> list;

    // Make Sure Database Reference is Connected
    if (!m_database.isConnected()) {
        log.log(UtilLog::LogLevel::Info, "Error, Database is not connected!", m_strTableName, __LINE__, __FILE__);
        return list;
    }

    // Create Pointer and Connect Query Object to Database.
    Query qry(m_database);

    if (!qry.isConnected()) {
        log.log(UtilLog::LogLevel::Info, "Error, Query has no connection to the database", m_strTableName, __LINE__,
                                      __FILE__);
        return list;
    }

    // Build Query String
    char *result = sqlite3_mprintf("SELECT * FROM %Q WHERE iUserId = %ld;", m_strTableName.c_str(), userId);
    const std::string queryString(result);
    sqlite3_free(result);

    // Execute Query.
    if (qry.getResult(queryString)) {
        const long rows = qry.getNumRows();

        if (rows > 0) {
            while (qry.fetchRow()) {
                SessionStats stat;
                pullSessionStatsResult(qry, stat);
                list.push_back(stat);
            }
        } else {
            log.log(UtilLog::LogLevel::Info, "Error, getAllStatsPerUser Returned Rows=", rows, m_strTableName, __LINE__,
                                          __FILE__);
        }
    } else {
        log.log(UtilLog::LogLevel::Info, "Error, getResult()", m_strTableName, __LINE__, __FILE__);
    }

    return list;
}

/**
 * @brief Return List of All Users
 * @return
 */
std::vector<SessionStats> SessionStatsDao::getLast10CallerStats() {
    UtilLog &log = UtilLog::getInstance();
    std::vector<SessionStats> list;

    // Make Sure Database Reference is Connected
    if (!m_database.isConnected()) {
        log.log(UtilLog::LogLevel::Info, "Error, Database is not connected", m_strTableName, __LINE__, __FILE__);
        return list;
    }

    // Create Pointer and Connect Query Object to Database.
    Query qry(m_database);

    if (!qry.isConnected()) {
        log.log(UtilLog::LogLevel::Info, "Error, Query has no connection to the database", m_strTableName, __LINE__,
                                      __FILE__);
        return list;
    }

    // Build Query String
    char *result = sqlite3_mprintf("SELECT * FROM %Q WHERE bLogonSuccess = '1' "
                                   "ORDER BY iID DESC;", m_strTableName.c_str());

    const std::string queryString(result);
    sqlite3_free(result);

    // Execute Query.
    if (qry.getResult(queryString)) {
        const long rows = qry.getNumRows();

        if (rows > 0) {
            while (qry.fetchRow()) {
                SessionStats stat;
                pullSessionStatsResult(qry, stat);
                list.push_back(stat);
            }
        } else {
            log.log(UtilLog::LogLevel::Info, "Error, getLast10CallerStats Returned Rows=", rows, m_strTableName, __LINE__,
                                          __FILE__);
        }
    } else {
        log.log(UtilLog::LogLevel::Info, "Error, getResult()", m_strTableName, __LINE__, __FILE__);
    }

    return list;
}

/**
 * @brief Return List of All Connections Today
 * @return
 */
std::vector<SessionStats> SessionStatsDao::getTodaysCallerStats() {
    UtilLog &log = UtilLog::getInstance();
    std::vector<SessionStats> list;

    // Make Sure Database Reference is Connected
    if (!m_database.isConnected()) {
        log.log(UtilLog::LogLevel::Info, "Error, Database is not connected!", m_strTableName, __LINE__, __FILE__);
        return list;
    }

    // Create Pointer and Connect Query Object to Database.
    Query qry(m_database);

    if (!qry.isConnected()) {
        log.log(UtilLog::LogLevel::Info, "Error, Query has no connection to the database", m_strTableName, __LINE__,
                                      __FILE__);
        return list;
    }

    // Build Query String
    char *result = sqlite3_mprintf(
        "SELECT * FROM %Q "
        "WHERE datetime(dtStartDate, 'unixepoch', 'localtime') >= datetime('now','start of day') "
        "ORDER BY iID DESC;", m_strTableName.c_str());

    const std::string queryString(result);
    sqlite3_free(result);

    // Execute Query.
    if (qry.getResult(queryString)) {
        const long rows = qry.getNumRows();

        if (rows > 0) {
            while (qry.fetchRow()) {
                SessionStats stat;
                pullSessionStatsResult(qry, stat);
                list.push_back(stat);
            }
        } else {
            log.log(UtilLog::LogLevel::Info, "Error, getTodaysCallerStats Returned Rows=", rows, m_strTableName, __LINE__,
                                          __FILE__);
        }
    } else {
        log.log(UtilLog::LogLevel::Info, "Error, getResult()", m_strTableName, __LINE__, __FILE__);
    }

    return list;
}
