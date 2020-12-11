// Copyright (c) 2017-2020, Mudita Sp. z.o.o. All rights reserved.
// For licensing, see https://github.com/mudita/MuditaOS/LICENSE.md

#pragma once

#include "application-alarm-clock/ApplicationAlarmClock.hpp"
#include "application-alarm-clock/models/AlarmsRepository.hpp"
#include <module-db/Interface/AlarmsRecord.hpp>

namespace gui
{
    class Option; // Fw declaration
    class Text;   // Fw declaration
} // namespace gui

namespace app::alarmClock
{
    std::list<gui::Option> alarmsListOptions(Application *application,
                                             const AlarmsRecord &record,
                                             AbstractAlarmsRepository &alarmsRepository);
}
