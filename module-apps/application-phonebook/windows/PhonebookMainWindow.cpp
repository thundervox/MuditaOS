#include "PhonebookMainWindow.hpp"
#include "application-phonebook/ApplicationPhonebook.hpp"
#include "application-phonebook/data/PhonebookItemData.hpp"
#include "application-phonebook/data/PhonebookStyle.hpp"

#include <service-appmgr/ApplicationManager.hpp>
#include <service-db/messages/DBContactMessage.hpp>

namespace gui
{
    PhonebookMainWindow::PhonebookMainWindow(app::Application *app)
        : AppWindow(app, gui::name::window::main_window), phonebookModel{new PhonebookModel(app)}
    {
        buildInterface();
    }

    void PhonebookMainWindow::rebuild()
    {
        destroyInterface();
        buildInterface();
    }

    void PhonebookMainWindow::buildInterface()
    {
        AppWindow::buildInterface();

        topBar->setActive(TopBar::Elements::TIME, true);
        setTitle(utils::localize.get("app_phonebook_title_main"));
        leftArrowImage  = new gui::Image(this,
                                        phonebookStyle::mainWindow::leftArrowImage::x,
                                        phonebookStyle::mainWindow::leftArrowImage::y,
                                        phonebookStyle::mainWindow::leftArrowImage::w,
                                        phonebookStyle::mainWindow::leftArrowImage::h,
                                        "arrow_left");
        rightArrowImage = new gui::Image(this,
                                         phonebookStyle::mainWindow::rightArrowImage::x,
                                         phonebookStyle::mainWindow::rightArrowImage::y,
                                         phonebookStyle::mainWindow::rightArrowImage::w,
                                         phonebookStyle::mainWindow::rightArrowImage::h,
                                         "arrow_right");
        newContactImage = new gui::Image(this,
                                         phonebookStyle::mainWindow::newContactImage::x,
                                         phonebookStyle::mainWindow::newContactImage::y,
                                         phonebookStyle::mainWindow::newContactImage::w,
                                         phonebookStyle::mainWindow::newContactImage::h,
                                         "cross");
        searchImage     = new gui::Image(this,
                                     phonebookStyle::mainWindow::searchImage::x,
                                     phonebookStyle::mainWindow::searchImage::y,
                                     phonebookStyle::mainWindow::searchImage::w,
                                     phonebookStyle::mainWindow::searchImage::h,
                                     "search");

        contactsList = new gui::PhonebookListView(this,
                                                  phonebookStyle::mainWindow::contactsList::x,
                                                  phonebookStyle::mainWindow::contactsList::y,
                                                  phonebookStyle::mainWindow::contactsList::w,
                                                  phonebookStyle::mainWindow::contactsList::h);
        contactsList->setPenFocusWidth(phonebookStyle::mainWindow::contactsList::penFocusWidth);
        contactsList->setPenWidth(phonebookStyle::mainWindow::contactsList::penWidth);
        contactsList->setProvider(phonebookModel);

        bottomBar->setActive(BottomBar::Side::LEFT, true);
        bottomBar->setActive(BottomBar::Side::CENTER, true);
        bottomBar->setActive(BottomBar::Side::RIGHT, true);
        bottomBar->setText(BottomBar::Side::LEFT, utils::localize.get("app_phonebook_call"));
        bottomBar->setText(BottomBar::Side::CENTER, utils::localize.get("app_phonebook_open"));
        bottomBar->setText(BottomBar::Side::RIGHT, utils::localize.get("app_phonebook_back"));
    }

    void PhonebookMainWindow::destroyInterface()
    {
        erase();
        delete phonebookModel;
    }

    PhonebookMainWindow::~PhonebookMainWindow()
    {
        destroyInterface();
    }

    void PhonebookMainWindow::onBeforeShow(ShowMode mode, SwitchData *data)
    {
        LOG_INFO("onBeforeShow");
        setFocusItem(contactsList);

        phonebookModel->clear();
        phonebookModel->requestRecordsCount();

        contactsList->clear();
        contactsList->setElementsCount(phonebookModel->getItemCount());

        auto contactRequest = dynamic_cast<PhonebookSearchReuqest *>(data);
        if (contactRequest) {
            enableNewContact                       = false;
            phonebookModel->messagesSelectCallback = [=](gui::PhonebookItem *item) {
                std::unique_ptr<PhonebookSearchReuqest> data = std::make_unique<PhonebookSearchReuqest>();
                data->result                                 = item->contact;
                data->setDescription("PhonebookSearchRequest");
                return sapm::ApplicationManager::messageSwitchPreviousApplication(
                    application, std::make_unique<sapm::APMSwitchPrevApp>(application->GetName(), std::move(data)));
            };

            leftArrowImage->setVisible(false);
            newContactImage->setVisible(false);

            bottomBar->setActive(BottomBar::Side::LEFT, false);
            bottomBar->setActive(BottomBar::Side::CENTER, true);
            bottomBar->setActive(BottomBar::Side::RIGHT, true);
            bottomBar->setText(BottomBar::Side::LEFT, utils::localize.get(""));
            bottomBar->setText(BottomBar::Side::CENTER, utils::localize.get("common_select"));
            bottomBar->setText(BottomBar::Side::RIGHT, utils::localize.get("app_phonebook_back"));
        }
    }

    bool PhonebookMainWindow::onInput(const InputEvent &inputEvent)
    {
        // process only if key is released
        if (inputEvent.state == InputEvent::State::keyReleasedShort) {
            switch (inputEvent.keyCode) {

            case KeyCode::KEY_LEFT: {
                if (enableNewContact) {
                    std::unique_ptr<gui::SwitchData> data = std::make_unique<PhonebookItemData>();
                    application->switchWindow(
                        gui::window::name::new_contact, gui::ShowMode::GUI_SHOW_INIT, std::move(data));
                }
            }
                return true;

            case KeyCode::KEY_RIGHT:
                application->switchWindow("Search");
                return true;

            default:
                break;
            }
        }

        // check if any of the lower inheritance onInput methods catch the event
        return AppWindow::onInput(inputEvent);
    }

    bool PhonebookMainWindow::onDatabaseMessage(sys::Message *msgl)
    {
        DBContactResponseMessage *msg = reinterpret_cast<DBContactResponseMessage *>(msgl);
        if (phonebookModel->updateRecords(std::move(msg->records), msg->offset, msg->limit, msg->count))
            return true;

        return false;
    }

} /* namespace gui */
