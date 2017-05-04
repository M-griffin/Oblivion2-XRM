#ifndef MESSAGE_AREA_DAO_HPP
#define MESSAGE_AREA_DAO_HPP

#include <boost/smart_ptr/shared_ptr.hpp>

#include <vector>

// Forward Declerations
namespace SQLW
{
class Database;
class Query;
}

class MessageArea;
// Handles to MessageArea
typedef boost::shared_ptr<MessageArea> msg_area_ptr;

// Handles to Database
typedef boost::shared_ptr<SQLW::Database> database_ptr;

// Handle to Database Queries
typedef boost::shared_ptr<SQLW::Query> query_ptr;

/**
 * @class MessageAreaDao
 * @author Michael Griffin
 * @date 04/05/2017
 * @file message_area_dao.hpp
 * @brief Message Area Data Access Object
 */
class MessageAreaDao
{
public:
    MessageAreaDao(SQLW::Database &database);
    ~MessageAreaDao();

    // Handle to Database
    SQLW::Database &m_msg_area_database;

    std::string strTableName;

    // Static Queries
    std::string cmdFirstTimeSetup;

    std::string cmdMessageAreaTableExists;
    std::string cmdCreateMessageAreaTable;
    std::string cmdDropMessageAreaTable;

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
