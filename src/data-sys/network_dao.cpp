#include "network_dao.hpp"

#include "../model-sys/network.hpp"

#include "libSqliteWrapped.h"
#include <sqlite3.h>

#include <boost/make_shared.hpp>

#include <iostream>
#include <string>


NetworkDao::NetworkDao(SQLW::Database &database)
    : m_network_database(database)
{
    // Setup Table name
    strTableName = "network";

    /**
     * Pre Popluate Static Queries one Time
     */
    cmdFirstTimeSetup =
        "PRAGMA synchronous=Normal; "
        "PRAGMA encoding=UTF-8; "
        "PRAGMA foreign_keys=ON; "
        "PRAGMA default_cache_size=10000; "
        "PRAGMA cache_size=10000; ";
    
    // Check if Database Exists.
    cmdNetworkTableExists = "SELECT name FROM sqlite_master WHERE type='table' AND name='" + strTableName + "' COLLATE NOCASE;";

    // Create Table Query (SQLite Only for the moment)
    cmdCreateNetworkTable =
        "CREATE TABLE IF NOT EXISTS " + strTableName + " ( "
        "iId               INTEGER PRIMARY KEY, "
        "sName             TEXT NOT NULL COLLATE NOCASE, "
        "sType             TEXT NOT NULL COLLATE NOCASE, "
        "sAddress          TEXT NOT NULL COLLATE NOCASE "
        "); ";

    // CREATE INDEX `IDX_testtbl_Name` ON `testtbl` (`Name` COLLATE UTF8CI)
    cmdDropNetworkTable = "DROP TABLE IF EXISTS " + strTableName + "; ";
}

NetworkDao::~NetworkDao()
{
}


/**
 * @brief Check if the Table Exists in Database
 * @return
 */
bool NetworkDao::isTableExists()
{
    bool result = false;

    // Make Sure Database Reference is Connected
    if (!m_network_database.isConnected())
    {
        std::cout << "Error, Database is not connected!" << std::endl;
        return result;
    }

    // Create Pointer and Connect Query Object to Database.
    query_ptr qry(new SQLW::Query(m_network_database));
    if (!qry || !qry->isConnected())
    {
        std::cout << "Error, Query has no connection to the database" << std::endl;
        return result;
    }

    // Execute and get result.
    if (qry->getResult(cmdNetworkTableExists))
    {
        long rows = qry->getNumRows();
        if (rows > 0)
        {
            std::cout << "FileArea Table Exists!" << std::endl;
            result = true;
        }
        else
        {
            // No rows means the table doesn't exist!
            std::cout << "Error, FileArea table Exists Returned Rows: " << rows << std::endl;
        }
    }
    else
    {
        std::cout << "Error, getResult()" << std::endl;
    }

    return result;
}

/**
 * @brief Run Setup Params for SQL Database.
  */
bool NetworkDao::firstTimeSetupParams()
{
    bool result = false;

    // Make Sure Database Reference is Connected
    if (!m_network_database.isConnected())
    {
        std::cout << "Error, Database is not connected!" << std::endl;
        return result;
    }

    // Create Pointer and Connect Query Object to Database.
    query_ptr qry(new SQLW::Query(m_network_database));
    if (!qry || !qry->isConnected())
    {
        std::cout << "Error, Query has no connection to the database" << std::endl;
        return result;
    }

    // Execute Statement.
    result = qry->execute(cmdFirstTimeSetup);
    return result;
}

/**
 * @brief Create Table
 * If Create Table Fails, skip trying to create index.
 */
bool NetworkDao::createTable()
{
    bool result = false;

    // Make Sure Database Reference is Connected
    if (!m_network_database.isConnected())
    {
        std::cout << "Error, Database is not connected!" << std::endl;
        return result;
    }

    // Create Pointer and Connect Query Object to Database.
    query_ptr qry(new SQLW::Query(m_network_database));
    if (!qry || !qry->isConnected())
    {
        std::cout << "Error, Query has no connection to the database" << std::endl;
        return result;
    }

    // Create List of statements to execute in a single transaction.
    std::vector<std::string> statements;
    statements.push_back(cmdCreateNetworkTable);

    // Execute Transaction.
    result = qry->executeTransaction(statements);
    return result;  
}

/**
 * @brief Drop Table
 * If Drop Index Fails, still try to Drop Tables as it will try both!
 */
bool NetworkDao::dropTable()
{
    bool result = false;

    // Make Sure Database Reference is Connected
    if (!m_network_database.isConnected())
    {
        std::cout << "Error, Database is not connected!" << std::endl;
        return result;
    }

    // Create Pointer and Connect Query Object to Database.
    query_ptr qry(new SQLW::Query(m_network_database));
    if (!qry || !qry->isConnected())
    {
        std::cout << "Error, Query has no connection to the database" << std::endl;
        return result;
    }

    // Create List of statements to execute in a single transaction.
    std::vector<std::string> statements;
    statements.push_back(cmdDropNetworkTable);

    // Execute Transaction.
    result = qry->executeTransaction(statements);
    return result;
}

/**
 * @brief Pulls results by FieldNames into their Class Variables. 
 * @param qry
 * @param net
 */
void NetworkDao::pullNetworkResult(query_ptr qry, network_ptr net)
{
    qry->getFieldByName("iId", net->iId);
    qry->getFieldByName("sName", net->sName);    
    qry->getFieldByName("sType", net->sType);
    qry->getFieldByName("sAddress", net->sAddress);    
}

/**
 * @brief Used for Insert Statement
 *        This takes a pair, and translates to (Column, .. ) VALUES (%d, %Q,) for formatting
 * @param qry
 * @param net
 * @param values
 */ 
void NetworkDao::fillColumnValues(query_ptr qry, network_ptr net, 
    std::vector< std::pair<std::string, std::string> > &values)
{
    // values.push_back(qry->translateFieldName("iId", conf->iId));
    values.push_back(qry->translateFieldName("sName", net->sName));
    values.push_back(qry->translateFieldName("sType", net->sType));
    values.push_back(qry->translateFieldName("sAddress", net->sAddress));    
}

/**
 * @brief Create Network Record Insert Statement, returns query string 
 * @param qry
 * @param net
 * @return 
 */
std::string NetworkDao::insertNetworkQryString(query_ptr qry, network_ptr net)
{
    std::stringstream ssColumn;
    std::stringstream ssType;
    std::vector< std::pair<std::string, std::string> >::iterator it;
    std::vector< std::pair<std::string, std::string> > values;

    ssColumn << "INSERT INTO " + strTableName + " (";
    ssType << ") VALUES (";

    // Populate the Pairs.
    fillColumnValues(qry, net, values);

    // Build Query (Columns) VALUES (Types) ..  ie %d, %Q into a full string.
    it = values.begin();
    for (int i = 0; it != values.end(); i++)
    {
        // First Build Column Names
        ssColumn << (*it).first;
        ssType << (*it).second;

        ++it;
        if (it != values.end())
        {
            ssColumn << ", ";
            ssType << ", ";
        }
    }

    // Closing For Query.
    ssType << "); ";

    // Setup String to build the Query.
    std::string newQueryString = ssColumn.str();
    newQueryString.append(ssType.str());

    // Mprint statement to avoid injections.
    std::string result = sqlite3_mprintf(newQueryString.c_str(),
        net->sName.c_str(),
        net->sType.c_str(),
        net->sAddress.c_str()
    );

    return result;
}

/**
 * @brief Update Existing Network Record. 
 * @param qry
 * @param net
 * @return 
 */
std::string NetworkDao::updateNetworkQryString(query_ptr qry, network_ptr net)
{
    std::stringstream ssColumn;
    std::vector< std::pair<std::string, std::string> >::iterator it;
    std::vector< std::pair<std::string, std::string> > values;

    // Setup start of Statement
    ssColumn << "UPDATE " + strTableName + " SET ";

    // Populate the Pairs. Variable = %Q formatted string.
    fillColumnValues(qry, net, values);

    // Build Query (Columns) = (Values) ..  ie %d, %Q into a full string.
    it = values.begin();
    for (int i = 0; it != values.end(); i++)
    {
        ssColumn << (*it).first << "=" << (*it).second;
        ++it;
        if (it != values.end())
        {
            ssColumn << ", ";
        }
    }

    // Closing For Query.
    std::string newQueryString = ssColumn.str();
    newQueryString.append(" WHERE iId = %ld; ");

    // Mprint statement to avoid injections.
    std::string result = sqlite3_mprintf(newQueryString.c_str(),        
        net->sName.c_str(),
        net->sType.c_str(),
        net->sAddress.c_str(),
        net->iId
    );

    return result;
}

/**
 * @brief Updates a Network Record in the database!
 * @param net
 * @return
 */
bool NetworkDao::updateNetworkRecord(network_ptr net)
{
    bool result = false;

    // Make Sure Database Reference is Connected
    if (!m_network_database.isConnected())
    {
        std::cout << "Error, Database is not connected!" << std::endl;
        return result;
    }

    // Create Pointer and Connect Query Object to Database.
    query_ptr qry(new SQLW::Query(m_network_database));
    if (!qry || !qry->isConnected())
    {
        std::cout << "Error, Query has no connection to the database" << std::endl;
        return result;
    }

    // Build update string
    std::string queryString = updateNetworkQryString(qry, net);

    // Execute Update in a Transaction, rollback if fails.
    std::vector<std::string> statements;
    statements.push_back(queryString);
    result = qry->executeTransaction(statements);

    return result;
}

/**
 * @brief Inserts a new Network Record in the database!
 * @param net
 * @return
 */
long NetworkDao::insertNetworkRecord(network_ptr net)
{    
    bool result = false;
    long lastInsertId = -1;

    // Make Sure Database Reference is Connected
    if (!m_network_database.isConnected())
    {
        std::cout << "Error, Database is not connected!" << std::endl;
        return result;
    }

    // Create Pointer and Connect Query Object to Database.
    query_ptr qry(new SQLW::Query(m_network_database));
    if (!qry || !qry->isConnected())
    {
        std::cout << "Error, Query has no connection to the database" << std::endl;
        return result;
    }

    // Build update string
    std::string queryString = insertNetworkQryString(qry, net);

    // Execute Update in a Transaction, rollback if fails.
    std::vector<std::string> statements;
    statements.push_back(queryString);
    result = qry->executeTransaction(statements);

    // We need the insert id for Conference table
    if (result)
    {
        lastInsertId = qry->getInsertId();
    }

    return lastInsertId;
}

/**
 * @brief Deletes a Network Record
 * @param netId
 * @return
 */
bool NetworkDao::deleteNetworkRecord(long netId)
{
    bool result = false;

    // Make Sure Database Reference is Connected
    if (!m_network_database.isConnected())
    {
        std::cout << "Error, Database is not connected!" << std::endl;
        return result;
    }

    // Create Pointer and Connect Query Object to Database.
    query_ptr qry(new SQLW::Query(m_network_database));
    if (!qry || !qry->isConnected())
    {
        std::cout << "Error, Query has no connection to the database" << std::endl;
        return result;
    }

    // Build string
    std::string queryString = sqlite3_mprintf("DELETE FROM %Q WHERE iId = %ld;", strTableName.c_str(), netId);

    // Execute Update in a Transaction, rollback if fails.
    std::vector<std::string> statements;
    statements.push_back(queryString);
    result = qry->executeTransaction(statements);

    return result;
}

/**
 * @brief Return Network Record By Index ID.
 * @return
 */
network_ptr NetworkDao::getNetworkById(long netId)
{
    network_ptr net(new Network);

    // Make Sure Database Reference is Connected
    if (!m_network_database.isConnected())
    {
        std::cout << "Error, Database is not connected!" << std::endl;
        return net;
    }

    // Create Pointer and Connect Query Object to Database.
    query_ptr qry(new SQLW::Query(m_network_database));
    if (!qry->isConnected())
    {
        std::cout << "Error, Query has no connection to the database" << std::endl;
        return net;
    }

    // Build Query String
    std::string queryString = sqlite3_mprintf("SELECT * FROM %Q WHERE iID = %ld;", strTableName.c_str(), netId);

    // Execute Query.
    if (qry->getResult(queryString))
    {
        long rows = qry->getNumRows();
        if (rows > 0)
        {
            qry->fetchRow();
            pullNetworkResult(qry, net);
        }
        else
        {
            std::cout << "Error, getConferenceById Returned Rows: " << rows << std::endl;
        }
    }
    else
    {
        std::cout << "Error, getResult()" << std::endl;
    }

    return net;
}

/**
 * @brief Return List of All Networks
 * @return
 */
std::vector<network_ptr> NetworkDao::getAllNetworks()
{
    network_ptr net(new Network);
    std::vector<network_ptr> net_list;

    // Make Sure Database Reference is Connected
    if (!m_network_database.isConnected())
    {
        std::cout << "Error, Database is not connected!" << std::endl;
        return net_list;
    }

    // Create Pointer and Connect Query Object to Database.
    query_ptr qry(new SQLW::Query(m_network_database));
    if (!qry->isConnected())
    {
        std::cout << "Error, Query has no connection to the database" << std::endl;
        return net_list;
    }

    // Build Query String
    std::string queryString = sqlite3_mprintf("SELECT * FROM %Q;", strTableName.c_str());

    // Execute Query.
    if (qry->getResult(queryString))
    {
        long rows = qry->getNumRows();
        if (rows > 0)
        {
            while(qry->fetchRow())
            {
                net.reset(new Network);
                pullNetworkResult(qry, net);
                net_list.push_back(net);
            }
        }
        else
        {
            std::cout << "Error, getAllNetworks Returned Rows: " << rows << std::endl;
        }
    }
    else
    {
        std::cout << "Error, getResult()" << std::endl;
    }

    return net_list;
}
