/*
 *  @file Operation.cpp
 *  @author Mateusz Piesta (mateusz.piesta@mudita.com)
 *  @date 24.07.19
 *  @brief  
 *  @copyright Copyright (C) 2019 mudita.com
 *  @details
 */



#include "Operation.hpp"

#include <algorithm>

#include "IdleOperation.hpp"
#include "PlaybackOperation.hpp"
#include "RecorderOperation.hpp"
#include "RouterOperation.hpp"


#include "bsp/audio/bsp_audio.hpp"
#include "Audio/decoder/decoder.hpp"

Operation::Operation(){}

std::optional<std::unique_ptr<Operation>> Operation::Create(Operation::Type t,const char* fileName,[[maybe_unused]]const Encoder::Format& frmt) {
    std::unique_ptr<Operation> inst ;

    switch(t){
        case Type ::Idle:
            inst = std::make_unique<IdleOperation>(fileName);
            break;
        case Type ::Playback:
            inst = std::make_unique<PlaybackOperation>(fileName);
            break;
        case Type ::Router:
            //TODO:M.P inst = std::make_unique<RouterOperation>(fileName,prof);
            break;
        case Type ::Recorder:
            inst = std::make_unique<RecorderOperation>(fileName,frmt);
            break;
    }

    if(inst->isInitialized){
        return inst;
    }
    else{
        return {};
    }
}

std::optional<Profile> Operation::GetProfile(const Profile::Type type) {
    auto ret = std::find_if(availableProfiles.begin(),availableProfiles.end(),[type](auto &w){return w.GetType() == type;});
    if(ret == availableProfiles.end()){
        return {};
    }
    else{
        return *ret;
    }
}