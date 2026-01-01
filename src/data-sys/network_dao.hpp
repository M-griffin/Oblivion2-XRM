#ifndef NETWORK_DAO_HPP
#define NETWORK_DAO_HPP

#include <vector>
#include <functional>

#include "../model-sys/network.hpp"
#include "../data-sys/base_dao.hpp"

// Base Dao Definition
typedef BaseDao<Network> baseNetworkClass;

/**
 * @class NetworkDao
 * @author Michael Griffin
 * @date 05/05/2017
 * @file network_dao.hpp
 * @brief Handles Message Fido and Internet Network Address
 */
class NetworkDao
        : public baseNetworkClass {
public:
    explicit NetworkDao(Database &database)
        : baseNetworkClass(database) {
        // Setup Table name
        m_strTableName = "network";

        /**
         * Pre Populate Static Queries one Time
         */
        m_cmdFirstTimeSetup =
                "PRAGMA synchronous=Normal; "
                "PRAGMA encoding=UTF-8; "
                "PRAGMA foreign_keys=ON; "
                "PRAGMA default_cache_size=10000; "
                "PRAGMA cache_size=10000; "
                "PRAGMA journal_mode = WAL; "
                "PRAGMA temp_store = MEMORY; "
                "PRAGMA mmap_size = 268435456; ";

        // Check if Database Exists.
        m_cmdTableExists = "SELECT name FROM sqlite_master WHERE type='table' AND name='" + m_strTableName +
                           "' COLLATE NOCASE;";

        // Create Table Query (SQLite Only for the moment)
        m_cmdCreateTable =
                "CREATE TABLE IF NOT EXISTS " + m_strTableName + " ( "
                "iId               INTEGER PRIMARY KEY, "
                "sName             TEXT NOT NULL COLLATE NOCASE, "
                "sType             TEXT NOT NULL COLLATE NOCASE, "
                "sAddress          TEXT NOT NULL COLLATE NOCASE "
                "); ";

        // CREATE INDEX `IDX_testtbl_Name` ON `testtbl` (`Name` COLLATE UTF8CI)
        m_cmdDropTable = "DROP TABLE IF EXISTS " + m_strTableName + "; ";

        // Setup the CallBack for Result Field Mapping
        m_result_callback = std::bind(&NetworkDao::pullNetworkResult, this,
                                      std::placeholders::_1, std::placeholders::_2);

        m_columns_callback = std::bind(&NetworkDao::fillNetworkColumnValues, this,
                                       std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);

        m_insert_callback = std::bind(&NetworkDao::insertNetworkQryString, this,
                                      std::placeholders::_1, std::placeholders::_2);

        m_update_callback = std::bind(&NetworkDao::updateNetworkQryString, this,
                                      std::placeholders::_1, std::placeholders::_2);
    }

    ~NetworkDao() = default;

    /**
     * Base Dao Calls for generic Object Data Calls
     * (Below This Point)
     */


    /**
     * @brief Check If Database Table Exists.
     * @return
     */
    bool doesTableExist();

    /**
     * @brief Run Setup Params for SQL Database Table.
     */
    bool firstTimeSetupParams();

    /**
     * @brief Create Database Table
     * @return
     */
    bool createTable();

    /**
     * @brief Drop Database
     * @return
     */
    bool dropTable();

    /**
     * @brief Updates a Record in the database!
     * @param obj
     * @return
     */
    bool updateRecord(Network &obj);

    /**
     * @brief Inserts a New Record in the database!
     * @param obj
     * @return
     */
    long insertRecord(Network &obj);

    /**
     * @brief Deletes a MessageArea Record
     * @param id
     * @return
     */
    bool deleteRecord(long id);

    /**
     * @brief Retrieve Record By Id.
     * @param id
     * @return
     */
    Network getRecordById(long id);

    /**
     * @brief Retrieve All Records in a Table
     * @return
     */
    std::vector<Network> getAllRecords();

    /**
     * @brief Retrieve Count of All Records in a Table
     * @return
     */
    long getRecordsCount();


    /**
     * Base Dao Call Back for Object Specific Data Mappings
     * (Below This Point)
     */


    /**
     * @brief (Callback) Create Record Insert Statement, returns query string
     * @param qry
     * @param obj
     * @return
     */
    std::string insertNetworkQryString(std::string qry, Network &obj);

    /**
     * @brief (CallBack) Update Existing Record.
     * @param qry
     * @param obj
     * @return
     */
    std::string updateNetworkQryString(std::string qry, Network &obj);

    /**
     * @brief (CallBack) Pulls results by FieldNames into their Class Variables.
     * @param qry
     * @param obj
     */
    void pullNetworkResult(Query &qry, Network &obj);

    /**
     * @brief (Callback) for Insert Statement translates to (Column, .. ) VALUES (%d, %Q,)
     * @param qry
     * @param obj
     * @param values
     */
    void fillNetworkColumnValues(Query &qry, Network &obj,
                                 std::vector<std::pair<std::string, std::string> > &values);
};

#endif
