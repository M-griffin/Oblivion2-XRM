
#include "data-sys/conference_dao.hpp"
#include "model-sys/conference.hpp"

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
class MyFixtureConference
{
    
public:

    MyFixtureConference()
        : m_database("xrm_itConferenceTest.sqlite3")
    { 
        // Can't remove database on closure, becasue the 
        // Object is not cleared till Destructor is finished.
        // Before Each Test, we need to remove existing database.
        std::cout << "xrm_itConferenceTest.sqlite3" << std::endl;
        remove("xrm_itConferenceTest.sqlite3");
    }
    
    ~MyFixtureConference() 
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
    TEST_FIXTURE(MyFixtureConference, ConferenceDaoTest)
    {        
        // Link to users dao for data access object
        conference_dao_ptr objdb(new ConferenceDao(m_database));

        // DataBase Table Should not exist.
        CHECK(!objdb->doesTableExist());
        
        // Setup database Param, cache sies etc..
        CHECK(objdb->firstTimeSetupParams());

        // Check Create Table and Index
        CHECK(objdb->createTable());
        
        // DataBase Table exists.
        CHECK(objdb->doesTableExist());
        
        // Check Insert
        conference_ptr objIn(new Conference());
        objIn->sName = "Test Message Conference";
        objIn->sType = "M";
        objIn->sACS = "s20";
        objIn->iSortOrder = 1;
        
        // Check Insert
        conference_ptr objIn2(new Conference());
        objIn2->sName = "Test FM Conference";
        objIn2->sType = "G";
        objIn2->sACS = "s45";
        objIn2->iSortOrder = 3;
        
        long lastInsertId = objdb->insertRecord(objIn);
        
        long InsertId2nd = objdb->insertRecord(objIn2);
        
        // Returns Lat Insert Id, Should not be -1.
        CHECK(lastInsertId == 1);
        CHECK(InsertId2nd == 2);
        
        // Check Retrieve
        conference_ptr objOut(new Conference());
        objOut = objdb->getRecordById(lastInsertId);

        CHECK(objOut->iId == 1);
        CHECK(objOut->sName == "Test Message Conference");
        CHECK(objOut->sType == "M");
        CHECK(objOut->sACS == "s20");
        CHECK(objOut->iSortOrder == 1);
        
        // Test Update, Id should be 1 already.
        objOut->sName = "Test File Conference";
        objOut->sType = "F";
        objOut->sACS = "s30";
        objOut->iSortOrder = 2;
        
        // Updates returns bool.
        CHECK(objdb->updateRecord(objOut));
        
        //Retrieve Update
        conference_ptr objUpdate(new Conference());
        objUpdate = objdb->getRecordById(lastInsertId);
        
        // Test Output After Update
        CHECK(objOut->iId == 1);
        CHECK(objOut->sName == "Test File Conference");
        CHECK(objOut->sType == "F");
        CHECK(objOut->sACS == "s30");
        CHECK(objOut->iSortOrder == 2);
        
        // Test Get All Records in Vector.
        std::vector<conference_ptr> getAllResults;
        getAllResults = objdb->getAllRecords();
        
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
        
        // Test Record Counts
        long checkCounts = objdb->getRecordsCount();
        
        // Check Record Counts
        CHECK(checkCounts == 2);
        
        // Test Delete Record
        CHECK(objdb->deleteRecord(lastInsertId));
                        
        // Test Getting Deleted Record
        conference_ptr objDel(new Conference());
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
