#include "file_area_dao.hpp"

#include <string>
#include <sqlite3.h>

#include "../model-sys/file_area.hpp"
#include "../logging.hpp"

#include "libSqliteWrapped.h"

/**
 * @brief Check If Database Table Exists.
 * @return
 */
bool FileAreaDao::doesTableExist() {
    return baseDoesTableExist();
}

/**
 * @brief Run Setup Params for SQL Database Table.
 */
bool FileAreaDao::firstTimeSetupParams() {
    return baseFirstTimeSetupParams();
}

/**
 * @brief Create Database Table
 * @return
 */
bool FileAreaDao::createTable() {
    return baseCreateTable();
}

/**
 * @brief Drop Database
 * @return
 */
bool FileAreaDao::dropTable() {
    return baseDropTable();
}

/**
 * @brief Updates a Record in the database!
 * @param obj
 * @return
 */
bool FileAreaDao::updateRecord(FileArea &obj) {
    return baseUpdateRecord(obj);
}

/**
 * @brief Inserts a New Record in the database!
 * @param obj
 * @return
 */
long FileAreaDao::insertRecord(FileArea &obj) {
    return baseInsertRecord(obj);
}

/**
 * @brief Deletes a Record
 * @param id
 * @return
 */
bool FileAreaDao::deleteRecord(long id) {
    return baseDeleteRecord(id);
}

/**
 * @brief Retrieve Record By Id.
 * @param id
 * @return
 */
FileArea FileAreaDao::getRecordById(long id) {
    return baseGetRecordById(id);
}

/**
 * @brief Retrieve All Records in a Table
 * @return
 */
std::vector<FileArea> FileAreaDao::getAllRecords() {
    return baseGetAllRecords();
}

/**
 * @brief Retrieve Count of All Records in a Table
 * @return
 */
long FileAreaDao::getRecordsCount() {
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
void FileAreaDao::pullFileAreaResult(Query &qry, FileArea &obj) {
    qry.getFieldByName("iId", obj.iId);
    qry.getFieldByName("sName", obj.sName);
    qry.getFieldByName("sAcsAccess", obj.sAcsAccess);
    qry.getFieldByName("sAcsUpload", obj.sAcsUpload);
    qry.getFieldByName("sAcsDownload", obj.sAcsDownload);
    qry.getFieldByName("sAcsList", obj.sAcsList);
    qry.getFieldByName("sSponsor", obj.sSponsor);
    qry.getFieldByName("iSecurityIndex", obj.iSecurityIndex);
    qry.getFieldByName("sLinkname", obj.sLinkname);
    qry.getFieldByName("sSort", obj.sSort);
    qry.getFieldByName("iMultiplier", obj.iMultiplier);
    qry.getFieldByName("bFreeArea", obj.bFreeArea);
    qry.getFieldByName("iSortOrder", obj.iSortOrder);
}

/**
 * @brief (Callback) for Insert Statement translates to (Column, .. ) VALUES (%d, %Q,)
 * @param qry
 * @param obj
 * @param values
 */
void FileAreaDao::fillFileAreaColumnValues(Query &qry, FileArea &obj,
                                           std::vector<std::pair<std::string, std::string> > &values) {
    // values.push_back(qry.translateFieldName("iId", conf->iId));
    values.push_back(qry.translateFieldName("sName", obj.sName));
    values.push_back(qry.translateFieldName("sAcsAccess", obj.sAcsAccess));
    values.push_back(qry.translateFieldName("sAcsUpload", obj.sAcsUpload));
    values.push_back(qry.translateFieldName("sAcsDownload", obj.sAcsDownload));
    values.push_back(qry.translateFieldName("sAcsList", obj.sAcsList));
    values.push_back(qry.translateFieldName("sSponsor", obj.sSponsor));
    values.push_back(qry.translateFieldName("iSecurityIndex", obj.iSecurityIndex));
    values.push_back(qry.translateFieldName("sLinkname", obj.sLinkname));
    values.push_back(qry.translateFieldName("sSort", obj.sSort));
    values.push_back(qry.translateFieldName("iMultiplier", obj.iMultiplier));
    values.push_back(qry.translateFieldName("bFreeArea", obj.bFreeArea));
    values.push_back(qry.translateFieldName("iSortOrder", obj.iSortOrder));
}

/**
 * @brief (Callback) Create Record Insert Statement, returns query string
 * @param qry
 * @param obj
 * @return
 */
std::string FileAreaDao::insertFileAreaQryString(std::string qry, FileArea &obj) {
    // Mprint statement to avoid injections.
    char *result = sqlite3_mprintf(qry.c_str(),
                                   obj.sName.c_str(),
                                   obj.sAcsAccess.c_str(),
                                   obj.sAcsUpload.c_str(),
                                   obj.sAcsDownload.c_str(),
                                   obj.sAcsList.c_str(),
                                   obj.sSponsor.c_str(),
                                   obj.iSecurityIndex,
                                   obj.sLinkname.c_str(),
                                   obj.sSort.c_str(),
                                   obj.iMultiplier,
                                   obj.bFreeArea,
                                   obj.iSortOrder
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
std::string FileAreaDao::updateFileAreaQryString(std::string qry, FileArea &obj) {
    // Mprint statement to avoid injections.
    char *result = sqlite3_mprintf(qry.c_str(),
                                   obj.sName.c_str(),
                                   obj.sAcsAccess.c_str(),
                                   obj.sAcsUpload.c_str(),
                                   obj.sAcsDownload.c_str(),
                                   obj.sAcsList.c_str(),
                                   obj.sSponsor.c_str(),
                                   obj.iSecurityIndex,
                                   obj.sLinkname.c_str(),
                                   obj.sSort.c_str(),
                                   obj.iMultiplier,
                                   obj.bFreeArea,
                                   obj.iSortOrder,
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
 * @brief Return List of All FileArea by ConferenceId
 * @param id
 * @return
 */
std::vector<FileArea> FileAreaDao::getAllFileAreasByConference(long id) {
    Logging &log = Logging::getInstance();
    std::vector<FileArea> list;

    // Make Sure Database Reference is Connected
    if (!m_database.isConnected()) {
        log.write<Logging::ERROR_LOG>("Error, Database is not connected!", m_strTableName, __LINE__, __FILE__);
        return list;
    }

    // Create Pointer and Connect Query Object to Database.
    Query qry(m_database);

    if (!qry.isConnected()) {
        log.write<Logging::ERROR_LOG>("Error, Query has no connection to the database", m_strTableName, __LINE__,
                                      __FILE__);
        return list;
    }

    // Build Query String
    char *result = sqlite3_mprintf(
        "SELECT a.* FROM %Q a, Grouping g WHERE g.iConferenceId = %ld AND a.iID = g.iFileAreaId;",
        m_strTableName.c_str(), id);
    std::string queryString(result);
    sqlite3_free(result);

    // Execute Query.
    if (qry.getResult(queryString)) {
        long rows = qry.getNumRows();

        if (rows > 0) {
            while (qry.fetchRow()) {
                FileArea area;
                pullFileAreaResult(qry, area);
                list.push_back(area);
            }
        } else {
            log.write<Logging::ERROR_LOG>("Error, getAllFileAreasByConference Returned Rows=", rows, m_strTableName,
                                          __LINE__, __FILE__);
        }
    } else {
        log.write<Logging::ERROR_LOG>("Error, getResult()", m_strTableName, __LINE__, __FILE__);
    }

    return list;
}
