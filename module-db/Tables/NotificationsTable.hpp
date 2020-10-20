// Copyright (c) 2017-2020, Mudita Sp. z.o.o. All rights reserved.
// For licensing, see https://github.com/mudita/MuditaOS/LICENSE.md

#pragma once

#include "Table.hpp"
#include "Record.hpp"
#include "Database/Database.hpp"
#include "utf8/UTF8.hpp"
#include "Common/Common.hpp"

struct NotificationsTableRow : public Record
{
    uint32_t key   = 0;
    uint32_t value = 0;
};

enum class NotificationsTableFields
{
    key
};

class NotificationsTable : public Table<NotificationsTableRow, NotificationsTableFields>
{
  public:
    NotificationsTable(Database *db);
    virtual ~NotificationsTable() = default;

    bool create() override final;
    bool add(NotificationsTableRow entry) override final;
    bool removeById(uint32_t id) override final;
    bool removeByField(NotificationsTableFields field, const char *str) override final;
    bool update(NotificationsTableRow entry) override final;
    NotificationsTableRow getById(uint32_t id) override final;
    NotificationsTableRow GetByKey(uint32_t key);
    uint32_t count() override final;
    uint32_t countByFieldId(const char *field, uint32_t id) override final;
    std::vector<NotificationsTableRow> getLimitOffset(uint32_t offset, uint32_t limit) override final;
    std::vector<NotificationsTableRow> getLimitOffsetByField(uint32_t offset,
                                                             uint32_t limit,
                                                             NotificationsTableFields field,
                                                             const char *str) override final;

  private:
    const char *createTableQuery = "CREATE TABLE IF NOT EXISTS notifications("
                                   "_id INTEGER PRIMARY KEY,"
                                   "key INTEGER UNIQUE DEFAULT 0,"
                                   "value INTEGER DEFAULT 0)";
};
