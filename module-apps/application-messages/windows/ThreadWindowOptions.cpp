// Copyright (c) 2017-2020, Mudita Sp. z.o.o. All rights reserved.
// For licensing, see https://github.com/mudita/MuditaOS/LICENSE.md

#include "ThreadWindowOptions.hpp"
#include "i18/i18.hpp"
#include "log/log.hpp"
#include <Options.hpp>
#include <OptionWindow.hpp>

/// below just for apps names...

std::list<gui::Option> threadWindowOptions(app::ApplicationMessages *app, const ThreadRecord *record)
{
    assert(record != nullptr);
    ContactRecord contact =
        record ? DBServiceAPI::ContactGetByIDWithTemporary(app, record->contactID)->front() : ContactRecord();
    std::list<gui::Option> options;

    options.emplace_back(gui::options::call(app, app::CallOperation::ExecuteCall, contact));
    auto contactOperation = contact.isTemporary() ? app::ContactOperation::Add : app::ContactOperation::Details;
    options.emplace_back(gui::options::contact(app, contactOperation, contact));

    if (record->isUnread()) {
        options.emplace_back(gui::Option{utils::localize.get("sms_mark_read"), [=](gui::Item &item) {
                                             app->markSmsThreadAsRead(record->ID);
                                             app->returnToPreviousWindow();
                                             return true;
                                         }});
    }
    else {
        options.emplace_back(gui::Option{utils::localize.get("sms_mark_unread"), [=](gui::Item &item) {
                                             app->markSmsThreadAsUnread(record->ID);
                                             app->returnToPreviousWindow();
                                             return true;
                                         }});
    }

    options.emplace_back(gui::Option{utils::localize.get("sms_delete_conversation"), [=](gui::Item &item) {
                                         LOG_INFO("Removing sms thread!");
                                         return app->removeSmsThread(record);
                                     }});

    // TODO
    // shouldn't this be in show contact details actually? it would be much easier too
    // {utils::localize.get("sms_add_to_contacts"), [=](gui::Item &item) { return true; }},

    return options;
};
