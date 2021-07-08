// Copyright (c) 2017-2021, Mudita Sp. z.o.o. All rights reserved.
// For licensing, see https://github.com/mudita/MuditaOS/LICENSE.md

#pragma once

#include <apps-common/widgets/BarGraph.hpp>
#include <apps-common/windows/AppWindow.hpp>
#include <apps-common/windows/Dialog.hpp>
#include <module-gui/gui/widgets/BottomBar.hpp>
#include <module-gui/gui/widgets/Image.hpp>
#include <module-gui/gui/widgets/Label.hpp>
#include <module-gui/gui/widgets/TextFixedSize.hpp>

#include <vector>

namespace gui
{

    class UpdateProgressWindow : public AppWindow
    {
        unsigned int progressPercent = 0;
        std::string textInfo;

        gui::Label *percentLabel  = nullptr;
        HBarGraph *updateProgress = nullptr;

        fs::path updateFile;

        Text *text  = nullptr;
        Image *icon = nullptr;

        void setVisibleState();
        void invalidate() noexcept;

      public:
        explicit UpdateProgressWindow(app::Application *app);
        void onBeforeShow(ShowMode mode, SwitchData *data) override;
        bool onInput(const InputEvent &inputEvent) override;
        bool handleSwitchData(SwitchData *data) override;
        void rebuild() override;
        void buildInterface() override;
        void destroyInterface() override;
        status_bar::Configuration configureStatusBar(status_bar::Configuration appConfiguration) override;
    };

} /* namespace gui */
