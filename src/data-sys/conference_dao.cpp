#include "conference_dao.hpp"
#include "../model-sys/conference.hpp"

#include "libSqliteWrapped.h"
#include <sqlite3.h>

#include <boost/make_shared.hpp>

#include <iostream>
#include <string>


ConferenceDao::ConferenceDao(SQLW::Database &database)
    : m_conference_database(database)
{
    // Setup Table name
    strTableName = "conference";

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
    cmdConferenceTableExists = "SELECT name FROM sqlite_master WHERE type='table' AND name='" + strTableName + "' COLLATE NOCASE;";

    // Create Users Table Query (SQLite Only for the moment)
    cmdCreateConferenceTable =
        "CREATE TABLE IF NOT EXISTS " + strTableName + " ( "
        "iId               INTEGER PRIMARY KEY, "
        "sName             TEXT NOT NULL COLLATE NOCASE, "
        "sType             TEXT NOT NULL COLLATE NOCASE, "
        "sACS              TEXT NOT NULL COLLATE NOCASE, "
        "iSortOrder        INTEGER NOT NULL "
        "); ";

    cmdCreateConferenceIndex = "";
        "CREATE INDEX IF NOT EXISTS conference_idx "
        "ON " + strTableName + " (sType COLLATE NOCASE); ";

    // CREATE INDEX `IDX_testtbl_Name` ON `testtbl` (`Name` COLLATE UTF8CI)
    cmdDropConferenceTable = "DROP TABLE IF EXISTS " + strTableName + "; ";
    cmdDropConferenceIndex = "DROP INDEX IF EXISTS conference_idx; ";
}

ConferenceDao::~ConferenceDao()
{
}

/**
 * @brief Check if the Table Exists in Database
 * @return
 */
bool ConferenceDao::isTableExists()
{
    bool result = false;

    // Make Sure Database Reference is Connected
    if (!m_conference_database.isConnected())
    {
        std::cout << "Error, Database is not connected!" << std::endl;
        return result;
    }

    // Create Pointer and Connect Query Object to Database.
    query_ptr qry(new SQLW::Query(m_conference_database));
    if (!qry || !qry->isConnected())
    {
        std::cout << "Error, Query has no connection to the database" << std::endl;
        return result;
    }

    // Execute and get result.
    if (qry->getResult(cmdConferenceTableExists))
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
bool ConferenceDao::firstTimeSetupParams()
{
    bool result = false;

    // Make Sure Database Reference is Connected
    if (!m_conference_database.isConnected())
    {
        std::cout << "Error, Database is not connected!" << std::endl;
        return result;
    }

    // Create Pointer and Connect Query Object to Database.
    query_ptr qry(new SQLW::Query(m_conference_database));
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
bool ConferenceDao::createTable()
{
    bool result = false;

    // Make Sure Database Reference is Connected
    if (!m_conference_database.isConnected())
    {
        std::cout << "Error, Database is not connected!" << std::endl;
        return result;
    }

    // Create Pointer and Connect Query Object to Database.
    query_ptr qry(new SQLW::Query(m_conference_database));
    if (!qry || !qry->isConnected())
    {
        std::cout << "Error, Query has no connection to the database" << std::endl;
        return result;
    }

    // Create List of statements to execute in a single transaction.
    std::vector<std::string> statements;

    statements.push_back(cmdCreateConferenceTable);
    statements.push_back(cmdCreateConferenceIndex);

    // Execute Transaction.
    result = qry->executeTransaction(statements);
    return result;  
}

/**
 * @brief Drop Users Table
 * If Drop Index Fails, still try to Drop Tables as it will try both!
 */
bool ConferenceDao::dropTable()
{
    bool result = false;

    // Make Sure Database Reference is Connected
    if (!m_conference_database.isConnected())
    {
        std::cout << "Error, Database is not connected!" << std::endl;
        return result;
    }

    // Create Pointer and Connect Query Object to Database.
    query_ptr qry(new SQLW::Query(m_conference_database));
    if (!qry || !qry->isConnected())
    {
        std::cout << "Error, Query has no connection to the database" << std::endl;
        return result;
    }

    // Create List of statements to execute in a single transaction.
    std::vector<std::string> statements;
    statements.push_back(cmdDropConferenceIndex);
    statements.push_back(cmdDropConferenceTable);

    // Execute Transaction.
    result = qry->executeTransaction(statements);
    return result;
}

/**
 * @brief Pulls results by FieldNames into their Class Variables. 
 * @param qry
 * @param conf
 */
void ConferenceDao::pullConferenceResult(query_ptr qry, conference_ptr conf)
{
    qry->getFieldByName("iId", conf->iId);
    qry->getFieldByName("sName", conf->sName);
    qry->getFieldByName("sType", conf->sType);
    qry->getFieldByName("sACS", conf->sACS);
    qry->getFieldByName("iSortOrder", conf->iSortOrder);    
}

/**
 * @brief Used for Insert Statement
 *        This takes a pair, and translates to (Column, .. ) VALUES (%d, %Q,) for formatting
 * @param qry
 * @param conf
 * @param values
 */ 
void ConferenceDao::fillColumnValues(query_ptr qry, conference_ptr conf, 
    std::vector< std::pair<std::string, std::string> > &values)
{
    // values.push_back(qry->translateFieldName("iId", conf->iId));
    values.push_back(qry->translateFieldName("sName", conf->sName));
    values.push_back(qry->translateFieldName("sType", conf->sType));
    values.push_back(qry->translateFieldName("sACS", conf->sACS));
    values.push_back(qry->translateFieldName("iSortOrder", conf->iSortOrder));    
}

/**
 * @brief Create Conference Record Insert Statement, returns query string 
 * @param qry
 * @param conf
 * @return 
 */
std::string ConferenceDao::insertConferenceQryString(query_ptr qry, conference_ptr conf)
{
    std::stringstream ssColumn;
    std::stringstream ssType;
    std::vector< std::pair<std::string, std::string> >::iterator it;
    std::vector< std::pair<std::string, std::string> > values;

    ssColumn << "INSERT INTO " + strTableName + " (";
    ssType << ") VALUES (";

    // Populate the Pairs.
    fillColumnValues(qry, conf, values);

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
        conf->sName.c_str(),
        conf->sType.c_str(),
        conf->sACS.c_str(),
        conf->iSortOrder
    );

    return result;
}

/**
 * @brief Update Existing Conference Record. 
 * @param qry
 * @param conf
 * @return 
 */
std::string ConferenceDao::updateConferenceQryString(query_ptr qry, conference_ptr conf)
{
    std::stringstream ssColumn;
    std::vector< std::pair<std::string, std::string> >::iterator it;
    std::vector< std::pair<std::string, std::string> > values;

    // Setup start of Statement
    ssColumn << "UPDATE " + strTableName + " SET ";

    // Populate the Pairs. Variable = %Q formatted string.
    fillColumnValues(qry, conf, values);

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
        conf->sName.c_str(),
        conf->sType.c_str(),
        conf->sACS.c_str(),
        conf->iSortOrder,
        conf->iId
    );

    return result;
}

/**
 * @brief Updates a Conference Record in the database!
 * @param conf
 * @return
 */
bool ConferenceDao::updateConferenceRecord(conference_ptr conf)
{
    bool result = false;

    // Make Sure Database Reference is Connected
    if (!m_conference_database.isConnected())
    {
        std::cout << "Error, Database is not connected!" << std::endl;
        return result;
    }

    // Create Pointer and Connect Query Object to Database.
    query_ptr qry(new SQLW::Query(m_conference_database));
    if (!qry || !qry->isConnected())
    {
        std::cout << "Error, Query has no connection to the database" << std::endl;
        return result;
    }

    // Build update string
    std::string queryString = updateConferenceQryString(qry, conf);

    // Execute Update in a Transaction, rollback if fails.
    std::vector<std::string> statements;
    statements.push_back(queryString);
    result = qry->executeTransaction(statements);

    return result;
}

/**
 * @brief Inserts a New Conference Record in the database!
 * @param conf
 * @return
 */
long ConferenceDao::insertConferenceRecord(conference_ptr conf)
{    
    bool result = false;
    long lastInsertId = -1;

    // Make Sure Database Reference is Connected
    if (!m_conference_database.isConnected())
    {
        std::cout << "Error, Database is not connected!" << std::endl;
        return result;
    }

    // Create Pointer and Connect Query Object to Database.
    query_ptr qry(new SQLW::Query(m_conference_database));
    if (!qry || !qry->isConnected())
    {
        std::cout << "Error, Query has no connection to the database" << std::endl;
        return result;
    }

    // Build update string
    std::string queryString = insertConferenceQryString(qry, conf);

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
 * @param confId
 * @return
 */
bool ConferenceDao::deleteConferenceRecord(long confId)
{
    bool result = false;

    // Make Sure Database Reference is Connected
    if (!m_conference_database.isConnected())
    {
        std::cout << "Error, Database is not connected!" << std::endl;
        return result;
    }

    // Create Pointer and Connect Query Object to Database.
    query_ptr qry(new SQLW::Query(m_conference_database));
    if (!qry || !qry->isConnected())
    {
        std::cout << "Error, Query has no connection to the database" << std::endl;
        return result;
    }

    // Build string
    std::string queryString = sqlite3_mprintf("DELETE FROM %Q WHERE iId = %ld;", strTableName.c_str(), confId);

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
conference_ptr ConferenceDao::getConferenceById(long confId)
{
    conference_ptr conf(new Conference);

    // Make Sure Database Reference is Connected
    if (!m_conference_database.isConnected())
    {
        std::cout << "Error, Database is not connected!" << std::endl;
        return conf;
    }

    // Create Pointer and Connect Query Object to Database.
    query_ptr qry(new SQLW::Query(m_conference_database));
    if (!qry->isConnected())
    {
        std::cout << "Error, Query has no connection to the database" << std::endl;
        return conf;
    }

    // Build Query String
    std::string queryString = sqlite3_mprintf("SELECT * FROM %Q WHERE iID = %ld;", strTableName.c_str(), confId);

    // Execute Query.
    if (qry->getResult(queryString))
    {
        long rows = qry->getNumRows();
        if (rows > 0)
        {
            qry->fetchRow();
            pullConferenceResult(qry, conf);
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

    return conf;
}

/**
 * @brief Return List of All Conference
 * @return
 */
std::vector<conference_ptr> ConferenceDao::getAllConferences()
{
    conference_ptr conf(new Conference);
    std::vector<conference_ptr> conf_list;

    // Make Sure Database Reference is Connected
    if (!m_conference_database.isConnected())
    {
        std::cout << "Error, Database is not connected!" << std::endl;
        return conf_list;
    }

    // Create Pointer and Connect Query Object to Database.
    query_ptr qry(new SQLW::Query(m_conference_database));
    if (!qry->isConnected())
    {
        std::cout << "Error, Query has no connection to the database" << std::endl;
        return conf_list;
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
                conf.reset(new Conference);
                pullConferenceResult(qry, conf);
                conf_list.push_back(conf);
            }
        }
        else
        {
            std::cout << "Error, getAllConference Returned Rows: " << rows << std::endl;
        }
    }
    else
    {
        std::cout << "Error, getResult()" << std::endl;
    }

    return conf_list;
}

/**
 * @brief Return List of All Conference by Type
 * @param type
 * @return
 */ 
std::vector<conference_ptr> ConferenceDao::getAllConferencesByType(std::string type)
{
    conference_ptr conf(new Conference);
    std::vector<conference_ptr> conf_list;

    // Make Sure Database Reference is Connected
    if (!m_conference_database.isConnected())
    {
        std::cout << "Error, Database is not connected!" << std::endl;
        return conf_list;
    }

    // Create Pointer and Connect Query Object to Database.
    query_ptr qry(new SQLW::Query(m_conference_database));
    if (!qry->isConnected())
    {
        std::cout << "Error, Query has no connection to the database" << std::endl;
        return conf_list;
    }

    // Build Query String
    std::string queryString = sqlite3_mprintf("SELECT * FROM %Q WHERE sType like %Q;", strTableName.c_str(), type.c_str());

    // Execute Query.
    if (qry->getResult(queryString))
    {
        long rows = qry->getNumRows();
        if (rows > 0)
        {
            while(qry->fetchRow())
            {
                conf.reset(new Conference);
                pullConferenceResult(qry, conf);
                conf_list.push_back(conf);
            }
        }
        else
        {
            std::cout << "Error, getAllConferencesByType Returned Rows: " << rows << std::endl;
        }
    }
    else
    {
        std::cout << "Error, getResult()" << std::endl;
    }

    return conf_list;
}

/**
 * @brief Return Count or Number of Existing Conferences by Type
 * @param type
 * @return
 */ 
long ConferenceDao::getConferencesCountByType(std::string type)
{
    conference_ptr conf(new Conference);
    std::vector<conference_ptr> conf_list;

    // Make Sure Database Reference is Connected
    if (!m_conference_database.isConnected())
    {
        std::cout << "Error, Database is not connected!" << std::endl;
        return conf_list.size();
    }

    // Create Pointer and Connect Query Object to Database.
    query_ptr qry(new SQLW::Query(m_conference_database));
    if (!qry->isConnected())
    {
        std::cout << "Error, Query has no connection to the database" << std::endl;
        return conf_list.size();
    }

    // Build Query String
    std::string queryString = sqlite3_mprintf("SELECT * FROM %Q WHERE sType like %Q;", strTableName.c_str(), type.c_str());

    // Execute Query.
    if (qry->getResult(queryString))
    {
        long rows = qry->getNumRows();
        if (rows > 0)
        {
            while(qry->fetchRow())
            {
                conf.reset(new Conference);
                pullConferenceResult(qry, conf);
                conf_list.push_back(conf);
            }
        }
        else
        {
            std::cout << "Error, getConferenceCount By Type Returned Rows: " << rows << std::endl;
        }
    }
    else
    {
        std::cout << "Error, getResult()" << std::endl;
    }

    return conf_list.size();
}
