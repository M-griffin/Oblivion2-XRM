#ifndef SESSION_STATS_DAO_HPP
#define SESSION_STATS_DAO_HPP

#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/smart_ptr/weak_ptr.hpp>

#include <vector>

// Forward Declerations
namespace SQLW
{
class Database;
class Query;
}

class SessionStats;
// Handles to Users
typedef boost::shared_ptr<SessionStats> session_stats_ptr;
typedef boost::weak_ptr<SessionStats> session_stats_weak_ptr;


// Handles to Database
typedef boost::shared_ptr<SQLW::Database> database_ptr;
typedef boost::weak_ptr<SQLW::Database> database_weak_ptr;

// Handle to Database Queries
typedef boost::shared_ptr<SQLW::Query> query_ptr;


class SessionStatsDao
{
public:
    SessionStatsDao(SQLW::Database &database);
    ~SessionStatsDao();

    // Handle to Database
    SQLW::Database &m_stats_database;

    std::string strTableName;

    // Static Queries
    std::string cmdFirstTimeSetup;

    std::string cmdSessionStatTableExists;
    std::string cmdCreateSessionStatTable;
    std::string cmdCreateSessionStatIndex;
    std::string cmdDropSessionStatTable;
    std::string cmdDropSessionStatIndex;

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
     * @brief Create Query String to Insert New Record
     */
    std::string insertSessionStatQryString(query_ptr qry, session_stats_ptr stats);

    /**
     * @brief Creates Query String to Update Existing User Record
     */
    std::string updateSessionStatQryString(query_ptr qry, session_stats_ptr stats);

    /**
     * @brief Updates a Record in the database!
     * @param user
     * @return
     */
    bool updateSessionStatRecord(session_stats_ptr stats);

    /**
     * @brief Inserts a New Record in the database!
     * @param user
     * @return
     */
    long insertSessionStatRecord(session_stats_ptr stats);

    /**
     * @brief Deletes an Existing Record
     * @param id
     * @return
     */
    bool deleteSessionStatRecord(long id);

    /**
     * @brief Helper To populate User Record with Query Results.
     */
    void pullSessionStatResult(query_ptr qry, session_stats_ptr stats);


    /**
     * @brief This takes a pair, and translates to (Column, .. ) VALUES (%d, %Q,) for formatting
     * @param values
     */
    void fillColumnValues(query_ptr qry, session_stats_ptr stats, std::vector< std::pair<std::string, std::string> > &values);

    /**
     * @brief Return Record By Id.
     * @return
     */
    session_stats_ptr getSessionStatById(long id);

    /**
     * @brief Return List of Stats per user
     * @return
     */
    std::vector<session_stats_ptr> getAllStatsPerUser(long userId);
   
    /**
     * @brief Return List of Last 10 Valid Sessions.
     * @return
     */
    std::vector<session_stats_ptr> getLast10Callers();

};

// Handle to Database Queries
typedef boost::shared_ptr<SessionStatsDao> session_stats_dao_ptr;

#endif // SESSION_STATS_DAO_HPP
