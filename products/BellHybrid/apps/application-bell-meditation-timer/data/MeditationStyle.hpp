// Copyright (c) 2017-2024, Mudita Sp. z.o.o. All rights reserved.
// For licensing, see https://github.com/mudita/MuditaOS/LICENSE.md

#pragma once

#include <Style.hpp>

namespace app::meditationStyle
{
    namespace runningStyle
    {
        namespace progress
        {
            inline constexpr auto radius                   = 192;
            inline constexpr auto penWidth                 = 3;
            inline constexpr auto verticalDeviationDegrees = 38;
        } // namespace progress

        namespace timer
        {
            inline constexpr auto marginTop = 39;
            inline constexpr auto font      = style::window::font::supersizeme;
            inline constexpr auto maxSizeX  = 340;
            inline constexpr auto maxSizeY  = 198;
        } // namespace timer

        namespace pauseIcon
        {
            inline constexpr auto image     = "big_pause";
            inline constexpr auto marginTop = 39;
            inline constexpr auto maxSizeX  = 203;
            inline constexpr auto maxSizeY  = 203;
        } // namespace pauseIcon

        namespace clock
        {
            inline constexpr auto marginTop = 17;
            inline constexpr auto maxSizeX  = 340;
            inline constexpr auto maxSizeY  = 84;
        } // namespace clock
    }     // namespace runningStyle

    namespace countdownStyle
    {
        namespace progress
        {
            inline constexpr auto radius                   = runningStyle::progress::radius;
            inline constexpr auto penWidth                 = 9;
            inline constexpr auto verticalDeviationDegrees = runningStyle::progress::verticalDeviationDegrees;
        } // namespace progress

        namespace timer
        {
            inline constexpr auto marginTop  = 23;
            inline constexpr auto marginLeft = 43;
            inline constexpr auto font       = runningStyle::timer::font;
            inline constexpr auto maxSizeX   = runningStyle::timer::maxSizeX;
            inline constexpr auto maxSizeY   = runningStyle::timer::maxSizeY;
        } // namespace timer

        namespace description
        {
            inline constexpr auto font      = style::window::font::large;
            inline constexpr auto marginTop = 63;
            inline constexpr auto maxSizeX  = 380;
            inline constexpr auto maxSizeY  = 102;
        } // namespace description
    }     // namespace countdownStyle

    namespace timerStyle
    {
        namespace text
        {
            inline constexpr auto font = style::window::font::supersizemelight;
        }

        namespace minute
        {
            inline constexpr auto font = style::window::font::largelight;
        }
    } // namespace timerStyle
} // namespace app::meditationStyle
