#ifndef GROUPING_DAO_HPP
#define GROUPING_DAO_HPP

#include <boost/smart_ptr/shared_ptr.hpp>

#include <vector>

// Forward Declerations
namespace SQLW
{
class Database;
class Query;
}

class Grouping;
// Handles to Conference
typedef boost::shared_ptr<Grouping> group_ptr;

// Handles to Database
typedef boost::shared_ptr<SQLW::Database> database_ptr;

// Handle to Database Queries
typedef boost::shared_ptr<SQLW::Query> query_ptr;


/**
 * @class GroupingDao
 * @author Michael Griffin
 * @date 04/05/2017
 * @file grouping_dao.hpp
 * @brief Data Access Objects for Conference Grouping
 */
class GroupingDao
{
public:
    GroupingDao(SQLW::Database &database);
    ~GroupingDao();

    // Handle to Database
    SQLW::Database &m_group_database;

    std::string strTableName;

    // Static Queries
    std::string cmdFirstTimeSetup;

    std::string cmdGroupingTableExists;
    std::string cmdCreateGroupingTable;
    std::string cmdCreateGroupingIndex;
    std::string cmdDropGroupingTable;
    std::string cmdDropGroupingIndex;

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
     * @brief Create Conference Database
     * @return
     */
    bool createTable();

    /**
     * @brief Drop Conference Database
     * @return
     */
    bool dropTable();

    /**
     * @brief Create Query String to Insert New Conference Record
     */
    std::string insertGroupingQryString(query_ptr qry, group_ptr group);

    /**
     * @brief Creates Query String to Update Existing Conference Record
     */
    std::string updateGroupingQryString(query_ptr qry, group_ptr group);

    /**
     * @brief Updates a Conference Record in the database!
     * @param group
     * @return
     */
    bool updateGroupingRecord(group_ptr group);

    /**
     * @brief Inserts a New Conference Record in the database!
     * @param group
     * @return
     */
    long insertGroupingRecord(group_ptr group);

    /**
     * @brief Deletes a Conference Record
     * @param groupId
     * @return
     */
    bool deleteGroupingRecord(long groupId);

    /**
     * @brief Helper To populate Conference Record with Query Results.
     */
    void pullGroupingResult(query_ptr qry, group_ptr group);

    /**
     * @brief This takes a pair, and translates to (Column, .. ) VALUES (%d, %Q,) for formatting
     * @param values
     */
    void fillColumnValues(query_ptr qry, group_ptr group, 
        std::vector< std::pair<std::string, std::string> > &values);

    /**
     * @brief Return Grouping Record By Id.
     * @return
     */
    group_ptr getGroupingById(long groupId);
    
    /**
     * @brief Return List of All Groupings
     * @return
     */
    std::vector<group_ptr> getAllGroupings();
    
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
