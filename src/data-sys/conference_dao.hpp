#ifndef CONFERENCE_DAO_HPP
#define CONFERENCE_DAO_HPP

#include <boost/smart_ptr/shared_ptr.hpp>

#include <vector>

// Forward Declerations
namespace SQLW
{
class Database;
class Query;
}

class Conference;
// Handles to Conference
typedef boost::shared_ptr<Conference> conference_ptr;

// Handles to Database
typedef boost::shared_ptr<SQLW::Database> database_ptr;

// Handle to Database Queries
typedef boost::shared_ptr<SQLW::Query> query_ptr;

/**
 * @class ConferenceDao
 * @author Michael Griffin
 * @date 03/05/2017
 * @file conference.hpp
 * @brief Conference Data Access Object
 */
class ConferenceDao
{
public:
    ConferenceDao(SQLW::Database &database);
    ~ConferenceDao();

    // Handle to Database
    SQLW::Database &m_conference_database;

    std::string strTableName;

    // Static Queries
    std::string cmdFirstTimeSetup;

    std::string cmdConferenceTableExists;
    std::string cmdCreateConferenceTable;
    std::string cmdCreateConferenceIndex;
    std::string cmdDropConferenceTable;
    std::string cmdDropConferenceIndex;

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
    std::string insertConferenceQryString(query_ptr qry, conference_ptr conf);

    /**
     * @brief Creates Query String to Update Existing Conference Record
     */
    std::string updateConferenceQryString(query_ptr qry, conference_ptr conf);

    /**
     * @brief Updates a Conference Record in the database!
     * @param conf
     * @return
     */
    bool updateConferenceRecord(conference_ptr conf);

    /**
     * @brief Inserts a New Conference Record in the database!
     * @param conf
     * @return
     */
    long insertConferenceRecord(conference_ptr conf);

    /**
     * @brief Deletes a Conference Record
     * @param userId
     * @return
     */
    bool deleteConferenceRecord(long confId);

    /**
     * @brief Helper To populate Conference Record with Query Results.
     */
    void pullConferenceResult(query_ptr qry, conference_ptr conf);

    /**
     * @brief This takes a pair, and translates to (Column, .. ) VALUES (%d, %Q,) for formatting
     * @param values
     */
    void fillColumnValues(query_ptr qry, conference_ptr conf, 
        std::vector< std::pair<std::string, std::string> > &values);

    /**
     * @brief Return Conference Record By Id.
     * @return
     */
    conference_ptr getConferenceById(long confId);
    
    /**
     * @brief Return List of All Conferences
     * @return
     */
    std::vector<conference_ptr> getAllConferences();
    
    /**
     * @brief Return List of All Conference by Type
     * @param type
     * @return
     */ 
    std::vector<conference_ptr> getAllConferencesByType(std::string type);
    
    /**
     * @brief Return Count or Number of Existing Conferences by Type
     * @param type
     * @return
     */ 
    long getConferencesCountByType(std::string type);

};

// Handle to Database Queries
typedef boost::shared_ptr<ConferenceDao> conference_dao_ptr;

#endif // CONFERENCE__DAO_HPP
