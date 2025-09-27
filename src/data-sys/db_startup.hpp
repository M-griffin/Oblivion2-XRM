#ifndef DB_STARTUP_HPP
#define DB_STARTUP_HPP


#include <memory>

/**
 * @class DbStartup
 * @author Michael Griffin
 * @date 18/05/2017
 * @file db_startup.hpp
 * @brief Handle DB Table Creation on Startup.
 */
class DbStartup
{
public:

    explicit DbStartup();
    ~DbStartup();

    /**
     * @brief Setup for Database and Tables
     */
    bool initDatabaseTables();

};

typedef std::shared_ptr<DbStartup> db_startup_ptr;

#endif // DB_STARTUP_HPP
