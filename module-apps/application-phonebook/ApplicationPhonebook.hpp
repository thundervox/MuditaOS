#pragma once

#include "Application.hpp"

#include <string>

namespace gui::window::name
{
    inline const std::string contact          = "Contact";
    inline const std::string contact_options  = "Options";
    inline const std::string no_results       = "NoResults";
    inline const std::string contact_blocked  = "ContactBlocked";
    inline const std::string namecard_options = "Namecard Options";
    inline const std::string new_contact      = "New";
    inline const std::string search           = "Search";
    inline const std::string search_results   = "SearchResults";
    inline const std::string dialog_yes_no    = "DialogYesNo";
    inline const std::string dialog_confirm   = "DialogConfirm";

} // namespace gui::window::name

namespace app
{
    const inline std::string name_phonebook      = "ApplicationPhonebook";
    constexpr std::uint16_t phonebook_stack_size = 8192;

    class ApplicationPhonebook : public app::Application
    {
      public:
        ApplicationPhonebook(std::string name = name_phonebook, std::string parent = "", bool startBackgound = false);
        ~ApplicationPhonebook() override = default;

        auto DataReceivedHandler(sys::DataMessage *msgl, sys::ResponseMessage *resp) -> sys::Message_t override;
        auto InitHandler() -> sys::ReturnCodes override;
        auto DeinitHandler() -> sys::ReturnCodes override;

        auto SwitchPowerModeHandler(const sys::ServicePowerMode mode) -> sys::ReturnCodes final
        {
            return sys::ReturnCodes::Success;
        }

        void createUserInterface() override;
        void destroyUserInterface() override;

        void onSearchRequest(const std::string &searchFilter);
    };

} // namespace app
