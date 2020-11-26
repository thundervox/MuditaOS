// Copyright (c) 2017-2020, Mudita Sp. z.o.o. All rights reserved.
// For licensing, see https://github.com/mudita/MuditaOS/LICENSE.md

#include "QueryAlarmsRemove.hpp"

namespace db::query::alarms
{
    Remove::Remove(const uint32_t &id) : Query(Query::Type::Delete), id(id)
    {}

    auto Remove::debugInfo() const -> std::string
    {
        return std::string{"Remove"};
    }

    RemoveResult::RemoveResult(const bool &ret) : ret(ret)
    {}

    auto RemoveResult::succeed() const -> bool
    {
        return ret;
    }

    auto RemoveResult::debugInfo() const -> std::string
    {
        return std::string{"RemoveResult"};
    }
} // namespace db::query::alarms
