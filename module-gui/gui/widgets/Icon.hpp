// Copyright (c) 2017-2020, Mudita Sp. z.o.o. All rights reserved.
// For licensing, see https://github.com/mudita/MuditaOS/LICENSE.md

#pragma once

#include <Text.hpp>
#include <Image.hpp>

namespace gui
{
    class Icon : public Rect
    {
        Text *text = nullptr;

      public:
        Icon(Item *parent,
             const uint32_t &x,
             const uint32_t &y,
             const uint32_t &w,
             const uint32_t &h,
             const UTF8 imageName,
             const UTF8 &text);

        ~Icon() override = default;
    };

}; // namespace gui
