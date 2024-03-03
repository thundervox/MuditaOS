// Copyright (c) 2017-2024, Mudita Sp. z.o.o. All rights reserved.
// For licensing, see https://github.com/mudita/MuditaOS/LICENSE.md

#pragma once

#include "data/RelaxationCommon.hpp"
#include "ListItem.hpp"
#include <utf8/UTF8.hpp>

namespace gui
{
    class RelaxationItem : public ListItem
    {
      private:
        app::relaxation::MusicType musicType{};

      public:
        explicit RelaxationItem(app::relaxation::MusicType musicType);
        virtual ~RelaxationItem() = default;
        app::relaxation::MusicType getMusicType();
    };

    class RelaxationMarkerItem : public ListItem
    {
      public:
        explicit RelaxationMarkerItem(const UTF8 &labelText);
        virtual ~RelaxationMarkerItem() = default;
    };
} // namespace gui
