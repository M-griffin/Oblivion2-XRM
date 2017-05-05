#include "oneliners_dao.hpp"

#include "../model-app/oneliners.hpp"

#include "libSqliteWrapped.h"
#include <sqlite3.h>

#include <boost/make_shared.hpp>

#include <iostream>
#include <string>

OnelinerDao::OnelinerDao(SQLW::Database &database)
    : m_oneliner_database(database)
{
    // Setup Table name
    strTableName = "oneliner";

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
    cmdOnelinerTableExists = "SELECT name FROM sqlite_master WHERE type='table' AND name='" + strTableName + "' COLLATE NOCASE;";

    // Create Table Query (SQLite Only for the moment)
    cmdCreateOnelinerTable =
        "CREATE TABLE IF NOT EXISTS " + strTableName + " ( "
        "iId               INTEGER PRIMARY KEY, "
        "iUserId           INTEGER NOT NULL, "
        "sText             TEXT NOT NULL COLLATE NOCASE, "
        "sUserName         TEXT NOT NULL COLLATE NOCASE, "
        "sUserInitials     TEXT NOT NULL COLLATE NOCASE, "
        "dtDatePosted      DATETIME DEFAULT CURRENT_TIMESTAMP, "
        "FOREIGN KEY(iUserId) REFERENCES User(iId) ON DELETE CASCADE "
        "); ";

    // CREATE INDEX `IDX_testtbl_Name` ON `testtbl` (`Name` COLLATE UTF8CI)
    cmdDropOnelinerTable = "DROP TABLE IF EXISTS " + strTableName + "; ";
}

OnelinerDao::~OnelinerDao()
{
}


/**
 * @brief Check if the Table Exists in Database
 * @return
 */
bool OnelinerDao::isTableExists()
{
    bool result = false;

    // Make Sure Database Reference is Connected
    if (!m_oneliner_database.isConnected())
    {
        std::cout << "Error, Database is not connected!" << std::endl;
        return result;
    }

    // Create Pointer and Connect Query Object to Database.
    query_ptr qry(new SQLW::Query(m_oneliner_database));
    if (!qry || !qry->isConnected())
    {
        std::cout << "Error, Query has no connection to the database" << std::endl;
        return result;
    }

    // Execute and get result.
    if (qry->getResult(cmdOnelinerTableExists))
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
bool OnelinerDao::firstTimeSetupParams()
{
    bool result = false;

    // Make Sure Database Reference is Connected
    if (!m_oneliner_database.isConnected())
    {
        std::cout << "Error, Database is not connected!" << std::endl;
        return result;
    }

    // Create Pointer and Connect Query Object to Database.
    query_ptr qry(new SQLW::Query(m_oneliner_database));
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
bool OnelinerDao::createTable()
{
    bool result = false;

    // Make Sure Database Reference is Connected
    if (!m_oneliner_database.isConnected())
    {
        std::cout << "Error, Database is not connected!" << std::endl;
        return result;
    }

    // Create Pointer and Connect Query Object to Database.
    query_ptr qry(new SQLW::Query(m_oneliner_database));
    if (!qry || !qry->isConnected())
    {
        std::cout << "Error, Query has no connection to the database" << std::endl;
        return result;
    }

    // Create List of statements to execute in a single transaction.
    std::vector<std::string> statements;
    statements.push_back(cmdCreateOnelinerTable);

    // Execute Transaction.
    result = qry->executeTransaction(statements);
    return result;  
}

/**
 * @brief Drop Table
 * If Drop Index Fails, still try to Drop Tables as it will try both!
 */
bool OnelinerDao::dropTable()
{
    bool result = false;

    // Make Sure Database Reference is Connected
    if (!m_oneliner_database.isConnected())
    {
        std::cout << "Error, Database is not connected!" << std::endl;
        return result;
    }

    // Create Pointer and Connect Query Object to Database.
    query_ptr qry(new SQLW::Query(m_oneliner_database));
    if (!qry || !qry->isConnected())
    {
        std::cout << "Error, Query has no connection to the database" << std::endl;
        return result;
    }

    // Create List of statements to execute in a single transaction.
    std::vector<std::string> statements;
    statements.push_back(cmdDropOnelinerTable);

    // Execute Transaction.
    result = qry->executeTransaction(statements);
    return result;
}

/**
 * @brief Pulls results by FieldNames into their Class Variables. 
 * @param qry
 * @param one
 */
void OnelinerDao::pullOnelinerResult(query_ptr qry, oneliner_ptr one)
{
    qry->getFieldByName("iId", one->iId);
    qry->getFieldByName("iUserId", one->iUserId);    
    qry->getFieldByName("sText", one->sText);
    qry->getFieldByName("sUserName", one->sUserName);
    qry->getFieldByName("sUserInitials", one->sUserInitials);
    qry->getFieldByName("dtDatePosted", one->dtDatePosted);
}

/**
 * @brief Used for Insert Statement
 *        This takes a pair, and translates to (Column, .. ) VALUES (%d, %Q,) for formatting
 * @param qry
 * @param one
 * @param values
 */ 
void OnelinerDao::fillColumnValues(query_ptr qry, oneliner_ptr one, 
    std::vector< std::pair<std::string, std::string> > &values)
{
    // values.push_back(qry->translateFieldName("iId", conf->iId));
    values.push_back(qry->translateFieldName("iUserId", one->iUserId));
    values.push_back(qry->translateFieldName("sText", one->sText));
    values.push_back(qry->translateFieldName("sUserName", one->sUserName));
    values.push_back(qry->translateFieldName("sUserInitials", one->sUserInitials));
    values.push_back(qry->translateFieldName("dtDatePosted", one->dtDatePosted));
}

/**
 * @brief Create Oneliners Record Insert Statement, returns query string 
 * @param qry
 * @param one
 * @return 
 */
std::string OnelinerDao::insertOnelinerQryString(query_ptr qry, oneliner_ptr one)
{
    std::stringstream ssColumn;
    std::stringstream ssType;
    std::vector< std::pair<std::string, std::string> >::iterator it;
    std::vector< std::pair<std::string, std::string> > values;

    ssColumn << "INSERT INTO " + strTableName + " (";
    ssType << ") VALUES (";

    // Populate the Pairs.
    fillColumnValues(qry, one, values);

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
        one->iUserId,
        one->sText.c_str(),
        one->sUserName.c_str(),
        one->sUserInitials.c_str(),
        one->dtDatePosted
    );
    
    return result;
}

/**
 * @brief Update Existing Oneliners Record. 
 * @param qry
 * @param one
 * @return 
 */
std::string OnelinerDao::updateOnelinerQryString(query_ptr qry, oneliner_ptr one)
{
    std::stringstream ssColumn;
    std::vector< std::pair<std::string, std::string> >::iterator it;
    std::vector< std::pair<std::string, std::string> > values;

    // Setup start of Statement
    ssColumn << "UPDATE " + strTableName + " SET ";

    // Populate the Pairs. Variable = %Q formatted string.
    fillColumnValues(qry, one, values);

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
        one->iUserId,
        one->sText.c_str(),
        one->sUserName.c_str(),
        one->sUserInitials.c_str(),
        one->dtDatePosted,
        one->iId
    );

    return result;
}

/**
 * @brief Updates a Oneliners Record in the database!
 * @param one
 * @return
 */
bool OnelinerDao::updateOnelinerRecord(oneliner_ptr one)
{
    bool result = false;

    // Make Sure Database Reference is Connected
    if (!m_oneliner_database.isConnected())
    {
        std::cout << "Error, Database is not connected!" << std::endl;
        return result;
    }

    // Create Pointer and Connect Query Object to Database.
    query_ptr qry(new SQLW::Query(m_oneliner_database));
    if (!qry || !qry->isConnected())
    {
        std::cout << "Error, Query has no connection to the database" << std::endl;
        return result;
    }

    // Build update string
    std::string queryString = updateOnelinerQryString(qry, one);

    // Execute Update in a Transaction, rollback if fails.
    std::vector<std::string> statements;
    statements.push_back(queryString);
    result = qry->executeTransaction(statements);

    return result;
}

/**
 * @brief Inserts a new Oneliners Record in the database!
 * @param one
 * @return
 */
long OnelinerDao::insertOnelinerRecord(oneliner_ptr one)
{    
    bool result = false;
    long lastInsertId = -1;

    // Make Sure Database Reference is Connected
    if (!m_oneliner_database.isConnected())
    {
        std::cout << "Error, Database is not connected!" << std::endl;
        return result;
    }

    // Create Pointer and Connect Query Object to Database.
    query_ptr qry(new SQLW::Query(m_oneliner_database));
    if (!qry || !qry->isConnected())
    {
        std::cout << "Error, Query has no connection to the database" << std::endl;
        return result;
    }

    // Build update string
    std::string queryString = insertOnelinerQryString(qry, one);

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
 * @brief Deletes a Oneliners Record
 * @param oneId
 * @return
 */
bool OnelinerDao::deleteOnelinerRecord(long oneId)
{
    bool result = false;

    // Make Sure Database Reference is Connected
    if (!m_oneliner_database.isConnected())
    {
        std::cout << "Error, Database is not connected!" << std::endl;
        return result;
    }

    // Create Pointer and Connect Query Object to Database.
    query_ptr qry(new SQLW::Query(m_oneliner_database));
    if (!qry || !qry->isConnected())
    {
        std::cout << "Error, Query has no connection to the database" << std::endl;
        return result;
    }

    // Build string
    std::string queryString = sqlite3_mprintf("DELETE FROM %Q WHERE iId = %ld;", strTableName.c_str(), oneId);

    // Execute Update in a Transaction, rollback if fails.
    std::vector<std::string> statements;
    statements.push_back(queryString);
    result = qry->executeTransaction(statements);

    return result;
}

/**
 * @brief Return Oneliners Record By Index ID.
 * @return
 */
oneliner_ptr OnelinerDao::getOnelinerById(long oneId)
{
    oneliner_ptr one(new Oneliners);

    // Make Sure Database Reference is Connected
    if (!m_oneliner_database.isConnected())
    {
        std::cout << "Error, Database is not connected!" << std::endl;
        return one;
    }

    // Create Pointer and Connect Query Object to Database.
    query_ptr qry(new SQLW::Query(m_oneliner_database));
    if (!qry->isConnected())
    {
        std::cout << "Error, Query has no connection to the database" << std::endl;
        return one;
    }

    // Build Query String
    std::string queryString = sqlite3_mprintf("SELECT * FROM %Q WHERE iID = %ld;", strTableName.c_str(), oneId);

    // Execute Query.
    if (qry->getResult(queryString))
    {
        long rows = qry->getNumRows();
        if (rows > 0)
        {
            qry->fetchRow();
            pullOnelinerResult(qry, one);
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

    return one;
}

/**
 * @brief Return All Oneliners Records By User ID.
 * @return
 */
std::vector<oneliner_ptr> OnelinerDao::getAllOnelinersByUserId(long userId)
{
    oneliner_ptr one(new Oneliners);
    std::vector<oneliner_ptr> one_list;

    // Make Sure Database Reference is Connected
    if (!m_oneliner_database.isConnected())
    {
        std::cout << "Error, Database is not connected!" << std::endl;
        return one_list;
    }

    // Create Pointer and Connect Query Object to Database.
    query_ptr qry(new SQLW::Query(m_oneliner_database));
    if (!qry->isConnected())
    {
        std::cout << "Error, Query has no connection to the database" << std::endl;
        return one_list;
    }

    // Build Query String
    std::string queryString = sqlite3_mprintf("SELECT * FROM %Q WHERE iUserId = %ld;", strTableName.c_str(), userId);

    // Execute Query.
    if (qry->getResult(queryString))
    {
        long rows = qry->getNumRows();
        if (rows > 0)
        {
            while(qry->fetchRow())
            {
                one.reset(new Oneliners);
                pullOnelinerResult(qry, one);
                one_list.push_back(one);
            }
        }
        else
        {
            std::cout << "Error, getAllOnelinerss Returned Rows: " << rows << std::endl;
        }
    }
    else
    {
        std::cout << "Error, getResult()" << std::endl;
    }

    return one_list;
}

/**
 * @brief Return List of All Oneliners
 * @return
 */
std::vector<oneliner_ptr> OnelinerDao::getAllOneliners()
{
    oneliner_ptr one(new Oneliners);
    std::vector<oneliner_ptr> one_list;

    // Make Sure Database Reference is Connected
    if (!m_oneliner_database.isConnected())
    {
        std::cout << "Error, Database is not connected!" << std::endl;
        return one_list;
    }

    // Create Pointer and Connect Query Object to Database.
    query_ptr qry(new SQLW::Query(m_oneliner_database));
    if (!qry->isConnected())
    {
        std::cout << "Error, Query has no connection to the database" << std::endl;
        return one_list;
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
                one.reset(new Oneliners);
                pullOnelinerResult(qry, one);
                one_list.push_back(one);
            }
        }
        else
        {
            std::cout << "Error, getAllOnelinerss Returned Rows: " << rows << std::endl;
        }
    }
    else
    {
        std::cout << "Error, getResult()" << std::endl;
    }

    return one_list;
}
