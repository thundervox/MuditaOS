// Copyright (c) 2017-2020, Mudita Sp. z.o.o. All rights reserved.
// For licensing, see https://github.com/mudita/MuditaOS/LICENSE.md

#pragma once

#include "Interface/SMSTemplateRecord.hpp"

#include <DatabaseModel.hpp>
#include <Application.hpp>
#include <ListItemProvider.hpp>

class SMSTemplateModel : public app::DatabaseModel<SMSTemplateRecord>, public gui::ListItemProvider
{
  public:
    SMSTemplateModel() = delete;
    SMSTemplateModel(app::Application *app);
    virtual ~SMSTemplateModel() = default;

    unsigned int requestRecordsCount() override;
    bool updateRecords(std::vector<SMSTemplateRecord> records) override;
    void requestRecords(const uint32_t offset, const uint32_t limit) override;

    unsigned int getMinimalItemHeight() const override;
    gui::ListItem *getItem(gui::Order order) override;
};
