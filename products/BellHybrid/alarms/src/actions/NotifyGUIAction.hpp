// Copyright (c) 2017-2024, Mudita Sp. z.o.o. All rights reserved.
// For licensing, see https://github.com/mudita/MuditaOS/LICENSE.md

#pragma once

#include "AbstractAlarmAction.hpp"

namespace sys
{
    class Service;
}

namespace alarms
{
    class NotifyGUIAction : public AbstractAlarmAction
    {
      public:
        explicit NotifyGUIAction(sys::Service &service);
        bool execute() override;
        bool turnOff() override;

      private:
        sys::Service &service;
    };
} // namespace alarms
