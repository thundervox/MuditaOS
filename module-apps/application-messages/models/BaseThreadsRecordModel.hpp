// Copyright (c) 2017-2020, Mudita Sp. z.o.o. All rights reserved.
// For licensing, see https://github.com/mudita/MuditaOS/LICENSE.md

#pragma once

#include "DatabaseModel.hpp"
#include "Application.hpp"
#include "ListItemProvider.hpp"
#include "Common/Query.hpp"
#include "Interface/ThreadRecord.hpp"

#include <vector>
#include <module-db/Interface/ContactRecord.hpp>

struct ThreadListStruct
{
    std::shared_ptr<ThreadRecord> thread;
    std::shared_ptr<ContactRecord> contact;
    std::shared_ptr<utils::PhoneNumber::View> number;

    ThreadListStruct(std::shared_ptr<ThreadRecord> thread,
                     std::shared_ptr<ContactRecord> contact,
                     std::shared_ptr<utils::PhoneNumber::View> number)
        : thread(thread), contact(contact), number(number)
    {}
};

class BaseThreadsRecordModel : public app::DatabaseModel<ThreadListStruct>, public gui::ListItemProvider
{
  public:
    BaseThreadsRecordModel() = delete;
    BaseThreadsRecordModel(app::Application *app);

    unsigned int requestRecordsCount() override;
    bool updateRecords(std::vector<ThreadListStruct> records) override;
    void requestRecords(const uint32_t offset, const uint32_t limit) override;

    app::Application *getApplication(void)
    {
        return application;
    };
};
