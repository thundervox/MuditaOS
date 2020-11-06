// Copyright (c) 2017-2020, Mudita Sp. z.o.o. All rights reserved.
// For licensing, see https://github.com/mudita/MuditaOS/LICENSE.md

#include "Application.hpp"

#include "MessageType.hpp"
#include "application-settings/windows/EinkModeWindow.hpp"
#include "windows/BtScanWindow.hpp"
#include "windows/BtWindow.hpp"
#include "windows/DateTimeWindow.hpp"
#include "windows/FotaWindow.hpp"
#include "windows/Info.hpp"
#include "windows/LanguageWindow.hpp"
#include "windows/SettingsMainWindow.hpp"
#include "windows/USSDWindow.hpp"

#include "windows/UITestWindow.hpp"

#include "windows/TestMessageWindow.hpp"

#include "ApplicationSettings.hpp"

#include "service-cellular/ServiceCellular.hpp"
#include "windows/SettingsMainWindow.hpp"
#include "windows/SimSelectWindow.hpp"
#include "windows/CellularPassthroughWindow.hpp"
#include "windows/SettingsChange.hpp"

#include <i18/i18.hpp>
#include <module-services/service-evtmgr/api/EventManagerServiceAPI.hpp>
#include <service-bluetooth/messages/BluetoothMessage.hpp>

namespace app
{
    ApplicationSettings::ApplicationSettings(std::string name, std::string parent, StartInBackground startInBackground)
        : Application(name, parent, startInBackground)
    {
        busChannels.push_back(sys::BusChannels::AntennaNotifications);
    }

    ApplicationSettings::~ApplicationSettings()
    {}

    // Invoked upon receiving data message
    sys::MessagePointer ApplicationSettings::DataReceivedHandler(sys::DataMessage *msgl,
                                                                 sys::ResponseMessage * /*resp*/)
    {

        auto retMsg = Application::DataReceivedHandler(msgl);
        // if message was handled by application's template there is no need to process further.
        if (reinterpret_cast<sys::ResponseMessage *>(retMsg.get())->retCode == sys::ReturnCodes::Success) {
            return retMsg;
        }
        if (auto btmsg = dynamic_cast<BluetoothScanResultMessage *>(msgl); btmsg != nullptr) {
            auto devices = btmsg->devices;
            LOG_INFO("received BT Scan message!");
            auto data = std::make_unique<gui::DeviceData>(devices);
            windowsFactory.build(this, gui::name::window::name_btscan);
            switchWindow(gui::name::window::name_btscan, gui::ShowMode::GUI_SHOW_INIT, std::move(data));

            render(gui::RefreshModes::GUI_REFRESH_FAST);
        }
        if (auto btmsg = dynamic_cast<BluetoothPairResultMessage *>(msgl); btmsg != nullptr) {
            if (btmsg->status) {
                LOG_INFO("Paired successfully");
            }
            else {
                LOG_ERROR("Pairing error!");
            }
        }
        // this variable defines whether message was processed.
        bool handled = true;

        if (msgl->messageType == MessageType::CellularNotification) {
            auto msg = dynamic_cast<CellularNotificationMessage *>(msgl);
            if (msg != nullptr) {
                if (msg->type == CellularNotificationMessage::Type::NewIncomingUSSD) {

                    auto window = this->getCurrentWindow();
                    if (window->getName() == gui::window::name::ussd_window) {
                        auto ussdWindow = dynamic_cast<gui::USSDWindow *>(window);
                        if (ussdWindow != nullptr) {
                            ussdWindow->handleIncomingUSSD(msg->data);
                        }
                    }
                }
            }
        }

        if (handled)
            return std::make_shared<sys::ResponseMessage>();
        else
            return std::make_shared<sys::ResponseMessage>(sys::ReturnCodes::Unresolved);
    }

    // Invoked during initialization
    sys::ReturnCodes ApplicationSettings::InitHandler()
    {
        board = EventManagerServiceAPI::GetBoard(this);

        auto ret = Application::InitHandler();
        if (ret != sys::ReturnCodes::Success)
            return ret;

        createUserInterface();

        setActiveWindow(gui::name::window::main_window);

        return ret;
    }

    sys::ReturnCodes ApplicationSettings::DeinitHandler()
    {
        return sys::ReturnCodes::Success;
    }

    void ApplicationSettings::createUserInterface()
    {
        windowsFactory.attach(gui::name::window::main_window, [](Application *app, const std::string &name) {
            return std::make_unique<gui::OptionWindow>(
                app, utils::localize.get("app_settings_title_main"), mainWindowOptions(app));
        });

        windowsFactory.attach(app::sim_select, [](Application *app, const std::string &name) {
            return std::make_unique<gui::OptionWindow>(app, name, simSelectWindow(app));
        });
        windowsFactory.attach(app::change_setting, [](Application *app, const std::string &name) {
            return std::make_unique<gui::OptionWindow>(app, name, settingsChangeWindow(app));
        });
        windowsFactory.attach("Languages", [](Application *app, const std::string &name) {
            return std::make_unique<gui::LanguageWindow>(app);
        });
        windowsFactory.attach("Bluetooth", [](Application *app, const std::string &name) {
            return std::make_unique<gui::BtWindow>(app);
        });
        windowsFactory.attach(gui::name::window::name_btscan, [](Application *app, const std::string &name) {
            return std::make_unique<gui::BtScanWindow>(app);
        });
        windowsFactory.attach("TEST_UI", [](Application *app, const std::string &name) {
            return std::make_unique<gui::UiTestWindow>(app);
        });
        windowsFactory.attach(gui::window::hw_info, [](Application *app, const std::string &name) {
            return std::make_unique<gui::Info>(app);
        });
        windowsFactory.attach("DateTime", [](Application *app, const std::string &name) {
            return std::make_unique<gui::DateTimeWindow>(app);
        });
        windowsFactory.attach(gui::window::name::fota_window, [](Application *app, const std::string &name) {
            return std::make_unique<gui::FotaWindow>(app);
        });

        windowsFactory.attach(gui::window::name::eink, [](Application *app, const std::string &name) {
            return std::make_unique<gui::EinkModeWindow>(app);
        });

        if (board == bsp::Board::T4) {
            windowsFactory.attach(gui::window::cellular_passthrough::window_name,
                                  [](Application *app, const std::string &name) {
                                      return std::make_unique<gui::CellularPassthroughWindow>(app);
                                  });
        }

        windowsFactory.attach(gui::window::name::ussd_window, [](Application *app, const std::string &name) {
            return std::make_unique<gui::USSDWindow>(app);
        });
    }

    void ApplicationSettings::destroyUserInterface()
    {}

} /* namespace app */
