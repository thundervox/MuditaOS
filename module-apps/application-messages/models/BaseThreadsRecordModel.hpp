#pragma once

#include "DatabaseModel.hpp"
#include "Application.hpp"
#include "ListItemProvider.hpp"
#include "Common/Query.hpp"
#include "Interface/ThreadRecord.hpp"

#include <vector>

class BaseThreadsRecordModel : public app::DatabaseModel<ThreadRecord>, public gui::ListItemProvider
{
  public:
    BaseThreadsRecordModel() = delete;
    BaseThreadsRecordModel(app::Application *app);

    unsigned int requestRecordsCount() override;
    bool updateRecords(std::vector<ThreadRecord> records) override;
    void requestRecords(const uint32_t offset, const uint32_t limit) override;

    app::Application *getApplication(void)
    {
        return application;
    };
};
