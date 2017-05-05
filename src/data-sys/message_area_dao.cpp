#include "message_area_dao.hpp"

#include "../model-sys/message_area.hpp"

#include "libSqliteWrapped.h"
#include <sqlite3.h>

#include <boost/make_shared.hpp>

#include <iostream>
#include <string>

MessageAreaDao::MessageAreaDao(SQLW::Database &database)
    : m_msg_area_database(database)
{
    // Setup Table name
    strTableName = "messagearea";

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
    cmdMessageAreaTableExists = "SELECT name FROM sqlite_master WHERE type='table' AND name='" + strTableName + "' COLLATE NOCASE;";

    // Create Table Query (SQLite Only for the moment)
    cmdCreateMessageAreaTable =
        "CREATE TABLE IF NOT EXISTS " + strTableName + " ( "
        "iId               INTEGER PRIMARY KEY, "
        "sName             TEXT NOT NULL COLLATE NOCASE, "
        "sAcsAccess        TEXT NOT NULL COLLATE NOCASE, "
        "sAcsPost          TEXT NOT NULL COLLATE NOCASE, "
        "bAnonymous        BOOLEAN NOT NULL, "
        "sSponsor          TEXT NOT NULL COLLATE NOCASE, "
        "sOriginLine       TEXT NOT NULL COLLATE NOCASE, "
        "sFidoPath         TEXT NOT NULL COLLATE NOCASE, "
        "iNetworkId        INTEGER NOT NULL, "
        "sQwkName          TEXT NOT NULL COLLATE NOCASE, "
        "iMaxMessages      INTEGER NOT NULL, "
        "bRealName         BOOLEAN NOT NULL, "
        "sLinkname         TEXT NOT NULL COLLATE NOCASE, "
        "bRequired         BOOLEAN NOT NULL, "
        "bPrivate          BOOLEAN NOT NULL, "
        "bNetmail          BOOLEAN NOT NULL, "
        "iSortOrder        INTEGER NOT NULL "
        "); ";

    // CREATE INDEX `IDX_testtbl_Name` ON `testtbl` (`Name` COLLATE UTF8CI)
    cmdDropMessageAreaTable = "DROP TABLE IF EXISTS " + strTableName + "; ";
}

MessageAreaDao::~MessageAreaDao()
{
}

/**
 * @brief Check if the Table Exists in Database
 * @return
 */
bool MessageAreaDao::isTableExists()
{
    bool result = false;

    // Make Sure Database Reference is Connected
    if (!m_msg_area_database.isConnected())
    {
        std::cout << "Error, Database is not connected!" << std::endl;
        return result;
    }

    // Create Pointer and Connect Query Object to Database.
    query_ptr qry(new SQLW::Query(m_msg_area_database));
    if (!qry || !qry->isConnected())
    {
        std::cout << "Error, Query has no connection to the database" << std::endl;
        return result;
    }

    // Execute and get result.
    if (qry->getResult(cmdMessageAreaTableExists))
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
bool MessageAreaDao::firstTimeSetupParams()
{
    bool result = false;

    // Make Sure Database Reference is Connected
    if (!m_msg_area_database.isConnected())
    {
        std::cout << "Error, Database is not connected!" << std::endl;
        return result;
    }

    // Create Pointer and Connect Query Object to Database.
    query_ptr qry(new SQLW::Query(m_msg_area_database));
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
bool MessageAreaDao::createTable()
{
    bool result = false;

    // Make Sure Database Reference is Connected
    if (!m_msg_area_database.isConnected())
    {
        std::cout << "Error, Database is not connected!" << std::endl;
        return result;
    }

    // Create Pointer and Connect Query Object to Database.
    query_ptr qry(new SQLW::Query(m_msg_area_database));
    if (!qry || !qry->isConnected())
    {
        std::cout << "Error, Query has no connection to the database" << std::endl;
        return result;
    }

    // Create List of statements to execute in a single transaction.
    std::vector<std::string> statements;
    statements.push_back(cmdCreateMessageAreaTable);

    // Execute Transaction.
    result = qry->executeTransaction(statements);
    return result;  
}

/**
 * @brief Drop Table
 * If Drop Index Fails, still try to Drop Tables as it will try both!
 */
bool MessageAreaDao::dropTable()
{
    bool result = false;

    // Make Sure Database Reference is Connected
    if (!m_msg_area_database.isConnected())
    {
        std::cout << "Error, Database is not connected!" << std::endl;
        return result;
    }

    // Create Pointer and Connect Query Object to Database.
    query_ptr qry(new SQLW::Query(m_msg_area_database));
    if (!qry || !qry->isConnected())
    {
        std::cout << "Error, Query has no connection to the database" << std::endl;
        return result;
    }

    // Create List of statements to execute in a single transaction.
    std::vector<std::string> statements;
    statements.push_back(cmdDropMessageAreaTable);

    // Execute Transaction.
    result = qry->executeTransaction(statements);
    return result;
}

/**
 * @brief Pulls results by FieldNames into their Class Variables. 
 * @param qry
 * @param area
 */
void MessageAreaDao::pullMessageAreaResult(query_ptr qry, msg_area_ptr area)
{
    qry->getFieldByName("iId", area->iId);
    qry->getFieldByName("sName", area->sName);    
    qry->getFieldByName("sAcsAccess", area->sAcsAccess);
    qry->getFieldByName("sAcsAccess", area->sAcsAccess);
    qry->getFieldByName("sAcsPost", area->sAcsPost);
    qry->getFieldByName("bAnonymous", area->bAnonymous);
    qry->getFieldByName("sSponsor", area->sSponsor);
    qry->getFieldByName("sOriginLine", area->sOriginLine);
    qry->getFieldByName("sFidoPath", area->sFidoPath);
    qry->getFieldByName("iNetworkId", area->iNetworkId);
    qry->getFieldByName("sQwkName", area->sQwkName);
    qry->getFieldByName("iMaxMessages", area->iMaxMessages);
    qry->getFieldByName("bRealName", area->bRealName);
    qry->getFieldByName("sLinkname", area->sLinkname);
    qry->getFieldByName("bRequired", area->bRequired);
    qry->getFieldByName("bPrivate", area->bPrivate);
    qry->getFieldByName("bNetmail", area->bNetmail);
    qry->getFieldByName("iSortOrder", area->iSortOrder);
}

/**
 * @brief Used for Insert Statement
 *        This takes a pair, and translates to (Column, .. ) VALUES (%d, %Q,) for formatting
 * @param qry
 * @param area
 * @param values
 */ 
void MessageAreaDao::fillColumnValues(query_ptr qry, msg_area_ptr area, 
    std::vector< std::pair<std::string, std::string> > &values)
{
    // values.push_back(qry->translateFieldName("iId", conf->iId));
    values.push_back(qry->translateFieldName("sName", area->sName));
    values.push_back(qry->translateFieldName("sAcsAccess", area->sAcsAccess));
    values.push_back(qry->translateFieldName("sAcsPost", area->sAcsPost));
    values.push_back(qry->translateFieldName("bAnonymous", area->bAnonymous));
    values.push_back(qry->translateFieldName("sSponsor", area->sSponsor));
    values.push_back(qry->translateFieldName("sOriginLine", area->sOriginLine));
    values.push_back(qry->translateFieldName("sFidoPath", area->sFidoPath));
    values.push_back(qry->translateFieldName("iNetworkId", area->iNetworkId));
    values.push_back(qry->translateFieldName("sQwkName", area->sQwkName));
    values.push_back(qry->translateFieldName("iMaxMessages", area->iMaxMessages));
    values.push_back(qry->translateFieldName("bRealName", area->bRealName));
    values.push_back(qry->translateFieldName("sLinkname", area->sLinkname));
    values.push_back(qry->translateFieldName("bRequired", area->bRequired));
    values.push_back(qry->translateFieldName("bPrivate", area->bPrivate));
    values.push_back(qry->translateFieldName("bNetmail", area->bNetmail));
    values.push_back(qry->translateFieldName("iSortOrder", area->iSortOrder));
}

/**
 * @brief Create MessageArea Record Insert Statement, returns query string 
 * @param qry
 * @param area
 * @return 
 */
std::string MessageAreaDao::insertMessageAreaQryString(query_ptr qry, msg_area_ptr area)
{
    std::stringstream ssColumn;
    std::stringstream ssType;
    std::vector< std::pair<std::string, std::string> >::iterator it;
    std::vector< std::pair<std::string, std::string> > values;

    ssColumn << "INSERT INTO " + strTableName + " (";
    ssType << ") VALUES (";

    // Populate the Pairs.
    fillColumnValues(qry, area, values);

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
        area->sName.c_str(),
        area->sAcsAccess.c_str(),
        area->sAcsPost.c_str(),
        area->bAnonymous,
        area->sSponsor.c_str(),  
        area->sOriginLine.c_str(),
        area->sFidoPath.c_str(),
        area->iNetworkId,
        area->sQwkName.c_str(),
        area->iMaxMessages,
        area->bRealName,
        area->sLinkname.c_str(),
        area->bRequired,
        area->bPrivate,
        area->bNetmail,
        area->iSortOrder
    );

    return result;
}

/**
 * @brief Update Existing MessageArea Record. 
 * @param qry
 * @param area
 * @return 
 */
std::string MessageAreaDao::updateMessageAreaQryString(query_ptr qry, msg_area_ptr area)
{
    std::stringstream ssColumn;
    std::vector< std::pair<std::string, std::string> >::iterator it;
    std::vector< std::pair<std::string, std::string> > values;

    // Setup start of Statement
    ssColumn << "UPDATE " + strTableName + " SET ";

    // Populate the Pairs. Variable = %Q formatted string.
    fillColumnValues(qry, area, values);

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
        area->sName.c_str(),
        area->sAcsAccess.c_str(),
        area->sAcsPost.c_str(),
        area->bAnonymous,
        area->sSponsor.c_str(),  
        area->sOriginLine.c_str(),
        area->sFidoPath.c_str(),
        area->iNetworkId,
        area->sQwkName.c_str(),
        area->iMaxMessages,
        area->bRealName,
        area->sLinkname.c_str(),
        area->bRequired,
        area->bPrivate,
        area->bNetmail,
        area->iSortOrder,
        area->iId
    );

    return result;
}

/**
 * @brief Updates a MessageArea Record in the database!
 * @param area
 * @return
 */
bool MessageAreaDao::updateMessageAreaRecord(msg_area_ptr area)
{
    bool result = false;

    // Make Sure Database Reference is Connected
    if (!m_msg_area_database.isConnected())
    {
        std::cout << "Error, Database is not connected!" << std::endl;
        return result;
    }

    // Create Pointer and Connect Query Object to Database.
    query_ptr qry(new SQLW::Query(m_msg_area_database));
    if (!qry || !qry->isConnected())
    {
        std::cout << "Error, Query has no connection to the database" << std::endl;
        return result;
    }

    // Build update string
    std::string queryString = updateMessageAreaQryString(qry, area);

    // Execute Update in a Transaction, rollback if fails.
    std::vector<std::string> statements;
    statements.push_back(queryString);
    result = qry->executeTransaction(statements);

    return result;
}

/**
 * @brief Inserts a new MessageArea Record in the database!
 * @param area
 * @return
 */
long MessageAreaDao::insertMessageAreaRecord(msg_area_ptr area)
{    
    bool result = false;
    long lastInsertId = -1;

    // Make Sure Database Reference is Connected
    if (!m_msg_area_database.isConnected())
    {
        std::cout << "Error, Database is not connected!" << std::endl;
        return result;
    }

    // Create Pointer and Connect Query Object to Database.
    query_ptr qry(new SQLW::Query(m_msg_area_database));
    if (!qry || !qry->isConnected())
    {
        std::cout << "Error, Query has no connection to the database" << std::endl;
        return result;
    }

    // Build update string
    std::string queryString = insertMessageAreaQryString(qry, area);

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
 * @brief Deletes a MessageArea Record
 * @param areaId
 * @return
 */
bool MessageAreaDao::deleteMessageAreaRecord(long areaId)
{
    bool result = false;

    // Make Sure Database Reference is Connected
    if (!m_msg_area_database.isConnected())
    {
        std::cout << "Error, Database is not connected!" << std::endl;
        return result;
    }

    // Create Pointer and Connect Query Object to Database.
    query_ptr qry(new SQLW::Query(m_msg_area_database));
    if (!qry || !qry->isConnected())
    {
        std::cout << "Error, Query has no connection to the database" << std::endl;
        return result;
    }

    // Build string
    std::string queryString = sqlite3_mprintf("DELETE FROM %Q WHERE iId = %ld;", strTableName.c_str(), areaId);

    // Execute Update in a Transaction, rollback if fails.
    std::vector<std::string> statements;
    statements.push_back(queryString);
    result = qry->executeTransaction(statements);

    return result;
}

/**
 * @brief Return MessageArea Record By Index ID.
 * @return
 */
msg_area_ptr MessageAreaDao::getMessageAreaById(long areaId)
{
    msg_area_ptr area(new MessageArea);

    // Make Sure Database Reference is Connected
    if (!m_msg_area_database.isConnected())
    {
        std::cout << "Error, Database is not connected!" << std::endl;
        return area;
    }

    // Create Pointer and Connect Query Object to Database.
    query_ptr qry(new SQLW::Query(m_msg_area_database));
    if (!qry->isConnected())
    {
        std::cout << "Error, Query has no connection to the database" << std::endl;
        return area;
    }

    // Build Query String
    std::string queryString = sqlite3_mprintf("SELECT * FROM %Q WHERE iID = %ld;", strTableName.c_str(), areaId);

    // Execute Query.
    if (qry->getResult(queryString))
    {
        long rows = qry->getNumRows();
        if (rows > 0)
        {
            qry->fetchRow();
            pullMessageAreaResult(qry, area);
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

    return area;
}

/**
 * @brief Return List of All MessageAreas
 * @return
 */
std::vector<msg_area_ptr> MessageAreaDao::getAllMessageAreas()
{
    msg_area_ptr area(new MessageArea);
    std::vector<msg_area_ptr> area_list;

    // Make Sure Database Reference is Connected
    if (!m_msg_area_database.isConnected())
    {
        std::cout << "Error, Database is not connected!" << std::endl;
        return area_list;
    }

    // Create Pointer and Connect Query Object to Database.
    query_ptr qry(new SQLW::Query(m_msg_area_database));
    if (!qry->isConnected())
    {
        std::cout << "Error, Query has no connection to the database" << std::endl;
        return area_list;
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
                area.reset(new MessageArea);
                pullMessageAreaResult(qry, area);
                area_list.push_back(area);
            }
        }
        else
        {
            std::cout << "Error, getAllMessageAreas Returned Rows: " << rows << std::endl;
        }
    }
    else
    {
        std::cout << "Error, getResult()" << std::endl;
    }

    return area_list;
}

/**
 * @brief Return List of All MessageArea by ConferenceId
 * @param areas
 * @return
 */ 
std::vector<msg_area_ptr> MessageAreaDao::getAllMessageAreasByConference(long confId)
{
    msg_area_ptr area(new MessageArea);
    std::vector<msg_area_ptr> area_list;

    // Make Sure Database Reference is Connected
    if (!m_msg_area_database.isConnected())
    {
        std::cout << "Error, Database is not connected!" << std::endl;
        return area_list;
    }

    // Create Pointer and Connect Query Object to Database.
    query_ptr qry(new SQLW::Query(m_msg_area_database));
    if (!qry->isConnected())
    {
        std::cout << "Error, Query has no connection to the database" << std::endl;
        return area_list;
    }

    // Build Query String
    std::string queryString = sqlite3_mprintf("SELECT a.* FROM %Q a, Grouping g WHERE g.iConferenceId = %ld AND a.iID = g.iMsgAreaId;", strTableName.c_str(), confId);

    // Execute Query.
    if (qry->getResult(queryString))
    {
        long rows = qry->getNumRows();
        if (rows > 0)
        {
            while(qry->fetchRow())
            {
                area.reset(new MessageArea);
                pullMessageAreaResult(qry, area);
                area_list.push_back(area);
            }
        }
        else
        {
            std::cout << "Error, getAllMessageAreasByConference Returned Rows: " << rows << std::endl;
        }
    }
    else
    {
        std::cout << "Error, getResult()" << std::endl;
    }

    return area_list;
}
