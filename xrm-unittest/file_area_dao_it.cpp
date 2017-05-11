
#include "data-sys/file_area_dao.hpp"
#include "model-sys/file_area.hpp"

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
class MyFixtureFileArea
{
    
public:

    MyFixtureFileArea()
        : m_database("xrm_itFileAreaTest.sqlite3")
    { 
        // Can't remove database on closure, becasue the 
        // Object is not cleared till Destructor is finished.
        // Before Each Test, we need to remove existing database.
        std::cout << "xrm_itFileAreaTest.sqlite3" << std::endl;
        remove("xrm_itFileAreaTest.sqlite3");
    }
    
    ~MyFixtureFileArea() 
    { }
   
    SQLW::Database m_database;
};


/**
 * @brief Unit Testing for Initial Sqlite Database DAO and BaseDao.
 * @return 
 */
SUITE(XRMFileAreaDao)
{
    
    // Test DAO with All Base Dao Calls.
    TEST_FIXTURE(MyFixtureFileArea, FileAreaDaoTest)
    {        
        // Link to users dao for data access object
        file_area_dao_ptr objdb(new FileAreaDao(m_database));

        // DataBase Table Should not exist.
        CHECK(!objdb->doesTableExist());
        
        // Setup database Param, cache sies etc..
        CHECK(objdb->firstTimeSetupParams());

        // Check Create Table and Index
        CHECK(objdb->createTable());
        
        // DataBase Table exists.
        CHECK(objdb->doesTableExist());
        
        // Check Insert
        file_area_ptr objIn(new FileArea());        
        objIn->sName = "General Area";
        objIn->sAcsAccess = "s20";
        objIn->sAcsUpload = "s21";
        objIn->sAcsDownload = "s22";
        objIn->sAcsList = "s23";
        objIn->sSponsor = "John Snow";
        objIn->iSecurityIndex = 1;
        objIn->sLinkname = "Link1";
        objIn->sSort = "F";
        objIn->iMultiplier = 1;
        objIn->bFreeArea = true;
        objIn->iSortOrder = 1;
        
        // Check Insert
        file_area_ptr objIn2(new FileArea());
        objIn2->sName = "Sysop Area";
        objIn2->sAcsAccess = "s220";
        objIn2->sAcsUpload = "s221";
        objIn2->sAcsDownload = "s222";
        objIn2->sAcsList = "s223";
        objIn2->sSponsor = "John Snow Sr.";
        objIn2->iSecurityIndex = 2;
        objIn2->sLinkname = "Link2";
        objIn2->sSort = "E";
        objIn2->iMultiplier = 2;
        objIn2->bFreeArea = false;
        objIn2->iSortOrder = 2;
        
        long lastInsertId = objdb->insertRecord(objIn);
        
        long InsertId2nd = objdb->insertRecord(objIn2);
        
        // Returns Lat Insert Id, Should not be -1.
        CHECK(lastInsertId == 1);
        CHECK(InsertId2nd == 2);
        
        // Check Retrieve
        file_area_ptr objOut(new FileArea());
        objOut = objdb->getRecordById(lastInsertId);
        
        CHECK(objOut->sName == "General Area");
        CHECK(objOut->sAcsAccess == "s20");
        CHECK(objOut->sAcsUpload == "s21");
        CHECK(objOut->sAcsDownload == "s22");
        CHECK(objOut->sAcsList == "s23");
        CHECK(objOut->sSponsor == "John Snow");
        CHECK(objOut->iSecurityIndex == 1);
        CHECK(objOut->sLinkname =="Link1");
        CHECK(objOut->sSort == "F");
        CHECK(objOut->iMultiplier == 1);
        CHECK(objOut->bFreeArea == true);
        CHECK(objOut->iSortOrder == 1);
        
        
        // Test Update, Id should be 1 already.                
        objOut->sName = "General Areas2";
        objOut->sAcsAccess = "s30";
        objOut->sAcsUpload = "s31";
        objOut->sAcsDownload = "s32";
        objOut->sAcsList = "s33";
        objOut->sSponsor = "John Snow II";
        objOut->iSecurityIndex = 3;
        objOut->sLinkname = "Link3";
        objOut->sSort = "M";
        objOut->iMultiplier = 3;
        objOut->bFreeArea = false;
        objOut->iSortOrder = 3;
        
        
        // Updates returns bool.
        CHECK(objdb->updateRecord(objOut));
        
        //Retrieve Update
        file_area_ptr objUpdate(new FileArea());
        objUpdate = objdb->getRecordById(lastInsertId);
        
        // Test Output After Update
        CHECK(objUpdate->sName == "General Areas2");
        CHECK(objUpdate->sAcsAccess == "s30");
        CHECK(objUpdate->sAcsUpload == "s31");
        CHECK(objUpdate->sAcsDownload == "s32");
        CHECK(objUpdate->sAcsList == "s33");
        CHECK(objUpdate->sSponsor == "John Snow II");
        CHECK(objUpdate->iSecurityIndex == 3);
        CHECK(objUpdate->sLinkname == "Link3");
        CHECK(objUpdate->sSort == "M");
        CHECK(objUpdate->iMultiplier == 3);
        CHECK(objUpdate->bFreeArea == false);
        CHECK(objUpdate->iSortOrder == 3);        
        
        // Test Get All Records in Vector.
        std::vector<file_area_ptr> getAllResults;
        getAllResults = objdb->getAllRecords();
        
        CHECK(getAllResults.size() == 2);
        
        // Test Output After Update
        // Test Output After Update
        CHECK(getAllResults[0]->iId == 1);
        CHECK(getAllResults[0]->sName == "General Areas2");
        CHECK(getAllResults[0]->sAcsAccess == "s30");
        CHECK(getAllResults[0]->sAcsUpload == "s31");
        CHECK(getAllResults[0]->sAcsDownload == "s32");
        CHECK(getAllResults[0]->sAcsList == "s33");
        CHECK(getAllResults[0]->sSponsor == "John Snow II");
        CHECK(getAllResults[0]->iSecurityIndex == 3);
        CHECK(getAllResults[0]->sLinkname == "Link3");
        CHECK(getAllResults[0]->sSort == "M");
        CHECK(getAllResults[0]->iMultiplier == 3);
        CHECK(getAllResults[0]->bFreeArea == false);
        CHECK(getAllResults[0]->iSortOrder == 3);        
               
        
        // Test 2nd Row next record.
        CHECK(getAllResults[1]->sName == "Sysop Area");
        CHECK(getAllResults[1]->sAcsAccess == "s220");
        CHECK(getAllResults[1]->sAcsUpload == "s221");
        CHECK(getAllResults[1]->sAcsDownload == "s222");
        CHECK(getAllResults[1]->sAcsList == "s223");
        CHECK(getAllResults[1]->sSponsor == "John Snow Sr.");
        CHECK(getAllResults[1]->iSecurityIndex == 2);
        CHECK(getAllResults[1]->sLinkname == "Link2");
        CHECK(getAllResults[1]->sSort == "E");
        CHECK(getAllResults[1]->iMultiplier == 2);
        CHECK(getAllResults[1]->bFreeArea == false);
        CHECK(getAllResults[1]->iSortOrder == 2);
        
        // Test Record Counts
        long checkCounts = objdb->getRecordsCount();
        
        // Check Record Counts
        CHECK(checkCounts == 2);
        
        // Test Delete Record
        CHECK(objdb->deleteRecord(lastInsertId));
                        
        // Test Getting Deleted Record
        file_area_ptr objDel(new FileArea());
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
