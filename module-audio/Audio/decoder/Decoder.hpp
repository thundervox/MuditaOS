// Copyright (c) 2017-2021, Mudita Sp. z.o.o. All rights reserved.
// For licensing, see https://github.com/mudita/MuditaOS/LICENSE.md

#pragma once

#include "Audio/AudioCommon.hpp"
#include "Audio/Endpoint.hpp"
#include "Audio/Stream.hpp"
#include "DecoderWorker.hpp"

#include <log/log.hpp>

#include <memory>
#include <optional>
#include <string>
#include <vector>

#include <cstring>
#include <cstdint>

namespace audio
{
    namespace channel
    {
        constexpr inline auto monoSound   = 1;
        constexpr inline auto stereoSound = 2;
    } // namespace channel

    struct Tags
    {

        /* Total audio duration in seconds */
        uint32_t total_duration_s = 0;
        /* Audio duration - hours part */
        uint32_t duration_hour = 0;
        /* Audio duration - minutes part */
        uint32_t duration_min = 0;
        /* Audio duration - seconds part */
        uint32_t duration_sec = 0;

        /* Sample rate */
        uint32_t sample_rate = 0;
        /* Number of channels */
        uint32_t num_channel = 0;
        /* bitrate */
        uint32_t bitrate = 0;

        std::string artist   = "";
        std::string genre    = "";
        std::string title    = "";
        std::string album    = "";
        std::string year     = "";
        std::string filePath = "";

        Tags()
        {}

        // Copy constructor
        Tags(const Tags &p2)
        {
            total_duration_s = p2.total_duration_s;
            duration_hour    = p2.duration_hour;
            duration_min     = p2.duration_min;
            duration_sec     = p2.duration_sec;
            sample_rate      = p2.sample_rate;
            num_channel      = p2.num_channel;
            artist           = p2.artist;
            genre            = p2.genre;
            title            = p2.title;
            album            = p2.album;
            year             = p2.year;
            filePath         = p2.filePath;
        }
    };

    class Decoder : public Source
    {

      public:
        Decoder(const char *fileName);

        virtual ~Decoder();

        virtual uint32_t decode(uint32_t samplesToRead, int16_t *pcmData) = 0;

        std::unique_ptr<Tags> fetchTags();

        // Range 0 - 1
        virtual void setPosition(float pos) = 0;

        uint32_t getSampleRate()
        {
            return sampleRate;
        }

        uint32_t getChannelNumber()
        {
            return chanNumber;
        }

        float getCurrentPosition()
        {
            return position;
        }

        void onDataReceive() override;
        void enableInput() override;
        void disableInput() override;

        auto getSourceFormat() -> AudioFormat override;
        auto getSupportedFormats() -> std::vector<AudioFormat> override;

        auto getTraits() const -> Endpoint::Traits override;

        void startDecodingWorker(DecoderWorker::EndOfFileCallback endOfFileCallback);
        void stopDecodingWorker();

        // Factory method
        static std::unique_ptr<Decoder> Create(const char *file);

      protected:
        virtual auto getBitWidth() -> unsigned int = 0;
        virtual void fetchTagsSpecific(){};

        void convertmono2stereo(int16_t *pcm, uint32_t samplecount);

        static constexpr auto workerBufferSize        = 1024 * 8;
        static constexpr Endpoint::Traits decoderCaps = {.usesDMA = false};

        uint32_t sampleRate = 0;
        uint32_t chanNumber = 0;
        float position      = 0;
        std::FILE *fd       = nullptr;
        uint32_t fileSize   = 0;
        std::string filePath;

        // Worker buffer used for converting mono stream to stereo
        std::unique_ptr<int16_t[]> workerBuffer;
        std::unique_ptr<Tags> tag;
        bool isInitialized = false;

        // decoding worker
        std::unique_ptr<DecoderWorker> audioWorker;
        DecoderWorker::EndOfFileCallback _endOfFileCallback;
    };

} // namespace audio
