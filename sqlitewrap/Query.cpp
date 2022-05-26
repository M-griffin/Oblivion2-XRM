/**
 *	Query.cpp
 *
 * Rewritten / author: 2016-02-19 / mrmisticismo@hotmail.com
 * Published / author: 2005-08-12 / grymse@alhem.net
 * Copyright (C) 2015-2019  Michael Griffin
 * Copyright (C) 2001-2006  Anders Hedstrom
 * This program is made available under the terms of the GNU GPL.
 */

#ifdef _WIN32
//#pragma warning(disable:4786)
#endif

#include <sqlite3.h>

#include <iostream>
#include <string>
#include <map>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <exception>

#include "Database.h"
#include "Query.h"
namespace SQLW
{
Query::Query(Database& dbin)
    : m_db(dbin)
    , odb(dbin.addDatabasePool())
    , res(nullptr)
    , row(false)
    , rowcount(0)
    , m_tmpstr("")
    , m_last_query("")
    , cache_rc(0)
    , cache_rc_valid(false)
    , m_row_count(0)
    , m_num_cols(0)
{
}

Query::Query(Database& dbin,const std::string& sql)
    : m_db(dbin)
    , odb(dbin.addDatabasePool())
    , res(nullptr)
    , row(false)
    , rowcount(0)
    , m_tmpstr("")
    , m_last_query("")
    , cache_rc(0)
    , cache_rc_valid(false)
    , m_row_count(0)
    , m_num_cols(0)
{
    execute(sql);
}


Query::~Query()
{
    if(res)
    {
        //GetDatabase().error(*this, "sqlite3_finalize in destructor");
        sqlite3_finalize(res);
    }

    res = nullptr;
    row = false;
    cache_rc_valid = false;

    if(odb)
    {
        m_db.freeDatabasePool(odb);
    }

    std::map<std::string, int>().swap(m_nmap);

    /*
    if(odb && res)
    {
        sqlite3_finalize(res);
        res = nullptr;
        row = false;
        cache_rc_valid = false;
    }
    // clear column names
    std::map<std::string, int>().swap(m_nmap);*/

}

Database& Query::getDatabase() const
{
    return m_db;
}

/*
The sqlite3_finalize() routine deallocates a prepared SQL statement.
All prepared statements must be finalized before the database can be closed.
*/
bool Query::execute(const std::string& sql)
{
    // query, no result
    m_last_query = sql;

    if(odb && res)
    {
        getDatabase().databaseError(*this, "execute: query busy");
    }

    if(odb && !res)
    {
        const char *s = nullptr;
        int rc = sqlite3_prepare_v2(odb->db, sql.c_str(), sql.size(), &res, &s);

        if(rc != SQLITE_OK)
        {
            getDatabase().databaseError(*this, "execute: prepare query failed");
            return false;
        }

        if(!res)
        {
            getDatabase().databaseError(*this, "execute: query failed");
            return false;
        }

        rc = sqlite3_step(res); // execute
        sqlite3_finalize(res);  // deallocate statement
        res = nullptr;

        switch(rc)
        {
            case SQLITE_BUSY:
                getDatabase().databaseError(*this, "execute: database busy");
                return false;

            case SQLITE_DONE:
            case SQLITE_ROW:
                return true;

            case SQLITE_ERROR:
                getDatabase().databaseError(*this, sqlite3_errmsg(odb->db));
                return false;

            case SQLITE_MISUSE:
                getDatabase().databaseError(*this, "execute: database misuse");
                return false;
        }

        getDatabase().databaseError(*this, "execute: unknown result code");
    }

    return false;
}

// methods using db specific api calls
sqlite3_stmt *Query::getResult(const std::string& sql)
{
    // query, result
    m_last_query = sql;

    if(odb && res)
    {
        getDatabase().databaseError(*this, "get_result: query busy");
    }

    if(odb && !res)
    {
        const char *s = nullptr;
        int rc = sqlite3_prepare_v2(odb->db, sql.c_str(), sql.size(), &res, &s);

        if(rc != SQLITE_OK)
        {
            getDatabase().databaseError(*this, "get_result: prepare query failed");
            return nullptr;
        }

        if(!res)
        {
            getDatabase().databaseError(*this, "get_result: query failed");
            return nullptr;
        }

        // get column names from result
        {
            int i = 0;

            // Don't require Free Result, do this automatically before running new queries!
            std::map<std::string, int>().swap(m_nmap);

            do
            {
                const char *p = sqlite3_column_name(res, i);

                if(!p)
                    break;

                //m_nmap[p] = ++i;
                m_nmap.insert(std::make_pair(p, ++i));
            }
            while(true);

            m_num_cols = i;
        }
        cache_rc = sqlite3_step(res);
        cache_rc_valid = true;
        m_row_count = (cache_rc == SQLITE_ROW) ? 1 : 0;
    }

    return res;
}

void Query::freeResult()
{
    if(odb && res)
    {
        sqlite3_finalize(res);
    }

    // Always reset anyways!
    res = nullptr;
    row = false;
    cache_rc_valid = false;

    std::map<std::string,int>().swap(m_nmap);
}

bool Query::fetchRow()
{
    rowcount = 0;
    row = false;

    if(odb && res)
    {
        int rc = cache_rc_valid ? cache_rc : sqlite3_step(res); // execute
        cache_rc_valid = false;

        switch(rc)
        {
            case SQLITE_BUSY:
                getDatabase().databaseError(*this, "execute: database busy");
                return false;

            case SQLITE_DONE:
                return false;

            case SQLITE_ROW:
                row = true;
                return true;

            case SQLITE_ERROR:
                getDatabase().databaseError(*this, sqlite3_errmsg(odb -> db));
                return false;

            case SQLITE_MISUSE:
                getDatabase().databaseError(*this, "execute: database misuse");
                return false;
        }

        getDatabase().databaseError(*this, "execute: unknown result code");
    }

    return false;
}

/*
 * Get the last Inserted Row ID for Primary Key Tables.
 */
sqlite_int64 Query::getInsertId()
{
    if(odb)
    {
        return sqlite3_last_insert_rowid(odb->db);
    }
    else
    {
        return 0;
    }
}

long Query::getNumRows()
{
    return odb && res ? m_row_count : 0;
}

int Query::getNumCols()
{
    return m_num_cols;
}

bool Query::isNull(int x)
{
    if(odb && res && row)
    {
        if(sqlite3_column_type(res, x) == SQLITE_NULL)
        {
            return true;
        }
    }

    return false;
}

const char *Query::getString(const std::string& x)
{
    int index = m_nmap[x] - 1;

    if(index >= 0)
    {
        return getString(index);
    }

    queryError("Column name lookup failure: " + x);
    return "";
}

const char *Query::getString(int x)
{
    if(odb && res && row && x < sqlite3_column_count(res))
    {
        const unsigned char *tmp = sqlite3_column_text(res, x);
        return tmp ? (const char *)tmp : "";
    }

    return "";
}

const char *Query::getString()
{
    return getString(rowcount++);
}

double Query::getNumber(const std::string& x)
{
    int index = m_nmap[x] - 1;

    if(index >= 0)
    {
        return getNumber(index);
    }

    queryError("Column name lookup failure: " + x);
    return 0;
}

double Query::getNumber(int x)
{
    if(odb && res && row)
    {
        return sqlite3_column_double(res, x);
    }

    return 0;
}

long Query::getValue(const std::string& x)
{
    int index = m_nmap[x] - 1;

    if(index >= 0)
    {
        return getValue(index);
    }

    queryError("Column name lookup failure: " + x);
    return 0;
}

long Query::getValue(int x)
{
    if(odb && res && row)
    {
        return sqlite3_column_int(res, x);
    }

    return 0;
}

double Query::getNumber()
{
    return getNumber(rowcount++);
}


long Query::getValue()
{
    return getValue(rowcount++);
}


unsigned long Query::getUnsignedValue(const std::string& x)
{
    int index = m_nmap[x] - 1;

    if(index >= 0)
    {
        return getUnsignedValue(index);
    }

    queryError("Column name lookup failure: " + x);
    return 0;
}

unsigned long Query::getUnsignedValue(int x)
{
    unsigned long l = 0;

    if(odb && res && row)
    {
        l = sqlite3_column_int(res, x);
    }

    return l;
}

unsigned long Query::getUnsignedValue()
{
    return getUnsignedValue(rowcount++);
}


int64_t Query::getBigInt(const std::string& x)
{
    int index = m_nmap[x] - 1;

    if(index >= 0)
    {
        return getBigInt(index);
    }

    queryError("Column name lookup failure: " + x);
    return 0;
}

int64_t Query::getBigInt(int x)
{
    if(odb && res && row)
    {
        return sqlite3_column_int64(res, x);
    }

    return 0;
}

int64_t Query::getBigInt()
{
    return getBigInt(rowcount++);
}

uint64_t Query::getUnsignedBitInt(const std::string& x)
{
    int index = m_nmap[x] - 1;

    if(index >= 0)
    {
        return getUnsignedBitInt(index);
    }

    queryError("Column name lookup failure: " + x);
    return 0;
}

uint64_t Query::getUnsignedBitInt(int x)
{
    uint64_t l = 0;

    if(odb && res && row)
    {
        l = sqlite3_column_int64(res, x);
    }

    return l;
}

uint64_t Query::getUnsignedBitInt()
{
    return getUnsignedBitInt(rowcount++);
}

double Query::exeGetResultDouble(const std::string& sql)
{
    double l = 0;

    if(getResult(sql))
    {
        if(fetchRow())
        {
            l = getNumber();
        }

        freeResult();
    }

    return l;
}

long Query::exeGetResultLong(const std::string& sql)
{
    long l = 0;

    if(getResult(sql))
    {
        if(fetchRow())
        {
            l = getValue();
        }

        freeResult();
    }

    return l;
}

const char *Query::exeGetCharString(const std::string& sql)
{
    m_tmpstr = "";

    if(getResult(sql))
    {
        if(fetchRow())
        {
            m_tmpstr = getString();
        }

        freeResult();
    }

    return m_tmpstr.c_str(); // %! changed from 1.0 which didn't return nullptr on failed query
}

const std::string& Query::getLastQuery()
{
    return m_last_query;
}

std::string Query::getError()
{
    if(odb)
    {
        return sqlite3_errmsg(odb->db);
    }

    return "";
}

int Query::getErrorCode()
{
    if(odb)
    {
        return sqlite3_errcode(odb->db);
    }

    return 0;
}

bool Query::isConnected()
{
    return odb ? true : false;
}

void Query::queryError(const std::string& msg)
{
    getDatabase().databaseError(*this, msg);
}

/**
 * Create a new Transaction
 */
bool Query::executeTransaction(const std::vector<std::string> &statements)
{
    bool result = false;
    char *errorMsg = 0;

    if(!odb)
    {
        return result;
    }

    //Start a transaction with: sqlite3_exec(db, "BEGIN", 0, 0, 0);
    int rc = sqlite3_exec(odb->db, "BEGIN;", 0, 0, 0);

    if(rc != SQLITE_OK)
    {
        queryError("BEGIN Transaction Failed.");
        // queryError("BEGIN Transaction Failed. ");
        return result;
    }

    for(std::string::size_type i = 0; i < statements.size(); i++)
    {
        // Execute Statement
        rc = sqlite3_exec(odb->db, statements[i].c_str(), nullptr, 0, &errorMsg);

        if(rc != SQLITE_OK)
        {
            queryError("Statement in Transaction Failed");
            queryError(errorMsg);

            sqlite3_free(errorMsg);

            // rollback all update/insert to sqlite
            rc = sqlite3_exec(odb->db, "ROLLBACK;", 0, 0, 0);

            if(rc != SQLITE_OK)
            {
                queryError("Unable to Rollback Transaction.");
            }


            return result;
        }
    }

    //Commit a transaction with: sqlite3_exec(db, "COMMIT", 0, 0, 0);
    rc = sqlite3_exec(odb->db, "COMMIT;", 0, 0, 0);

    if(rc != SQLITE_OK)
    {
        queryError("COMMIT Transaction Failed.");
        return result;
    }
    else
    {
        result = true;
    }

    return result;
}

} // namespace SQLW
