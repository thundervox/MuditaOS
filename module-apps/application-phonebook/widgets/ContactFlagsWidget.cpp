// Copyright (c) 2017-2020, Mudita Sp. z.o.o. All rights reserved.
// For licensing, see https://github.com/mudita/MuditaOS/LICENSE.md

#include "ContactFlagsWidget.hpp"

#include "ContactFlagIconWidget.hpp"
#include "ContactFlagsStyle.hpp"

#include <Alignment.hpp>
#include <BoxLayout.hpp>
#include <i18/i18.hpp>
#include <tools/Common.hpp>
#include <Style.hpp>

namespace gui
{

    const unsigned char ContactFlagsWidget::iconsCount = 4;
    ContactFlagsWidget::ContactFlagsWidget(Item *parent)
        : Rect(parent, 0, style::header::height, style::window_width, style::widget::ContactFlag::height)
    {
        favourites = speedDial = ice = true;
        blocked                      = false;
        setEdges(RectangleEdge::Bottom);
        initGUIIcons();
        buildWidget();
        repositionIcons();
    }

    void ContactFlagsWidget::setFavourites(bool isEnabled)
    {
        if (favourites != isEnabled) {
            favourites = isEnabled;
            repositionIcons();
        }
    }

    void ContactFlagsWidget::setSpeedDial(bool isEnabled, unsigned char position)
    {
        if (speedDial != isEnabled) {
            speedDial = isEnabled;
            repositionIcons();
        }
        if (speedDialPos != position) {
            speedDialPos = position;
            speedDialIcon->setIconNumber(speedDialPos);
        }
    }

    void ContactFlagsWidget::setICE(bool isEnabled)
    {
        if (ice != isEnabled) {
            ice = isEnabled;
            repositionIcons();
        }
    }

    void ContactFlagsWidget::setBlocked(bool isBlocked)
    {
        if (blocked != isBlocked) {
            blocked = isBlocked;
            repositionIcons();
        }
    }

    void ContactFlagsWidget::initGUIIcons()
    {
        favouritesIcon = new ContactFlagIconWidget("small_heart_W_M", utils::localize.get("FAVOURITES"), this);
        speedDialIcon  = new ContactFlagIconWidget(speedDialPos, utils::localize.get("SPEED DIAL"), this);
        iceIcon        = new ContactFlagIconWidget("ice", utils::localize.get("ICE"), this);
        blockedIcon    = new ContactFlagIconWidget("block", utils::localize.get("BLOCKED"), this);
    }

    void ContactFlagsWidget::buildWidget()
    {
        mainBox = new HBox(this, 0, 0, style::window_width, style::widget::ContactFlag::itemHeight);
        mainBox->setAlignment(Alignment(Alignment::Horizontal::Center));
        mainBox->setEdges(RectangleEdge::None);
        mainBox->addWidget(favouritesIcon);
        mainBox->addWidget(iceIcon);
        mainBox->addWidget(speedDialIcon);
        mainBox->addWidget(blockedIcon);
    }

    void ContactFlagsWidget::repositionIcons()
    {
        favouritesIcon->setVisible(false);
        speedDialIcon->setVisible(false);
        iceIcon->setVisible(false);
        blockedIcon->setVisible(false);

        if (blocked) {
            blockedIcon->setVisible(true);
        }
        else {
            if (favourites) {
                favouritesIcon->setVisible(true);
            }

            if (ice) {
                iceIcon->setVisible(true);
            }

            if (speedDial) {
                speedDialIcon->setVisible(true);
            }

        }
        hideIfNoFlags();
        mainBox->resizeItems();
    }

    void ContactFlagsWidget::hideIfNoFlags()
    {
        if (blocked || favourites || ice || speedDial) {
            setVisible(true);
        }
        else {
            setVisible(false);
        }
    }

} // namespace gui
