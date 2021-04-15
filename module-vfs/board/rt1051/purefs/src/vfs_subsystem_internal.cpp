// Copyright (c) 2017-2021, Mudita Sp. z.o.o. All rights reserved.
// For licensing, see https://github.com/mudita/MuditaOS/LICENSE.md

#include <purefs/vfs_subsystem_internal.hpp>
#include <purefs/blkdev/disk_manager.hpp>
#include <purefs/blkdev/disk_emmc.hpp>

namespace purefs::subsystem::internal
{
    auto create_default_block_device() -> std::shared_ptr<blkdev::disk>
    {
        return std::make_shared<purefs::blkdev::disk_emmc>();
    }

    auto create_default_nvm_device() -> std::shared_ptr<blkdev::disk>
    {
        return {};
    }
} // namespace purefs::subsystem::internal
