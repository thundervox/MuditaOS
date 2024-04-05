// Copyright (c) 2017-2024, Mudita Sp. z.o.o. All rights reserved.
// For licensing, see https://github.com/mudita/MuditaOS/LICENSE.md

#include "FocusTimerPresenter.hpp"
#include "FocusCommon.hpp"
#include "models/FocusSettingsModel.hpp"
#include "Application.hpp"

#include <common/models/TimeModel.hpp>
#include <common/windows/BellFinishedWindow.hpp>
#include <common/LanguageUtils.hpp>

namespace
{
    constexpr std::chrono::seconds delayBetweenSessions{5};
    constexpr std::chrono::seconds summaryWindowTimeout{5};

    std::string createSummaryText(const std::string &str, const std::string &minutesOfFocus)
    {
        auto parser = gui::text::RichTextParser{};
        const auto result =
            parser.parse(str, nullptr, gui::text::RichTextParser::TokenMap({{"$VALUE", minutesOfFocus}}));
        return result->getText();
    }
} // namespace

namespace app::focus
{

    FocusTimerPresenter::FocusTimerPresenter(app::ApplicationCommon *app,
                                             models::FocusSettingsModel &focusTimeModel,
                                             models::FocusSettingsModel &focusRepeatsModel,
                                             models::FocusSettingsModel &shortBreakTimeModel,
                                             std::unique_ptr<AbstractTimeModel> timeModel,
                                             AbstractBatteryModel &batteryModel,
                                             AbstractLowBatteryInfoModel &lowBatteryInfoModel)
        : app{app}, focusTimeModel{focusTimeModel}, focusRepeatsModel{focusRepeatsModel},
          shortBreakTimeModel{shortBreakTimeModel}, batteryModel{batteryModel},
          lowBatteryInfoModel{lowBatteryInfoModel}, timeModel{std::move(timeModel)}
    {
        focusSessionDuration  = std::chrono::minutes{focusTimeModel.getValue()};
        shortBreakDuration    = std::chrono::minutes{shortBreakTimeModel.getValue()};
        allFocusSessionsCount = focusRepeatsModel.getValue();
        focusSessionsLeft     = allFocusSessionsCount;

        betweenSessionTimer = sys::TimerFactory::createSingleShotTimer(
            app, "betweenSessionTimer", delayBetweenSessions, [this](sys::Timer &) { executeNextStep(); });
    }

    void FocusTimerPresenter::setTimer(std::unique_ptr<app::TimerWithCallbacks> &&_timer)
    {
        timer = std::move(_timer);
        timer->registerOnFinishedCallback([this]() { executeNextStep(); });
    }

    void FocusTimerPresenter::handleUpdateTimeEvent()
    {
        getView()->setTime(timeModel->getCurrentTime());
    }

    void FocusTimerPresenter::start()
    {
        static_cast<app::Application *>(app)->suspendIdleTimer();
        startTime();
    }

    void FocusTimerPresenter::stop()
    {
        finish();
    }

    bool FocusTimerPresenter::isTimerStopped()
    {
        return timer->isStopped();
    }

    bool FocusTimerPresenter::isAllSessionsFinished()
    {
        return currentTimerPhase == FocusTimerPhase::AllFocusSessionsEnded;
    }

    void FocusTimerPresenter::pause()
    {
        betweenSessionTimer.stop();
        timer->stop();
        getView()->pause();
    }

    void FocusTimerPresenter::resume()
    {
        isMiddleTimeBetweenBreakAndFocus() ? executeNextStep() : timer->start();
        getView()->resume();
    }

    void FocusTimerPresenter::abandon()
    {
        finish();
    }

    void FocusTimerPresenter::finish()
    {
        timer->stop();
        betweenSessionTimer.stop();

        const auto elapsed        = std::chrono::duration_cast<std::chrono::minutes>(timer->getElapsed());
        const auto minutesInFocus = ((currentTimerPhase == FocusTimerPhase::FocusTime) ? elapsed.count() : 0) +
                                    (allFocusSessionsCount - focusSessionsLeft) * focusSessionDuration.count();
        const auto sumOfFocusTime =
            std::to_string(minutesInFocus) + " " + utils::language::getCorrectMinutesAccusativeForm(minutesInFocus);
        const auto textToComplete = utils::translate("app_bell_focus_timer_summary");
        const auto summaryText    = createSummaryText(textToComplete, sumOfFocusTime);

        app->switchWindow(
            gui::window::bell_finished::defaultName,
            gui::BellFinishedWindowData::Factory::create("big_namaste_W_G",
                                                         focus::window::name::main,
                                                         summaryText,
                                                         gui::BellFinishedWindowData::ExitBehaviour::SwitchWindow,
                                                         summaryWindowTimeout));
    }

    void FocusTimerPresenter::executeNextStep()
    {
        switch (currentTimerPhase) {
        case FocusTimerPhase::FocusTime:
            focusSessionsLeft--;
            currentTimerPhase = handleInfoAfterFocusPhase();
            startTime();
            break;

        case FocusTimerPhase::FocusTimeEnded:
            currentTimerPhase = handleCountdownAfterFocusPhase();
            startTime();
            break;

        case FocusTimerPhase::ShortBreakTime:
            currentTimerPhase = FocusTimerPhase::ShortBreakTimeEnded;
            getView()->showTimeForFocusInfo();
            startTime();
            break;

        case FocusTimerPhase::ShortBreakTimeEnded:
            currentTimerPhase = FocusTimerPhase::FocusTime;
            getView()->showFocusSessionCountdown();
            startTime();
            break;

        case FocusTimerPhase::AllFocusSessionsEnded:
            finish();
            break;
        }
    }

    void FocusTimerPresenter::onBeforeShow()
    {
        getView()->setTimeFormat(timeModel->getTimeFormat());
    }

    void FocusTimerPresenter::startTime()
    {
        switch (currentTimerPhase) {
        case FocusTimerPhase::FocusTime:
            timer->reset(focusSessionDuration);
            timer->start();
            break;

        case FocusTimerPhase::ShortBreakTime:
            timer->reset(shortBreakDuration);
            timer->start();
            break;

        case FocusTimerPhase::FocusTimeEnded:
        case FocusTimerPhase::ShortBreakTimeEnded:
        case FocusTimerPhase::AllFocusSessionsEnded:
            betweenSessionTimer.start();
            break;
        }
    }

    bool FocusTimerPresenter::isMiddleTimeBetweenBreakAndFocus()
    {
        switch (currentTimerPhase) {
        case FocusTimerPhase::FocusTimeEnded:
        case FocusTimerPhase::ShortBreakTimeEnded:
            return true;
        default:
            return false;
        }
    }

    FocusTimerPresenter::FocusTimerPhase FocusTimerPresenter::handleInfoAfterFocusPhase()
    {
        if (focusSessionsLeft == 0) {
            getView()->showEndOfAllSessionsInfo();
            return FocusTimerPhase::AllFocusSessionsEnded;
        }
        if (shortBreakDuration.count() > 0) {
            getView()->showTimeForBreakInfo();
        }
        else {
            getView()->showTimeForFocusInfo();
        }
        return FocusTimerPhase::FocusTimeEnded;
    }

    FocusTimerPresenter::FocusTimerPhase FocusTimerPresenter::handleCountdownAfterFocusPhase()
    {
        if (shortBreakDuration.count() > 0) {
            getView()->showShortBreakCountdown();
            return FocusTimerPhase::ShortBreakTime;
        }
        getView()->showFocusSessionCountdown();
        return FocusTimerPhase::FocusTime;
    }

} // namespace app::focus
