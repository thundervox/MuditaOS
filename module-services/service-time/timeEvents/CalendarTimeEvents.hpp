// Copyright (c) 2017-2020, Mudita Sp. z.o.o. All rights reserved.
// For licensing, see https://github.com/mudita/MuditaOS/LICENSE.md

#pragma once
#include <module-db/queries/calendar/QueryEventsEdit.hpp>
#include <stdint.h> // for uint32_t
#include <memory>   // for allocator, unique_ptr
#include <string>   // for string

#include "TimeEvents.hpp"                           // for TimeEvents
#include "EventsRecord.hpp"                         // for EventsRecord
#include "application-calendar/data/dateCommon.hpp" // for TIME_POINT_INVALID, TimePoint

namespace db
{
    class QueryResult;
} // namespace db
namespace sys
{
    class Service;
} // namespace sys

namespace stm
{
    class CalendarTimeEvents : public TimeEvents
    {
      private:
        EventsRecord eventRecord;
        TimePoint startTP = TIME_POINT_INVALID;

      protected:
        const std::string timerName() override
        {
            return "CalendarTimeEvents_timer";
        };
        uint32_t calcToNextEventInterval(std::unique_ptr<db::QueryResult> nextEventQueryResult) override;

        bool sendNextEventQuery() override;
        bool sendEventFiredQuery() override;
        void invokeEvent() override;

      public:
        CalendarTimeEvents() = delete;
        explicit CalendarTimeEvents(sys::Service *service);
        ~CalendarTimeEvents() = default;
    };

} // namespace stm
