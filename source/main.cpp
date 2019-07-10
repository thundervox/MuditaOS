
#include <memory>
#include <list>

#include "../module-gui/gui/core/ImageManager.hpp"
#include "log/log.hpp"
#include "memory/usermem.h"
#include "ticks.hpp"

//module-applications
#include "application-clock/ApplicationClock.hpp"
#include "application-viewer/ApplicationViewer.hpp"
#include "application-desktop/ApplicationDesktop.hpp"

//module-services
#include "service-gui/ServiceGUI.hpp"
#include "service-gui/messages/DrawMessage.hpp"
#include "ServiceEink.hpp"
#include "service-appmgr/ApplicationManager.hpp"
#include "service-evtmgr/EventManager.hpp"
#include "service-db/ServiceDB.hpp"
#include "service-db/api/DBServiceAPI.hpp"

//module-bsp
#include "bsp.hpp"
#include "vfs.hpp"
#include "keyboard/keyboard.hpp"

#include "SystemManager/SystemManager.hpp"

#include "battery-charger/battery_charger.hpp"

class vfs vfs;

class BlinkyService : public sys::Service {
public:
    BlinkyService(const std::string& name)
            : sys::Service(name)
    {
        timer_id = CreateTimer(1000,true);
        ReloadTimer(timer_id);
    }

    ~BlinkyService(){
    }

    // Invoked upon receiving data message
    sys::Message_t DataReceivedHandler(sys::DataMessage* msgl) override{

        return std::make_shared<sys::ResponseMessage>( );
    }

    // Invoked when timer ticked
    void TickHandler(uint32_t id) override{
        LOG_DEBUG("Blinky service tick!");
    }


    // Invoked during initialization
    sys::ReturnCodes InitHandler() override{
        return sys::ReturnCodes::Success;
    }

    sys::ReturnCodes DeinitHandler() override{
        return sys::ReturnCodes::Success;
    }

    sys::ReturnCodes WakeUpHandler() override{
        return sys::ReturnCodes::Success;
    }


    sys::ReturnCodes SleepHandler() override{
        return sys::ReturnCodes::Success;
    }

    uint32_t timer_id= 0;
};


int SystemStart(sys::SystemManager* sysmgr)
{
    vfs.Init();

    bool ret;
    ret = sysmgr->CreateService(std::make_shared<sgui::ServiceGUI>("ServiceGUI", 480, 600 ),sysmgr);
    ret |= sysmgr->CreateService(std::make_shared<ServiceEink>("ServiceEink"),sysmgr);
    ret |= sysmgr->CreateService(std::make_shared<EventManager>("EventManager"),sysmgr);
    ret |= sysmgr->CreateService(std::make_shared<ServiceDB>(),sysmgr);
 //   ret |= sysmgr->CreateService(std::make_shared<BlinkyService>("Blinky"),sysmgr);

    //vector with launchers to applications
    std::vector< std::unique_ptr<app::ApplicationLauncher> > applications;

    //launcher for desktop application
    std::unique_ptr<app::ApplicationLauncher> viewerLauncher = std::unique_ptr<app::ApplicationDesktopLauncher>(new app::ApplicationDesktopLauncher());
    applications.push_back( std::move(viewerLauncher) );

    //start application manager
   ret |= sysmgr->CreateService(std::make_shared<sapm::ApplicationManager>("ApplicationManager",sysmgr,applications),sysmgr );

    if(ret){
        return 0;
    }

    return 0;
}

int main() {


	LOG_PRINTF("Launching PurePhone..\n ");

    bsp::BoardInit();

    auto sysmgr = std::make_shared<sys::SystemManager>(5000);

    sysmgr->StartSystem();

    sysmgr->RegisterInitFunction(SystemStart);

    cpp_freertos::Thread::StartScheduler();

    return 0;
}
