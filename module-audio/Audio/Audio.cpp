/*
 *  @file Audio.cpp
 *  @author Mateusz Piesta (mateusz.piesta@mudita.com)
 *  @date 22.07.19
 *  @brief  
 *  @copyright Copyright (C) 2019 mudita.com
 *  @details
 */



#include "Audio.hpp"

#include "Operation/Operation.hpp"

#include "log/log.hpp"


Audio::Audio(std::function<int32_t(uint32_t)> asyncCallback) : currentOperation(), asyncCallback(asyncCallback) {

    auto ret = Operation::Create(Operation::Type::Idle, "");
    if (ret) {
        currentOperation = std::move(ret.value());
    }
}

Position Audio::GetPosition() {
    return currentOperation->GetPosition();
}

int32_t Audio::SendEvent(const Operation::Event evt, const EventData *data) {
    return currentOperation->SendEvent(evt, data);
}

int32_t Audio::SetOutputVolume(Volume vol) {
    float volSet = 0;
    if(vol > 1){
        volSet = 1;
    }
    if(vol < 0){
        volSet = 0;
    }
    return currentOperation->SetOutputVolume(volSet);
}

int32_t Audio::SetInputGain(Gain gain) {
    float gainToSet = 0;
    if(gain > 10){
        gainToSet = 10.0;
    }
    if(gain < -10){
        gainToSet = -10;
    }
    return currentOperation->SetInputGain(gainToSet);
}

int32_t Audio::Start(Operation::Type op, const char *fileName) {

    auto ret = Operation::Create(op, fileName,Encoder::Format{.chanNr=1,.sampleRate=44100});
    if (ret) {

        switch (op) {
            case Operation::Type::Playback:
                currentState = State::Playback;
                break;
            case Operation::Type::Recorder:
                currentState = State::Recording;
                break;
            case Operation::Type::Router:
                currentState = State::Routing;
                break;
        }
        currentOperation = std::move(ret.value());
    } else {
        return static_cast<int32_t >(RetCode::OperationCreateFailed);
    }

    return currentOperation->Start(asyncCallback);
}

int32_t Audio::Stop() {
    if (currentState == State::Idle) {
        return static_cast<int32_t >(RetCode::Success);
    }

    auto retStop = currentOperation->Stop();

    auto ret = Operation::Create(Operation::Type::Idle, "");
    if (ret) {
        currentState = State::Idle;
        currentOperation = std::move(ret.value());
        return static_cast<int32_t >(RetCode::Success);
    } else {
        return static_cast<int32_t >(RetCode::OperationCreateFailed);
    }
}

int32_t Audio::Pause() {
    if (currentState == State::Idle) {
        return static_cast<int32_t >(RetCode::InvokedInIncorrectState);
    }

    return currentOperation->Pause();
}

int32_t Audio::Resume() {
    if (currentState == State::Idle) {
        return static_cast<int32_t >(RetCode::InvokedInIncorrectState);
    }
    return currentOperation->Resume();
}


