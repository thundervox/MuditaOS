// Copyright (c) 2017-2024, Mudita Sp. z.o.o. All rights reserved.
// For licensing, see https://github.com/mudita/MuditaOS/LICENSE.md

#pragma once

#include "presenter/RelaxationRunningLoopPresenter.hpp"
#include <data/RelaxationAudioData.hpp>

#include <AppWindow.hpp>

namespace gui
{
    class Text;
    class BellStatusClock;
    class BellBattery;

    class RelaxationRunningLoopWindow : public AppWindow, public app::relaxation::RelaxationRunningLoopContract::View
    {
      public:
        RelaxationRunningLoopWindow(
            app::ApplicationCommon *app,
            std::unique_ptr<app::relaxation::RelaxationRunningLoopContract::Presenter> &&presenter);

      private:
        std::unique_ptr<app::relaxation::RelaxationRunningLoopContract::Presenter> presenter;
        std::unique_ptr<RelaxationAudioContext> audioContext;

        gui::Text *title           = nullptr;
        gui::Text *bottomText      = nullptr;
        gui::BellStatusClock *time = nullptr;
        gui::BellBattery *battery  = nullptr;

        void buildInterface() override;
        void onBeforeShow(ShowMode mode, SwitchData *data) override;
        bool onInput(const InputEvent &inputEvent) override;
        void setTime(std::time_t newTime) override;
        void setTimeFormat(utils::time::Locale::TimeFormat fmt) override;
        RefreshModes updateTime() override;
        void onFinished() override;
        void onPaused() override;
        void resume() override;
        bool updateBatteryStatus() override;
        void handleError() override;
        void handleDeletedFile() override;

        void buildLayout();
        void configureTimer();
    };
} // namespace gui
