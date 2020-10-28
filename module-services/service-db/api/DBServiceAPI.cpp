// Copyright (c) 2017-2020, Mudita Sp. z.o.o. All rights reserved.
// For licensing, see https://github.com/mudita/MuditaOS/LICENSE.md

#include "DBServiceAPI.hpp"

#include <messages/DBThreadMessage.hpp> // for DBThreadMessage, DBThreadResponseMessage, DBThreadGetCountMessage, DBThreadMessageGet
#include <messages/DBSettingsMessage.hpp> // for DBSettingsMessage, DBSettingsResponseMessage
#include <messages/DBSMSMessage.hpp>      // for DBSMSMessage, DBSMSResponseMessage, DBSMSGetCount
#include <messages/DBSMSTemplateMessage.hpp> // for DBSMSTemplateMessage, DBSMSTemplateResponseMessage, DBSMSTemplateGetCount
#include <messages/DBContactMessage.hpp> // for DBContactMessage, DBContactResponseMessage, DBContactBlock, DBContactNumberMessage, DBContactSearchMessage, DBContactNumberResponseMessage
#include <messages/DBAlarmMessage.hpp>   // for DBAlarmMessage, DBAlarmResponseMessage
#include <messages/DBNotesMessage.hpp>   // for DBNotesMessage, DBNotesResponseMessage
#include <messages/DBCalllogMessage.hpp> // for DBCalllogMessage, DBCalllogResponseMessage, DBCalllogGetCount
#include <messages/DBCountryCodeMessage.hpp>                   // for DBCountryCodeMessage, DBCountryCodeResponseMessage
#include <messages/DBServiceMessage.hpp>                       // for DBServiceMessageBackup
#include <messages/QueryMessage.hpp>                           // for QueryResponse
#include <Common/Query.hpp>                                    // for Query, QueryResult
#include <queries/phonebook/QueryNumberGetByID.hpp>            // for NumberGetByID, NumberGetByIDResult
#include <queries/messages/threads/QueryThreadGetByNumber.hpp> // for ThreadGetByNumber, ThreadGetByNumberResult
#include <includes/DBServiceName.hpp>                          // for db
#include <Utils.hpp>                                           // for to_string
#include <ext/alloc_traits.h>                                  // for __alloc_traits<>::value_type
#include <cassert>                                             // for assert
#include <utility>                                             // for pair, move, tuple_element<>::type

#include "AlarmsRecord.hpp"  // for AlarmsRecord
#include "CalllogRecord.hpp" // for CalllogRecord, operator<<
#include "ContactRecord.hpp" // for ContactRecord, ContactRecord::Number
#include "MessageType.hpp" // for MessageType, MessageType::DBContactGetByID, MessageType::DBAlarmAdd, MessageType::DBAlarmGetCount, MessageType::DBAlarmGetLimitOffset, MessageType::DBAlarmGetNext, MessageType::DBAlarmRemove, MessageType::DBAlarmUpdate, MessageType::DBCalllogAdd, MessageType::DBCalllogGetLimitOffset, MessageType::DBCalllogRemove, MessageType::DBCalllogUpdate, MessageType::DBContactAdd, MessageType::DBContactBlock, MessageType::DBContactGetByName, MessageType::DBContactGetByNumber, MessageType::DBContactGetBySpeedDial, MessageType::DBContactGetCount, MessageType::DBContactGetLimitOffset, MessageType::DBContactRemove, MessageType::DBContactSearch, MessageType::DBContactUpdate, MessageType::DBCountryCode, MessageType::DBNotesAdd, MessageType::DBNotesGetCount, MessageType::DBNotesGetLimitOffset, MessageType::DBNotesRemove, MessageType::DBNotesUpdate, MessageType::DBSMSAdd, MessageType::DBSMSGetLastRecord, MessageType::DBSMSGetSMSLimitOffset, MessageType::DBSMSGetSMSLimitOffsetByThreadID, MessageType::DBSMSRemove, MessageType::DBSMSTemplateAdd, MessageType::DBSMSTemplateGetLimitOffset, MessageType::DBSMSTemplateRemove, MessageType::DBSMSTemplateUpdate, MessageType::DBSMSUpdate, MessageType::DBServiceBackup, MessageType::DBSettingsGet, MessageType::DBSettingsUpdate, MessageType::DBThreadGet, MessageType::DBThreadGetForContact, MessageType::DBThreadGetLimitOffset, MessageType::DBThreadRemove, MessageType::DBThreadUpdate
#include "PhoneNumber.hpp" // for PhoneNumber::View, PhoneNumber
#include "SMSRecord.hpp"   // for SMSRecord
#include "SMSTemplateRecord.hpp"        // for SMSTemplateRecord
#include "Service/Bus.hpp"              // for Bus
#include "Service/Common.hpp"           // for ReturnCodes, ReturnCodes::Success, c_str
#include "SettingsRecord.hpp"           // for SettingsRecord
#include "Tables/CountryCodesTable.hpp" // for CodesTableRow
#include "Tables/Record.hpp"            // for DB_ID_NONE
#include "ThreadRecord.hpp"             // for ThreadRecord
#include "log/log.hpp"                  // for LOG_DEBUG, LOG_ERROR, LOG_INFO

namespace sys
{
    class Service;
} // namespace sys
struct NotesRecord;

SettingsRecord DBServiceAPI::SettingsGet(sys::Service *serv)
{

    std::shared_ptr<DBSettingsMessage> msg = std::make_shared<DBSettingsMessage>(MessageType::DBSettingsGet);

    auto ret = sys::Bus::SendUnicast(msg, service::name::db, serv, DefaultTimeoutInMs);
    if (ret.first == sys::ReturnCodes::Success) {
        auto respMsg                                = ret.second;
        DBSettingsResponseMessage *settingsResponse = reinterpret_cast<DBSettingsResponseMessage *>(respMsg.get());

        if (settingsResponse->retCode == true) {
            return settingsResponse->record;
        }
        else {
            return SettingsRecord{};
        }
    }
    else {
        return SettingsRecord{};
    }
}

bool DBServiceAPI::SettingsUpdate(sys::Service *serv, const SettingsRecord &rec)
{
    std::shared_ptr<DBSettingsMessage> msg = std::make_shared<DBSettingsMessage>(MessageType::DBSettingsUpdate, rec);

    auto ret = sys::Bus::SendUnicast(msg, service::name::db, serv, DefaultTimeoutInMs);
    if (ret.first == sys::ReturnCodes::Success) {
        return true;
    }
    else {
        return false;
    }
}

uint32_t DBServiceAPI::SMSAdd(sys::Service *serv, const SMSRecord &rec)
{
    std::shared_ptr<DBSMSMessage> msg = std::make_shared<DBSMSMessage>(MessageType::DBSMSAdd, rec);

    auto ret                          = sys::Bus::SendUnicast(msg, service::name::db, serv, DefaultTimeoutInMs);
    DBSMSResponseMessage *smsResponse = reinterpret_cast<DBSMSResponseMessage *>(ret.second.get());
    if ((ret.first == sys::ReturnCodes::Success) && (smsResponse->retCode == true)) {
        return (*smsResponse->records)[0].ID;
    }
    else {
        return DB_ID_NONE;
    }
}

bool DBServiceAPI::SMSRemove(sys::Service *serv, const SMSRecord &rec)
{
    std::shared_ptr<DBSMSMessage> msg = std::make_shared<DBSMSMessage>(MessageType::DBSMSRemove);
    msg->record                       = rec;

    auto ret                          = sys::Bus::SendUnicast(msg, service::name::db, serv, DefaultTimeoutInMs);
    DBSMSResponseMessage *smsResponse = reinterpret_cast<DBSMSResponseMessage *>(ret.second.get());
    if ((ret.first == sys::ReturnCodes::Success) && (smsResponse->retCode == true)) {
        return true;
    }
    else {
        return false;
    }
}

bool DBServiceAPI::SMSUpdate(sys::Service *serv, const SMSRecord &rec)
{
    std::shared_ptr<DBSMSMessage> msg = std::make_shared<DBSMSMessage>(MessageType::DBSMSUpdate, rec);

    auto ret                          = sys::Bus::SendUnicast(msg, service::name::db, serv, DefaultTimeoutInMs);
    DBSMSResponseMessage *smsResponse = reinterpret_cast<DBSMSResponseMessage *>(ret.second.get());
    if ((ret.first == sys::ReturnCodes::Success) && (smsResponse->retCode == true)) {
        return true;
    }
    else {
        return false;
    }
}

SMSRecord DBServiceAPI::SMSGetLastRecord(sys::Service *serv)
{
    std::shared_ptr<DBSMSMessage> msg = std::make_shared<DBSMSMessage>(MessageType::DBSMSGetLastRecord);

    auto ret                          = sys::Bus::SendUnicast(msg, service::name::db, serv, DefaultTimeoutInMs);
    DBSMSResponseMessage *smsResponse = reinterpret_cast<DBSMSResponseMessage *>(ret.second.get());
    if ((ret.first == sys::ReturnCodes::Success) && (smsResponse->retCode == true)) {
        return (*smsResponse->records)[0];
    }
    else {
        return SMSRecord();
    }
}

std::unique_ptr<std::vector<SMSRecord>> DBServiceAPI::SMSGetLimitOffset(sys::Service *serv,
                                                                        uint32_t offset,
                                                                        uint32_t limit)
{
    std::shared_ptr<DBSMSMessage> msg = std::make_shared<DBSMSMessage>(MessageType::DBSMSGetSMSLimitOffset);
    msg->offset                       = offset;
    msg->limit                        = limit;

    auto ret                          = sys::Bus::SendUnicast(msg, service::name::db, serv, DefaultTimeoutInMs);
    DBSMSResponseMessage *smsResponse = reinterpret_cast<DBSMSResponseMessage *>(ret.second.get());
    if ((ret.first == sys::ReturnCodes::Success) && (smsResponse->retCode == true)) {
        return std::move(smsResponse->records);
    }
    else {
        return std::make_unique<std::vector<SMSRecord>>();
    }
}

std::unique_ptr<std::vector<SMSRecord>> DBServiceAPI::SMSGetLimitOffsetByThreadID(sys::Service *serv,
                                                                                  uint32_t offset,
                                                                                  uint32_t limit,
                                                                                  uint32_t id)
{
    std::shared_ptr<DBSMSMessage> msg = std::make_shared<DBSMSMessage>(MessageType::DBSMSGetSMSLimitOffsetByThreadID);
    msg->offset                       = offset;
    msg->limit                        = limit;
    msg->id                           = id;

    auto ret                          = sys::Bus::SendUnicast(msg, service::name::db, serv, DefaultTimeoutInMs);
    DBSMSResponseMessage *smsResponse = reinterpret_cast<DBSMSResponseMessage *>(ret.second.get());
    if ((ret.first == sys::ReturnCodes::Success) && (smsResponse->retCode == true)) {
        return std::move(smsResponse->records);
    }
    else {
        return std::make_unique<std::vector<SMSRecord>>();
    }
}

uint32_t DBServiceAPI::SMSGetCount(sys::Service *serv)
{
    std::shared_ptr<DBSMSMessage> msg = std::make_shared<DBSMSGetCount>();

    auto ret  = sys::Bus::SendUnicast(msg, service::name::db, serv, DefaultTimeoutInMs);
    auto *sms = reinterpret_cast<DBSMSResponseMessage *>(ret.second.get());
    if ((ret.first == sys::ReturnCodes::Success) && (sms->retCode == true)) {
        return sms->count;
    }
    else {
        return 0;
    }
}

std::unique_ptr<ThreadRecord> DBServiceAPI::ThreadGet(sys::Service *serv, uint32_t id)
{
    std::shared_ptr<DBThreadMessage> msg = std::make_shared<DBThreadMessage>(MessageType::DBThreadGet);
    msg->id                              = id;

    auto ret                                = sys::Bus::SendUnicast(msg, service::name::db, serv, DefaultTimeoutInMs);
    DBThreadResponseMessage *threadResponse = reinterpret_cast<DBThreadResponseMessage *>(ret.second.get());
    if ((ret.first == sys::ReturnCodes::Success) && (threadResponse->retCode == true)) {
        return std::make_unique<ThreadRecord>((*threadResponse->records)[0]);
    }
    else {
        return nullptr;
    }
}

std::unique_ptr<ThreadRecord> DBServiceAPI::ThreadGetByContact(sys::Service *serv, uint32_t contact)
{
    std::shared_ptr<DBThreadMessageGet> msg =
        std::make_shared<DBThreadMessageGet>(MessageType::DBThreadGetForContact, contact);
    auto ret                                = sys::Bus::SendUnicast(msg, service::name::db, serv, DefaultTimeoutInMs);
    DBThreadResponseMessage *threadResponse = reinterpret_cast<DBThreadResponseMessage *>(ret.second.get());
    if ((ret.first == sys::ReturnCodes::Success) && (threadResponse->retCode == true)) {
        return std::make_unique<ThreadRecord>((*threadResponse->records)[0]);
    }
    else {
        return nullptr;
    }
}

std::unique_ptr<ThreadRecord> DBServiceAPI::ThreadGetByNumber(sys::Service *serv,
                                                              const utils::PhoneNumber::View &phoneNumber,
                                                              std::uint32_t timeout)
{
    auto [code, msg] = DBServiceAPI::GetQueryWithReply(
        serv, db::Interface::Name::SMSThread, std::make_unique<db::query::ThreadGetByNumber>(phoneNumber), timeout);

    if (code == sys::ReturnCodes::Success && msg != nullptr) {
        auto queryResponse = dynamic_cast<db::QueryResponse *>(msg.get());
        assert(queryResponse != nullptr);

        auto threadResponse = queryResponse->getResult();
        auto threadResult   = dynamic_cast<db::query::ThreadGetByNumberResult *>(threadResponse.get());
        assert(threadResult != nullptr);

        return std::make_unique<ThreadRecord>(std::move(threadResult->getThread()));
    }

    return nullptr;
}

bool DBServiceAPI::ThreadRemove(sys::Service *serv, uint32_t id)
{
    std::shared_ptr<DBThreadMessage> msg = std::make_shared<DBThreadMessage>(MessageType::DBThreadRemove);
    msg->id                              = id;

    auto ret                                = sys::Bus::SendUnicast(msg, service::name::db, serv, DefaultTimeoutInMs);
    DBThreadResponseMessage *threadResponse = reinterpret_cast<DBThreadResponseMessage *>(ret.second.get());
    if ((ret.first == sys::ReturnCodes::Success) && (threadResponse->retCode == true)) {
        return true;
    }
    else {
        return false;
    }
}

bool DBServiceAPI::ThreadGetLimitOffset(sys::Service *serv, uint32_t offset, uint32_t limit)
{
    std::shared_ptr<DBThreadMessage> msg = std::make_shared<DBThreadMessage>(MessageType::DBThreadGetLimitOffset);
    msg->offset                          = offset;
    msg->limit                           = limit;

    sys::Bus::SendUnicast(msg, service::name::db, serv);

    return true;
}

uint32_t DBServiceAPI::ThreadGetCount(sys::Service *serv, EntryState state)
{
    auto msg = std::make_shared<DBThreadGetCountMessage>(state);

    auto ret                                = sys::Bus::SendUnicast(msg, service::name::db, serv, DefaultTimeoutInMs);
    DBThreadResponseMessage *threadResponse = reinterpret_cast<DBThreadResponseMessage *>(ret.second.get());
    if ((ret.first == sys::ReturnCodes::Success) && (threadResponse->retCode == true)) {
        return threadResponse->count;
    }
    else {
        return false;
    }
}

bool DBServiceAPI::ThreadUpdate(sys::Service *serv, const ThreadRecord &rec)
{
    std::shared_ptr<DBThreadMessage> msg = std::make_shared<DBThreadMessage>(MessageType::DBThreadUpdate, rec);

    sys::Bus::SendUnicast(msg, service::name::db, serv);
    return true;
}

bool DBServiceAPI::SMSTemplateAdd(sys::Service *serv, const SMSTemplateRecord &rec)
{
    auto msg = std::make_shared<DBSMSTemplateMessage>(MessageType::DBSMSTemplateAdd, rec);

    auto ret  = sys::Bus::SendUnicast(msg, service::name::db, serv, DefaultTimeoutInMs);
    auto resp = dynamic_cast<DBSMSTemplateResponseMessage *>(ret.second.get());
    if (ret.first == sys::ReturnCodes::Success && resp != nullptr) {
        return resp->retCode;
    }

    return false;
}

bool DBServiceAPI::SMSTemplateRemove(sys::Service *serv, uint32_t id)
{
    auto msg = std::make_shared<DBSMSTemplateMessage>(MessageType::DBSMSTemplateRemove);
    msg->id  = id;

    auto ret  = sys::Bus::SendUnicast(msg, service::name::db, serv, DefaultTimeoutInMs);
    auto resp = dynamic_cast<DBSMSTemplateResponseMessage *>(ret.second.get());
    if (ret.first == sys::ReturnCodes::Success && resp != nullptr) {
        return resp->retCode;
    }

    return false;
}

bool DBServiceAPI::SMSTemplateUpdate(sys::Service *serv, const SMSTemplateRecord &rec)
{
    auto msg = std::make_shared<DBSMSTemplateMessage>(MessageType::DBSMSTemplateUpdate, rec);

    auto ret  = sys::Bus::SendUnicast(msg, service::name::db, serv, DefaultTimeoutInMs);
    auto resp = dynamic_cast<DBSMSTemplateResponseMessage *>(ret.second.get());
    if (ret.first == sys::ReturnCodes::Success && resp != nullptr) {
        return resp->retCode;
    }

    return false;
}

uint32_t DBServiceAPI::SMSTemplateGetCount(sys::Service *serv)
{
    auto msg = std::make_shared<DBSMSTemplateGetCount>();

    auto ret  = sys::Bus::SendUnicast(msg, service::name::db, serv, DefaultTimeoutInMs);
    auto resp = dynamic_cast<DBSMSTemplateResponseMessage *>(ret.second.get());
    if (ret.first == sys::ReturnCodes::Success && resp != nullptr) {
        return resp->count;
    }

    return 0;
}

bool DBServiceAPI::SMSTemplateGetLimitOffset(sys::Service *serv, uint32_t offset, uint32_t limit)
{
    auto msg    = std::make_shared<DBSMSTemplateMessage>(MessageType::DBSMSTemplateGetLimitOffset);
    msg->offset = offset;
    msg->limit  = limit;

    return sys::Bus::SendUnicast(msg, service::name::db, serv);
}

std::unique_ptr<std::vector<ContactRecord>> DBServiceAPI::ContactGetByName(sys::Service *serv,
                                                                           UTF8 primaryName,
                                                                           UTF8 alternativeName)
{

    ContactRecord rec;
    rec.primaryName     = primaryName;
    rec.alternativeName = alternativeName;

    std::shared_ptr<DBContactMessage> msg = std::make_shared<DBContactMessage>(MessageType::DBContactGetByName, rec);

    auto ret                                  = sys::Bus::SendUnicast(msg, service::name::db, serv, DefaultTimeoutInMs);
    DBContactResponseMessage *contactResponse = reinterpret_cast<DBContactResponseMessage *>(ret.second.get());
    if ((ret.first == sys::ReturnCodes::Success) && (contactResponse->retCode == true)) {
        return std::move(contactResponse->records);
    }
    else {
        return std::make_unique<std::vector<ContactRecord>>();
    }
}

std::unique_ptr<std::vector<ContactRecord>> DBServiceAPI::ContactGetByID(sys::Service *serv, uint32_t contactID)
{
    ContactRecord rec;
    rec.ID = contactID;

    std::shared_ptr<DBContactMessage> msg = std::make_shared<DBContactMessage>(MessageType::DBContactGetByID, rec);
    return std::move(ContactGetByIDCommon(serv, std::move(msg)));
}

std::unique_ptr<std::vector<ContactRecord>> DBServiceAPI::ContactGetByIDWithTemporary(sys::Service *serv,
                                                                                      uint32_t contactID)
{
    ContactRecord rec;
    rec.ID                                = contactID;
    std::shared_ptr<DBContactMessage> msg = std::make_shared<DBContactMessage>(MessageType::DBContactGetByID, rec);
    msg->withTemporary                    = true;
    return std::move(ContactGetByIDCommon(serv, std::move(msg)));
}

std::unique_ptr<std::vector<ContactRecord>> DBServiceAPI::ContactGetByIDCommon(
    sys::Service *serv, std::shared_ptr<DBContactMessage> contactMsg)
{
    auto ret = sys::Bus::SendUnicast(contactMsg, service::name::db, serv, DefaultTimeoutInMs);
    DBContactResponseMessage *contactResponse = reinterpret_cast<DBContactResponseMessage *>(ret.second.get());
    if ((ret.first == sys::ReturnCodes::Success) && (contactResponse->retCode == true)) {
        return std::move(contactResponse->records);
    }
    else {
        return std::make_unique<std::vector<ContactRecord>>();
    }
}

std::unique_ptr<std::vector<ContactRecord>> DBServiceAPI::ContactGetBySpeeddial(sys::Service *serv, UTF8 speeddial)
{
    if (speeddial.length() == 0)
        return std::make_unique<std::vector<ContactRecord>>();

    ContactRecord rec;
    rec.speeddial = speeddial;

    std::shared_ptr<DBContactMessage> msg =
        std::make_shared<DBContactMessage>(MessageType::DBContactGetBySpeedDial, rec);

    auto ret                                  = sys::Bus::SendUnicast(msg, service::name::db, serv, DefaultTimeoutInMs);
    DBContactResponseMessage *contactResponse = reinterpret_cast<DBContactResponseMessage *>(ret.second.get());
    if ((ret.first == sys::ReturnCodes::Success) && (contactResponse->retCode == true)) {
        return std::move(contactResponse->records);
    }
    else {
        return std::make_unique<std::vector<ContactRecord>>();
    }
}

std::unique_ptr<std::vector<ContactRecord>> DBServiceAPI::ContactGetByPhoneNumber(sys::Service *serv, UTF8 phoneNumber)
{
    ContactRecord rec;
    auto number = ContactRecord::Number(utils::PhoneNumber(phoneNumber).getView(), ContactNumberType ::PAGER);
    rec.numbers.push_back(number);

    std::shared_ptr<DBContactMessage> msg = std::make_shared<DBContactMessage>(MessageType::DBContactGetByNumber, rec);

    auto ret              = sys::Bus::SendUnicast(msg, service::name::db, serv, DefaultTimeoutInMs);
    auto *contactResponse = dynamic_cast<DBContactResponseMessage *>(ret.second.get());

    if ((ret.first == sys::ReturnCodes::Success) && (contactResponse->retCode == true)) {
        return std::move(contactResponse->records);
    }
    else {
        return std::make_unique<std::vector<ContactRecord>>();
    }
}

std::unique_ptr<ContactRecord> DBServiceAPI::MatchContactByPhoneNumber(sys::Service *serv,
                                                                       const utils::PhoneNumber::View &numberView)
{
    auto msg = std::make_shared<DBContactNumberMessage>(numberView);

    auto ret              = sys::Bus::SendUnicast(msg, service::name::db, serv, DefaultTimeoutInMs);
    auto *contactResponse = dynamic_cast<DBContactNumberResponseMessage *>(ret.second.get());
    assert(contactResponse);
    return std::move(contactResponse->contact);
}

DBServiceAPI::ContactVerificationError DBServiceAPI::verifyContact(sys::Service *serv, const ContactRecord &rec)
{

    if (rec.primaryName.length() == 0 && rec.alternativeName.length() == 0 && rec.numbers.empty() &&
        rec.mail.length() == 0 && rec.address.length() == 0 && rec.note.length() == 0) {
        return emptyContactError;
    }

    auto retSpeedDial = ContactGetBySpeeddial(serv, rec.speeddial);
    if (!retSpeedDial->empty() && (*retSpeedDial)[0].ID != rec.ID) {
        return speedDialError;
    }

    if (rec.numbers.size() > 0 && rec.numbers[0].number.getEntered().size() > 0) {
        auto retPhone1 = MatchContactByPhoneNumber(serv, rec.numbers[0].number);
        if (retPhone1 && retPhone1->ID != rec.ID) {
            return primaryNumberError;
        }
    }

    if (rec.numbers.size() > 1 && rec.numbers[1].number.getEntered().size() > 0) {
        auto retPhone2 = MatchContactByPhoneNumber(serv, rec.numbers[1].number);
        if (retPhone2 && retPhone2->ID != rec.ID) {
            return secondaryNumberError;
        }
    }

    return noError;
}

std::string DBServiceAPI::getVerificationErrorString(const ContactVerificationError err)
{
    switch (err) {
    case noError:
        return "No error occurred";
    case emptyContactError:
        return "Contact record is empty";
    case speedDialError:
        return "Invalid or duplicate speed dial number";
    case primaryNumberError:
        return "Invalid or duplicate primary number";
    case secondaryNumberError:
        return "Invalid or duplicate secondary number";
    default:
        return "Unknown error";
    }
}

bool DBServiceAPI::ContactAdd(sys::Service *serv, const ContactRecord &rec)
{
    std::shared_ptr<DBContactMessage> msg = std::make_shared<DBContactMessage>(MessageType::DBContactAdd, rec);

    auto ret                                  = sys::Bus::SendUnicast(msg, service::name::db, serv, DefaultTimeoutInMs);
    DBContactResponseMessage *contactResponse = reinterpret_cast<DBContactResponseMessage *>(ret.second.get());
    if ((ret.first == sys::ReturnCodes::Success) && (contactResponse->retCode == true)) {
        return true;
    }
    else {
        return false;
    }
}

bool DBServiceAPI::ContactRemove(sys::Service *serv, uint32_t id)
{
    std::shared_ptr<DBContactMessage> msg = std::make_shared<DBContactMessage>(MessageType::DBContactRemove);
    msg->id                               = id;

    auto ret                                  = sys::Bus::SendUnicast(msg, service::name::db, serv, DefaultTimeoutInMs);
    DBContactResponseMessage *contactResponse = reinterpret_cast<DBContactResponseMessage *>(ret.second.get());
    if ((ret.first == sys::ReturnCodes::Success) && (contactResponse->retCode == true)) {
        return true;
    }
    else {
        return false;
    }
}

bool DBServiceAPI::ContactUpdate(sys::Service *serv, const ContactRecord &rec)
{
    std::shared_ptr<DBContactMessage> msg = std::make_shared<DBContactMessage>(MessageType::DBContactUpdate, rec);

    auto ret                                  = sys::Bus::SendUnicast(msg, service::name::db, serv, DefaultTimeoutInMs);
    DBContactResponseMessage *contactResponse = reinterpret_cast<DBContactResponseMessage *>(ret.second.get());
    if ((ret.first == sys::ReturnCodes::Success) && (contactResponse->retCode == true)) {
        return true;
    }
    else {
        return false;
    }
}

bool DBServiceAPI::ContactBlock(sys::Service *serv, uint32_t id, const bool shouldBeBlocked)
{
    std::shared_ptr<DBContactBlock> msg =
        std::make_shared<DBContactBlock>(MessageType::DBContactBlock, id, shouldBeBlocked);

    auto ret                                  = sys::Bus::SendUnicast(msg, service::name::db, serv, DefaultTimeoutInMs);
    DBContactResponseMessage *contactResponse = reinterpret_cast<DBContactResponseMessage *>(ret.second.get());
    if ((ret.first == sys::ReturnCodes::Success) && (contactResponse->retCode == true)) {
        return true;
    }
    else {
        return false;
    }
}

uint32_t DBServiceAPI::ContactGetCount(sys::Service *serv, bool favourites)
{
    std::shared_ptr<DBContactMessage> msg =
        std::make_shared<DBContactMessage>(MessageType::DBContactGetCount, ContactRecord{}, favourites);

    auto ret                                  = sys::Bus::SendUnicast(msg, service::name::db, serv, DefaultTimeoutInMs);
    DBContactResponseMessage *contactResponse = reinterpret_cast<DBContactResponseMessage *>(ret.second.get());
    if ((ret.first == sys::ReturnCodes::Success) && (contactResponse->retCode == true)) {
        return contactResponse->count;
    }
    else {
        return false;
    }
}

bool DBServiceAPI::ContactGetLimitOffset(sys::Service *serv, uint32_t offset, uint32_t limit)
{
    std::shared_ptr<DBContactMessage> msg =
        std::make_shared<DBContactMessage>(MessageType::DBContactGetLimitOffset, ContactRecord{});
    msg->offset = offset;
    msg->limit  = limit;

    sys::Bus::SendUnicast(msg, service::name::db, serv);
    return true;
}

std::unique_ptr<std::vector<ContactRecord>> DBServiceAPI::ContactSearch(sys::Service *serv,
                                                                        UTF8 primaryName,
                                                                        UTF8 alternativeName,
                                                                        UTF8 number)
{
    std::shared_ptr<DBContactSearchMessage> msg =
        std::make_shared<DBContactSearchMessage>(MessageType::DBContactSearch,
                                                 (primaryName.length() > 0) ? primaryName.c_str() : "",
                                                 (alternativeName.length() > 0) ? alternativeName.c_str() : "",
                                                 (number.length() > 0) ? number.c_str() : "");

    auto ret                                  = sys::Bus::SendUnicast(msg, service::name::db, serv, DefaultTimeoutInMs);
    DBContactResponseMessage *contactResponse = reinterpret_cast<DBContactResponseMessage *>(ret.second.get());
    if ((ret.first == sys::ReturnCodes::Success) && (contactResponse->retCode == true)) {
        return std::move(contactResponse->records);
    }
    else {
        return std::make_unique<std::vector<ContactRecord>>();
    }
}

bool DBServiceAPI::AlarmAdd(sys::Service *serv, const AlarmsRecord &rec)
{
    std::shared_ptr<DBAlarmMessage> msg = std::make_shared<DBAlarmMessage>(MessageType::DBAlarmAdd, rec);

    auto ret                              = sys::Bus::SendUnicast(msg, service::name::db, serv, DefaultTimeoutInMs);
    DBAlarmResponseMessage *alarmResponse = reinterpret_cast<DBAlarmResponseMessage *>(ret.second.get());
    if ((ret.first == sys::ReturnCodes::Success) && (alarmResponse->retCode == true)) {
        return true;
    }
    else {
        return false;
    }
}

bool DBServiceAPI::AlarmRemove(sys::Service *serv, uint32_t id)
{
    std::shared_ptr<DBAlarmMessage> msg = std::make_shared<DBAlarmMessage>(MessageType::DBAlarmRemove);
    msg->id                             = id;

    auto ret                              = sys::Bus::SendUnicast(msg, service::name::db, serv, DefaultTimeoutInMs);
    DBAlarmResponseMessage *alarmResponse = reinterpret_cast<DBAlarmResponseMessage *>(ret.second.get());
    if ((ret.first == sys::ReturnCodes::Success) && (alarmResponse->retCode == true)) {
        return true;
    }
    else {
        return false;
    }
}

bool DBServiceAPI::AlarmUpdate(sys::Service *serv, const AlarmsRecord &rec)
{
    std::shared_ptr<DBAlarmMessage> msg = std::make_shared<DBAlarmMessage>(MessageType::DBAlarmUpdate, rec);

    auto ret                              = sys::Bus::SendUnicast(msg, service::name::db, serv, DefaultTimeoutInMs);
    DBAlarmResponseMessage *alarmResponse = reinterpret_cast<DBAlarmResponseMessage *>(ret.second.get());
    if ((ret.first == sys::ReturnCodes::Success) && (alarmResponse->retCode == true)) {
        return true;
    }
    else {
        return false;
    }
}

uint32_t DBServiceAPI::AlarmGetCount(sys::Service *serv)
{
    std::shared_ptr<DBAlarmMessage> msg = std::make_shared<DBAlarmMessage>(MessageType::DBAlarmGetCount);

    auto ret                              = sys::Bus::SendUnicast(msg, service::name::db, serv, DefaultTimeoutInMs);
    DBAlarmResponseMessage *alarmResponse = reinterpret_cast<DBAlarmResponseMessage *>(ret.second.get());
    if ((ret.first == sys::ReturnCodes::Success) && (alarmResponse->retCode == true)) {
        return alarmResponse->count;
    }
    else {
        return false;
    }
}

std::unique_ptr<std::vector<AlarmsRecord>> DBServiceAPI::AlarmGetLimitOffset(sys::Service *serv,
                                                                             uint32_t offset,
                                                                             uint32_t limit)
{
    std::shared_ptr<DBAlarmMessage> msg = std::make_shared<DBAlarmMessage>(MessageType::DBAlarmGetLimitOffset);
    msg->offset                         = offset;
    msg->limit                          = limit;

    auto ret                              = sys::Bus::SendUnicast(msg, service::name::db, serv, DefaultTimeoutInMs);
    DBAlarmResponseMessage *alarmResponse = reinterpret_cast<DBAlarmResponseMessage *>(ret.second.get());
    if ((ret.first == sys::ReturnCodes::Success) && (alarmResponse->retCode == true)) {
        return std::move(alarmResponse->records);
    }
    else {
        return std::make_unique<std::vector<AlarmsRecord>>();
    }
}

AlarmsRecord DBServiceAPI::AlarmGetNext(sys::Service *serv, time_t time)
{

    std::shared_ptr<DBAlarmMessage> msg   = std::make_shared<DBAlarmMessage>(MessageType::DBAlarmGetNext);
    msg->time                             = time;
    auto ret                              = sys::Bus::SendUnicast(msg, service::name::db, serv, DefaultTimeoutInMs);
    DBAlarmResponseMessage *alarmResponse = reinterpret_cast<DBAlarmResponseMessage *>(ret.second.get());
    if ((ret.first == sys::ReturnCodes::Success) && (alarmResponse->retCode == true)) {
        return std::move((*alarmResponse->records)[0]);
    }
    else {
        return AlarmsRecord{};
    }
}

bool DBServiceAPI::NotesAdd(sys::Service *serv, const NotesRecord &rec)
{
    std::shared_ptr<DBNotesMessage> msg = std::make_shared<DBNotesMessage>(MessageType::DBNotesAdd, rec);

    auto ret                              = sys::Bus::SendUnicast(msg, service::name::db, serv, DefaultTimeoutInMs);
    DBNotesResponseMessage *notesResponse = reinterpret_cast<DBNotesResponseMessage *>(ret.second.get());
    if ((ret.first == sys::ReturnCodes::Success) && (notesResponse->retCode == true)) {
        return true;
    }
    else {
        return false;
    }
}

bool DBServiceAPI::NotesRemove(sys::Service *serv, uint32_t id)
{
    std::shared_ptr<DBNotesMessage> msg = std::make_shared<DBNotesMessage>(MessageType::DBNotesRemove);
    msg->id                             = id;

    auto ret                              = sys::Bus::SendUnicast(msg, service::name::db, serv, DefaultTimeoutInMs);
    DBNotesResponseMessage *notesResponse = reinterpret_cast<DBNotesResponseMessage *>(ret.second.get());
    if ((ret.first == sys::ReturnCodes::Success) && (notesResponse->retCode == true)) {
        return true;
    }
    else {
        return false;
    }
}

bool DBServiceAPI::NotesUpdate(sys::Service *serv, const NotesRecord &rec)
{
    std::shared_ptr<DBNotesMessage> msg = std::make_shared<DBNotesMessage>(MessageType::DBNotesUpdate, rec);

    auto ret                              = sys::Bus::SendUnicast(msg, service::name::db, serv, DefaultTimeoutInMs);
    DBNotesResponseMessage *notesResponse = reinterpret_cast<DBNotesResponseMessage *>(ret.second.get());
    if ((ret.first == sys::ReturnCodes::Success) && (notesResponse->retCode == true)) {
        return true;
    }
    else {
        return false;
    }
}

uint32_t DBServiceAPI::NotesGetCount(sys::Service *serv)
{
    std::shared_ptr<DBNotesMessage> msg = std::make_shared<DBNotesMessage>(MessageType::DBNotesGetCount);

    auto ret                              = sys::Bus::SendUnicast(msg, service::name::db, serv, DefaultTimeoutInMs);
    DBNotesResponseMessage *notesResponse = reinterpret_cast<DBNotesResponseMessage *>(ret.second.get());
    if ((ret.first == sys::ReturnCodes::Success) && (notesResponse->retCode == true)) {
        return notesResponse->count;
    }
    else {
        return false;
    }
}

bool DBServiceAPI::NotesGetLimitOffset(sys::Service *serv, uint32_t offset, uint32_t limit)
{
    std::shared_ptr<DBNotesMessage> msg = std::make_shared<DBNotesMessage>(MessageType::DBNotesGetLimitOffset);
    msg->offset                         = offset;
    msg->limit                          = limit;

    sys::Bus::SendUnicast(msg, service::name::db, serv);
    return true;
}

CalllogRecord DBServiceAPI::CalllogAdd(sys::Service *serv, const CalllogRecord &rec)
{
    std::shared_ptr<DBCalllogMessage> msg = std::make_shared<DBCalllogMessage>(MessageType::DBCalllogAdd, rec);

    LOG_DEBUG("CalllogAdd %s", utils::to_string(rec).c_str());

    auto ret                                  = sys::Bus::SendUnicast(msg, service::name::db, serv, DefaultTimeoutInMs);
    DBCalllogResponseMessage *calllogResponse = reinterpret_cast<DBCalllogResponseMessage *>(ret.second.get());
    if ((ret.first == sys::ReturnCodes::Success) && (calllogResponse->retCode == true)) {
        auto records = *calllogResponse->records;
        if (!records.empty()) {
            return records[0];
        }
    }

    return CalllogRecord();
}

bool DBServiceAPI::CalllogRemove(sys::Service *serv, uint32_t id)
{
    std::shared_ptr<DBCalllogMessage> msg = std::make_shared<DBCalllogMessage>(MessageType::DBCalllogRemove);
    msg->id                               = id;

    auto ret                                  = sys::Bus::SendUnicast(msg, service::name::db, serv, DefaultTimeoutInMs);
    DBCalllogResponseMessage *calllogResponse = reinterpret_cast<DBCalllogResponseMessage *>(ret.second.get());
    if ((ret.first == sys::ReturnCodes::Success) && (calllogResponse->retCode == true)) {
        return true;
    }
    else {
        return false;
    }
}

bool DBServiceAPI::CalllogUpdate(sys::Service *serv, const CalllogRecord &rec)
{
    std::shared_ptr<DBCalllogMessage> msg = std::make_shared<DBCalllogMessage>(MessageType::DBCalllogUpdate, rec);

    LOG_DEBUG("CalllogUpdate %s", utils::to_string(rec).c_str());

    auto ret                                  = sys::Bus::SendUnicast(msg, service::name::db, serv, DefaultTimeoutInMs);
    DBCalllogResponseMessage *calllogResponse = reinterpret_cast<DBCalllogResponseMessage *>(ret.second.get());
    if ((ret.first == sys::ReturnCodes::Success) && (calllogResponse->retCode == true)) {
        return true;
    }
    else {
        return false;
    }
}

uint32_t DBServiceAPI::CalllogGetCount(sys::Service *serv, EntryState state)
{
    std::shared_ptr<DBCalllogMessage> msg = std::make_shared<DBCalllogGetCount>(state);

    auto ret                                  = sys::Bus::SendUnicast(msg, service::name::db, serv, DefaultTimeoutInMs);
    DBCalllogResponseMessage *calllogResponse = reinterpret_cast<DBCalllogResponseMessage *>(ret.second.get());
    if ((ret.first == sys::ReturnCodes::Success) && (calllogResponse->retCode == true)) {
        return calllogResponse->count;
    }
    else {
        return 0;
    }
}

bool DBServiceAPI::CalllogGetLimitOffset(sys::Service *serv, uint32_t offset, uint32_t limit)
{
    std::shared_ptr<DBCalllogMessage> msg = std::make_shared<DBCalllogMessage>(MessageType::DBCalllogGetLimitOffset);
    msg->offset                           = offset;
    msg->limit                            = limit;

    sys::Bus::SendUnicast(msg, service::name::db, serv);
    return true;
}

uint32_t DBServiceAPI::GetCountryCodeByMCC(sys::Service *serv, uint32_t mcc)
{
    std::shared_ptr<DBCountryCodeMessage> msg =
        std::make_shared<DBCountryCodeMessage>(MessageType::DBCountryCode, mcc, 0);

    auto ret                               = sys::Bus::SendUnicast(msg, service::name::db, serv, DefaultTimeoutInMs);
    DBCountryCodeResponseMessage *response = reinterpret_cast<DBCountryCodeResponseMessage *>(ret.second.get());
    if (ret.first == sys::ReturnCodes::Success) {
        return (response->row.country_code);
    }
    else {
        return (0);
    }
}

bool DBServiceAPI::DBBackup(sys::Service *serv, std::string backupPath)
{
    LOG_INFO("DBBackup %s", backupPath.c_str());

    std::shared_ptr<DBServiceMessageBackup> msg =
        std::make_shared<DBServiceMessageBackup>(MessageType::DBServiceBackup, backupPath);

    auto ret = sys::Bus::SendUnicast(msg, service::name::db, serv, DefaultTimeoutInMs);
    if (ret.first == sys::ReturnCodes::Success) {
        return true;
    }
    else {
        LOG_ERROR("DBBackup error, return code: %s", c_str(ret.first));
        return false;
    }
}

std::unique_ptr<utils::PhoneNumber::View> DBServiceAPI::GetNumberById(sys::Service *serv,
                                                                      std::uint32_t numberId,
                                                                      std::uint32_t timeout)
{
    auto [code, msg] = DBServiceAPI::GetQueryWithReply(
        serv, db::Interface::Name::Contact, std::make_unique<db::query::NumberGetByID>(numberId), timeout);

    if (code == sys::ReturnCodes::Success && msg != nullptr) {
        auto queryResponse = dynamic_cast<db::QueryResponse *>(msg.get());
        assert(queryResponse != nullptr);

        auto numberResponse = queryResponse->getResult();
        auto numberResult   = dynamic_cast<db::query::NumberGetByIDResult *>(numberResponse.get());
        assert(numberResult != nullptr);

        return std::make_unique<utils::PhoneNumber::View>(std::move(numberResult->getNumber()));
    }

    return nullptr;
}
