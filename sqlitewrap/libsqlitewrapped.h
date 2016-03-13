/**
 *	IError.h
 *
 * Rewritten / author: 2016-02-19 / mrmisticismo@hotmail.com
 * Published / author: 2005-08-12 / grymse@alhem.net
 * Copyright (C) 2015-2016  Michael Griffin
 * Copyright (C) 2001-2006  Anders Hedstrom
 * This program is made available under the terms of the GNU GPL.
 */

#ifndef _IERROR_H_SQLITE
#define _IERROR_H_SQLITE

#include <string>

namespace SQLW
{
    class Database;
    class Query;

    /** Log class interface. */
    class IError
    {
    public:
        virtual void databaseError(Database&, const std::string&) = 0;
        virtual void databaseError(Database&, Query&, const std::string&) = 0;
    };

} // namespace SQLW {

#endif // _IERROR_H
/**
 *	StderrLog.h
 *
 * Rewritten / author: 2016-02-19 / mrmisticismo@hotmail.com
 * Published / author: 2005-08-12 / grymse@alhem.net
 * Copyright (C) 2015-2016  Michael Griffin
 * Copyright (C) 2001-2006  Anders Hedstrom
 * This program is made available under the terms of the GNU GPL.
 */

#ifndef _STDERRLOG_H_SQLITE
#define _STDERRLOG_H_SQLITE

#include "IError.h"
#include <string>

namespace SQLW
{
    class Database;

    /** Log class writing to standard error. */
    class StderrLog : public IError
    {
    public:
        void databaseError(Database&, const std::string&);
        void databaseError(Database&, Query&, const std::string&);
    };

} // namespace SQLW {


#endif // _STDERRLOG_H
/**
 *	SysLog.h
 *
 * Rewritten / author: 2016-02-19 / mrmisticismo@hotmail.com
 * Published / author: 2005-08-12 / grymse@alhem.net
 * Copyright (C) 2015-2016  Michael Griffin
 * Copyright (C) 2001-2006  Anders Hedstrom
 * This program is made available under the terms of the GNU GPL.
 */
#ifndef _SYSLOG_H_SQLITE
#define _SYSLOG_H_SQLITE
#ifndef _WIN32

#include <syslog.h>


namespace SQLW
{
    /** Log class writing to syslog. */
    class SysLog : public IError
    {
    public:
        SysLog(const std::string& = "database", int = LOG_PID, int = LOG_USER);
        virtual ~SysLog();

        void databaseError(Database&, const std::string&);
        void databaseError(Database&, Query&, const std::string&);

    };

} // namespace SQLW {

#endif // WIN32
#endif // _SYSLOG_H
/**
 *	Database.h
 *
 * Rewritten / author: 2016-02-19 / mrmisticismo@hotmail.com
 * Published / author: 2005-08-12 / grymse@alhem.net
 * Copyright (C) 2015-2016  Michael Griffin
 * Copyright (C) 2001-2006  Anders Hedstrom
 * This program is made available under the terms of the GNU GPL.
 */

#ifndef _DATABASE_H_SQLITE
#define _DATABASE_H_SQLITE

#include <sqlite3.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <pthread.h>
#endif

#include <string>
#include <list>
#include <stdint.h>

namespace SQLW
{
    // handle unused paramters for template funcations.
    #define UNUSED(x) [&x]{}()

    // Forward Declarations
    class IError;
    class Query;
    class Mutex;

    /** Connection information and pool. */
    class Database
    {
    public:
        /** Mutex container class, used by Lock. ingroup threading */

        class Mutex
        {
        public:

            Mutex();
            ~Mutex();
            void MutexLock();
            void Unlock();

        private:

// Replace this with std::mutex!
#ifdef _WIN32
            HANDLE          m_mutex;
#else
            pthread_mutex_t m_mutex;
#endif
        };
    private:

        // Mutex helper class.
        class MutexLock
        {
        public:
            MutexLock(Mutex& mutex, bool use);
            ~MutexLock();
        private:
            Mutex& m_mutex;
            bool m_is_inuse;
        };

    public:

        // Connection pool struct.
        struct DatabasePool
        {
            DatabasePool()
                : db(nullptr)
                , busy(false)
            { }
            ~DatabasePool()
            {
                if (db)
                {
                    sqlite3_close_v2(db);
                }
            }

            sqlite3 *db;
            bool busy;
        };
        typedef std::list<DatabasePool *> m_database_pool;

    public:

        // use file
        Database(const std::string& database, IError *databaseError = nullptr);

        // Use file + thread safe
        Database(Mutex&, const std::string& database, IError *databaseError = nullptr);

        virtual ~Database();

        /** try to establish connection with given host */
        bool isConnected();

        void errorHandler(IError *);
        void databaseError(Query&,const char *format, ...);
        void databaseError(Query&,const std::string&);

        /** Request a database connection.
        The "grabdb" method is used by the Query class, so that each object instance of Query gets a unique
        database connection. I will re-implement your connection check logic in the Query class, as that's where
        the database connection is really used.
        It should be used something like this.
        {
        	Query q(db);
        	if (!q.isConnected())
            {
        		 return false;
            }
        	q.execute("delete * from user"); // well maybe not
        }

        When the Query object is deleted, then "freeDatabasePool" is called - the database connection stays open in the
        m_opendbs vector. New Query objects can then reuse old connections.
        */

        DatabasePool *addDatabasePool();

        void freeDatabasePool(DatabasePool *odb);

    	// Escape string - change all ' to ''.
    	std::string safestr(const std::string& );
		
        // Make string xml safe.
        std::string xmlsafestr(const std::string&);

        // Convert string to 64-bit integer.
        int64_t a2bigint(const std::string&);

        // Convert string to unsigned 64-bit integer.
        uint64_t a2ubigint(const std::string&);

    private:

        /*
        Database(const Database& db)
            : m_mutex(m_mutex)
        {
            this = db;
        }*/

/*
        Database& operator=(const Database&)
        {
            return *this;
        }*/

        void databaseError(const char *format, ...);

        std::string         m_database;
        m_database_pool     m_opendbs;
        IError             *m_errhandler;
        Mutex&              m_mutex;
        bool                m_is_mutex;
    };



} // namespace SQLW


#endif // _DATABASE_H
/**
 *	Query.h
 *
 * Rewritten / author: 2016-02-19 / mrmisticismo@hotmail.com
 * Published / author: 2005-08-12 / grymse@alhem.net
 * Copyright (C) 2015-2016  Michael Griffin
 * Copyright (C) 2001-2006  Anders Hedstrom
 * This program is made available under the terms of the GNU GPL.
 */

#ifndef _QUERY_H_SQLITE
#define _QUERY_H_SQLITE

#include <sqlite3.h>

#include <iostream>
#include <ctime>
#include <string>
#include <map>
#include <vector>
#include <memory>
#include <stdint.h>
#include <sstream>


namespace SQLW
{
    /** SQL Statement execute / result. */
    class Query
    {
    public:
        /** Constructor accepting reference to database object. */
        Query(Database& dbin);
        /** Constructor accepting reference to database object
        	and query string to execute. */
        Query(Database& dbin,const std::string& sql);
        ~Query();

        /** Check if database object is connectable. */
        bool isConnected();
        /** Return reference to database object. */
        Database& getDatabase() const;

        /** Return string containing last query executed. */
        const std::string& getLastQuery();

        /** execute() returns true if query is successful,
        	does not store result. */
        bool execute(const std::string& sql);

        /** Execute query and store result. */
        sqlite3_stmt *getResult(const std::string& sql);
        /** Free stored result, must be called after get_result() before calling
        	execute()/get_result() again. */

        void freeResult();
        /** Fetch next result row.
        	\return false if there was no row to fetch (end of rows) */

        // Fill the Resource with the current row, also incriments to next row.
        bool fetchRow();

        /** Get id of last insert. */
        sqlite_int64 getInsertId();

        /** Returns 0 if there are no rows to fetch. */
        long getNumRows();

        /** Number of columns in current result. */
        int getNumCols();

        /** Last error string. */
        std::string GetError();

        /** Last error code. */
        int GetErrno();

        /** Check if column x in current row is null. */
        bool isNull(int x);

        /**
         * @brief Execute Query and Get first Result as the following.
         */

        /** Execute query and return first result as a string. */
        const char *exeGetCharString(const std::string& sql);

        /** Execute query and return first result as a long integer. */
        long exeGetResultLong(const std::string& sql);

        /** Execute query and return first result as a double. */
        double exeGetResultDouble(const std::string& sql);

        /**
         * @brief On FetchRow, get the current column values as.
         */

        /** Return column named x as a string value. */
        const char *getstr(const std::string& x);

        /** Return column x as a string value. */
        const char *getstr(int x);

        /** Return next column as a string value - see rowcount. */
        const char *getstr();

        /**
         * @brief On FetchRow, get the current column values as.
         */

        /** Return column named x as a long integer. */
        long getval(const std::string& x);

        /** Return column x as a long integer. */
        long getval(int x);

        /** Return next column as a long integer - see rowcount. */
        long getval();

        /**
         * @brief On FetchRow, get the current column values as.
         */

        /** Return column named x as an unsigned long integer. */
        unsigned long getuval(const std::string& x);

        /** Return column x as an unsigned long integer. */
        unsigned long getuval(int x);

        /** Return next column as an unsigned long integer. */
        unsigned long getuval();

        /**
         * @brief On FetchRow, get the current column values as.
         */

        /** Return column named x as a 64-bit integer value. */
        int64_t getbigint(const std::string& x);

        /** Return column x as a 64-bit integer value. */
        int64_t getbigint(int x);

        /** Return next column as a 64-bit integer value. */
        int64_t getbigint();

        /** Return column named x as an unsigned 64-bit integer value. */
        uint64_t getubigint(const std::string& x);

        /** Return column x as an unsigned 64-bit integer value. */
        uint64_t getubigint(int x);

        /** Return next column as an unsigned 64-bit integer value. */
        uint64_t getubigint();

        /**
         * @brief On FetchRow, get the current column values as.
         */

        /** Return column named x as a double. */
        double getnum(const std::string& x);

        /** Return column x as a double. */
        double getnum(int x);

        /** Return next column as a double. */
        double getnum();

        /**
         * @brief On FetchRow, Get Field Value by Column Name and Type
         */

        // Int
        void getFieldValue(int &type, int index)
        {
            //UNUSED(type);
            type = static_cast<int>(sqlite3_column_int(res, index));
        }

        // Long or Time_T for Date/Time.
        void getFieldValue(long &type, int index)
        {
            type = static_cast<long>(sqlite3_column_int64(res, index));
        }

        // Long Long
        void getFieldValue(long long &type, int index)
        {
            type = static_cast<long long>(sqlite3_column_int64(res, index));
        }

        // Double
        void getFieldValue(double &type, int index)
        {
            type = static_cast<double>(sqlite3_column_double(res, index));
        }

        // Float
        void getFieldValue(float &type, int index)
        {
            type = static_cast<float>(sqlite3_column_double(res, index));
        }

        // Long Double
        void getFieldValue(long double &type, int index)
        {
            type = static_cast<long double>(sqlite3_column_double(res, index));
        }

        // Std::String
        void getFieldValue(std::string &type, int index)
        {
            const char *text_result = reinterpret_cast<const char *>(sqlite3_column_text(res , index));
            type = text_result;
        }

        // Single Character
        void getFieldValue(char &type, int index)
        {
            const char *result = reinterpret_cast<const char *>(sqlite3_column_text(res , index));
            type = result[0];
        }

        // Char *
        const char *getFieldValue(char *, int index)
        {
            const char *result = reinterpret_cast<const char *>(sqlite3_column_text(res , index));
            return result;
        }

        /*
        // Char *, or BLOB. sqlite3_column_blob ?  test this lateron!
        const char *getFieldValue(char *type, int index)
        {
            if (strcmp(type, "BLOB") == 0)
            {
                // Test if we need to get len in bytes then cutoff!
                const char *result = reinterpret_cast<const char *>(sqlite3_column_blob(res , index));
                return result;
            }
            const char *result = reinterpret_cast<const char *>(sqlite3_column_text(res , index));
            return result;
        }*/

        // Handle Boolean Values.
        void getFieldValue(bool &type, int index)
        {
            int result = static_cast<int>(sqlite3_column_int(res , index));
            if (result == 1)
                type = true;
            else
                type = false;
        }

        /**
        * @brief Template to Get Field Value by Column Name and Populate Type passed.
        */
        template <typename TT, typename T>
        T getFieldByName(const TT &tt, T &t)
        {
            // Grab the index of the Matching Field Name
            std::map<std::string, int>::iterator it;
            int index = 0;

            if (m_nmap.empty())
            {
                return t;
            }

            it = m_nmap.find(tt);
            if (it != m_nmap.end())
            {
                index = it->second;
                --index;
            }
            else
            {
                return t;
            }
            if (index >= 0)
            {
                switch(sqlite3_column_type(res, index))
                {
                    case SQLITE_NULL:
                        break;

                    default :
                        // Template to pull field value dynamically from overloads.
                        getFieldValue(t, index);
                        return t;
                }
            }
            return t;
        }


        std::string getFieldType(float &)
        {
            std::string result("%f");
            return result;
        }

        std::string getFieldType(double &)
        {
            std::string result("%d");
            return result;
        }

        std::string getFieldType(long long &)
        {
            // %llu or lld ?!?
            std::string result("%llu");
            return result;
        }

        std::string getFieldType(char &)
        {
            std::string result("%Q");
            return result;
        }

        std::string getFieldType(bool &)
        {
            std::string result("%d");
            return result;
        }

        std::string getFieldType(char *)
        {
            std::string result("%Q");
            return result;
        }

        std::string getFieldType(std::string &)
        {
            std::string result("%Q");
            return result;
        }

        std::string getFieldType(long &)
        {
            std::string result("%ld");
            return result;
        }

        std::string getFieldType(int &)
        {
            std::string result("%d");
            return result;
        }

        /**
         * @brief This takes a Description, and a Type, for translation inserts statements (Column) VALUES (TYPE)
         */
        template <typename TT, typename T>
        std::pair<std::string, std::string> translateFieldName(const TT &tt, T &t)
        {
            std::pair<std::string, std::string> pair;

            // Template to pull field type, int = %d, text = '%q' etc..
            pair = std::make_pair(tt, getFieldType(t));
            return pair;
        }
               
        /**
         * Create a new Executate Transaction
         */
        bool executeTransaction(const std::vector<std::string> &statements);

    private:

        /** Hide the copy constructor. */
        /*
        Query(const Query& q)
            : m_db(q.getDatabase())
        {
            std::cout << "Query Constructor: q.getDatabase()" << std::endl;
            *this = q;
        }*/

        /** Hide the assignment operator. */
        /*
        Query& operator=(const Query&)
        {
            std::cout << "Query Constructor: return *this" << std::endl;
            return *this;
        }*/

        /** Print error to debug class. */
        void queryError(const std::string&);

        

        Database&                  m_db;           ///< Reference to database object
        Database::DatabasePool    *odb;            ///< Connection pool handle

        sqlite3_stmt              *res;            ///< Stored result
        bool                       row;            ///< true if fetch_row succeeded
        short                      rowcount;       ///< Current column pointer in result
        std::string                m_tmpstr;       ///< Used to store result in get_string() call
        std::string                m_last_query;   ///< Last query executed
        int                        cache_rc;       ///< Cached result after call to get_result()
        bool                       cache_rc_valid; ///< Indicates cache_rc is valid
        int                        m_row_count;    ///< 0 if get_result() returned no rows

        std::map<std::string, int> m_nmap;         ///< map translating column names to index
        int                        m_num_cols;     ///< number of columns in result

    };

} // namespace SQLW


#endif // _QUERY_H
