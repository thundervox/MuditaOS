#pragma once

#include "Service/Message.hpp"
#include "MessageType.hpp"
#include "UpdatePureOS.hpp"
#include <vfs.hpp>

namespace sdesktop
{
    class UpdateOsMessage : public sys::DataMessage
    {
    public:
        UpdateOsMessage(const std::string updateFilePath, const uint32_t requestUUID)
                : sys::DataMessage(MessageType::UpdateOS)
                {
                    updateStats.updateFile = updateFilePath;
                    updateStats.uuid = requestUUID;
                };

        UpdateOsMessage() : sys::DataMessage(MessageType::UpdateOS) {}

        UpdateOsMessage(const updateos::UpdateMessageType updateMessageType)
            : sys::DataMessage(MessageType::UpdateOS), messageType(updateMessageType)
        {}

        UpdateOsMessage(const updateos::UpdateMessageType updateMessageType, fs::path updateFileFoundOnBoot)
            : sys::DataMessage(MessageType::UpdateOS), messageType(updateMessageType)
        {
            updateStats.updateFile = updateFileFoundOnBoot;
        }

        ~UpdateOsMessage() override = default;

        updateos::UpdateStats updateStats       = {};
        updateos::UpdateMessageType messageType = updateos::UpdateMessageType::UpdateNow;
        int rebootDelay                         = 0;
    };

    class BackupMessage : public sys::DataMessage
    {
      public:
        BackupMessage() : sys::DataMessage(MessageType::Backup)
        {}
        ~BackupMessage() override = default;
    };

    class RestoreMessage : public sys::DataMessage
    {
      public:
        RestoreMessage() : sys::DataMessage(MessageType::Restore)
        {}
        ~RestoreMessage() override = default;
    };

    class FactoryMessage : public sys::DataMessage
    {
      public:
        FactoryMessage() : sys::DataMessage(MessageType::Factory)
        {}
        ~FactoryMessage() override = default;
    };
} // namespace sdesktop
