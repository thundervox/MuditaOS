// Copyright (c) 2017-2023, Mudita Sp. z.o.o. All rights reserved.
// For licensing, see https://github.com/mudita/MuditaOS/LICENSE.md

#include "PlatformFactory.hpp"
#include "init_prof.hpp"

// applications
#include <application-bell-alarm/ApplicationBellAlarm.hpp>
#include <application-bell-onboarding/ApplicationBellOnBoarding.hpp>
#include <application-bell-relaxation/ApplicationBellRelaxation.hpp>
#include <application-bell-bedtime/ApplicationBellBedtime.hpp>
#include <application-bell-main/ApplicationBellMain.hpp>
#include <application-bell-meditation-timer/MeditationTimer.hpp>
#include <application-bell-settings/ApplicationBellSettings.hpp>
#include <application-bell-powernap/ApplicationBellPowerNap.hpp>

// modules
#include <module-db/databases/MultimediaFilesDB.hpp>
#include <module-db/Interface/MultimediaFilesRecord.hpp>

// services
#include <appmgr/ApplicationManager.hpp>
#include <audio/ServiceAudio.hpp>
#include <db/ServiceDB.hpp>
#include <evtmgr/EventManager.hpp>
#include <Service/ServiceCreator.hpp>
#include <service-appmgr/Constants.hpp>
#include <service-desktop/ServiceDesktop.hpp>
#include <service-eink/ServiceEink.hpp>
#include <service-gui/ServiceGUI.hpp>
#include <service-time/ServiceTime.hpp>
#include <service-fileindexer/ServiceFileIndexer.hpp>

#include <Application.hpp>
#include <ApplicationLauncher.hpp>
#include <log/log.hpp>
#include <logdump/logdump.h>
#include <Logger.hpp>
#include <product/version.hpp>
#include <sys/SystemManager.hpp>
#include <SystemWatchdog/SystemWatchdog.hpp>
#include <thread.hpp>
#include <time/AlarmOperations.hpp>
#include <Paths.hpp>

#include <memory>
#include <vector>

#include <bsp/lpm/RT1051LPMCommon.hpp>
#include <bsp/lpm/RT1051LPM.hpp>
#include <fsl_rtwdog.h>

#if SYSTEM_VIEW_ENABLED
#include <SEGGER/SEGGER_SYSVIEW.h>
#endif

int main()
{
    constexpr auto ApplicationName = "BellHybrid";

    std::vector<std::string> fileIndexerAudioPaths = {paths::audio::proprietary(), {purefs::dir::getUserMediaPath()}};

#if SYSTEM_VIEW_ENABLED
    SEGGER_SYSVIEW_Conf();
    SEGGER_SYSVIEW_DisableEvents(SYSVIEW_EVTMASK_ISR_ENTER);
    SEGGER_SYSVIEW_DisableEvents(SYSVIEW_EVTMASK_ISR_EXIT);
    SEGGER_SYSVIEW_DisableEvents(SYSVIEW_EVTMASK_ISR_TO_SCHEDULER);
    SEGGER_SYSVIEW_WaitForConnection();
    SEGGER_SYSVIEW_Start();
#endif

    prof::init();

    auto platformFactory = bellhybrid::PlatformFactory();
    auto platform        = platformFactory.makePlatform();

    gpio_pin_config_t gpio_config = {kGPIO_DigitalOutput, 0, kGPIO_NoIntmode};

    // Level_0 = 4,
    // Level_1 = 12,
    // Level_2 = 24,
    // Level_3 = 66,
    // Level_4 = 132,
    // Level_5 = 264,
    // Level_6 = 528

    bsp::RT1051LPM cpu;
    bsp::CpuFrequencyMHz freq = bsp::CpuFrequencyMHz::Level_0;
    // constexpr std::uint32_t MHz = 1'000'000;
    // constexpr std::uint32_t MaxCpuSpeed = 528'000'000;
    GPIO_PinInit(GPIO2, 31U, &gpio_config);
    RTWDOG_Deinit(RTWDOG);

    // std::uint8_t state   = 0;
    // const uint16_t tab[] = {4, 12, 24, 66, 132, 264, 528};
    int tab2[] = {4, 528};

#define LOCAL_UART 0

#if LOCAL_UART
    bsp::board::initDebugConsole();
#endif

    int i = 0;

    while (1) {
        // int f = std::rand() % 7;
        // int f = i++ % 7;
        i++;
        freq = static_cast<bsp::CpuFrequencyMHz>(tab2[i % 2]);
        // freq = static_cast<bsp::CpuFrequencyMHz>(tab[f]);

#if LOCAL_UART
        while (!(LPUART3->STAT & LPUART_STAT_TDRE_MASK)) {}
        LPUART3->DATA = '0' + f;
#endif

        cpu.SetCpuFrequency(freq);

        // for (uint8_t idx = 0; idx < 2 * f; ++idx) {
        //     const std::uint32_t cpuSpeed = CLOCK_GetCpuClkFreq();
        //     SDK_DelayAtLeastUs(200, cpuSpeed);
        //     GPIO_PinWrite(GPIO2, 31U, state);
        //     state = !state;
        // }
#if LOCAL_UART
        LPUART3->DATA = '\n';
#endif
    }

    if (!sys::SystemWatchdog::getInstance().init()) {
        LOG_ERROR("System watchdog failed to initialize");
        // wait for the hardware watchdog (initialized in reset ISR) to reset the system
        while (true)
            ;
    }

    std::vector<std::unique_ptr<sys::BaseServiceCreator>> systemServices;
    systemServices.emplace_back(sys::CreatorFor<EventManager>([]() { return diagnosticDumpLogs(); }));
    systemServices.emplace_back(sys::CreatorFor<service::ServiceFileIndexer>(std::move(fileIndexerAudioPaths)));
    systemServices.emplace_back(sys::CreatorFor<ServiceDB>());
    systemServices.emplace_back(sys::CreatorFor<service::Audio>());
    /// Due to the problem with USB MTP not supporting hierarchical folders structure, we cannot use
    /// 'purefs::dir::getUserMediaPath()'. Instead, we can only pass a specific app folder which is very limiting.
    /// Hopefully, support for hierarchical folders will be added in the future and such a case won't be relevant
    /// anymore.
    systemServices.emplace_back(sys::CreatorFor<ServiceDesktop>(paths::audio::userApp() / paths::audio::relaxation()));
    systemServices.emplace_back(sys::CreatorFor<stm::ServiceTime>(std::make_shared<alarms::AlarmOperationsFactory>()));
    systemServices.emplace_back(sys::CreatorFor<service::eink::ServiceEink>(service::eink::ExitAction::None));
    systemServices.emplace_back(
        sys::CreatorFor<service::gui::ServiceGUI>(gui::Size{BOARD_EINK_DISPLAY_RES_X, BOARD_EINK_DISPLAY_RES_Y}));

    auto sysmgr = std::make_shared<sys::SystemManager>(std::move(systemServices));
    sysmgr->StartSystem(
        [&platform]() {
            try {
                platform->init();
            }
            catch (const std::runtime_error &e) {
                LOG_FATAL("%s", e.what());
                abort();
            }

            Log::Logger::get().init(Log::Application{ApplicationName, GIT_REV, VERSION, GIT_BRANCH});
            return true;
        },
        [sysmgr]() {
            // vector with launchers to applications
            std::vector<std::unique_ptr<app::ApplicationLauncher>> applications;
            applications.push_back(
                app::CreateLauncher<app::ApplicationBellMain>(app::applicationBellName, app::Closeable::False));
            applications.push_back(app::CreateLauncher<app::ApplicationBellSettings>(app::applicationBellSettingsName));
            applications.push_back(app::CreateLauncher<app::ApplicationBellAlarm>(app::applicationBellAlarmName));
            applications.push_back(app::CreateLauncher<app::ApplicationBellBedtime>(app::applicationBellBedtimeName));
            applications.push_back(app::CreateLauncher<app::ApplicationBellPowerNap>(app::applicationBellPowerNapName));
            applications.push_back(
                app::CreateLauncher<app::ApplicationBellOnBoarding>(app::applicationBellOnBoardingName));
            applications.push_back(
                app::CreateLauncher<app::ApplicationBellRelaxation>(app::applicationBellRelaxationName));
            applications.push_back(app::CreateLauncher<app::MeditationTimer>(app::MeditationTimer::defaultName));
            // start application manager
            return sysmgr->RunSystemService(
                std::make_shared<app::manager::ApplicationManager>(
                    service::name::appmgr, std::move(applications), app::applicationBellName),
                sysmgr.get());
        },
        [&platform] {
            try {
                LOG_DEBUG("System deinit");
                if (shutdownFlushLogs() != 1) {
                    LOG_ERROR("Cannot dump logs");
                }
                platform->deinit();
            }
            catch (const std::runtime_error &e) {
                LOG_FATAL("%s", e.what());
                abort();
            }
            sys::SystemWatchdog::getInstance().deinit();
            return true;
        });

    LOG_PRINTF("Launching %s \n", ApplicationName);
    LOG_PRINTF("commit: %s tag: %s branch: %s\n", GIT_REV, VERSION, GIT_BRANCH);
    cpp_freertos::Thread::StartScheduler();

    return 0;
}
