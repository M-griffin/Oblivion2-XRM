#ifndef FILE_AREA_DAO_HPP
#define FILE_AREA_DAO_HPP

#include <boost/smart_ptr/shared_ptr.hpp>

#include <vector>
#include <cstdint>

// Forward Declerations
namespace SQLW
{
class Database;
class Query;
}

class FileArea;
// Handles to FileArea
typedef boost::shared_ptr<FileArea> file_area_ptr;

// Handles to Database
typedef boost::shared_ptr<SQLW::Database> database_ptr;

// Handle to Database Queries
typedef boost::shared_ptr<SQLW::Query> query_ptr;

/**
 * @class FileAreaDao
 * @author Michael Griffin
 * @date 04/05/2017
 * @file file_area_dao.hpp
 * @brief File Area Data Access Object
 */
class FileAreaDao
{
public:
    FileAreaDao(SQLW::Database &database);
    ~FileAreaDao();

    // Handle to Database
    SQLW::Database &m_file_area_database;

    std::string strTableName;

    // Static Queries
    std::string cmdFirstTimeSetup;

    std::string cmdFileAreaTableExists;
    std::string cmdCreateFileAreaTable;
    std::string cmdDropFileAreaTable;

    /**
     * @brief Check of the Database Exists.
     * @return
     */
    bool isTableExists();

    /**
     * @brief Run Setup Params for SQL Database.
     */
    bool firstTimeSetupParams();

    /**
     * @brief Create FileArea Database
     * @return
     */
    bool createTable();

    /**
     * @brief Drop FileArea Database
     * @return
     */
    bool dropTable();

    /**
     * @brief Create Query String to Insert New FileArea Record
     */
    std::string insertFileAreaQryString(query_ptr qry, file_area_ptr area);

    /**
     * @brief Creates Query String to Update Existing FileArea Record
     */
    std::string updateFileAreaQryString(query_ptr qry, file_area_ptr area);

    /**
     * @brief Updates a FileArea Record in the database!
     * @param area
     * @return
     */
    bool updateFileAreaRecord(file_area_ptr area);

    /**
     * @brief Inserts a New FileArea Record in the database!
     * @param area
     * @return
     */
    long insertFileAreaRecord(file_area_ptr area);

    /**
     * @brief Deletes a FileArea Record
     * @param areaId
     * @return
     */
    bool deleteFileAreaRecord(long areaId);

    /**
     * @brief Helper To populate FileArea Record with Query Results.
     */
    void pullFileAreaResult(query_ptr qry, file_area_ptr area);

    /**
     * @brief This takes a pair, and translates to (Column, .. ) VALUES (%d, %Q,) for formatting
     * @param values
     */
    void fillColumnValues(query_ptr qry, file_area_ptr area, 
        std::vector< std::pair<std::string, std::string> > &values);

    /**
     * @brief Return FileArea Record By Id.
     * @return
     */
    file_area_ptr getFileAreaById(long confId);
    
    /**
     * @brief Return List of All FileAreas
     * @return
     */
    std::vector<file_area_ptr> getAllFileAreas();
    
    /**
     * @brief Return List of All FileArea by ConferenceId
     * @param areas
     * @return
     */ 
    std::vector<file_area_ptr> getAllFileAreasByConference(long confId);   
    
};

// Handle to Database Queries
typedef boost::shared_ptr<FileAreaDao> file_area_dao_ptr;

#endif // FILE_AREA_DAO_HPP
