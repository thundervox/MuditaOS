// Copyright (c) 2017-2020, Mudita Sp. z.o.o. All rights reserved.
// For licensing, see https://github.com/mudita/MuditaOS/LICENSE.md

#pragma once

#include <string> // for string

#include "Endpoint.hpp" // for Endpoint
#include "Service/Service.hpp"
#include "ParserUtils.hpp" // for parserFSM

namespace parserFSM
{
    class Context;
} // namespace parserFSM
namespace sys
{
    class Service;
} // namespace sys

using namespace parserFSM;

class DeviceInfoEndpoint : public Endpoint
{

  public:
    DeviceInfoEndpoint(sys::Service *ownerServicePtr) : Endpoint(ownerServicePtr)
    {
        debugName = "DeviceInfoEndpoint";
    }
    auto handle(Context &context) -> void override;
    auto getDeviceInfo(Context &context) -> bool;
};
