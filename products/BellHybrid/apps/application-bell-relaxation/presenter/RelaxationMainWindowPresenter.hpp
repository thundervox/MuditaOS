// Copyright (c) 2017-2024, Mudita Sp. z.o.o. All rights reserved.
// For licensing, see https://github.com/mudita/MuditaOS/LICENSE.md

#pragma once

#include <apps-common/BasePresenter.hpp>
#include <module-db/Interface/MultimediaFilesRecord.hpp>
#include <common/models/SongsModel.hpp>
#include <vector>

namespace app::music
{
    class AbstractSongsRepository;
}
namespace app::relaxation
{

    class RelaxationMainWindowContract
    {
      public:
        class View
        {
          public:
            virtual ~View() = default;

            virtual void updateViewState()                                                             = 0;
            virtual void handleError()                                                                 = 0;
        };

        class Presenter : public BasePresenter<RelaxationMainWindowContract::View>
        {

          public:
            virtual void createData(SongsModel::OnActivateCallback activateCallback)           = 0;
            virtual void updateViewState()                                                     = 0;
            virtual void updateRecordsCount()                                                  = 0;
            virtual std::shared_ptr<SongsModel> getSongsModel()                                = 0;
        };
    };

    class RelaxationMainWindowPresenter : public RelaxationMainWindowContract::Presenter
    {
      private:
        std::shared_ptr<SongsModel> songsModel;
        void createData(SongsModel::OnActivateCallback activateCallback) override;
        void updateViewState() override;
        void updateRecordsCount() override;
        std::shared_ptr<SongsModel> getSongsModel() override;

      public:
        explicit RelaxationMainWindowPresenter(std::unique_ptr<SongsModel> songsModel);
    };

} // namespace app::relaxation
