#include "InputSelector.hpp"
#include "service-appmgr/ApplicationManager.hpp"
#include "messages/AppMessage.hpp"
#include "../AppSpecialInput.hpp"
#include "Style.hpp"
#include <InputMode.hpp>

using namespace gui;

UiCharSelector::UiCharSelector(app::Application *app) : AppWindow(app, gui::char_select )
{
    AppWindow::buildInterface();
    box = new GridLayout(0, title->offset_h(), style::window_width, bottomBar->offset_h() - title->offset_h(), {90, 90});
    for (auto schar : gui::special_chars)
    {
        auto el = new gui::Label(box, 0, 0, 80, 80);
        style::window::decorate(el);
        el->setAlignement(gui::Alignment(gui::Alignment::ALIGN_HORIZONTAL_CENTER, gui::Alignment::ALIGN_VERTICAL_CENTER));
        el->setText(std::string(1, schar));
        el->setFont(style::window::font::medium);
        el->activatedCallback = [=](Item &it) {
            // message special char -> to actual requester ...
            // messageSpecialChar();
// TODO
//            auto msg = std::make_shared<app::msg::AppSpecialInput>(InputEvent(el->getText()[0]));
//            sys::Bus::SendUnicast(msg, dynamic_cast<app::AppSpecialInput*>(application)->getRequester(), application);
//            return true;
//            LOG_INFO("handled %s", el->getText().c_str());
            return true;
        };
        // on click -> send onInput event with mapped & special keypress (...)
        // better - read special_keys_keymap and handle accordingly ... to be done
    }
    box->setVisible(true);
    box->setNavigation(nullptr, nullptr);
    /// TODO do not resize on each addWidget .. just once :(
    /// TODO shouldn't be needed here
    /// TODO add widget -> put widget in it's position
    box->resizeItems();
    addWidget(box);
    // TODO TODO attach(cb - show special characters, && input somehow)
    setFocusItem(box->getNavigationItem());
}

void UiCharSelector::onBeforeShow(ShowMode mode, SwitchData *data)
{
    auto ret = dynamic_cast<SwitchSpecialChar*>(data);
    if(ret) {
         LOG_INFO("handle!");
         // TODO STORE FROM WHOM!
        application->refreshWindow(gui::RefreshModes::GUI_REFRESH_FAST);
    }
}
void UiCharSelector::rebuild() {}
void UiCharSelector::buildInterface() {}
void UiCharSelector::destroyInterface() {}
