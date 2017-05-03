#ifndef USERS_DAO_HPP
#define USERS_DAO_HPP

#include <boost/smart_ptr/shared_ptr.hpp>

#include <vector>

// Forward Declerations
namespace SQLW
{
class Database;
class Query;
}

class Users;
// Handles to Users
typedef boost::shared_ptr<Users> user_ptr;

// Handles to Database
typedef boost::shared_ptr<SQLW::Database> database_ptr;

// Handle to Database Queries
typedef boost::shared_ptr<SQLW::Query> query_ptr;

/**
 * @class UsersDao
 * @author Michael Griffin
 * @date 2/28/2016
 * @file users_dao.hpp
 * @brief Handles Reading and Writting Users Class from Database
 *        Database Objects are Instaniated in the Session then passed to the DAO for work.
 */
class UsersDao
{
public:
    UsersDao(SQLW::Database &database);
    ~UsersDao();

    // Handle to Database
    SQLW::Database &m_users_database;

    std::string strTableName;

    // Static Queries
    std::string cmdFirstTimeSetup;

    std::string cmdUserTableExists;
    std::string cmdCreateUserTable;
    std::string cmdCreateUserIndex;
    std::string cmdDropUserTable;
    std::string cmdDropUserIndex;

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
     * @brief Create Users Database
     * @return
     */
    bool createTable();

    /**
     * @brief Drop Users Database
     * @return
     */
    bool dropTable();

    /**
     * @brief Create Query String to Insert New User Record
     */
    std::string insertUserQryString(query_ptr qry, user_ptr user);

    /**
     * @brief Creates Query String to Update Existing User Record
     */
    std::string updateUserQryString(query_ptr qry, user_ptr user);

    /**
     * @brief Updates a User Record in the database!
     * @param user
     * @return
     */
    bool updateUserRecord(user_ptr user);

    /**
     * @brief Inserts a New User Record in the database!
     * @param user
     * @return
     */
    long insertUserRecord(user_ptr user);

    /**
     * @brief Deletes a User Record
     * @param userId
     * @return
     */
    bool deleteUserRecord(long userId);

    /**
     * @brief Helper To populate User Record with Query Results.
     */
    void pullUserResult(query_ptr qry, user_ptr user);


    /**
     * @brief This takes a pair, and translates to (Column, .. ) VALUES (%d, %Q,) for formatting
     * @param values
     */
    void fillColumnValues(query_ptr qry, user_ptr user, std::vector< std::pair<std::string, std::string> > &values);

    /**
     * @brief Return User Record By Id.
     * @return
     */
    user_ptr getUserById(long userId);

    /**
     * @brief Return User Record By Handle.
     * @return
     */
    user_ptr getUserByHandle(std::string name);

    /**
     * @brief Return User Record By RealName.
     * @return
     */
    user_ptr getUserByRealName(std::string name);

    /**
     * @brief Return User Record By Email.
     * @return
     */
    user_ptr getUserByEmail(std::string email);
    
    /**
     * @brief Return List of All Users
     * @return
     */
    std::vector<user_ptr> getAllUsers();

};

// Handle to Database Queries
typedef boost::shared_ptr<UsersDao> users_dao_ptr;

#endif // USERS_DAO_HPP
