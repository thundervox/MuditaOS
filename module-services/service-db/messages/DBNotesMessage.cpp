﻿// Copyright (c) 2017-2020, Mudita Sp. z.o.o. All rights reserved.
// For licensing, see https://github.com/mudita/MuditaOS/LICENSE.md

#include <service-db/DBNotesMessage.hpp>
#include <service-db/DBMessage.hpp>

#include <NotesRecord.hpp>

#include <utility>

DBNotesMessage::DBNotesMessage(MessageType messageType, const NotesRecord &rec) : DBMessage(messageType), record(rec)
{}

DBNotesResponseMessage::DBNotesResponseMessage(std::unique_ptr<std::vector<NotesRecord>> rec,
                                               uint32_t retCode,
                                               uint32_t limit,
                                               uint32_t offset,
                                               uint32_t count,
                                               MessageType respTo)
    : DBResponseMessage(retCode, count, respTo), records(std::move(rec)), limit(limit), offset(offset)
{
    this->count = count;
}
