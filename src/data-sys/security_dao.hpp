#ifndef SECURITY_DAO_HPP
#define SECURITY_DAO_HPP

#include "../model-sys/security.hpp"
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
typedef BaseDao<Security> baseSecurityClass;

/**
 * @class SecurityDao
 * @author Michael Griffin
 * @date 8/21/2016
 * @file security_dao.hpp
 * @brief Handles Reading and Writting Security Class from Database
 *        Database Objects are Instaniated in the Session then passed to the DAO for work.
 */
class SecurityDao
    : baseSecurityClass
{
public:
        
    explicit SecurityDao(SQLW::Database &database)
        : baseSecurityClass(database)
    {
        // Setup Table name
        m_strTableName = "security";

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
        m_cmdTableExists = "SELECT name FROM sqlite_master WHERE type='table' "
            "AND name='" + m_strTableName + "' COLLATE NOCASE;";

        // Crate Seciroty Table Query (SQLite Only for the moment)
        m_cmdCreateTable =
            "CREATE TABLE IF NOT EXISTS " + m_strTableName + " ( "
            "iId                   INTEGER PRIMARY KEY, "
            "sPasswordHash         TEXT NOT NULL, "
            "sSaltHash             TEXT NOT NULL, "
            "sChallengeQuestion    TEXT NOT NULL, "
            "sChallengeAnswerHash  TEXT NOT NULL "
            "); ";

        // Drops and cleanup security table.
        m_cmdDropTable = "DROP TABLE IF EXISTS " + m_strTableName + "; ";
        
        // Setup the CallBack for Result Field Mapping
        m_result_callback = std::bind(&SecurityDao::pullSecurityResult, this, 
            std::placeholders::_1, std::placeholders::_2);
            
        m_columns_callback = std::bind(&SecurityDao::fillSecurityColumnValues, this, 
            std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
            
        m_insert_callback = std::bind(&SecurityDao::insertSecurityQryString, this, 
            std::placeholders::_1, std::placeholders::_2);
        
        m_update_callback = std::bind(&SecurityDao::updateSecurityQryString, this, 
            std::placeholders::_1, std::placeholders::_2);
    }

    ~SecurityDao()
    {
        std::cout << "~SecurityDao" << std::endl;
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
    bool updateRecord(security_ptr obj);

    /**
     * @brief Inserts a New Record in the database!
     * @param obj
     * @return
     */
    long insertRecord(security_ptr obj);
        
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
    security_ptr getRecordById(long id);
    
    /**
     * @brief Retrieve All Records in a Table
     * @return
     */
    std::vector<security_ptr> getAllRecords();
    
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
    std::string insertSecurityQryString(std::string qry, security_ptr obj);

    /**
     * @brief (CallBack) Update Existing Record. 
     * @param qry
     * @param obj
     * @return 
     */
    std::string updateSecurityQryString(std::string qry, security_ptr obj);
   
    /**
     * @brief (CallBack) Pulls results by FieldNames into their Class Variables. 
     * @param qry
     * @param obj
     */
    void pullSecurityResult(query_ptr qry, security_ptr obj);

    /**
     * @brief (Callback) for Insert Statement translates to (Column, .. ) VALUES (%d, %Q,)
     * @param qry
     * @param obj
     * @param values
     */ 
    void fillSecurityColumnValues(query_ptr qry, security_ptr obj, std::vector< std::pair<std::string, std::string> > &values);

};

// Handle to Database Queries
typedef boost::shared_ptr<SecurityDao> security_dao_ptr;

#endif // USERS_DAO_HPP
