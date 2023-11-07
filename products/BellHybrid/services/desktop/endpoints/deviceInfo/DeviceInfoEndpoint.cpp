// Copyright (c) 2017-2023, Mudita Sp. z.o.o. All rights reserved.
// For licensing, see https://github.com/mudita/MuditaOS/LICENSE.md

#include <endpoints/deviceInfo/DeviceInfoEndpoint.hpp>
#include <endpoints/message/Sender.hpp>
#include <endpoints/JsonKeyNames.hpp>

#include <EventStore.hpp>
#include <product/version.hpp>
#include <service-desktop/ServiceDesktop.hpp>

#include <cstdint>
#include <string>
#include <purefs/filesystem_paths.hpp>

#include <ctime>

namespace sdesktop::endpoints
{
    auto DeviceInfoEndpoint::getSerialNumber() -> std::string
    {
        return static_cast<ServiceDesktop *>(ownerServicePtr)->getSerialNumber();
    }

    auto DeviceInfoEndpoint::getCaseColour() -> std::string
    {
        return static_cast<ServiceDesktop *>(ownerServicePtr)->getCaseColour();
    }

    auto DeviceInfoEndpoint::getOnboardingState() -> OnboardingState
    {
        return static_cast<ServiceDesktop *>(ownerServicePtr)->getOnboardingState();
    }

    auto DeviceInfoEndpoint::getDeviceInfo(Context &context) -> http::Code
    {
        const auto [totalDeviceSpaceMiB, reservedSystemSpaceMiB, usedUserSpaceMiB] = getStorageInfo();

        context.setResponseBody(json11::Json::object(
            {{json::deviceInfo::batteryLevel, std::to_string(Store::Battery::get().level)},
             {json::deviceInfo::batteryState, std::to_string(static_cast<int>(Store::Battery::get().state))},
             {json::deviceInfo::deviceSpaceTotal, std::to_string(totalDeviceSpaceMiB)},
             {json::deviceInfo::systemReservedSpace, std::to_string(reservedSystemSpaceMiB)},
             {json::deviceInfo::usedUserSpace, std::to_string(usedUserSpaceMiB)},
             {json::deviceInfo::gitRevision, std::string(GIT_REV)},
             {json::deviceInfo::gitBranch, std::string(GIT_BRANCH)},
             {json::deviceInfo::currentRTCTime, std::to_string(static_cast<std::uint32_t>(std::time(nullptr)))},
             {json::deviceInfo::version, std::string(VERSION)},
             {json::deviceInfo::serialNumber, getSerialNumber()},
             {json::deviceInfo::caseColour, getCaseColour()},
             {json::deviceInfo::recoveryStatusFilePath,
              (purefs::dir::getTemporaryPath() / sdesktop::paths::recoveryStatusFilename).string()},
             {json::deviceInfo::updateFilePath,
              (purefs::dir::getTemporaryPath() / sdesktop::paths::updateFilename).string()},
             {json::deviceInfo::backupFilePath,
              (purefs::dir::getTemporaryPath() / sdesktop::paths::backupFilename).string()},
             {json::deviceInfo::syncFilePath,
              (purefs::dir::getTemporaryPath() / sdesktop::paths::syncFilename).string()},
             {json::deviceInfo::mtpPath, getMtpPath().string()},
             {json::deviceInfo::onboardingState, std::to_string(static_cast<int>(getOnboardingState()))}}));

        return http::Code::OK;
    }

} // namespace sdesktop::endpoints
