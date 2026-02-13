#include "dao_security.hpp"

#include <string>
#include <sqlite3.h>

#include "../model-sys/security.hpp"

#include "libSqliteWrapped.h"

/**
 * @brief Check If Database Table Exists.
 * @return
 */
bool SecurityDao::doesTableExist() {
    return baseDoesTableExist();
}

/**
 * @brief Run Setup Params for SQL Database Table.
 */
bool SecurityDao::firstTimeSetupParams() {
    return baseFirstTimeSetupParams();
}

/**
 * @brief Create Database Table
 * @return
 */
bool SecurityDao::createTable() {
    return baseCreateTable();
}

/**
 * @brief Drop Database
 * @return
 */
bool SecurityDao::dropTable() {
    return baseDropTable();
}

/**
 * @brief Updates a Record in the database!
 * @param obj
 * @return
 */
bool SecurityDao::updateRecord(Security &obj) {
    return baseUpdateRecord(obj);
}

/**
 * @brief Inserts a New Record in the database!
 * @param obj
 * @return
 */
long SecurityDao::insertRecord(Security &obj) {
    return baseInsertRecord(obj);
}

/**
 * @brief Deletes a Record
 * @param id
 * @return
 */
bool SecurityDao::deleteRecord(long id) {
    return baseDeleteRecord(id);
}

/**
 * @brief Retrieve Record By Id.
 * @param id
 * @return
 */
Security SecurityDao::getRecordById(long id) {
    return baseGetRecordById(id);
}

/**
 * @brief Retrieve All Records in a Table
 * @return
 */
std::vector<Security> SecurityDao::getAllRecords() {
    return baseGetAllRecords();
}

/**
 * @brief Retrieve Count of All Records in a Table
 * @return
 */
long SecurityDao::getRecordsCount() {
    return baseGetRecordsCount();
}


/**
 * Base Dao Call Back for Object Specific Data Mappings
 * (Below This Point)
 */


/**
 * @brief Pulls results by FieldNames into their Class Variables.
 */
void SecurityDao::pullSecurityResult(Query &qry, Security &obj) {
    qry.getFieldByName("iId", obj.iId);
    qry.getFieldByName("sPasswordHash", obj.sPasswordHash);
    qry.getFieldByName("sChallengeQuestion", obj.sChallengeQuestion);
    qry.getFieldByName("sChallengeAnswerHash", obj.sChallengeAnswerHash);
}

/**
 * @brief Security for Insert Statement
 *        This takes a pair, and translates to (Column, .. ) VALUES (%d, %Q,) for formatting
 * @param values
 */
void SecurityDao::fillSecurityColumnValues(Query &qry, Security &obj,
                                           std::vector<std::pair<std::string, std::string> > &values) {
    // values.push_back(qry.translateFieldName("iId", obj.iId));
    values.push_back(qry.translateFieldName("sPasswordHash", obj.sPasswordHash));
    values.push_back(qry.translateFieldName("sChallengeQuestion", obj.sChallengeQuestion));
    values.push_back(qry.translateFieldName("sChallengeAnswerHash", obj.sChallengeAnswerHash));
}

/**
 * @brief Create Security Record Insert Statement, returns query string
 */
std::string SecurityDao::insertSecurityQryString(std::string qry, Security &obj) {
    // Mprint statement to avoid injections.
    char *result = sqlite3_mprintf(qry.c_str(),
                                   obj.sPasswordHash.c_str(),
                                   obj.sChallengeQuestion.c_str(),
                                   obj.sChallengeAnswerHash.c_str()
    );

    std::string queryString(result);
    sqlite3_free(result);
    return queryString;
}

/**
 * @brief Update Existing Security Record.
 */
std::string SecurityDao::updateSecurityQryString(std::string qry, Security &obj) {
    // Mprint statement to avoid injections.
    char *result = sqlite3_mprintf(qry.c_str(),
                                   obj.sPasswordHash.c_str(),
                                   obj.sChallengeQuestion.c_str(),
                                   obj.sChallengeAnswerHash.c_str(),
                                   obj.iId
    );

    std::string queryString(result);
    sqlite3_free(result);
    return queryString;
}
