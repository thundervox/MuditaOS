
/*
 * @file SMSRecord_tests.cpp
 * @author Mateusz Piesta (mateusz.piesta@mudita.com)
 * @date 05.06.19
 * @brief
 * @copyright Copyright (C) 2019 mudita.com
 * @details
 */


#include "vfs.hpp"

#include "catch.hpp"

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <algorithm>

#include <iostream>
#include <Interface/ContactRecord.hpp>

#include "../Database/Database.hpp"
#include "../Databases/ContactsDB.hpp"
#include "../Databases/SmsDB.hpp"
#include "../Interface/SMSRecord.hpp"

struct test{
    UTF8 test;
    UTF8 test2;
};


TEST_CASE("SMS Record tests")
{
    Database::Initialize();

    vfs.remove(ContactsDB::GetDBName());
    vfs.remove(SmsDB::GetDBName());


    const uint32_t dateTest = 123456789;
    const uint32_t dateSentTest = 987654321;
    const uint32_t errorCodeTest = 555;
    const char *numberTest = "111222333";
    const char *numberTest2 = "222333444";
    const char *bodyTest = "Test SMS Body";
    const char *bodyTest2 = "Test SMS Body2";
    const bool isReadTest = true;
    const SMSType typeTest = SMSType ::DRAFT;


    SMSRecordInterface smsRecInterface;

    SMSRecord recordIN;
    recordIN.date = dateTest;
    recordIN.dateSent = dateSentTest;
    recordIN.errorCode = errorCodeTest;
    recordIN.number = numberTest;
    recordIN.body = bodyTest;
    recordIN.isRead = isReadTest;
    recordIN.type = typeTest;

    // Add 2 records
    REQUIRE(smsRecInterface.Add(recordIN));
    REQUIRE(smsRecInterface.Add(recordIN));

    // Get all available records
    auto records = smsRecInterface.GetLimitOffset(0,100);
    REQUIRE((*records).size() == 2);

    // Check if fetched records contain valid data
    for(const auto &w : *records){
        REQUIRE(w.body == bodyTest);
        REQUIRE(w.number == numberTest);
        REQUIRE(w.isRead == isReadTest);

    }

    // Get all available records by specified thread ID and check for invalid data
    records = smsRecInterface.GetLimitOffsetByField(0,100,SMSRecordField::ThreadID,"1");
    REQUIRE((*records).size() == 2);
    for(const auto &w : *records){
        REQUIRE(w.body == bodyTest);
        REQUIRE(w.number == numberTest);
        REQUIRE(w.isRead == isReadTest);

    }

    // Get all available records by specified contact ID and check for invalid data
    records = smsRecInterface.GetLimitOffsetByField(0,100,SMSRecordField::ContactID,"1");
    REQUIRE((*records).size() == 2);
    for(const auto &w : *records){
        REQUIRE(w.body == bodyTest);
        REQUIRE(w.number == numberTest);
        REQUIRE(w.isRead == isReadTest);

    }

    // Remove records one by one
    REQUIRE(smsRecInterface.RemoveByID(1));
    REQUIRE(smsRecInterface.RemoveByID(2));

    // SMS database should not contain any records
    REQUIRE(smsRecInterface.GetCount() == 0);

    // Test updating record
    REQUIRE(smsRecInterface.Add(recordIN));
    recordIN.dbID=1;
    recordIN.body = bodyTest2;
    REQUIRE(smsRecInterface.Update(recordIN));

    auto record = smsRecInterface.GetByID(1);
    REQUIRE(record.dbID != 0);
    REQUIRE(record.body == bodyTest2);

    // SMS database should contain 1 record
    REQUIRE(smsRecInterface.GetCount() == 1);

    // Remove existing record
    REQUIRE(smsRecInterface.RemoveByID(1));
    // SMS database should be empty
    REQUIRE(smsRecInterface.GetCount() == 0);

    // Test fetching record by using Thread ID
    // 1) Add records with different numbers, they should be placed in separate threads&contacts dbs
    recordIN.body = bodyTest;
    recordIN.number = numberTest;
    REQUIRE(smsRecInterface.Add(recordIN));
    REQUIRE(smsRecInterface.Add(recordIN));

    recordIN.number =numberTest2;
    REQUIRE(smsRecInterface.Add(recordIN));
    REQUIRE(smsRecInterface.Add(recordIN));

    // Get all available records by specified thread ID and check for invalid data
    records = smsRecInterface.GetLimitOffsetByField(0,100,SMSRecordField::ThreadID,"1");
    REQUIRE((*records).size() == 2);
    for(const auto &w : *records){
        REQUIRE(w.body == bodyTest);
        REQUIRE(w.number == numberTest);
        REQUIRE(w.isRead == isReadTest);

    }

    // Get all available records by specified thread ID and check for invalid data
    records = smsRecInterface.GetLimitOffsetByField(0,100,SMSRecordField::ThreadID,"2");
    REQUIRE((*records).size() == 2);
    for(const auto &w : *records){
        REQUIRE(w.body == bodyTest);
        REQUIRE(w.number == numberTest2);
        REQUIRE(w.isRead == isReadTest);

    }

    // Get all available records by specified contact ID and check for invalid data
    records = smsRecInterface.GetLimitOffsetByField(0,100,SMSRecordField::ContactID,"1");
    REQUIRE((*records).size() == 2);
    for(const auto &w : *records){
        REQUIRE(w.body == bodyTest);
        REQUIRE(w.number == numberTest);
        REQUIRE(w.isRead == isReadTest);

    }

    // Get all available records by specified contact ID and check for invalid data
    records = smsRecInterface.GetLimitOffsetByField(0,100,SMSRecordField::ContactID,"2");
    REQUIRE((*records).size() == 2);
    for(const auto &w : *records){
        REQUIRE(w.body == bodyTest);
        REQUIRE(w.number == numberTest2);
        REQUIRE(w.isRead == isReadTest);

    }


}