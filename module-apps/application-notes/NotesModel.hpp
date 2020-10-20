// Copyright (c) 2017-2020, Mudita Sp. z.o.o. All rights reserved.
// For licensing, see https://github.com/mudita/MuditaOS/LICENSE.md

#ifndef MODULE_APPS_APPLICATION_NOTES_NOTESMODEL_HPP_
#define MODULE_APPS_APPLICATION_NOTES_NOTESMODEL_HPP_

#include <vector>

#include "Interface/NotesRecord.hpp"
#include "DatabaseModel.hpp"
#include "NotesRecord.hpp"
#include "Application.hpp"
#include "ListItemProvider.hpp"

/*
 *
 */
class NotesModel : public app::DatabaseModel<NotesRecord>, public gui::ListItemProvider
{
  public:
    NotesModel(app::Application *app);

    // virtual methods
    bool updateRecords(std::vector<NotesRecord> records) override;
    void requestRecords(const uint32_t offset, const uint32_t limit) override;

    // virtual methods for ListViewProvider
    unsigned int getMinimalItemHeight() const override;
    gui::ListItem *getItem(gui::Order order) override;
    [[nodiscard]] unsigned int requestRecordsCount() override;
};

#endif /* MODULE_APPS_APPLICATION_NOTES_NOTESMODEL_HPP_ */
