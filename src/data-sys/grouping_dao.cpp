#include "grouping_dao.hpp"
#include "../model-sys/grouping.hpp"

#include "libSqliteWrapped.h"
#include <sqlite3.h>

#include <boost/make_shared.hpp>

#include <iostream>
#include <string>

GroupingDao::GroupingDao(SQLW::Database &database)
    : m_group_database(database)
{
    // Setup Table name
    strTableName = "grouping";

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
    cmdGroupingTableExists = "SELECT name FROM sqlite_master WHERE type='table' AND name='" + strTableName + "' COLLATE NOCASE;";

    // Create Table Query (SQLite Only for the moment)
    cmdCreateGroupingTable =
        "CREATE TABLE IF NOT EXISTS " + strTableName + " ( "
        "iId               INTEGER PRIMARY KEY, "
        "iConferenceId     INTEGER NOT NULL, "
        "iFileAreaId       INTEGER NOT NULL, "
        "iMsgAreaId        INTEGER NOT NULL, "        
        "FOREIGN KEY(iConferenceId) REFERENCES Conference(iId) ON DELETE CASCADE "
        "); ";

    cmdCreateGroupingIndex = "";
        "CREATE INDEX IF NOT EXISTS grouping_idx "
        "ON " + strTableName + " (iConferenceId); ";

    // CREATE INDEX `IDX_testtbl_Name` ON `testtbl` (`Name` COLLATE UTF8CI)
    cmdDropGroupingTable = "DROP TABLE IF EXISTS " + strTableName + "; ";
    cmdDropGroupingIndex = "DROP INDEX IF EXISTS grouping_idx; ";
}

GroupingDao::~GroupingDao()
{
}

/**
 * @brief Check if the Table Exists in Database
 * @return
 */
bool GroupingDao::isTableExists()
{
    bool result = false;

    // Make Sure Database Reference is Connected
    if (!m_group_database.isConnected())
    {
        std::cout << "Error, Database is not connected!" << std::endl;
        return result;
    }

    // Create Pointer and Connect Query Object to Database.
    query_ptr qry(new SQLW::Query(m_group_database));
    if (!qry || !qry->isConnected())
    {
        std::cout << "Error, Query has no connection to the database" << std::endl;
        return result;
    }

    // Execute and get result.
    if (qry->getResult(cmdGroupingTableExists))
    {
        long rows = qry->getNumRows();
        if (rows > 0)
        {
            std::cout << "Conference Table Exists!" << std::endl;
            result = true;
        }
        else
        {
            // No rows means the table doesn't exist!
            std::cout << "Error, Conference table Exists Returned Rows: " << rows << std::endl;
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
bool GroupingDao::firstTimeSetupParams()
{
    bool result = false;

    // Make Sure Database Reference is Connected
    if (!m_group_database.isConnected())
    {
        std::cout << "Error, Database is not connected!" << std::endl;
        return result;
    }

    // Create Pointer and Connect Query Object to Database.
    query_ptr qry(new SQLW::Query(m_group_database));
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
 * @brief Create Users Table
 * If Create Table Fails, skip trying to create index.
 */
bool GroupingDao::createTable()
{
    bool result = false;

    // Make Sure Database Reference is Connected
    if (!m_group_database.isConnected())
    {
        std::cout << "Error, Database is not connected!" << std::endl;
        return result;
    }

    // Create Pointer and Connect Query Object to Database.
    query_ptr qry(new SQLW::Query(m_group_database));
    if (!qry || !qry->isConnected())
    {
        std::cout << "Error, Query has no connection to the database" << std::endl;
        return result;
    }

    // Create List of statements to execute in a single transaction.
    std::vector<std::string> statements;

    statements.push_back(cmdCreateGroupingTable);
    statements.push_back(cmdCreateGroupingIndex);

    // Execute Transaction.
    result = qry->executeTransaction(statements);
    return result;  
}

/**
 * @brief Drop Table
 * If Drop Index Fails, still try to Drop Tables as it will try both!
 */
bool GroupingDao::dropTable()
{
    bool result = false;

    // Make Sure Database Reference is Connected
    if (!m_group_database.isConnected())
    {
        std::cout << "Error, Database is not connected!" << std::endl;
        return result;
    }

    // Create Pointer and Connect Query Object to Database.
    query_ptr qry(new SQLW::Query(m_group_database));
    if (!qry || !qry->isConnected())
    {
        std::cout << "Error, Query has no connection to the database" << std::endl;
        return result;
    }

    // Create List of statements to execute in a single transaction.
    std::vector<std::string> statements;
    statements.push_back(cmdDropGroupingIndex);
    statements.push_back(cmdDropGroupingTable);

    // Execute Transaction.
    result = qry->executeTransaction(statements);
    return result;
}

/**
 * @brief Pulls results by FieldNames into their Class Variables. 
 * @param qry
 * @param group
 */
void GroupingDao::pullGroupingResult(query_ptr qry, group_ptr group)
{
    qry->getFieldByName("iId", group->iId);
    qry->getFieldByName("iConferenceId", group->iConferenceId);
    qry->getFieldByName("iFileAreaId", group->iFileAreaId);
    qry->getFieldByName("iMsgAreaId", group->iMsgAreaId); 
}

/**
 * @brief Used for Insert Statement
 *        This takes a pair, and translates to (Column, .. ) VALUES (%d, %Q,) for formatting
 * @param qry
 * @param group
 * @param values
 */ 
void GroupingDao::fillColumnValues(query_ptr qry, group_ptr group, 
    std::vector< std::pair<std::string, std::string> > &values)
{
    // values.push_back(qry->translateFieldName("iId", group->iId));
    values.push_back(qry->translateFieldName("iConferenceId", group->iConferenceId));
    values.push_back(qry->translateFieldName("iFileAreaId", group->iFileAreaId));
    values.push_back(qry->translateFieldName("iMsgAreaId", group->iMsgAreaId));   
}

/**
 * @brief Create Grouping Record Insert Statement, returns query string 
 * @param qry
 * @param group
 * @return 
 */
std::string GroupingDao::insertGroupingQryString(query_ptr qry, group_ptr group)
{
    std::stringstream ssColumn;
    std::stringstream ssType;
    std::vector< std::pair<std::string, std::string> >::iterator it;
    std::vector< std::pair<std::string, std::string> > values;

    ssColumn << "INSERT INTO " + strTableName + " (";
    ssType << ") VALUES (";

    // Populate the Pairs.
    fillColumnValues(qry, group, values);

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
        group->iConferenceId,
        group->iFileAreaId,
        group->iMsgAreaId
    );

    return result;
}

/**
 * @brief Update Existing Grouping Record. 
 * @param qry
 * @param group
 * @return 
 */
std::string GroupingDao::updateGroupingQryString(query_ptr qry, group_ptr group)
{
    std::stringstream ssColumn;
    std::vector< std::pair<std::string, std::string> >::iterator it;
    std::vector< std::pair<std::string, std::string> > values;

    // Setup start of Statement
    ssColumn << "UPDATE " + strTableName + " SET ";

    // Populate the Pairs. Variable = %Q formatted string.
    fillColumnValues(qry, group, values);

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
        group->iConferenceId,
        group->iFileAreaId,
        group->iMsgAreaId,
        group->iId
    );

    return result;
}

/**
 * @brief Updates a Grouping Record in the database!
 * @param group
 * @return
 */
bool GroupingDao::updateGroupingRecord(group_ptr group)
{
    bool result = false;

    // Make Sure Database Reference is Connected
    if (!m_group_database.isConnected())
    {
        std::cout << "Error, Database is not connected!" << std::endl;
        return result;
    }

    // Create Pointer and Connect Query Object to Database.
    query_ptr qry(new SQLW::Query(m_group_database));
    if (!qry || !qry->isConnected())
    {
        std::cout << "Error, Query has no connection to the database" << std::endl;
        return result;
    }

    // Build update string
    std::string queryString = updateGroupingQryString(qry, group);

    // Execute Update in a Transaction, rollback if fails.
    std::vector<std::string> statements;
    statements.push_back(queryString);
    result = qry->executeTransaction(statements);

    return result;
}

/**
 * @brief Inserts a new Grouping Record in the database!
 * @param group
 * @return
 */
long GroupingDao::insertGroupingRecord(group_ptr group)
{    
    bool result = false;
    long lastInsertId = -1;

    // Make Sure Database Reference is Connected
    if (!m_group_database.isConnected())
    {
        std::cout << "Error, Database is not connected!" << std::endl;
        return result;
    }

    // Create Pointer and Connect Query Object to Database.
    query_ptr qry(new SQLW::Query(m_group_database));
    if (!qry || !qry->isConnected())
    {
        std::cout << "Error, Query has no connection to the database" << std::endl;
        return result;
    }

    // Build update string
    std::string queryString = insertGroupingQryString(qry, group);

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
 * @brief Deletes a Conference Record
 * @param groupId
 * @return
 */
bool GroupingDao::deleteGroupingRecord(long groupId)
{
    bool result = false;

    // Make Sure Database Reference is Connected
    if (!m_group_database.isConnected())
    {
        std::cout << "Error, Database is not connected!" << std::endl;
        return result;
    }

    // Create Pointer and Connect Query Object to Database.
    query_ptr qry(new SQLW::Query(m_group_database));
    if (!qry || !qry->isConnected())
    {
        std::cout << "Error, Query has no connection to the database" << std::endl;
        return result;
    }

    // Build string
    std::string queryString = sqlite3_mprintf("DELETE FROM %Q WHERE iId = %ld;", strTableName.c_str(), groupId);

    // Execute Update in a Transaction, rollback if fails.
    std::vector<std::string> statements;
    statements.push_back(queryString);
    result = qry->executeTransaction(statements);

    return result;
}

/**
 * @brief Return User Record By Index ID.
 * @return
 */
group_ptr GroupingDao::getGroupingById(long groupId)
{
    group_ptr group(new Grouping);

    // Make Sure Database Reference is Connected
    if (!m_group_database.isConnected())
    {
        std::cout << "Error, Database is not connected!" << std::endl;
        return group;
    }

    // Create Pointer and Connect Query Object to Database.
    query_ptr qry(new SQLW::Query(m_group_database));
    if (!qry->isConnected())
    {
        std::cout << "Error, Query has no connection to the database" << std::endl;
        return group;
    }

    // Build Query String
    std::string queryString = sqlite3_mprintf("SELECT * FROM %Q WHERE iID = %ld;", strTableName.c_str(), groupId);

    // Execute Query.
    if (qry->getResult(queryString))
    {
        long rows = qry->getNumRows();
        if (rows > 0)
        {
            qry->fetchRow();
            pullGroupingResult(qry, group);
        }
        else
        {
            std::cout << "Error, getGroupingById Returned Rows: " << rows << std::endl;
        }
    }
    else
    {
        std::cout << "Error, getResult()" << std::endl;
    }

    return group;
}

/**
 * @brief Return List of All Conference
 * @return
 */
std::vector<group_ptr> GroupingDao::getAllGroupings()
{
    group_ptr group(new Grouping);
    std::vector<group_ptr> group_list;

    // Make Sure Database Reference is Connected
    if (!m_group_database.isConnected())
    {
        std::cout << "Error, Database is not connected!" << std::endl;
        return group_list;
    }

    // Create Pointer and Connect Query Object to Database.
    query_ptr qry(new SQLW::Query(m_group_database));
    if (!qry->isConnected())
    {
        std::cout << "Error, Query has no connection to the database" << std::endl;
        return group_list;
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
                group.reset(new Grouping);
                pullGroupingResult(qry, group);
                group_list.push_back(group);
            }
        }
        else
        {
            std::cout << "Error, getAllGrouping Returned Rows: " << rows << std::endl;
        }
    }
    else
    {
        std::cout << "Error, getResult()" << std::endl;
    }

    return group_list;
}

/**
 * @brief Return List of All Groupings by ConferenceId
 * @param confId
 * @return
 */ 
std::vector<group_ptr> GroupingDao::getAllGroupingsByConferenceId(long confId)
{
    group_ptr group(new Grouping);
    std::vector<group_ptr> group_list;

    // Make Sure Database Reference is Connected
    if (!m_group_database.isConnected())
    {
        std::cout << "Error, Database is not connected!" << std::endl;
        return group_list;
    }

    // Create Pointer and Connect Query Object to Database.
    query_ptr qry(new SQLW::Query(m_group_database));
    if (!qry->isConnected())
    {
        std::cout << "Error, Query has no connection to the database" << std::endl;
        return group_list;
    }

    // Build Query String
    std::string queryString = sqlite3_mprintf("SELECT * FROM %Q WHERE iConferenceId = %ld;", strTableName.c_str(), confId);

    // Execute Query.
    if (qry->getResult(queryString))
    {
        long rows = qry->getNumRows();
        if (rows > 0)
        {
            while(qry->fetchRow())
            {
                group.reset(new Grouping);
                pullGroupingResult(qry, group);
                group_list.push_back(group);
            }
        }
        else
        {
            std::cout << "Error, getAllGroupingsByConferenceId Returned Rows: " << rows << std::endl;
        }
    }
    else
    {
        std::cout << "Error, getResult()" << std::endl;
    }

    return group_list;
}

