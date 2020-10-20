// Copyright (c) 2017-2020, Mudita Sp. z.o.o. All rights reserved.
// For licensing, see https://github.com/mudita/MuditaOS/LICENSE.md

#include "SMSInputWidget.hpp"
#include "AppWindow.hpp"
#include "application-messages/data/MessagesStyle.hpp"
#include <module-apps/application-messages/ApplicationMessages.hpp>

#include <Style.hpp>
#include <i18/i18.hpp>
#include <Font.hpp>
#include <utility>
#include <algorithm>

#include "TextParse.hpp"

namespace gui
{

    SMSInputWidget::SMSInputWidget(app::Application *application) : application(application)
    {
        setMinimumSize(style::window::default_body_width, style::messages::smsInput::min_h);
        setMargins(Margins(0, style::messages::smsInput::new_sms_vertical_spacer, 0, 0));
        setEdges(gui::RectangleEdge::None);

        body = new HBox(this, 0, 0, 0, 0);
        body->setEdges(RectangleEdge::Bottom);
        body->setMaximumSize(style::window::default_body_width, style::messages::smsInput::max_input_h);

        deleteByList = false;

        inputText = new gui::Text(body, 0, 0, 0, 0, "", ExpandMode::EXPAND_UP);
        inputText->setMaximumSize(style::messages::smsInput::default_input_w, style::messages::smsInput::max_input_h);
        inputText->setMinimumSize(style::messages::smsInput::default_input_w,
                                  style::messages::smsInput::default_input_h);
        inputText->setFont(style::window::font::medium);
        inputText->setPadding(Padding(0, 0, 0, style::messages::smsInput::bottom_padding));
        inputText->setPenFocusWidth(style::window::default_border_focus_w);
        inputText->setPenWidth(style::window::default_border_focus_w);
        inputText->setEdges(gui::RectangleEdge::None);

        replyImage = new Image(body, 0, 0, "messages_reply");
        replyImage->setAlignment(Alignment(gui::Alignment::Vertical::Bottom));
        replyImage->setMargins(Margins(0, 0, 0, style::messages::smsInput::reply_bottom_margin));
        replyImage->activeItem = false;

        inputCallback = [&]([[maybe_unused]] Item &item, const InputEvent &event) { return inputText->onInput(event); };

        focusChangedCallback = [this]([[maybe_unused]] Item &item) {
            setFocusItem(focus ? body : nullptr);
            return true;
        };

        inputText->inputCallback = [this, application]([[maybe_unused]] Item &, const InputEvent &event) {
            if (event.state == InputEvent::State::keyReleasedShort && event.keyCode == KeyCode::KEY_LF) {
                auto app = dynamic_cast<app::ApplicationMessages *>(application);
                assert(app != nullptr);
                return app->newMessageOptions(application->getCurrentWindow()->getName(), inputText);
            }
            return false;
        };

        inputText->focusChangedCallback = [this, application]([[maybe_unused]] Item &) -> bool {
            assert(body != nullptr);
            assert(application != nullptr);

            if (inputText->focus) {

                application->getWindow(gui::name::window::thread_view)
                    ->setBottomBarText(utils::localize.get("sms_reply"), BottomBar::Side::CENTER);

                inputText->setInputMode(new InputMode(
                    {InputMode::ABC, InputMode::abc, InputMode::digit},
                    [=](const UTF8 &Text) { application->getCurrentWindow()->bottomBarTemporaryMode(Text); },
                    [=]() { application->getCurrentWindow()->bottomBarRestoreFromTemporaryMode(); },
                    [=]() { application->getCurrentWindow()->selectSpecialCharacter(); }));

                if (inputText->getText() == utils::localize.get("sms_temp_reply")) {
                    inputText->clear();
                }
            }
            else {

                if (inputText->isEmpty()) {

                    // Temporary solution to be fixed when proper Text Color handling will be added.
                    auto format = TextFormat(Font(27).raw(), Color(7, 0));
                    for (auto &el : textToTextBlocks(utils::localize.get("sms_temp_reply"), format)) {
                        inputText->addText(el);
                    }
                }

                application->getWindow(gui::name::window::thread_view)->clearBottomBarText(BottomBar::Side::CENTER);
            }

            return true;
        };
    }

    void SMSInputWidget::handleDraftMessage()
    {
        if (const auto &text = inputText->getText(); text.empty() || (text == utils::localize.get("sms_temp_reply"))) {
            clearDraftMessage();
        }
        else {
            updateDraftMessage(text);
        }
    }

    void SMSInputWidget::clearDraftMessage()
    {
        if (!draft.has_value()) {
            displayDraftMessage();
            return;
        }

        auto app = dynamic_cast<app::ApplicationMessages *>(application);
        assert(app != nullptr);
        if (const auto removed = app->removeDraft(draft.value()); removed) {
            draft = std::nullopt;
            displayDraftMessage();
        }
    }

    void SMSInputWidget::displayDraftMessage() const
    {
        if (draft.has_value()) {
            inputText->setText(draft->body);
        }
        else {
            inputText->clear();
        }
    }

    void SMSInputWidget::updateDraftMessage(const UTF8 &inputText)
    {
        auto app = dynamic_cast<app::ApplicationMessages *>(application);
        assert(app != nullptr);
        assert(number != nullptr);

        if (draft.has_value()) {
            app->updateDraft(draft.value(), inputText);
        }
        else {
            const auto &[draft, success] = app->createDraft(*number, inputText);
            if (success) {
                this->draft = draft;
            }
        }
    }

    auto SMSInputWidget::onDimensionChanged(const BoundingBox &oldDim, const BoundingBox &newDim) -> bool
    {
        body->setPosition(0, 0);
        body->setSize(newDim.w, newDim.h);

        return true;
    }

    auto SMSInputWidget::handleRequestResize([[maybe_unused]] const Item *child,
                                             unsigned short request_w,
                                             unsigned short request_h) -> Size
    {
        request_h =
            std::clamp((Length)request_h, style::messages::smsInput::min_h, style::messages::smsInput::max_input_h);

        setMinimumHeight(request_h);
        if (parent != nullptr) {
            requestSize(request_w, request_h);
        }

        return Size(request_w, request_h);
    }

} /* namespace gui */
