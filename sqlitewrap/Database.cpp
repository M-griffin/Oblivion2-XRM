/**
 *	Database.cpp
 *
 * Rewritten / author: 2016-02-19 / mrmisticismo@hotmail.com
 * Published / author: 2005-08-12 / grymse@alhem.net
 * Copyright (C) 2015-2018  Michael Griffin
 * Copyright (C) 2001-2006  Anders Hedstrom
 * This program is made available under the terms of the GNU GPL.
 */

#include <sqlite3.h>

#include <stdio.h>
#ifdef _WIN32
//#pragma warning(disable:4786)
#endif

#include <iostream>
#include <string>
#include <map>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstdarg>
#include <sstream>

#include "IError.h"
#include "Database.h"


namespace SQLW
{
    Database::Database(const std::string& database, IError *error)
        : m_database(database)
        , m_errhandler(error)
        , m_mutex(m_mutex)
        , m_is_mutex(false)
    {
        std::cout << "Database Created" << std::endl;
    }


    Database::Database(Mutex& m, const std::string& database, IError *error)
        : m_database(database)
        , m_errhandler(error)
        , m_mutex(m)
        , m_is_mutex(true)
    {
        std::cout << "Database Created w/ Mutex" << std::endl;
    }


    Database::~Database()
    {
        std::cout << "~Database" << std::endl;

        // Check Open Databases and pop off stack!
        m_database_pool::iterator it;
        while(m_opendbs.size())
        {
            it = m_opendbs.begin();
            DatabasePool *p = *it;
            if(p->busy)
            {
                databaseError("destroying Database object before Query object");
            }
            delete p;
            m_opendbs.erase(it);
        }
    }


    void Database::errorHandler(IError *p)
    {
        m_errhandler = p;
    }


    Database::DatabasePool *Database::addDatabasePool()
    {
        MutexLock lck(m_mutex, m_is_mutex);
        DatabasePool *odb = nullptr;

        for(m_database_pool::iterator it = m_opendbs.begin(); it != m_opendbs.end(); ++it)
        {
            odb = *it;
            if(!odb->busy)
            {
                break;
            }
            else
            {
                odb = nullptr;
            }
        }

        if(!odb)
        {
            odb = new DatabasePool;
            if(!odb)
            {
                databaseError("addDatabasePool: DatabasePool couldn't be created");
                return nullptr;
            }

            // Look into shared chache more!
            // int sqlite3_enable_shared_cache(int);
            // SQLITE_OPEN_SHAREDCACHE.
            int rc = sqlite3_open_v2(m_database.c_str(), &odb->db, SQLITE_OPEN_CREATE | SQLITE_OPEN_READWRITE, NULL);
            if(rc)
            {
                databaseError("Can't open database: %s\n", sqlite3_errmsg(odb -> db));
                delete odb;
                return nullptr;
            }

            odb->busy = true;
            m_opendbs.push_back(odb);
        }
        else
        {
            odb->busy = true;
        }
        return odb;
    }


    void Database::freeDatabasePool(Database::DatabasePool *odb)
    {
        MutexLock lck(m_mutex, m_is_mutex);
        if(odb)
        {
            odb->busy = false;
        }
    }


    void Database::databaseError(const char *format, ...)
    {
        if(m_errhandler)
        {
            va_list ap;
            char errstr[5000];
            va_start(ap, format);
#ifdef WIN32
            vsprintf(errstr, format, ap);
#else
            vsnprintf(errstr, 5000, format, ap);
#endif
            va_end(ap);
            m_errhandler->databaseError(*this, errstr);
        }
    }


    void Database::databaseError(Query& q,const char *format, ...)
    {
        if(m_errhandler)
        {
            va_list ap;
            char errstr[5000];
            va_start(ap, format);
#ifdef WIN32
            vsprintf(errstr, format, ap);
#else
            vsnprintf(errstr, 5000, format, ap);
#endif
            va_end(ap);
            m_errhandler->databaseError(*this, q, errstr);
        }
    }


    void Database::databaseError(Query& q,const std::string& msg)
    {
        if(m_errhandler)
        {
            m_errhandler->databaseError(*this, q, msg);
        }
    }


    bool Database::isConnected()
    {
        DatabasePool *odb = addDatabasePool();
        if(!odb)
        {
            return false;
        }
        freeDatabasePool(odb);
        return true;
    }


    Database::MutexLock::MutexLock(Mutex& mutex, bool use)
        : m_mutex(mutex)
        , m_is_inuse(use)
    {
        if(m_is_inuse)
        {
            m_mutex.MutexLock();
        }
    }


    Database::MutexLock::~MutexLock()
    {
        if(m_is_inuse)
        {
            m_mutex.Unlock();
        }
    }


    Database::Mutex::Mutex()
    {
#ifdef _WIN32
        m_mutex = ::CreateMutex(nullptr, FALSE, nullptr);
#else
        pthread_mutex_init(&m_mutex, nullptr);
#endif
    }


    Database::Mutex::~Mutex()
    {
#ifdef _WIN32
        ::CloseHandle(m_mutex);
#else
        pthread_mutex_destroy(&m_mutex);
#endif
    }


    void Database::Mutex::MutexLock()
    {
#ifdef _WIN32
        DWORD d = WaitForSingleObject(m_mutex, INFINITE);
        UNUSED(d);
        // %! check 'd' for result
#else
        pthread_mutex_lock(&m_mutex);
#endif
    }


    void Database::Mutex::Unlock()
    {
#ifdef _WIN32
        ::ReleaseMutex(m_mutex);
#else
        pthread_mutex_unlock(&m_mutex);
#endif
    }

    /*
     * Should not be used, use sqlite3_mprintf()
     * To avoid Injection Attacks.
     */
    std::string Database::safeString(const std::string& str)
    {
        std::string str2;
        for(size_t i = 0; i < str.size(); i++)
        {
            switch(str[i])
            {
                case '\'': // Double Single Quotes
                    str2 += "\'\'";
                    break;

                case '\\': // Double Back Slashes.
                    str2 += "\\\\";
                    break;

                case '\0': // Skip nullptrs, not allowed!
                    break;

                default:
                    str2 += str[i];
            }
        }
        return str2;
    }


    std::string Database::xmlSafeString(const std::string& str)
    {
        std::string str2;
        for(size_t i = 0; i < str.size(); i++)
        {
            switch(str[i])
            {
                case '&':
                    str2 += "&amp;";
                    break;
                case '<':
                    str2 += "&lt;";
                    break;
                case '>':
                    str2 += "&gt;";
                    break;
                case '"':
                    str2 += "&quot;";
                    break;
                case '\'':
                    str2 += "&apos;";
                    break;
                default:
                    str2 += str[i];
            }
        }
        return str2;
    }

    // Convert String to signed int64
    int64_t Database::alphaToBigInt(const std::string& str)
    {
        std::istringstream ss(str);
        int64_t bitInt;
        ss >> bitInt;
        if (ss.fail())
        {
            bitInt = 0;
        }
        return bitInt;
    }

    // Convert String to Unsigned int64
    uint64_t Database::alphaToUnsignedBitInt(const std::string& str)
    {
        std::istringstream ss(str);
        uint64_t bitInt;
        ss >> bitInt;
        if (ss.fail())
        {
            bitInt = 0;
        }
        return bitInt;
    }

} // namespace SQLW {
