﻿// Copyright (c) 2017-2020, Mudita Sp. z.o.o. All rights reserved.
// For licensing, see https://github.com/mudita/MuditaOS/LICENSE.md

#pragma once

#include "KbdMessage.hpp"
#include "BatteryMessages.hpp"

#include <MessageType.hpp>
#include <Service/Message.hpp>
#include <Service/Service.hpp>
#include <SwitchData.hpp>
#include <bsp/cellular/bsp_cellular.hpp>
#include <bsp/common.hpp>
#include <bsp/keyboard/key_codes.hpp>
#include <bsp/torch/torch.hpp>
#include <bsp/keypad_backlight/keypad_backlight.hpp>
#include <bsp/eink_frontlight/eink_frontlight.hpp>
#include <bsp/light_sensor/light_sensor.hpp>

#include <string>

namespace sevm
{
    namespace message
    {
        class GPIO : public Message
        {
          public:
            GPIO() : Message(MessageType::EVM_GPIO)
            {}
            uint32_t num = 0, port = 0, state = 0;
        };
    } // namespace message

    class RtcMinuteAlarmMessage : public Message
    {
      public:
        RtcMinuteAlarmMessage(MessageType messageType) : Message(messageType)
        {
            type = Type::Data;
        }
        uint32_t timestamp = 0;
    };

    class SIMMessage : public sys::DataMessage
    {
      public:
        SIMMessage() : DataMessage(MessageType::SIMTrayEvent)
        {
            type = Type::Data;
        }
    };

    /*
     * @brief Template for all messages that go to application manager
     */
    class EVMMessage : public sys::DataMessage
    {
      public:
        EVMMessage(MessageType messageType) : sys::DataMessage(messageType){};
        ~EVMMessage() override = default;
    };

    class EVMFocusApplication : public EVMMessage
    {
        std::string application;

      public:
        EVMFocusApplication(const std::string application)
            : EVMMessage(MessageType::EVMFocusApplication), application{application}
        {}
        [[nodiscard]] auto getApplication() const -> const std::string &
        {
            return application;
        };
    };

    class EVMAlarmSwitchData : public gui::SwitchData
    {
      public:
        EVMAlarmSwitchData() = default;
        EVMAlarmSwitchData(uint32_t id) : dbID(id){};
        ~EVMAlarmSwitchData() override = default;
        uint32_t dbID                  = 0;
    };

    class EVMBoardResponseMessage : public sys::ResponseMessage
    {
      public:
        EVMBoardResponseMessage(bool retCode,
                                std::string retdata    = std::string(),
                                MessageType responseTo = MessageType::MessageTypeUninitialized)
            : sys::ResponseMessage(sys::ReturnCodes::Success, responseTo), retCode(retCode){};
        EVMBoardResponseMessage(bool retCode, MessageType responseTo)
            : sys::ResponseMessage(sys::ReturnCodes::Success, responseTo), retCode(retCode){};

        bool retCode;
        bsp::Board board = bsp::Board::none;
    };

    class StatusStateMessage : public Message
    {
      public:
        StatusStateMessage(MessageType messageType) : Message(messageType)
        {
            type = Type::Data;
        }
        bsp::cellular::status::value state = bsp::cellular::status::value::INACTIVE;
    };

    class TorchStateMessage : public Message
    {
      public:
        TorchStateMessage(bsp::torch::Action direction) : Message(MessageType::EVMTorchStateMessage), action(direction)
        {}
        bsp::torch::Action action;
        bsp::torch::State state                  = bsp::torch::State::off;
        bsp::torch::ColourTemperature colourTemp = bsp::torch::ColourTemperature::no_change;
    };

    class TorchStateResultMessage : public TorchStateMessage
    {
      public:
        TorchStateResultMessage(bsp::torch::Action direction) : TorchStateMessage(direction)
        {}
        bool success = false;
    };
    class KeypadBacklightMessage : public Message
    {
      public:
        KeypadBacklightMessage() : Message(MessageType::EVMKeypadBacklightMessage)
        {}

        bsp::keypad_backlight::Action action;
    };

    class KeypadBacklightResponseMessage : public KeypadBacklightMessage
    {
      public:
        KeypadBacklightResponseMessage() : KeypadBacklightMessage()
        {}
        bool success;
    };

    class EinkFrontlightMessage : public Message
    {
      public:
        explicit EinkFrontlightMessage(bsp::eink_frontlight::Action act = bsp::eink_frontlight::Action::turnOff,
                                       std::uint8_t val                 = 0)
            : Message(MessageType::EVMEinkFrontlightMessage), action(act), value(val)
        {}

        const bsp::eink_frontlight::Action action;
        const bsp::eink_frontlight::BrightnessPercentage value;
    };

    class LightSensorMessage : public Message
    {
      public:
        LightSensorMessage() : Message(MessageType::EVMLightSensorMessage)
        {}
    };

    class LightSensorReadoutMessage : public LightSensorMessage
    {
      public:
        explicit LightSensorReadoutMessage(bsp::light_sensor::IlluminanceLux val) : LightSensorMessage(), value(val)
        {}
        const bsp::light_sensor::IlluminanceLux value;
    };

} /* namespace sevm*/
