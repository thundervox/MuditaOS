// Copyright (c) 2017-2024, Mudita Sp. z.o.o. All rights reserved.
// For licensing, see https://github.com/mudita/MuditaOS/LICENSE.md

#pragma once

#include <apps-common/popups/data/PopupRequestParams.hpp>

class RebootPopupRequestParams : public gui::PopupRequestParams
{
  public:
    RebootPopupRequestParams() : PopupRequestParams{gui::popup::ID::Reboot}
    {}
};
