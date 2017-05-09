/**
 *   Oblivion/2 XRM - UNIT TESTS
 *   Copyright (C) 2015 by Michael Griffin
 */

#ifdef _WIN32
#include <UnitTest++.h>
#else
#include <unittest++/UnitTest++.h>
#endif

#include <boost/smart_ptr/shared_ptr.hpp>

#include "data-sys/conference_dao.hpp"
#include "model-sys/conference.hpp"

#include "libSqliteWrapped.h"


// C Standard
#include <cstdio>
#include <cstring>
// C++ Standard
#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>

// Mocking Paths
std::string GLOBAL_BBS_PATH      = "";
std::string GLOBAL_DATA_PATH     = "";
std::string GLOBAL_MENU_PATH     = "";
std::string GLOBAL_TEXTFILE_PATH = "";
std::string USERS_DATABASE       = "";


/**
 * Handle Setup and Tear Down of Integration Test for SQLite
 */
class MyFixture
{
    
public:

    MyFixture()
        : m_database("xrm_itTest_users.sqlite3")
    { 
        // Can't remove database on closure, becasue the 
        // Object is not cleared till Destructor is finished.
        // Before Each Test, we need to remove existing database.
        std::cout << "Remove xrm_itTest_users" << std::endl;
        remove("xrm_itTest_users.sqlite3");
    }
    
    ~MyFixture() 
    { }
   
    SQLW::Database m_database;
};


/**
 * @brief Unit Testing for Initial Sqlite Database DAO and BaseDao.
 * @return 
 */
SUITE(XRMConferenceDao)
{
    
    // Test DAO with All Base Dao Calls.
    TEST_FIXTURE(MyFixture, ConferenceDaoTest)
    {        
        // Link to users dao for data access object
        conference_dao_ptr confdb(new ConferenceDao(m_database));

        // DataBase Table Should not exist.
        CHECK(!confdb->doesTableExist());
        
        // Setup database Param, cache sies etc..
        CHECK(confdb->firstTimeSetupParams());

        // Check Create Table and Index
        CHECK(confdb->createTable());
        
        // DataBase Table exists.
        CHECK(confdb->doesTableExist());
        
        // Check Insert
        conference_ptr confIn(new Conference());
        confIn->sName = "Test Message Conference";
        confIn->sType = "M";
        confIn->sACS = "s20";
        confIn->iSortOrder = 1;
        
        // Check Insert
        conference_ptr confIn2(new Conference());
        confIn2->sName = "Test FM Conference";
        confIn2->sType = "G";
        confIn2->sACS = "s45";
        confIn2->iSortOrder = 3;
        
        long lastInsertId = confdb->insertRecord(confIn);
        
        long InsertId2nd = confdb->insertRecord(confIn2);
        
        // Returns Lat Insert Id, Should not be -1.
        CHECK(lastInsertId == 1);
        CHECK(InsertId2nd == 2);
        
        // Check Retrieve
        conference_ptr confOut(new Conference());
        confOut = confdb->getRecordById(lastInsertId);

        CHECK(confOut->iId == 1);
        CHECK(confOut->sName == "Test Message Conference");
        CHECK(confOut->sType == "M");
        CHECK(confOut->sACS == "s20");
        CHECK(confOut->iSortOrder == 1);
        
        // Test Update, Id should be 1 already.
        confOut->sName = "Test File Conference";
        confOut->sType = "F";
        confOut->sACS = "s30";
        confOut->iSortOrder = 2;
        
        // Updates returns bool.
        CHECK(confdb->updateRecord(confOut));
        
        //Retrieve Update
        conference_ptr confUpdate(new Conference());
        confUpdate = confdb->getRecordById(lastInsertId);
        
        // Test Output After Update
        CHECK(confOut->iId == 1);
        CHECK(confOut->sName == "Test File Conference");
        CHECK(confOut->sType == "F");
        CHECK(confOut->sACS == "s30");
        CHECK(confOut->iSortOrder == 2);
        
        // Test Get All Records in Vector.
        std::vector<conference_ptr> getAllResults;
        getAllResults = confdb->getAllRecords();
        
        CHECK(getAllResults.size() == 2);
        
        // Test Output After Update
        CHECK(getAllResults[0]->iId == 1);
        CHECK(getAllResults[0]->sName == "Test File Conference");
        CHECK(getAllResults[0]->sType == "F");
        CHECK(getAllResults[0]->sACS == "s30");
        CHECK(getAllResults[0]->iSortOrder == 2);
        
        // Test 2nd Row next record.
        CHECK(getAllResults[1]->iId == 2);
        CHECK(getAllResults[1]->sName == "Test FM Conference");
        CHECK(getAllResults[1]->sType == "G");
        CHECK(getAllResults[1]->sACS == "s45");
        CHECK(getAllResults[1]->iSortOrder == 3);        
        
        // Test Delete Record
        CHECK(confdb->deleteRecord(lastInsertId));
        
        
        // Test Getting Deleted Record
        conference_ptr confDel(new Conference());
        confDel = confdb->getRecordById(lastInsertId);
        
        // Invalid Records return with -1
        CHECK(confDel->iId == -1);
        
        // Test Drop Table and Index returns true;
        CHECK(confdb->dropTable());
        
        // DataBase Table Should not exist.
        CHECK(!confdb->doesTableExist());        
    }    

}


/*
* These Unit Tests are To test the core logic of classes and methods.
* Any method that hit IO, Database, File, Network, is considered an Integration Test
* and will be setup in seperate Tests Suite or done manaully through the interface.
*/


// run all unit tests
int main(int argc, char **argv)
{
    return UnitTest::RunAllTests();
}
