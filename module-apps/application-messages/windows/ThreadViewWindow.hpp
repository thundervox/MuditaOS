#pragma once

#include <functional>
#include <string>

#include "AppWindow.hpp"
#include "ListView.hpp"
#include "gui/widgets/Image.hpp"
#include "gui/widgets/Label.hpp"
#include "gui/widgets/Window.hpp"
#include "service-db/api/DBServiceAPI.hpp"
#include <Text.hpp>
#include <gui/widgets/BoxLayout.hpp>

namespace gui
{

    namespace name
    {
        namespace window
        {
            inline std::string thread_view = "ThreadViewWindow";
        };
    }; // namespace name

    class ThreadViewWindow : public AppWindow
    {
      private:
        gui::VBox *body = nullptr;
        void cleanView();
        enum class Action
        {
            Start,   /// first load of sms
            Next,    /// load previous sms
            Previous /// load next sms
        };
        /// return if request was handled
        bool showMessages(Action what);
        void addSMS(Action what);
        bool smsBuild(const SMSRecord &el, bool top);
        const ssize_t maxsmsinwindow = 7;

        struct
        {
            int start = 0;                                         // actual shown position start
            int end = 7;                                           // actual shown position end
            int thread = 0;                                        // thread we are showing
            int dbsize = 0;                                        // size of elements in db
            std::unique_ptr<std::vector<SMSRecord>> sms = nullptr; // loaded sms from db
        } SMS;

      public:
        ThreadViewWindow(app::Application *app);
        virtual ~ThreadViewWindow();

        // virtual methods
        bool onInput(const InputEvent &inputEvent) override;
        void onBeforeShow(ShowMode mode, SwitchData *data) override;

        bool onDatabaseMessage(sys::Message *msgl) override;

        void rebuild() override;
        void buildInterface() override;
        void destroyInterface() override;

        gui::Text *text = nullptr;
        void rebuildText();
    };

} /* namespace gui */
