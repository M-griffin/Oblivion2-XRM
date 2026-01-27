#include "dao_network.hpp"

#include <string>
#include <sqlite3.h>

#include "../model-sys/network.hpp"

#include "libSqliteWrapped.h"

/**
 * @brief Check If Database Table Exists.
 * @return
 */
bool NetworkDao::doesTableExist() {
    return baseDoesTableExist();
}

/**
 * @brief Run Setup Params for SQL Database Table.
 */
bool NetworkDao::firstTimeSetupParams() {
    return baseFirstTimeSetupParams();
}

/**
 * @brief Create Database Table
 * @return
 */
bool NetworkDao::createTable() {
    return baseCreateTable();
}

/**
 * @brief Drop Database
 * @return
 */
bool NetworkDao::dropTable() {
    return baseDropTable();
}

/**
 * @brief Updates a Record in the database!
 * @param area
 * @return
 */
bool NetworkDao::updateRecord(Network &obj) {
    return baseUpdateRecord(obj);
}

/**
 * @brief Inserts a New Record in the database!
 * @param obj
 * @return
 */
long NetworkDao::insertRecord(Network &obj) {
    return baseInsertRecord(obj);
}

/**
 * @brief Deletes a Record
 * @param id
 * @return
 */
bool NetworkDao::deleteRecord(long id) {
    return baseDeleteRecord(id);
}

/**
 * @brief Retrieve Record By Id.
 * @param id
 * @return
 */
Network NetworkDao::getRecordById(long id) {
    return baseGetRecordById(id);
}

/**
 * @brief Retrieve All Records in a Table
 * @return
 */
std::vector<Network> NetworkDao::getAllRecords() {
    return baseGetAllRecords();
}

/**
 * @brief Retrieve Count of All Records in a Table
 * @return
 */
long NetworkDao::getRecordsCount() {
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
void NetworkDao::pullNetworkResult(Query &qry, Network &obj) {
    qry.getFieldByName("iId", obj.iId);
    qry.getFieldByName("sName", obj.sName);
    qry.getFieldByName("sType", obj.sType);
    qry.getFieldByName("sAddress", obj.sAddress);
}

/**
 * @brief (Callback) for Insert Statement translates to (Column, .. ) VALUES (%d, %Q,)
 * @param qry
 * @param obj
 * @param values
 */
void NetworkDao::fillNetworkColumnValues(Query &qry, Network &obj,
                                         std::vector<std::pair<std::string, std::string> > &values) {
    // values.push_back(qry.translateFieldName("iId", conf->iId));
    values.push_back(qry.translateFieldName("sName", obj.sName));
    values.push_back(qry.translateFieldName("sType", obj.sType));
    values.push_back(qry.translateFieldName("sAddress", obj.sAddress));
}

/**
 * @brief (Callback) Create Record Insert Statement, returns query string
 * @param qry
 * @param obj
 * @return
 */
std::string NetworkDao::insertNetworkQryString(std::string qry, Network &obj) {
    // Mprint statement to avoid injections.
    char *result = sqlite3_mprintf(qry.c_str(),
                                   obj.sName.c_str(),
                                   obj.sType.c_str(),
                                   obj.sAddress.c_str()
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
std::string NetworkDao::updateNetworkQryString(std::string qry, Network &obj) {
    // Mprint statement to avoid injections.
    char *result = sqlite3_mprintf(qry.c_str(),
                                   obj.sName.c_str(),
                                   obj.sType.c_str(),
                                   obj.sAddress.c_str(),
                                   obj.iId
    );

    std::string queryString(result);
    sqlite3_free(result);
    return queryString;
}
