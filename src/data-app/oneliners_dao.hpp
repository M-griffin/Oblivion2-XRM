#ifndef ONELINERS_DAO_HPP
#define ONELINERS_DAO_HPP

#include "../model-app/oneliners.hpp"
#include "../data-sys/base_dao.hpp"
#include <boost/smart_ptr/shared_ptr.hpp>

#include <vector>

// Forward Declerations
namespace SQLW
{
class Database;
class Query;
}

// Handles to Database
typedef boost::shared_ptr<SQLW::Database> database_ptr;

// Handle to Database Queries
typedef boost::shared_ptr<SQLW::Query> query_ptr;

// Base Dao Definition
typedef BaseDao<Oneliners> baseOnelinerClass;

/**
 * @class OnelinerDao
 * @author Michael Griffin
 * @date 05/05/2017
 * @file oneliners_dao.hpp
 * @brief One Liners Data Access Object
 */
class OnelinerDao
    : public baseOnelinerClass
{
public:
    
    OnelinerDao(SQLW::Database &database)
        : baseOnelinerClass(database) 
    {
        // Setup Table name
        m_strTableName = "oneliner";

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
            "iUserId           INTEGER NOT NULL, "
            "sText             TEXT NOT NULL COLLATE NOCASE, "
            "sUserName         TEXT NOT NULL COLLATE NOCASE, "
            "sUserInitials     TEXT NOT NULL COLLATE NOCASE, "
            "dtDatePosted      DATETIME DEFAULT CURRENT_TIMESTAMP, "
            "FOREIGN KEY(iUserId) REFERENCES User(iId) ON DELETE CASCADE "
            "); ";

        // CREATE INDEX `IDX_testtbl_Name` ON `testtbl` (`Name` COLLATE UTF8CI)
        m_cmdDropTable = "DROP TABLE IF EXISTS " + m_strTableName + "; ";
        
        // Setup the CallBack for Result Field Mapping
        m_result_function.push_back(std::bind(&OnelinerDao::pullOnelinerResult, this, 
            std::placeholders::_1, std::placeholders::_2));
            
        m_columns_function.push_back(std::bind(&OnelinerDao::fillOnelinerColumnValues, this, 
            std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
            
        m_insert_function.push_back(std::bind(&OnelinerDao::insertOnelinerQryString, this, 
            std::placeholders::_1, std::placeholders::_2));
        
        m_update_function.push_back(std::bind(&OnelinerDao::updateOnelinerQryString, this, 
            std::placeholders::_1, std::placeholders::_2));
    }

    ~OnelinerDao()
    {        
    }
   
   /**
     * @brief Check If Database Table Exists.
     * @return
     */
    bool doesTableExists();
    
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
     * @brief Updates a MessageArea Record in the database!
     * @param area
     * @return
     */
    bool updateRecord(oneliner_ptr obj);

    /**
     * @brief Inserts a New MessageArea Record in the database!
     * @param area
     * @return
     */
    long insertRecord(oneliner_ptr obj);

    /**
     * @brief Deletes a MessageArea Record
     * @param areaId
     * @return
     */
    bool deleteRecord(long id);      
    
    
    /**
     * @brief CallBack, Pulls results by FieldNames into their Class Variables. 
     * @param qry
     * @param obj
     */
    void pullOnelinerResult(query_ptr qry, oneliner_ptr obj);
    
    /**
     * @brief Used for Insert Statement translates to (Column, .. ) VALUES (%d, %Q,)
     * @param qry
     * @param obj
     * @param values
     */ 
    void fillOnelinerColumnValues(query_ptr qry, oneliner_ptr obj, 
        std::vector< std::pair<std::string, std::string> > &values);

    /**
     * @brief Create Oneliners Record Insert Statement, returns query string 
     * @param qry
     * @param obj
     * @return 
     */
    std::string insertOnelinerQryString(std::string qry, oneliner_ptr obj);

    /**
     * @brief Update Existing Oneliners Record. 
     * @param qry
     * @param obj
     * @return 
     */
    std::string updateOnelinerQryString(std::string qry, oneliner_ptr obj);

    /**
     * @brief Return All Oneliners Records By User ID.
     * @return
     */
    std::vector<oneliner_ptr> getAllOnelinersByUserId(long userId);
        
};

// Handle to Database Queries
typedef boost::shared_ptr<OnelinerDao> oneliner_dao_ptr;

#endif // ONELINERS_DAO_HPP
