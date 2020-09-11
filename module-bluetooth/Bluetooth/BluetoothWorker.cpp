#include "BluetoothWorker.hpp"
#include "BtCommand.hpp"
#include "log/log.hpp"
#include "module-bluetooth/Bluetooth/interface/profiles/A2DP/A2DP.hpp"

extern "C"
{
#include "module-bluetooth/lib/btstack/src/btstack_util.h"
};

using namespace bsp;

const char *c_str(Bt::Error::Code code)
{
    switch (code) {
    case Bt::Error::Code::Success:
        return "Success";
    case Bt::Error::Code::NotReady:
        return "NotReady";
    case Bt::Error::Code::SystemError:
        return "SystemError";
    case Bt::Error::Code::LibraryError:
        return "LibraryError";
    }
    return "";
}

BluetoothWorker::BluetoothWorker(sys::Service *service) : Worker(service), currentProfile(std::make_shared<Bt::A2DP>())
{
    init({
        {"qBtIO", sizeof(Bt::Message), 10},
        {"qBtWork", sizeof(Bt::EvtWorker), 10},
    });
};

BluetoothWorker::~BluetoothWorker()
{
    if (this->bt_worker_task != nullptr) {
        vTaskDelete(this->bt_worker_task);
    }
    LOG_INFO("Worker removed");
}

bool BluetoothWorker::run()
{
    LOG_INFO("-> BluetoothWorker run request");
    if (is_running) {
        return true;
    }
    if (Worker::run()) {
        is_running                          = true;
        auto el                             = queues[queueIO_handle];
        BlueKitchen::getInstance()->qHandle = el;
        Bt::initialize_stack();
        Bt::register_hw_error_callback();
        Bt::GAP::register_scan();
        std::string name = "PurePhone";
        Bt::set_name(name);
        // set local namne
        Bt::GAP::set_visibility(true);
        // Bt::GAP::
        Bt::run_stack(&this->bt_worker_task);
        return true;
    }
    else {
        return false;
    }
}

bool BluetoothWorker::scan()
{
    std::vector<Device> empty;
    Bt::GAP::setOwnerService(service);
    auto ret = Bt::GAP::scan();
    if (ret.err != Bt::Error::Success) {
        LOG_ERROR("Cant start scan!: %s %" PRIu32 "", c_str(ret.err), ret.lib_code);
        return false;
    }
    else {
        LOG_INFO("Scan started!");
        // open new scan window
        return true;
    }
}

void BluetoothWorker::stop_scan()
{
    Bt::GAP::stop_scan();
}

bool BluetoothWorker::set_visible()
{
    static bool visibility = true;
    Bt::GAP::set_visibility(visibility);
    visibility = !visibility;

    return false;
}

bool BluetoothWorker::start_pan()
{
    Bt::PAN::bnep_setup();
    auto err = Bt::PAN::bnep_start();
    if (err.err != Bt::Error::Success) {
        LOG_ERROR("PAN setup error: %s %" PRIu32, c_str(err.err), err.lib_code);
    }
    return false;
}

BluetoothWorker::Error BluetoothWorker::aud_init()
{
    LOG_INFO("%s", __PRETTY_FUNCTION__);
    Error err = SuccessBt;
    LOG_INFO("AUDIO - TODO");
    // start GAVD
    // &&  ASSIGN_CLASS_OF_DEVICE(ClassOfDevice, 0x28, 0x04, 0x10);
    return err;
}

#include <sstream>

bool BluetoothWorker::handleMessage(uint32_t queueID)
{

    QueueHandle_t queue = queues[queueID];
    if (queueID == queueService) {
        LOG_DEBUG("not interested");
        return true;
    }
    if (queueID != queueIO_handle) {
        LOG_ERROR("Wrong queue! %" PRIu32, queueID);
        return false;
    }

    Bt::Message notification = Bt::Message::EvtErrorRec;
    if (xQueueReceive(queue, &notification, 0) != pdTRUE) {
        LOG_ERROR("Receive failure!");
        return false;
    }
    auto bt = BlueKitchen::getInstance();
    switch (notification) {
    case Bt::Message::EvtSent:
#ifdef DO_DEBUG_HCI_COMS
        LOG_INFO("[evt] sent");
#endif
        if (bt->write_done_cb) {
            bt->write_done_cb();
        }
        break;
    case Bt::Message::EvtReceived: {
        if (bt->to_read_req > bt->in.len) {
            // LOG_ERROR("%d vs %d", bt->to_read_req, bt->in.len);
            break;
        }
        for (int i = 0; i < bt->to_read_req; ++i) {
            // error in pop should never happen
            if (int ret = bt->in.pop((char *)bt->read_buff + i)) {
                LOG_ERROR("This shall never happen: %d", ret);
            }
        }
        // LOG_DEBUG(">> %d",bt->in.len);
#ifdef DO_DEBUG_HCI_COMS
        std::stringstream ss;
        for (int i = 0; i < bt->to_read_req; ++i) {
            ss << " 0x" << std::hex << (int)*(bt->read_buff + i);
        }
        LOG_DEBUG("[evt] recieved <-- [%d]>%s<", bt->to_read_req, ss.str().c_str());
#endif
        bt->to_read_req = 0;

        if (bt->read_ready_cb) {
            bt->read_ready_cb();
        }
    } break;
    case Bt::Message::EvtSentError:
    case Bt::Message::EvtRecError:
    case Bt::Message::EvtUartError:
    case Bt::Message::EvtRecUnwanted: {
        LOG_ERROR("Uart error [%d]: %s", notification, Bt::MessageCstr(notification));
        break;
    } break;
    default:
        LOG_ERROR("ERROR");
    }

    return true;
}

void BluetoothWorker::initAudioBT()
{}

bool BluetoothWorker::play_audio()
{
    auto profile = dynamic_cast<Bt::A2DP *>(currentProfile.get());
    if (profile == nullptr) {
        return false;
    }

    profile->init();
    profile->setOwnerService(service);
    profile->start();
    return true;
}
bool BluetoothWorker::stop_audio()
{
    auto profile = dynamic_cast<Bt::A2DP *>(currentProfile.get());
    if (profile == nullptr) {
        return false;
    }

    profile->stop();
    return true;
}
void BluetoothWorker::set_addr(bd_addr_t addr)
{
    Bt::GAP::do_pairing(addr);
    auto profile = dynamic_cast<Bt::A2DP *>(currentProfile.get());
    if (profile != nullptr) {
        profile->setDeviceAddress(addr);
    }
}
