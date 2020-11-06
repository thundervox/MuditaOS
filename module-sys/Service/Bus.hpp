// Copyright (c) 2017-2020, Mudita Sp. z.o.o. All rights reserved.
// For licensing, see https://github.com/mudita/MuditaOS/LICENSE.md

#pragma once

#include <memory>
#include <map>
#include <queue>
#include "Channel.hpp"
#include "Common.hpp"
#include "Message.hpp"

namespace sys
{
    class Service;
    class Message;

    constexpr uint32_t defaultCmdTimeout = 5000;
    class Bus
    {

      public:
        // Send message directly to specified service (unicast type)
        static bool SendUnicast(std::shared_ptr<Message> msg, const std::string &service, Service *s);

        // Send message directly to specified service with timeout
        static SendResult SendUnicast(std::shared_ptr<Message> msg,
                                      const std::string &service,
                                      Service *s,
                                      uint32_t timeout);

        // Send message to specific channel
        static void SendMulticast(std::shared_ptr<Message> msg, BusChannels channel, Service *source);

        // Send message to all registered services ()
        static void SendBroadcast(std::shared_ptr<Message> msg, Service *source);

        // Register new service
        static void Add(std::shared_ptr<Service> service);

        // Unregister specified service
        static void Remove(std::shared_ptr<Service> service);

        static void SendResponse(std::shared_ptr<Message> response, std::shared_ptr<Message> request, Service *sender);

      private:
        Bus();
        ~Bus();

        static std::map<BusChannels, Channel> channels;
        static std::map<std::string, std::shared_ptr<Service>> servicesRegistered;

        static uint64_t uniqueMsgId;
        static uint64_t unicastMsgId;
    };

} // namespace sys
