#include "users_dao.hpp"
#include "../model/users.hpp"

#include "libSqliteWrapped.h"
#include <sqlite3.h>

#include <boost/make_shared.hpp>

#include <iostream>
#include <string>

/*
 * void enforceForeignKeys( sqlite3* db, bool enforceForeignKeyConstraints ) {

    int expectedNewValue = enforceForeignKeyConstraints ? 1 : 0;
    int actualNewValue;

    int err = sqlite3_db_config( db, SQLITE_DBCONFIG_ENABLE_FKEY, expectedNewValue, &actualNewValue);
    if( err != SQLITE_OK ) throw err;
    if( actualNewValue != expectedNewValue ) throw SOME_USER_DEFINED_ERROR;
}
 */

UsersDao::UsersDao(SQLW::Database &database)
    : m_users_database(database)
{

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
    cmdUserTableExists = "SELECT name FROM sqlite_master WHERE type='table' AND name='users' COLLATE NOCASE;";
   
    // Create Users Table Query (SQLite Only for the moment)
    cmdCreateUserTable =
        "CREATE TABLE IF NOT EXISTS users ( "
        "iId               INTEGER PRIMARY KEY, "
        "sHandle           TEXT NOT NULL, "
        "sRealName         TEXT NOT NULL, "
        "sAddress          TEXT NOT NULL, "
        "sLocation         TEXT NOT NULL, "
        "sCountry          TEXT NOT NULL, "
        "sEmail            TEXT NOT NULL, "
        "sUserNote         TEXT NOT NULL, "
        "dtBirthday        DATETIME NOT NULL, "
        "iLevel            INTEGER NOT NULL, "
        "iFileLevel        INTEGER NOT NULL, "
        "iLastFileArea     INTEGER NOT NULL, "
        "iLastMessageArea  INTEGER NOT NULL, "
        "dtLastCallDate    DATETIME DEFAULT CURRENT_TIMESTAMP, "
        "iTimeLeft         INTEGER NOT NULL, "
        "iTimeLimit        INTEGER NOT NULL, "
        "sRegColor         TEXT NOT NULL, "
        "sPromptColor      TEXT NOT NULL, "
        "sInputColor       TEXT NOT NULL, "
        "sInverseColor     TEXT NOT NULL, "
        "sStatColor        TEXT NOT NULL, "
        "sBoxColor         TEXT NOT NULL, "
        "iPostCallRatio    INTEGER NOT NULL, "
        "iPromptSelected   INTEGER NOT NULL, "
        "iMenuSelected     INTEGER NOT NULL, "
        "iStatusSelected   INTEGER NOT NULL, "
        "bAnsi             BOOLEAN NOT NULL, "
        "bBackSpaceVt100   BOOLEAN NOT NULL, "
        "iNuvVotesYes      INTEGER NOT NULL, "
        "iNuvVotesNo       INTEGER NOT NULL, "
        "sUsersVotingYes   TEXT NOT NULL, "
        "sUusersVotingNo   TEXT NOT NULL, "
        "dtPassChangeDate  DATETIME DEFAULT CURRENT_TIMESTAMP, "
        "dtLastReplyDate   DATETIME DEFAULT CURRENT_TIMESTAMP, "
        "bScrollFL         BOOLEAN NOT NULL, "
        "iCallsToday       INTEGER NOT NULL, "
        "iNewLevel         INTEGER NOT NULL, "
        "iCSPassChange     INTEGER NOT NULL, "
        "sControlFlags     TEXT NOT NULL, "
        "bWanted           BOOLEAN NOT NULL, "
        "sHeaderType       TEXT NOT NULL, "
        "iLastMesConf      INTEGER NOT NULL, "
        "iLastFileConf     INTEGER NOT NULL, "
        "dtExpirationDate  DATETIME DEFAULT('01/01/2050'), "
        "sGender           TEXT NOT NULL, "
        "dtFirstOn         DATETIME DEFAULT CURRENT_TIMESTAMP, "
        "bYesNoBars        BOOLEAN NOT NULL, "
        "iHackAttempts     INTEGER NOT NULL, "
        "bDoPause          BOOLEAN NOT NULL, "
        "bClearOrScroll    BOOLEAN NOT NULL, "
        "bIgnoreTimeLimit  BOOLEAN NOT NULL, "
        "bAllowPurge       BOOLEAN NOT NULL "
        "); ";

    cmdCreateUserIndex =
        "CREATE UNIQUE INDEX IF NOT EXISTS users_idx "
        "ON users (sHandle, sRealName, sEmail); ";

    cmdDropUserTable = "DROP TABLE IF EXISTS users; ";
    cmdDropUserIndex = "DROP INDEX IF EXISTS users_idx; ";

}

UsersDao::~UsersDao()
{
    std::cout << "~UsersDao" << std::endl;
}

/**
 * @brief Check if the Table Exists in Database
 * @return
 */
bool UsersDao::isTableExists()
{
    bool result = false;

    // Make Sure Database Reference is Connected
    if (!m_users_database.isConnected())
    {
        std::cout << "Error, Database is not connected!" << std::endl;
        return result;
    }

    // Create Pointer and Connect Query Object to Database.
    query_ptr qry(new SQLW::Query(m_users_database));
    if (!qry || !qry->isConnected())
    {
        std::cout << "Error, Query has no connection to the database" << std::endl;
        return result;
    }

    // Execute and get result.
    if (qry->getResult(cmdUserTableExists))
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
 * @brief Create Users Table
 * If Create Table Fails, skip trying to create index.
 */
bool UsersDao::createTable()
{
    bool result = false;

    // Make Sure Database Reference is Connected
    if (!m_users_database.isConnected())
    {
        std::cout << "Error, Database is not connected!" << std::endl;
        return result;
    }

    // Create Pointer and Connect Query Object to Database.
    query_ptr qry(new SQLW::Query(m_users_database));
    if (!qry || !qry->isConnected())
    {
        std::cout << "Error, Query has no connection to the database" << std::endl;
        return result;
    }

    // Create List of statements to execute in a single transaction.
    std::vector<std::string> statements;
    statements.push_back(cmdCreateUserTable);
    statements.push_back(cmdCreateUserIndex);

    // Execute Transaction.
    result = qry->executeTransaction(statements);
    return result;  
}

/**
 * @brief Drop Users Table
 * If Drop Index Fails, still try to Drop Tables as it will try both!
 */
bool UsersDao::dropTable()
{
    bool result = false;

    // Make Sure Database Reference is Connected
    if (!m_users_database.isConnected())
    {
        std::cout << "Error, Database is not connected!" << std::endl;
        return result;
    }

    // Create Pointer and Connect Query Object to Database.
    query_ptr qry(new SQLW::Query(m_users_database));
    if (!qry || !qry->isConnected())
    {
        std::cout << "Error, Query has no connection to the database" << std::endl;
        return result;
    }

    // Create List of statements to execute in a single transaction.
    std::vector<std::string> statements;
    statements.push_back(cmdDropUserIndex);
    statements.push_back(cmdDropUserTable);

    // Execute Transaction.
    result = qry->executeTransaction(statements);
    return result;
}

/**
 * @brief Pulls results by FileNames into their Class Variables.
 */
void UsersDao::pullUserResult(query_ptr qry, user_ptr user)
{
    qry->getFieldByName("iId", user->iId);
    qry->getFieldByName("sHandle", user->sHandle);
    qry->getFieldByName("sRealName", user->sRealName);
    qry->getFieldByName("sAddress", user->sAddress);
    qry->getFieldByName("sLocation", user->sLocation);
    qry->getFieldByName("sCountry", user->sCountry);
    qry->getFieldByName("sEmail", user->sEmail);
    qry->getFieldByName("sUserNote", user->sUserNote);
    qry->getFieldByName("dtBirthday", user->dtBirthday);
    qry->getFieldByName("iLevel", user->iLevel);
    qry->getFieldByName("iFileLevel", user->iFileLevel);
    qry->getFieldByName("iLastFileArea", user->iLastFileArea);
    qry->getFieldByName("iLastMessageArea", user->iLastMessageArea);
    qry->getFieldByName("dtLastCallDate", user->dtLastCallDate);
    qry->getFieldByName("iTimeLeft", user->iTimeLeft);
    qry->getFieldByName("iTimeLimit", user->iTimeLimit);
    qry->getFieldByName("sRegColor", user->sRegColor);
    qry->getFieldByName("sPromptColor", user->sPromptColor);
    qry->getFieldByName("sInputColor", user->sInputColor);
    qry->getFieldByName("sInverseColor", user->sInverseColor);
    qry->getFieldByName("sStatColor", user->sStatColor);
    qry->getFieldByName("sBoxColor", user->sBoxColor);
    qry->getFieldByName("iPostCallRatio", user->iPostCallRatio);
    qry->getFieldByName("iPromptSelected", user->iPromptSelected);
    qry->getFieldByName("iMenuSelected", user->iMenuSelected);
    qry->getFieldByName("iStatusSelected", user->iStatusSelected);
    qry->getFieldByName("bAnsi", user->bAnsi);
    qry->getFieldByName("bBackSpaceVt100", user->bBackSpaceVt100);
    qry->getFieldByName("iNuvVotesYes", user->iNuvVotesYes);
    qry->getFieldByName("iNuvVotesNo", user->iNuvVotesNo);
    qry->getFieldByName("dtPassChangeDate", user->dtPassChangeDate);
    qry->getFieldByName("dtLastReplyDate", user->dtLastReplyDate);
    qry->getFieldByName("bScrollFL", user->bAnsi);  // this should be redone!
    qry->getFieldByName("iCallsToday", user->iCallsToday);
    qry->getFieldByName("iNewLevel", user->iNewLevel);
    qry->getFieldByName("iCSPassChange", user->iCSPassChange);
    qry->getFieldByName("sControlFlags", user->sControlFlags);
    qry->getFieldByName("bWanted", user->bWanted);
    qry->getFieldByName("sHeaderType", user->sHeaderType);
    qry->getFieldByName("iLastMesConf", user->iLastMesConf);
    qry->getFieldByName("iLastFileConf", user->iLastFileConf);
    qry->getFieldByName("dtExpirationDate", user->dtExpirationDate);
    qry->getFieldByName("sGender", user->sGender);
    qry->getFieldByName("dtFirstOn", user->dtFirstOn);
    qry->getFieldByName("bYesNoBars", user->bYesNoBars);
    qry->getFieldByName("iHackAttempts", user->iHackAttempts);
    qry->getFieldByName("bDoPause", user->bDoPause);
    qry->getFieldByName("bClearOrScroll", user->bClearOrScroll);
    qry->getFieldByName("bIgnoreTimeLimit", user->bIgnoreTimeLimit);
    qry->getFieldByName("bAllowPurge", user->bAllowPurge);
}


/**
 * @brief Used for Insert Statement
 *        This takes a pair, and translates to (Column, .. ) VALUES (%d, %Q,) for formatting
 * @param values
 */
void UsersDao::fillColumnValues(query_ptr qry, user_ptr user, std::vector< std::pair<std::string, std::string> > &values)
{
    // values.push_back(qry->translateFieldName("iId", user->iId));
    values.push_back(qry->translateFieldName("sHandle", user->sHandle));
    values.push_back(qry->translateFieldName("sRealName", user->sRealName));
    values.push_back(qry->translateFieldName("sAddress", user->sAddress));
    values.push_back(qry->translateFieldName("sLocation", user->sLocation));
    values.push_back(qry->translateFieldName("sCountry", user->sCountry));
    values.push_back(qry->translateFieldName("sEmail", user->sEmail));
    values.push_back(qry->translateFieldName("sUserNote", user->sUserNote));
    values.push_back(qry->translateFieldName("dtBirthday", user->dtBirthday));
    values.push_back(qry->translateFieldName("iLevel", user->iLevel));
    values.push_back(qry->translateFieldName("iFileLevel", user->iFileLevel));
    values.push_back(qry->translateFieldName("iLastFileArea", user->iLastFileArea));
    values.push_back(qry->translateFieldName("iLastMessageArea", user->iLastMessageArea));
    values.push_back(qry->translateFieldName("dtLastCallDate", user->dtLastCallDate));
    values.push_back(qry->translateFieldName("iTimeLeft", user->iTimeLeft));
    values.push_back(qry->translateFieldName("iTimeLimit", user->iTimeLimit));
    values.push_back(qry->translateFieldName("sRegColor", user->sRegColor));
    values.push_back(qry->translateFieldName("sPromptColor", user->sPromptColor));
    values.push_back(qry->translateFieldName("sInputColor", user->sInputColor));
    values.push_back(qry->translateFieldName("sInverseColor", user->sInverseColor));
    values.push_back(qry->translateFieldName("sStatColor", user->sStatColor));
    values.push_back(qry->translateFieldName("sBoxColor", user->sBoxColor));
    values.push_back(qry->translateFieldName("iPostCallRatio", user->iPostCallRatio));
    values.push_back(qry->translateFieldName("iPromptSelected", user->iPromptSelected));
    values.push_back(qry->translateFieldName("iMenuSelected", user->iMenuSelected));
    values.push_back(qry->translateFieldName("iStatusSelected", user->iStatusSelected));
    values.push_back(qry->translateFieldName("bAnsi", user->bAnsi));
    values.push_back(qry->translateFieldName("bBackSpaceVt100", user->bBackSpaceVt100));
    values.push_back(qry->translateFieldName("iNuvVotesYes", user->iNuvVotesYes));
    values.push_back(qry->translateFieldName("iNuvVotesNo", user->iNuvVotesNo));
    values.push_back(qry->translateFieldName("dtPassChangeDate", user->dtPassChangeDate));
    values.push_back(qry->translateFieldName("dtLastReplyDate", user->dtLastReplyDate));
    values.push_back(qry->translateFieldName("bScrollFL", user->bAnsi));
    values.push_back(qry->translateFieldName("iCallsToday", user->iCallsToday));
    values.push_back(qry->translateFieldName("iNewLevel", user->iNewLevel));
    values.push_back(qry->translateFieldName("iCSPassChange", user->iCSPassChange));
    values.push_back(qry->translateFieldName("sControlFlags", user->sControlFlags));
    values.push_back(qry->translateFieldName("bWanted", user->bWanted));
    values.push_back(qry->translateFieldName("sHeaderType", user->sHeaderType));
    values.push_back(qry->translateFieldName("iLastMesConf", user->iLastMesConf));
    values.push_back(qry->translateFieldName("iLastFileConf", user->iLastFileConf));
    values.push_back(qry->translateFieldName("dtExpirationDate", user->dtExpirationDate));
    values.push_back(qry->translateFieldName("sGender", user->sGender));
    values.push_back(qry->translateFieldName("dtFirstOn", user->dtFirstOn));
    values.push_back(qry->translateFieldName("bYesNoBars", user->bYesNoBars));
    values.push_back(qry->translateFieldName("iHackAttempts", user->iHackAttempts));
    values.push_back(qry->translateFieldName("bDoPause", user->bDoPause));
    values.push_back(qry->translateFieldName("bClearOrScroll", user->bClearOrScroll));
    values.push_back(qry->translateFieldName("bIgnoreTimeLimit", user->bIgnoreTimeLimit));
    values.push_back(qry->translateFieldName("bAllowPurge", user->bAllowPurge));
}


/**
 * @brief Create User Record Insert Statement, returns query string
 */
std::string UsersDao::insertUserQryString(query_ptr qry, user_ptr user)
{
    std::stringstream ssColumn;
    std::stringstream ssType;
    std::vector< std::pair<std::string, std::string> >::iterator it;
    std::vector< std::pair<std::string, std::string> > values;

    ssColumn << "INSERT INTO users (";
    ssType << ") VALUES (";

    // Populate the Pairs.
    fillColumnValues(qry, user, values);

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
    std::string newQueryString = "";
    newQueryString = ssColumn.str();
    newQueryString.append(ssType.str());

    // Mprint statement to avoid injections.
    std::string result = sqlite3_mprintf(newQueryString.c_str(),
        user->sHandle.c_str(),
        user->sRealName.c_str(),
        user->sAddress.c_str(),
        user->sLocation.c_str(),
        user->sCountry.c_str(),
        user->sEmail.c_str(),
        user->sUserNote.c_str(),
        user->dtBirthday,
        user->iLevel,
        user->iFileLevel,
        user->iLastFileArea,
        user->iLastMessageArea,
        user->dtLastCallDate,
        user->iTimeLeft,
        user->iTimeLimit,
        user->sRegColor.c_str(),
        user->sPromptColor.c_str(),
        user->sInputColor.c_str(),
        user->sInverseColor.c_str(),
        user->sStatColor.c_str(),
        user->sBoxColor.c_str(),
        user->iPostCallRatio,
        user->iPromptSelected,
        user->iMenuSelected,
        user->iStatusSelected,
        user->bAnsi,
        user->bBackSpaceVt100,
        user->iNuvVotesYes,
        user->iNuvVotesNo,
        user->dtPassChangeDate,
        user->dtLastReplyDate,
        user->bAnsi,
        user->iCallsToday,
        user->iNewLevel,
        user->iCSPassChange,
        user->sControlFlags.c_str(),
        user->bWanted,
        user->sHeaderType.c_str(),
        user->iLastMesConf,
        user->iLastFileConf,
        user->dtExpirationDate,
        user->sGender.c_str(),
        user->dtFirstOn,
        user->bYesNoBars,
        user->iHackAttempts,
        user->bDoPause,
        user->bClearOrScroll,
        user->bIgnoreTimeLimit,
        user->bAllowPurge
    );

    return result;
}


/**
 * @brief Update Existing user Record.
 */
std::string UsersDao::updateUserQryString(query_ptr qry, user_ptr user)
{
    std::stringstream ssColumn;
    std::vector< std::pair<std::string, std::string> >::iterator it;
    std::vector< std::pair<std::string, std::string> > values;

    // Setup start of Statement
    ssColumn << "UPDATE users SET ";

    // Populate the Pairs. Variable = %Q formatted string.
    fillColumnValues(qry, user, values);

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
    std::string newQueryString = "";
    newQueryString = ssColumn.str();
    newQueryString.append(" WHERE iId = %ld; ");

    // Mprint statement to avoid injections.
    std::string result = sqlite3_mprintf(newQueryString.c_str(),
        user->sHandle.c_str(),
        user->sRealName.c_str(),
        user->sAddress.c_str(),
        user->sLocation.c_str(),
        user->sCountry.c_str(),
        user->sEmail.c_str(),
        user->sUserNote.c_str(),
        user->dtBirthday,
        user->iLevel,
        user->iFileLevel,
        user->iLastFileArea,
        user->iLastMessageArea,
        user->dtLastCallDate,
        user->iTimeLeft,
        user->iTimeLimit,
        user->sRegColor.c_str(),
        user->sPromptColor.c_str(),
        user->sInputColor.c_str(),
        user->sInverseColor.c_str(),
        user->sStatColor.c_str(),
        user->sBoxColor.c_str(),
        user->iPostCallRatio,
        user->iPromptSelected,
        user->iMenuSelected,
        user->iStatusSelected,
        user->bAnsi,
        user->bBackSpaceVt100,
        user->iNuvVotesYes,
        user->iNuvVotesNo,
        user->dtPassChangeDate,
        user->dtLastReplyDate,
        user->bAnsi,
        user->iCallsToday,
        user->iNewLevel,
        user->iCSPassChange,
        user->sControlFlags.c_str(),
        user->bWanted,
        user->sHeaderType.c_str(),
        user->iLastMesConf,
        user->iLastFileConf,
        user->dtExpirationDate,
        user->sGender.c_str(),
        user->dtFirstOn,
        user->bYesNoBars,
        user->iHackAttempts,
        user->bDoPause,
        user->bClearOrScroll,
        user->bIgnoreTimeLimit,
        user->bAllowPurge,
        user->iId
    );

    return result;
}


/**
 * @brief Updates a User Record in the database!
 * @param user
 * @return
 */
bool UsersDao::updateUserRecord(user_ptr user)
{
    bool result = false;

    // Make Sure Database Reference is Connected
    if (!m_users_database.isConnected())
    {
        std::cout << "Error, Database is not connected!" << std::endl;
        return result;
    }

    // Create Pointer and Connect Query Object to Database.
    query_ptr qry(new SQLW::Query(m_users_database));
    if (!qry || !qry->isConnected())
    {
        std::cout << "Error, Query has no connection to the database" << std::endl;
        return result;
    }

    // Build update string
    std::string queryString = updateUserQryString(qry, user);

    // Execute Update in a Transaction, rollback if fails.
    std::vector<std::string> statements;
    statements.push_back(queryString);
    result = qry->executeTransaction(statements);

    return result;
}

/**
 * @brief Inserts a New User Record in the database!
 * @param user
 * @return
 */
bool UsersDao::insertUserRecord(user_ptr user)
{    
    bool result = false;

    // Make Sure Database Reference is Connected
    if (!m_users_database.isConnected())
    {
        std::cout << "Error, Database is not connected!" << std::endl;
        return result;
    }

    // Create Pointer and Connect Query Object to Database.
    query_ptr qry(new SQLW::Query(m_users_database));
    if (!qry || !qry->isConnected())
    {
        std::cout << "Error, Query has no connection to the database" << std::endl;
        return result;
    }

    // Build update string
    std::string queryString = insertUserQryString(qry, user);

    // Execute Update in a Transaction, rollback if fails.
    std::vector<std::string> statements;
    statements.push_back(queryString);
    result = qry->executeTransaction(statements);

    return result;
}

/**
 * @brief Deletes a User Record
 * @param userId
 * @return
 */
bool UsersDao::deleteUserRecord(long userId)
{
    bool result = false;

    // Make Sure Database Reference is Connected
    if (!m_users_database.isConnected())
    {
        std::cout << "Error, Database is not connected!" << std::endl;
        return result;
    }

    // Create Pointer and Connect Query Object to Database.
    query_ptr qry(new SQLW::Query(m_users_database));
    if (!qry || !qry->isConnected())
    {
        std::cout << "Error, Query has no connection to the database" << std::endl;
        return result;
    }

    // Build string
    std::string queryString = sqlite3_mprintf("DELETE FROM users WHERE iId = %ld;", userId);

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
user_ptr UsersDao::getUserById(long userId)
{
    user_ptr user(new Users);

    // Make Sure Database Reference is Connected
    if (!m_users_database.isConnected())
    {
        std::cout << "Error, Database is not connected!" << std::endl;
        return user;
    }

    // Create Pointer and Connect Query Object to Database.
    query_ptr qry(new SQLW::Query(m_users_database));
    if (!qry->isConnected())
    {
        std::cout << "Error, Query has no connection to the database" << std::endl;
        return user;
    }

    // Build Query String
    std::string queryString = sqlite3_mprintf("SELECT * FROM users WHERE iID = %ld;", userId);

    // Execute Query.
    if (qry->getResult(queryString))
    {
        long rows = qry->getNumRows();
        if (rows > 0)
        {
            qry->fetchRow();
            pullUserResult(qry, user);
        }
        else
        {
            std::cout << "Error, getUserById Returned Rows: " << rows << std::endl;
        }
    }
    else
    {
        std::cout << "Error, getResult()" << std::endl;
    }

    return user;
}

/**
 * @brief Return User Record By Handle.
 * @return
 */
user_ptr UsersDao::getUserByHandle(std::string name)
{
    user_ptr user(new Users);

    // Make Sure Database Reference is Connected
    if (!m_users_database.isConnected())
    {
        std::cout << "Error, Database is not connected!" << std::endl;
        return user;
    }

    // Create Pointer and Connect Query Object to Database.
    query_ptr qry(new SQLW::Query(m_users_database));
    if (!qry->isConnected())
    {
        std::cout << "Error, Query has no connection to the database" << std::endl;
        return user;
    }

    // Build Query String
    std::string queryString = sqlite3_mprintf("SELECT * FROM users WHERE sHandle = %Q; COLLATE NOCASE;", name.c_str());

    // Execute Query.
    if (qry->getResult(queryString))
    {
        long rows = qry->getNumRows();
        if (rows > 0)
        {
            qry->fetchRow();
            pullUserResult(qry, user);
        }
        else
        {
            std::cout << "Error, getUserByHandle Returned Rows: " << rows << std::endl;
        }
    }
    else
    {
        std::cout << "Error, getResult()!" << std::endl;
    }

    return user;
}

/**
 * @brief Return User Record By FullName.
 * @return
 */
user_ptr UsersDao::getUserByRealName(std::string name)
{
    user_ptr user(new Users);

    // Make Sure Database Reference is Connected
    if (!m_users_database.isConnected())
    {
        std::cout << "Error, Database is not connected!" << std::endl;
        return user;
    }

    // Create Pointer and Connect Query Object to Database.
    query_ptr qry(new SQLW::Query(m_users_database));
    if (!qry->isConnected())
    {
        std::cout << "Error, Query has no connection to the database" << std::endl;
        return user;
    }

    // Build Query String
    std::string queryString = sqlite3_mprintf("SELECT * FROM users WHERE sRealName = %Q COLLATE NOCASE;", name.c_str());

    // Execute Query.
    if (qry->getResult(queryString))
    {
        long rows = qry->getNumRows();
        if (rows > 0)
        {
            qry->fetchRow();
            pullUserResult(qry, user);
        }
        else
        {
            std::cout << "Error, getUserByRealName Returned Rows: " << rows << std::endl;
        }
    }
    else
    {
        std::cout << "Error, getResult()" << std::endl;
    }
    return user;
}

/**
 * @brief Return User Record By Email.
 * @return
 */
user_ptr UsersDao::getUserByEmail(std::string email)
{
    user_ptr user(new Users);

    // Make Sure Database Reference is Connected
    if (!m_users_database.isConnected())
    {
        std::cout << "Error, Database is not connected!" << std::endl;
        return user;
    }

    // Create Pointer and Connect Query Object to Database.
    query_ptr qry(new SQLW::Query(m_users_database));
    if (!qry->isConnected())
    {
        std::cout << "Error, Query has no connection to the database" << std::endl;
        return user;
    }

    // Build Query String
    std::string queryString = sqlite3_mprintf("SELECT * FROM users WHERE sEmail = %Q COLLATE NOCASE;", email.c_str());

    // create a test3 table
    if (qry->getResult(queryString))
    {
        long rows = qry->getNumRows();
        if (rows > 0)
        {
            qry->fetchRow();
            pullUserResult(qry, user);
        }
        else
        {
            std::cout << "Error, getUserByEmail Returned Rows: " << rows << std::endl;
        }
    }
    else
    {
        std::cout << "Error, getResult()" << std::endl;
    }
 
    return user;
}


