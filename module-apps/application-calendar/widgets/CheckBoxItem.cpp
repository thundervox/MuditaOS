#include "CheckBoxItem.hpp"
#include "application-calendar/widgets/CalendarStyle.hpp"
#include "application-calendar/windows/NewEditEventWindow.hpp"
#include "windows/AppWindow.hpp"
#include <ListView.hpp>
#include <Style.hpp>
#include <time/time_conversion.hpp>

namespace gui
{

    CheckBoxItem::CheckBoxItem(app::Application *application, const std::string &description) : app(application)
    {
        app = application;
        assert(app != nullptr);
        setMinimumSize(style::window::default_body_width, style::window::calendar::item::checkBox::height);
        // setMaximumSize(style::window::default_body_width, style::window::calendar::item::checkBox::height);

        setEdges(RectangleEdgeFlags::GUI_RECT_EDGE_NO_EDGES);

        hBox = new gui::HBox(this, 0, 0, 0, 0);
        hBox->setEdges(gui::RectangleEdgeFlags::GUI_RECT_EDGE_NO_EDGES);
        hBox->setPenFocusWidth(style::window::default_border_focus_w);
        hBox->setPenWidth(style::window::default_border_rect_no_focus);

        checkBox = new gui::CheckBox(
            hBox,
            0,
            0,
            0,
            0,
            new Image("small_tick"),
            [=](const UTF8 &text) {
                app->getCurrentWindow()->bottomBarTemporaryMode(text, BottomBar::Side::LEFT, false);
            },
            [=]() { app->getCurrentWindow()->bottomBarRestoreFromTemporaryMode(); });
        checkBox->setMinimumSize(50, 60);
        // checkBox->setMargins(gui::Margins(0, 35, 0, 0));
        checkBox->setAlignment(Alignment(gui::Alignment::Horizontal::Center, gui::Alignment::Vertical::Center));
        checkBox->activeItem = false;

        descriptionLabel = new gui::Label(hBox, 0, 0, 0, 0);
        descriptionLabel->setMinimumSize(style::window::calendar::item::checkBox::description_label_w,
                                         style::window::calendar::item::checkBox::description_label_h);
        descriptionLabel->setMargins(gui::Margins(15, 0, 0, 0));
        descriptionLabel->setEdges(gui::RectangleEdgeFlags::GUI_RECT_EDGE_NO_EDGES);
        descriptionLabel->setAlignment(Alignment(gui::Alignment::Horizontal::Left, gui::Alignment::Vertical::Center));
        descriptionLabel->setFont(style::window::font::medium);
        descriptionLabel->activeItem = false;

        descriptionLabel->setText(description);

        focusChangedCallback = [&](Item &item) {
            if (focus) {
                descriptionLabel->setFont(style::window::font::mediumbold);
                setFocusItem(checkBox);
            }
            else {
                descriptionLabel->setFont(style::window::font::medium);
                setFocusItem(nullptr);
            }
            return true;
        };

        inputCallback = [&](gui::Item &item, const gui::InputEvent &event) {
            if (event.state != gui::InputEvent::State::keyReleasedShort) {
                return false;
            }
            if (event.keyCode == gui::KeyCode::KEY_LEFT) {
                LOG_DEBUG("Temporary bottomBar check");
                app->getCurrentWindow()->bottomBarTemporaryMode("MHM", BottomBar::Side::LEFT, false);
                return true;
            }

            return checkBox->onInput(event);
        };
    }

    bool CheckBoxItem::onDimensionChanged(const BoundingBox &oldDim, const BoundingBox &newDim)
    {
        hBox->setPosition(0, 0);
        hBox->setSize(newDim.w, newDim.h);
        LOG_DEBUG("SIZE: %i,  %i", newDim.w, newDim.h);

        // descriptionLabel->setArea(BoundingBox(style::window::calendar::item::checkBox::description_label_x,
        //                                      newDim.h - style::window::calendar::item::checkBox::description_label_h,
        //                                      style::window::calendar::item::checkBox::description_label_w,
        //                                     style::window::calendar::item::checkBox::description_label_h));

        return true;
    }

} /* namespace gui */
