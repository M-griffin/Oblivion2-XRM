#ifndef ACCESS_LEVEL_DAO_HPP
#define ACCESS_LEVEL_DAO_HPP

#include "../model-sys/access_level.hpp"

#include <memory>
#include <vector>
#include <functional>

#include "../data-sys/base_dao.hpp"

// Forward Decelerations
namespace SQLW
{
class Database;
class Query;
}

// Handle to Database Queries
typedef std::shared_ptr<SQLW::Query> query_ptr;

// Base Dao Definition
typedef BaseDao<AccessLevel> baseAccessLevelClass;

/**
 * @class AccessLevelDao
 * @author Michael Griffin
 * @date 14/05/2017
 * @file access_level_dao.hpp
 * @brief Data Access Objects for Access Levels
 */
class AccessLevelDao
    : public baseAccessLevelClass
{
public:
    explicit AccessLevelDao(SQLW::Database &database)
        : baseAccessLevelClass(database)
    {
        // Setup Table name
        m_strTableName = "accesslevel";

        /**
         * Pre Populate Static Queries one Time
         */
        m_cmdFirstTimeSetup =
            "PRAGMA synchronous=Normal; "
            "PRAGMA encoding=UTF-8; "
            "PRAGMA foreign_keys=ON; "
            "PRAGMA default_cache_size=10000; "
            "PRAGMA cache_size=10000; ";

        // Check if Database Exists.
        m_cmdTableExists = "SELECT name FROM sqlite_master WHERE type='table' AND name='" + m_strTableName + "' COLLATE NOCASE;";

        // Create Users Table Query (SQLite Only for the moment)
        m_cmdCreateTable =
            "CREATE TABLE IF NOT EXISTS " + m_strTableName + " ( "
            "iId               INTEGER PRIMARY KEY, "
            "sName             TEXT NOT NULL COLLATE NOCASE, "
            "sStartMenu        TEXT NOT NULL COLLATE NOCASE, "
            "iLevel            INTEGER NOT NULL, "
            "iFileLevel        INTEGER NOT NULL, "
            "iMessageLevel     INTEGER NOT NULL, "
            "iPostCallRatio    INTEGER NOT NULL, "
            "iFileRatio        INTEGER NOT NULL, "
            "iTimeLimit        INTEGER NOT NULL, "
            "iCallLimit        INTEGER NOT NULL, "
            "iDownloads        INTEGER NOT NULL, "
            "iDownloadMB       INTEGER NOT NULL, "
            "iARFlags1         INTEGER NOT NULL, "
            "iARFlags2         INTEGER NOT NULL, "
            "bPostCallRatio    BOOLEAN NOT NULL, "
            "bFileRatio        BOOLEAN NOT NULL, "
            "bTimeLimit        BOOLEAN NOT NULL, "
            "bCallLimit        BOOLEAN NOT NULL, "
            "bDownloads        BOOLEAN NOT NULL, "
            "bDownloadMB       BOOLEAN NOT NULL "
            "); ";


        // CREATE INDEX `IDX_testtbl_Name` ON `testtbl` (`Name` COLLATE UTF8CI)
        m_cmdDropTable = "DROP TABLE IF EXISTS " + m_strTableName + "; ";


        // Setup the CallBack for Result Field Mapping
        m_result_callback = std::bind(&AccessLevelDao::pullAccessLevelResult, this,
                                      std::placeholders::_1, std::placeholders::_2);

        m_columns_callback = std::bind(&AccessLevelDao::fillAccessLevelColumnValues, this,
                                       std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);

        m_insert_callback = std::bind(&AccessLevelDao::insertAccessLevelQryString, this,
                                      std::placeholders::_1, std::placeholders::_2);

        m_update_callback = std::bind(&AccessLevelDao::updateAccessLevelQryString, this,
                                      std::placeholders::_1, std::placeholders::_2);
    }

    ~AccessLevelDao()
    {
    }


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
    bool updateRecord(access_level_ptr obj);

    /**
     * @brief Inserts a New Record in the database!
     * @param obj
     * @return
     */
    long insertRecord(access_level_ptr obj);

    /**
     * @brief Deletes a MessageArea Record
     * @param areaId
     * @return
     */
    bool deleteRecord(long id);

    /**
     * @brief Retrieve Record By Id.
     * @param id
     * @return
     */
    access_level_ptr getRecordById(long id);

    /**
     * @brief Retrieve All Records in a Table
     * @return
     */
    std::vector<access_level_ptr> getAllRecords();

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
    std::string insertAccessLevelQryString(std::string qry, access_level_ptr obj);

    /**
     * @brief (CallBack) Update Existing Record.
     * @param qry
     * @param obj
     * @return
     */
    std::string updateAccessLevelQryString(std::string qry, access_level_ptr obj);

    /**
     * @brief (CallBack) Pulls results by FieldNames into their Class Variables.
     * @param qry
     * @param obj
     */
    void pullAccessLevelResult(query_ptr qry, access_level_ptr obj);

    /**
     * @brief (Callback) for Insert Statement translates to (Column, .. ) VALUES (%d, %Q,)
     * @param qry
     * @param obj
     * @param values
     */
    void fillAccessLevelColumnValues(query_ptr qry, access_level_ptr obj, std::vector< std::pair<std::string, std::string> > &values);


    /**
     * One Off Methods SQL Queries not included in the BaseDao
     * (Below This Point)
     */

    /**
     * @brief Return AccessLevel Record by Level.
     * @return
     */
    access_level_ptr getAccessLevelByLevel(long access_level);
};

// Handle to Database Queries
typedef std::shared_ptr<AccessLevelDao> access_level_dao_ptr;

#endif // ACCESS_LEVEL_DAO_HPP
