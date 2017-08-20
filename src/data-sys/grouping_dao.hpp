#ifndef GROUPING_DAO_HPP
#define GROUPING_DAO_HPP

#include "../model-sys/grouping.hpp"
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
typedef BaseDao<Grouping> baseGroupingClass;


/**
 * @class GroupingDao
 * @author Michael Griffin
 * @date 04/05/2017
 * @file grouping_dao.hpp
 * @brief Data Access Objects for Conference Grouping
 */
class GroupingDao
    : public baseGroupingClass
{
public:

    explicit GroupingDao(SQLW::Database &database)
        : baseGroupingClass(database)
    {
        // Setup Table name
        m_strTableName = "grouping";

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

        // Create Table Query (SQLite Only for the moment)
        m_cmdCreateTable =
            "CREATE TABLE IF NOT EXISTS " + m_strTableName + " ( "
            "iId               INTEGER PRIMARY KEY, "
            "iConferenceId     INTEGER NOT NULL, "
            "iAreaId           INTEGER NOT NULL, "
            "FOREIGN KEY(iConferenceId) REFERENCES Conference(iId) ON DELETE CASCADE "
            "); ";

        m_cmdCreateIndex = "";
            "CREATE INDEX IF NOT EXISTS grouping_idx "
            "ON " + m_strTableName + " (iConferenceId); ";

        // CREATE INDEX `IDX_testtbl_Name` ON `testtbl` (`Name` COLLATE UTF8CI)
        m_cmdDropTable = "DROP TABLE IF EXISTS " + m_strTableName + "; ";
        m_cmdDropIndex = "DROP INDEX IF EXISTS grouping_idx; ";
        
        // Setup the CallBack for Result Field Mapping
        m_result_callback = std::bind(&GroupingDao::pullGroupingResult, this, 
            std::placeholders::_1, std::placeholders::_2);
            
        m_columns_callback = std::bind(&GroupingDao::fillGroupingColumnValues, this, 
            std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
            
        m_insert_callback = std::bind(&GroupingDao::insertGroupingQryString, this, 
            std::placeholders::_1, std::placeholders::_2);
        
        m_update_callback = std::bind(&GroupingDao::updateGroupingQryString, this, 
            std::placeholders::_1, std::placeholders::_2);
    }

    ~GroupingDao()
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
    bool updateRecord(group_ptr obj);

    /**
     * @brief Inserts a New Record in the database!
     * @param obj
     * @return
     */
    long insertRecord(group_ptr obj);
        
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
    group_ptr getRecordById(long id);
    
    /**
     * @brief Retrieve All Records in a Table
     * @return
     */
    std::vector<group_ptr> getAllRecords();
    
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
     * @brief Create Query String to Insert New Conference Record
     */
    std::string insertGroupingQryString(std::string qry, group_ptr obj);

    /**
     * @brief Creates Query String to Update Existing Conference Record
     */
    std::string updateGroupingQryString(std::string qry, group_ptr obj);
   
    /**
     * @brief Helper To populate Conference Record with Query Results.
     */
    void pullGroupingResult(query_ptr qry, group_ptr obj);

    /**
     * @brief This takes a pair, and translates to (Column, .. ) VALUES (%d, %Q,) for formatting
     * @param values
     */
    void fillGroupingColumnValues(query_ptr qry, group_ptr obj, 
        std::vector< std::pair<std::string, std::string> > &values);


    /**
     * One Off Methods SQL Queries not included in the BaseDao
     * (Below This Point)
     */
    
   
    /**
     * @brief Return List of All Groupings by ConferenceId
     * @param confId
     * @return
     */ 
    std::vector<group_ptr> getAllGroupingsByConferenceId(long confId);
    
};

// Handle to Database Queries
typedef boost::shared_ptr<GroupingDao> group_dao_ptr;

#endif // GROUPING_DAO_HPP
