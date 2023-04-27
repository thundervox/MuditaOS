// Copyright (c) 2017-2023, Mudita Sp. z.o.o. All rights reserved.
// For licensing, see https://github.com/mudita/MuditaOS/LICENSE.md

#include <serial-number-parser/SerialNumberParser.hpp>
#include <purefs/vfs_subsystem.hpp>
#include <Utils.hpp>
#include <crc32.h>
#include <cstring>

/*
 * Currently BOOT2 eMMC partition holds only device's serial number. The data are
 * just raw ASCII digits occupying first 13 bytes of second block. To add
 * information about case colour, following data frame was implemented:
 *
 *       13 bytes (ASCII)      var. length ASCII  8 bytes (ASCII hex)      1 byte
 * |----- Serial number -----|----- Colour -----|----- Checksum -----|----- 0x00 -----|
 * |                                                                                  |
 * 0                                                                       13 + len(colour) + 8 + 1
 *
 * This structure enables checking whether the frame is valid and update it in case the device
 * lacks colour info (mechanism required to patch the devices already released to customers).
 * Information about colour is in such case obtained by checking substring of serial number
 * (workingly called "ID string"), which consists of 6 first characters.
 *
 * The entire SN is in MMYYWWNNNNNNN form, where:
 * MM - model code, 04 for Harmony;
 * YY - production year;
 * WW - production week;
 * NNNNNNN - unique device number, defined per batch.
 *
 * First 6 characters are in reality just model code, production year and production week.
 * The substrings of all batches already released to customers and their respective colours
 * are stored in idStringToColourMap variable below. When the algorithm detects that the
 * frame in BOOT2 is invalid, it tries to obtain the desired colour for the batch the device
 * is from. If that succeeds, the colour is appended to the serial number to form the payload.
 * In the next step CRC32 checksum is computed and appended to the payload to form complete
 * frame, which is then written to the eMMC BOOT2 partition.
 *
 * If the colour cannot be found then the algorithm bails out without changing anything.
 * In such case obviously only info about serial number is available.
 *
 * The ASCII encoding of all the data stored in frame was chosen on purpose, so that typical
 * C-style null terminator can be used to mark the end of frame, what simplifies its usage,
 * it can be handled just like regular string.
 *
 * After the updated, valid frame is successfully written back to the eMMC card, next boot
 * of the device will result in successful frame validation and just readout of the
 * device's metadata.
 *
 */

namespace serial_number_parser
{
    namespace
    {
        /* Block device constants */
        constexpr auto blkdevName  = "emmc0sys1";
        constexpr auto blockSize   = 512;
        constexpr auto blockToRead = 2;
        constexpr auto blocksCount = 1;

        /* Frame constants */
        constexpr auto checksumLength     = 8;  // CRC32 checksum is stored as ASCII hex chars, after colour
        constexpr auto serialNumberOffset = 0;  // SN is first in frame
        constexpr auto serialNumberLength = 13; // SN is always 13 chars
        constexpr auto colourOffset       = serialNumberLength;                               // Colour comes after SN
        constexpr auto maxColourLength = blockSize - serialNumberLength - checksumLength - 1; // -1 for null-terminator

        /* Serial number constants */
        constexpr auto idStringOffset = 0;
        constexpr auto idStringLength = 6;

        // TODO check if these are all possibilities
        /* ID string to colour map for the already released batches */
        const std::map<std::string, std::string> idStringToColourMap{
            {"042148", "gray"}, {"042213", "gray"}, {"042242", "black"}};

        bool readMetadataBlock(char *block)
        {
            if (block == nullptr) {
                return false;
            }

            const auto diskManager = purefs::subsystem::disk_mgr();
            if (diskManager == nullptr) {
                return false;
            }

            const auto status = diskManager->read(blkdevName, block, blockToRead, blocksCount);
            return (status == 0);
        }

        bool writeMetadataBlock(const char *block)
        {
            if (block == nullptr) {
                return false;
            }

            const auto diskManager = purefs::subsystem::disk_mgr();
            if (diskManager == nullptr) {
                return false;
            }

            const auto status = diskManager->write(blkdevName, block, blockToRead, blocksCount);
            return (status == 0);
        }

        bool isChecksumValid(const char *block)
        {
            if (block == nullptr) {
                return false;
            }

            const auto frameLength = strnlen(block, blockSize);
            if ((frameLength == blockSize) || (frameLength <= (serialNumberLength + checksumLength))) {
                return false;
            }

            const auto payloadLength = frameLength - checksumLength;
            const auto realChecksum  = &block[payloadLength];

            CRC32 crc32;
            crc32.add(block, payloadLength);

            return (crc32.getHash() == realChecksum);
        }

        std::string getDesiredColour(const std::string &serialNumber)
        {
            const auto idString = serialNumber.substr(idStringOffset, idStringLength);
            const auto item     = idStringToColourMap.find(idString);
            if (item == idStringToColourMap.end()) {
                return "";
            }

            return item->second;
        }

        bool createMetadataBlock(char *block, const std::string &serialNumber, const std::string &colour)
        {
            if (block == nullptr) {
                return false;
            }

            if (colour.empty() || (colour.length() > maxColourLength) ||
                (serialNumber.length() != serialNumberLength)) {
                return false;
            }

            std::string frame = serialNumber;
            frame += colour;

            CRC32 crc32;
            crc32.add(frame.c_str(), frame.length());
            frame += crc32.getHash();

            std::memset(block, 0, blockSize);
            std::memcpy(block, frame.c_str(), frame.length());

            LOG_DEBUG("Created block: %s", block);

            return true;
        }

        std::string readSerialNumber(const char *block) {
            const auto frame = std::string(block);
            return frame.substr(serialNumberOffset, serialNumberLength);
        }

        std::string readColour(const char *block)
        {
            const auto frame = std::string(block);
            const auto colourLength = frame.length() - serialNumberLength - checksumLength;
            return frame.substr(colourOffset, colourLength);
        }

        bool isLineZeros(char *line, size_t length)
        {
            for (size_t i = 0; i < length; ++i) {
                if (line[i] != 0) {
                    return false;
                }
            }
            return true;
        }

        void logFrame(char *block)
        {
            char buf[serialNumberLength];

            size_t length;
            for (size_t i = 0; i < blockSize; i += length) {
                const auto bytesLeft = blockSize - i;
                length               = std::min(static_cast<unsigned>(serialNumberLength), bytesLeft);
                if (isLineZeros(&block[i], length)) {
                    memset(buf, '0', length);
                }
                else {
                    memcpy(buf, &block[i], length);
                }
                printf("%03u: %.*s", i, length, buf);
            }
        }
    } // namespace

    bool removeUpdatedFrame()
    {
        char block[blockSize];

        auto status = readMetadataBlock(block);
        if (!status) {
            LOG_ERROR("Error reading metadata block!");
            return false;
        }

        if (!isChecksumValid(block)) {
            printf("\nInvalid frame checksum, probably device not patched yet.\n"
                   "To make sure, check the metadata block layout displayed below.\n"
                   "For the unpatched device, first line should contain serial number,\n"
                   "others should be all zeros.\n"
                   "Last line should contain only 5 characters.");
            logFrame(block);
            printf("\n");
            return false;
        }

        LOG_INFO("Detected valid frame, erasing...");
        memset(&block[serialNumberLength], 0, blockSize - serialNumberLength);
        status = writeMetadataBlock(block);
        if (!status) {
            printf("Error writing updated metadata block!");
            return false;
        }

        printf("\nDone! Check current metadata block layout displayed below.\n"
               "First line should contain valid SN, others should be all zeros.\n"
               "Last line should contain only 5 characters.");
        logFrame(block);
        printf("\n");

        return true;
    }

    std::pair<std::string, std::string> getOrUpdateDeviceMetadata()
    {
        char block[blockSize];

        auto status = readMetadataBlock(block);
        if (!status) {
            LOG_ERROR("Error reading metadata block!");
            return {"", ""};
        }

        const auto serialNumber = readSerialNumber(block);

        if (isChecksumValid(block)) {
            LOG_INFO("eMMC metadata frame valid");
            const auto colour = readColour(block);
            return {serialNumber, colour};
        }

        LOG_ERROR("Invalid eMMC metadata frame! Trying to update...");

        const auto desiredColour = getDesiredColour(serialNumber);
        if (desiredColour.empty()) {
            LOG_ERROR("Desired colour for serial number '%s' not defined, metadata update failed!",
                      serialNumber.c_str());
            return {serialNumber, ""};
        }

        status = createMetadataBlock(block, serialNumber, desiredColour);
        if (!status) {
            LOG_ERROR("Pair '%s', '%s' does not represent valid data, metadata update failed!",
                      serialNumber.c_str(),
                      desiredColour.c_str());
            return {serialNumber, desiredColour};
        }

        status = writeMetadataBlock(block);
        if (!status) {
            LOG_ERROR("Error writing eMMC block, metadata update failed!");
            return {serialNumber, desiredColour};
        }

        LOG_INFO(
            "Metadata updated successfully, values written: '%s', '%s'", serialNumber.c_str(), desiredColour.c_str());
        return {serialNumber, desiredColour};
    }
}
