// Copyright (c) 2017-2020, Mudita Sp. z.o.o. All rights reserved.
// For licensing, see https://github.com/mudita/MuditaOS/LICENSE.md

#include "CallLogOptionsWindow.hpp"
#include "i18/i18.hpp"
#include "log/log.hpp"
#include <Options.hpp>
#include <module-services/service-db/service-db/DBServiceAPI.hpp>

/// below just for apps names...

std::list<gui::Option> calllogWindowOptions(app::ApplicationCallLog *app, const CalllogRecord &record)
{
    auto searchResults = DBServiceAPI::ContactGetByIDWithTemporary(app, record.getContactId());

    std::list<gui::Option> options;

    if (searchResults->empty() || !searchResults->front().isValid() || searchResults->front().isTemporary()) {
        // add option - add contact
        options.push_back(gui::options::contact(app, gui::options::ContactOperation::Add, searchResults->front()));
    }
    else {
        // add option - contact details
        options.push_back(gui::options::contact(app, gui::options::ContactOperation::Details, searchResults->front()));
    }

    // add option delete call option
    options.push_back(gui::Option(
        utils::localize.get("app_calllog_options_delete_call"),
        [=](gui::Item &item) { return app->removeCalllogEntry(record); },
        gui::Arrow::Disabled));

    return options;
};
