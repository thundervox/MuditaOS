// Copyright (c) 2017-2021, Mudita Sp. z.o.o. All rights reserved.
// For licensing, see https://github.com/mudita/MuditaOS/LICENSE.md

#pragma once

#include <gui/widgets/Style.hpp>

namespace style::meditation
{
    constexpr auto VerticalWidgetGap = 20;

    namespace timer
    {
        constexpr auto Radius = 142;
        constexpr auto X      = style::window_width / 2 - Radius;
        constexpr auto Y      = style::window::default_vertical_pos + 18;

        constexpr auto BorderColor   = gui::ColorFullBlack;
        constexpr auto PenWidth      = 1;
        constexpr auto FocusPenWidth = 3;

        namespace setterValueLabel
        {
            constexpr auto Width           = 140;
            constexpr auto Height          = 96;
            constexpr auto TopMargin       = 80;
            constexpr auto BottomLineWidth = 100;
            constexpr auto BottomLinePen   = 2;
        } // namespace setterValueLabel

        namespace setterUnitLabel
        {
            constexpr auto Width     = 220;
            constexpr auto Height    = 20;
            constexpr auto TopMargin = 15;
        } // namespace setterUnitLabel

        namespace infoText
        {
            constexpr auto X      = style::window::default_left_margin;
            constexpr auto Y      = 0;
            constexpr auto Width  = style::window::default_body_width;
            constexpr auto Height = style::window_height;
        } // namespace infoText

    } // namespace timer

    namespace intervalBox
    {
        constexpr auto X      = style::window::default_left_margin;
        constexpr auto Y      = style::meditation::timer::Y + 2 * style::meditation::timer::Radius + VerticalWidgetGap;
        constexpr auto Width  = style::window::default_body_width;
        constexpr auto Height = 60;

        namespace topLabel
        {
            constexpr auto X      = 0;
            constexpr auto Y      = 0;
            constexpr auto Width  = intervalBox::Width;
            constexpr auto Height = intervalBox::Height / 2;
        } // namespace topLabel

        namespace bottomLabel
        {
            constexpr auto X      = 0;
            constexpr auto Y      = topLabel::Y + topLabel::Height;
            constexpr auto Width  = intervalBox::Width;
            constexpr auto Height = intervalBox::Height / 2;
        } // namespace bottomLabel

        namespace arrow
        {
            constexpr auto Y      = 10;
            constexpr auto Width  = 15;
            constexpr auto Height = intervalBox::Height / 2;
            constexpr auto LeftX  = 0;
            constexpr auto RightX = intervalBox::Width - arrow::Width;
        } // namespace arrow

    } // namespace intervalBox

    namespace itemList
    {
        namespace text
        {
            constexpr auto X      = 10;
            constexpr auto Y      = 0;
            constexpr auto Width  = style::window::default_body_width;
            constexpr auto Height = style::window::label::big_h;
        } // namespace text

        namespace image
        {
            constexpr auto X      = style::window::default_body_width - 50;
            constexpr auto Y      = 12;
            constexpr auto OnOffY = 15;
            constexpr auto Width  = 0;
            constexpr auto Height = 0;
        } // namespace image

    } // namespace itemList

    namespace listView
    {
        namespace window
        {
            constexpr auto X      = style::window::default_left_margin;
            constexpr auto Y      = style::window::default_vertical_pos;
            constexpr auto Width  = style::listview::body_width_with_scroll;
            constexpr auto Height = style::window_height - Y - style::footer::height;
        } // namespace window

    } // namespace listView

} // namespace style::meditation
