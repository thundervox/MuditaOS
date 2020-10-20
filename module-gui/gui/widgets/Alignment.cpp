// Copyright (c) 2017-2020, Mudita Sp. z.o.o. All rights reserved.
// For licensing, see https://github.com/mudita/MuditaOS/LICENSE.md

#include "Alignment.hpp"

namespace gui
{

    Alignment::Alignment(Alignment::Horizontal valH, Alignment::Vertical valV) : horizontal(valH), vertical(valV)
    {}

    Alignment::Alignment(Alignment::Horizontal valH) : horizontal(valH)
    {}

    Alignment::Alignment(Alignment::Vertical valV) : vertical(valV)
    {}

    bool Alignment::operator==(const Alignment &alignment) const
    {
        return !(horizontal != alignment.horizontal || vertical != alignment.vertical);
    }

    bool Alignment::operator!=(const Alignment &alignment) const
    {
        return (horizontal != alignment.horizontal || vertical != alignment.vertical);
    }

    Position Alignment::calculateHAlignment(Length parentSize, Length childSize) const
    {
        switch (horizontal) {
        case Alignment::Horizontal::Left:
            return 0;
        case (Alignment::Horizontal::Center):
            return (parentSize - childSize) / 2;
        case Alignment::Horizontal::Right:
            return parentSize - childSize;
        default:
            return 0;
        }
    }

    Position Alignment::calculateVAlignment(Length parentSize, Length childSize) const
    {
        switch (vertical) {
        case Alignment::Vertical::Top:
            return 0;
        case (Alignment::Vertical::Center):
            return (parentSize - childSize) / 2;
        case Alignment::Vertical::Bottom:
            return parentSize - childSize;
        default:
            return 0;
        }
    }

} /* namespace gui */
