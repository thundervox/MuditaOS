// Copyright (c) 2017-2020, Mudita Sp. z.o.o. All rights reserved.
// For licensing, see https://github.com/mudita/MuditaOS/LICENSE.md

#pragma once

#include "AppWindow.hpp"
#include "gui/widgets/BottomBar.hpp"
#include "gui/widgets/Image.hpp"
#include "gui/widgets/Label.hpp"
#include "gui/widgets/Text.hpp"
#include "gui/widgets/TopBar.hpp"
#include "gui/widgets/Window.hpp"

namespace app
{
    class ApplicationDesktop;
}

namespace gui
{

    class DesktopMainWindow : public AppWindow
    {
      protected:
        gui::Label *time          = nullptr;
        gui::Label *dayText       = nullptr;
        gui::VBox *notifications  = nullptr;

        /// Timed enter value cache, could be templated to any value really
        class EnterCache
        {
            /// to tell if enter was pressed or not
            bool enterPressed = false;
            /// val to check timeout
            uint32_t unlockStartTime = 0;
            /// val to clear start time
            uint32_t unclockTime = 3000;

          public:
            bool storeEnter(const InputEvent &evt)
            {
                enterPressed    = evt.is(KeyCode::KEY_ENTER);
                unlockStartTime = xTaskGetTickCount();
                return enterPressed;
            }

            void clear()
            {
                enterPressed = false;
            }

            bool cached()
            {
                // value timed out -> clear cache
                if (!(xTaskGetTickCount() - unlockStartTime < unclockTime)) {
                    enterPressed = false;
                }
                return enterPressed;
            }
        } enter_cache;
        /**
         * Name of the appliction that was on top when lock timeout occured
         */
        std::string lockTimeoutApplilcation = "";

        // method hides or show widgets and sets bars according to provided state
        void setVisibleState();
        auto fillNotifications(app::ApplicationDesktop *app) -> bool;
        bool processLongPressEvent(const InputEvent &inputEvent);
        bool processShortPressEventOnUnlocked(const InputEvent &inputEvent);
        bool processShortPressEventOnLocked(const InputEvent &inputEvent);
        app::ApplicationDesktop *getAppDesktop() const;

      public:
        DesktopMainWindow(app::Application *app);

        // virtual methods gui::Window
        bool onInput(const InputEvent &inputEvent) override;
        void onBeforeShow(ShowMode mode, SwitchData *data) override;
        // virtual methods gui::AppWindow
        void rebuild() override;
        void buildInterface() override;
        void destroyInterface() override;
        bool updateTime(const UTF8 &timeStr) override;
        bool updateTime(const uint32_t &timestamp, bool mode24H) override;
        std::list<DrawCommand *> buildDrawList() override;

      private:
        void invalidate() noexcept;

        gui::KeyInputMappedTranslation translator;
    };

} /* namespace gui */
