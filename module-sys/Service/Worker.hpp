// Copyright (c) 2017-2020, Mudita Sp. z.o.o. All rights reserved.
// For licensing, see https://github.com/mudita/MuditaOS/LICENSE.md

#pragma once

#include "Service.hpp"

#include <memory>
#include <map>
#include <string>
#include <vector>

namespace sys
{

    class WorkerQueueInfo
    {
      public:
        std::string name;
        int elementSize;
        int length;
    };

    struct WorkerCommand
    {
        uint32_t command = 0;
        uint32_t *data   = NULL;
    };

    /*
     * @brief Worker is a wrapper for freeRTOS task used to separate sysmanager environment from
     * the rest of the system. Its purpose is to handle asynchronous events like IRQ or timers.
     * Flow of creating worker is as follows:
     * - create new Worker object and provide pointer to the service that owns the worker,
     * - call init method and provide list of parameters to create queues. Those queues can be later
     * used to wake up the worker.
     * - call run method to start the worker.
     *
     * Flow for closing the worker is as follows:
     * - call stop method - task will end itself
     * - call join method to wait for the task to end
     * - call deinit to destroy all resources owned by the worker
     * - delete the object.
     *
     */
    class Worker
    {
      private:
        enum class ControlMessage
        {
            Stop,
            MessageCount
        };

        enum class State
        {
            New,
            Initiated,
            Running,
            Stopping,
            Stopped,
            Destroyed,
            Invalid
        };

        using Id = unsigned int;

        static void taskAdapter(void *taskParam);
        bool handleControlMessage();
        void task();
        void addQueueInfo(xQueueHandle queue, std::string queueName);
        void setState(State newState);
        std::string getControlQueueName() const;

        static constexpr std::size_t controlMessagesCount = static_cast<std::size_t>(ControlMessage::MessageCount);
        static constexpr std::size_t defaultStackSize     = 2048;
        static constexpr TickType_t defaultJoinTimeout    = portMAX_DELAY;
        static inline std::string controlQueueNamePrefix  = "wctrl";

        xQueueHandle controlQueue      = nullptr;
        xSemaphoreHandle joinSemaphore = nullptr;
        xTaskHandle runnerTask         = nullptr;
        xSemaphoreHandle stateMutex    = nullptr;
        xTaskHandle taskHandle         = nullptr;

        Id id;
        std::string name;
        State state = State::New;

      protected:
        virtual bool handleMessage(uint32_t queueID) = 0;
        xQueueHandle getQueueByName(std::string queueName);
        bool sendControlMessage(ControlMessage message);
        State getState() const;

        const static uint32_t SERVICE_QUEUE_LENGTH = 10;
        const static uint32_t CONTROL_QUEUE_LENGTH = 4;
        const static uint32_t SERVICE_QUEUE_SIZE   = sizeof(WorkerCommand);
        const std::string SERVICE_QUEUE_NAME       = "ServiceQueue";

        static unsigned int count;

        sys::Service *service     = nullptr;
        xQueueHandle serviceQueue = nullptr;
        QueueSetHandle_t queueSet = nullptr;
        std::vector<xQueueHandle> queues;
        std::map<xQueueHandle, std::string> queueNameMap;

      public:
        Worker(sys::Service *service);
        virtual ~Worker();

        /**
         * @brief This function is responsible for creating all queues provided in the constructor.
         * When all queues are created this method creates set of queues.
         */
        virtual bool init(std::list<WorkerQueueInfo> queues = std::list<WorkerQueueInfo>());
        /**
         * @brief This function is responsible for destroying all resources created in the
         * init mehtod.
         */
        virtual bool deinit();
        /**
         * @brief Starts RTOS thread that waits for incoming queue events.
         */
        virtual bool run();
        /**
         * @brief Sends stop command to worker.
         */
        virtual bool stop();
        /**
         * @brief Joins the thread
         *
         * @param timeout - ticks to wait for the thread to end
         */
        bool join(TickType_t timeout = defaultJoinTimeout);
        /**
         * @brief Sends command and pointer to data to worker
         */
        virtual bool send(uint32_t cmd, uint32_t *data);
        /**
         * @brief Closes worker by combining stop, join and deinit operations in a single call.
         * If it is not possible to close the worker gently it would kill it forcibly.
         */
        void close();
        /**
         * @brief Kills the worker. Does not deinit it.
         */
        void kill();
    };

} /* namespace sys */
