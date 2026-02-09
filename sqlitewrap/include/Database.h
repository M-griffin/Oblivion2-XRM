#ifndef DATABASE_H_SQLITE
#define DATABASE_H_SQLITE

/**
 * Refactoring / author: 2026-02-06 / mrmisticismo@hotmail.com
 * Rewritten   / author: 2016-02-19 / mrmisticismo@hotmail.com
 * Published   / author: 2005-08-12 / grymse@alhem.net
 * Copyright (C) 2015-2026  Michael Griffin
 * Copyright (C) 2001-2006  Anders Hedstrom
 * This program is made available under the terms of the GNU GPL.
 */

#include <sqlite3.h>
#include <map>
#include <string>
#include <memory>
#include <future>
#include <chrono>
#include <vector>
#include <queue>

#include "../include/IError.h"

namespace SQLW {
    class SQLiteConnection;
    class PreparedStatement;
    class Transaction;

    inline std::string filenameFromPath(const std::string &path) {
        auto pos = path.find_last_of("/\\");
        if (pos != std::string::npos) return path.substr(pos + 1);
        return path;
    }

    /**
     * SQL Lite Connection Pool
     */
    class SQLiteConnectionPool {
    public:
        explicit SQLiteConnectionPool(
            const std::string& dbPath,
            IError* err = nullptr,

            // Default to one, to reuse the same connection
            // Otherwise Create, and Inserts need to be chained in same transaction
            // You can extend and expand as needed.
            size_t poolSize = 1
        );

        ~SQLiteConnectionPool() = default;

        std::shared_ptr<SQLiteConnection> acquire();
        void release(SQLiteConnection* conn);

    private:
        std::string m_dbPath;
        IError* m_errhandler;

        std::mutex m_mutex;
        std::condition_variable m_cv;

        std::vector<std::unique_ptr<SQLiteConnection>> m_connections;
        std::queue<SQLiteConnection*> m_available;
    };

    /**
     * SQL Lite Database
     */
    class Database {
    public:
        //explicit Database(const std::string &dbPath, IError *err = nullptr);
        explicit Database(const std::string& dbPath,
                          IError* err = nullptr,
                          size_t poolSize = 4);

        ~Database();

        Database(const Database&) = delete;
        Database& operator=(const Database&) = delete;

        [[nodiscard]]
        std::string getName() const;

        [[nodiscard]]
        bool isConnected();

        // Get thread-local connection (RAII)
        sqlite3 *connection();

        // Prepare statement
        std::unique_ptr<PreparedStatement> prepare(const std::string &sql);

        // Synchronous query helper
        void execute(const std::string &sql);

        // Asynchronous query helper
        std::future<void> executeAsync(const std::string &sql);

        std::future<void> executeTransactionAsync(const std::vector<std::string> &statements);

        // Transactions
        std::unique_ptr<Transaction> beginTransaction();

        void traceConnectionOpened(const std::string &context);

        void databaseError(const std::string &msg);

        std::string getError();

        int getErrorCode();

    private:
        friend class Transaction; // for m_pool access
        friend class Query;       // for m_errhandler access
        //SQLiteConnection &getThreadLocalConnection();
        SQLiteConnectionPool m_pool;
        std::shared_ptr<SQLiteConnection> m_activeConn;

        std::string m_dbPath;
        IError *m_errhandler{nullptr};
    };



    /**
     * SQL Lite Connection
     */
    class SQLiteConnection {
    public:
        explicit SQLiteConnection(const std::string &path);
        ~SQLiteConnection();

        [[nodiscard]] sqlite3 *get() const noexcept { return m_db; }

        void applyDefaults();

        [[nodiscard]] std::chrono::steady_clock::time_point openedAt() const { return m_opened; }

    private:
        std::string m_path;
        sqlite3 *m_db{nullptr};
        std::chrono::steady_clock::time_point m_opened;
    };

    /**
     * Transactions
     */
    class Transaction {
    public:
        explicit Transaction(Database &db);
        ~Transaction();

        void commit();

        void rollback();

    private:
        Database &m_db;
        std::shared_ptr<SQLiteConnection> m_conn;
        bool m_committed{false};
    };

    /**
     * Prepared Statement
     */
    class PreparedStatement {
    public:
        PreparedStatement(sqlite3* db, const std::string& sql);
        ~PreparedStatement();

        PreparedStatement(const PreparedStatement&) = delete;
        PreparedStatement& operator=(const PreparedStatement&) = delete;
        PreparedStatement(PreparedStatement&&) noexcept = default;
        PreparedStatement& operator=(PreparedStatement&&) noexcept = default;

        // Execute a step
        [[nodiscard]] int step();

        // Get field values by index
        [[nodiscard]] const char* getString(int index) const;
        [[nodiscard]] double getNumber(int index) const;
        [[nodiscard]] long getValue(int index) const;
        [[nodiscard]] int64_t getBigInt(int index) const;

        // Column helpers
        [[nodiscard]] int getColumnIndex(const std::string& name) const;
        [[nodiscard]] bool isNull(int index) const;
        [[nodiscard]] int getNumColumns() const;
        [[nodiscard]] std::string getColumnName(int index) const;
        [[nodiscard]] int64_t getInsertId(sqlite3* db) const;

        [[nodiscard]] sqlite3_stmt* stmt() const;

    private:
        sqlite3_stmt* m_stmt;

        // column name -> index mapping
        std::map<std::string,int> m_colMap;
    };
} // namespace SQLW


#endif
