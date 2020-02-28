/*
 * @file ApplicationCall.cpp
 * @author Robert Borzecki (robert.borzecki@mudita.com)
 * @date 1 lip 2019
 * @brief
 * @copyright Copyright (C) 2019 mudita.com
 * @details
 */
#include "Application.hpp"

#include "MessageType.hpp"
#include "windows/CallMainWindow.hpp"
#include "windows/EnterNumberWindow.hpp"
#include "windows/EmergencyCallWindow.hpp"
#include "windows/CallWindow.hpp"
#include "data/CallSwitchData.hpp"
#include <ticks.hpp>

#include "service-cellular/ServiceCellular.hpp"
#include "service-cellular/api/CellularServiceAPI.hpp"
#include "service-audio/api/AudioServiceAPI.hpp"

#include "service-appmgr/ApplicationManager.hpp"

#include "ApplicationCall.hpp"
namespace app {

    ApplicationCall::ApplicationCall(std::string name, std::string parent, bool startBackgound)
        : Application(name, parent, startBackgound, 4096 + 2048), timerCall(CreateAppTimer(1000, true, [=]() { timerCallCallback(); }))
    {
    }

ApplicationCall::~ApplicationCall() {
}

void ApplicationCall::timerCallCallback()
{
    // Invoked when timer ticked, 3 seconds after end call event if user didn't press back button earlier.
    ++callDuration;

    auto it = windows.find(window::name_call);
    if (getCurrentWindow() == it->second)
    {
        gui::CallWindow *callWindow = reinterpret_cast<gui::CallWindow *>(getCurrentWindow());

        if (callWindow->getState() == gui::CallWindow::State::CALL_IN_PROGRESS)
        {
            callWindow->updateDuration(callDuration);
            refreshWindow(gui::RefreshModes::GUI_REFRESH_FAST);
        }
    }

    if (callDuration >= callEndTime)
    {
        LOG_INFO("callDuration %d, callEndTime id %d", callDuration, callEndTime);
        timerCall.stop();
        sapm::ApplicationManager::messageSwitchPreviousApplication(this);
    }
}

// Invoked upon receiving data message
sys::Message_t ApplicationCall::DataReceivedHandler(sys::DataMessage* msgl,sys::ResponseMessage* resp) {

	auto retMsg = Application::DataReceivedHandler(msgl);
	//if message was handled by application's template there is no need to process further.
	if( (reinterpret_cast<sys::ResponseMessage*>( retMsg.get() )->retCode ==
		sys::ReturnCodes::Success ) ){
		return retMsg;
	}

	//this variable defines whether message was processed.
	bool handled = false;
    if (msgl->messageType == MessageType::CellularNotification)
    {

        CellularNotificationMessage *msg = reinterpret_cast<CellularNotificationMessage *>(msgl);
        gui::CallWindow *callWindow = reinterpret_cast<gui::CallWindow *>(windows.find(window::name_call)->second);

        if (msg->type == CellularNotificationMessage::Type::CallAborted)
        {
            LOG_INFO("---------------------------------CallAborted");
            AudioServiceAPI::Stop(this);
            callEndTime = callDuration + 3;
            callWindow->setState(gui::CallWindow::State::CALL_ENDED);
            refreshWindow(gui::RefreshModes::GUI_REFRESH_DEEP);
        }
        else if( msg->type == CellularNotificationMessage::Type::CallBusy) {
			callEndTime = callDuration + 3;
		    LOG_INFO("---------------------------------CallBusy");
		    AudioServiceAPI::Stop(this);
		    callWindow->setState( gui::CallWindow::State::CALL_ENDED );
		    refreshWindow( gui::RefreshModes::GUI_REFRESH_DEEP );
		}
		else if( msg->type == CellularNotificationMessage::Type::CallActive ) {
			callDuration = 0;

			LOG_INFO("---------------------------------CallActive");
			callWindow->setState( gui::CallWindow::State::CALL_IN_PROGRESS );
			refreshWindow( gui::RefreshModes::GUI_REFRESH_DEEP );
		}
		else if( msg->type == CellularNotificationMessage::Type::IncomingCall ) {
			//reset call duration
//		    callDuration = 0;
			LOG_INFO("---------------------------------IncomingCall");
			if( callWindow->getState() == gui::CallWindow::State::INCOMING_CALL ) {
				LOG_INFO("ignoring call incoming");
			}
			else {

				AudioServiceAPI::RoutingStart(this);
				runCallTimer();
				std::unique_ptr<gui::SwitchData> data = std::make_unique<app::IncommingCallData>(msg->data);
				//send to itself message to switch (run) call application
				callWindow->setState( gui::CallWindow::State::INCOMING_CALL );
                if (getState() == State::ACTIVE_FORGROUND)
                {
                    LOG_INFO("++++++++++++WINDOW SWITCH");
                    switchWindow(window::name_call, std::move(data));
                }
                else {
					LOG_INFO("++++++++++++APP SWITCH");

                    sapm::ApplicationManager::messageSwitchApplication(this, name_call, window::name_call, std::move(data));
                }
			}
		}
		else if( msg->type == CellularNotificationMessage::Type::Ringing ) {
			//reset call duration
			runCallTimer();
			LOG_INFO("---------------------------------Ringing");
			AudioServiceAPI::RoutingStart(this);

			std::unique_ptr<gui::SwitchData> data = std::make_unique<app::ExecuteCallData>(msg->data);
			callWindow->setState( gui::CallWindow::State::OUTGOING_CALL );
            if (getState() == State::ACTIVE_FORGROUND)
            {
                switchWindow(window::name_call, std::move(data));
            }
        }
		handled = true;
    }

    if( handled )
		return std::make_shared<sys::ResponseMessage>();
	else
		return std::make_shared<sys::ResponseMessage>(sys::ReturnCodes::Unresolved);
}

// Invoked during initialization
sys::ReturnCodes ApplicationCall::InitHandler() {

	auto ret = Application::InitHandler();
	if( ret != sys::ReturnCodes::Success )
		return ret;

	createUserInterface();

	setActiveWindow("MainWindow");

	return ret;
}

sys::ReturnCodes ApplicationCall::DeinitHandler() {
	return sys::ReturnCodes::Success;
}

void ApplicationCall::stopCallTimer() {
	LOG_INFO("switching to prev calldur: %d endTime: %d", callDuration, callEndTime );
	timerCall.stop();
	sapm::ApplicationManager::messageSwitchPreviousApplication( this );
}


void ApplicationCall::createUserInterface() {

	gui::AppWindow* window = nullptr;

	window = new gui::CallMainWindow(this);
	windows.insert(std::pair<std::string,gui::AppWindow*>(window->getName(), window));

	window = new gui::EnterNumberWindow(this);
	windows.insert(std::pair<std::string,gui::AppWindow*>(window->getName(), window));

	window = new gui::CallWindow(this);
	windows.insert(std::pair<std::string,gui::AppWindow*>(window->getName(), window));

	window = new gui::EmergencyCallWindow(this);
	windows.insert(std::pair<std::string,gui::AppWindow*>(window->getName(), window));
}

void ApplicationCall::setDisplayedNumber( std::string num ) {
	phoneNumber = num;
}

const std::string& ApplicationCall::getDisplayedNumber() {
	return phoneNumber;
}

void ApplicationCall::runCallTimer() {
	callDuration = 0;
	callEndTime = -1;
	timerCall.restart();
}

void ApplicationCall::destroyUserInterface() {
}

} /* namespace gui */
