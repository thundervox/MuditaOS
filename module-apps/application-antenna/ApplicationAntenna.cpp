/*
 * ApplicationAntenna.cpp
 *
 *  Created on: 18 lut 2020
 *      Author: kuba
 */

#include "ApplicationAntenna.hpp"
#include "service-cellular/api/CellularServiceAPI.hpp"
#include "module-cellular/at/response.hpp"

#include "windows/AntennaMainWindow.hpp"
#include "windows/ScanModesWindow.hpp"
#include "windows/AlgoParamsWindow.hpp"

#include <ticks.hpp>
namespace app
{

    void ApplicationAntenna::timerHandler(void)
    {
        auto win = getCurrentWindow();
        auto windowName = win->getName();
        if ((windowName == gui::name::window::main_window) || (windowName == gui::name::window::algo_window)) {
            if (!cellularRequestInProgress) {
                LOG_INFO("Get Network info");
                cellularRequestInProgress = true;
                CellularServiceAPI::GetNetworkInfo(this);
            }
        }
    }

    ApplicationAntenna::ApplicationAntenna(std::string name, std::string parent, bool startBackgound)
        : Application(name, parent, startBackgound, 4096 * 2),
          appTimer(CreateAppTimer(2000, true, [=]() { timerHandler(); }))

    {
        busChannels.push_back(sys::BusChannels::AntennaNotifications);
        busChannels.push_back(sys::BusChannels::AntennaNotifications);
        appTimer.restart();
    }

    ApplicationAntenna::~ApplicationAntenna()
    {}

    // Invoked upon receiving data message
    sys::Message_t ApplicationAntenna::DataReceivedHandler(sys::DataMessage *msgl, sys::ResponseMessage *resp)
    {
        auto retMsg = Application::DataReceivedHandler(msgl);
        // if message was handled by application's template there is no need to process further.
        if ((reinterpret_cast<sys::ResponseMessage *>(retMsg.get())->retCode == sys::ReturnCodes::Success)) {
            return retMsg;
        }

        // this variable defines whether message was processed.
        bool handled = false;
        if (msgl->messageType == MessageType::CellularOperatorsScanResult) {
            auto msg = dynamic_cast<cellular::RawCommandRespAsync *>(msgl);
            if (msg != nullptr) {
                auto win = getCurrentWindow();

                if (win->getName() == gui::name::window::main_window) {
                    auto window = dynamic_cast<gui::AntennaMainWindow *>(win);
                    if (window != nullptr) {

                        window->updateOperatorsScan(msg->data);
                    }
                }
                cellularRequestInProgress = false;
            }
            handled = true;
        }
        if (msgl->messageType == MessageType::CellularNetworkInfoResult) {
            auto msg = dynamic_cast<cellular::RawCommandRespAsync *>(msgl);
            if (msg != nullptr) {
                handleNetworkParams(msg->data);
                auto win = getCurrentWindow();

                if (win->getName() == gui::name::window::main_window) {
                    auto window = dynamic_cast<gui::AntennaMainWindow *>(win);
                    if (window != nullptr) {

                        window->updateDebugInfo(msg->data);
                    }
                }

                cellularRequestInProgress = false;
            }
            handled = true;
        }
        if (msgl->messageType == MessageType::CellularGetScanModeResult) {
            auto msg = dynamic_cast<cellular::RawCommandRespAsync *>(msgl);
            if (msg != nullptr) {
                auto win = getWindow(gui::name::window::scan_window);

                if (win->getName() == gui::name::window::scan_window) {
                    auto window = dynamic_cast<gui::ScanModesWindow *>(win);
                    if (window != nullptr) {

                        window->updateCurrentMode(msg->data[0]);
                    }
                }
                cellularRequestInProgress = false;
            }
            handled = true;
        }
        if (msgl->messageType == MessageType::AntennaChanged) {

            CellularServiceAPI::GetAntenna(this, antenna);
            auto win = getCurrentWindow();

            if (win->getName() == gui::name::window::main_window) {
                auto window = dynamic_cast<gui::AntennaMainWindow *>(win);
                if (window != nullptr) {

                    window->updateAntennaButtons(antenna);
                }
            }

            bool refresh = false;
            if (win->getName() == gui::name::window::algo_window) {
                refresh = true;
            }
            auto window     = getWindow(gui::name::window::algo_window);
            auto algoWindow = dynamic_cast<gui::AlgoParamsWindow *>(window);
            if (algoWindow != nullptr) {
                algoWindow->handleAntennaChanged(antenna, refresh);
            }
            handled = true;
        }
        if (msgl->messageType == MessageType::AntennaCSQChange) {
            csqUpdated = true;
            handled    = true;
        }
        if (handled)
            return std::make_shared<sys::ResponseMessage>();
        else
            return std::make_shared<sys::ResponseMessage>(sys::ReturnCodes::Unresolved);
    }

    // Invoked during initialization
    sys::ReturnCodes ApplicationAntenna::InitHandler()
    {

        auto ret = Application::InitHandler();
        if (ret != sys::ReturnCodes::Success)
            return ret;

        createUserInterface();

        setActiveWindow(gui::name::window::main_window);

        CellularServiceAPI::GetAntenna(this, antenna);

        return ret;
    }

    sys::ReturnCodes ApplicationAntenna::DeinitHandler()
    {
        return sys::ReturnCodes::Success;
    }

    void ApplicationAntenna::createUserInterface()
    {

        gui::AppWindow *win = new gui::AntennaMainWindow(this);
        windows.insert(std::pair<std::string, gui::AppWindow *>(gui::name::window::main_window, win));

        win = new gui::ScanModesWindow(this);
        windows.insert(std::pair<std::string, gui::AppWindow *>(gui::name::window::scan_window, win));

        win = new gui::AlgoParamsWindow(this);
        windows.insert(std::pair<std::string, gui::AppWindow *>(gui::name::window::algo_window, win));
    }

    void ApplicationAntenna::destroyUserInterface()
    {}

    void ApplicationAntenna::handleNetworkParams(std::vector<std::string> &data)
    {

        uint32_t creg;
        at::response::parseCREG(data[1], creg);

        uint32_t bandFreq = at::response::qnwinfo::parseNetworkFrequency(data[2]);

        if ((csqUpdated) || lastCreg != creg || lastFreq != bandFreq) {
            std::string param;
            at::response::parseCSQ(data[0], param);
            param += " | " + std::to_string(creg);
            param += " | " + std::to_string(bandFreq);

            csqUpdated = false;
            lastCreg   = creg;
            lastFreq   = bandFreq;

            bool refresh = false;
            auto win     = getWindow(gui::name::window::algo_window);
            if (win == getCurrentWindow()) {
                refresh = true;
            }
            auto window = dynamic_cast<gui::AlgoParamsWindow *>(win);
            if (window != nullptr) {

                window->handleNewParams(param, refresh);
            }
        }
    }
} /* namespace app */
