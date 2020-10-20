// Copyright (c) 2017-2020, Mudita Sp. z.o.o. All rights reserved.
// For licensing, see https://github.com/mudita/MuditaOS/LICENSE.md

#include <module-services/service-db/messages/QueryMessage.hpp>
#include <module-services/service-db/api/DBServiceAPI.hpp>
#include <module-db/queries/messages/sms/QuerySMSGetCountByThreadID.hpp>
#include <module-db/queries/messages/sms/QuerySMSGetForList.hpp>

#include <application-messages/widgets/SMSOutputWidget.hpp>
#include <module-apps/application-messages/ApplicationMessages.hpp>
#include "application-messages/data/MessagesStyle.hpp"
#include "SMSThreadModel.hpp"
#include "ListView.hpp"

SMSThreadModel::SMSThreadModel(app::Application *app) : DatabaseModel(app)
{
    smsInput = new gui::SMSInputWidget(application);
}

SMSThreadModel::~SMSThreadModel()
{
    delete smsInput;
}

unsigned int SMSThreadModel::getMinimalItemHeight() const
{
    return style::messages::smsOutput::default_h;
}

gui::ListItem *SMSThreadModel::getItem(gui::Order order)
{
    std::shared_ptr<SMSRecord> sms = getRecord(order);

    if (sms == nullptr) {
        return nullptr;
    }

    // Small hack to trick current model logic -> adding empty row into query result for Input Widget
    if (sms->type == SMSType::INPUT) {
        addReturnNumber();
        return smsInput;
    }

    return new gui::SMSOutputWidget(application, sms);
}

unsigned int SMSThreadModel::requestRecordsCount()
{
    return recordsCount;
}

void SMSThreadModel::requestRecords(uint32_t offset, uint32_t limit)
{
    auto query = std::make_unique<db::query::SMSGetForList>(smsThreadID, offset, limit);
    query->setQueryListener(
        db::QueryCallback::fromFunction([this](auto response) { return handleQueryResponse(response); }));
    DBServiceAPI::GetQuery(application, db::Interface::Name::SMS, std::move(query));
}

bool SMSThreadModel::updateRecords(std::vector<SMSRecord> records)
{
    DatabaseModel::updateRecords(std::move(records));
    list->onProviderDataUpdate();
    return true;
}

auto SMSThreadModel::handleQueryResponse(db::QueryResult *queryResult) -> bool
{
    auto msgResponse = dynamic_cast<db::query::SMSGetForListResult *>(queryResult);
    assert(msgResponse != nullptr);

    auto records_data = msgResponse->getResults();

    // If list record count has changed we need to rebuild list.
    if (recordsCount != (msgResponse->getCount() + 1)) {
        // Additional one element for SMSInputWidget.
        recordsCount = msgResponse->getCount() + 1;
        list->rebuildList(style::listview::RebuildType::Full, 0, true);
        return false;
    }

    resetInputWidget();

    if (msgResponse->getDraft().isValid() && smsInput->inputText->isEmpty()) {
        smsInput->draft = msgResponse->getDraft().type == SMSType::DRAFT
                              ? std::optional<SMSRecord>{msgResponse->getDraft()}
                              : std::nullopt;
        smsInput->displayDraftMessage();
    }

    return this->updateRecords(std::move(records_data));
}

void SMSThreadModel::addReturnNumber()
{
    if (number != nullptr) {
        smsInput->number = std::move(number);
    }

    smsInput->activatedCallback = [this]([[maybe_unused]] gui::Item &item) {
        auto app = dynamic_cast<app::ApplicationMessages *>(application);
        assert(app != nullptr);
        assert(smsInput->number != nullptr);
        if (app->handleSendSmsFromThread(*smsInput->number, smsInput->inputText->getText())) {
            LOG_ERROR("handleSendSmsFromThread failed");
        }
        smsInput->inputText->clear();
        smsInput->clearDraftMessage();
        return true;
    };
}

void SMSThreadModel::handleDraftMessage()
{
    smsInput->handleDraftMessage();
}

void SMSThreadModel::resetInputWidget()
{
    smsInput->setFocus(false);
    smsInput->setVisible(true);
    smsInput->clearNavigationItem(gui::NavigationDirection::UP);
    smsInput->clearNavigationItem(gui::NavigationDirection::DOWN);
}
