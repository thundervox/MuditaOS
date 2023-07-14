// Copyright (c) 2017-2023, Mudita Sp. z.o.o. All rights reserved.
// For licensing, see https://github.com/mudita/MuditaOS/LICENSE.md

#include <endpoints/timeSync/TimeSyncEndpoint.hpp>
#include <endpoints/JsonKeyNames.hpp>
#include <endpoints/message/Sender.hpp>
#include <service-time/Constants.hpp>
#include <service-time/service-time/TimeMessage.hpp>
#include <time.h>
#include <log/log.hpp>

namespace sdesktop::endpoints
{
    auto TimeSyncEndpoint::logReceivedDateTime(std::time_t timestamp) const noexcept -> void
    {
        const struct tm *t = localtime(&timestamp);
        LOG_INFO("Received time set request: %d-%02d-%02d %02d:%02d:%02d",
                 t->tm_year + 1900,
                 t->tm_mon + 1,
                 t->tm_mday,
                 t->tm_hour,
                 t->tm_min,
                 t->tm_sec);
    }

    auto TimeSyncEndpoint::sendTimeUpdateMessage(std::time_t timestamp) const -> bool
    {
        auto msg = std::make_shared<stm::message::TimeChangeRequestMessage>(timestamp);
        return ownerServicePtr->bus.sendUnicast(std::move(msg), service::name::service_time);
    }

    auto TimeSyncEndpoint::handle(Context &context) -> void
    {
        switch (context.getMethod()) {
        case http::Method::post: {
            const auto &body = context.getBody();
            if (!body[json::timeSync::timestamp].is_number()) {
                LOG_ERROR("Timestamp data type not a number!");
                context.setResponseStatus(http::Code::UnprocessableEntity);
                break;
            }

            const auto timestamp = static_cast<std::time_t>(body[json::timeSync::timestamp].number_value());
            logReceivedDateTime(timestamp);
            if (const auto success = sendTimeUpdateMessage(timestamp); !success) {
                LOG_ERROR("Failed to send time update message!");
                context.setResponseStatus(http::Code::InternalServerError);
                break;
            }

            context.setResponseStatus(http::Code::OK);
        } break;

        default:
            LOG_ERROR("Invalid method: %s!", magic_enum::enum_name(context.getMethod()).data());
            context.setResponseStatus(http::Code::BadRequest);
            break;
        }

        sender::putToSendQueue(context.createSimpleResponse());
    }
} // namespace sdesktop::endpoints
