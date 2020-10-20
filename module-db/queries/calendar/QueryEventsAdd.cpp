// Copyright (c) 2017-2020, Mudita Sp. z.o.o. All rights reserved.
// For licensing, see https://github.com/mudita/MuditaOS/LICENSE.md

#include "QueryEventsAdd.hpp"

namespace db::query::events
{
    Add::Add(EventsRecord record) : Query(Query::Type::Create), record(record)
    {}

    auto Add::getRecord() const -> EventsRecord
    {
        return record;
    }

    auto Add::debugInfo() const -> std::string
    {
        return "Add";
    }

    AddResult::AddResult(bool ret) : ret(ret)
    {}

    auto AddResult::getResult() const -> bool
    {
        return ret;
    }

    auto AddResult::debugInfo() const -> std::string
    {
        return "AddResult";
    }
} // namespace db::query::events
