// Copyright (c) 2017-2023, Mudita Sp. z.o.o. All rights reserved.
// For licensing, see https://github.com/mudita/MuditaOS/LICENSE.md

#include "RT1051LPM.hpp"
#include "WfiController.hpp"

namespace bsp
{
    void RT1051LPM::EnableDcdcPowerSaveMode()
    {}

    void RT1051LPM::DisableDcdcPowerSaveMode()
    {}

    void RT1051LPM::AllowEnteringWfiMode()
    {
        allowEnteringWfiMode();
    }

    void RT1051LPM::BlockEnteringWfiMode()
    {
        blockEnteringWfiMode();
    }

    std::uint32_t RT1051LPM::EnterWfiModeIfAllowed()
    {
        RT1051LPMCommon::SetWaitModeConfig();
        const auto timeSpentInWFI = enterWfiModeIfAllowed();
        RT1051LPMCommon::SetRunModeConfig();
        return timeSpentInWFI;
    }

    std::uint32_t RT1051LPM::GetLastTimeSpentInWfi()
    {
        return getLastTimeSpentInWfi();
    }
} // namespace bsp
