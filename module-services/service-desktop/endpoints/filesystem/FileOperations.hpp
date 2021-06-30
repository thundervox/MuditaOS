// Copyright (c) 2017-2021, Mudita Sp. z.o.o. All rights reserved.
// For licensing, see https://github.com/mudita/MuditaOS/LICENSE.md

#pragma once

#include "FileContext.hpp"

#include <filesystem>
#include <vector>
#include <map>
#include <atomic>

class FileOperations
{
    FileOperations()  = default;
    ~FileOperations() = default;

    FileOperations(const FileOperations &) = delete;
    FileOperations &operator=(const FileOperations &) = delete;

    using transfer_id = std::uint32_t;

    std::map<transfer_id, std::unique_ptr<FileReadContext>> readTransfers;
    std::map<transfer_id, std::unique_ptr<FileWriteContext>> writeTransfers;

    std::atomic<transfer_id> runningXfrId{0};

    auto createFileReadContextFor(const std::filesystem::path &file, std::size_t fileSize, transfer_id xfrId) -> void;

    auto createFileWriteContextFor(const std::filesystem::path &file,
                                   std::size_t fileSize,
                                   const std::string Crc32,
                                   transfer_id xfrId) -> void;

    auto encodeDataAsBase64(const std::vector<std::uint8_t> &binaryData) const -> std::string;

    auto decodeDataFromBase64(const std::string &encodedData) const -> std::vector<std::uint8_t>;

    auto encodedSize(std::size_t binarySize) const -> std::size_t;

    auto decodedSize(std::size_t encodedSize) const -> std::size_t;

    auto cancelTimedOutReadTransfer() -> void;

    auto cancelTimedOutWriteTransfer() -> void;

  public:
    static constexpr auto BinToBase64Factor = 4u;
    static constexpr auto Base64ToBinFactor = 3u;
    static constexpr auto Mod3MaxReminder   = 2u;
    // ChunkSize must be a multiple of 3 and 4 so that ChunkSize % 12 == 0
    static constexpr auto ChunkSize = Base64ToBinFactor * BinToBase64Factor * 1024u;

    static FileOperations &instance();

    auto createReceiveIDForFile(const std::filesystem::path &file) -> std::pair<transfer_id, std::size_t>;

    auto getDataForReceiveID(transfer_id, std::uint32_t chunkNo) -> std::string;

    auto getFileHashForReceiveID(transfer_id rxID) -> std::string;

    auto createTransmitIDForFile(const std::filesystem::path &file, std::size_t size, const std::string Crc32)
        -> transfer_id;

    auto sendDataForTransmitID(transfer_id, std::uint32_t chunkNo, const std::string &data) -> sys::ReturnCodes;
};
