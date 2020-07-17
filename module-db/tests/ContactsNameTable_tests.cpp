
/*
 * @file ContactsNameTable_tests.cpp
 * @author Mateusz Piesta (mateusz.piesta@mudita.com)
 * @date 28.05.19
 * @brief
 * @copyright Copyright (C) 2019 mudita.com
 * @details
 */

#include "vfs.hpp"

#include <catch2/catch.hpp>

#include "Database/Database.hpp"
#include "Databases/ContactsDB.hpp"

#include <algorithm>

#include <cstdint>
#include <cstdio>
#include <cstring>

TEST_CASE("Contacts Name Table tests")
{
    Database::initialize();

    vfs.remove(ContactsDB::GetDBName());

    ContactsDB contactsdb;
    REQUIRE(contactsdb.isInitialized());

    ContactsNameTableRow testRow1 = {
        {.ID = DB_ID_NONE}, .contactID = DB_ID_NONE, .namePrimary = "Mateusz", .nameAlternative = "Pati"};

    // add 4 elements into table
    REQUIRE(contactsdb.name.add(testRow1));
    REQUIRE(contactsdb.name.add(testRow1));
    REQUIRE(contactsdb.name.add(testRow1));
    REQUIRE(contactsdb.name.add(testRow1));

    // Table should have 4 elements
    REQUIRE(contactsdb.name.count() == 4);

    // update existing element in table
    testRow1.ID              = 4;
    testRow1.nameAlternative = "Pateusz";
    REQUIRE(contactsdb.name.update(testRow1));

    // Get table row using valid ID & check if it was updated
    auto sms = contactsdb.name.getById(4);
    REQUIRE(sms.nameAlternative == testRow1.nameAlternative);

    // Get table row using invalid ID(should return empty contactsdb.nameRow)
    auto smsFailed = contactsdb.name.getById(100);
    REQUIRE(smsFailed.nameAlternative == "");

    // Get table rows using valid offset/limit parameters
    auto retOffsetLimit = contactsdb.name.getLimitOffset(0, 4);
    REQUIRE(retOffsetLimit.size() == 4);

    // Get table rows using valid offset/limit parameters and specific field's ID
    REQUIRE(contactsdb.name.getLimitOffsetByField(0, 4, ContactNameTableFields::NamePrimary, "Mateusz").size() == 4);

    // Get table rows using invalid limit parameters(should return 4 elements instead of 100)
    auto retOffsetLimitBigger = contactsdb.name.getLimitOffset(0, 100);
    REQUIRE(retOffsetLimitBigger.size() == 4);

    // Get table rows using invalid offset/limit parameters(should return empty object)
    auto retOffsetLimitFailed = contactsdb.name.getLimitOffset(5, 4);
    REQUIRE(retOffsetLimitFailed.size() == 0);

    // Get count of elements by field's ID
    REQUIRE(contactsdb.name.countByFieldId("contact_id", 0) == 4);

    // Get count of elements by invalid field's ID
    REQUIRE(contactsdb.name.countByFieldId("invalid_field", 0) == 0);

    REQUIRE(contactsdb.name.removeById(2));

    // Table should have now 3 elements
    REQUIRE(contactsdb.name.count() == 3);

    // Remove non existing element
    REQUIRE(contactsdb.name.removeById(100));

    // Remove all elements from table
    REQUIRE(contactsdb.name.removeById(1));
    REQUIRE(contactsdb.name.removeById(3));
    REQUIRE(contactsdb.name.removeById(4));

    // Table should be empty now
    REQUIRE(contactsdb.name.count() == 0);

    Database::deinitialize();
}
