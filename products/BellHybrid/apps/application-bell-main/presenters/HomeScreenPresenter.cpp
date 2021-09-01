// Copyright (c) 2017-2021, Mudita Sp. z.o.o. All rights reserved.
// For licensing, see https://github.com/mudita/MuditaOS/LICENSE.md

#include "HomeScreenPresenter.hpp"
#include "StateController.hpp"
#include "models/AlarmModel.hpp"
#include "models/TemperatureModel.hpp"
#include "models/TimeModel.hpp"

#include <apps-common/Application.hpp>
#include <module-sys/Timers/SystemTimer.hpp>
#include <module-sys/Timers/TimerFactory.hpp>
#include <time/time_constants.hpp>
#include <time/time_conversion.hpp>

namespace
{
    constexpr auto defaultAlarmTimeHour = 7U;
    constexpr auto defaultAlarmTimeMin  = 0U;
} // namespace

namespace app::home_screen
{
    HomeScreenPresenter::HomeScreenPresenter(Application *app,
                                             std::unique_ptr<AbstractAlarmModel> alarmModel,
                                             std::unique_ptr<AbstractTemperatureModel> temperatureModel,
                                             std::unique_ptr<AbstractTimeModel> timeModel)
        : app{app}, alarmModel{std::move(alarmModel)},
          temperatureModel{std::move(temperatureModel)}, timeModel{std::move(timeModel)}
    {}

    void HomeScreenPresenter::handleUpdateTimeEvent()
    {
        getView()->setTime(timeModel->getCurrentTime());
        stateController->handleTimeUpdateEvent();
    }

    void HomeScreenPresenter::handleAlarmRingingEvent()
    {
        stateController->handleAlarmRingingEvent();
    }

    bool HomeScreenPresenter::handleInputEvent(const gui::InputEvent &inputEvent)
    {
        return stateController->handleInputEvent(inputEvent);
    }

    void HomeScreenPresenter::spawnTimer(std::chrono::milliseconds timeout)
    {
        if (not timer.isValid()) {
            auto callback = [this](sys::Timer &) { stateController->handleTimerEvent(); };
            timer         = sys::TimerFactory::createSingleShotTimer(app, timerName, defaultTimeout, callback);
        }
        timer.stop();
        timer.start();
    }

    void HomeScreenPresenter::detachTimer()
    {
        if (timer.isValid()) {
            timer.stop();
            timer.reset();
        }
    }

    HomeScreenPresenter::~HomeScreenPresenter()
    {
        detachTimer();
    }

    void HomeScreenPresenter::onBeforeShow()
    {
        getView()->setTimeFormat(timeModel->getTimeFormat());
        getView()->setTime(timeModel->getCurrentTime());
        getView()->setAlarmTimeFormat(timeModel->getTimeFormat());
        getView()->setTemperature(temperatureModel->getTemperature());
    }

    void HomeScreenPresenter::setDefaultAlarmTime()
    {
        const auto now     = timeModel->getCurrentTime();
        const auto newTime = std::localtime(&now);
        newTime->tm_hour   = defaultAlarmTimeHour;
        newTime->tm_min    = defaultAlarmTimeMin;
        auto alarmTime     = std::mktime(newTime);

        if (alarmTime < now) {
            alarmTime += utils::time::secondsInDay;
        }
        getView()->setAlarmTime(alarmTime);
    }

    void HomeScreenPresenter::createData()
    {
        setDefaultAlarmTime();
        stateController =
            std::make_unique<StateController>(*getView(), *this, *temperatureModel, *alarmModel, *timeModel);
    }

    void HomeScreenPresenter::refreshWindow()
    {
        app->refreshWindow(gui::RefreshModes::GUI_REFRESH_FAST);
    }
} // namespace app::home_screen
