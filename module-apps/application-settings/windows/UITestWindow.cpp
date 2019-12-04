#include "UITestWindow.hpp"
#include "../ApplicationSettings.hpp"
#include "Label.hpp"
#include "Margins.hpp"
#include "i18/i18.hpp"
#include "service-appmgr/ApplicationManager.hpp"
#include <Style.hpp>
#include <functional>
#include <memory>
#include <GridLayout.hpp>
#include "messages/AppMessage.hpp"

namespace gui
{

UiTestWindow::UiTestWindow(app::Application *app) : AppWindow(app, "TEST_UI")
{
    AppWindow::buildInterface();
    buildInterface();
    // prebuild
    bottomBar->setActive(BottomBar::Side::CENTER, true);
    bottomBar->setActive(BottomBar::Side::RIGHT, true);
    bottomBar->setText(BottomBar::Side::CENTER, utils::localize.get("common_select"));
    bottomBar->setText(BottomBar::Side::RIGHT, utils::localize.get("common_back"));
    setTitle("UI TEST");
    text = new gui::Text(this, style::window::default_left_margin, title->offset_h(), style::window_width - 2 * style::window::default_left_margin, 100,
                         "LoL ...");
    text->setEditMode(gui::Text::EditMode::EDIT);
    text->setFont(style::window::font::bigbold);
    text->setInputMode(new InputMode({InputMode::ABC, InputMode::abc, InputMode::digit}, [=](const UTF8 &text) {
        bottomBar->setText(BottomBar::Side::CENTER, text);
        application->refreshWindow(gui::RefreshModes::GUI_REFRESH_FAST);
        bottomBar->getText(BottomBar::Side::CENTER);
    },
    [=]() {
        LOG_INFO("Get special char!");
        sapm::ApplicationManager::messageSwitchSpecialInput( application,
                std::make_unique<gui::SwitchSpecialChar>(gui::SwitchSpecialChar::Type::Request,application->GetName()));
    }
    ));
    setFocusItem(text);
}

void UiTestWindow::rebuild() { LOG_INFO("Only if I have to!"); }

void UiTestWindow::buildInterface() {}

void UiTestWindow::destroyInterface()
{
    this->focusItem = nullptr;
    AppWindow::destroyInterface();
}

UiTestWindow::~UiTestWindow() { destroyInterface(); }

void UiTestWindow::onBeforeShow(ShowMode mode, SwitchData *data) {}

} // namespace gui
