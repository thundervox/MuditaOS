#include "PhonebookContact.hpp"
#include "../ApplicationPhonebook.hpp"
#include "../data/PhonebookInternals.hpp"
#include "InputEvent.hpp"
#include "Label.hpp"
#include "Margins.hpp"
#include "PhonebookNewContact.hpp"
#include "Text.hpp"
#include "UiCommonActions.hpp"
#include "Utils.hpp"
#include "application-call/ApplicationCall.hpp"
#include "application-messages/ApplicationMessages.hpp"
#include "application-messages/windows/ThreadViewWindow.hpp"
#include "i18/i18.hpp"
#include "service-appmgr/ApplicationManager.hpp"
#include "service-db/api/DBServiceAPI.hpp"
#include <log/log.hpp>
#include <limits.h>

namespace gui
{

    PhonebookContact::PhonebookContact(app::Application *app) : AppWindow(app, window::name::contact)
    {
        buildInterface();
    }

    void PhonebookContact::rebuild()
    {
        destroyInterface();
        buildInterface();
    }

    Label *PhonebookContact::addLabel(std::list<Item *> *parentPage,
                                      int x,
                                      int y,
                                      int w,
                                      int h,
                                      const std::string text         = "",
                                      const std::string fontName     = style::window::font::small,
                                      const RectangleEdgeFlags edges = RectangleEdgeFlags::GUI_RECT_EDGE_NO_EDGES,
                                      const Alignment alignment      = Alignment(Alignment::ALIGN_HORIZONTAL_LEFT,
                                                                            Alignment::ALIGN_VERTICAL_BOTTOM),
                                      const bool lineMode            = false)
    {
        Label *l = new Label(this, x, y, w, h);
        l->setFilled(false);
        l->setBorderColor(ColorFullBlack);
        l->setEdges(edges);
        l->setFont(fontName);
        l->setText(text);
        l->setAlignment(alignment);
        l->setLineMode(lineMode);

        if (parentPage)
            parentPage->push_back(l);

        return (l);
    }

    void PhonebookContact::buildInterface()
    {
        AppWindow::buildInterface();
        topBar->setActive(TopBar::Elements::TIME, true);

        bottomBar->setActive(BottomBar::Side::LEFT, true);
        bottomBar->setActive(BottomBar::Side::CENTER, true);
        bottomBar->setActive(BottomBar::Side::RIGHT, true);
        bottomBar->setText(BottomBar::Side::LEFT, utils::localize.get(style::strings::common::options));
        bottomBar->setText(BottomBar::Side::CENTER, utils::localize.get("app_phonebook_call"));
        bottomBar->setText(BottomBar::Side::RIGHT, utils::localize.get("app_phonebook_back"));

        favouritesIcon  = new Image(this, 97, 107, 32, 32, "small_heart");
        favouritesLabel = addLabel(nullptr,
                                   65,
                                   144,
                                   89,
                                   20,
                                   utils::localize.get("app_phonebook_contact_favourites_upper"),
                                   style::window::font::smallbold,
                                   RectangleEdgeFlags::GUI_RECT_EDGE_NO_EDGES,
                                   Alignment(Alignment::ALIGN_HORIZONTAL_CENTER, Alignment::ALIGN_VERTICAL_CENTER));

        speedDial = addLabel(nullptr,
                             225,
                             107,
                             32,
                             32,
                             "",
                             style::footer::font::bold,
                             RectangleEdgeFlags::GUI_RECT_EDGE_NO_EDGES,
                             Alignment(Alignment::ALIGN_HORIZONTAL_CENTER, Alignment::ALIGN_VERTICAL_CENTER));

        speedDialLabel = addLabel(nullptr,
                                  196,
                                  144,
                                  89,
                                  20,
                                  utils::localize.get("app_phonebook_contact_speed_dial_upper"),
                                  style::window::font::smallbold,
                                  RectangleEdgeFlags::GUI_RECT_EDGE_NO_EDGES,
                                  Alignment(Alignment::ALIGN_HORIZONTAL_CENTER, Alignment::ALIGN_VERTICAL_CENTER));

        blockedIcon  = new Image(this, 351, 107, 32, 32, "small_circle");
        blockedLabel = addLabel(nullptr,
                                329,
                                144,
                                75,
                                20,
                                utils::localize.get("app_phonebook_contact_speed_blocked_uppper"),
                                style::window::font::smallbold,
                                RectangleEdgeFlags::GUI_RECT_EDGE_NO_EDGES,
                                Alignment(Alignment::ALIGN_HORIZONTAL_CENTER, Alignment::ALIGN_VERTICAL_CENTER));

        // page1 contents
        informationLabel = addLabel(&page1,
                                    30,
                                    203,
                                    413,
                                    20,
                                    utils::localize.get("app_phonebook_contact_information"),
                                    style::window::font::small,
                                    RectangleEdgeFlags::GUI_RECT_EDGE_NO_EDGES,
                                    Alignment(Alignment::ALIGN_HORIZONTAL_LEFT, Alignment::ALIGN_VERTICAL_BOTTOM),
                                    true);

        // first number line
        numberPrimary = addLabel(&page1,
                                 30,
                                 249,
                                 178,
                                 33,
                                 "",
                                 style::window::font::bigbold,
                                 RectangleEdgeFlags::GUI_RECT_EDGE_NO_EDGES,
                                 Alignment(Alignment::ALIGN_HORIZONTAL_CENTER, Alignment::ALIGN_VERTICAL_CENTER));

        numberPrimaryLabel                = addLabel(&page1,
                                      328,
                                      237,
                                      32,
                                      53,
                                      "",
                                      style::window::font::small,
                                      RectangleEdgeFlags::GUI_RECT_EDGE_BOTTOM | RectangleEdgeFlags::GUI_RECT_EDGE_TOP,
                                      Alignment(Alignment::ALIGN_HORIZONTAL_CENTER, Alignment::ALIGN_VERTICAL_CENTER));
        numberPrimaryLabel->inputCallback = [=](Item &item, const InputEvent &input) {
            if (input.keyCode == KeyCode::KEY_ENTER && input.state == InputEvent::State::keyReleasedShort) {
                return app::call(application, contact->numbers[0].numberE164);
            }
            LOG_DEBUG("numberPrimayLabel->inputCallback just return false");
            return (false);
        };

        numberPrimaryLabel->setPenFocusWidth(3);
        numberPrimaryLabel->setPenWidth(0);

        numberPrimaryIcon = new Image(this, 328, 249, 32, 32, "phonebook_phone_ringing");
        page1.push_back(numberPrimaryIcon);

        numberPrimaryMessageLabel =
            addLabel(&page1,
                     401,
                     237,
                     32,
                     53,
                     "",
                     style::window::font::small,
                     RectangleEdgeFlags::GUI_RECT_EDGE_BOTTOM | RectangleEdgeFlags::GUI_RECT_EDGE_TOP,
                     Alignment(Alignment::ALIGN_HORIZONTAL_CENTER, Alignment::ALIGN_VERTICAL_CENTER));

        numberPrimaryMessageLabel->setPenFocusWidth(3);
        numberPrimaryMessageLabel->setPenWidth(0);
        numberPrimaryMessageLabel->activatedCallback = [=](Item &item) {
            LOG_ERROR("TODO missing support for libphonenumber in phonebook, simply open new messagew window");
            return app::sms(
                application, app::SmsOperation::New, utils::PhoneNumber::View()); // get number from primary number
        };

        numberPrimaryMessage = new Image(this, 401, 248, 32, 32, "mail");
        page1.push_back(numberPrimaryMessage);

        // second number line
        numberSecondary = addLabel(&page1,
                                   30,
                                   306,
                                   178,
                                   33,
                                   "",
                                   style::window::font::big,
                                   RectangleEdgeFlags::GUI_RECT_EDGE_NO_EDGES,
                                   Alignment(Alignment::ALIGN_HORIZONTAL_CENTER, Alignment::ALIGN_VERTICAL_CENTER));

        numberSecondaryLabel =
            addLabel(&page1,
                     328,
                     297,
                     32,
                     53,
                     "",
                     style::window::font::small,
                     RectangleEdgeFlags::GUI_RECT_EDGE_BOTTOM | RectangleEdgeFlags::GUI_RECT_EDGE_TOP,
                     Alignment(Alignment::ALIGN_HORIZONTAL_CENTER, Alignment::ALIGN_VERTICAL_CENTER));
        numberSecondaryLabel->setPenFocusWidth(3);
        numberSecondaryLabel->setPenWidth(0);
        numberSecondaryLabel->inputCallback = [=](Item &item, const InputEvent &input) {
            if (input.keyCode == KeyCode::KEY_ENTER) {
                return app::call(application, contact->numbers[1].numberE164);
            }
            return (false);
        };

        numberSecondaryIcon = new Image(this, 328, 308, 32, 32, "phonebook_phone_ringing");
        page1.push_back(numberSecondaryIcon);

        numberSecondaryMessageLabel =
            addLabel(&page1,
                     401,
                     297,
                     32,
                     53,
                     "",
                     style::window::font::small,
                     RectangleEdgeFlags::GUI_RECT_EDGE_BOTTOM | RectangleEdgeFlags::GUI_RECT_EDGE_TOP,
                     Alignment(Alignment::ALIGN_HORIZONTAL_CENTER, Alignment::ALIGN_VERTICAL_CENTER));
        numberSecondaryMessageLabel->setPenFocusWidth(3);
        numberSecondaryMessageLabel->setPenWidth(0);
        numberSecondaryMessageLabel->activatedCallback = [=](Item &item) {
            LOG_ERROR("TODO missing support for libphonenumber in phonebook, simply open new messagew window");
            return app::sms(
                application, app::SmsOperation::New, utils::PhoneNumber::View()); // get number from secondary number
        };

        numberSecondaryMessage = new Image(this, 401, 306, 32, 32, "mail");
        page1.push_back(numberSecondaryMessage);
        email = addLabel(&page1,
                         30,
                         363,
                         413,
                         33,
                         "",
                         style::window::font::big,
                         RectangleEdgeFlags::GUI_RECT_EDGE_NO_EDGES,
                         Alignment(Alignment::ALIGN_HORIZONTAL_LEFT, Alignment::ALIGN_VERTICAL_CENTER));

        addressLabel = addLabel(&page1,
                                30,
                                429,
                                413,
                                20,
                                utils::localize.get("app_phonebook_contact_address"),
                                style::window::font::small,
                                RectangleEdgeFlags::GUI_RECT_EDGE_NO_EDGES,
                                Alignment(Alignment::ALIGN_HORIZONTAL_LEFT, Alignment::ALIGN_VERTICAL_BOTTOM),
                                true);

        address = addLabel(&page1,
                           30,
                           475,
                           422,
                           33,
                           "",
                           style::window::font::big,
                           RectangleEdgeFlags::GUI_RECT_EDGE_NO_EDGES,
                           Alignment(Alignment::ALIGN_HORIZONTAL_LEFT, Alignment::ALIGN_VERTICAL_CENTER));

        noteLabel = addLabel(&page2,
                             30,
                             203,
                             413,
                             20,
                             utils::localize.get("app_phonebook_contact_note"),
                             style::window::font::small,
                             RectangleEdgeFlags::GUI_RECT_EDGE_NO_EDGES,
                             Alignment(Alignment::ALIGN_HORIZONTAL_LEFT, Alignment::ALIGN_VERTICAL_BOTTOM),
                             true);

        noteText = new Text(this, 30, 249, 422, 600 - 249 - bottomBar->getHeight());
        noteText->setEdges(RectangleEdgeFlags::GUI_RECT_EDGE_NO_EDGES);
        noteText->setFont(style::window::font::small);
        page2.push_back(noteText);
        noteText->focusChangedCallback = [=](gui::Item &item) {
            setVisible(&page2, item.focus);
            setVisible(&page1, !item.focus);
            setContactData();
            application->refreshWindow(RefreshModes::GUI_REFRESH_FAST);
            return (true);
        };

        // naviagation
        numberPrimaryLabel->setNavigationItem(NavigationDirection::DOWN, numberPrimaryMessageLabel);
        numberPrimaryMessageLabel->setNavigationItem(NavigationDirection::UP, numberPrimaryLabel);
        numberPrimaryMessageLabel->setNavigationItem(NavigationDirection::DOWN, numberSecondaryLabel);
        numberSecondaryLabel->setNavigationItem(NavigationDirection::UP, numberPrimaryMessageLabel);
        numberSecondaryLabel->setNavigationItem(NavigationDirection::DOWN, numberSecondaryMessageLabel);
        numberSecondaryMessageLabel->setNavigationItem(NavigationDirection::UP, numberSecondaryLabel);
        numberSecondaryMessageLabel->setNavigationItem(NavigationDirection::DOWN, noteText);
        noteText->setNavigationItem(NavigationDirection::UP, numberSecondaryMessageLabel);
        setFocusItem(numberPrimaryLabel);
        setVisible(&page2, false);
    }

    void PhonebookContact::setVisible(std::list<Item *> *page, bool shouldBeVisible)
    {
        for (auto i : *page) {
            i->setVisible(shouldBeVisible);
        }
    }

    void PhonebookContact::destroyInterface()
    {
        erase();
        // data was erased, now we need to clear page so that we won't use old pointers
        page1.clear();
        page2.clear();
    }

    PhonebookContact::~PhonebookContact()
    {
        destroyInterface();
    }

    void PhonebookContact::onBeforeShow(ShowMode mode, SwitchData *data)
    {}

    ContactRecord PhonebookContact::readContact()
    {
        ContactRecord ret;
        return ret;
    }

    void PhonebookContact::setContactData()
    {
        if (contact == nullptr)
            return;

        setTitle(contact->getFormattedName(ContactRecord::NameFormatType::Title));

        auto isSpeedDialInRange = [&](const UTF8 &speedDialStr) {
            if (speedDialStr.length() == 0)
                return false;

            char *endPtr      = nullptr;
            long speedDialInt = std::strtol(speedDialStr.c_str(), &endPtr, 10);
            if (endPtr == speedDialStr.c_str()) {
                LOG_ERROR("%s: not a decimal number", speedDialStr.c_str());
                return false;
            }
            else if ('\0' != *endPtr) {
                LOG_ERROR("%s: extra characters at end of input: %s\n", speedDialStr.c_str(), endPtr);
                return false;
            }
            else if ((LONG_MIN == speedDialInt || LONG_MAX == speedDialInt) && ERANGE == errno) {
                LOG_ERROR("%s out of range of type long\n", speedDialStr.c_str());
                return false;
            }
            else if (speedDialInt > phonebookInternals::speedDialMaxValue) {
                LOG_ERROR("%ld greater than speedDialMaxValue\n", speedDialInt);
                return false;
            }
            return true;
        };

        speedDial->setText(isSpeedDialInRange(contact->speeddial) ? contact->speeddial : UTF8(""));

        if (!contact->isOnFavourites) {
            LOG_INFO("setContactData contact %s is not on fav list", contact->primaryName.c_str());
            favouritesIcon->setVisible(false);
            favouritesLabel->setVisible(false);
        }
        else {
            LOG_INFO("setContactData contact %s is on fav list", contact->primaryName.c_str());
            favouritesIcon->setVisible(true);
            favouritesLabel->setVisible(true);
        }

        if (!contact->isOnBlacklist) {
            blockedLabel->setVisible(false);
            blockedIcon->setVisible(false);
        }
        else {
            blockedLabel->setVisible(true);
            blockedIcon->setVisible(true);
        }

        if (contact->numbers.size() == 0) {
            numberPrimary->setVisible(false);
            numberPrimaryLabel->setVisible(false);
            numberPrimaryIcon->setVisible(false);
            numberPrimaryMessage->setVisible(false);
            numberPrimaryMessageLabel->setVisible(false);

            numberSecondary->setVisible(false);
            numberSecondaryLabel->setVisible(false);
            numberSecondaryIcon->setVisible(false);
            numberSecondaryMessage->setVisible(false);
            numberSecondaryMessageLabel->setVisible(false);

            email->setY(363 - 66);
            addressLabel->setY(429 - 33);
            address->setY(475 - 33);
        }

        if (contact->numbers.size() == 1) {
            numberPrimary->setText(contact->numbers[0].numberUser);

            numberSecondary->setVisible(false);
            numberSecondaryLabel->setVisible(false);
            numberSecondaryIcon->setVisible(false);
            numberSecondaryMessage->setVisible(false);
            numberSecondaryMessageLabel->setVisible(false);

            numberPrimaryLabel->setNavigationItem(NavigationDirection::DOWN, numberPrimaryMessageLabel);
            numberPrimaryMessageLabel->setNavigationItem(NavigationDirection::UP, numberPrimaryLabel);
            numberPrimaryMessageLabel->setNavigationItem(NavigationDirection::DOWN, noteText);
            noteText->setNavigationItem(NavigationDirection::UP, numberPrimaryMessageLabel);

            email->setY(363 - 33);
            addressLabel->setY(429 - 33);
            address->setY(475 - 33);
        }

        if (contact->numbers.size() == 2) {

            numberPrimary->setText(contact->numbers[0].numberUser);
            numberSecondary->setText(contact->numbers[1].numberUser);
            email->setY(363);
            addressLabel->setY(429);
            address->setY(475);
        }

        if (contact->mail.length() > 0) {
            email->setText(contact->mail);
        }
        else {
            email->setVisible(false);
        }

        if (contact->address.length() > 0) {
            address->setText(contact->address);
        }

        if (contact->note.length() > 0) {
            noteText->setText(contact->note);
        }
    }

    bool PhonebookContact::handleSwitchData(SwitchData *data)
    {
        if (data == nullptr) {
            LOG_ERROR("Received null pointer");
            return false;
        }

        PhonebookItemData *item = dynamic_cast<PhonebookItemData *>(data);
        contact                 = item->getContact();

        setContactData();

        return (true);
    }

    bool PhonebookContact::onInput(const InputEvent &inputEvent)
    {
        if ((inputEvent.state != InputEvent::State::keyReleasedShort) &&
            ((inputEvent.state != InputEvent::State::keyReleasedLong)) && inputEvent.keyCode == KeyCode::KEY_LF) {
            std::unique_ptr<gui::SwitchData> data = std::make_unique<PhonebookItemData>(contact);
            application->switchWindow("Options", gui::ShowMode::GUI_SHOW_INIT, std::move(data));
            return (true);
        }

        if (inputEvent.keyCode == KeyCode::KEY_RF) {
            contact = nullptr;
            rebuild();
        }

        return (AppWindow::onInput(inputEvent));
    }

} // namespace gui
