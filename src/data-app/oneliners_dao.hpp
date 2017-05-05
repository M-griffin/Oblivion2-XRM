#ifndef ONELINERS_DAO_HPP
#define ONELINERS_DAO_HPP

#include <boost/smart_ptr/shared_ptr.hpp>

#include <vector>

// Forward Declerations
namespace SQLW
{
class Database;
class Query;
}

class Oneliners;
// Handles to MessageArea
typedef boost::shared_ptr<Oneliners> oneliner_ptr;

// Handles to Database
typedef boost::shared_ptr<SQLW::Database> database_ptr;

// Handle to Database Queries
typedef boost::shared_ptr<SQLW::Query> query_ptr;

/**
 * @class OnelinerDao
 * @author Michael Griffin
 * @date 05/05/2017
 * @file oneliners_dao.hpp
 * @brief One Liners Data Access Object
 */
class OnelinerDao
{
public:
    OnelinerDao(SQLW::Database &database);
    ~OnelinerDao();
    
    // Handle to Database
    SQLW::Database &m_oneliner_database;

    std::string strTableName;

    std::string cmdFirstTimeSetup;

    std::string cmdOnelinerTableExists;
    // Static Queries
    std::string cmdCreateOnelinerTable;
    std::string cmdDropOnelinerTable;

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
    std::string insertOnelinerQryString(query_ptr qry, oneliner_ptr area);

    /**
     * @brief Creates Query String to Update Existing MessageArea Record
     */
    std::string updateOnelinerQryString(query_ptr qry, oneliner_ptr area);

    /**
     * @brief Updates a MessageArea Record in the database!
     * @param area
     * @return
     */
    bool updateOnelinerRecord(oneliner_ptr area);

    /**
     * @brief Inserts a New MessageArea Record in the database!
     * @param area
     * @return
     */
    long insertOnelinerRecord(oneliner_ptr area);

    /**
     * @brief Deletes a MessageArea Record
     * @param areaId
     * @return
     */
    bool deleteOnelinerRecord(long areaId);

    /**
     * @brief Helper To populate MessageArea Record with Query Results.
     */
    void pullOnelinerResult(query_ptr qry, oneliner_ptr area);

    /**
     * @brief This takes a pair, and translates to (Column, .. ) VALUES (%d, %Q,) for formatting
     * @param values
     */
    void fillColumnValues(query_ptr qry, oneliner_ptr area, 
        std::vector< std::pair<std::string, std::string> > &values);

    /**
     * @brief Return MessageArea Record By Id.
     * @return
     */
    oneliner_ptr getOnelinerById(long confId);
    
    /**
     * @brief Return All Oneliners Records By User ID.
     * @return
     */
    std::vector<oneliner_ptr> getAllOnelinersByUserId(long userId);

    /**
     * @brief Return List of All MessageAreas
     * @return
     */
    std::vector<oneliner_ptr> getAllOneliners();
        
};

// Handle to Database Queries
typedef boost::shared_ptr<OnelinerDao> oneliner_dao_ptr;

#endif // ONELINERS_DAO_HPP
