#ifndef ONELINERS_DAO_HPP
#define ONELINERS_DAO_HPP

#include <string>
#include <vector>

#include "../model-app/oneliners.hpp"
#include "../data-sys/dao_base.hpp"


// Base Dao Definition
using baseOnelinerClass = BaseDao<Oneliners>;

/**
 * @class OnelinerDao
 * @author Michael Griffin
 * @date 05/05/2017
 * @file oneliners_dao.hpp
 * @brief One Liners Data Access Object
 */
class OnelinerDao
        : public baseOnelinerClass {
public:
    explicit OnelinerDao(Database &database)
        : baseOnelinerClass(database) {
        // Setup Table name
        m_strTableName = "Oneliner";

        // Check if Database Exists.
        m_cmdTableExists = "SELECT name FROM sqlite_master WHERE type='table' AND name='" + m_strTableName +
                           "' COLLATE NOCASE;";

        // Create Table Query (SQLite Only for the moment)
        m_cmdCreateTable =
                "CREATE TABLE IF NOT EXISTS " + m_strTableName + " ( "
                "iId               INTEGER PRIMARY KEY, "
                "iUserId           INTEGER NOT NULL, "
                "sText             TEXT NOT NULL COLLATE NOCASE, "
                "sUserName         TEXT NOT NULL COLLATE NOCASE, "
                "sUserInitials     TEXT NOT NULL COLLATE NOCASE, "
                "dtDatePosted      DATETIME DEFAULT CURRENT_TIMESTAMP "
                "); ";
                // Add this back once we create default sysop record and can link it properly.
                //"FOREIGN KEY(iUserId) REFERENCES UserRecord(iId) ON DELETE CASCADE "


        // CREATE INDEX `IDX_testtbl_Name` ON `testtbl` (`Name` COLLATE UTF8CI)
        m_cmdDropTable = "DROP TABLE IF EXISTS " + m_strTableName + "; ";

        // Set up the CallBack for Result Field Mapping
        m_result_callback = std::bind(&OnelinerDao::pullOnelinerResult, this,
                                      std::placeholders::_1, std::placeholders::_2);

        m_columns_callback = std::bind(&OnelinerDao::fillOnelinerColumnValues, this,
                                       std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);

        m_insert_callback = std::bind(&OnelinerDao::insertOnelinerQryString, this,
                                      std::placeholders::_1, std::placeholders::_2);

        m_update_callback = std::bind(&OnelinerDao::updateOnelinerQryString, this,
                                      std::placeholders::_1, std::placeholders::_2);
    }

    ~OnelinerDao() = default;

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
    bool updateRecord(Oneliners &obj);

    /**
     * @brief Inserts a New Record in the database!
     * @param obj
     * @return
     */
    long insertRecord(Oneliners &obj);

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
    Oneliners getRecordById(long id);

    /**
     * @brief Retrieve All Records in a Table
     * @return
     */
    std::vector<Oneliners> getAllRecords();

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
     * @brief (CallBack) Pulls results by FieldNames into their Class Variables.
     * @param qry
     * @param obj
     */
    void pullOnelinerResult(Query &qry, Oneliners &obj);

    /**
     * @brief (CallBack) Insert Statement translates to (Column, .. ) VALUES (%d, %Q,)
     * @param qry
     * @param obj
     * @param values
     */
    void fillOnelinerColumnValues(Query &qry, Oneliners &obj,
                                  std::vector<std::pair<std::string, std::string> > &values);

    /**
     * @brief (Callback) Create Record Insert Statement, returns query string
     * @param qry
     * @param obj
     * @return
     */
    std::string insertOnelinerQryString(std::string qry, Oneliners &obj);

    /**
     * @brief (CallBack) Update Existing Record.
     * @param qry
     * @param obj
     * @return
     */
    std::string updateOnelinerQryString(std::string qry, Oneliners &obj);


    /**
     * One Off Methods SQL Queries not included in the BaseDao
     * (Below This Point)
     */


    /**
     * @brief Return All Records By User ID.
     * @return
     */
    std::vector<Oneliners> getAllOnelinersByUserId(long userId);
};

#endif
