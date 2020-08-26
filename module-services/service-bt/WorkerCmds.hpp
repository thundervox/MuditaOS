#pragma once

#include <string>
#include <FreeRTOS.h>
#include <FreeRTOS/include/task.h>

class BtData
{
    public:
        virtual ~BtData() = default;
};

struct BtCmd
{
    enum class Cmd {
        BtOpen,
        BtClose,
        Init,
        TimerPoll,
        Write,
        Read,
    } cmd;
    BtData *ptr = nullptr;
    // this can be done in element copy -> so dtor won't fit
    void cleanup() { if (ptr!= nullptr) {delete ptr;} }
};

class BtWrite : public BtData
{
    public:
        BtWrite(BtInject::Command cmd) : command(cmd) {}
        BtInject::Command command;
};

class BtRead : public BtData
{
    public:
        std::list<char> data;
        bool timed_out = false;
        uint32_t to_read_count;
        TaskHandle_t read_handle = nullptr;
};
