// Copyright (c) 2017-2024, Mudita Sp. z.o.o. All rights reserved.
// For licensing, see https://github.com/mudita/MuditaOS/LICENSE.md

#pragma once

#include "AbstractAlarmAction.hpp"

#include <Service/Service.hpp>

namespace alarms
{
    class IgnoreKeysAction : public AbstractAlarmAction
    {
      public:
        explicit IgnoreKeysAction(sys::Service &service);
        bool execute() override;
        bool turnOff() override;

      private:
        sys::Service &service;
    };

} // namespace alarms
