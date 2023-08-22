#ifndef BASE_DAO_HPP
#define BASE_DAO_HPP

#include "logging.hpp"

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#endif

#include "libSqliteWrapped.h"
#include <sqlite3.h>

#include <iostream>
#include <memory>
#include <functional>
#include <vector>


// Handle to Database Queries
typedef std::shared_ptr<SQLW::Query> query_ptr;

/**
 * @class BaseDao
 * @author Michael Griffin
 * @date 05/05/2017
 * @file base_dao.hpp
 * @brief Base Query Class for All Database Data Access Objects.
 */
template <class T>
class BaseDao
{

public:

    explicit BaseDao(SQLW::Database &database)
        : m_log(Logging::getInstance())
        , m_database(database)
        , m_strTableName("")
        , m_cmdFirstTimeSetup("")
        , m_cmdTableExists("")
        , m_cmdCreateTable("")
        , m_cmdCreateIndex("")
        , m_cmdDropTable("")
        , m_cmdDropIndex("")
    { }

    ~BaseDao()
    { 
        m_log.write<Logging::DEBUG_LOG>("~BaseDao()");
    }

    Logging        &m_log;

    // Handle to Database
    SQLW::Database &m_database;

    // Static Table Queries
    std::string m_strTableName;
    std::string m_cmdFirstTimeSetup;
    std::string m_cmdTableExists;
    std::string m_cmdCreateTable;
    std::string m_cmdCreateIndex;
    std::string m_cmdDropTable;
    std::string m_cmdDropIndex;

    // Dynamic Callbacks to Calling Class for Specific Object Mappings
    std::function<void(query_ptr qry, std::shared_ptr<T> obj)> m_result_callback;
    std::function<void(query_ptr qry, std::shared_ptr<T> obj,
                       std::vector< std::pair<std::string, std::string> > &values)> m_columns_callback;

    std::function<std::string(std::string qry, std::shared_ptr<T> obj)> m_insert_callback;
    std::function<std::string(std::string qry, std::shared_ptr<T> obj)> m_update_callback;


    /**
     * @brief Check if the Table Exists in Database
     * @return
     */
    bool baseDoesTableExist()
    {
        bool result = false;        

        // Make Sure Database Reference is Connected
        if(!m_database.isConnected())
        {
            m_log.write<Logging::ERROR_LOG>(m_strTableName, "Error, Database is not connected!", __LINE__, __FILE__);
            return result;
        }

        // Create Pointer and Connect Query Object to Database.
        query_ptr qry(new SQLW::Query(m_database));

        if(!qry || !qry->isConnected())
        {
            m_log.write<Logging::ERROR_LOG>(m_strTableName, "Error, Query has no connection to the database", __LINE__, __FILE__);
            return result;
        }

        // Execute and get result.
        if(qry->getResult(m_cmdTableExists))
        {
            long rows = qry->getNumRows();

            if(rows > 0)
            {
                m_log.write<Logging::DEBUG_LOG>(m_strTableName, "Table Exists!", __LINE__, __FILE__);
                result = true;
            }
            else
            {
                // No rows means the table doesn't exist!
                m_log.write<Logging::DEBUG_LOG>(m_strTableName, "table doesn't exist returned rows", rows, __LINE__, __FILE__);
            }
        }
        else
        {
            m_log.write<Logging::ERROR_LOG>(m_strTableName, "baseDoesTableExist getResult()", __LINE__, __FILE__);
        }

        return result;
    }

    /**
     * @brief Run Setup Params for SQL Database.
     */
    bool baseFirstTimeSetupParams()
    {
        bool result = false;

        // Make Sure Database Reference is Connected
        if(!m_database.isConnected())
        {
            m_log.write<Logging::ERROR_LOG>(m_strTableName, "Error, Database is not connected!", __LINE__, __FILE__);
            return result;
        }

        // Create Pointer and Connect Query Object to Database.
        query_ptr qry(new SQLW::Query(m_database));

        if(!qry || !qry->isConnected())
        {
            m_log.write<Logging::ERROR_LOG>(m_strTableName, "Error, Query has no connection to the database", __LINE__, __FILE__);
            return result;
        }

        // Execute Statement.
        result = qry->execute(m_cmdFirstTimeSetup);
        return result;
    }

    /**
     * @brief Create Table
     */
    bool baseCreateTable()
    {
        bool result = false;

        // Make Sure Database Reference is Connected
        if(!m_database.isConnected())
        {
            m_log.write<Logging::ERROR_LOG>(m_strTableName, "Error, Database is not connected!", __LINE__, __FILE__);
            return result;
        }

        // Create Pointer and Connect Query Object to Database.
        query_ptr qry(new SQLW::Query(m_database));

        if(!qry || !qry->isConnected())
        {
            m_log.write<Logging::ERROR_LOG>(m_strTableName, "Error, Query has no connection to the database", __LINE__, __FILE__);
            return result;
        }

        // Create List of statements to execute in a single transaction.
        std::vector<std::string> statements;
        statements.push_back(m_cmdCreateTable);

        if(m_cmdCreateIndex.size() > 0)
        {
            statements.push_back(m_cmdCreateIndex);
        }

        // Execute Transaction.
        result = qry->executeTransaction(statements);
        return result;
    }

    /**
     * @brief Drop Table
     */
    bool baseDropTable()
    {
        bool result = false;

        // Make Sure Database Reference is Connected
        if(!m_database.isConnected())
        {
            m_log.write<Logging::ERROR_LOG>(m_strTableName, "Error, Database is not connected!", __LINE__, __FILE__);
            return result;
        }

        // Create Pointer and Connect Query Object to Database.
        query_ptr qry(new SQLW::Query(m_database));

        if(!qry || !qry->isConnected())
        {
            m_log.write<Logging::ERROR_LOG>(m_strTableName, "Error, Query has no connection to the database", __LINE__, __FILE__);
            return result;
        }

        // Create List of statements to execute in a single transaction.
        std::vector<std::string> statements;

        if(m_cmdDropIndex.size() > 0)
        {
            statements.push_back(m_cmdDropIndex);
        }

        statements.push_back(m_cmdDropTable);

        // Execute Transaction.
        result = qry->executeTransaction(statements);
        return result;
    }

    /**
     * @brief Pulls results by FieldNames into their Class Variables.
     * @param qry
     * @param one
     */
    void basePullResult(query_ptr qry, std::shared_ptr<T> obj)
    {
        m_result_callback(qry, obj);
    }

    /**
     * @brief Used for Insert Statement
     *        This takes a pair, and translates to (Column, .. ) VALUES (%d, %Q,) for formatting
     * @param qry
     * @param one
     * @param values
     */
    void baseFillColumnValues(query_ptr qry, std::shared_ptr<T> obj,
                              std::vector< std::pair<std::string, std::string> > &values)
    {
        m_columns_callback(qry, obj, values);
    }

    /**
     * @brief Builds Insert Insert Query String from object
     * @param qry
     * @param obj
     * @return
     */
    std::string baseInsertQryString(query_ptr qry, std::shared_ptr<T> obj)
    {
        std::stringstream ssColumn;
        std::stringstream ssType;
        std::vector< std::pair<std::string, std::string> >::iterator it;
        std::vector< std::pair<std::string, std::string> > values;

        ssColumn << "INSERT INTO " + m_strTableName + " (";
        ssType << ") VALUES (";

        // Populate the Pairs.
        baseFillColumnValues(qry, obj, values);

        // Build Query (Columns) VALUES (Types) ..  ie %d, %Q into a full string.
        it = values.begin();

        for(int i = 0; it != values.end(); i++)
        {
            // First Build Column Names
            ssColumn << (*it).first;
            ssType << (*it).second;

            ++it;

            if(it != values.end())
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
        std::string result = m_insert_callback(newQueryString, obj);

        return result;
    }

    /**
     * @brief Builds Query Update String From Object
     * @param qry
     * @param obj
     * @return
     */
    std::string baseUpdateQryString(query_ptr qry, std::shared_ptr<T> obj)
    {
        std::stringstream ssColumn;
        std::vector< std::pair<std::string, std::string> >::iterator it;
        std::vector< std::pair<std::string, std::string> > values;

        // Setup start of Statement
        ssColumn << "UPDATE " + m_strTableName + " SET ";

        // Populate the Pairs. Variable = %Q formatted string.
        baseFillColumnValues(qry, obj, values);

        // Build Query (Columns) = (Values) ..  ie %d, %Q into a full string.
        it = values.begin();

        for(int i = 0; it != values.end(); i++)
        {
            ssColumn << (*it).first << "=" << (*it).second;
            ++it;

            if(it != values.end())
            {
                ssColumn << ", ";
            }
        }

        // Closing For Query.
        std::string newQueryString = ssColumn.str();
        newQueryString.append(" WHERE iId = %ld; ");

        // Mprint statement to avoid injections.
        std::string result = m_update_callback(newQueryString, obj);

        return result;
    }

    /**
     * @brief Updates a Record in the database!
     * @param obj
     * @return
     */
    bool baseUpdateRecord(std::shared_ptr<T> obj)
    {
        bool result = false;

        // Make Sure Database Reference is Connected
        if(!m_database.isConnected())
        {
            m_log.write<Logging::ERROR_LOG>(m_strTableName, "Error, Database is not connected!", __LINE__, __FILE__);
            return result;
        }

        // Create Pointer and Connect Query Object to Database.
        query_ptr qry(new SQLW::Query(m_database));

        if(!qry || !qry->isConnected())
        {
            m_log.write<Logging::ERROR_LOG>(m_strTableName, "Error, Query has no connection to the database", __LINE__, __FILE__);
            return result;
        }

        // Build update string
        std::string queryString = baseUpdateQryString(qry, obj);

        // Execute Update in a Transaction, rollback if fails.
        std::vector<std::string> statements;
        statements.push_back(queryString);
        result = qry->executeTransaction(statements);

        return result;
    }

    /**
     * @brief Inserts a new Record in the database!
     * @param obj
     * @return
     */
    long baseInsertRecord(std::shared_ptr<T> obj)
    {
        bool result = false;
        long lastInsertId = -1;

        // Make Sure Database Reference is Connected
        if(!m_database.isConnected())
        {
            m_log.write<Logging::ERROR_LOG>(m_strTableName, "Error, Database is not connected!", __LINE__, __FILE__);
            return result;
        }

        // Create Pointer and Connect Query Object to Database.
        query_ptr qry(new SQLW::Query(m_database));

        if(!qry || !qry->isConnected())
        {
            m_log.write<Logging::ERROR_LOG>(m_strTableName, "Error, Query has no connection to the database", __LINE__, __FILE__);
            return result;
        }

        // Build update string
        std::string queryString = baseInsertQryString(qry, obj);

        // Execute Update in a Transaction, rollback if fails.
        std::vector<std::string> statements;
        statements.push_back(queryString);
        result = qry->executeTransaction(statements);

        // We need the insert id for table
        if(result)
        {
            lastInsertId = qry->getInsertId();
        }

        return lastInsertId;
    }

    /**
     * @brief Deletes a Record by Id
     * @param oneId
     * @return
     */
    bool baseDeleteRecord(long id)
    {
        bool results = false;

        // Make Sure Database Reference is Connected
        if(!m_database.isConnected())
        {
            m_log.write<Logging::ERROR_LOG>(m_strTableName, "Error, Database is not connected!", __LINE__, __FILE__);
            return results;
        }

        // Create Pointer and Connect Query Object to Database.
        query_ptr qry(new SQLW::Query(m_database));

        if(!qry || !qry->isConnected())
        {
            m_log.write<Logging::ERROR_LOG>(m_strTableName, "Error, Query has no connection to the database", __LINE__, __FILE__);
            return results;
        }

        // Build string
        char *result = sqlite3_mprintf("DELETE FROM %Q WHERE iId = %ld;", m_strTableName.c_str(), id);
        std::string queryString(result);
        sqlite3_free(result);

        // Execute Update in a Transaction, rollback if fails.
        std::vector<std::string> statements;
        statements.push_back(queryString);
        results = qry->executeTransaction(statements);

        return results;
    }

    /**
     * @brief Retrieve Record By Id.
     * @param id
     * @return
     */
    std::shared_ptr<T> baseGetRecordById(long id)
    {
        std::shared_ptr<T> obj(new T);

        // Make Sure Database Reference is Connected
        if(!m_database.isConnected())
        {
            m_log.write<Logging::ERROR_LOG>(m_strTableName, "Error, Database is not connected!", __LINE__, __FILE__);
            return obj;
        }

        // Create Pointer and Connect Query Object to Database.
        query_ptr qry(new SQLW::Query(m_database));

        if(!qry->isConnected())
        {
            m_log.write<Logging::ERROR_LOG>(m_strTableName, "Error, Query has no connection to the database", __LINE__, __FILE__);
            return obj;
        }

        // Build Query String
        char *result = sqlite3_mprintf("SELECT * FROM %Q WHERE iID = %ld;", m_strTableName.c_str(), id);
        std::string queryString(result);
        sqlite3_free(result);

        // Execute Query.
        if(qry->getResult(queryString))
        {
            long rows = qry->getNumRows();

            if(rows > 0)
            {
                qry->fetchRow();
                basePullResult(qry, obj);
            }
            else
            {
                m_log.write<Logging::ERROR_LOG>(m_strTableName, "Error, getRecordById Returned Rows", rows, __LINE__, __FILE__);
            }
        }
        else
        {
            m_log.write<Logging::ERROR_LOG>(m_strTableName, "Error, getRecordById getResult()", __LINE__, __FILE__);
        }

        return obj;
    }

    /**
     * @brief Retrieve All Records in a Table
     * @return
     */
    std::vector< std::shared_ptr<T> > baseGetAllRecords()
    {
        std::shared_ptr<T> obj(new T);
        std::vector<std::shared_ptr<T>> list;

        // Make Sure Database Reference is Connected
        if(!m_database.isConnected())
        {
            m_log.write<Logging::ERROR_LOG>(m_strTableName, "Error, Database is not connected!", __LINE__, __FILE__);
            return list;
        }

        // Create Pointer and Connect Query Object to Database.
        query_ptr qry(new SQLW::Query(m_database));

        if(!qry->isConnected())
        {
            m_log.write<Logging::ERROR_LOG>(m_strTableName, "Error, Query has no connection to the database", __LINE__, __FILE__);
            return list;
        }

        // Build Query String
        char *result = sqlite3_mprintf("SELECT * FROM %Q;", m_strTableName.c_str());
        std::string queryString(result);
        sqlite3_free(result);

        // Execute Query.
        if(qry->getResult(queryString))
        {
            long rows = qry->getNumRows();

            if(rows > 0)
            {
                while(qry->fetchRow())
                {
                    obj.reset(new T);
                    basePullResult(qry, obj);
                    list.push_back(obj);
                }
            }
            else
            {
                m_log.write<Logging::ERROR_LOG>(m_strTableName, "Error, baseGetAllRecords Returned Rows", rows, __LINE__, __FILE__);
            }
        }
        else
        {
            m_log.write<Logging::ERROR_LOG>(m_strTableName, "Error, baseGetAllRecords getResult()", __LINE__, __FILE__);
        }

        return list;
    }

    /**
     * @brief Retrieve Count of All Records in a Table
     * @return
     */
    long baseGetRecordsCount()
    {
        std::shared_ptr<T> obj(new T);
        std::vector<std::shared_ptr<T>> list;

        // Make Sure Database Reference is Connected
        if(!m_database.isConnected())
        {
            m_log.write<Logging::ERROR_LOG>(m_strTableName, "Error, Database is not connected!", __LINE__, __FILE__);
            return list.size();
        }

        // Create Pointer and Connect Query Object to Database.
        query_ptr qry(new SQLW::Query(m_database));

        if(!qry->isConnected())
        {
            m_log.write<Logging::ERROR_LOG>(m_strTableName, "Error, Query has no connection to the database", __LINE__, __FILE__);
            return list.size();
        }

        // Build Query String
        char *result = sqlite3_mprintf("SELECT * FROM %Q;", m_strTableName.c_str());
        std::string queryString(result);
        sqlite3_free(result);

        // Execute Query.
        if(qry->getResult(queryString))
        {
            long rows = qry->getNumRows();

            if(rows > 0)
            {
                while(qry->fetchRow())
                {
                    obj.reset(new T);
                    basePullResult(qry, obj);
                    list.push_back(obj);
                }
            }
            else
            {
                m_log.write<Logging::ERROR_LOG>(m_strTableName, "Error, baseGetRecordsCount Returned Rows", rows, __LINE__, __FILE__);
            }
        }
        else
        {
            m_log.write<Logging::ERROR_LOG>(m_strTableName, "Error, baseGetRecordsCount getResult()", __LINE__, __FILE__);
        }

        return list.size();
    }

};

#endif // BASE_DAO_HPP
