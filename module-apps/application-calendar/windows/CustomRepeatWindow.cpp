#include "CustomRepeatWindow.hpp"
#include <gui/widgets/Window.hpp>
#include <gui/widgets/BoxLayout.hpp>
#include <gui/widgets/BottomBar.hpp>
#include <gui/widgets/TopBar.hpp>
#include <Utils.hpp>

namespace gui
{

    CustomRepeatWindow::CustomRepeatWindow(app::Application *app, std::string name)
        : AppWindow(app, style::window::calendar::name::custom_repeat_window),
          customRepeatModel{std::make_shared<CustomRepeatModel>(this->application)}
    {
        buildInterface();
    }

    void CustomRepeatWindow::rebuild()
    {
        AppWindow::destroyInterface();
        buildInterface();
    }

    void CustomRepeatWindow::buildInterface()
    {
        AppWindow::buildInterface();

        topBar->setActive(gui::TopBar::Elements::TIME, true);
        bottomBar->setActive(gui::BottomBar::Side::RIGHT, true);
        bottomBar->setText(gui::BottomBar::Side::RIGHT, utils::localize.get(style::strings::common::back));

        setTitle(utils::localize.get("app_calendar_custom_repeat_title"));
        list = new gui::ListView(this,
                                 style::window::calendar::listView_x,
                                 style::window::calendar::listView_y - 8,
                                 style::window::calendar::listView_w,
                                 style::window::calendar::listView_h,
                                 customRepeatModel);
        list->setPenFocusWidth(style::window::default_border_no_focus_w);
        list->setPenWidth(style::window::default_border_no_focus_w);
        setFocusItem(list);
    }

} /* namespace gui */
