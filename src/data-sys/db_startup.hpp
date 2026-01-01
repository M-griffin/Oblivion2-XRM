#ifndef DB_STARTUP_HPP
#define DB_STARTUP_HPP

/**
 * @class DBStartUp
 * @author Michael Griffin
 * @date 18/05/2017
 * @file db_startup.hpp
 * @brief Handle DB Table Creation on Startup.
 */
class DBStartUp {
public:
    explicit DBStartUp() = default;

    ~DBStartUp() = default;

    /**
     * @brief Setup for Database and Tables
     */
    bool initDatabaseTables();
};

#endif
