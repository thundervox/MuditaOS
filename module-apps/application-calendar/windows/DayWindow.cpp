#include "DayWindow.hpp"

#include <gui/widgets/Window.hpp>
#include <gui/widgets/Label.hpp>
#include <gui/widgets/Item.hpp>
#include <gui/widgets/BoxLayout.hpp>
#include <gui/widgets/BottomBar.hpp>
#include <gui/widgets/TopBar.hpp>

#include <time/time_conversion.hpp>

namespace app
{

    DayWindow::DayWindow(app::Application *app, std::string name)
        : AppWindow(app, style::window::calendar::name::day_window),
          calendarEventsModel{std::make_shared<CalendarEventsModel>(this->application)}
    {
        buildInterface();
    }

    void DayWindow::rebuild()
    {
        buildInterface();
    }

    void DayWindow::buildInterface()
    {
        AppWindow::buildInterface();

        auto ttime = utils::time::Time();
        topBar->setActive(gui::TopBar::Elements::TIME, true);
        bottomBar->setActive(gui::BottomBar::Side::RIGHT, true);
        bottomBar->setText(gui::BottomBar::Side::RIGHT, utils::localize.get(style::strings::common::back));
        bottomBar->setText(gui::BottomBar::Side::CENTER, utils::localize.get(style::strings::common::open));

        setTitle(ttime.str("%d %B"));
        leftArrowImage = new gui::Image(
            this, style::window::calendar::arrow_x, style::window::calendar::arrow_y, 0, 0, "arrow_left");
        newDayEventImage =
            new gui::Image(this, style::window::calendar::cross_x, style::window::calendar::cross_y, 0, 0, "cross");

        dayEventsList = new gui::ListView(this,
                                          style::window::calendar::listView_x,
                                          style::window::calendar::listView_y,
                                          style::window::calendar::listView_w,
                                          style::window::calendar::listView_h,
                                          calendarEventsModel);

        dayEventsList->setPenFocusWidth(style::window::default_border_no_focus_w);
        dayEventsList->setPenWidth(style::window::default_border_no_focus_w);

        dayEventsList->focusChangedCallback = [=](gui::Item &) {
            bottomBar->setActive(gui::BottomBar::Side::CENTER, true);
            return true;
        };
        setFocusItem(dayEventsList);
    }

    bool DayWindow::onInput(const gui::InputEvent &inputEvent)
    {
        if (AppWindow::onInput(inputEvent)) {
            return true;
        }

        if (!inputEvent.isShortPress()) {
            return false;
        }

        if (inputEvent.keyCode == gui::KeyCode::KEY_LEFT) {
            LOG_DEBUG("TODO: Switch to new window - edit window");
            return true;
        }

        return false;
    }
} /* namespace app */
