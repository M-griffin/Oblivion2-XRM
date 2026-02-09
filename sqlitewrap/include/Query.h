#ifndef QUERY_H_SQLITE
#define QUERY_H_SQLITE

/**
 * Refactoring / author: 2026-02-06 / mrmisticismo@hotmail.com
 * Rewritten   / author: 2016-02-19 / mrmisticismo@hotmail.com
 * Published   / author: 2005-08-12 / grymse@alhem.net
 * Copyright (C) 2015-2026  Michael Griffin
 * Copyright (C) 2001-2006  Anders Hedstrom
 * This program is made available under the terms of the GNU GPL.
 */

#include "../include/Database.h"

#include <string>
#include <map>
#include <memory>
#include <vector>
#include <utility>
#include <sqlite3.h>

namespace SQLW {

    class Query {
    public:
        explicit Query(Database &db,
                       std::shared_ptr<SQLiteConnection> txnConn = nullptr);

        Query(Database &db, const std::string &sql);
        ~Query();

        bool isConnected();

        std::unique_ptr<PreparedStatement> prepare(const std::string& sql);

        // Execute a statement (INSERT / UPDATE / DELETE)
        bool execute(const std::string &sql);

        // Prepare + execute SELECT
        bool getResult(const std::string &sql);

        bool fetchRow();

        // Column access (index / name)
        const char *getString(int index);

        const char *getString(const std::string &name);

        double getNumber(int index);

        double getNumber(const std::string &name);

        long getValue(int index);

        long getValue(const std::string &name);

        [[nodiscard]]
        sqlite_int64 getInsertId() const;

        [[nodiscard]]
        int getNumCols() const;

        [[nodiscard]]
        long getNumRows() const;

        bool isNull(int index);

        bool isNull(const std::string& name);

        [[nodiscard]]
        const std::string &getLastQuery() const;

        [[nodiscard]]
        std::string getError() const;

        [[nodiscard]]
        int getErrorCode() const;

        void queryError(const std::string &msg);

        // Transaction support
        bool executeTransaction(const std::vector<std::string> &statements);

    private:
        Database &m_db;
        std::shared_ptr<SQLiteConnection> m_txnConn;
        std::unique_ptr<PreparedStatement> m_stmt;
        std::map<std::string, int> m_colMap;
        long m_rowCount;
        int m_numCols;
        bool m_rowValid;
        std::string m_lastQuery;

        void buildColumnMap();

        // --- Value extraction overloads ---
        void getFieldValue(uint32_t& v, int index);
        void getFieldValue(int& v, int index);
        void getFieldValue(long& v, int index);
        void getFieldValue(long long& v, int index);
        void getFieldValue(double& v, int index);
        void getFieldValue(float& v, int index);
        void getFieldValue(long double& v, int index);
        void getFieldValue(std::string& v, int index);
        void getFieldValue(char& v, int index);
        const char* getFieldValue(char*, int index);
        void getFieldValue(bool& v, int index);

        // BLOB
        void getFieldValue(std::vector<std::uint8_t>&, int);

        // --- Type translation overloads ---
        std::string getFieldType(float&);
        std::string getFieldType(double&);
        std::string getFieldType(long long&);
        std::string getFieldType(char&);
        std::string getFieldType(bool&);
        std::string getFieldType(char*);
        std::string getFieldType(std::string&);
        std::string getFieldType(long&);
        std::string getFieldType(int&);
        std::string getFieldType(uint32_t&);

    public:

        template<typename TT, typename T>
        T getFieldByName(const TT& fieldName, T& out) {
            if (!m_stmt || !m_rowValid || m_colMap.empty())
                return out;

            std::string_view name(fieldName);
            auto it = m_colMap.find(std::string(name));
            if (it == m_colMap.end())
                return out;

            int index = it->second;
            if (sqlite3_column_type(m_stmt->stmt(), index) != SQLITE_NULL)
                getFieldValue(out, index);

            return out;
        }

        template<typename TT, typename T>
        std::pair<std::string, std::string>
        translateFieldName(const TT& fieldName, T& value) {
            return { std::string(fieldName), getFieldType(value) };
        }
    };
} // namespace SQLW

#endif
