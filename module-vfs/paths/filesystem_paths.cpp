// Copyright (c) 2017-2022, Mudita Sp. z.o.o. All rights reserved.
// For licensing, see https://github.com/mudita/MuditaOS/LICENSE.md

#include <purefs/filesystem_paths.hpp>

namespace
{
    constexpr inline auto PATH_SYS          = "/sys";
    constexpr inline auto PATH_CONF         = "/mfgconf";
    constexpr inline auto PATH_USER         = "user";
    constexpr inline auto PATH_OS          = "os";
    constexpr inline auto PATH_CURRENT      = "current";
    constexpr inline auto PATH_PREVIOUS     = "previous";
    constexpr inline auto PATH_UPDATES      = "updates";
    constexpr inline auto PATH_TMP          = "tmp";
    constexpr inline auto PATH_BACKUP       = "backup";
    constexpr inline auto PATH_SYNC  = "sync";
    constexpr inline auto PATH_FACTORY      = "factory";
    constexpr inline auto PATH_LOGS         = "logs";
    constexpr inline auto PATH_CRASH_DUMPS  = "crash_dumps";
    constexpr inline auto PATH_USER_AUDIO   = "audio";
    constexpr inline auto PATH_USER_STORAGE = "storage";
    constexpr inline auto eMMC_disk         = PATH_SYS;
} // namespace

namespace purefs
{
    std::filesystem::path createPath(const std::string &parent, const std::string &child) noexcept
    {
        return std::filesystem::path{parent} / child;
    }

    namespace dir
    {
        std::filesystem::path getRootDiskPath() noexcept
        {
            return std::filesystem::path{eMMC_disk} / PATH_OS;
        }

        std::filesystem::path getMfgConfPath() noexcept
        {
            return std::filesystem::path{PATH_CONF};
        }

        std::filesystem::path getUserDiskPath() noexcept
        {
            return std::filesystem::path{eMMC_disk} / PATH_USER;
        }

        std::filesystem::path getCurrentOSPath() noexcept
        {
            return getRootDiskPath() / PATH_CURRENT;
        }

        std::filesystem::path getPreviousOSPath() noexcept
        {
            return getRootDiskPath() / PATH_PREVIOUS;
        }

        std::filesystem::path getUpdatesOSPath() noexcept
        {
            return getUserDiskPath() / PATH_UPDATES;
        }

        std::filesystem::path getTemporaryPath() noexcept
        {
            return getUserDiskPath() / PATH_TMP;
        }

        std::filesystem::path getBackupOSPath() noexcept
        {
            return getUserDiskPath() / PATH_BACKUP;
        }

        std::filesystem::path getSyncPackagePath() noexcept
        {
            return getUserDiskPath() / PATH_SYNC;
        }

        std::filesystem::path getFactoryOSPath() noexcept
        {
            return std::filesystem::path{eMMC_disk} / PATH_FACTORY;
        }

        std::filesystem::path getLogsPath() noexcept
        {
            return getUserDiskPath() / PATH_LOGS;
        }

        std::filesystem::path getCrashDumpsPath() noexcept
        {
            return getUserDiskPath() / PATH_CRASH_DUMPS;
        }
        std::filesystem::path getUserAudioPath() noexcept
        {
            return getUserDiskPath() / PATH_USER_AUDIO;
        }
        std::filesystem::path getUserStoragePath() noexcept
        {
            return getUserDiskPath() / PATH_USER_STORAGE;
        }
    } // namespace dir
} // namespace purefs
