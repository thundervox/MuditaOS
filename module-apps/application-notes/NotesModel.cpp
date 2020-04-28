/*
 * @file NotesModel.cpp
 * @author Robert Borzecki (robert.borzecki@mudita.com)
 * @date 8 sie 2019
 * @brief
 * @copyright Copyright (C) 2019 mudita.com
 * @details
 */
#include "service-db/api/DBServiceAPI.hpp"
#include "widgets/NotesItem.hpp"

#include "NotesModel.hpp"

NotesModel::NotesModel(app::Application *app) : DatabaseModel(app)
{}

NotesModel::~NotesModel()
{}

void NotesModel::requestRecordsCount()
{
    uint32_t start = xTaskGetTickCount();
    recordsCount   = DBServiceAPI::NotesGetCount(application);
    uint32_t stop  = xTaskGetTickCount();
    LOG_INFO("DBServiceAPI::NotesGetCount %d records %" PRIu32 " ms", recordsCount, stop - start);

    // request first and second page if possible
    if (recordsCount > 0) {
        DBServiceAPI::NotesGetLimitOffset(application, 0, 3);
    }
}

void NotesModel::requestRecords(const uint32_t offset, const uint32_t limit)
{
    DBServiceAPI::NotesGetLimitOffset(application, offset, limit);
}

bool NotesModel::updateRecords(std::unique_ptr<std::vector<NotesRecord>> records,
                               const uint32_t offset,
                               const uint32_t limit,
                               uint32_t count)
{
#if DEBUG_DB_MODEL_DATA == 1
    LOG_DEBUG("Offset: %d, Limit: %d Count:%d", offset, limit, count);
    for (uint32_t i = 0; i < records.get()->size(); ++i) {
        LOG_DEBUG("id: %d, filename: %s", records.get()->operator[](i).ID, records.get()->operator[](i).path.c_str());
    }
#endif

    DatabaseModel::updateRecords(std::move(records), offset, limit, count);
    list->onProviderDataUpdate();

    return true;
}

gui::ListItem *NotesModel::getItem(int index)
{

    std::shared_ptr<NotesRecord> note = getRecord(index);

    SettingsRecord &settings = application->getSettings();

    if (note == nullptr)
        return nullptr;

    gui::NotesItem *item = new gui::NotesItem(this, !settings.timeFormat12);
    if (item != nullptr) {
        item->setNote(note);
        item->setID(index);
        return item;
    }

    return nullptr;
}
