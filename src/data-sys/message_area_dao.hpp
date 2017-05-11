#ifndef MESSAGE_AREA_DAO_HPP
#define MESSAGE_AREA_DAO_HPP

#include "../model-sys/file_area.hpp"
#include "../data-sys/base_dao.hpp"
#include <boost/smart_ptr/shared_ptr.hpp>

#include <vector>

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
    MessageAreaDao(SQLW::Database &database)
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
        m_cmdMessageAreaTableExists = "SELECT name FROM sqlite_master WHERE type='table' AND name='" + m_strTableName + "' COLLATE NOCASE;";

        // Create Table Query (SQLite Only for the moment)
        m_cmdCreateMessageAreaTable =
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
        m_cmdDropMessageAreaTable = "DROP TABLE IF EXISTS " + m_strTableName + "; ";
        
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
     * @brief Check of the Database Exists.
     * @return
     */
    bool isTableExists();

    /**
     * @brief Run Setup Params for SQL Database.
     */
    bool firstTimeSetupParams();

    /**
     * @brief Create MessageArea Database
     * @return
     */
    bool createTable();

    /**
     * @brief Drop MessageArea Database
     * @return
     */
    bool dropTable();

    /**
     * @brief Create Query String to Insert New MessageArea Record
     */
    std::string insertMessageAreaQryString(query_ptr qry, msg_area_ptr area);

    /**
     * @brief Creates Query String to Update Existing MessageArea Record
     */
    std::string updateMessageAreaQryString(query_ptr qry, msg_area_ptr area);

    /**
     * @brief Updates a MessageArea Record in the database!
     * @param area
     * @return
     */
    bool updateMessageAreaRecord(msg_area_ptr area);

    /**
     * @brief Inserts a New MessageArea Record in the database!
     * @param area
     * @return
     */
    long insertMessageAreaRecord(msg_area_ptr area);

    /**
     * @brief Deletes a MessageArea Record
     * @param areaId
     * @return
     */
    bool deleteMessageAreaRecord(long areaId);

    /**
     * @brief Helper To populate MessageArea Record with Query Results.
     */
    void pullMessageAreaResult(query_ptr qry, msg_area_ptr area);

    /**
     * @brief This takes a pair, and translates to (Column, .. ) VALUES (%d, %Q,) for formatting
     * @param values
     */
    void fillColumnValues(query_ptr qry, msg_area_ptr area, 
        std::vector< std::pair<std::string, std::string> > &values);

    /**
     * @brief Return MessageArea Record By Id.
     * @return
     */
    msg_area_ptr getMessageAreaById(long confId);
    
    /**
     * @brief Return List of All MessageAreas
     * @return
     */
    std::vector<msg_area_ptr> getAllMessageAreas();
    
    /**
     * @brief Return List of All MessageArea by ConferenceId
     * @param areas
     * @return
     */ 
    std::vector<msg_area_ptr> getAllMessageAreasByConference(long confId);
    
};

// Handle to Database Queries
typedef boost::shared_ptr<MessageAreaDao> msg_area_dao_ptr;

#endif // MESSAGE_AREA_DAO_HPP
