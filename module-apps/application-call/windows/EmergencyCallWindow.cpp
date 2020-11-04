﻿// Copyright (c) 2017-2020, Mudita Sp. z.o.o. All rights reserved.
// For licensing, see https://github.com/mudita/MuditaOS/LICENSE.md

#include "../data/CallSwitchData.hpp"
#include "../ApplicationCall.hpp"
#include <service-appmgr/model/ApplicationManager.hpp>

#include "i18/i18.hpp"
#include "EmergencyCallWindow.hpp"

namespace gui
{

    EmergencyCallWindow::EmergencyCallWindow(app::Application *app)
        : EnterNumberWindow(app, app::window::name_emergencyCall)
    {

        numberLabel->setText(utils::localize.get("app_call_emergency"));
    }

    void EmergencyCallWindow::rebuild()
    {}
    void EmergencyCallWindow::buildInterface()
    {
        AppWindow::buildInterface();
    }
    void EmergencyCallWindow::destroyInterface()
    {
        AppWindow::destroyInterface();
    }

    bool EmergencyCallWindow::onInput(const InputEvent &inputEvent)
    {
        bool ret = EnterNumberWindow::onInput(inputEvent);

        //	if( number.empty() ) {
        //		numberLabel->setText( utils::localize.get("app_call_emergency") );
        //	}

        return ret;
    }
    bool EmergencyCallWindow::handleSwitchData(SwitchData *data)
    {
        bool ret = EnterNumberWindow::handleSwitchData(data);

        return ret;
    }

} /* namespace gui */
