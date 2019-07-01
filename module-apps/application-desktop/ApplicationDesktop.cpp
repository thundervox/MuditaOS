/*
 * @file ApplicationDesktop.cpp
 * @author Robert Borzecki (robert.borzecki@mudita.com)
 * @date 18 cze 2019
 * @brief
 * @copyright Copyright (C) 2019 mudita.com
 * @details
 */

#include "windows/DesktopMainWindow.hpp"

#include "Application.hpp"
#include "ApplicationDesktop.hpp"

#include "MessageType.hpp"
#include "windows/PinLockWindow.hpp"


namespace app {

ApplicationDesktop::ApplicationDesktop(std::string name) :
	Application( name, 8192 ) {
}

ApplicationDesktop::~ApplicationDesktop() {
}

uint32_t ApplicationDesktop::getMisseedCalls() {return missedCalls; }
uint32_t ApplicationDesktop::getUnreadMessages() { return unreadMessages; }

// Invoked upon receiving data message
sys::Message_t ApplicationDesktop::DataReceivedHandler(sys::DataMessage* msgl) {

	auto retMsg = Application::DataReceivedHandler(msgl);
	//if message was handled by application's template there is no need to process further.
	if( (reinterpret_cast<sys::ResponseMessage*>( retMsg.get() )->retCode ==
		sys::ReturnCodes::Success ) ){
		return retMsg;
	}

	//this variable defines whether message was processed.
	bool handled = true;

	if( handled )
		return std::make_shared<sys::ResponseMessage>();
	else
		return std::make_shared<sys::ResponseMessage>(sys::ReturnCodes::Unresolved);
}

// Invoked during initialization
sys::ReturnCodes ApplicationDesktop::InitHandler() {

	auto ret = Application::InitHandler();
	if( ret != sys::ReturnCodes::Success )
		return ret;

	//if value of the pin hash is different than 0 it means that home screen is pin protected
	if( settings.lockPassHash ) {
		pinLocked = true;
		screenLocked = true;
	}

	createUserInterface();

	setActiveWindow("MainWindow");

	return ret;
}

sys::ReturnCodes ApplicationDesktop::DeinitHandler() {
	return sys::ReturnCodes::Success;
}

sys::ReturnCodes ApplicationDesktop::WakeUpHandler() {
	return sys::ReturnCodes::Success;
}


sys::ReturnCodes ApplicationDesktop::SleepHandler() {
	return sys::ReturnCodes::Success;
}

void ApplicationDesktop::createUserInterface() {

	gui::Window* window = nullptr;

	window = new gui::DesktopMainWindow(this);
	windows.insert(std::pair<std::string,gui::Window*>(window->getName(), window));

	window = new gui::PinLockWindow(this);
	windows.insert(std::pair<std::string,gui::Window*>( window->getName(), window));
}

bool ApplicationDesktop::getScreenLocked() {
	return screenLocked;
}

bool ApplicationDesktop::getPinLocked() {
	return pinLocked;
}

void ApplicationDesktop::setScreenLocked( bool val ) {
	screenLocked = val;
};

void ApplicationDesktop::destroyUserInterface() {
}

} /* namespace gui */
