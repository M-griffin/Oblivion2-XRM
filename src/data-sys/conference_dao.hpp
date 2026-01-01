#ifndef CONFERENCE_DAO_HPP
#define CONFERENCE_DAO_HPP

#include <vector>
#include <functional>

#include "../model-sys/conference.hpp"
#include "../data-sys/base_dao.hpp"


// Base Dao Definition
typedef BaseDao<Conference> baseConferenceClass;

/**
 * @class ConferenceDao
 * @author Michael Griffin
 * @date 03/05/2017
 * @file conference_dao.hpp
 * @brief Conference Data Access Object
 */
class ConferenceDao
        : public baseConferenceClass {
public:
    explicit ConferenceDao(Database &database)
        : baseConferenceClass(database) {
        // Setup Table name
        m_strTableName = "conference";

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
                "sACS              TEXT NOT NULL COLLATE NOCASE, "
                "iSortOrder        INTEGER NOT NULL "
                "); ";

        m_cmdCreateIndex =
                "CREATE INDEX IF NOT EXISTS conference_idx "
                "ON " + m_strTableName + " (sType COLLATE NOCASE); ";

        // CREATE INDEX `IDX_testtbl_Name` ON `testtbl` (`Name` COLLATE UTF8CI)
        m_cmdDropTable = "DROP TABLE IF EXISTS " + m_strTableName + "; ";

        m_cmdDropIndex = "DROP INDEX IF EXISTS conference_idx; ";

        // Set up the CallBack for Result Field Mapping
        m_result_callback = std::bind(&ConferenceDao::pullConferenceResult, this,
                                      std::placeholders::_1, std::placeholders::_2);

        m_columns_callback = std::bind(&ConferenceDao::fillConferenceColumnValues, this,
                                       std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);

        m_insert_callback = std::bind(&ConferenceDao::insertConferenceQryString, this,
                                      std::placeholders::_1, std::placeholders::_2);

        m_update_callback = std::bind(&ConferenceDao::updateConferenceQryString, this,
                                      std::placeholders::_1, std::placeholders::_2);
    }

    ~ConferenceDao() = default;

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
    bool updateRecord(Conference &obj);

    /**
     * @brief Inserts a New Record in the database!
     * @param obj
     * @return
     */
    long insertRecord(Conference &obj);

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
    Conference getRecordById(long id);

    /**
     * @brief Retrieve All Records in a Table
     * @return
     */
    std::vector<Conference> getAllRecords();

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
     * @brief (CallBack) Pulls results by FieldNames into their Class Variables.
     * @param qry
     * @param obj
     */
    void pullConferenceResult(Query &qry, Conference &obj);

    /**
     * @brief (CallBack) Insert Statement translates to (Column, .. ) VALUES (%d, %Q,)
     * @param qry
     * @param obj
     * @param values
     */
    void fillConferenceColumnValues(Query &qry, Conference &obj,
                                    std::vector<std::pair<std::string, std::string> > &values);

    /**
     * @brief (Callback) Create Record Insert Statement, returns query string
     * @param qry
     * @param obj
     * @return
     */
    std::string insertConferenceQryString(std::string qry, Conference &obj);

    /**
     * @brief (CallBack) Update Existing Record.
     * @param qry
     * @param obj
     * @return
     */
    std::string updateConferenceQryString(std::string qry, Conference &obj);


    /**
     * One Off Methods SQL Queries not included in the BaseDao
     * (Below This Point)
     */


    /**
     * @brief Return List of All Conference by Type
     * @param type
     * @return
     */
    std::vector<Conference> getAllConferencesByType(std::string type);

    /**
     * @brief Return Count or Number of Existing Conferences by Type
     * @param type
     * @return
     */
    long getConferencesCountByType(std::string type);
};

#endif
