// Copyright (c) 2017-2024, Mudita Sp. z.o.o. All rights reserved.
// For licensing, see https://github.com/mudita/MuditaOS/LICENSE.md

#pragma once

#include "presenter/AlarmActivatedPresenter.hpp"

#include <apps-common/popups/WindowWithTimer.hpp>

struct AlarmEventRecord;

namespace gui
{
    class Icon;

    class AlarmActivatedWindow : public WindowWithTimer, app::popup::AlarmActivatedContract::View
    {
      public:
        AlarmActivatedWindow(app::ApplicationCommon *app,
                             std::shared_ptr<app::popup::AlarmActivatedPresenter> presenter);
        void rebuild() override;

      private:
        bool onInput(const InputEvent &inputEvent) override;
        void buildInterface() override;
        void returnToPreviousWindow();
        void setAlarmTime(time_t alarmTime);
        void buildLowBatteryLayout();
        void buildAlarmInfoLayout();
        void lowBatteryInfoHandled();

        Icon *icon{};
    };
} /* namespace gui */
