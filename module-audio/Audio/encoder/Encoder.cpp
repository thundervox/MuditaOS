// Copyright (c) 2017-2020, Mudita Sp. z.o.o. All rights reserved.
// For licensing, see https://github.com/mudita/MuditaOS/LICENSE.md

#include "Encoder.hpp"
#include "EncoderWAV.hpp"

#include <stdint.h>
#include <string>
#include <memory>
#include <cstring>

namespace audio
{

    Encoder::Encoder(const char *fileName, const Format &frmt) : format(frmt), filePath(fileName)
    {

        fd = vfs.fopen(fileName, "w");
        if (fd == NULL) {
            return;
        }
        isInitialized = true;
    }

    Encoder::~Encoder()
    {
        if (fd) {
            vfs.fclose(fd);
        }
    }

    std::unique_ptr<Encoder> Encoder::Create(const char *file, const Format &frmt)
    {
        std::unique_ptr<Encoder> enc;
        if ((strstr(file, ".wav") != NULL) || (strstr(file, ".WAV") != NULL)) {
            enc = std::make_unique<EncoderWAV>(file, frmt);
        }
        else {
            return nullptr;
        }

        if (enc->isInitialized) {
            return enc;
        }
        else {
            return nullptr;
        }
    }

} // namespace audio
