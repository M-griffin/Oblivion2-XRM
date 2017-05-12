
#include "data-app/oneliners_dao.hpp"
#include "model-app/oneliners.hpp"

#ifdef _WIN32
#include <UnitTest++.h>
#else
#include <unittest++/UnitTest++.h>
#endif

#include "libSqliteWrapped.h"

#include <boost/smart_ptr/shared_ptr.hpp>

/**
 * Handle Setup and Tear Down of Integration Test for SQLite
 */
class MyFixtureOneLiners
{
    
public:

    MyFixtureOneLiners()
        : m_database("xrm_itOnelinersTest.sqlite3")
    { 
        // Can't remove database on closure, becasue the 
        // Object is not cleared till Destructor is finished.
        // Before Each Test, we need to remove existing database.
        std::cout << "xrm_itOnelinersTest.sqlite3" << std::endl;
        remove("xrm_itOnelinersTest.sqlite3");
    }
    
    ~MyFixtureOneLiners() 
    { }
   
    SQLW::Database m_database;
};


/**
 * @brief Unit Testing for Initial Sqlite Database DAO and BaseDao.
 * @return 
 */
SUITE(XRMOnelinersDao)
{
    
    // Test DAO with All Base Dao Calls.
    TEST_FIXTURE(MyFixtureOneLiners, OnelinersDaoTest)
    {        
        // Link to users dao for data access object
        oneliner_dao_ptr objdb(new OnelinerDao(m_database));

        // DataBase Table Should not exist.
        CHECK(!objdb->doesTableExist());
        
        // Setup database Param, cache sies etc..
        CHECK(objdb->firstTimeSetupParams());

        // Check Create Table and Index
        CHECK(objdb->createTable());
        
        // DataBase Table exists.
        CHECK(objdb->doesTableExist());
        
        // Check Insert
        oneliner_ptr objIn(new Oneliners());
        objIn->iUserId = 1;
        objIn->sText = "One Liner Text";
        objIn->sUserName = "John Snow";
        objIn->sUserInitials = "JS";
        objIn->dtDatePosted = 0;
        
        // Check Insert
        oneliner_ptr objIn2(new Oneliners());
        objIn2->iUserId = 2;
        objIn2->sText = "One Liner Text 2";
        objIn2->sUserName = "Troll Boy";
        objIn2->sUserInitials = "TB";
        objIn2->dtDatePosted = 1;
        
        long lastInsertId = objdb->insertRecord(objIn);
        
        long InsertId2nd = objdb->insertRecord(objIn2);
        
        // Returns Lat Insert Id, Should not be -1.
        CHECK(lastInsertId == 1);
        CHECK(InsertId2nd == 2);
        
        // Check Retrieve
        oneliner_ptr objOut(new Oneliners());
        objOut = objdb->getRecordById(lastInsertId);

        CHECK(objOut->iUserId == 1);
        CHECK(objOut->sText =="One Liner Text");
        CHECK(objOut->sUserName == "John Snow");
        CHECK(objOut->sUserInitials == "JS");
        CHECK(objOut->dtDatePosted == 0);
        
        
        // Test Update, Id should be 1 already.        
        objOut->iUserId = 3;
        objOut->sText = "One Liner Text 3";
        objOut->sUserName = "John Snow SR.";
        objOut->sUserInitials = "JSS";
        objOut->dtDatePosted = 0;
        
        
        // Updates returns bool.
        CHECK(objdb->updateRecord(objOut));
        
        //Retrieve Update
        oneliner_ptr objUpdate(new Oneliners());
        objUpdate = objdb->getRecordById(lastInsertId);
        
        // Test Output After Update
        CHECK(objUpdate->iId == 1);
        CHECK(objUpdate->iUserId == 3);
        CHECK(objUpdate->sText == "One Liner Text 3");
        CHECK(objUpdate->sUserName == "John Snow SR.");
        CHECK(objUpdate->sUserInitials == "JSS");
        CHECK(objUpdate->dtDatePosted == 0);
        
        // Test Get All Records in Vector.
        std::vector<oneliner_ptr> getAllResults;
        getAllResults = objdb->getAllRecords();
        
        CHECK(getAllResults.size() == 2);
        
        // Test Output After Update        
        CHECK(getAllResults[0]->iId == 1);
        CHECK(getAllResults[0]->iUserId == 3);
        CHECK(getAllResults[0]->sText == "One Liner Text 3");
        CHECK(getAllResults[0]->sUserName == "John Snow SR.");
        CHECK(getAllResults[0]->sUserInitials == "JSS");
        CHECK(getAllResults[0]->dtDatePosted == 0);
        
        
        // Test 2nd Row next record.
        CHECK(getAllResults[1]->iUserId == 2);
        CHECK(getAllResults[1]->sText == "One Liner Text 2");
        CHECK(getAllResults[1]->sUserName == "Troll Boy");
        CHECK(getAllResults[1]->sUserInitials == "TB");
        CHECK(getAllResults[1]->dtDatePosted == 1);
        
        // Test Record Counts
        long checkCounts = objdb->getRecordsCount();
        
        // Check Record Counts
        CHECK(checkCounts == 2);
        
        // Test Delete Record
        CHECK(objdb->deleteRecord(lastInsertId));
                        
        // Test Getting Deleted Record
        oneliner_ptr objDel(new Oneliners());
        objDel = objdb->getRecordById(lastInsertId);
        
        // Invalid Records return with -1
        CHECK(objDel->iId == -1);
        
        // Test Record Counts After Purge
        long checkCounts2 = objdb->getRecordsCount();
        
        // Check Record Counts
        CHECK(checkCounts2 == 1);
        
        // Test Drop Table and Index returns true;
        CHECK(objdb->dropTable());
        
        // DataBase Table Should not exist.
        CHECK(!objdb->doesTableExist());        
    }    

}
