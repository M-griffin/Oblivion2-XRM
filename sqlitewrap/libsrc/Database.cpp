/**
 * Refactoring / author: 2026-02-06 / mrmisticismo@hotmail.com
 * Rewritten   / author: 2016-02-19 / mrmisticismo@hotmail.com
 * Published   / author: 2005-08-12 / grymse@alhem.net
 * Copyright (C) 2015-2026  Michael Griffin
 * Copyright (C) 2001-2006  Anders Hedstrom
 * This program is made available under the terms of the GNU GPL.
 */

#include "../include/Database.h"

#include <thread>
#include <iostream>
#include <stdexcept>
#include <sstream>
#include <vector>
#include <queue>

namespace SQLW {
    /**
     * SQL Lite Connection Pool (Back to Legacy)
     */
    SQLiteConnectionPool::SQLiteConnectionPool(const std::string &dbPath, IError *err, size_t poolSize)
        : m_dbPath(dbPath)
          , m_errhandler(err) {
        for (size_t i = 0; i < poolSize; ++i) {
            auto conn = std::make_unique<SQLiteConnection>(m_dbPath);
            conn->applyDefaults();
            m_available.push(conn.get());
            m_connections.push_back(std::move(conn));
        }
    }

    std::shared_ptr<SQLiteConnection> SQLiteConnectionPool::acquire() {
        std::unique_lock<std::mutex> lock(m_mutex);

        m_cv.wait(lock, [&] {
            return !m_available.empty();
        });

        SQLiteConnection *raw = m_available.front();
        m_available.pop();

        return std::shared_ptr<SQLiteConnection>(
            raw,
            [this](SQLiteConnection *c) {
                this->release(c);
            }
        );
    }

    void SQLiteConnectionPool::release(SQLiteConnection *conn) { {
            std::lock_guard<std::mutex> lock(m_mutex);
            m_available.push(conn);
        }
        m_cv.notify_one();
    }


    /**
     * Core Database
     */
    Database::Database(const std::string &dbPath, IError *err, size_t poolSize)
        : m_pool(dbPath, err, poolSize)
        , m_dbPath(dbPath)
        , m_errhandler(err) {
    }

    Database::~Database() = default;

    std::string Database::getName() const {
        return filenameFromPath(m_dbPath);
    }

    bool Database::isConnected() {
        try {
            auto conn = m_pool.acquire();
            return conn && conn->get() != nullptr;
        } catch (...) {
            return false;
        }
    }

    sqlite3 *Database::connection() {
        if (!m_activeConn) {
            m_activeConn = m_pool.acquire();
        }

        return m_activeConn->get();
    }

    /*bool Database::isConnected() {
        return getThreadLocalConnection().get() != nullptr;
    }*/

    /*
    sqlite3 *Database::connection() {
        return getThreadLocalConnection().get();
    }*/

    /*
    SQLiteConnection &Database::getThreadLocalConnection() {
        thread_local std::unique_ptr<SQLiteConnection> conn;
        if (!conn) {
            conn = std::make_unique<SQLiteConnection>(m_dbPath);
            conn->applyDefaults();
            traceConnectionOpened("thread-local connection opened");
        }
        return *conn;
    }*/

    std::unique_ptr<PreparedStatement> Database::prepare(const std::string &sql) {
        return std::make_unique<PreparedStatement>(connection(), sql);
    }

    void Database::execute(const std::string &sql) {
        char *err = nullptr;
        if (sqlite3_exec(connection(), sql.c_str(), nullptr, nullptr, &err) != SQLITE_OK) {
            std::string msg = err ? err : "Unknown SQLite error";
            std::cout << "execute ExceptionMsg=" << msg << " sql=" << sql << std::endl;
            sqlite3_free(err);
            databaseError(msg);
        }
    }

    std::future<void> Database::executeAsync(const std::string &sql) {
        return std::async(std::launch::async, [dbPath = m_dbPath, err = m_errhandler, sql]() {
            Database db(dbPath, err);
            db.execute(sql);
        });
    }

    std::future<void> Database::executeTransactionAsync(const std::vector<std::string> &statements) {
        return std::async(std::launch::async, [this, statements]() {
            auto txn = this->beginTransaction();
            if (!txn) {
                throw std::runtime_error("Failed to begin transaction");
            }

            try {
                for (const auto &sql: statements) {
                    this->execute(sql); // will throw if execution fails
                }
                txn->commit();
            } catch (...) {
                // Transaction RAII will roll back if not committed
                throw; // rethrow for the future
            }
        });
    }

    std::unique_ptr<Transaction> Database::beginTransaction() {
        return std::make_unique<Transaction>(*this);
    }

    void Database::traceConnectionOpened(const std::string &context) {
        if (m_errhandler) {
            m_errhandler->databaseError(*this,
                                        context + " (thread=" +
                                        std::to_string(std::hash<std::thread::id>{}(std::this_thread::get_id())) + ")");
        }
    }

    void Database::databaseError(const std::string &msg) {
        if (m_errhandler)
            m_errhandler->databaseError(*this, msg);
    }

    std::string Database::getError() {
        return connection() ? sqlite3_errmsg(connection()) : "";
    }

    int Database::getErrorCode() {
        return connection() ? sqlite3_errcode(connection()) : 0;
    }

    std::string Database::getDatabasePath() const {
        return m_dbPath;
    }

    /**
     * Session Database "InMemory" Wrapper
     */
    SessionDatabase::SessionDatabase(Database& coreDb, IError* err)
        : m_sessionDb(":memory:", err)
        , m_coreDb(coreDb)
        , m_corePath(coreDb.getDatabasePath())
    {
    }

    SessionDatabase::~SessionDatabase() {
        // safe even if not attached
        detachCore();
    }

    Database& SessionDatabase::db() {
        return m_sessionDb;
    }

    bool SessionDatabase::attachCore() {
        if (m_attached)
            return true;

        std::ostringstream sql;
        sql << "ATTACH DATABASE '" << m_corePath << "' AS core;";

        try {
            m_sessionDb.execute(sql.str());
            m_attached = true;
            return true;
        } catch (...) {
            return false;
        }
    }

    void SessionDatabase::detachCore() {
        if (!m_attached)
            return;

        try {
            m_sessionDb.execute("DETACH DATABASE core;");
        } catch (...) {
            // ignore — memory DB is dying anyway
        }

        m_attached = false;
    }

    bool SessionDatabase::createSchema(std::string &sql) {
        try {
            m_sessionDb.execute(sql);
            return true;
        } catch (...) {
            return false;
        }
    }

    // WIP Placeholder, load user data from Core to Session Database
    bool SessionDatabase::loadData(std::string &sql) {

        std::ostringstream sqlStream;
        sqlStream << sql;

        try {
            m_sessionDb.execute(sqlStream.str());
            return true;
        } catch (...) {
            return false;
        }
    }

    /**
     * SQL Lite Connection
     */
    SQLiteConnection::SQLiteConnection(const std::string &path)
        : m_path(path), m_opened(std::chrono::steady_clock::now()) {
        if (sqlite3_open(m_path.c_str(), &m_db) != SQLITE_OK) {
            throw std::runtime_error(sqlite3_errmsg(m_db));
        }
    }

    SQLiteConnection::~SQLiteConnection() {
        if (m_db) {
            sqlite3_close_v2(m_db);
        }
    }

    void SQLiteConnection::applyDefaults() {
        if (sqlite3_busy_timeout(m_db, 5000) != SQLITE_OK) {
            throw std::runtime_error("Failed to set busy timeout");
        }

        // Enable WAL mode
        char *err = nullptr;
        if (sqlite3_exec(m_db, "PRAGMA journal_mode=WAL;", nullptr, nullptr, &err) != SQLITE_OK) {
            std::string msg = err ? err : "Failed to set journal_mode";
            sqlite3_free(err);
            throw std::runtime_error(msg);
        }

        // Performance tuning
        if (sqlite3_exec(m_db, "PRAGMA synchronous=NORMAL;", nullptr, nullptr, &err) != SQLITE_OK) {
            std::string msg = err ? err : "Failed to set synchronous";
            sqlite3_free(err);
            throw std::runtime_error(msg);
        }

        if (sqlite3_exec(m_db, "PRAGMA temp_store=MEMORY;", nullptr, nullptr, &err) != SQLITE_OK) {
            std::string msg = err ? err : "Failed to set temp_store";
            sqlite3_free(err);
            throw std::runtime_error(msg);
        }

        if (sqlite3_exec(m_db, "PRAGMA foreign_keys=ON;", nullptr, nullptr, &err) != SQLITE_OK) {
            std::string msg = err ? err : "Failed to set foreign_keys";
            sqlite3_free(err);
            throw std::runtime_error(msg);
        }

        /** Set by Default only prior to table creation, and first time, default is UTF-* so don't need to set it
        if (sqlite3_exec(m_db, "PRAGMA encoding=UTF-8;", nullptr, nullptr, &err) != SQLITE_OK) {
            std::string msg = err ? err : "Failed to set encoding";
            sqlite3_free(err);
            std::cout << "ExceptionMsg=Failed to set encoding" << msg << std::endl;
            throw std::runtime_error(msg);
        }*/

        if (sqlite3_exec(m_db, "PRAGMA default_cache_size=10000;", nullptr, nullptr, &err) != SQLITE_OK) {
            std::string msg = err ? err : "Failed to set default_cache_size";
            sqlite3_free(err);
            throw std::runtime_error(msg);
        }

        if (sqlite3_exec(m_db, "PRAGMA cache_size=10000;", nullptr, nullptr, &err) != SQLITE_OK) {
            std::string msg = err ? err : "Failed to set cache_size";
            sqlite3_free(err);
            throw std::runtime_error(msg);
        }

        if (sqlite3_exec(m_db, "PRAGMA mmap_size = 268435456;", nullptr, nullptr, &err) != SQLITE_OK) {
            std::string msg = err ? err : "Failed to set mmap_size";
            sqlite3_free(err);

            // Non-Fatal Errors, some older SQLite, network filesystems, Windows FAT
            //throw std::runtime_error(msg);
        }
    }

    /**
     * Transaction
     */
    Transaction::Transaction(Database &db) : m_db(db) {
        m_conn = m_db.m_pool.acquire();
        sqlite3_exec(m_conn->get(), "BEGIN;", nullptr, nullptr, nullptr);
    }

    Transaction::~Transaction() {
        if (!m_committed) {
            try {
                sqlite3_exec(m_conn->get(), "ROLLBACK;", nullptr, nullptr, nullptr);
            } catch (...) {
                // swallow
            }
        }
    }

    void Transaction::commit() {
        sqlite3_exec(m_conn->get(), "COMMIT;", nullptr, nullptr, nullptr);
        m_committed = true;
    }

    void Transaction::rollback() {
        sqlite3_exec(m_conn->get(), "ROLLBACK;", nullptr, nullptr, nullptr);
        m_committed = true;
    }

    /**
     * Prepared Statement
     */
    PreparedStatement::PreparedStatement(sqlite3 *db, const std::string &sql)
        : m_stmt(nullptr) {
        if (sqlite3_prepare_v2(db, sql.c_str(), -1, &m_stmt, nullptr) != SQLITE_OK) {
            std::cout << "Exception PreparedStatement - Could not Prepare sql: " << sql << " " << sqlite3_errmsg(db) <<
                    std::endl;
            throw std::runtime_error(sqlite3_errmsg(db));
        }

        // Clear it when Preparing a Statement.
        m_colMap.clear();
    }

    PreparedStatement::~PreparedStatement() {
        if (m_stmt) sqlite3_finalize(m_stmt);
    }


    int PreparedStatement::step() {
        if (!m_stmt) return SQLITE_MISUSE;

        int rc = sqlite3_step(m_stmt);

        // Build column map lazily on first row
        if (rc == SQLITE_ROW && m_colMap.empty()) {
            int nCols = sqlite3_column_count(m_stmt);
            for (int i = 0; i < nCols; ++i) {
                const char *name = sqlite3_column_name(m_stmt, i);
                if (name) m_colMap[name] = i;
            }
        }

        return rc;
    }

    const char *PreparedStatement::getString(int index) const {
        if (!m_stmt || index < 0 || index >= sqlite3_column_count(m_stmt)) return "";
        const unsigned char *txt = sqlite3_column_text(m_stmt, index);
        return txt ? reinterpret_cast<const char *>(txt) : "";
    }

    double PreparedStatement::getNumber(int index) const {
        if (!m_stmt || index < 0 || index >= sqlite3_column_count(m_stmt)) return 0.0;
        return sqlite3_column_double(m_stmt, index);
    }

    long PreparedStatement::getValue(int index) const {
        if (!m_stmt || index < 0 || index >= sqlite3_column_count(m_stmt)) return 0;
        return sqlite3_column_int(m_stmt, index);
    }

    int64_t PreparedStatement::getBigInt(int index) const {
        if (!m_stmt || index < 0 || index >= sqlite3_column_count(m_stmt)) return 0;
        return sqlite3_column_int64(m_stmt, index);
    }

    int PreparedStatement::getColumnIndex(const std::string &name) const {
        auto it = m_colMap.find(name);
        if (it != m_colMap.end()) return it->second;
        return -1;
    }

    bool PreparedStatement::isNull(int index) const {
        if (!m_stmt || index < 0 || index >= sqlite3_column_count(m_stmt)) return true;
        return sqlite3_column_type(m_stmt, index) == SQLITE_NULL;
    }

    int PreparedStatement::getNumColumns() const {
        return m_stmt ? sqlite3_column_count(m_stmt) : 0;
    }

    std::string PreparedStatement::getColumnName(int index) const {
        return m_stmt ? sqlite3_column_name(m_stmt, index) : "";
    }

    int64_t PreparedStatement::getInsertId(sqlite3 *db) const {
        return m_stmt ? sqlite3_last_insert_rowid(db) : 0;
    }

    sqlite3_stmt *PreparedStatement::stmt() const {
        return m_stmt;
    }
} // namespace SQLW
