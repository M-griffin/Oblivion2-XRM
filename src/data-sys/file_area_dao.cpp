#include "file_area_dao.hpp"

#include "../model-sys/file_area.hpp"

#include "libSqliteWrapped.h"
#include <sqlite3.h>

#include <boost/make_shared.hpp>

#include <iostream>
#include <string>

FileAreaDao::FileAreaDao(SQLW::Database &database)
    : m_file_area_database(database)
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
    cmdFileAreaTableExists = "SELECT name FROM sqlite_master WHERE type='table' AND name='" + strTableName + "' COLLATE NOCASE;";

    // Create Users Table Query (SQLite Only for the moment)
    cmdCreateFileAreaTable =
        "CREATE TABLE IF NOT EXISTS " + strTableName + " ( "
        "iId               INTEGER PRIMARY KEY, "
        "sName             TEXT NOT NULL COLLATE NOCASE, "
        "sAcsAccess        TEXT NOT NULL COLLATE NOCASE, "
        "sAcsUpload        TEXT NOT NULL COLLATE NOCASE, "
        "sAcsDownload      TEXT NOT NULL COLLATE NOCASE, "
        "sAcsList          TEXT NOT NULL COLLATE NOCASE, "
        "sSponsor          TEXT NOT NULL COLLATE NOCASE, "
        "iSecurityIndex    INT NOT NULL, "
        "sLinkname         TEXT NOT NULL COLLATE NOCASE, "
        "sSort             TEXT NOT NULL COLLATE NOCASE, "
        "iMultiplier       INTEGER NOT NULL, "
        "bFreeArea         BOOLEAN NOT NULL, "
        "iSortOrder        INTEGER NOT NULL, "
        "FOREIGN KEY(iSecurityIndex) REFERENCES Secutiry(iId) ON DELETE CASCADE "
        "); ";

    // CREATE INDEX `IDX_testtbl_Name` ON `testtbl` (`Name` COLLATE UTF8CI)
    cmdDropFileAreaTable = "DROP TABLE IF EXISTS " + strTableName + "; ";

}

FileAreaDao::~FileAreaDao()
{
}

/**
 * @brief Check if the Table Exists in Database
 * @return
 */
bool FileAreaDao::isTableExists()
{
    bool result = false;

    // Make Sure Database Reference is Connected
    if (!m_file_area_database.isConnected())
    {
        std::cout << "Error, Database is not connected!" << std::endl;
        return result;
    }

    // Create Pointer and Connect Query Object to Database.
    query_ptr qry(new SQLW::Query(m_file_area_database));
    if (!qry || !qry->isConnected())
    {
        std::cout << "Error, Query has no connection to the database" << std::endl;
        return result;
    }

    // Execute and get result.
    if (qry->getResult(cmdFileAreaTableExists))
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
bool FileAreaDao::firstTimeSetupParams()
{
    bool result = false;

    // Make Sure Database Reference is Connected
    if (!m_file_area_database.isConnected())
    {
        std::cout << "Error, Database is not connected!" << std::endl;
        return result;
    }

    // Create Pointer and Connect Query Object to Database.
    query_ptr qry(new SQLW::Query(m_file_area_database));
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
bool FileAreaDao::createTable()
{
    bool result = false;

    // Make Sure Database Reference is Connected
    if (!m_file_area_database.isConnected())
    {
        std::cout << "Error, Database is not connected!" << std::endl;
        return result;
    }

    // Create Pointer and Connect Query Object to Database.
    query_ptr qry(new SQLW::Query(m_file_area_database));
    if (!qry || !qry->isConnected())
    {
        std::cout << "Error, Query has no connection to the database" << std::endl;
        return result;
    }

    // Create List of statements to execute in a single transaction.
    std::vector<std::string> statements;

    statements.push_back(cmdCreateFileAreaTable);

    // Execute Transaction.
    result = qry->executeTransaction(statements);
    return result;  
}

/**
 * @brief Drop Users Table
 * If Drop Index Fails, still try to Drop Tables as it will try both!
 */
bool FileAreaDao::dropTable()
{
    bool result = false;

    // Make Sure Database Reference is Connected
    if (!m_file_area_database.isConnected())
    {
        std::cout << "Error, Database is not connected!" << std::endl;
        return result;
    }

    // Create Pointer and Connect Query Object to Database.
    query_ptr qry(new SQLW::Query(m_file_area_database));
    if (!qry || !qry->isConnected())
    {
        std::cout << "Error, Query has no connection to the database" << std::endl;
        return result;
    }

    // Create List of statements to execute in a single transaction.
    std::vector<std::string> statements;
    statements.push_back(cmdDropFileAreaTable);

    // Execute Transaction.
    result = qry->executeTransaction(statements);
    return result;
}

/**
 * @brief Pulls results by FieldNames into their Class Variables. 
 * @param qry
 * @param area
 */
void FileAreaDao::pullFileAreaResult(query_ptr qry, file_area_ptr area)
{
    qry->getFieldByName("iId", area->iId);
    qry->getFieldByName("sName", area->sName);    
    qry->getFieldByName("sAcsAccess", area->sAcsAccess);
    qry->getFieldByName("sAcsUpload", area->sAcsUpload);
    qry->getFieldByName("sAcsDownload", area->sAcsDownload);
    qry->getFieldByName("sAcsList", area->sAcsList);
    qry->getFieldByName("sSponsor", area->sSponsor);
    qry->getFieldByName("iSecurityIndex", area->iSecurityIndex);
    qry->getFieldByName("sLinkname", area->sLinkname);
    qry->getFieldByName("sSort", area->sSort);
    qry->getFieldByName("iMultiplier", area->iMultiplier);
    qry->getFieldByName("bFreeArea", area->bFreeArea);
    qry->getFieldByName("iSortOrder", area->iSortOrder);    
}

/**
 * @brief Used for Insert Statement
 *        This takes a pair, and translates to (Column, .. ) VALUES (%d, %Q,) for formatting
 * @param qry
 * @param area
 * @param values
 */ 
void FileAreaDao::fillColumnValues(query_ptr qry, file_area_ptr area, 
    std::vector< std::pair<std::string, std::string> > &values)
{
    // values.push_back(qry->translateFieldName("iId", conf->iId));
    values.push_back(qry->translateFieldName("sName", area->sName));
    values.push_back(qry->translateFieldName("sAcsAccess", area->sAcsAccess));
    values.push_back(qry->translateFieldName("sAcsUpload", area->sAcsUpload));
    values.push_back(qry->translateFieldName("sAcsDownload", area->sAcsDownload));
    values.push_back(qry->translateFieldName("sAcsList", area->sAcsList));
    values.push_back(qry->translateFieldName("sSponsor", area->sSponsor));
    values.push_back(qry->translateFieldName("iSecurityIndex", area->iSecurityIndex));
    values.push_back(qry->translateFieldName("sLinkname", area->sLinkname));
    values.push_back(qry->translateFieldName("sSort", area->sSort));
    values.push_back(qry->translateFieldName("iMultiplier", area->iMultiplier));
    values.push_back(qry->translateFieldName("bFreeArea", area->bFreeArea));
    values.push_back(qry->translateFieldName("iSortOrder", area->iSortOrder));
}

/**
 * @brief Create FileArea Record Insert Statement, returns query string 
 * @param qry
 * @param area
 * @return 
 */
std::string FileAreaDao::insertFileAreaQryString(query_ptr qry, file_area_ptr area)
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
        area->sAcsUpload.c_str(),
        area->sAcsDownload.c_str(),
        area->sAcsList.c_str(),
        area->sSponsor.c_str(),
        area->iSecurityIndex,
        area->sLinkname.c_str(),
        area->sSort.c_str(),
        area->iMultiplier,
        area->bFreeArea,
        area->iSortOrder
    );

    return result;
}

/**
 * @brief Update Existing FileArea Record. 
 * @param qry
 * @param area
 * @return 
 */
std::string FileAreaDao::updateFileAreaQryString(query_ptr qry, file_area_ptr area)
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
        area->sAcsUpload.c_str(),
        area->sAcsDownload.c_str(),
        area->sAcsList.c_str(),
        area->sSponsor.c_str(),
        area->iSecurityIndex,
        area->sLinkname.c_str(),
        area->sSort.c_str(),
        area->iMultiplier,
        area->bFreeArea,
        area->iSortOrder,
        area->iId
    );

    return result;
}

/**
 * @brief Updates a Conference Record in the database!
 * @param area
 * @return
 */
bool FileAreaDao::updateFileAreaRecord(file_area_ptr area)
{
    bool result = false;

    // Make Sure Database Reference is Connected
    if (!m_file_area_database.isConnected())
    {
        std::cout << "Error, Database is not connected!" << std::endl;
        return result;
    }

    // Create Pointer and Connect Query Object to Database.
    query_ptr qry(new SQLW::Query(m_file_area_database));
    if (!qry || !qry->isConnected())
    {
        std::cout << "Error, Query has no connection to the database" << std::endl;
        return result;
    }

    // Build update string
    std::string queryString = updateFileAreaQryString(qry, area);

    // Execute Update in a Transaction, rollback if fails.
    std::vector<std::string> statements;
    statements.push_back(queryString);
    result = qry->executeTransaction(statements);

    return result;
}

/**
 * @brief Inserts a New FileArea Record in the database!
 * @param area
 * @return
 */
long FileAreaDao::insertFileAreaRecord(file_area_ptr area)
{    
    bool result = false;
    long lastInsertId = -1;

    // Make Sure Database Reference is Connected
    if (!m_file_area_database.isConnected())
    {
        std::cout << "Error, Database is not connected!" << std::endl;
        return result;
    }

    // Create Pointer and Connect Query Object to Database.
    query_ptr qry(new SQLW::Query(m_file_area_database));
    if (!qry || !qry->isConnected())
    {
        std::cout << "Error, Query has no connection to the database" << std::endl;
        return result;
    }

    // Build update string
    std::string queryString = insertFileAreaQryString(qry, area);

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
 * @brief Deletes a FileArea Record
 * @param areaId
 * @return
 */
bool FileAreaDao::deleteFileAreaRecord(long areaId)
{
    bool result = false;

    // Make Sure Database Reference is Connected
    if (!m_file_area_database.isConnected())
    {
        std::cout << "Error, Database is not connected!" << std::endl;
        return result;
    }

    // Create Pointer and Connect Query Object to Database.
    query_ptr qry(new SQLW::Query(m_file_area_database));
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
 * @brief Return FileArea Record By Index ID.
 * @return
 */
file_area_ptr FileAreaDao::getFileAreaById(long areaId)
{
    file_area_ptr area(new FileArea);

    // Make Sure Database Reference is Connected
    if (!m_file_area_database.isConnected())
    {
        std::cout << "Error, Database is not connected!" << std::endl;
        return area;
    }

    // Create Pointer and Connect Query Object to Database.
    query_ptr qry(new SQLW::Query(m_file_area_database));
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
            pullFileAreaResult(qry, area);
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
 * @brief Return List of All FileAreas
 * @return
 */
std::vector<file_area_ptr> FileAreaDao::getAllFileAreas()
{
    file_area_ptr area(new FileArea);
    std::vector<file_area_ptr> area_list;

    // Make Sure Database Reference is Connected
    if (!m_file_area_database.isConnected())
    {
        std::cout << "Error, Database is not connected!" << std::endl;
        return area_list;
    }

    // Create Pointer and Connect Query Object to Database.
    query_ptr qry(new SQLW::Query(m_file_area_database));
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
                area.reset(new FileArea);
                pullFileAreaResult(qry, area);
                area_list.push_back(area);
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

    return area_list;
}

/**
 * @brief Return List of All FileArea by Type
 * @param areas
 * @return
 */ 
std::vector<file_area_ptr> FileAreaDao::getAllFileAreasByConference(uint32_t areas)
{
    file_area_ptr area(new FileArea);
    std::vector<file_area_ptr> area_list;

    // Make Sure Database Reference is Connected
    if (!m_file_area_database.isConnected())
    {
        std::cout << "Error, Database is not connected!" << std::endl;
        return area_list;
    }

    // Create Pointer and Connect Query Object to Database.
    query_ptr qry(new SQLW::Query(m_file_area_database));
    if (!qry->isConnected())
    {
        std::cout << "Error, Query has no connection to the database" << std::endl;
        return area_list;
    }


    // TODO, Build list of bitflags in areas, 32 bit bit flags.
    

    // Build Query String
    std::string queryString = sqlite3_mprintf("SELECT * FROM %Q WHERE sType like %Q;", strTableName.c_str(), areas);

    // Execute Query.
    if (qry->getResult(queryString))
    {
        long rows = qry->getNumRows();
        if (rows > 0)
        {
            while(qry->fetchRow())
            {
                area.reset(new FileArea);
                pullFileAreaResult(qry, area);
                area_list.push_back(area);
            }
        }
        else
        {
            std::cout << "Error, getAllFileAreasByConference Returned Rows: " << rows << std::endl;
        }
    }
    else
    {
        std::cout << "Error, getResult()" << std::endl;
    }

    return area_list;
}
