// Copyright (c) 2017-2021, Mudita Sp. z.o.o. All rights reserved.
// For licensing, see https://github.com/mudita/MuditaOS/LICENSE.md

#include "NewApnWindow.hpp"
#include "application-settings-new/ApplicationSettings.hpp"

#include <Dialog.hpp>
#include <messages/DialogMetadataMessage.hpp>

namespace gui
{

    NewApnWindow::NewApnWindow(app::Application *app)
        : AppWindow(app, gui::window::name::new_apn), newApnModel{std::make_shared<NewApnModel>(app)}
    {
        buildInterface();
    }

    void NewApnWindow::rebuild()
    {
        destroyInterface();
        buildInterface();
    }

    void NewApnWindow::buildInterface()
    {
        AppWindow::buildInterface();

        bottomBar->setText(BottomBar::Side::CENTER, utils::localize.get(style::strings::common::save));
        bottomBar->setText(BottomBar::Side::RIGHT, utils::localize.get(style::strings::common::back));

        setTitle(utils::localize.get("app_settings_new_edit_apn"));

        list = new gui::ListView(this,
                                 style::settings::window::newApn::x,
                                 style::settings::window::newApn::y,
                                 style::settings::window::newApn::w,
                                 style::settings::window::newApn::h,
                                 newApnModel);
        setFocusItem(list);
    }

    void NewApnWindow::destroyInterface()
    {
        erase();
    }

    void NewApnWindow::onBeforeShow(ShowMode mode, SwitchData *data)
    {
        if (mode != ShowMode::GUI_SHOW_RETURN) {
            newApnModel->clearData();
        }

        if (mode == ShowMode::GUI_SHOW_INIT) {
            list->rebuildList();
        }

        if (apn != nullptr) {
            newApnModel->loadData(apn);
        }
    }

    auto NewApnWindow::handleSwitchData(SwitchData *data) -> bool
    {
        setSaveButtonVisible(false);

        if (data == nullptr) {
            return false;
        }

        auto *item = dynamic_cast<ApnItemData *>(data);
        if (item == nullptr) {
            return false;
        }

        apn = item->getApn();
        if (apn == nullptr) {
            apn = std::make_shared<packet_data::APN::Config>();
            return true;
        }

        return true;
    }

    void NewApnWindow::setSaveButtonVisible(bool visible)
    {
        bottomBar->setActive(BottomBar::Side::CENTER, visible);
    }

    auto NewApnWindow::onInput(const InputEvent &inputEvent) -> bool
    {
        if (inputEvent.isShortPress()) {
            if (inputEvent.is(KeyCode::KEY_ENTER)) {
                if (apn != nullptr)
                    newApnModel->saveData(apn);
                verifyAndSave();
                return true;
            }
        }

        return AppWindow::onInput(inputEvent);
    }

    auto NewApnWindow::verifyAndSave() -> bool
    {
        if (apn != nullptr) {
            newApnModel->apnSendRecord(*apn);
            LOG_DEBUG("APN record  saved : \"%s\" ", apn->apn.c_str());
        }
        else
            LOG_DEBUG("APN record not found");

        return true;
    }

} // namespace gui
