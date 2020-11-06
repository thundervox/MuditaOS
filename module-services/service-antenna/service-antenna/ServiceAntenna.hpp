﻿// Copyright (c) 2017-2020, Mudita Sp. z.o.o. All rights reserved.
// For licensing, see https://github.com/mudita/MuditaOS/LICENSE.md

#pragma once

#include <bsp/cellular/bsp_cellular.hpp>
#include <MessageType.hpp>
#include <module-utils/state/ServiceState.hpp>
#include <Service/Common.hpp>
#include <Service/Message.hpp>
#include <Service/Service.hpp>
#include <Service/Worker.hpp>

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <memory>

namespace sys
{
    class Timer;
} // namespace sys
namespace utils
{
    namespace state
    {
        template <typename T> class State;
    } // namespace state
} // namespace utils

namespace antenna
{
    enum class State
    {
        none,
        init,
        connectionStatus,
        bandCheck,
        switchAntenna,
        signalCheck,
        idle,
        csqChange,
        locked
    };

    const char *c_str(antenna::State state);

    constexpr uint32_t signalTreshold          = 10;
    constexpr uint32_t connectionStatusTimeout = 60000;

    enum class lockState
    {
        locked,
        unlocked
    };
} // namespace antenna

class ServiceAntenna : public sys::Service
{
  private:
    utils::state::State<antenna::State> *state;
    bool HandleStateChange(antenna::State state);

    std::unique_ptr<sys::Timer> timer;

    bsp::cellular::antenna currentAntenna;
    uint32_t lastCsq    = 0;
    uint32_t currentCsq = 0;

    antenna::lockState serviceLocked = antenna::lockState::unlocked;

    void handleLockRequest(antenna::lockState request);

  protected:
    // flag informs about suspend/resume status
    bool suspended = false;

  public:
    static const char *serviceName;

    ServiceAntenna();
    ~ServiceAntenna();

    sys::MessagePointer DataReceivedHandler(sys::DataMessage *msgl, sys::ResponseMessage *resp) override;

    // Invoked during initialization
    sys::ReturnCodes InitHandler() override;

    sys::ReturnCodes DeinitHandler() override;

    sys::ReturnCodes SwitchPowerModeHandler(const sys::ServicePowerMode mode) override final;

    void storeCurrentState(void);

    bool initStateHandler(void);
    bool noneStateHandler(void);
    bool connectionStatusStateHandler(void);
    bool switchAntennaStateHandler(void);
    bool signalCheckStateHandler(void);
    bool bandCheckStateHandler(void);
    bool idleStateHandler(void);
    bool csqChangeStateHandler(void);
    bool lockedStateHandler(void);
};
