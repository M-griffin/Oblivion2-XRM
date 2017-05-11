
#include "data-sys/message_area_dao.hpp"
#include "model-sys/message_area.hpp"

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
class MyFixtureMessageArea
{
    
public:

    MyFixtureMessageArea()
        : m_database("xrm_itMessageAreaTest.sqlite3")
    { 
        // Can't remove database on closure, becasue the 
        // Object is not cleared till Destructor is finished.
        // Before Each Test, we need to remove existing database.
        std::cout << "xrm_itMessageAreaTest.sqlite3" << std::endl;
        remove("xrm_itMessageAreaTest.sqlite3");
    }
    
    ~MyFixtureMessageArea() 
    { }
   
    SQLW::Database m_database;
};


/**
 * @brief Unit Testing for Initial Sqlite Database DAO and BaseDao.
 * @return 
 */
SUITE(XRMMessageAreaDao)
{
    
    // Test DAO with All Base Dao Calls.
    TEST_FIXTURE(MyFixtureMessageArea, MessageAreaDaoTest)
    {        
        // Link to users dao for data access object
        msg_area_dao_ptr objdb(new MessageAreaDao(m_database));

        // DataBase Table Should not exist.
        CHECK(!objdb->doesTableExist());
        
        // Setup database Param, cache sies etc..
        CHECK(objdb->firstTimeSetupParams());

        // Check Create Table and Index
        CHECK(objdb->createTable());
        
        // DataBase Table exists.
        CHECK(objdb->doesTableExist());
        
        // Check Insert
        message_area_ptr objIn(new MessageArea());        
        objIn->sName = "General Area";
        objIn->sAcsAccess = "s20";
        objIn->sAcsPost = "s21";
        objIn->bAnonymous = false;
        objIn->sSponsor = "John Snow";
        objIn->sOriginLine = "obv/2 xrm";
        objIn->sFidoPath = "C:\\xrm\\";
        objIn->iNetworkId = 1;
        objIn->sQwkName = "PACKET";
        objIn->iMaxMessages = 200;
        objIn->bRealName = false;
        objIn->sLinkname = "link";
        objIn->bRequired = false;
        objIn->bPrivate = false;
        objIn->bNetmail = false;
        objIn->iSortOrder = 1;
        
        // Check Insert
        message_area_ptr objIn2(new MessageArea());
        objIn2->sName = "Sysop Area";
        objIn2->sAcsAccess = "s220";
        objIn2->sAcsPost = "s221";
        objIn2->bAnonymous = true;
        objIn2->sSponsor = "John Snow Sr";
        objIn2->sOriginLine = "obv/2 xrm ++";
        objIn2->sFidoPath = "D:\\xrm\\network";
        objIn2->iNetworkId = 2;
        objIn2->sQwkName = "PACKET1";
        objIn2->iMaxMessages = 300;
        objIn2->bRealName = true;
        objIn2->sLinkname = "link2";
        objIn2->bRequired = true;
        objIn2->bPrivate = true;
        objIn2->bNetmail = true;
        objIn2->iSortOrder = 2;
        
        long lastInsertId = objdb->insertRecord(objIn);
        
        long InsertId2nd = objdb->insertRecord(objIn2);
        
        // Returns Lat Insert Id, Should not be -1.
        CHECK(lastInsertId == 1);
        CHECK(InsertId2nd == 2);
        
        // Check Retrieve
        message_area_ptr objOut(new MessageArea());
        objOut = objdb->getRecordById(lastInsertId);
        
        CHECK(objOut->sName == "General Area");
        CHECK(objOut->sAcsAccess == "s20");
        CHECK(objOut->sAcsPost == "s21");
        CHECK(objOut->bAnonymous == false);
        CHECK(objOut->sSponsor == "John Snow");
        CHECK(objOut->sOriginLine == "obv/2 xrm");
        CHECK(objOut->sFidoPath == "C:\\xrm\\");
        CHECK(objOut->iNetworkId == 1);
        CHECK(objOut->sQwkName == "PACKET");
        CHECK(objOut->iMaxMessages == 200);
        CHECK(objOut->bRealName == false);
        CHECK(objOut->sLinkname == "link");
        CHECK(objOut->bRequired == false);
        CHECK(objOut->bPrivate == false);
        CHECK(objOut->bNetmail == false);
        CHECK(objOut->iSortOrder == 1);
                
        
        // Test Update, Id should be 1 already.   
        objOut->sName = "General Area 3";
        objOut->sAcsAccess = "s30";
        objOut->sAcsPost = "s31";
        objOut->bAnonymous = true;
        objOut->sSponsor = "John Snow XX";
        objOut->sOriginLine = "obv/2 xrm 3";
        objOut->sFidoPath = "C:\\xrm\\fido";
        objOut->iNetworkId = 3;
        objOut->sQwkName = "PACKET3";
        objOut->iMaxMessages = 3200;
        objOut->bRealName = true;
        objOut->sLinkname = "link3";
        objOut->bRequired = true;
        objOut->bPrivate = true;
        objOut->bNetmail = true;
        objOut->iSortOrder = 3;
        
        // Updates returns bool.
        CHECK(objdb->updateRecord(objOut));
        
        //Retrieve Update
        message_area_ptr objUpdate(new MessageArea());
        objUpdate = objdb->getRecordById(lastInsertId);
        
        // Test Output After Update
        CHECK(objUpdate->sName == "General Area 3");
        CHECK(objUpdate->sAcsAccess == "s30");
        CHECK(objUpdate->sAcsPost == "s31");
        CHECK(objUpdate->bAnonymous == true);
        CHECK(objUpdate->sSponsor == "John Snow XX");
        CHECK(objUpdate->sOriginLine == "obv/2 xrm 3");
        CHECK(objUpdate->sFidoPath == "C:\\xrm\\fido");
        CHECK(objUpdate->iNetworkId == 3);
        CHECK(objUpdate->sQwkName == "PACKET3");
        CHECK(objUpdate->iMaxMessages == 3200);
        CHECK(objUpdate->bRealName == true);
        CHECK(objUpdate->sLinkname == "link3");
        CHECK(objUpdate->bRequired == true);
        CHECK(objUpdate->bPrivate == true);
        CHECK(objUpdate->bNetmail == true);
        CHECK(objUpdate->iSortOrder == 3);
               
        // Test Get All Records in Vector.
        std::vector<message_area_ptr> getAllResults;
        getAllResults = objdb->getAllRecords();
        
        CHECK(getAllResults.size() == 2);
        
        // Test Output After Update        
        CHECK(getAllResults[0]->iId == 1);
        CHECK(getAllResults[0]->sName == "General Area 3");
        CHECK(getAllResults[0]->sAcsAccess == "s30");
        CHECK(getAllResults[0]->sAcsPost == "s31");
        CHECK(getAllResults[0]->bAnonymous == true);
        CHECK(getAllResults[0]->sSponsor == "John Snow XX");
        CHECK(getAllResults[0]->sOriginLine == "obv/2 xrm 3");
        CHECK(getAllResults[0]->sFidoPath == "C:\\xrm\\fido");
        CHECK(getAllResults[0]->iNetworkId == 3);
        CHECK(getAllResults[0]->sQwkName == "PACKET3");
        CHECK(getAllResults[0]->iMaxMessages == 3200);
        CHECK(getAllResults[0]->bRealName == true);
        CHECK(getAllResults[0]->sLinkname == "link3");
        CHECK(getAllResults[0]->bRequired == true);
        CHECK(getAllResults[0]->bPrivate == true);
        CHECK(getAllResults[0]->bNetmail == true);
        CHECK(getAllResults[0]->iSortOrder == 3);
               
        
        // Test 2nd Row next record.
        CHECK(getAllResults[1]->iId == 2);
        CHECK(getAllResults[1]->sName == "Sysop Area");
        CHECK(getAllResults[1]->sAcsAccess == "s220");
        CHECK(getAllResults[1]->sAcsPost == "s221");
        CHECK(getAllResults[1]->bAnonymous == true);
        CHECK(getAllResults[1]->sSponsor == "John Snow Sr");
        CHECK(getAllResults[1]->sOriginLine == "obv/2 xrm ++");
        CHECK(getAllResults[1]->sFidoPath == "D:\\xrm\\network");
        CHECK(getAllResults[1]->iNetworkId == 2);
        CHECK(getAllResults[1]->sQwkName == "PACKET1");
        CHECK(getAllResults[1]->iMaxMessages == 300);
        CHECK(getAllResults[1]->bRealName == true);
        CHECK(getAllResults[1]->sLinkname == "link2");
        CHECK(getAllResults[1]->bRequired == true);
        CHECK(getAllResults[1]->bPrivate == true);
        CHECK(getAllResults[1]->bNetmail == true);
        CHECK(getAllResults[1]->iSortOrder == 2);
                
        // Test Record Counts
        long checkCounts = objdb->getRecordsCount();
        
        // Check Record Counts
        CHECK(checkCounts == 2);
        
        // Test Delete Record
        CHECK(objdb->deleteRecord(lastInsertId));
                        
        // Test Getting Deleted Record
        message_area_ptr objDel(new MessageArea());
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
