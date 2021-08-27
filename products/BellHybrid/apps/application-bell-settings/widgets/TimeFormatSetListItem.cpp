// Copyright (c) 2017-2021, Mudita Sp. z.o.o. All rights reserved.
// For licensing, see https://github.com/mudita/MuditaOS/LICENSE.md

#include "TimeFormatSetListItem.hpp"
#include "BellSettingsStyle.hpp"

#include <gui/core/FontManager.hpp>
#include <gui/core/RawFont.hpp>
#include <gui/widgets/Label.hpp>
#include <gui/widgets/Spinner.hpp>

#include <widgets/TimeSetFmtSpinner.hpp>

namespace
{
    constexpr auto fmtSpinnerMin  = 12U;
    constexpr auto fmtSpinnerMax  = 24U;
    constexpr auto fmtSpinnerStep = 12U;
} // namespace

namespace gui
{
    TimeFormatSetListItem::TimeFormatSetListItem(
        Length x, Length y, Length w, Length h, const UTF8 &topDesc, const UTF8 &botDesc)
        : BellSideListItem(topDesc)
    {
        setMinimumSize(style::sidelistview::list_item::w, style::sidelistview::list_item::h);
        setEdges(RectangleEdge::None);
        setFocusItem(body);

        timeFormat = new Spinner(fmtSpinnerMin, fmtSpinnerMax, fmtSpinnerStep, Boundaries::Continuous);
        timeFormat->setMaximumSize(style::bell_base_layout::w, style::bell_base_layout::h);
        timeFormat->setFont(bell_settings_style::time_fmt_set_list_item::font);

        timeFormat->setAlignment(Alignment(Alignment::Horizontal::Center, Alignment::Vertical::Center));
        timeFormat->setFixedFieldWidth(2);
        timeFormat->setEdges(RectangleEdge::None);
        timeFormat->setCurrentValue(fmtSpinnerMin);
        timeFormat->setFocusEdges(RectangleEdge::None);
        body->centerBox->addWidget(timeFormat);

        bottomDescription = new Label(body->lastBox);
        bottomDescription->setMaximumSize(style::bell_base_layout::w, style::bell_base_layout::outer_layouts_h);
        bottomDescription->setFont(style::bell_sidelist_item::description_font);
        bottomDescription->setEdges(RectangleEdge::None);
        bottomDescription->activeItem = false;
        bottomDescription->setAlignment(Alignment(Alignment::Horizontal::Center, Alignment::Vertical::Center));
        bottomDescription->setText(botDesc);

        dimensionChangedCallback = [&](Item &, const BoundingBox &newDim) -> bool {
            body->setArea({0, 0, newDim.w, newDim.h});
            return true;
        };

        focusChangedCallback = [&](Item &) {
            setFocusItem(focus ? body : nullptr);
            if (focus) {
                setFocusItem(body);
            }
            else {
                setFocusItem(nullptr);
                if (onNextCallback) {
                    onNextCallback(*this);
                }
            }
            return true;
        };

        inputCallback = [&](Item &, const InputEvent &inputEvent) -> bool { return body->onInput(inputEvent); };
    }

    auto TimeFormatSetListItem::getTimeFmt() const noexcept -> utils::time::Locale::TimeFormat
    {
        return timeFormat->getCurrentValue() == fmtSpinnerMin ? utils::time::Locale::TimeFormat::FormatTime12H
                                                              : utils::time::Locale::TimeFormat::FormatTime24H;
    }

    auto TimeFormatSetListItem::setTimeFmt(utils::time::Locale::TimeFormat fmt) noexcept -> void
    {
        using namespace utils::time;
        if (fmt == Locale::TimeFormat::FormatTime12H) {
            timeFormat->setCurrentValue(fmtSpinnerMin);
        }
        else if (fmt == Locale::TimeFormat::FormatTime24H) {
            timeFormat->setCurrentValue(fmtSpinnerMax);
        }
    }
} // namespace gui