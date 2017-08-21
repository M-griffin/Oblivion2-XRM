#ifndef SESSION_STATS_DAO_HPP
#define SESSION_STATS_DAO_HPP

#include "../model-sys/session_stats.hpp"
#include "../data-sys/base_dao.hpp"
#include <boost/smart_ptr/shared_ptr.hpp>

#include <vector>
#include <functional>

// Forward Declerations
namespace SQLW
{
class Database;
class Query;
}

// Handle to Database Queries
typedef boost::shared_ptr<SQLW::Query> query_ptr;

// Base Dao Definition
typedef BaseDao<SessionStats> baseSessionStatsClass;


class SessionStatsDao
    : public baseSessionStatsClass
{
public:
    
    explicit SessionStatsDao(SQLW::Database &database)
        : baseSessionStatsClass(database)
    {
        // Setup Table name
        m_strTableName = "sessionstats";

        /**
         * Pre Popluate Static Queries one Time
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
            "iUserId           INTEGER NOT NULL, "
            "sSessionType      TEXT NOT NULL COLLATE NOCASE, "
            "sCodePage         TEXT NOT NULL COLLATE NOCASE, "
            "sTerminal         TEXT NOT NULL COLLATE NOCASE, "
            "sIPAddress        TEXT NOT NULL COLLATE NOCASE, "
            "iTermWidth        INTEGER NOT NULL, "
            "iTermHeight       INTEGER NOT NULL, "
            "dtStartDate       DATETIME NOT NULL, "
            "dtEndDate         DATETIME NOT NULL, "
            "iInvalidAttempts  INTEGER NOT NULL, "
            "bNewUser          BOOLEAN NOT NULL, "
            "bLogonSuccess     BOOLEAN NOT NULL, "
            "bHungup           BOOLEAN NOT NULL, "
            "iMsgRead          INTEGER NOT NULL, "
            "iMsgPost          INTEGER NOT NULL, "
            "iFilesUl          INTEGER NOT NULL, "
            "iFilesDl          INTEGER NOT NULL, "
            "iFilesUlMb        INTEGER NOT NULL, "
            "iFilesDlMb        INTEGER NOT NULL "
            "); ";

        m_cmdCreateIndex = "";
            "CREATE INDEX IF NOT EXISTS session_stats_idx "
            "ON " + m_strTableName + " (iUserId); ";

        // CREATE INDEX `IDX_testtbl_Name` ON `testtbl` (`Name` COLLATE UTF8CI)
        m_cmdDropTable = "DROP TABLE IF EXISTS " + m_strTableName + "; ";
        m_cmdDropIndex = "DROP INDEX IF EXISTS session_stats_idx; ";    
        
        // Setup the CallBack for Result Field Mapping
        m_result_callback = std::bind(&SessionStatsDao::pullSessionStatsResult, this, 
            std::placeholders::_1, std::placeholders::_2);
            
        m_columns_callback = std::bind(&SessionStatsDao::fillSessionStatsColumnValues, this, 
            std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
            
        m_insert_callback = std::bind(&SessionStatsDao::insertSessionStatsQryString, this, 
            std::placeholders::_1, std::placeholders::_2);
        
        m_update_callback = std::bind(&SessionStatsDao::updateSessionStatsQryString, this, 
            std::placeholders::_1, std::placeholders::_2);
    }

    ~SessionStatsDao()
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
    bool updateRecord(session_stats_ptr obj);

    /**
     * @brief Inserts a New Record in the database!
     * @param obj
     * @return
     */
    long insertRecord(session_stats_ptr obj);
        
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
    session_stats_ptr getRecordById(long id);
    
    /**
     * @brief Retrieve All Records in a Table
     * @return
     */
    std::vector<session_stats_ptr> getAllRecords();
    
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
    std::string insertSessionStatsQryString(std::string qry, session_stats_ptr obj);

    /**
     * @brief (CallBack) Update Existing Record. 
     * @param qry
     * @param obj
     * @return 
     */
    std::string updateSessionStatsQryString(std::string qry, session_stats_ptr obj);
   
    /**
     * @brief (CallBack) Pulls results by FieldNames into their Class Variables. 
     * @param qry
     * @param obj
     */
    void pullSessionStatsResult(query_ptr qry, session_stats_ptr obj);

    /**
     * @brief (Callback) for Insert Statement translates to (Column, .. ) VALUES (%d, %Q,)
     * @param qry
     * @param obj
     * @param values
     */ 
    void fillSessionStatsColumnValues(query_ptr qry, session_stats_ptr obj, std::vector< std::pair<std::string, std::string> > &values);


    /**
     * One Off Methods SQL Queries not included in the BaseDao
     * (Below This Point)
     */
    
    
    /**
     * @brief Return List of Stats per user
     * @return
     */
    std::vector<session_stats_ptr> getAllStatsPerUser(long userId);
   
    /**
     * @brief Return List of Last 10 Valid Sessions.
     * @return
     */
    std::vector<session_stats_ptr> getLast10CallerStats();
    
    /**
     * @brief Return List of All Connections Today
     * @return
     */
    std::vector<session_stats_ptr> getTodaysCallerStats();

};

// Handle to Database Queries
typedef boost::shared_ptr<SessionStatsDao> session_stats_dao_ptr;

#endif // SESSION_STATS_DAO_HPP
