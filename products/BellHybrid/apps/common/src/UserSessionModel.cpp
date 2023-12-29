// Copyright (c) 2017-2024, Mudita Sp. z.o.o. All rights reserved.
// For licensing, see https://github.com/mudita/MuditaOS/LICENSE.md

#include "models/UserSessionModel.hpp"
#include "log/log.hpp" //TODO: is it still needed?

namespace app
{

    UserSessionModel::UserSessionModel(sys::Service *serv) : serv{serv}
    {
        endSessionDelayTimer = sys::TimerFactory::createPeriodicTimer(
            serv, "EndOfUserSessionDelay", endOfUserSessionTimeout, [this](sys::Timer &) {
                endOfSessionTimerCallback();
            });
    }

    void UserSessionModel::activateUserSession() // TODO: maybe rename to: refresh... or activeOrRefresh...
    {
        LOG_WARN("  ***  "); // TODO: remove log
        if (sessionState == SessionState::INACTIVE) {
            LOG_INFO("User session begun");
            isCurrentUserSessionHandled = false;
        }
        sessionState = SessionState::ACTIVE;
        endSessionDelayTimer.stop();
    }

    void UserSessionModel::restartUserSession()
    {
        LOG_WARN("   ***   "); // TODO: remove log
        LOG_INFO("User session begun (restarted)");
        isCurrentUserSessionHandled = false;
        sessionState                = SessionState::ACTIVE;
        endSessionDelayTimer.stop();
    }

    void UserSessionModel::deactivateUserSessionWithDelay()
    {
        LOG_WARN("   ***   "); // TODO: remove log
        sessionState = SessionState::ENDING_WITH_DELAY;
        endSessionDelayTimer.restart(endOfUserSessionTimeout);
    }

    void UserSessionModel::setCurrentUserSessionAsHandled()
    {
        isCurrentUserSessionHandled = true;
    }

    bool UserSessionModel::isUserSessionActive()
    {
        LOG_WARN("   ***   "); // TODO: remove log
        return (sessionState != SessionState::INACTIVE);
    }

    bool UserSessionModel::isActiveUserSessionHandled()
    {
        LOG_WARN("   ***   "); // TODO: remove log
        auto retVal = isCurrentUserSessionHandled && isUserSessionActive();
        LOG_WARN("---------isActiveUserSessionHandled(): %d", static_cast<int>(retVal));
        return retVal;
    }

    void UserSessionModel::endOfSessionTimerCallback()
    {
        LOG_INFO("User session is ended");
        endSessionDelayTimer.stop();
        sessionState = SessionState::INACTIVE;
    }

} // namespace app
