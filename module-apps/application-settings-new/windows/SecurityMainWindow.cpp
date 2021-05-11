// Copyright (c) 2017-2021, Mudita Sp. z.o.o. All rights reserved.
// For licensing, see https://github.com/mudita/MuditaOS/LICENSE.md

#include <locks/widgets/Lock.hpp>
#include <locks/data/LockData.hpp>
#include "application-settings-new/ApplicationSettings.hpp"
#include "application-settings-new/data/ChangePasscodeData.hpp"
#include "module-apps/application-desktop/windows/Names.hpp"
#include "OptionSetting.hpp"
#include "SecurityMainWindow.hpp"

namespace gui
{
    SecurityMainWindow::SecurityMainWindow(app::Application *app, app::settingsInterface::SecuritySettings *settings)
        : BaseSettingsWindow(app, window::name::security), securitySettings(settings)
    {}

    void SecurityMainWindow::onBeforeShow(ShowMode mode, SwitchData *data)
    {
        isPhoneLockEnabled = application->isPhoneLockEnabled();
        BaseSettingsWindow::onBeforeShow(mode, data);
    }

    auto SecurityMainWindow::buildOptionsList() -> std::list<Option>
    {
        std::list<Option> optionList;

        optionList.emplace_back(std::make_unique<option::OptionSettings>(
            utils::translate("app_settings_security_phone_lock"),
            [=](Item &item) {
                isPhoneLockEnabled ? application->getPhoneLockSubject().disablePhoneLock()
                                   : application->getPhoneLockSubject().enablePhoneLock();
                return true;
            },
            [=](Item &item) {
                if (item.focus) {
                    this->setBottomBarText(utils::translate(style::strings::common::Switch), BottomBar::Side::CENTER);
                }
                else {
                    this->setBottomBarText(utils::translate(style::strings::common::select), BottomBar::Side::CENTER);
                }
                return true;
            },
            nullptr,
            isPhoneLockEnabled ? option::SettingRightItem::On : option::SettingRightItem::Off));

        optionList.emplace_back(std::make_unique<option::OptionSettings>(
            utils::translate("app_settings_security_usb_passcode"),
            [=](Item &item) {
                auto lock = std::make_unique<locks::Lock>(
                    Store::GSM::SIM::NONE, locks::Lock::LockState::InputRequired, locks::Lock::LockType::Screen);
                lock->onActivatedCallback = [this](locks::Lock::LockType type, const std::vector<unsigned int> &data) {
                    securitySettings->setUSBSecurity(!securitySettings->isUSBSecured());
                    application->returnToPreviousWindow();
                };
                application->switchWindow(
                    app::window::name::desktop_pin_lock,
                    gui::ShowMode::GUI_SHOW_INIT,
                    std::make_unique<locks::LockData>(*lock, locks::PhoneLockInputTypeAction::Change));
                return true;
            },
            [=](Item &item) {
                if (item.focus) {
                    this->setBottomBarText(utils::translate(style::strings::common::Switch), BottomBar::Side::CENTER);
                }
                else {
                    this->setBottomBarText(utils::translate(style::strings::common::select), BottomBar::Side::CENTER);
                }
                return true;
            },
            nullptr,
            securitySettings->isUSBSecured() ? option::SettingRightItem::On : option::SettingRightItem::Off));

        if (isPhoneLockEnabled) {
            optionList.emplace_back(std::make_unique<option::OptionSettings>(
                utils::translate("app_settings_security_change_phone_lock"),
                [=](Item &item) {
                    application->getPhoneLockSubject().changePhoneLock();
                    return true;
                },
                nullptr,
                nullptr,
                option::SettingRightItem::ArrowWhite));
        }

        return optionList;
    }
} // namespace gui
