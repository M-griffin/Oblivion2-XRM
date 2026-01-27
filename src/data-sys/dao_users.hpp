#ifndef USERS_DAO_HPP
#define USERS_DAO_HPP

#include <iostream>
#include <vector>
#include <functional>

#include "../data-sys/dao_base.hpp"
#include "../model-sys/users.hpp"

// Base Dao Definition
using baseUsersClass = BaseDao<Users>;

/**
 * @class UsersDao
 * @author Michael Griffin
 * @date 2/28/2016
 * @file users_dao.hpp
 * @brief Handles Reading and Writing Users Class from Database
 *         Objects are Instantiated in the Session then passed to the DAO for work.
 */
class UsersDao
        : public baseUsersClass {
public:
    explicit UsersDao(Database &database)
        : baseUsersClass(database) {
        // Setup Table name
        m_strTableName = "users";

        /**
         * Pre Populate Static Queries one Time
         */
        m_cmdFirstTimeSetup =
                "PRAGMA synchronous=Normal; "
                "PRAGMA encoding=UTF-8; "
                "PRAGMA foreign_keys=ON; "
                "PRAGMA default_cache_size=10000; "
                "PRAGMA cache_size=10000; "
                "PRAGMA journal_mode = WAL; "
                "PRAGMA temp_store = MEMORY; "
                "PRAGMA mmap_size = 268435456; ";

        // Check if Database Exists.
        m_cmdTableExists = "SELECT name FROM sqlite_master WHERE type='table' AND name='" + m_strTableName +
                           "' COLLATE NOCASE;";

        // Create Users Table Query (SQLite Only for the moment)
        m_cmdCreateTable =
                "CREATE TABLE IF NOT EXISTS " + m_strTableName + " ( "
                "iId               INTEGER PRIMARY KEY, "
                "sHandle           TEXT NOT NULL COLLATE NOCASE, "
                "sRealName         TEXT NOT NULL COLLATE NOCASE, "
                "sAddress          TEXT NOT NULL, "
                "sLocation         TEXT NOT NULL, "
                "sCountry          TEXT NOT NULL, "
                "sEmail            TEXT NOT NULL COLLATE NOCASE, "
                "sUserNote         TEXT NOT NULL, "
                "dtBirthday        DATETIME NOT NULL, "
                "iLevel            INTEGER NOT NULL, "
                "iFileLevel        INTEGER NOT NULL, "
                "iMessageLevel     INTEGER NOT NULL, "
                "iLastFileArea     INTEGER NOT NULL, "
                "iLastMessageArea  INTEGER NOT NULL, "
                "iTimeLeft         INTEGER NOT NULL, "
                "iTimeLimit        INTEGER NOT NULL, "
                "sRegColor         TEXT NOT NULL, "
                "sPromptColor      TEXT NOT NULL, "
                "sInputColor       TEXT NOT NULL, "
                "sInverseColor     TEXT NOT NULL, "
                "sStatColor        TEXT NOT NULL, "
                "sBoxColor         TEXT NOT NULL, "
                "iFilePoints       INTEGER NOT NULL, "
                "iPostCallRatio    INTEGER NOT NULL, "
                "sMenuPromptName   TEXT NOT NULL, "
                "iMenuSelected     INTEGER NOT NULL, "
                "iStatusSelected   INTEGER NOT NULL, "
                "bAnsi             BOOLEAN NOT NULL, "
                "bBackSpaceVt100   BOOLEAN NOT NULL, "
                "iNuvVotesYes      INTEGER NOT NULL, "
                "iNuvVotesNo       INTEGER NOT NULL, "
                "dtPassChangeDate  DATETIME DEFAULT CURRENT_TIMESTAMP, "
                "dtLastReplyDate   DATETIME DEFAULT CURRENT_TIMESTAMP, "
                "bScrollFL         BOOLEAN NOT NULL, "
                "iCSPassChange     INTEGER NOT NULL, "
                "iControlFlags1    INTEGER NOT NULL, "
                "iControlFlags2    INTEGER NOT NULL, "
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
                "bAllowPurge       BOOLEAN NOT NULL, "
                "iSecurityIndex    INT NOT NULL, "
                "FOREIGN KEY(iSecurityIndex) REFERENCES Security(iId) ON DELETE CASCADE "
                "); ";

        m_cmdCreateIndex = "";
        "CREATE INDEX IF NOT EXISTS users_idx "
                "ON " + m_strTableName + " (sHandle COLLATE NOCASE, sRealName COLLATE NOCASE, sEmail COLLATE NOCASE); ";

        // CREATE INDEX `IDX_testtbl_Name` ON `testtbl` (`Name` COLLATE UTF8CI)
        m_cmdDropTable = "DROP TABLE IF EXISTS " + m_strTableName + "; ";
        m_cmdDropIndex = "DROP INDEX IF EXISTS users_idx; ";

        // CREATE INDEX `IDX_testtbl_Name` ON `testtbl` (`Name` COLLATE UTF8CI)
        m_cmdDropTable = "DROP TABLE IF EXISTS " + m_strTableName + "; ";

        // Set up the CallBack for Result Field Mapping
        m_result_callback = std::bind(&UsersDao::pullUsersResult, this,
                                      std::placeholders::_1, std::placeholders::_2);

        m_columns_callback = std::bind(&UsersDao::fillUsersColumnValues, this,
                                       std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);

        m_insert_callback = std::bind(&UsersDao::insertUsersQryString, this,
                                      std::placeholders::_1, std::placeholders::_2);

        m_update_callback = std::bind(&UsersDao::updateUsersQryString, this,
                                      std::placeholders::_1, std::placeholders::_2);
    }

    ~UsersDao() = default;

    /**
     * Base Dao Calls for generic Object Data Calls
     * (Below This Point)
     */


    /**
     * @brief Check If Database Table Exists.
     * @return
     */
    bool doesTableExist();

    /**
     * @brief Run Setup Params for SQL Database Table.
     */
    bool firstTimeSetupParams();

    /**
     * @brief Create Database Table
     * @return
     */
    bool createTable();

    /**
     * @brief Drop Database
     * @return
     */
    bool dropTable();

    /**
     * @brief Updates a Record in the database!
     * @param obj
     * @return
     */
    bool updateRecord(Users &obj);

    /**
     * @brief Inserts a New Record in the database!
     * @param obj
     * @return
     */
    long insertRecord(Users &obj);

    /**
     * @brief Deletes a MessageArea Record
     * @param id
     * @return
     */
    bool deleteRecord(long id);

    /**
     * @brief Retrieve Record By Id.
     * @param id
     * @return
     */
    Users getRecordById(long id);

    /**
     * @brief Retrieve All Records in a Table
     * @return
     */
    std::vector<Users> getAllRecords();

    /**
     * @brief Retrieve Count of All Records in a Table
     * @return
     */
    long getRecordsCount();


    /**
     * Base Dao Call Back for Object Specific Data Mappings
     * (Below This Point)
     */


    /**
     * @brief (Callback) Create Record Insert Statement, returns query string
     * @param qry
     * @param obj
     * @return
     */
    std::string insertUsersQryString(std::string qry, Users &obj);

    /**
     * @brief (CallBack) Update Existing Record.
     * @param qry
     * @param obj
     * @return
     */
    std::string updateUsersQryString(std::string qry, Users &obj);

    /**
     * @brief (CallBack) Pulls results by FieldNames into their Class Variables.
     * @param qry
     * @param obj
     */
    void pullUsersResult(Query &qry, Users &obj);

    /**
     * @brief (Callback) for Insert Statement translates to (Column, .. ) VALUES (%d, %Q,)
     * @param qry
     * @param obj
     * @param values
     */
    void fillUsersColumnValues(Query &qry, Users &obj, std::vector<std::pair<std::string, std::string> > &values);


    /**
     * One Off Methods SQL Queries not included in the BaseDao
     * (Below This Point)
     */


    /**
     * @brief Return User Record By Handle.
     * @return
     */
    Users getUserByHandle(std::string name);

    /**
     * @brief Return User Record By RealName.
     * @return
     */
    Users getUserByRealName(std::string name);

    /**
     * @brief Return User Record By Email.
     * @return
     */
    Users getUserByEmail(std::string email);

    /**
     * @brief Return User Records By WildCard (filtered Searches)
     * @return
     */
    std::vector<Users> getUsersByWildcard(std::string filter);
};

#endif
