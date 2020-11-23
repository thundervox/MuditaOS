// Copyright (c) 2017-2020, Mudita Sp. z.o.o. All rights reserved.
// For licensing, see https://github.com/mudita/MuditaOS/LICENSE.md

#include "ApplicationSpecialInput.hpp"
#include "messages/AppMessage.hpp"
#include "windows/SpecialInputMainWindow.hpp"

using namespace app;

ApplicationSpecialInput::ApplicationSpecialInput(std::string name,
                                                 std::string parent,
                                                 StartInBackground startInBackground)
    : Application(name, parent, startInBackground)
{
    addActionReceiver(manager::actions::ShowSpecialInput, [this](auto &&data) {
        switchWindow(app::char_select, std::move(data));
        return msgHandled();
    });

    windowsFactory.attach(app::char_select, [](Application *app, const std::string &name) {
        return std::make_unique<gui::SpecialInputMainWindow>(app);
    });

    setActiveWindow(app::char_select);
}

sys::MessagePointer ApplicationSpecialInput::DataReceivedHandler(sys::DataMessage *msgl, sys::ResponseMessage *resp)
{
    auto retMsg = Application::DataReceivedHandler(msgl);
    if (dynamic_cast<sys::ResponseMessage *>(retMsg.get())->retCode == sys::ReturnCodes::Success) {
        return retMsg;
    }

    return std::make_shared<sys::ResponseMessage>(sys::ReturnCodes::Unresolved);
}

sys::ReturnCodes ApplicationSpecialInput::InitHandler()
{
    auto ret = Application::InitHandler();
    if (ret != sys::ReturnCodes::Success) {
        LOG_ERROR("Can't initialize ApplicationSpecialInput");
    }
    createUserInterface();
    setActiveWindow(gui::name::window::main_window);
    return ret;
}

void ApplicationSpecialInput::createUserInterface()
{
    windowsFactory.build(this, app::char_select);
}

void ApplicationSpecialInput::destroyUserInterface()
{}

sys::ReturnCodes ApplicationSpecialInput::DeinitHandler()
{
    return sys::ReturnCodes::Success;
}
