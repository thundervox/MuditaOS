// Copyright (c) 2017-2020, Mudita Sp. z.o.o. All rights reserved.
// For licensing, see https://github.com/mudita/MuditaOS/LICENSE.md

#pragma once

#include <Application.hpp>
#include <AppWindow.hpp>
#include <gui/widgets/BoxLayout.hpp>
#include <gui/widgets/Label.hpp>
#include <gui/widgets/Text.hpp>
#include <gui/widgets/BoxLayout.hpp>
#include <gui/widgets/ProgressBar.hpp>

#include <memory>
#include <string>

class Fota;

namespace gui
{
    namespace window
    {
        namespace name
        {
            inline constexpr auto fota_window = "fota_window";
        } // namespace name
    }     // namespace window
    /** @brief Window managing modem firmware update
     *
     */
    class FotaWindow : public AppWindow
    {
      public:
        FotaWindow(app::Application *app);
        virtual ~FotaWindow();

        void onBeforeShow(ShowMode mode, SwitchData *data) override;
        void rebuild() override;
        void buildInterface() override;
        void destroyInterface() override;

        //        Text apnName;
        //        Text apnUser;
        //        Text apnPassword;
        gui::BottomBar *getBottomBar();

      public:
        Label *addBoxLabel(BoxLayout *layout, const std::string &text);

        Label *currentFirmwareLabel   = nullptr;
        Label *statusLabel            = nullptr;
        Label *newFirmwareLabelText   = nullptr;
        Label *newFirmwareLabel       = nullptr;
        ProgressBar *downloadProgress = nullptr;
        VBox *mainBox                 = nullptr;

      private:
        void invalidate() noexcept;

        std::unique_ptr<Fota> fotaUpdater;
    };
} // namespace gui
