// Copyright (c) 2017-2021, Mudita Sp. z.o.o. All rights reserved.
// For licensing, see https://github.com/mudita/MuditaOS/LICENSE.md

#include <locks/widgets/Lock.hpp>
#include <locks/data/LockData.hpp>
#include "application-settings/ApplicationSettings.hpp"
#include "application-settings/data/ChangePasscodeData.hpp"
#include "OptionSetting.hpp"
#include "SecurityMainWindow.hpp"

namespace gui
{
    SecurityMainWindow::SecurityMainWindow(app::Application *app) : BaseSettingsWindow(app, window::name::security)
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

        optionList.emplace_back(std::make_unique<gui::option::OptionSettings>(
            utils::translate("app_settings_display_locked_screen_autolock"),
            [=](gui::Item &item) {
                this->application->switchWindow(gui::window::name::autolock, nullptr);
                return true;
            },
            nullptr,
            nullptr,
            gui::option::SettingRightItem::ArrowWhite,
            false));

        return optionList;
    }
} // namespace gui
