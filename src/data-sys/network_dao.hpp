#ifndef NETWORK_DAO_HPP
#define NETWORK_DAO_HPP

#include <boost/smart_ptr/shared_ptr.hpp>

#include <vector>

// Forward Declerations
namespace SQLW
{
class Database;
class Query;
}

class Network;
// Handles to MessageArea
typedef boost::shared_ptr<Network> network_ptr;

// Handles to Database
typedef boost::shared_ptr<SQLW::Database> database_ptr;

// Handle to Database Queries
typedef boost::shared_ptr<SQLW::Query> query_ptr;

/**
 * @class NetworkDao
 * @author Michael Griffin
 * @date 05/05/2017
 * @file network_dao.hpp
 * @brief Handles Message Fido and Internet Network Address
 */
class NetworkDao
{
public:
    NetworkDao(SQLW::Database &database);
    ~NetworkDao();
    
    // Handle to Database
    SQLW::Database &m_network_database;

    std::string strTableName;

    // Static Queries
    std::string cmdFirstTimeSetup;

    std::string cmdNetworkTableExists;
    std::string cmdCreateNetworkTable;
    std::string cmdDropNetworkTable;

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
    std::string insertNetworkQryString(query_ptr qry, network_ptr net);

    /**
     * @brief Creates Query String to Update Existing MessageArea Record
     */
    std::string updateNetworkQryString(query_ptr qry, network_ptr net);

    /**
     * @brief Updates a MessageArea Record in the database!
     * @param net
     * @return
     */
    bool updateNetworkRecord(network_ptr net);

    /**
     * @brief Inserts a New MessageArea Record in the database!
     * @param net
     * @return
     */
    long insertNetworkRecord(network_ptr net);

    /**
     * @brief Deletes a MessageArea Record
     * @param areaId
     * @return
     */
    bool deleteNetworkRecord(long areaId);

    /**
     * @brief Helper To populate MessageArea Record with Query Results.
     */
    void pullNetworkResult(query_ptr qry, network_ptr net);

    /**
     * @brief This takes a pair, and translates to (Column, .. ) VALUES (%d, %Q,) for formatting
     * @param values
     */
    void fillColumnValues(query_ptr qry, network_ptr net, 
        std::vector< std::pair<std::string, std::string> > &values);

    /**
     * @brief Return MessageArea Record By Id.
     * @return
     */
    network_ptr getNetworkById(long netId);
    
    /**
     * @brief Return List of All MessageAreas
     * @return
     */
    std::vector<network_ptr> getAllNetworks();
        
};

// Handle to Database Queries
typedef boost::shared_ptr<NetworkDao> network_dao_ptr;

#endif // NETWORK_DAO_HPP
