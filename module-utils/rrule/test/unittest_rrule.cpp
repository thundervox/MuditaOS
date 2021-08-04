// Copyright (c) 2017-2021, Mudita Sp. z.o.o. All rights reserved.
// For licensing, see https://github.com/mudita/MuditaOS/LICENSE.md

#include <rrule/rrule.hpp>

#include <date/date.h>
#include <cstring>
#include <iostream>
#include <memory>
#include <unistd.h>
#include <chrono>
#include <regex>

#define CATCH_CONFIG_MAIN // This tells Catch to provide a main() - only do this in one cpp file

#include <catch2/catch.hpp>

namespace rrule
{
    static TimePoint TimePointFromString(const char *s1)
    {
        TimePoint tp;
        std::istringstream(s1) >> date::parse("%F %T", tp);
        return tp;
    }
    TEST_CASE("RRule")
    {
        SECTION("ParseFromString")
        {
            SECTION("Basic")
            {
                RRule rrule;
                rrule.parseFromString("FREQ=DAILY;INTERVAL=1");
                REQUIRE(rrule.freq == RRule::RRuleFrequency::DAILY_RECURRENCE);
                REQUIRE(rrule.count == 0);
                REQUIRE(rrule.until == TimePoint::min());
                REQUIRE(rrule.interval == 1);
                REQUIRE(rrule.weekStart == RRule::RRuleWeekday::MONDAY_WEEKDAY);
                REQUIRE((rrule.bySecond.empty() && rrule.byMinute.empty() && rrule.byHour.empty() &&
                         rrule.byDay.empty() && rrule.byMonthDay.empty() && rrule.byYearDay.empty() &&
                         rrule.byMonth.empty() && rrule.byWeekNo.empty() && rrule.bySetPos.empty()));
            }

            SECTION("Monthly")
            {
                RRule rrule;
                rrule.parseFromString("FREQ=MONTHLY;BYMONTHDAY=1;INTERVAL=1");
                REQUIRE(rrule.freq == RRule::RRuleFrequency::MONTHLY_RECURRENCE);
                REQUIRE(rrule.count == 0);
                REQUIRE(rrule.until == TimePoint::min());
                REQUIRE(rrule.interval == 1);
                REQUIRE(rrule.weekStart == RRule::RRuleWeekday::MONDAY_WEEKDAY);
                REQUIRE(rrule.byMonthDay.size() == 1);
                REQUIRE(rrule.byMonthDay[0] == 1);
                REQUIRE((rrule.bySecond.empty() && rrule.byMinute.empty() && rrule.byHour.empty() &&
                         rrule.byDay.empty() && rrule.byYearDay.empty() && rrule.byMonth.empty() &&
                         rrule.byWeekNo.empty() && rrule.bySetPos.empty()));
            }

            SECTION("Until")
            {
                RRule rrule;
                rrule.parseFromString("FREQ=DAILY;INTERVAL=1;UNTIL=20210806T000000");
                REQUIRE(rrule.freq == RRule::RRuleFrequency::DAILY_RECURRENCE);
                REQUIRE(rrule.count == 0);
                REQUIRE(rrule.until == date::sys_days{date::August / 6 / 2021});
                REQUIRE(rrule.interval == 1);
                REQUIRE(rrule.weekStart == RRule::RRuleWeekday::MONDAY_WEEKDAY);
                REQUIRE((rrule.bySecond.empty() && rrule.byMinute.empty() && rrule.byHour.empty() &&
                         rrule.byDay.empty() && rrule.byMonthDay.empty() && rrule.byYearDay.empty() &&
                         rrule.byMonth.empty() && rrule.byWeekNo.empty() && rrule.bySetPos.empty()));
            }
            SECTION("Complex rule")
            {
                RRule rrule;
                rrule.parseFromString("FREQ=MONTHLY;BYSETPOS=4;BYDAY=TU;INTERVAL=2;COUNT=10");
                REQUIRE(rrule.freq == RRule::RRuleFrequency::MONTHLY_RECURRENCE);
                REQUIRE(rrule.count == 10);
                REQUIRE(rrule.until == TimePoint::min());
                REQUIRE(rrule.interval == 2);
                REQUIRE(rrule.weekStart == RRule::RRuleWeekday::MONDAY_WEEKDAY);
                REQUIRE(rrule.byDay.size() == 1);
                REQUIRE(rrule.byDay[0] == 3);
                REQUIRE(rrule.bySetPos.size() == 1);
                REQUIRE(rrule.bySetPos[0] == 4);
                REQUIRE((rrule.bySecond.empty() && rrule.byMinute.empty() && rrule.byHour.empty() &&
                         rrule.byMonthDay.empty() && rrule.byYearDay.empty() && rrule.byMonth.empty() &&
                         rrule.byWeekNo.empty()));
            }
        }

        SECTION("ParseString")
        {
            SECTION("Basic")
            {
                RRule rrule;
                auto teststring{""};

                teststring = "FREQ=DAILY";
                rrule.parseFromString(teststring);
                REQUIRE(rrule.parseToString() == teststring);

                teststring = "FREQ=MONTHLY;BYMONTHDAY=1";
                rrule.parseFromString(teststring);
                REQUIRE(rrule.parseToString() == teststring);
            }

            SECTION("String simplification")
            {
                RRule rrule;
                auto teststring{""};

                teststring = "FREQ=DAILY;INTERVAL=1";
                rrule.parseFromString(teststring);
                REQUIRE(rrule.parseToString() == "FREQ=DAILY");

                teststring = "FREQ=MONTHLY;BYMONTHDAY=1;INTERVAL=1";
                rrule.parseFromString(teststring);
                REQUIRE(rrule.parseToString() == "FREQ=MONTHLY;BYMONTHDAY=1");
            }
            SECTION("Until")
            {
                RRule rrule;
                const auto teststring = "FREQ=DAILY;UNTIL=20210806T000000";

                rrule.parseFromString(teststring);
                REQUIRE(rrule.parseToString() == "FREQ=DAILY;UNTIL=20210806T000000");
            }
            SECTION("Complex rule")
            {
                RRule rrule;
                auto teststring{""};

                teststring = "FREQ=MONTHLY;INTERVAL=2;BYDAY=TU;BYSETPOS=4;COUNT=10";
                rrule.parseFromString(teststring);
                REQUIRE(rrule.parseToString() == teststring);

                teststring = "FREQ=YEARLY;BYMONTH=4;BYDAY=TH;BYSETPOS=1;COUNT=10";
                rrule.parseFromString(teststring);
                REQUIRE(rrule.parseToString() == teststring);
            }
        }

        SECTION("Generate timestamps")
        {
            RRule rrule;
            TimePoint start         = TimePointFromString("2020-01-01 12:00:00");
            TimePoint end           = TimePointFromString("2020-02-01 12:00:00");
            const auto GENERATE_ALL = 99999;

            SECTION("Basic daily")
            {
                const auto teststring = "FREQ=DAILY";
                start                 = TimePointFromString("2020-01-01 12:00:00");
                end                   = TimePointFromString("2020-02-01 12:00:00");
                rrule.parseFromString(teststring);

                auto timestamps = rrule.generateEventTimePoints(start, end, GENERATE_ALL);

                REQUIRE(timestamps.size() == 31);
                REQUIRE(timestamps[0] == TimePointFromString("2020-01-01 12:00:00"));
                REQUIRE(timestamps[1] == TimePointFromString("2020-01-02 12:00:00"));
                REQUIRE(timestamps[30] == TimePointFromString("2020-01-31 12:00:00"));
            }
            SECTION("Basic hourly interval count")
            {
                const auto teststring = "FREQ=HOURLY;INTERVAL=2;COUNT=10";
                start                 = TimePointFromString("2020-01-01 12:00:00");
                end                   = TimePointFromString("2020-02-01 12:00:00");
                rrule.parseFromString(teststring);

                auto timestamps = rrule.generateEventTimePoints(start, end, GENERATE_ALL);

                REQUIRE(timestamps.size() == 10);
                REQUIRE(timestamps[0] == TimePointFromString("2020-01-01 12:00:00"));
                REQUIRE(timestamps[1] == TimePointFromString("2020-01-01 14:00:00"));
                REQUIRE(timestamps[9] == TimePointFromString("2020-01-02 06:00:00"));
            }
        }
    }
} // namespace rrule
