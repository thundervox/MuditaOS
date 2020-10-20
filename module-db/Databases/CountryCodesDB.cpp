// Copyright (c) 2017-2020, Mudita Sp. z.o.o. All rights reserved.
// For licensing, see https://github.com/mudita/MuditaOS/LICENSE.md

#include "CountryCodesDB.hpp"

const char *CountryCodesDB::dbName = "country-codes.db";

CountryCodesDB::CountryCodesDB() : Database(dbName), countryCodes(this)
{
    if (countryCodes.create() == false)
        return;
    isInitialized_ = true;
}

CountryCodesDB::~CountryCodesDB()
{}