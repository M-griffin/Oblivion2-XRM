/**
 * Refactoring / author: 2026-02-06 / mrmisticismo@hotmail.com
 * Rewritten   / author: 2016-02-19 / mrmisticismo@hotmail.com
 * Published   / author: 2005-08-12 / grymse@alhem.net
 * Copyright (C) 2015-2026  Michael Griffin
 * Copyright (C) 2001-2006  Anders Hedstrom
 * This program is made available under the terms of the GNU GPL.
 */

#include "../include/Query.h"

#include <iostream>
#include <sqlite3.h>
#include <string>
#include <memory>

namespace SQLW {
    Query::Query(Database &db, std::shared_ptr<SQLiteConnection> txnConn)
        : m_db(db),
          m_txnConn(std::move(txnConn)),
          m_rowCount(0),
          m_numCols(0),
          m_rowValid(false),
          m_cache_rc(0),
          m_cache_rc_valid(false) {
    }

    Query::Query(Database &db, const std::string &sql)
        : Query(db) {
        getResult(sql);
    }

    Query::~Query() {
        m_stmt.reset();
        m_colMap.clear();
    }

    bool Query::isConnected() {
        return m_db.connection() != nullptr;
    }

    std::unique_ptr<PreparedStatement> Query::prepare(const std::string &sql) {
        sqlite3 *db = m_txnConn
                          ? m_txnConn->get()
                          : m_db.connection();

        return std::make_unique<PreparedStatement>(db, sql);
    }

    bool Query::execute(const std::string &sql) {
        m_lastQuery = sql;

        try {
            m_stmt = prepare(sql);
            int rc = m_stmt->step();
            if (rc != SQLITE_DONE) {
                queryError("execute failed");
                return false;
            }
            return true;
        } catch (...) {
            queryError("execute threw exception");
            return false;
        }
    }

    bool Query::getResult(const std::string &sql) {
        m_lastQuery = sql;
        m_rowCount = 0;
        m_rowValid = false;

        m_stmt = prepare(sql);
        if (!m_stmt) {
            queryError("step called on null statement");
            return false;
        }

        // Cache the first row so we can loop easily when valid.
        m_cache_rc = m_stmt->step();
        m_cache_rc_valid = true;

        // Build column map lazily if not done yet
        if (m_colMap.empty()) {
            int nCols = m_stmt->getNumColumns();
            for (int i = 0; i < nCols; ++i) {
                m_colMap[m_stmt->getColumnName(i)] = i;
            }
        }

        if (m_cache_rc == SQLITE_ROW) {
            m_rowValid = true;
            m_rowCount = 1;
            return true;
        }

        if (m_cache_rc == SQLITE_DONE) {
            m_rowValid = false;
            m_rowCount = 0;
            return true;
        }

        queryError("getResult failed");
        return false;
    }

    bool Query::fetchRow() {
        if (!m_stmt) {
            return false;
        }

        int rc = m_cache_rc_valid ? m_cache_rc : m_stmt->step();
        m_cache_rc_valid = false;

        if (rc == SQLITE_ROW) {
            m_rowValid = true;
            ++m_rowCount;
            return true;
        }

        if (rc == SQLITE_DONE) {
            return false;
        }

        queryError("fetchRow failed");
        m_rowValid = false;
        return false;
    }

    void Query::buildColumnMap() {
        if (!m_stmt) return;
        m_colMap.clear();
        m_numCols = m_stmt->getNumColumns();
        for (int i = 0; i < m_numCols; ++i) {
            m_colMap[m_stmt->getColumnName(i)] = i;
        }
    }

    const char *Query::getString(int index) {
        return m_stmt ? m_stmt->getString(index) : "";
    }

    const char *Query::getString(const std::string &name) {
        auto it = m_colMap.find(name);
        return it != m_colMap.end() ? getString(it->second) : "";
    }

    double Query::getNumber(int index) {
        return m_stmt ? m_stmt->getNumber(index) : 0.0;
    }

    double Query::getNumber(const std::string &name) {
        auto it = m_colMap.find(name);
        return it != m_colMap.end() ? getNumber(it->second) : 0.0;
    }

    long Query::getValue(int index) {
        return m_stmt ? m_stmt->getValue(index) : 0;
    }

    long Query::getValue(const std::string &name) {
        auto it = m_colMap.find(name);
        return it != m_colMap.end() ? getValue(it->second) : 0;
    }

    sqlite_int64 Query::getInsertId() const {

        sqlite3* db = m_txnConn ? m_txnConn->get() : m_stmt
            ? sqlite3_db_handle(m_stmt->stmt())
            : nullptr;

        return sqlite3_last_insert_rowid(db);
    }

    int Query::getNumCols() const {
        return m_numCols;
    }

    long Query::getNumRows() const {
        return m_rowCount;
    }

    bool Query::isNull(int index) {
        return m_stmt ? m_stmt->isNull(index) : true;
    }

    bool Query::isNull(const std::string &name) {
        if (!m_stmt) return true;
        int idx = m_stmt->getColumnIndex(name);
        return idx < 0 || m_stmt->isNull(idx);
    }

    const std::string &Query::getLastQuery() const {
        return m_lastQuery;
    }

    std::string Query::getError() const {
        return m_db.getError();
    }

    int Query::getErrorCode() const {
        return m_db.getErrorCode();
    }

    void Query::queryError(const std::string &msg) {
        if (m_db.m_errhandler) {
            m_db.m_errhandler->databaseError(m_db, *this, msg);
        }
    }

    bool Query::executeTransaction(const std::vector<std::string> &statements) {
        if (statements.empty()) return true;

        // Start transaction
        //std::unique_ptr<Transaction> transaction = m_db.beginTransaction();
        auto transaction = m_db.beginTransaction();
        m_txnConn = transaction->getConnection(); // you must expose this

        if (!transaction) {
            std::cout << "executeTransaction - Failed to begin transaction" << std::endl;
            queryError("Failed to begin transaction");
            return false;
        }

        try {
            for (size_t i = 0; i < statements.size(); ++i) {
                const auto &sql = statements[i];

                // Prepare and execute statement in a local scope to ensure destruction
                {
                    std::unique_ptr<PreparedStatement> stmt = prepare(sql);
                    if (!stmt) {
                        queryError("Failed to prepare statement: " + sql +
                                   " | SQLite error: " + m_db.getError());
                        std::cout << "executeTransaction - Failed to prepare statement, msg: " << m_db.getError() <<
                                std::endl;
                        // transaction->rollback(); in Destructor.
                        return false;
                    }

                    int rc = stmt->step();
                    if (rc != SQLITE_DONE) {
                        queryError("Failed to execute statement: " + sql +
                                   " | rc=" + std::to_string(rc) +
                                   " | SQLite error: " + m_db.getError());
                        std::cout << "executeTransaction - Failed to execute statement:, msg: " << m_db.getError() <<
                                std::endl;
                        // transaction->rollback(); in Destructor.
                        return false;
                    }

                    // stmt goes out of scope here, automatically finalized
                }
            }

            // Commit transaction after all statements succeed
            transaction->commit();
            return true;
        } catch (const std::exception &e) {
            queryError(std::string("Transaction exception: ") + e.what());
            std::cout << "executeTransaction - Transaction exception:, msg: " << e.what() << std::endl;
            // transaction->rollback(); in Destructor.
            return false;
        } catch (...) {
            queryError("Unknown exception in executeTransaction");
            std::cout << "executeTransaction - Unknown exception:, msg: " << std::endl;
            // transaction->rollback(); in Destructor.
            return false;
        }
    }


    /* ================= FIELD VALUE EXTRACTION ================= */

    void Query::getFieldValue(uint32_t &v, int index) {
        v = static_cast<uint32_t>(sqlite3_column_int(m_stmt->stmt(), index));
    }

    void Query::getFieldValue(int &v, int index) {
        v = sqlite3_column_int(m_stmt->stmt(), index);
    }

    void Query::getFieldValue(long &v, int index) {
        v = static_cast<long>(sqlite3_column_int64(m_stmt->stmt(), index));
    }

    void Query::getFieldValue(long long &v, int index) {
        v = sqlite3_column_int64(m_stmt->stmt(), index);
    }

    void Query::getFieldValue(double &v, int index) {
        v = sqlite3_column_double(m_stmt->stmt(), index);
    }

    void Query::getFieldValue(float &v, int index) {
        v = static_cast<float>(sqlite3_column_double(m_stmt->stmt(), index));
    }

    void Query::getFieldValue(long double &v, int index) {
        v = static_cast<long double>(sqlite3_column_double(m_stmt->stmt(), index));
    }

    void Query::getFieldValue(std::string &v, int index) {
        const char *txt =
                reinterpret_cast<const char *>(sqlite3_column_text(m_stmt->stmt(), index));
        v = txt ? txt : "";
    }

    void Query::getFieldValue(char &v, int index) {
        const char *txt =
                reinterpret_cast<const char *>(sqlite3_column_text(m_stmt->stmt(), index));
        v = (txt && *txt) ? txt[0] : '\0';
    }

    const char *Query::getFieldValue(char *, int index) {
        return reinterpret_cast<const char *>(
            sqlite3_column_text(m_stmt->stmt(), index));
    }

    void Query::getFieldValue(bool &v, int index) {
        v = sqlite3_column_int(m_stmt->stmt(), index) != 0;
    }

    void Query::getFieldValue(std::vector<std::uint8_t> &out, int index) {
        const void *data = sqlite3_column_blob(m_stmt->stmt(), index);
        int size = sqlite3_column_bytes(m_stmt->stmt(), index);

        out.clear();
        if (data && size > 0) {
            const std::uint8_t *ptr = static_cast<const std::uint8_t *>(data);
            out.assign(ptr, ptr + size);
        }
    }

    /* ================= FIELD TYPE TRANSLATION ================= */

    std::string Query::getFieldType(float &) { return "%f"; }
    std::string Query::getFieldType(double &) { return "%f"; }
    std::string Query::getFieldType(long long &) { return "%llu"; }
    std::string Query::getFieldType(char &) { return "%Q"; }
    std::string Query::getFieldType(bool &) { return "%d"; }
    std::string Query::getFieldType(char *) { return "%Q"; }
    std::string Query::getFieldType(std::string &) { return "%Q"; }
    std::string Query::getFieldType(long &) { return "%ld"; }
    std::string Query::getFieldType(int &) { return "%d"; }
    std::string Query::getFieldType(uint32_t &) { return "%d"; }
} // namespace SQLW
