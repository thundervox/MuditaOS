// Copyright (c) 2017-2020, Mudita Sp. z.o.o. All rights reserved.
// For licensing, see https://github.com/mudita/MuditaOS/LICENSE.md

#pragma once

#include "Text.hpp"

namespace gui
{

    class TextFixedSize : public Text
    {
      protected:
        unsigned int linesCount   = style::text::maxTextLines;
        Position underlinePadding = 0;

        void drawLines() override;

      public:
        TextFixedSize(Item *parent, Position x, Position y, Length w, Length h);

        void setLines(const unsigned int val);
        void setUnderlinePadding(Position val);
    };

} // namespace gui
