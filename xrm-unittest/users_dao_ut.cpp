
/*
 * Incomplete stub for Integration Testing of users_dao_ class.
 * Initial Testing done in Main()

#include "data/users.hpp"
#include "data/users_dao.hpp"

#include "libSqliteWrapped.h"




    SQLW::StderrLog log;
    SQLW::Database db("test3.db", &log);

    // Handle to the Users Database Access Object
    users_dao_ptr dao(new UsersDao(db));

    // Check if it Exists.
    bool exist = dao->isTableExists();
    if (exist)
        std::cout << "table exist" << std::endl;
    else
        std::cout << "table doesn't exist" << std::endl;

    // Create Table using Transaction
    exist = dao->createTable();
    if (exist)
        std::cout << "table created" << std::endl;
    else
        std::cout << "table not created" << std::endl;

    // Check if Exists.
    exist = dao->isTableExists();
    if (exist)
        std::cout << "table exist" << std::endl;
    else
        std::cout << "table doesn't exist" << std::endl;

    // Drop Table using Transaction
    exist = dao->dropTable();
    if (exist)
        std::cout << "table was dropped" << std::endl;
    else
        std::cout << "table was not dropped" << std::endl;


    // Check if Exists.
    exist = dao->isTableExists();
    if (exist)
        std::cout << "table exist" << std::endl;
    else
        std::cout << "table doesn't exist" << std::endl;


    // Create Temp User Record.
    user_ptr usr(new Users());

    // Test update query string creation.
    dao->updateUserRecord(usr);
    dao->insertUserRecord(usr);

 */
