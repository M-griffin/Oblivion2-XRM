#include "security_dao.hpp"
#include "../model/security.hpp"

#include "libSqliteWrapped.h"
#include <sqlite3.h>

#include <boost/make_shared.hpp>

#include <iostream>
#include <string>


SecurityDao::SecurityDao(SQLW::Database &database)
    : m_security_database(database)
{

    strTableName = "security";

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
    cmdSecurityTableExists = "SELECT name FROM sqlite_master WHERE type='table' "
        "AND name='" + strTableName + "' COLLATE NOCASE;";

    // Crate Seciroty Table Query (SQLite Only for the moment)
    cmdCreateSecurityTable =
        "CREATE TABLE IF NOT EXISTS " + strTableName + " ( "
        "iId                   INTEGER PRIMARY KEY, "
        "sPasswordHash         TEXT NOT NULL, "
        "sSaltHash             TEXT NOT NULL, "
        "sChallengeQuestion    TEXT NOT NULL, "
        "sChallengeAnswerHash  TEXT NOT NULL "
        "); ";

    // Drops and cleanup security table.
    cmdDropSecurityTable = "DROP TABLE IF EXISTS " + strTableName + "; ";
}

SecurityDao::~SecurityDao()
{
    std::cout << "~SecurityDao" << std::endl;
}

/**
 * @brief Check if the Table Exists in Database
 * @return
 */
bool SecurityDao::isTableExists()
{
    bool result = false;

    // Make Sure Database Reference is Connected
    if (!m_security_database.isConnected())
    {
        std::cout << "Error, Database is not connected!" << std::endl;
        return result;
    }

    // Create Pointer and Connect Query Object to Database.
    query_ptr qry(new SQLW::Query(m_security_database));
    if (!qry || !qry->isConnected())
    {
        std::cout << "Error, Query has no connection to the database" << std::endl;
        return result;
    }

    // Execute and get result.
    if (qry->getResult(cmdSecurityTableExists))
    {
        long rows = qry->getNumRows();
        if (rows > 0)
        {
            std::cout << "Security Table Exists!" << std::endl;
            result = true;
        }
        else
        {
            // No rows means the table doesn't exist!
            std::cout << "Error, Security table Exists Returned Rows: " << rows << std::endl;
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
bool SecurityDao::firstTimeSetupParams()
{
    bool result = false;

    // Make Sure Database Reference is Connected
    if (!m_security_database.isConnected())
    {
        std::cout << "Error, Database is not connected!" << std::endl;
        return result;
    }

    // Create Pointer and Connect Query Object to Database.
    query_ptr qry(new SQLW::Query(m_security_database));
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
 * @brief Create Security Table
 * If Create Table Fails, skip trying to create index.
 */
bool SecurityDao::createTable()
{
    bool result = false;

    // Make Sure Database Reference is Connected
    if (!m_security_database.isConnected())
    {
        std::cout << "Error, Database is not connected!" << std::endl;
        return result;
    }

    // Create Pointer and Connect Query Object to Database.
    query_ptr qry(new SQLW::Query(m_security_database));
    if (!qry || !qry->isConnected())
    {
        std::cout << "Error, Query has no connection to the database" << std::endl;
        return result;
    }

    // Create List of statements to execute in a single transaction.
    std::vector<std::string> statements;
    statements.push_back(cmdCreateSecurityTable);

    // Execute Transaction.
    result = qry->executeTransaction(statements);
    return result;  
}

/**
 * @brief Drop Security Table
 * If Drop Index Fails, still try to Drop Tables as it will try both!
 */
bool SecurityDao::dropTable()
{
    bool result = false;

    // Make Sure Database Reference is Connected
    if (!m_security_database.isConnected())
    {
        std::cout << "Error, Database is not connected!" << std::endl;
        return result;
    }

    // Create Pointer and Connect Query Object to Database.
    query_ptr qry(new SQLW::Query(m_security_database));
    if (!qry || !qry->isConnected())
    {
        std::cout << "Error, Query has no connection to the database" << std::endl;
        return result;
    }

    // Create List of statements to execute in a single transaction.
    std::vector<std::string> statements;
    statements.push_back(cmdDropSecurityTable);

    // Execute Transaction.
    result = qry->executeTransaction(statements);
    return result;
}

/**
 * @brief Pulls results by FieldNames into their Class Variables.
 */
void SecurityDao::pullSecurityResult(query_ptr qry, security_ptr security)
{
    qry->getFieldByName("iId", security->iId);
    qry->getFieldByName("sPasswordHash", security->sPasswordHash);
    qry->getFieldByName("sSaltHash", security->sSaltHash);
    qry->getFieldByName("sChallengeQuestion", security->sChallengeQuestion);
    qry->getFieldByName("sChallengeAnswerHash", security->sChallengeAnswerHash);
}


/**
 * @brief Security for Insert Statement
 *        This takes a pair, and translates to (Column, .. ) VALUES (%d, %Q,) for formatting
 * @param values
 */
void SecurityDao::fillColumnValues(query_ptr qry, security_ptr security, std::vector< std::pair<std::string, std::string> > &values)
{
    // values.push_back(qry->translateFieldName("iId", security->iId));
    values.push_back(qry->translateFieldName("sPasswordHash", security->sPasswordHash));
    values.push_back(qry->translateFieldName("sSaltHash", security->sSaltHash));
    values.push_back(qry->translateFieldName("sChallengeQuestion", security->sChallengeQuestion));
    values.push_back(qry->translateFieldName("sChallengeAnswerHash", security->sChallengeAnswerHash));
}


/**
 * @brief Create Security Record Insert Statement, returns query string
 */
std::string SecurityDao::insertSecurityQryString(query_ptr qry, security_ptr security)
{
    std::stringstream ssColumn;
    std::stringstream ssType;
    std::vector< std::pair<std::string, std::string> >::iterator it;
    std::vector< std::pair<std::string, std::string> > values;

    ssColumn << "INSERT INTO " << strTableName << " (";
    ssType << ") VALUES (";

    // Populate the Pairs.
    fillColumnValues(qry, security, values);

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
        security->sPasswordHash.c_str(),
        security->sSaltHash.c_str(),
        security->sChallengeQuestion.c_str(),
        security->sChallengeAnswerHash.c_str()
    );

    return result;
}


/**
 * @brief Update Existing Security Record.
 */
std::string SecurityDao::updateSecurityQryString(query_ptr qry, security_ptr security)
{
    std::stringstream ssColumn;
    std::vector< std::pair<std::string, std::string> >::iterator it;
    std::vector< std::pair<std::string, std::string> > values;

    // Setup start of Statement
    ssColumn << "UPDATE " << strTableName << " SET ";

    // Populate the Pairs. Variable = %Q formatted string.
    fillColumnValues(qry, security, values);

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
        security->sPasswordHash.c_str(),
        security->sSaltHash.c_str(),
        security->sChallengeQuestion.c_str(),
        security->sChallengeAnswerHash.c_str(),
        security->iId
    );

    return result;
}


/**
 * @brief Updates a Security Record in the database!
 * @param user
 * @return
 */
bool SecurityDao::updateSecurityRecord(security_ptr security)
{
    bool result = false;

    // Make Sure Database Reference is Connected
    if (!m_security_database.isConnected())
    {
        std::cout << "Error, Database is not connected!" << std::endl;
        return result;
    }

    // Create Pointer and Connect Query Object to Database.
    query_ptr qry(new SQLW::Query(m_security_database));
    if (!qry || !qry->isConnected())
    {
        std::cout << "Error, Query has no connection to the database" << std::endl;
        return result;
    }

    // Build update string
    std::string queryString = updateSecurityQryString(qry, security);

    // Execute Update in a Transaction, rollback if fails.
    std::vector<std::string> statements;
    statements.push_back(queryString);
    result = qry->executeTransaction(statements);

    return result;
}

/**
 * @brief Inserts a New Security Record in the database!
 * @param user
 * @return
 */
long SecurityDao::insertSecurityRecord(security_ptr security)
{    
    bool result = false;
    long lastInsertId = -1;

    // Make Sure Database Reference is Connected
    if (!m_security_database.isConnected())
    {
        std::cout << "Error, Database is not connected!" << std::endl;
        return result;
    }

    // Create Pointer and Connect Query Object to Database.
    query_ptr qry(new SQLW::Query(m_security_database));
    if (!qry || !qry->isConnected())
    {
        std::cout << "Error, Query has no connection to the database" << std::endl;
        return result;
    }

    // Build update string
    std::string queryString = insertSecurityQryString(qry, security);

    // Execute Update in a Transaction, rollback if fails.
    std::vector<std::string> statements;
    statements.push_back(queryString);
    result = qry->executeTransaction(statements);

    // We need the insert id for user table
    if (result)
    {
        lastInsertId = qry->getInsertId();
    }

    return lastInsertId;
}

/**
 * @brief Deletes a Security Record
 * @param securityId
 * @return
 */
bool SecurityDao::deleteSecurityRecord(long securityId)
{
    bool result = false;

    // Make Sure Database Reference is Connected
    if (!m_security_database.isConnected())
    {
        std::cout << "Error, Database is not connected!" << std::endl;
        return result;
    }

    // Create Pointer and Connect Query Object to Database.
    query_ptr qry(new SQLW::Query(m_security_database));
    if (!qry || !qry->isConnected())
    {
        std::cout << "Error, Query has no connection to the database" << std::endl;
        return result;
    }

    // Build string
    std::string queryString = sqlite3_mprintf("DELETE FROM %Q WHERE iId = %ld;", 
        strTableName.c_str(), securityId);

    // Execute Update in a Transaction, rollback if fails.
    std::vector<std::string> statements;
    statements.push_back(queryString);
    result = qry->executeTransaction(statements);

    return result;
}

/**
 * @brief Return User Security By Index ID.
 * @return
 */
security_ptr SecurityDao::getSecurityById(long securityId)
{
    security_ptr security(new Security());

    // Make Sure Database Reference is Connected
    if (!m_security_database.isConnected())
    {
        std::cout << "Error, Database is not connected!" << std::endl;
        return security;
    }

    // Create Pointer and Connect Query Object to Database.
    query_ptr qry(new SQLW::Query(m_security_database));
    if (!qry->isConnected())
    {
        std::cout << "Error, Query has no connection to the database" << std::endl;
        return security;
    }

    // Build Query String
    std::string queryString = sqlite3_mprintf("SELECT * FROM %Q WHERE iID = %ld;", 
        strTableName.c_str(), securityId);

    // Execute Query.
    if (qry->getResult(queryString))
    {
        long rows = qry->getNumRows();
        if (rows > 0)
        {
            qry->fetchRow();
            pullSecurityResult(qry, security);
        }
        else
        {
            std::cout << "Error, getSecurityById Returned Rows: " << rows << std::endl;
        }
    }
    else
    {
        std::cout << "Error, getResult()" << std::endl;
    }

    return security;
}



