// Copyright (c) 2017-2020, Mudita Sp. z.o.o. All rights reserved.
// For licensing, see https://github.com/mudita/MuditaOS/LICENSE.md

#ifndef MODULE_APPS_APPLICATION_CALL_WINDOWS_CALLMAINWINDOW_HPP_
#define MODULE_APPS_APPLICATION_CALL_WINDOWS_CALLMAINWINDOW_HPP_

#include "AppWindow.hpp"
#include "Label.hpp"

namespace gui
{

    /*
     * This window is defined only to keep common approach to creating application.
     * This window should not be displayed.
     */
    class CallMainWindow : public AppWindow
    {
      protected:
      public:
        CallMainWindow(app::Application *app);
        virtual ~CallMainWindow();

        void rebuild() override;
        void buildInterface() override;
        void destroyInterface() override;
    };

} /* namespace gui */

#endif /* MODULE_APPS_APPLICATION_CALL_WINDOWS_CALLMAINWINDOW_HPP_ */
