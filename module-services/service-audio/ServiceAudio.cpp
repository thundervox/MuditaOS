// Copyright (c) 2017-2020, Mudita Sp. z.o.o. All rights reserved.
// For licensing, see https://github.com/mudita/MuditaOS/LICENSE.md

/*
 *  @file ServiceAudio.cpp
 *  @author Mateusz Piesta (mateusz.piesta@mudita.com)
 *  @date 29.07.19
 *  @brief
 *  @copyright Copyright (C) 2019 mudita.com
 *  @details
 */

#include "ServiceAudio.hpp"
#include "messages/AudioMessage.hpp"
#include "Audio/Operation/IdleOperation.hpp"
#include "Audio/Operation/PlaybackOperation.hpp"

#include <type_traits>

const char *ServiceAudio::serviceName = "ServiceAudio";

using namespace audio;

ServiceAudio::ServiceAudio()
    : sys::Service(serviceName, "", 4096 * 2, sys::ServicePriority::Idle),
      audioMux([this](auto... params) { return this->AsyncCallback(params...); },
               [this](auto... params) { return this->DbCallback(params...); })
{
    LOG_INFO("[ServiceAudio] Initializing");
    busChannels.push_back(sys::BusChannels::ServiceAudioNotifications);
}

ServiceAudio::~ServiceAudio()
{
    LOG_INFO("[ServiceAudio] Cleaning resources");
}

sys::ReturnCodes ServiceAudio::InitHandler()
{
    static const std::string defaultVolumeHigh = "10";
    static const std::string defaultVolumeLow  = "2";
    static const std::string defaultTrue       = "1";
    static const std::string defaultFalse      = "0";

    static const int IdxPath  = 0;
    static const int IdxValue = 1;

    const static std::vector<std::vector<std::string>> defaultSettings = {

        // PLAYBACK
        {dbPath(Setting::Volume, PlaybackType::Multimedia, Profile::Type::PlaybackHeadphones), defaultVolumeLow},
        {dbPath(Setting::Volume, PlaybackType::Multimedia, Profile::Type::PlaybackBTA2DP), defaultVolumeLow},
        {dbPath(Setting::Volume, PlaybackType::Multimedia, Profile::Type::PlaybackLoudspeaker), defaultVolumeHigh},

        {dbPath(Setting::Volume, PlaybackType::Notifications, Profile::Type::PlaybackHeadphones), defaultVolumeLow},
        {dbPath(Setting::Volume, PlaybackType::Notifications, Profile::Type::PlaybackBTA2DP), defaultVolumeLow},
        {dbPath(Setting::Volume, PlaybackType::Notifications, Profile::Type::PlaybackLoudspeaker), defaultVolumeHigh},

        {dbPath(Setting::Volume, PlaybackType::KeypadSound, Profile::Type::PlaybackHeadphones), defaultVolumeLow},
        {dbPath(Setting::Volume, PlaybackType::KeypadSound, Profile::Type::PlaybackBTA2DP), defaultVolumeLow},
        {dbPath(Setting::Volume, PlaybackType::KeypadSound, Profile::Type::PlaybackLoudspeaker), defaultVolumeHigh},

        {dbPath(Setting::Volume, PlaybackType::CallRingtone, Profile::Type::PlaybackHeadphones), defaultVolumeLow},
        {dbPath(Setting::Volume, PlaybackType::CallRingtone, Profile::Type::PlaybackBTA2DP), defaultVolumeLow},
        {dbPath(Setting::Volume, PlaybackType::CallRingtone, Profile::Type::PlaybackLoudspeaker), defaultVolumeHigh},

        {dbPath(Setting::Volume, PlaybackType::TextMessageRingtone, Profile::Type::PlaybackHeadphones),
         defaultVolumeLow},
        {dbPath(Setting::Volume, PlaybackType::TextMessageRingtone, Profile::Type::PlaybackBTA2DP), defaultVolumeLow},
        {dbPath(Setting::Volume, PlaybackType::TextMessageRingtone, Profile::Type::PlaybackLoudspeaker),
         defaultVolumeHigh},

        // ROUTING
        {dbPath(Setting::Gain, PlaybackType::None, Profile::Type::RoutingBTHeadset), "20"},
        {dbPath(Setting::Gain, PlaybackType::None, Profile::Type::RoutingEarspeaker), "20"},
        {dbPath(Setting::Gain, PlaybackType::None, Profile::Type::RoutingHeadphones), "20"},
        {dbPath(Setting::Gain, PlaybackType::None, Profile::Type::RoutingSpeakerphone), "20"},
        {dbPath(Setting::Gain, PlaybackType::None, Profile::Type::RoutingHeadset), "50"},

        {dbPath(Setting::Volume, PlaybackType::None, Profile::Type::RoutingBTHeadset), defaultVolumeHigh},
        {dbPath(Setting::Volume, PlaybackType::None, Profile::Type::RoutingEarspeaker), defaultVolumeHigh},
        {dbPath(Setting::Volume, PlaybackType::None, Profile::Type::RoutingHeadphones), defaultVolumeHigh},
        {dbPath(Setting::Volume, PlaybackType::None, Profile::Type::RoutingSpeakerphone), defaultVolumeHigh},
        {dbPath(Setting::Volume, PlaybackType::None, Profile::Type::RoutingHeadset), defaultVolumeHigh},

        // MISC
        {dbPath(Setting::EnableVibration, PlaybackType::Multimedia, Profile::Type::Idle), defaultFalse},
        {dbPath(Setting::EnableVibration, PlaybackType::Notifications, Profile::Type::Idle), defaultTrue},
        {dbPath(Setting::EnableVibration, PlaybackType::KeypadSound, Profile::Type::Idle), defaultFalse},
        {dbPath(Setting::EnableVibration, PlaybackType::CallRingtone, Profile::Type::Idle), defaultTrue},
        {dbPath(Setting::EnableVibration, PlaybackType::TextMessageRingtone, Profile::Type::Idle), defaultTrue},

        {dbPath(Setting::EnableSound, PlaybackType::Multimedia, Profile::Type::Idle), defaultTrue},
        {dbPath(Setting::EnableSound, PlaybackType::Notifications, Profile::Type::Idle), defaultTrue},
        {dbPath(Setting::EnableSound, PlaybackType::KeypadSound, Profile::Type::Idle), defaultTrue},
        {dbPath(Setting::EnableSound, PlaybackType::CallRingtone, Profile::Type::Idle), defaultTrue},
        {dbPath(Setting::EnableSound, PlaybackType::TextMessageRingtone, Profile::Type::Idle), defaultTrue},
    };

    for (const auto &defaultSet : defaultSettings) {
        addOrIgnoreEntry(defaultSet[IdxPath], defaultSet[IdxValue]);
    }

    return sys::ReturnCodes::Success;
}

sys::ReturnCodes ServiceAudio::DeinitHandler()
{
    return sys::ReturnCodes::Success;
}

int32_t ServiceAudio::AsyncCallback(PlaybackEvent e)
{
    switch (e.event) {
    case audio::PlaybackEventType::EndOfFile: {
        auto msg = std::make_shared<AudioNotificationMessage>(AudioNotificationMessage::Type::EndOfFile, e.token);
        sys::Bus::SendMulticast(msg, sys::BusChannels::ServiceAudioNotifications, this);
    } break;
    case audio::PlaybackEventType::FileSystemNoSpace:
    case audio::PlaybackEventType::Empty:
        break;
    }
    return 0;
};

uint32_t ServiceAudio::DbCallback(const std::string &path, const uint32_t &defaultValue)
{
    this->addOrIgnoreEntry(path, std::to_string(defaultValue));
    return this->fetchAudioSettingFromDb(path, defaultValue);
};

sys::ReturnCodes ServiceAudio::SwitchPowerModeHandler(const sys::ServicePowerMode mode)
{
    LOG_FATAL("[ServiceAudio] PowerModeHandler: %s", c_str(mode));
    return sys::ReturnCodes::Success;
}

constexpr bool ServiceAudio::IsResumable(const audio::PlaybackType &type) const
{
    return type == audio::PlaybackType::Multimedia;
}

constexpr bool ServiceAudio::ShouldLoop(const std::optional<audio::PlaybackType> &type) const
{

    return type.value_or(audio::PlaybackType::None) == audio::PlaybackType::CallRingtone;
}

// below static methods will be replaced by final vibration API
static void ExtVibrateOnce()
{
    LOG_ERROR("[Vibration] - Unimplemented - vibration one shot");
}
static void ExtVibrationStart()
{
    LOG_ERROR("[Vibration] - Unimplemented - vibration start");
}
static void ExtVibrationStop()
{
    LOG_ERROR("[Vibration] - Unimplemented - vibration stop");
}

bool ServiceAudio::IsVibrationEnabled(const audio::PlaybackType &type)
{
    auto isEnabled = utils::getValue<audio::Vibrate>(getSetting(Setting::EnableVibration, Profile::Type::Idle, type));
    return isEnabled;
}
bool ServiceAudio::IsOperationEnabled(const audio::PlaybackType &plType, const Operation::Type &opType)
{
    if (opType == Operation::Type::Router || opType == Operation::Type::Recorder) {
        return true;
    }
    auto isEnabled = utils::getValue<audio::EnableSound>(getSetting(Setting::EnableSound, Profile::Type::Idle, plType));
    return isEnabled;
}

ServiceAudio::VibrationType ServiceAudio::GetVibrationType(const audio::PlaybackType &type)
{
    if (!IsVibrationEnabled(type)) {
        return VibrationType::None;
    }

    if (type == PlaybackType::CallRingtone) {
        return VibrationType::Continuous;
    }
    else if (type == PlaybackType::Notifications || type == PlaybackType::TextMessageRingtone) {
        return VibrationType::OneShot;
    }
    return VibrationType::None;
}

void ServiceAudio::VibrationUpdate(const audio::PlaybackType &type, std::optional<AudioMux::Input *> input)
{
    auto curVibrationType = GetVibrationType(type);
    if (curVibrationType == VibrationType::OneShot && !IsVibrationMotorOn()) {
        ExtVibrateOnce();
    }
    else if (input && curVibrationType == VibrationType::Continuous) {
        input.value()->EnableVibration();
    }

    auto &inputs       = audioMux.GetAllInputs();
    auto anyOfInputsOn = std::any_of(inputs.cbegin(), inputs.cend(), [](auto &i) {
        return i.GetVibrationStatus() == AudioMux::VibrationStatus::On;
    });
    if (anyOfInputsOn && !IsVibrationMotorOn()) {
        ExtVibrationStart();
        vibrationMotorStatus = AudioMux::VibrationStatus::On;
    }
    else if (!anyOfInputsOn && IsVibrationMotorOn()) {
        ExtVibrationStop();
        vibrationMotorStatus = AudioMux::VibrationStatus::Off;
    }
}

std::unique_ptr<AudioResponseMessage> ServiceAudio::HandleGetFileTags(const std::string &fileName)
{
    if (auto tag = Audio::GetFileTags(fileName.c_str())) {
        return std::make_unique<AudioResponseMessage>(RetCode::Success, tag.value());
    }
    return std::make_unique<AudioResponseMessage>(RetCode::FileDoesntExist);
}

std::unique_ptr<AudioResponseMessage> ServiceAudio::HandlePause(const Token &token)
{
    auto input = audioMux.GetInput(token);
    return HandlePause(input);
}

void ServiceAudio::addOrIgnoreEntry(const std::string &profilePath, const std::string &defaultValue)
{
    auto [code, msg] = DBServiceAPI::GetQueryWithReply(
        this,
        db::Interface::Name::Settings_v2,
        std::make_unique<db::query::settings::AddOrIgnoreQuery>(
            SettingsRecord_v2{SettingsTableRow_v2{{.ID = DB_ID_NONE}, .path = profilePath, .value = defaultValue}}),
        audio::audioOperationTimeout);

    if (code == sys::ReturnCodes::Success && msg != nullptr) {
        auto queryResponse = dynamic_cast<db::QueryResponse *>(msg.get());
        assert(queryResponse != nullptr);

        auto settingsResultResponse = queryResponse->getResult();
        assert(dynamic_cast<db::query::settings::AddOrIgnoreResult *>(settingsResultResponse.get()) != nullptr);
    }
}

std::unique_ptr<AudioResponseMessage> ServiceAudio::HandlePause(std::optional<AudioMux::Input *> input)
{
    auto retCode  = audio::RetCode::Failed;
    auto retToken = Token();

    if (!input) {
        return std::make_unique<AudioPauseResponse>(RetCode::TokenNotFound, Token::MakeBadToken());
    }

    auto &audioInput = input.value();

    auto playbackType = audioInput->audio->GetCurrentOperationPlaybackType();
    if (IsResumable(playbackType)) {
        retCode  = audioInput->audio->Pause();
        retToken = audioInput->token;
        audioInput->DisableVibration();
    }
    else {
        retCode = audioInput->audio->Stop();
        auto broadMsg =
            std::make_shared<AudioNotificationMessage>(AudioNotificationMessage::Type::Stop, audioInput->token);
        sys::Bus::SendMulticast(broadMsg, sys::BusChannels::ServiceAudioNotifications, this);
        audioMux.ResetInput(audioInput);
    }

    VibrationUpdate();
    return std::make_unique<AudioPauseResponse>(retCode, retToken);
}

std::unique_ptr<AudioResponseMessage> ServiceAudio::HandleResume(const Token &token)
{
    if (auto input = audioMux.GetInput(token)) {
        VibrationUpdate(input.value()->audio->GetCurrentOperationPlaybackType(), input);
        return std::make_unique<AudioResumeResponse>((*input)->audio->Resume(), token);
    }
    return std::make_unique<AudioPauseResponse>(RetCode::TokenNotFound, Token::MakeBadToken());
}

std::unique_ptr<AudioResponseMessage> ServiceAudio::HandleStart(const Operation::Type opType,
                                                                const std::string fileName,
                                                                const audio::PlaybackType &playbackType)
{
    auto retCode  = audio::RetCode::Failed;
    auto retToken = Token::MakeBadToken();

    auto AudioStart = [&](auto &input) {
        if (input) {
            for (auto &audioInput : audioMux.GetAllInputs()) {
                HandlePause(&audioInput);
            }
            retToken = audioMux.ResetInput(input);

            if (IsOperationEnabled(playbackType, opType)) {
                retCode = (*input)->audio->Start(opType, retToken, fileName.c_str(), playbackType);
            }
        }

        if (retToken.IsValid()) {
            VibrationUpdate(playbackType, input);
            retCode = audio::RetCode::Success;
        }
    };

    if (opType == Operation::Type::Playback) {
        auto input = audioMux.GetPlaybackInput(playbackType);
        AudioStart(input);
        return std::make_unique<AudioStartPlaybackResponse>(retCode, retToken);
    }
    else if (opType == Operation::Type::Recorder) {
        auto input = audioMux.GetIdleInput();
        AudioStart(input);
        return std::make_unique<AudioStartRecorderResponse>(retCode, retToken);
    }
    else if (opType == Operation::Type::Router) {
        auto input = audioMux.GetRoutingInput(true);
        AudioStart(input);
        return std::make_unique<AudioStartRoutingResponse>(retCode, retToken);
    }
    return {};
}

std::unique_ptr<AudioResponseMessage> ServiceAudio::HandleSendEvent(std::shared_ptr<Event> evt)
{
    for (auto &input : audioMux.GetAllInputs()) {
        input.audio->SendEvent(evt);
    }
    return std::make_unique<AudioEventResponse>(RetCode::Success);
}

std::unique_ptr<AudioResponseMessage> ServiceAudio::HandleStop(const std::vector<audio::PlaybackType> &stopTypes,
                                                               const Token &token,
                                                               bool &muted)
{
    std::vector<std::pair<Token, audio::RetCode>> retCodes;

    auto stopInput = [this](auto inp) {
        if (inp->audio->GetCurrentState() == Audio::State::Idle) {
            return audio::RetCode::Success;
        }
        auto rCode = inp->audio->Stop();
        // Send notification that audio file was stopped
        auto msgStop = std::make_shared<AudioNotificationMessage>(AudioNotificationMessage::Type::Stop, inp->token);
        sys::Bus::SendMulticast(msgStop, sys::BusChannels::ServiceAudioNotifications, this);
        audioMux.ResetInput(inp);
        return rCode;
    };

    // stop by token
    if (auto tokenInput = audioMux.GetInput(token); token.IsValid() && tokenInput) {
        retCodes.emplace_back(std::make_pair(token, stopInput(tokenInput.value())));
    }
    else if (token.IsValid()) {
        return std::make_unique<AudioStopResponse>(RetCode::TokenNotFound, Token::MakeBadToken());
    }
    // stop with vector of playback types
    else if (!stopTypes.empty()) {
        for (auto &input : audioMux.GetAllInputs()) {
            const auto &currentOperation = input.audio->GetCurrentOperation();
            if (std::find(stopTypes.begin(), stopTypes.end(), currentOperation.GetPlaybackType()) != stopTypes.end()) {
                muted = true;
                auto t = input.token;
                retCodes.emplace_back(t, stopInput(&input));
            }
        }
    }
    // stop all audio
    else if (token.IsUninitialized()) {
        for (auto &input : audioMux.GetAllInputs()) {
            auto t = input.token;
            retCodes.emplace_back(t, stopInput(&input));
        }
    }

    // on failure return first false code
    auto it =
        std::find_if_not(retCodes.begin(), retCodes.end(), [](auto p) { return p.second == audio::RetCode::Success; });
    if (it != retCodes.end()) {
        return std::make_unique<AudioStopResponse>(it->second, it->first);
    }

    VibrationUpdate();
    return std::make_unique<AudioStopResponse>(audio::RetCode::Success, token);
}

std::unique_ptr<AudioResponseMessage> ServiceAudio::HandleStop(const std::vector<audio::PlaybackType> &stopTypes,
                                                               const Token &token)
{
    bool muted = false;
    return HandleStop(stopTypes, token, muted);
}

void ServiceAudio::HandleNotification(const AudioNotificationMessage::Type &type, const Token &token)
{
    if (type == AudioNotificationMessage::Type::EndOfFile) {
        auto input = audioMux.GetInput(token);
        if (input && ShouldLoop((*input)->audio->GetCurrentOperationPlaybackType())) {
            (*input)->audio->Start();
        }
        else {
            auto newMsg = std::make_shared<AudioStopRequest>(token);
            sys::Bus::SendUnicast(newMsg, ServiceAudio::serviceName, this);
        }
        return;
    }

    if (type != AudioNotificationMessage::Type::Stop) {
        LOG_DEBUG("Unhandled AudioNotificationMessage");
    }
}

auto ServiceAudio::HandleKeyPressed(const int step) -> std::unique_ptr<AudioKeyPressedResponse>
{
    const std::vector<audio::PlaybackType> typesToMute = {audio::PlaybackType::Notifications,
                                                          audio::PlaybackType::CallRingtone,
                                                          audio::PlaybackType::TextMessageRingtone};

    // mute if 0 and return with parameter shouldn't popup
    bool muted         = false;
    const auto context = getCurrentContext();
    if (step < 0) {
        HandleStop(typesToMute, Token(), muted);
        if (muted) {
            return std::make_unique<AudioKeyPressedResponse>(audio::RetCode::Success, 0, muted, context);
        }
    }
    const auto volume    = utils::getValue<int>(getSetting(Setting::Volume, Profile::Type::Idle, PlaybackType::None));
    const auto newVolume = std::clamp(volume + step, static_cast<int>(minVolume), static_cast<int>(maxVolume));
    setSetting(Setting::Volume, std::to_string(newVolume), Profile::Type::Idle, PlaybackType::None);
    return std::make_unique<AudioKeyPressedResponse>(audio::RetCode::Success, newVolume, muted, context);
}

sys::Message_t ServiceAudio::DataReceivedHandler(sys::DataMessage *msgl, sys::ResponseMessage *resp)
{
    sys::Message_t responseMsg;

    auto &msgType = typeid(*msgl);
    LOG_DEBUG("msgType %d %s", static_cast<int>(msgl->messageType), msgType.name());

    if (msgType == typeid(AudioNotificationMessage)) {
        auto *msg = static_cast<AudioNotificationMessage *>(msgl);
        HandleNotification(msg->type, msg->token);
    }
    else if (msgType == typeid(AudioGetSetting)) {
        auto *msg   = static_cast<AudioGetSetting *>(msgl);
        auto value  = getSetting(msg->setting, msg->profileType, msg->playbackType);
        responseMsg = std::make_shared<AudioResponseMessage>(RetCode::Success, utils::getValue<float>(value));
    }
    else if (msgType == typeid(AudioSetSetting)) {
        auto *msg = static_cast<AudioSetSetting *>(msgl);
        setSetting(msg->setting, msg->val, msg->profileType, msg->playbackType);
        responseMsg = std::make_shared<AudioResponseMessage>(RetCode::Success);
    }
    else if (msgType == typeid(AudioStopRequest)) {
        auto *msg   = static_cast<AudioStopRequest *>(msgl);
        responseMsg = HandleStop(msg->stopVec, msg->token);
    }
    else if (msgType == typeid(AudioStartPlaybackRequest)) {
        auto *msg   = static_cast<AudioStartPlaybackRequest *>(msgl);
        responseMsg = HandleStart(Operation::Type::Playback, msg->fileName.c_str(), msg->playbackType);
    }
    else if (msgType == typeid(AudioStartRecorderRequest)) {
        auto *msg   = static_cast<AudioStartRecorderRequest *>(msgl);
        responseMsg = HandleStart(Operation::Type::Recorder, msg->fileName.c_str());
    }
    else if (msgType == typeid(AudioStartRoutingRequest)) {
        LOG_DEBUG("AudioRoutingStart");
        responseMsg = HandleStart(Operation::Type::Router);
    }
    else if (msgType == typeid(AudioPauseRequest)) {
        auto *msg   = static_cast<AudioPauseRequest *>(msgl);
        responseMsg = HandlePause(msg->token);
    }
    else if (msgType == typeid(AudioResumeRequest)) {
        auto *msg   = static_cast<AudioResumeRequest *>(msgl);
        responseMsg = HandleResume(msg->token);
    }
    else if (msgType == typeid(AudioGetFileTagsRequest)) {
        auto *msg   = static_cast<AudioGetFileTagsRequest *>(msgl);
        responseMsg = HandleGetFileTags(msg->fileName);
    }
    else if (msgType == typeid(AudioEventRequest)) {
        auto *msg   = static_cast<AudioEventRequest *>(msgl);
        responseMsg = HandleSendEvent(msg->getEvent());
    }
    else if (msgType == typeid(AudioKeyPressedRequest)) {
        auto *msg   = static_cast<AudioKeyPressedRequest *>(msgl);
        responseMsg = HandleKeyPressed(msg->step);
    }
    else {
        LOG_DEBUG("Unhandled message");
    }

    if (responseMsg) {
        return responseMsg;
    }
    else {
        return std::make_shared<AudioResponseMessage>(RetCode::Failed);
    }
}

void ServiceAudio::updateDbValue(const std::string &path, const std::string &value)
{
    if (path.empty()) {
        return;
    }

    auto query = std::make_unique<db::query::settings::UpdateQuery>(
        SettingsRecord_v2({{.ID = DB_ID_NONE}, .path = path, .value = value}));

    DBServiceAPI::GetQuery(this, db::Interface::Name::Settings_v2, std::move(query));
}

std::string ServiceAudio::getSetting(const Setting &setting,
                                     const Profile::Type &profileType,
                                     const PlaybackType &playbackType)
{
    const std::string defaultValue = {};
    auto targetProfile             = profileType;
    auto targetPlayback            = playbackType;

    if (profileType == Profile::Type::Idle && playbackType == PlaybackType::None) {
        if (const auto activeInput = audioMux.GetActiveInput(); activeInput.has_value()) {
            const auto &currentOperation    = (*activeInput)->audio->GetCurrentOperation();
            const auto &currentProfile      = currentOperation.GetProfile()->GetType();
            const auto &currentPlaybackType = (*activeInput)->audio->GetCurrentOperationPlaybackType();

            targetProfile  = currentProfile;
            targetPlayback = currentPlaybackType;
        }
        else if (auto input = audioMux.GetIdleInput(); input && (setting == Setting::Volume)) {

            targetProfile = ((*input)->audio->GetHeadphonesInserted()) ? Profile::Type::PlaybackHeadphones
                                                                       : Profile::Type::PlaybackLoudspeaker;
            targetPlayback = PlaybackType::CallRingtone;
        }
        else {
            return defaultValue;
        }
    }

    if (setting == Setting::EnableVibration || setting == Setting::EnableSound) {
        targetProfile = Profile::Type::Idle;
    }

    std::string path = dbPath(setting, targetPlayback, targetProfile);
    return fetchAudioSettingFromDb(path, defaultValue);
}

void ServiceAudio::setSetting(const Setting &setting,
                              const std::string &value,
                              const Profile::Type &profileType,
                              const PlaybackType &playbackType)
{
    std::string valueToSet, path;
    auto retCode         = audio::RetCode::Success;
    auto updatedProfile  = profileType;
    auto updatedPlayback = playbackType;

    std::optional<AudioMux::Input *> activeInput;

    if (profileType == Profile::Type::Idle && playbackType == PlaybackType::None) {
        if (activeInput = audioMux.GetActiveInput(); activeInput.has_value()) {
            const auto &currentOperation = (*activeInput)->audio->GetCurrentOperation();
            updatedProfile               = currentOperation.GetProfile()->GetType();
            updatedPlayback              = (*activeInput)->audio->GetCurrentOperationPlaybackType();
        }
        else if (auto input = audioMux.GetIdleInput(); input && (setting == audio::Setting::Volume)) {
            updatedProfile = (*input)->audio->GetHeadphonesInserted() ? Profile::Type::PlaybackHeadphones
                                                                      : Profile::Type::PlaybackLoudspeaker;
            updatedPlayback = PlaybackType::CallRingtone;
            valueToSet      = std::clamp(utils::getValue<audio::Volume>(value), minVolume, maxVolume);
        }
        else {
            return;
        }
    }

    switch (setting) {
    case Setting::Volume: {
        const auto clampedValue = std::clamp(utils::getValue<audio::Volume>(value), minVolume, maxVolume);
        valueToSet              = std::to_string(clampedValue);
        if (activeInput) {
            retCode = activeInput.value()->audio->SetOutputVolume(clampedValue);
        }
    } break;
    case Setting::Gain: {
        const auto clampedValue = std::clamp(utils::getValue<audio::Gain>(value), minGain, maxGain);
        valueToSet              = std::to_string(clampedValue);
        if (activeInput) {
            retCode = activeInput.value()->audio->SetInputGain(clampedValue);
        }
    } break;
    case Setting::EnableVibration:
    case Setting::EnableSound: {
        updatedProfile = audio::Profile::Type::Idle;
        valueToSet     = value;
    } break;
    }

    if (retCode == RetCode::Success) {
        path = dbPath(setting, updatedPlayback, updatedProfile);
        updateDbValue(path, valueToSet);
    }
}

const std::pair<audio::Profile::Type, audio::PlaybackType> ServiceAudio::getCurrentContext()
{
    const auto activeInput = audioMux.GetActiveInput();
    if (!activeInput.has_value()) {
        const auto idleInput = audioMux.GetIdleInput();
        if (idleInput.has_value()) {
            return {(*idleInput)->audio->GetHeadphonesInserted() ? Profile::Type::PlaybackHeadphones
                                                                 : Profile::Type::PlaybackLoudspeaker,
                    audio::PlaybackType::CallRingtone};
        }
    }
    auto &audio                  = (*activeInput)->audio;
    const auto &currentOperation = audio->GetCurrentOperation();
    const auto currentProfile    = currentOperation.GetProfile();
    return {currentProfile->GetType(), currentOperation.GetPlaybackType()};
}
