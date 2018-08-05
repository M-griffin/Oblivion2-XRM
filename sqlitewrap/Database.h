/**
 *	Database.h
 *
 * Rewritten / author: 2016-02-19 / mrmisticismo@hotmail.com
 * Published / author: 2005-08-12 / grymse@alhem.net
 * Copyright (C) 2015-2018  Michael Griffin
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
    	std::string safeString(const std::string& );
		
        // Make string xml safe.
        std::string xmlSafeString(const std::string&);

        // Convert string to 64-bit integer.
        int64_t alphaToBigInt(const std::string&);

        // Convert string to unsigned 64-bit integer.
        uint64_t alphaToUnsignedBitInt(const std::string&);

    private:

        void databaseError(const char *format, ...);

        std::string         m_database;
        m_database_pool     m_opendbs;
        IError             *m_errhandler;
        Mutex&              m_mutex;
        bool                m_is_mutex;
    };



} // namespace SQLW


#endif // _DATABASE_H
