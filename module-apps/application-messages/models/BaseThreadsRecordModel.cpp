﻿// Copyright (c) 2017-2020, Mudita Sp. z.o.o. All rights reserved.
// For licensing, see https://github.com/mudita/MuditaOS/LICENSE.md

#include "BaseThreadsRecordModel.hpp"
#include "ListView.hpp"
#include <service-db/DBServiceAPI.hpp>

BaseThreadsRecordModel::BaseThreadsRecordModel(app::Application *app) : DatabaseModel(app)
{
    requestRecordsCount();
}

unsigned int BaseThreadsRecordModel::requestRecordsCount()
{
    return recordsCount;
}

bool BaseThreadsRecordModel::updateRecords(std::vector<ThreadListStruct> records)
{
    DatabaseModel::updateRecords(std::move(records));
    list->onProviderDataUpdate();
    return true;
}

void BaseThreadsRecordModel::requestRecords(uint32_t offset, uint32_t limit)
{
    DBServiceAPI::ThreadGetLimitOffset(application, offset, limit);
}
