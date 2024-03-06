// Copyright (c) 2017-2024, Mudita Sp. z.o.o. All rights reserved.
// For licensing, see https://github.com/mudita/MuditaOS/LICENSE.md

#include "RelaxationSongsModel.hpp"
#include <common/options/OptionBellMenu.hpp>
#include <common/widgets/LabelOption.hpp>

namespace app::relaxation
{
    RelaxationSongsProvider::RelaxationSongsProvider(ApplicationCommon *app) : DatabaseModel(app)
    {}

    RelaxationSongsModel::RelaxationSongsModel(ApplicationCommon *application,
                                               std::unique_ptr<AbstractSoundsRepository> soundsRepository,
                                               const std::map<int, std::string> &pathPrefixes)
        : RelaxationSongsProvider(application),
          application(application), songsRepository{std::move(soundsRepository)}, pathPrefixes{pathPrefixes}
    {}

    void RelaxationSongsModel::createData(OnActivateCallback callback)
    {
        activateCallback = callback;

        songsRepository->init();
    }

    void RelaxationSongsModel::updateRecordsCount()
    {
        songsRepository->updateFilesCount();
    }

    unsigned int RelaxationSongsModel::requestRecordsCount()
    {
        return songsRepository->getFilesCount();
    }
    unsigned int RelaxationSongsModel::getMinimalItemSpaceRequired() const
    {
        return style::bell_options::h + 2 * style::bell_options::option_margin;
    }
    bool RelaxationSongsModel::nextRecordExist(gui::Order order)
    {
        const auto getOppositeOrder = [order]() {
            return order == gui::Order::Next ? gui::Order::Previous : gui::Order::Next;
        };

        auto exist = getRecord(order) != nullptr;
        getRecord(getOppositeOrder());
        return exist;
    }
    gui::ListItem *RelaxationSongsModel::getItem(gui::Order order)
    {
        const auto sound = getRecord(order);
        if (!sound) {
            return nullptr;
        }
        auto item = gui::option::LabelOption{getTypeFromPath(sound->fileInfo.path),
                                                  sound->tags.title,
                                                  [=]([[maybe_unused]] gui::Item &item) {
                                                      activateCallback(*sound);
                                                      return true;
                                                  },
                                                  []([[maybe_unused]] gui::Item &item) { return true; },
                                                  nullptr};

        return item.build();
    }
    void RelaxationSongsModel::requestRecords(std::uint32_t offset, std::uint32_t limit)
    {
        songsRepository->getMusicFiles(
            offset,
            limit,
            [this](const std::vector<db::multimedia_files::MultimediaFilesRecord> &records,
                   unsigned int repoRecordsCount) { return onMusicListRetrieved(records, repoRecordsCount); });
    }
    bool RelaxationSongsModel::onMusicListRetrieved(
        const std::vector<db::multimedia_files::MultimediaFilesRecord> &records, unsigned int repoRecordsCount)
    {
        if (list != nullptr && recordsCount != repoRecordsCount) {
            recordsCount = repoRecordsCount;
            list->reSendLastRebuildRequest();
            return false;
        }
        return updateRecords(records);
    }
    bool RelaxationSongsModel::updateRecords(std::vector<db::multimedia_files::MultimediaFilesRecord> records)
    {
        DatabaseModel::updateRecords(std::move(records));
        list->onProviderDataUpdate();
        return true;
    }
    gui::ItemsType RelaxationSongsModel::getTypeFromPath(const std::string &path)
    {
        for (const auto &[type, pathPrefix] : pathPrefixes) {
            if (path.find(pathPrefix) != std::string::npos) {
                return type;
            }
        }
        return std::nullopt;
    }
} // namespace app::relaxation
