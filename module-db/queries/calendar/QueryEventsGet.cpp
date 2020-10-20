// Copyright (c) 2017-2020, Mudita Sp. z.o.o. All rights reserved.
// For licensing, see https://github.com/mudita/MuditaOS/LICENSE.md

#include "QueryEventsGet.hpp"

namespace db::query::events
{
    Get::Get(const uint32_t &id) : Query(Query::Type::Read), id(id)
    {}

    auto Get::debugInfo() const -> std::string
    {
        return "Get";
    }

    GetResult::GetResult(EventsRecord record) : record(record)
    {}

    auto GetResult::getResult() const -> EventsRecord
    {
        return record;
    }

    auto GetResult::debugInfo() const -> std::string
    {
        return "GetResult";
    }
} // namespace db::query::events
