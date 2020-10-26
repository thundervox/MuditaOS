// Copyright (c) 2017-2020, Mudita Sp. z.o.o. All rights reserved.
// For licensing, see https://github.com/mudita/MuditaOS/LICENSE.md

#include "ThreadsModel.hpp"
#include "InputEvent.hpp"
#include "OptionWindow.hpp"
#include "OptionsWindow.hpp"
#include "ListView.hpp"
#include "application-messages/data/SMSdata.hpp"
#include "application-messages/data/MessagesStyle.hpp"
#include "application-messages/widgets/ThreadItem.hpp"
#include "application-messages/windows/ThreadWindowOptions.hpp"
#include "log/log.hpp"
#include <module-services/service-db/api/DBServiceAPI.hpp>
#include <module-db/queries/messages/threads/QueryThreadsGet.hpp>
#include <module-db/queries/messages/threads/QueryThreadsGetForList.hpp>

ThreadsModel::ThreadsModel(app::Application *app) : BaseThreadsRecordModel(app)
{}

auto ThreadsModel::getMinimalItemHeight() const -> unsigned int
{
    return style::messages::threadItem::sms_thread_item_h;
}

auto ThreadsModel::getItem(gui::Order order) -> gui::ListItem *
{
    std::shared_ptr<ThreadListStruct> threadStruct = getRecord(order);

    if (!threadStruct) {
        return nullptr;
    }

    auto item = gui::ThreadItem::makeThreadItem(threadStruct);

    item->activatedCallback = [this, threadStruct](gui::Item &item) {
        LOG_INFO("ThreadItem ActivatedCallback");
        if (application) {
            const auto &threadItem = static_cast<gui::ThreadItem &>(item);
            application->switchWindow(
                gui::name::window::thread_view,
                std::make_unique<SMSThreadData>(threadStruct->thread, threadItem.getThreadName()));
        }
        else {
            LOG_ERROR("No application!");
        }
        return true;
    };

    item->inputCallback = [this, item](gui::Item &, const gui::InputEvent &event) {
        auto app = dynamic_cast<app::ApplicationMessages *>(application);
        assert(app);
        if (event.state != gui::InputEvent::State::keyReleasedShort) {
            return false;
        }
        if (event.keyCode == gui::KeyCode::KEY_LF) {
            application->switchWindow(
                utils::localize.get("app_phonebook_options_title"),
                std::make_unique<gui::OptionsWindowOptions>(threadWindowOptions(app, item->getThreadItem().get())));
        }
        return false;
    };
    return item;
}

void ThreadsModel::requestRecords(uint32_t offset, uint32_t limit)
{
    auto query = std::make_unique<db::query::ThreadsGetForList>(offset, limit);
    query->setQueryListener(
        db::QueryCallback::fromFunction([this](auto response) { return handleQueryResponse(response); }));
    DBServiceAPI::GetQuery(getApplication(), db::Interface::Name::SMSThread, std::move(query));
}

auto ThreadsModel::handleQueryResponse(db::QueryResult *queryResult) -> bool
{
    auto msgResponse = dynamic_cast<db::query::ThreadsGetForListResults *>(queryResult);
    assert(msgResponse != nullptr);

    // If list record count has changed we need to rebuild list.
    if (recordsCount != (msgResponse->getCount())) {
        recordsCount = msgResponse->getCount();
        list->rebuildList(style::listview::RebuildType::Full, 0, true);
        return false;
    }

    auto threads  = msgResponse->getResults();
    auto contacts = msgResponse->getContacts();
    auto numbers  = msgResponse->getNumbers();

    std::vector<ThreadListStruct> records;

    assert(threads.size() == contacts.size() && threads.size() == numbers.size());

    for (unsigned int i = 0; i < threads.size(); i++) {
        records.emplace_back(std::make_shared<ThreadRecord>(threads[i]),
                             std::make_shared<ContactRecord>(contacts[i]),
                             std::make_shared<utils::PhoneNumber::View>(numbers[i]));
    }

    return this->updateRecords(std::move(records));
}
