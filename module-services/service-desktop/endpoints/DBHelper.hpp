﻿// Copyright (c) 2017-2020, Mudita Sp. z.o.o. All rights reserved.
// For licensing, see https://github.com/mudita/MuditaOS/LICENSE.md

#pragma once

#include "Context.hpp"
#include "Endpoint.hpp"
#include <parser/ParserFSM.hpp>

#include <Common/Query.hpp>
#include <Service/Common.hpp>
#include <Service/Service.hpp>

namespace parserFSM
{

    class DBHelper
    {
      public:
        virtual auto createDBEntry(Context &context) -> sys::ReturnCodes     = 0;
        virtual auto requestDataFromDB(Context &context) -> sys::ReturnCodes = 0;
        virtual auto updateDBEntry(Context &context) -> sys::ReturnCodes     = 0;
        virtual auto deleteDBEntry(Context &context) -> sys::ReturnCodes     = 0;

        DBHelper(sys::Service *_ownerServicePtr) : ownerServicePtr(_ownerServicePtr){};
        virtual ~DBHelper() = default;

      protected:
        sys::Service *ownerServicePtr = nullptr;
    };
} // namespace parserFSM
