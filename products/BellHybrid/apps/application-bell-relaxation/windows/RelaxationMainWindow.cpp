// Copyright (c) 2017-2024, Mudita Sp. z.o.o. All rights reserved.
// For licensing, see https://github.com/mudita/MuditaOS/LICENSE.md

#include "RelaxationMainWindow.hpp"
#include <data/RelaxationAudioData.hpp>
#include <data/RelaxationCommon.hpp>
#include <data/RelaxationErrorData.hpp>
#include <ApplicationBellRelaxation.hpp>
#include <common/options/BellOptionsNavigation.hpp>
#include <common/widgets/ListViewWithLabels.hpp>
#include <common/options/OptionBellMenu.hpp>

#include <i18n/i18n.hpp>

namespace
{
    const std::map<app::relaxation::MusicType, std::string> typeToLabel{
        {app::relaxation::MusicType::Relaxation, "app_bell_relaxation_sounds"},
        {app::relaxation::MusicType::ColorsOfNoise, "app_bell_relaxation_noises"},
        {app::relaxation::MusicType::User, "app_bell_relaxation_uploaded_sounds"}};
}

namespace gui
{
    RelaxationMainWindow::RelaxationMainWindow(
        app::ApplicationCommon *app,
        std::unique_ptr<app::relaxation::RelaxationMainWindowContract::Presenter> &&presenter)
        : AppWindow(app, gui::name::window::main_window), presenter{std::move(presenter)}
    {
        this->presenter->attach(this);
        buildInterface();
    }

    void RelaxationMainWindow::buildInterface()
    {
        AppWindow::buildInterface();

        statusBar->setVisible(false);
        header->setTitleVisibility(false);
        navBar->setVisible(false);

        songList = new ListViewWithLabels(
            this, 0, 0, style::window_width, style::window_height, presenter->getSongsModel(), typeToLabel);
        songList->applySizeRestrictions(style::bell_options_list::w,
                                        style::bell_options_list::h,
                                        style::bell_options_list::outer_layouts_h,
                                        style::bell_options_list::outer_layouts_margin);

        songList->setListTitle(utils::translate("app_bellmain_relaxation"));

        auto storedCallback     = songList->inputCallback;
        songList->inputCallback = [&, storedCallback](Item &item, const InputEvent &event) {
            return storedCallback(item, invertNavigationDirection(event));
        };
        setFocusItem(songList);

        presenter->createData([this](const db::multimedia_files::MultimediaFilesRecord &selectedSound) {
            activate(selectedSound);
            return true;
        });
    }

    void RelaxationMainWindow::activate(const db::multimedia_files::MultimediaFilesRecord &selectedSound)
    {
        auto audioContext = std::make_unique<RelaxationAudioContext>(selectedSound);
        auto switchData   = std::make_unique<RelaxationSwitchData>(std::move(audioContext));
        application->switchWindow(gui::window::name::relaxationTimerSelect, std::move(switchData));
    }

    void RelaxationMainWindow::handleError()
    {
        auto switchData = std::make_unique<RelaxationErrorData>(RelaxationErrorType::FilesLimitExceeded);
        application->switchWindow(gui::window::name::relaxationError, std::move(switchData));
    }

    void RelaxationMainWindow::updateViewState()
    {
        songList->rebuildList(gui::listview::RebuildType::InPlace);
    }
    void RelaxationMainWindow::rebuild()
    {
        presenter->updateRecordsCount();
        songList->reset();
        songList->rebuildList(gui::listview::RebuildType::Full);
    }

} // namespace gui
