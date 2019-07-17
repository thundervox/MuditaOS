
/*
 * @file ATParser.cpp
 * @author Mateusz Piesta (mateusz.piesta@mudita.com)
 * @date 17.07.19
 * @brief
 * @copyright Copyright (C) 2019 mudita.com
 * @details
 */


#include "ATParser.hpp"
#include "bsp/cellular/bsp_cellular.hpp"
#include "ticks.hpp"
#include "InOutSerialWorker.hpp"

std::optional<std::unique_ptr<ATParser>>
ATParser::Create(MuxDaemon *mux, InOutSerialWorker *inOutSerial, bsp::Cellular *cellular) {
    auto inst = std::make_unique<ATParser>(mux, inOutSerial, cellular);

    if (inst->isInitialized) {
        return inst;
    } else {
        return {};
    }
}

ATParser::ATParser(MuxDaemon *mux, InOutSerialWorker *inOutSerial, bsp::Cellular *cellular) : mux(mux),inOutSerialWorker(inOutSerial),cellular(cellular) {
    isInitialized = true;
}

int ATParser::ProcessNewData() {
    char rawBuffer[256] = {0};

    auto length = cellular->Read(rawBuffer, sizeof(rawBuffer));

    if (blockedTaskHandle) {
        cpp_freertos::LockGuard lock(mutex);
        responseBuffer.append(reinterpret_cast<char *>(rawBuffer), length);
        xTaskNotifyGive(blockedTaskHandle);
    }
        // Received response data without active request, drop it
    else {
        responseBuffer.erase();
        LOG_INFO("Received unneeded data");
    }
    return 1;
}

std::vector<std::string> ATParser::SendCommand(const char *cmd, size_t rxCount, uint32_t timeout) {
    std::vector<std::string> tokens;

    blockedTaskHandle = xTaskGetCurrentTaskHandle();
    auto cmdSigned = const_cast<char *>(cmd);
    inOutSerialWorker->WriteData(reinterpret_cast<unsigned char *>(cmdSigned), strlen(cmd));

    uint32_t currentTime = cpp_freertos::Ticks::GetTicks();
    uint32_t timeoutNeeded = timeout == UINT32_MAX ? UINT32_MAX : currentTime + timeout;
    uint32_t timeElapsed = currentTime;

    wait_for_data:

    if (timeElapsed >= timeoutNeeded) {
        blockedTaskHandle = nullptr;
        return tokens;
    }

    auto ret = ulTaskNotifyTake(pdTRUE, timeoutNeeded - timeElapsed);
    timeElapsed = cpp_freertos::Ticks::GetTicks();
    if (ret) {

        std::vector<std::string> strings;

        cpp_freertos::LockGuard lock(mutex);
        //tokenize responseBuffer
        auto ret = Tokenizer(responseBuffer, rxCount, "\r\n");
        tokens.insert(std::end(tokens), std::begin(ret), std::end(ret));

        if (tokens.size() < rxCount) {
            goto wait_for_data;
        }
        blockedTaskHandle = nullptr;
        return tokens;
    } else {
        //timeout
        blockedTaskHandle = nullptr;
        return tokens;
    }
}

std::vector<std::string> ATParser::Tokenizer(std::string &input, uint32_t maxTokenCount,
                                             const std::string &delimiter) {
    std::vector<std::string> strings;
    uint32_t tokenCount = 0;


    std::string::size_type pos = 0;
    std::string::size_type prev = 0;
    while (((pos = input.find(delimiter, prev)) != std::string::npos) && (tokenCount < maxTokenCount)) {
        if (pos == prev) {
            prev = pos + delimiter.size();
            continue;
        }

        strings.push_back(input.substr(prev, pos - prev));
        prev = pos + delimiter.size();
        tokenCount++;
    }

    // To get the last substring (or only, if delimiter is not found)
    input = input.substr(prev);
    return strings;
}
