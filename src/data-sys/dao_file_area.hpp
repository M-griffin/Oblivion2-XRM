#ifndef FILE_AREA_DAO_HPP
#define FILE_AREA_DAO_HPP

#include <vector>
#include <functional>

#include "../model-sys/file_area.hpp"
#include "../data-sys/dao_base.hpp"

// Base Dao Definition
using baseFileAreaClass = BaseDao<FileArea>;

/**
 * @class FileAreaDao
 * @author Michael Griffin
 * @date 04/05/2017
 * @file file_area_dao.hpp
 * @brief File Area Data Access Object
 */
class FileAreaDao
        : public baseFileAreaClass {
public:
    explicit FileAreaDao(Database &database)
        : baseFileAreaClass(database) {
        // Setup Table name
        m_strTableName = "FileArea";

        // Check if Database Exists.
        m_cmdTableExists = "SELECT name FROM sqlite_master WHERE type='table' AND name='" + m_strTableName +
                           "' COLLATE NOCASE;";

        // Create Table Query (SQLite Only for the moment)
        m_cmdCreateTable =
                "CREATE TABLE IF NOT EXISTS " + m_strTableName + " ( "
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
                "FOREIGN KEY(iSecurityIndex) REFERENCES Security(iId) ON DELETE CASCADE "
                "); ";

        // CREATE INDEX `IDX_testtbl_Name` ON `testtbl` (`Name` COLLATE UTF8CI)
        m_cmdDropTable = "DROP TABLE IF EXISTS " + m_strTableName + "; ";

        // Set up the CallBack for Result Field Mapping
        m_result_callback = std::bind(&FileAreaDao::pullFileAreaResult, this,
                                      std::placeholders::_1, std::placeholders::_2);

        m_columns_callback = std::bind(&FileAreaDao::fillFileAreaColumnValues, this,
                                       std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);

        m_insert_callback = std::bind(&FileAreaDao::insertFileAreaQryString, this,
                                      std::placeholders::_1, std::placeholders::_2);

        m_update_callback = std::bind(&FileAreaDao::updateFileAreaQryString, this,
                                      std::placeholders::_1, std::placeholders::_2);
    }

    ~FileAreaDao() = default;

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
    bool updateRecord(FileArea &obj);

    /**
     * @brief Inserts a New Record in the database!
     * @param obj
     * @return
     */
    long insertRecord(FileArea &obj);

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
    FileArea getRecordById(long id);

    /**
     * @brief Retrieve All Records in a Table
     * @return
     */
    std::vector<FileArea> getAllRecords();

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
    std::string insertFileAreaQryString(std::string qry, FileArea &obj);

    /**
     * @brief (CallBack) Update Existing Record.
     * @param qry
     * @param obj
     * @return
     */
    std::string updateFileAreaQryString(std::string qry, FileArea &obj);

    /**
     * @brief (CallBack) Pulls results by FieldNames into their Class Variables.
     * @param qry
     * @param obj
     */
    void pullFileAreaResult(Query &qry, FileArea &obj);

    /**
     * @brief (Callback) for Insert Statement translates to (Column, .. ) VALUES (%d, %Q,)
     * @param qry
     * @param obj
     * @param values
     */
    void fillFileAreaColumnValues(Query &qry, FileArea &obj,
                                  std::vector<std::pair<std::string, std::string> > &values);


    /**
     * One Off Methods SQL Queries not included in the BaseDao
     * (Below This Point)
     */


    /**
     * @brief Return List of All FileArea by ConferenceId
     * @param id
     * @return
     */
    std::vector<FileArea> getAllFileAreasByConference(long id);
};

#endif
