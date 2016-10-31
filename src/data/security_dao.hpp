#ifndef SECURITY_DAO_HPP
#define SECURITY_DAO_HPP

#include "../model/security.hpp"

#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/smart_ptr/weak_ptr.hpp>

#include <vector>

// Forward Declerations
namespace SQLW
{
class Database;
class Query;
}


// Handles to Database
typedef boost::shared_ptr<SQLW::Database> database_ptr;
typedef boost::weak_ptr<SQLW::Database> database_weak_ptr;

// Handle to Database Queries
typedef boost::shared_ptr<SQLW::Query> query_ptr;

/**
 * @class SecurityDao
 * @author Michael Griffin
 * @date 8/21/2016
 * @file security_dao.hpp
 * @brief Handles Reading and Writting Security Class from Database
 *        Database Objects are Instaniated in the Session then passed to the DAO for work.
 */
class SecurityDao
{
public:
    SecurityDao(SQLW::Database &database);
    ~SecurityDao();

    // Handle to Database
    SQLW::Database &m_security_database;

    // Table Name
    std::string strTableName;

    // Static Queries
    std::string cmdFirstTimeSetup;

    std::string cmdSecurityTableExists;
    std::string cmdCreateSecurityTable;
    std::string cmdDropSecurityTable;


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
     * @brief Create Security Database
     * @return
     */
    bool createTable();

    /**
     * @brief Drop Security Database
     * @return
     */
    bool dropTable();

    /**
     * @brief Create Query String to Insert New Security Record
     */
    std::string insertSecurityQryString(query_ptr qry, security_ptr security);

    /**
     * @brief Creates QueryString to Update Security Record
     */
    std::string updateSecurityQryString(query_ptr qry, security_ptr security);

    /**
     * @brief Updates a Security Record in the database!
     * @param security
     * @return
     */
    bool updateSecurityRecord(security_ptr security);

    /**
     * @brief Inserts a New Security Record in the database!
     * @param security
     * @return
     */
    long insertSecurityRecord(security_ptr security);

    /**
     * @brief Deletes a Security Record
     * @param securityId
     * @return
     */
    bool deleteSecurityRecord(long securityId);

    /**
     * @brief Helper To populate Security Record with Query Results.
     */
    void pullSecurityResult(query_ptr qry, security_ptr security);


    /**
     * @brief This takes a pair, and translates to (Column, .. ) VALUES (%d, %Q,) for formatting
     * @param values
     */
    void fillColumnValues(query_ptr qry, security_ptr security, std::vector< std::pair<std::string, std::string> > &values);

    /**
     * @brief Returns Security Record By Id
     * @return
     */
    security_ptr getSecurityById(long securityId);

};

// Handle to Database Queries
typedef boost::shared_ptr<SecurityDao> security_dao_ptr;

#endif // USERS_DAO_HPP
