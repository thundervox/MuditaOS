/*
 *  @file RouterOperation.cpp
 *  @author Mateusz Piesta (mateusz.piesta@mudita.com)
 *  @date 25.07.19
 *  @brief  
 *  @copyright Copyright (C) 2019 mudita.com
 *  @details
 */



#include "RouterOperation.hpp"

namespace audio {

    RouterOperation::RouterOperation(const char *file) {

        isInitialized = true;
    }

    int32_t RouterOperation::SetOutputVolume(float vol) {
        currentProfile->SetOutputVolume(vol);
        //audioDevice->OutputVolumeCtrl(vol);
        return static_cast<int32_t >(RetCode::Success);
    }

    int32_t RouterOperation::SetInputGain(float gain) {
        currentProfile->SetInputGain(gain);
        //audioDevice->InputGainCtrl(gain);
        return static_cast<int32_t >(RetCode::Success);
    }

}