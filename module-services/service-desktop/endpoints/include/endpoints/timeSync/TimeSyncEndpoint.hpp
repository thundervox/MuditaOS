// Copyright (c) 2017-2023, Mudita Sp. z.o.o. All rights reserved.
// For licensing, see https://github.com/mudita/MuditaOS/LICENSE.md

#pragma once

#include <endpoints/Endpoint.hpp>
#include <Service/Service.hpp>

namespace sdesktop::endpoints
{
    class TimeSyncEndpoint : public Endpoint
    {
      public:
        explicit TimeSyncEndpoint(sys::Service *ownerServicePtr) : Endpoint(ownerServicePtr)
        {
            debugName = "TimeSyncEndpoint";
        }
        auto handle(Context &context) -> void override;

      private:
        auto sendTimeUpdateMessage(std::time_t timestamp) const -> bool;
        auto logReceivedDateTime(std::time_t timestamp) const noexcept -> void;
    };
} // namespace sdesktop::endpoints
