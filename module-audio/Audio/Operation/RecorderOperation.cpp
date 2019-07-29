/*
 *  @file RecorderOperation.cpp
 *  @author Mateusz Piesta (mateusz.piesta@mudita.com)
 *  @date 23.07.19
 *  @brief  
 *  @copyright Copyright (C) 2019 mudita.com
 *  @details
 */



#include "RecorderOperation.hpp"
#include "Audio/encoder/Encoder.hpp"
#include "bsp/audio/bsp_audio.hpp"
#include "Audio/Profiles/Profile.hpp"
#include "Audio/Profiles/ProfileRecordingHeadset.hpp"
#include "Audio/Profiles/ProfileRecordingOnBoardMic.hpp"
#include "Audio/Common.hpp"

using namespace bsp;

RecorderOperation::RecorderOperation(const char *file, const Encoder::Format &frmt) : format(frmt) {

    audioCallback = [this](const void *inputBuffer,
                           void *outputBuffer,
                           unsigned long framesPerBuffer) -> int32_t {

        auto ret = enc->Encode(framesPerBuffer, reinterpret_cast<int16_t *>(const_cast<void *>(inputBuffer)));
        if (ret == 0) {
            state = State::Idle;
            eventCallback(0);//TODO:M.P pass correct err code
        }
        return ret;

    };

    //TODO:M.P should be fetched from DB
    availableProfiles.push_back(std::make_unique<ProfileRecordingOnBoardMic>(nullptr, 20.0));
    availableProfiles.push_back(std::make_unique<ProfileRecordingHeadset>(nullptr, 10.0));
    profile = availableProfiles[0].get();

    enc = Encoder::Create(file, format);
    audioDevice = AudioDevice::Create(profile->GetAudioDeviceType(), audioCallback).value_or(nullptr);


    isInitialized = true;
}


int32_t RecorderOperation::Start(std::function<int32_t(uint32_t)> callback) {

    if (state == State::Paused || state == State::Active) {
        return static_cast<int32_t >(RetCode::InvokedInIncorrectState);
    }

    eventCallback = callback;
    state = State::Active;

    uint32_t flags = 0;
    if (format.chanNr == 2) {
        flags = static_cast<uint32_t >(AudioDevice::Flags::InputLeft) |
                static_cast<uint32_t >(AudioDevice::Flags::InputRight);
    } else if (format.chanNr == 1) {
        flags = static_cast<uint32_t >(AudioDevice::Flags::InputLeft);
    }

    audioFormat = AudioDevice::Format{.sampleRate_Hz=format.sampleRate,
            .bitWidth=16, // M.P Only 16-bit samples are supported
            .flags=flags,
            .outputVolume=profile->GetOutputVolume(),
            .inputGain=profile->GetInputGain(),
            .inputPath=profile->GetInputPath(),
            .outputPath=profile->GetOutputPath()
    };

    if(audioDevice->IsFormatSupported(audioFormat)){
        return audioDevice->Start(audioFormat);
    }
    else{
        return static_cast<int32_t >(RetCode ::InvalidFormat);
    }


}

int32_t RecorderOperation::Stop() {

    if (state == State::Paused || state == State::Idle) {
        return static_cast<int32_t >(RetCode::InvokedInIncorrectState);
    }

    state = State::Idle;
    return audioDevice->Stop();
}

int32_t RecorderOperation::Pause() {

    if (state == State::Paused || state == State::Idle) {
        return static_cast<int32_t >(RetCode::InvokedInIncorrectState);
    }

    state = State::Paused;
    return audioDevice->Stop();
}

int32_t RecorderOperation::Resume() {

    if (state == State::Active || state == State::Idle) {
        return static_cast<int32_t >(RetCode::InvokedInIncorrectState);
    }

    state = State::Active;
    return audioDevice->Start(audioFormat);
}

int32_t RecorderOperation::SendEvent(const Operation::Event evt, const EventData *data) {
    switch (evt) {
        case Event::HeadphonesPlugin:
            SwitchProfile(Profile::Type::RecordingHeadset);
            break;
        case Event::HeadphonesUnplug:
            SwitchProfile(Profile::Type::RecordingBuiltInMic);
            break;
        case Event::BTHeadsetOn:
            SwitchProfile(Profile::Type::RecordingBTHeadset);
            break;
        case Event::BTHeadsetOff:
            SwitchProfile(Profile::Type::RecordingBuiltInMic);
            break;
        default:
            return static_cast<int32_t >(RetCode::UnsupportedProfile);

    }

    return static_cast<int32_t >(RetCode::Success);
}

int32_t RecorderOperation::SwitchProfile(const Profile::Type type) {

    auto ret = GetProfile(type);
    if (ret) {
        profile = ret.value();
    } else {
        return static_cast<int32_t >(RetCode::UnsupportedProfile);
    }

    audioDevice = AudioDevice::Create(profile->GetAudioDeviceType(), audioCallback).value_or(nullptr);

    audioFormat = AudioDevice::Format{.sampleRate_Hz=audioFormat.sampleRate_Hz,
            .bitWidth=16,
            .flags=audioFormat.flags,
            .outputVolume=profile->GetOutputVolume(),
            .inputGain=profile->GetInputGain(),
            .inputPath=profile->GetInputPath(),
            .outputPath=profile->GetOutputPath()
    };

    switch (state) {
        case State::Idle:
        case State::Paused:
            break;

        case State::Active:
            state = State::Idle;
            Start(eventCallback);
            break;
    }


    //TODO:M.P add error handling
    return 0;
}

int32_t RecorderOperation::SetOutputVolume(float vol) {
    profile->SetOutputVolume(vol);
    audioDevice->OutputVolumeCtrl(vol);
    return static_cast<int32_t >(RetCode::Success);
}

int32_t RecorderOperation::SetInputGain(float gain) {
    profile->SetInputGain(gain);
    audioDevice->InputGainCtrl(gain);
    return static_cast<int32_t >(RetCode::Success);
}

Position RecorderOperation::GetPosition() {
    return enc->getCurrentPosition();
}
