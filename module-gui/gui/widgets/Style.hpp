// Copyright (c) 2017-2021, Mudita Sp. z.o.o. All rights reserved.
// For licensing, see https://github.com/mudita/MuditaOS/LICENSE.md

#pragma once

#include <limits>
#include <gui/core/Color.hpp>
#include <gui/Common.hpp>
#include <Alignment.hpp>
#include <Margins.hpp>

namespace gui
{
    class Rect;
    class Label;
}; // namespace gui

/// one place to gather all common magical numbers from design
namespace style
{
    inline constexpr auto window_height = 600U;
    inline constexpr auto window_width  = 480U;
    namespace header
    {
        inline constexpr auto height = 105U;
        namespace status_bar
        {
            inline constexpr auto height = 46U;
            inline constexpr auto width  = 440U;
        }; // namespace status_bar
        namespace font
        {
            inline constexpr auto title = "gt_pressura_bold_32";
        }; // namespace font
    };     // namespace header

    namespace footer
    {
        inline constexpr auto height = 54U;
        namespace font
        {
            inline constexpr auto bold   = "gt_pressura_bold_24";
            inline constexpr auto medium = "gt_pressura_regular_24";
        }; // namespace font
    };     // namespace footer

    namespace window
    {
        inline constexpr auto default_left_margin  = 20U;
        inline constexpr auto default_right_margin = 20U;
        inline constexpr auto default_body_width =
            style::window_width - style::window::default_right_margin - style::window::default_left_margin;
        inline constexpr auto default_body_height =
            style::window_height - style::header::height - style::footer::height;
        inline constexpr auto default_border_focus_w       = 2U;
        inline constexpr auto default_border_rect_no_focus = 1U;
        inline constexpr auto default_border_no_focus_w    = 0U;
        inline constexpr auto default_rect_yaps            = 10U;
        namespace font
        {
            inline constexpr auto supersizemelight = "gt_pressura_light_90";
            inline constexpr auto largelight       = "gt_pressura_light_46";
            inline constexpr auto verybigbold      = "gt_pressura_bold_32";
            inline constexpr auto bigbold          = "gt_pressura_bold_30";
            inline constexpr auto big              = "gt_pressura_regular_30";
            inline constexpr auto biglight         = "gt_pressura_light_30";
            inline constexpr auto mediumbold       = "gt_pressura_bold_27";
            inline constexpr auto medium           = "gt_pressura_regular_27";
            inline constexpr auto mediumlight      = "gt_pressura_light_27";
            inline constexpr auto smallbold        = "gt_pressura_bold_24";
            inline constexpr auto small            = "gt_pressura_regular_24";
            inline constexpr auto verysmallbold    = "gt_pressura_bold_20";
            inline constexpr auto verysmall        = "gt_pressura_regular_20";
        }; // namespace font

        inline constexpr auto list_offset_default = 12U;
        namespace label
        {
            inline constexpr auto small_h   = 33U;
            inline constexpr auto default_h = 50U;
            inline constexpr auto big_h     = 55U;
        }; // namespace label

        /// minimal label decoration - edges, focus & alignment
        void decorate(gui::Rect *el);
        void decorate(gui::Label *el);
        /// minimal label decoration for Option
        void decorateOption(gui::Label *el);

        namespace bottomBar
        {
            inline constexpr auto leftMargin  = 30U;
            inline constexpr auto rightMargin = 30U;

            inline constexpr auto h = 54U;
            inline constexpr auto w = window_width;
        } // namespace bottomBar

        namespace progressBar
        {
            inline constexpr auto x     = 100U;
            inline constexpr auto y     = 400U;
            inline constexpr auto width = 280U;
            inline constexpr auto h     = 50U;
        }; // namespace progressBar
    };    // namespace window

    namespace settings
    {
        namespace date
        {
            inline constexpr auto date_time_item_height       = 107U;
            inline constexpr auto date_time_item_width        = 120U;
            inline constexpr auto date_time_item_title_height = 30U;
            inline constexpr auto date_time_spacer_width      = 20U;
            inline constexpr auto date_time_x_offset          = 30U;

            inline constexpr auto date_time_box_h = 107U;
            inline constexpr auto date_box_y_pos  = 123U;
            inline constexpr auto time_box_y_pos  = 285U;

        } // namespace date
        namespace ussd
        {
            inline constexpr auto commonXPos = 40U;
            inline constexpr auto commonYPos = 110U;

            inline constexpr auto commonW      = 420U;
            inline constexpr auto commonLabelH = 33U;
            inline constexpr auto commonTextH  = 99U;
        } // namespace ussd
    }     // namespace settings
    namespace color
    {
        inline constexpr auto lightgrey = gui::Color(3, 0);
    }; //  namespace color
    namespace text
    {
        inline constexpr auto defaultTextAlignment =
            gui::Alignment(gui::Alignment::Horizontal::Left, gui::Alignment::Vertical::Bottom);
        inline constexpr auto maxTextLines = 10;
    }; // namespace text

    namespace strings
    {
        namespace common
        {
            inline constexpr auto add            = "common_add";
            inline constexpr auto open           = "common_open";
            inline constexpr auto call           = "common_call";
            inline constexpr auto send           = "common_send";
            inline constexpr auto save           = "common_save";
            inline constexpr auto confirm        = "common_confirm";
            inline constexpr auto select         = "common_select";
            inline constexpr auto use            = "common_use";
            inline constexpr auto ok             = "common_ok";
            inline constexpr auto back           = "common_back";
            inline constexpr auto skip           = "common_skip";
            inline constexpr auto set            = "common_set";
            inline constexpr auto yes            = "common_yes";
            inline constexpr auto no             = "common_no";
            inline constexpr auto check          = "common_check";
            inline constexpr auto Switch         = "common_switch";
            inline constexpr auto options        = "common_options";
            inline constexpr auto information    = "common_information";
            inline constexpr auto search         = "common_search";
            inline constexpr auto search_results = "common_search_results";
            inline constexpr auto emoji          = "common_emoji";
            inline constexpr auto special_chars  = "common_special_characters";
            inline constexpr auto start          = "common_start";
            inline constexpr auto stop           = "common_stop";
            inline constexpr auto resume         = "common_resume";
            inline constexpr auto pause          = "common_pause";
            inline constexpr auto accept         = "common_accept";
            inline constexpr auto retry          = "common_retry";
            inline constexpr auto abort          = "common_abort";
            inline constexpr auto adjust         = "common_adjust";
            // days
            inline constexpr auto Monday    = "common_monday";
            inline constexpr auto Tuesday   = "common_tuesday";
            inline constexpr auto Wednesday = "common_wednesday";
            inline constexpr auto Thursday  = "common_thursday";
            inline constexpr auto Friday    = "common_friday";
            inline constexpr auto Saturday  = "common_saturday";
            inline constexpr auto Sunday    = "common_sunday";
            // months
            inline constexpr auto January   = "common_january";
            inline constexpr auto February  = "common_february";
            inline constexpr auto March     = "common_march";
            inline constexpr auto April     = "common_april";
            inline constexpr auto May       = "common_may";
            inline constexpr auto June      = "common_june";
            inline constexpr auto July      = "common_july";
            inline constexpr auto August    = "common_august";
            inline constexpr auto September = "common_september";
            inline constexpr auto October   = "common_october";
            inline constexpr auto November  = "common_november";
            inline constexpr auto December  = "common_december";
            inline constexpr auto Yesterday = "common_yesterday";
            inline constexpr auto Today     = "common_today";
        } // namespace common
    }     // namespace strings

    namespace listview
    {
        inline constexpr auto nPos = std::numeric_limits<unsigned int>::max();

        /// Possible List boundaries handling types
        enum class Boundaries
        {
            Fixed,     ///< Fixed - list will stop scrolling on first or last elements on appropriate top or bottom
                       ///< directions.
            Continuous ///< Continuous - list will continue to beginning or end on first or last elements on
                       ///< appropriate top or bottom directions.
        };

        /// Possible List scrolling directions
        enum class Direction
        {
            Top,
            Bottom
        };

        /// Possible List rebuild types
        enum class RebuildType
        {
            Full,          ///< Full rebuild - resets lists to all initial conditions and request data from beginning.
            InPlace,       ///< InPlace rebuild - stores currently focused part of list and rebuild from that part.
            OnPageElement, ///< OnPageElement rebuild - same page but focus changed on provided element index.
            OnOffset       ///< OnOffset rebuild - resets lists to all initial conditions and request data from provided
                           ///< offset.
        };

        /// Possible List ScrollBar types
        enum class ScrollBarType
        {
            None,         ///< None - list without scroll bar (but with scrolling).
            Proportional, ///< Proportional - scroll bar size calculated based on elements count in model and currently
                          ///< displayed number of elements. Use with large unequal heights lists elements.
            Fixed,        ///< Fixed - scroll bar size calculated based on fixed equal elements sizes in list.
                          ///< Use when all elements have equal heights.
            PreRendered   ///< PreRendered - scroll bar size calculated based on pre rendered pages on whole list. Use
                          ///< when elements are not equal heights but there are few of them as its renders whole
                          ///< context and can be time consuming.
        };

        enum class Orientation
        {
            TopBottom,
            BottomTop
        };

        namespace scroll
        {
            inline constexpr auto x           = 0U;
            inline constexpr auto y           = 0U;
            inline constexpr auto w           = 5U;
            inline constexpr auto h           = 0U;
            inline constexpr auto radius      = 2U;
            inline constexpr auto color       = gui::Color{0, 0};
            inline constexpr auto margin      = 5U;
            inline constexpr auto min_space   = 10U;
            inline constexpr auto item_margin = 10U;
        } // namespace scroll

        inline constexpr auto item_width_with_scroll =
            style::window::default_body_width - style::listview::scroll::item_margin;
        inline constexpr auto body_width_with_scroll =
            style::window::default_body_width + style::listview::scroll::margin;

    } // namespace listview

    namespace margins
    {
        inline constexpr auto small    = 6U;
        inline constexpr auto big      = 8U;
        inline constexpr auto very_big = 12U;
        inline constexpr auto huge     = 24U;
    } // namespace margins

    namespace padding
    {
        inline constexpr auto default_left_text_padding = 10U;
    } // namespace padding

    namespace widgets
    {
        inline constexpr auto h           = 55U;
        inline constexpr auto iconsSize   = h;
        inline constexpr auto leftMargin  = 10U;
        inline constexpr auto rightMargin = 10U;
    } // namespace widgets

}; // namespace style
