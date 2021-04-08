// Copyright (c) 2017-2021, Mudita Sp. z.o.o. All rights reserved.
// For licensing, see https://github.com/mudita/MuditaOS/LICENSE.md

#include <service-appmgr/data/NotificationsChangedActionsParams.hpp>

#include <module-apps/notifications/NotificationData.hpp>

using namespace app::manager::actions;

NotificationsChangedParams::NotificationsChangedParams(Notifications notifications)
    : notifications{std::move(notifications)}
{}

auto NotificationsChangedParams::getNotifications() const noexcept -> const Notifications &
{
    return notifications;
}
