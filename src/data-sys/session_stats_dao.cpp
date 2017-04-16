#include "session_stats_dao.hpp"

#include "../model-sys/session_stats.hpp"

#include "libSqliteWrapped.h"
#include <sqlite3.h>

#include <boost/make_shared.hpp>

#include <iostream>
#include <string>

SessionStatsDao::SessionStatsDao(SQLW::Database &database)
    : m_stats_database(database)
{
    // Setup Table name
    strTableName = "sessionstat";

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
    cmdSessionStatTableExists = "SELECT name FROM sqlite_master WHERE type='table' AND name='" + strTableName + "' COLLATE NOCASE;";

    // Create Users Table Query (SQLite Only for the moment)
    cmdCreateSessionStatTable =
        "CREATE TABLE IF NOT EXISTS " + strTableName + " ( "
        "iId               INTEGER PRIMARY KEY, "        
        "iUserId           INTEGER NOT NULL, "
        "sSessionType      TEXT NOT NULL COLLATE NOCASE, "
        "sCodePage         TEXT NOT NULL COLLATE NOCASE, "
        "sTerminal         TEXT NOT NULL COLLATE NOCASE, "
        "sIPAddress        TEXT NOT NULL COLLATE NOCASE, "
        "iTermWidth        INTEGER NOT NULL, "
        "iTermHeight       INTEGER NOT NULL, "
        "dtStartDate       DATETIME NOT NULL, "
        "dtEndDate         DATETIME NOT NULL, "
        "iInvalidAttempts  INTEGER NOT NULL, "
        "bLogonSuccess     BOOLEAN NOT NULL, "
        "bHungup           BOOLEAN NOT NULL, "
        "iMsgRead          INTEGER NOT NULL, "
        "iMsgPost          INTEGER NOT NULL, "
        "iFilesUl          INTEGER NOT NULL, "
        "iFilesDl          INTEGER NOT NULL, "
        "iFilesUlMb        INTEGER NOT NULL, "
        "iFilesDlMb        INTEGER NOT NULL, "
        "iDoorsExec        INTEGER NOT NULL  "
        "); ";


    cmdCreateSessionStatIndex = "";
        "CREATE INDEX IF NOT EXISTS session_stats_idx "
        "ON " + strTableName + " (iUserId); ";

    // CREATE INDEX `IDX_testtbl_Name` ON `testtbl` (`Name` COLLATE UTF8CI)
    cmdDropSessionStatTable = "DROP TABLE IF EXISTS " + strTableName + "; ";
    cmdDropSessionStatIndex = "DROP INDEX IF EXISTS session_stats_idx; ";
    
}

SessionStatsDao::~SessionStatsDao()
{
    std::cout << "~SessionStatsDao" << std::endl;
}

/**
 * @brief Check if the Table Exists in Database
 * @return
 */
bool SessionStatsDao::isTableExists()
{
    bool result = false;

    // Make Sure Database Reference is Connected
    if (!m_stats_database.isConnected())
    {
        std::cout << "Error, Database is not connected!" << std::endl;
        return result;
    }

    // Create Pointer and Connect Query Object to Database.
    query_ptr qry(new SQLW::Query(m_stats_database));
    if (!qry || !qry->isConnected())
    {
        std::cout << "Error, Query has no connection to the database" << std::endl;
        return result;
    }

    // Execute and get result.
    if (qry->getResult(cmdSessionStatTableExists))
    {
        long rows = qry->getNumRows();
        if (rows > 0)
        {
            std::cout << "Users Table Exists!" << std::endl;
            result = true;
        }
        else
        {
            // No rows means the table doesn't exist!
            std::cout << "Error, Users table Exists Returned Rows: " << rows << std::endl;
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
bool SessionStatsDao::firstTimeSetupParams()
{
    bool result = false;

    // Make Sure Database Reference is Connected
    if (!m_stats_database.isConnected())
    {
        std::cout << "Error, Database is not connected!" << std::endl;
        return result;
    }

    // Create Pointer and Connect Query Object to Database.
    query_ptr qry(new SQLW::Query(m_stats_database));
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
bool SessionStatsDao::createTable()
{
    bool result = false;

    // Make Sure Database Reference is Connected
    if (!m_stats_database.isConnected())
    {
        std::cout << "Error, Database is not connected!" << std::endl;
        return result;
    }

    // Create Pointer and Connect Query Object to Database.
    query_ptr qry(new SQLW::Query(m_stats_database));
    if (!qry || !qry->isConnected())
    {
        std::cout << "Error, Query has no connection to the database" << std::endl;
        return result;
    }

    // Create List of statements to execute in a single transaction.
    std::vector<std::string> statements;

    statements.push_back(cmdCreateSessionStatTable);
    statements.push_back(cmdCreateSessionStatIndex);

    // Execute Transaction.
    result = qry->executeTransaction(statements);
    return result;
}

/**
 * @brief Drop Users Table
 * If Drop Index Fails, still try to Drop Tables as it will try both!
 */
bool SessionStatsDao::dropTable()
{
    bool result = false;

    // Make Sure Database Reference is Connected
    if (!m_stats_database.isConnected())
    {
        std::cout << "Error, Database is not connected!" << std::endl;
        return result;
    }

    // Create Pointer and Connect Query Object to Database.
    query_ptr qry(new SQLW::Query(m_stats_database));
    if (!qry || !qry->isConnected())
    {
        std::cout << "Error, Query has no connection to the database" << std::endl;
        return result;
    }

    // Create List of statements to execute in a single transaction.
    std::vector<std::string> statements;
    statements.push_back(cmdDropSessionStatIndex);
    statements.push_back(cmdDropSessionStatTable);

    // Execute Transaction.
    result = qry->executeTransaction(statements);
    return result;
}

/**
 * @brief Pulls results by FieldNames into their Class Variables.
 */
void SessionStatsDao::pullSessionStatResult(query_ptr qry, session_stats_ptr stat)
{
    qry->getFieldByName("iId", stat->iId);
    qry->getFieldByName("iUserId", stat->iUserId);
    qry->getFieldByName("sSessionType", stat->sSessionType);
    qry->getFieldByName("sCodePage", stat->sCodePage);
    qry->getFieldByName("sTerminal", stat->sTerminal);
    qry->getFieldByName("sIPAddress", stat->sIPAddress);
    qry->getFieldByName("iTermWidth", stat->iTermWidth);
    qry->getFieldByName("iTermHeight", stat->iTermHeight);
    qry->getFieldByName("dtStartDate", stat->dtStartDate);
    qry->getFieldByName("dtEndDate", stat->dtEndDate);
    qry->getFieldByName("iInvalidAttempts", stat->iInvalidAttempts);
    qry->getFieldByName("bLogonSuccess", stat->bLogonSuccess);
    qry->getFieldByName("bHungup", stat->bHungup);
    qry->getFieldByName("iMsgRead", stat->iMsgRead);
    qry->getFieldByName("iMsgPost", stat->iMsgPost);
    qry->getFieldByName("iFilesUl", stat->iFilesUl);
    qry->getFieldByName("iFilesDl", stat->iFilesDl);
    qry->getFieldByName("iFilesUlMb", stat->iFilesUlMb);
    qry->getFieldByName("iFilesDlMb", stat->iFilesDlMb);
    qry->getFieldByName("iDoorsExec", stat->iDoorsExec);
}

/**
 * @brief Used for Insert Statement
 *        This takes a pair, and translates to (Column, .. ) VALUES (%d, %Q,) for formatting
 * @param values
 */
void SessionStatsDao::fillColumnValues(query_ptr qry, session_stats_ptr stat, std::vector< std::pair<std::string, std::string> > &values)
{
    // values.push_back(qry->translateFieldName("iId", stat->iId));
    values.push_back(qry->translateFieldName("iId", stat->iId));
    values.push_back(qry->translateFieldName("iUserId", stat->iUserId));
    values.push_back(qry->translateFieldName("sSessionType", stat->sSessionType));
    values.push_back(qry->translateFieldName("sCodePage", stat->sCodePage));
    values.push_back(qry->translateFieldName("sTerminal", stat->sTerminal));
    values.push_back(qry->translateFieldName("sIPAddress", stat->sIPAddress));
    values.push_back(qry->translateFieldName("iTermWidth", stat->iTermWidth));
    values.push_back(qry->translateFieldName("iTermHeight", stat->iTermHeight));
    values.push_back(qry->translateFieldName("dtStartDate", stat->dtStartDate));
    values.push_back(qry->translateFieldName("dtEndDate", stat->dtEndDate));    
    values.push_back(qry->translateFieldName("iInvalidAttempts", stat->iInvalidAttempts));
    values.push_back(qry->translateFieldName("bLogonSuccess", stat->bLogonSuccess));
    values.push_back(qry->translateFieldName("bHungup", stat->bHungup));
    values.push_back(qry->translateFieldName("iMsgRead", stat->iMsgRead));
    values.push_back(qry->translateFieldName("iMsgPost", stat->iMsgPost));
    values.push_back(qry->translateFieldName("iFilesUl", stat->iFilesUl));
    values.push_back(qry->translateFieldName("iFilesDl", stat->iFilesDl));
    values.push_back(qry->translateFieldName("iFilesUlMb", stat->iFilesUlMb));
    values.push_back(qry->translateFieldName("iFilesDlMb", stat->iFilesDlMb));
    values.push_back(qry->translateFieldName("iDoorsExec", stat->iDoorsExec));
}

/**
 * @brief Create User Record Insert Statement, returns query string
 */
std::string SessionStatsDao::insertSessionStatQryString(query_ptr qry, session_stats_ptr stat)
{
    std::stringstream ssColumn;
    std::stringstream ssType;
    std::vector< std::pair<std::string, std::string> >::iterator it;
    std::vector< std::pair<std::string, std::string> > values;

    ssColumn << "INSERT INTO " + strTableName + " (";
    ssType << ") VALUES (";

    // Populate the Pairs.
    fillColumnValues(qry, stat, values);

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
        stat->iId,
        stat->iUserId,
        stat->sSessionType.c_str(),
        stat->sCodePage.c_str(),
        stat->sTerminal.c_str(),
        stat->sIPAddress.c_str(),
        stat->iTermWidth,
        stat->iTermHeight,
        stat->dtStartDate,
        stat->dtEndDate,
        stat->iInvalidAttempts,
        stat->bLogonSuccess,
        stat->bHungup,
        stat->iMsgRead,
        stat->iMsgPost,
        stat->iFilesUl,
        stat->iFilesDl,
        stat->iFilesUlMb,
        stat->iFilesDlMb,
        stat->iDoorsExec    
    );

    return result;
}

/**
 * @brief Update Existing stat Record.
 */
std::string SessionStatsDao::updateSessionStatQryString(query_ptr qry, session_stats_ptr stat)
{
    std::stringstream ssColumn;
    std::vector< std::pair<std::string, std::string> >::iterator it;
    std::vector< std::pair<std::string, std::string> > values;

    // Setup start of Statement
    ssColumn << "UPDATE " + strTableName + " SET ";

    // Populate the Pairs. Variable = %Q formatted string.
    fillColumnValues(qry, stat, values);

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
        stat->iId,
        stat->iUserId,
        stat->sSessionType.c_str(),
        stat->sCodePage.c_str(),
        stat->sTerminal.c_str(),
        stat->sIPAddress.c_str(),
        stat->iTermWidth,
        stat->iTermHeight,
        stat->dtStartDate,
        stat->dtEndDate,
        stat->iInvalidAttempts,
        stat->bLogonSuccess,
        stat->bHungup,
        stat->iMsgRead,
        stat->iMsgPost,
        stat->iFilesUl,
        stat->iFilesDl,
        stat->iFilesUlMb,
        stat->iFilesDlMb,
        stat->iDoorsExec
    );

    return result;
}

/**
 * @brief Updates a User Record in the database!
 * @param stat
 * @return
 */
bool SessionStatsDao::updateSessionStatRecord(session_stats_ptr stat)
{
    bool result = false;

    // Make Sure Database Reference is Connected
    if (!m_stats_database.isConnected())
    {
        std::cout << "Error, Database is not connected!" << std::endl;
        return result;
    }

    // Create Pointer and Connect Query Object to Database.
    query_ptr qry(new SQLW::Query(m_stats_database));
    if (!qry || !qry->isConnected())
    {
        std::cout << "Error, Query has no connection to the database" << std::endl;
        return result;
    }

    // Build update string
    std::string queryString = updateSessionStatQryString(qry, stat);

    // Execute Update in a Transaction, rollback if fails.
    std::vector<std::string> statements;
    statements.push_back(queryString);
    result = qry->executeTransaction(statements);

    return result;
}

/**
 * @brief Inserts a New User Record in the database!
 * @param stat
 * @return
 */
long SessionStatsDao::insertSessionStatRecord(session_stats_ptr stat)
{    
    bool result = false;
    long lastInsertId = -1;

    // Make Sure Database Reference is Connected
    if (!m_stats_database.isConnected())
    {
        std::cout << "Error, Database is not connected!" << std::endl;
        return result;
    }

    // Create Pointer and Connect Query Object to Database.
    query_ptr qry(new SQLW::Query(m_stats_database));
    if (!qry || !qry->isConnected())
    {
        std::cout << "Error, Query has no connection to the database" << std::endl;
        return result;
    }

    // Build update string
    std::string queryString = insertSessionStatQryString(qry, stat);

    // Execute Update in a Transaction, rollback if fails.
    std::vector<std::string> statements;
    statements.push_back(queryString);
    result = qry->executeTransaction(statements);

    // We need the insert id for stat table
    if (result)
    {
        lastInsertId = qry->getInsertId();
    }

    return lastInsertId;
}

/**
 * @brief Deletes a User Record
 * @param statId
 * @return
 */
bool SessionStatsDao::deleteSessionStatRecord(long statId)
{
    bool result = false;

    // Make Sure Database Reference is Connected
    if (!m_stats_database.isConnected())
    {
        std::cout << "Error, Database is not connected!" << std::endl;
        return result;
    }

    // Create Pointer and Connect Query Object to Database.
    query_ptr qry(new SQLW::Query(m_stats_database));
    if (!qry || !qry->isConnected())
    {
        std::cout << "Error, Query has no connection to the database" << std::endl;
        return result;
    }

    // Build string
    std::string queryString = sqlite3_mprintf("DELETE FROM %Q WHERE iId = %ld;", strTableName.c_str(), statId);

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
session_stats_ptr SessionStatsDao::getSessionStatById(long id)
{
    session_stats_ptr stat(new SessionStats);

    // Make Sure Database Reference is Connected
    if (!m_stats_database.isConnected())
    {
        std::cout << "Error, Database is not connected!" << std::endl;
        return stat;
    }

    // Create Pointer and Connect Query Object to Database.
    query_ptr qry(new SQLW::Query(m_stats_database));
    if (!qry->isConnected())
    {
        std::cout << "Error, Query has no connection to the database" << std::endl;
        return stat;
    }

    // Build Query String
    std::string queryString = sqlite3_mprintf("SELECT * FROM %Q WHERE iID = %ld;", strTableName.c_str(), id);

    // Execute Query.
    if (qry->getResult(queryString))
    {
        long rows = qry->getNumRows();
        if (rows > 0)
        {
            qry->fetchRow();
            pullSessionStatResult(qry, stat);
        }
        else
        {
            std::cout << "Error, getSessionStatById Returned Rows: " << rows << std::endl;
        }
    }
    else
    {
        std::cout << "Error, getResult()" << std::endl;
    }

    return stat;
}

/**
 * @brief Return List of All Users
 * @return
 */
std::vector<session_stats_ptr> SessionStatsDao::getAllStatsPerUser(long userId)
{
    session_stats_ptr stat(new SessionStats);
    std::vector<session_stats_ptr> stat_list;

    // Make Sure Database Reference is Connected
    if (!m_stats_database.isConnected())
    {
        std::cout << "Error, Database is not connected!" << std::endl;
        return stat_list;
    }

    // Create Pointer and Connect Query Object to Database.
    query_ptr qry(new SQLW::Query(m_stats_database));
    if (!qry->isConnected())
    {
        std::cout << "Error, Query has no connection to the database" << std::endl;
        return stat_list;
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
                stat.reset(new SessionStats);
                pullSessionStatResult(qry, stat);
                stat_list.push_back(stat);
            }
        }
        else
        {
            std::cout << "Error, getAllStatsPerUser Returned Rows: " << rows << std::endl;
        }
    }
    else
    {
        std::cout << "Error, getResult()" << std::endl;
    }

    return stat_list;
}

/**
 * @brief Return List of All Users
 * @return
 */
std::vector<session_stats_ptr> SessionStatsDao::getLast10CallerStats()
{
    session_stats_ptr stat(new SessionStats);
    std::vector<session_stats_ptr> stat_list;

    // Make Sure Database Reference is Connected
    if (!m_stats_database.isConnected())
    {
        std::cout << "Error, Database is not connected!" << std::endl;
        return stat_list;
    }

    // Create Pointer and Connect Query Object to Database.
    query_ptr qry(new SQLW::Query(m_stats_database));
    if (!qry->isConnected())
    {
        std::cout << "Error, Query has no connection to the database" << std::endl;
        return stat_list;
    }

    // Build Query String
    std::string queryString = sqlite3_mprintf("SELECT * FROM %Q WHERE bLogonSuccess = '1' "
        "ORDER BY iID DESC;", strTableName.c_str());

    // Execute Query.
    if (qry->getResult(queryString))
    {
        long rows = qry->getNumRows();
        if (rows > 0)
        {
            while(qry->fetchRow())
            {
                stat.reset(new SessionStats);
                pullSessionStatResult(qry, stat);
                stat_list.push_back(stat);
            }
        }
        else
        {
            std::cout << "Error, getLast10CallerStats Returned Rows: " << rows << std::endl;
        }
    }
    else
    {
        std::cout << "Error, getResult()" << std::endl;
    }

    return stat_list;
}
