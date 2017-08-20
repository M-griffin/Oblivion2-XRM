#ifndef MESSAGE_AREA_DAO_HPP
#define MESSAGE_AREA_DAO_HPP

#include "../model-sys/message_area.hpp"
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
typedef BaseDao<MessageArea> baseMessageAreaClass;

/**
 * @class MessageAreaDao
 * @author Michael Griffin
 * @date 04/05/2017
 * @file message_area_dao.hpp
 * @brief Message Area Data Access Object
 */
class MessageAreaDao
    : public baseMessageAreaClass
{
public:
    
    explicit MessageAreaDao(SQLW::Database &database)
        : baseMessageAreaClass(database)
    {
        // Setup Table name
        m_strTableName = "messagearea";

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
            "sName             TEXT NOT NULL COLLATE NOCASE, "
            "sAcsAccess        TEXT NOT NULL COLLATE NOCASE, "
            "sAcsPost          TEXT NOT NULL COLLATE NOCASE, "
            "bAnonymous        BOOLEAN NOT NULL, "
            "sSponsor          TEXT NOT NULL COLLATE NOCASE, "
            "sOriginLine       TEXT NOT NULL COLLATE NOCASE, "
            "sFidoPath         TEXT NOT NULL COLLATE NOCASE, "
            "iNetworkId        INTEGER NOT NULL, "
            "sQwkName          TEXT NOT NULL COLLATE NOCASE, "
            "iMaxMessages      INTEGER NOT NULL, "
            "bRealName         BOOLEAN NOT NULL, "
            "sLinkname         TEXT NOT NULL COLLATE NOCASE, "
            "bRequired         BOOLEAN NOT NULL, "
            "bPrivate          BOOLEAN NOT NULL, "
            "bNetmail          BOOLEAN NOT NULL, "
            "iSortOrder        INTEGER NOT NULL "
            "); ";

        // CREATE INDEX `IDX_testtbl_Name` ON `testtbl` (`Name` COLLATE UTF8CI)
        m_cmdDropTable = "DROP TABLE IF EXISTS " + m_strTableName + "; ";
        
        // Setup the CallBack for Result Field Mapping
        m_result_callback = std::bind(&MessageAreaDao::pullMessageAreaResult, this, 
            std::placeholders::_1, std::placeholders::_2);
            
        m_columns_callback = std::bind(&MessageAreaDao::fillMessageAreaColumnValues, this, 
            std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
            
        m_insert_callback = std::bind(&MessageAreaDao::insertMessageAreaQryString, this, 
            std::placeholders::_1, std::placeholders::_2);
        
        m_update_callback = std::bind(&MessageAreaDao::updateMessageAreaQryString, this, 
            std::placeholders::_1, std::placeholders::_2);
    }

    ~MessageAreaDao()
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
    bool updateRecord(message_area_ptr obj);

    /**
     * @brief Inserts a New Record in the database!
     * @param obj
     * @return
     */
    long insertRecord(message_area_ptr obj);
        
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
    message_area_ptr getRecordById(long id);
    
    /**
     * @brief Retrieve All Records in a Table
     * @return
     */
    std::vector<message_area_ptr> getAllRecords();
    
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
    std::string insertMessageAreaQryString(std::string qry, message_area_ptr obj);

    /**
     * @brief (CallBack) Update Existing Record. 
     * @param qry
     * @param obj
     * @return 
     */
    std::string updateMessageAreaQryString(std::string qry, message_area_ptr obj);
    
    /**
     * @brief (CallBack) Pulls results by FieldNames into their Class Variables. 
     * @param qry
     * @param obj
     */
    void pullMessageAreaResult(query_ptr qry, message_area_ptr obj);

    /**
     * @brief (Callback) for Insert Statement translates to (Column, .. ) VALUES (%d, %Q,)
     * @param qry
     * @param obj
     * @param values
     */ 
    void fillMessageAreaColumnValues(query_ptr qry, message_area_ptr obj, 
        std::vector< std::pair<std::string, std::string> > &values);

    
    /**
     * One Off Methods SQL Queries not included in the BaseDao
     * (Below This Point)
     */
        
    
    /**
     * @brief Return List of All MessageArea by ConferenceId
     * @param areas
     * @return
     */ 
    std::vector<message_area_ptr> getAllMessageAreasByConference(long confId);
    
};

// Handle to Database Queries
typedef boost::shared_ptr<MessageAreaDao> msg_area_dao_ptr;

#endif // MESSAGE_AREA_DAO_HPP
