// Copyright (c) 2017-2020, Mudita Sp. z.o.o. All rights reserved.
// For licensing, see https://github.com/mudita/MuditaOS/LICENSE.md

/*
 * @file ApplicationCallLog.hpp
 * @author Aleksander Rudnik (aleksander.rudnik@mudita.com)
 * @date 19.09.2019
 * @brief Application Call Log
 * @copyright Copyright (C) 2019 mudita.com
 * @details
 */
#pragma once

#include <Application.hpp>
#include <CalllogRecord.hpp>
#include <OptionWindow.hpp>

namespace app
{

    const inline std::string CallLogAppStr = "ApplicationCallLog";

    class ApplicationCallLog : public Application
    {
      protected:
      public:
        ApplicationCallLog(std::string name = CallLogAppStr, std::string parent = "", bool startBackgound = false);
        ~ApplicationCallLog() override;

        sys::Message_t DataReceivedHandler(sys::DataMessage *msgl, sys::ResponseMessage *resp) override;
        sys::ReturnCodes InitHandler() override;
        sys::ReturnCodes DeinitHandler() override;

        sys::ReturnCodes SwitchPowerModeHandler(const sys::ServicePowerMode mode) override final
        {
            return sys::ReturnCodes::Success;
        }

        void createUserInterface() final;
        void destroyUserInterface() final;

        bool removeCalllogEntry(const CalllogRecord &record);

        bool setAllEntriesRead();
    };

} /* namespace app */
