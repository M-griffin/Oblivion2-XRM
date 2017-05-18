#ifndef DB_STARTUP_HPP
#define DB_STARTUP_HPP

#include <boost/smart_ptr/shared_ptr.hpp>

/**
 * @class DbStartup
 * @author Micahel Griffin
 * @date 18/05/2017
 * @file db_startup.hpp
 * @brief Handle DB Table Creation on Startup.
 */
class DbStartup
{
public:
    DbStartup();
    ~DbStartup();

};

typedef boost::shared_ptr<DbStartup> db_startup_ptr;

#endif // DB_STARTUP_HPP
