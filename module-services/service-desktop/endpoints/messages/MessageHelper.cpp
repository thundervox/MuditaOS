// Copyright (c) 2017-2020, Mudita Sp. z.o.o. All rights reserved.
// For licensing, see https://github.com/mudita/MuditaOS/LICENSE.md

#include "MessageHelper.hpp"

#include <memory>  // for make_unique, unique_ptr
#include <string>  // for string
#include <utility> // for move
#include <vector>  // for vector

#include "Common/Query.hpp" // for EndpointListener, Query, QueryListener, QueryResult
#include "ParserUtils.hpp" // for Code, id, count, Code::InternalServerError, Code::OK, contactID, msgTemplate, threadID, messageBody, templateText, date, isUnread, offset, type, dateSent, msgCount, parserFSM, phoneNumber, snippet, unreadMsgCount
#include "PhoneNumber.hpp" // for PhoneNumber::View
#include "SMSRecord.hpp"   // for SMSRecord
#include "SMSTemplateRecord.hpp"                                   // for SMSTemplateRecord
#include "Service/Common.hpp"                                      // for ReturnCodes, ReturnCodes::Success
#include "ThreadRecord.hpp"                                        // for ThreadRecord
#include "api/DBServiceAPI.hpp"                                    // for DBServiceAPI
#include "queries/messages/sms/QuerySMSGet.hpp"                    // for SMSGet, SMSGetResult
#include "queries/messages/sms/QuerySMSGetByContactID.hpp"         // for SMSGetByContactID, SMSGetByContactIDResult
#include "queries/messages/sms/QuerySMSGetByThreadID.hpp"          // for SMSGetByThreadID, SMSGetByThreadIDResult
#include "queries/messages/sms/QuerySMSGetByID.hpp"                // for SMSGetByID, SMSGetByIDResult
#include "queries/messages/sms/QuerySMSGetByText.hpp"              // for SMSGetByText, SMSGetByTextResult
#include "queries/messages/sms/QuerySMSGetCount.hpp"               // for SMSGetCount, SMSGetCountResult
#include "queries/messages/sms/QuerySMSRemove.hpp"                 // for SMSRemove, SMSRemoveResult
#include "queries/messages/templates/QuerySMSTemplateGet.hpp"      // for SMSTemplateGet, SMSTemplateGetResult
#include "queries/messages/templates/QuerySMSTemplateRemove.hpp"   // for SMSTemplateRemove, SMSTemplateRemoveResult
#include "queries/messages/templates/QuerySMSTemplateUpdate.hpp"   // for SMSTemplateUpdate, SMSTemplateUpdateResult
#include "queries/messages/templates/QuerySMSTemplateAdd.hpp"      // for SMSTemplateAdd, SMSTemplateAddResult
#include "queries/messages/templates/QuerySMSTemplateGetByID.hpp"  // for SMSTemplateGetByID, SMSTemplateGetByIDResult
#include "queries/messages/templates/QuerySMSTemplateGetCount.hpp" // for SMSTemplateGetCount, SMSTemplateGetCountResult
#include "queries/messages/threads/QueryThreadMarkAsRead.hpp" // for MarkAsRead, MarkAsRead::Read, MarkAsReadResult, MarkAsRead::Read::False, MarkAsRead::Read::True, query
#include "utf8/UTF8.hpp"   // for UTF8
#include "json/json11.hpp" // for Json, Json::array, Json::object
#include "BaseInterface.hpp" // for Interface, Interface::Name, Interface::Name::SMS, Interface::Name::SMSTemplate, Interface::Name::SMSThread
#include "Context.hpp"        // for Context
#include "MessageHandler.hpp" // for MessageHandler

using namespace parserFSM;

auto MessageHelper::to_json(SMSRecord record) -> json11::Json
{

    auto recordEntry = json11::Json::object{{json::messages::contactID, static_cast<int>(record.contactID)},
                                            {json::messages::date, static_cast<int>(record.date)},
                                            {json::messages::dateSent, static_cast<int>(record.dateSent)},
                                            {json::messages::id, static_cast<int>(record.ID)},
                                            {json::messages::messageBody, record.body.c_str()},
                                            {json::messages::phoneNumber, record.number.getEntered()},
                                            {json::messages::type, static_cast<int>(record.type)},
                                            {json::messages::threadID, static_cast<int>(record.threadID)}};
    return recordEntry;
}

auto MessageHelper::to_json(SMSTemplateRecord record) -> json11::Json
{

    auto recordEntry = json11::Json::object{{json::messages::id, static_cast<int>(record.ID)},
                                            {json::messages::templateText, record.text.c_str()}};
    return recordEntry;
}

auto MessageHelper::to_json(ThreadRecord record) -> json11::Json
{

    auto recordEntry =
        json11::Json::object{{json::messages::contactID, static_cast<int>(record.contactID)},
                             {json::messages::date, static_cast<int>(record.date)},
                             {json::messages::thread::msgCount, static_cast<int>(record.msgCount)},
                             {json::messages::id, static_cast<int>(record.ID)},
                             {json::messages::thread::snippet, record.snippet.c_str()},
                             {json::messages::isUnread, record.isUnread()},
                             {json::messages::type, static_cast<int>(record.type)},
                             {json::messages::thread::unreadMsgCount, static_cast<int>(record.unreadMsgCount)}};
    return recordEntry;
}

auto MessageHelper::from_json(json11::Json msgJson) -> SMSTemplateRecord
{
    SMSTemplateRecord record;

    record.text = UTF8(msgJson[json::messages::templateText].string_value());

    return record;
}

auto MessageHelper::requestDataFromDB(Context &context) -> sys::ReturnCodes
{

    if (context.getBody()[json::messages::msgTemplate].bool_value() == true) {
        return requestTemplate(context);
    }
    else {
        return requestSMS(context);
    }
}

auto MessageHelper::requestSMS(Context &context) -> sys::ReturnCodes
{
    if (context.getBody()[json::messages::count].bool_value() == true) // get messages count
    {
        auto query = std::make_unique<db::query::SMSGetCount>();

        auto listener = std::make_unique<db::EndpointListener>(
            [=](db::QueryResult *result, Context context) {
                if (auto SMSResult = dynamic_cast<db::query::SMSGetCountResult *>(result)) {
                    auto id = SMSResult->getResults();

                    context.setResponseBody(json11::Json::object{{json::messages::count, static_cast<int>(id)}});
                    MessageHandler::putToSendQueue(context.createSimpleResponse());
                    return true;
                }
                else {
                    return false;
                }
            },
            context);

        query->setQueryListener(std::move(listener));
        DBServiceAPI::GetQuery(ownerServicePtr, db::Interface::Name::SMS, std::move(query));
    }
    else if (context.getBody()[json::messages::id].int_value() != 0) { // messages search

        auto query = std::make_unique<db::query::SMSGetByID>(context.getBody()[json::messages::id].int_value());

        auto listener = std::make_unique<db::EndpointListener>(
            [=](db::QueryResult *result, Context context) {
                if (auto SMSResult = dynamic_cast<db::query::SMSGetByIDResult *>(result)) {

                    context.setResponseBody(MessageHelper::to_json(SMSResult->getResults()));
                    MessageHandler::putToSendQueue(context.createSimpleResponse());
                    return true;
                }
                else {
                    return false;
                }
            },
            context);

        query->setQueryListener(std::move(listener));
        DBServiceAPI::GetQuery(ownerServicePtr, db::Interface::Name::SMS, std::move(query));
    }
    else if (context.getBody()[json::messages::contactID].int_value() != 0) {

        auto query =
            std::make_unique<db::query::SMSGetByContactID>(context.getBody()[json::messages::contactID].int_value());

        auto listener = std::make_unique<db::EndpointListener>(
            [=](db::QueryResult *result, Context context) {
                if (auto SMSResult = dynamic_cast<db::query::SMSGetByContactIDResult *>(result)) {

                    json11::Json::array SMSarray;
                    for (auto record : SMSResult->getResults()) {
                        SMSarray.emplace_back(MessageHelper::to_json(record));
                    }

                    context.setResponseBody(SMSarray);
                    MessageHandler::putToSendQueue(context.createSimpleResponse());
                    return true;
                }
                else {
                    return false;
                }
            },
            context);

        query->setQueryListener(std::move(listener));
        DBServiceAPI::GetQuery(ownerServicePtr, db::Interface::Name::SMS, std::move(query));
    }
    else if (context.getBody()[json::messages::threadID].int_value() != 0) {

        auto query =
            std::make_unique<db::query::SMSGetByThreadID>(context.getBody()[json::messages::threadID].int_value());

        auto listener = std::make_unique<db::EndpointListener>(
            [=](db::QueryResult *result, Context context) {
                if (auto SMSResult = dynamic_cast<db::query::SMSGetByThreadIDResult *>(result)) {

                    json11::Json::array SMSarray;
                    for (auto record : SMSResult->getResults()) {
                        SMSarray.emplace_back(MessageHelper::to_json(record));
                    }

                    context.setResponseBody(SMSarray);
                    MessageHandler::putToSendQueue(context.createSimpleResponse());
                    return true;
                }
                else {
                    return false;
                }
            },
            context);

        query->setQueryListener(std::move(listener));
        DBServiceAPI::GetQuery(ownerServicePtr, db::Interface::Name::SMS, std::move(query));
    }
    else if (context.getBody()[json::messages::messageBody].string_value().empty() == false) {
        auto query =
            std::make_unique<db::query::SMSGetByText>(context.getBody()[json::messages::messageBody].string_value());

        auto listener = std::make_unique<db::EndpointListener>(
            [=](db::QueryResult *result, Context context) {
                if (auto SMSResult = dynamic_cast<db::query::SMSGetByTextResult *>(result)) {

                    json11::Json::array SMSarray;
                    for (auto record : SMSResult->getResults()) {
                        SMSarray.emplace_back(MessageHelper::to_json(record));
                    }

                    context.setResponseBody(SMSarray);
                    MessageHandler::putToSendQueue(context.createSimpleResponse());
                    return true;
                }
                else {
                    return false;
                }
            },
            context);

        query->setQueryListener(std::move(listener));
        DBServiceAPI::GetQuery(ownerServicePtr, db::Interface::Name::SMS, std::move(query));
    }
    else // get messages
    {
        auto query = std::make_unique<db::query::SMSGet>(context.getBody()[json::messages::count].int_value(),
                                                         context.getBody()[json::messages::offset].int_value());

        auto listener = std::make_unique<db::EndpointListener>(
            [=](db::QueryResult *result, Context context) {
                if (auto SMSResult = dynamic_cast<db::query::SMSGetResult *>(result)) {

                    json11::Json::array SMSarray;
                    for (auto record : SMSResult->getRecords()) {
                        SMSarray.emplace_back(MessageHelper::to_json(record));
                    }

                    context.setResponseBody(SMSarray);
                    MessageHandler::putToSendQueue(context.createSimpleResponse());
                    return true;
                }
                else {
                    return false;
                }
            },
            context);

        query->setQueryListener(std::move(listener));
        DBServiceAPI::GetQuery(ownerServicePtr, db::Interface::Name::SMS, std::move(query));
    }

    return sys::ReturnCodes::Success;
}

auto MessageHelper::requestTemplate(Context &context) -> sys::ReturnCodes
{
    if (context.getBody()[json::messages::count].bool_value() == true) // get templates count
    {
        auto query = std::make_unique<db::query::SMSTemplateGetCount>();

        auto listener = std::make_unique<db::EndpointListener>(
            [=](db::QueryResult *result, Context context) {
                if (auto SMSResult = dynamic_cast<db::query::SMSTemplateGetCountResult *>(result)) {
                    auto id = SMSResult->getResults();

                    context.setResponseBody(json11::Json::object{{json::messages::count, static_cast<int>(id)}});
                    MessageHandler::putToSendQueue(context.createSimpleResponse());
                    return true;
                }
                else {
                    return false;
                }
            },
            context);

        query->setQueryListener(std::move(listener));
        DBServiceAPI::GetQuery(ownerServicePtr, db::Interface::Name::SMSTemplate, std::move(query));
    }
    else if (context.getBody()[json::messages::id].int_value() != 0) { // templates search
        auto query = std::make_unique<db::query::SMSTemplateGetByID>(context.getBody()[json::messages::id].int_value());

        auto listener = std::make_unique<db::EndpointListener>(
            [=](db::QueryResult *result, Context context) {
                if (auto SMSTemplateResult = dynamic_cast<db::query::SMSTemplateGetByIDResult *>(result)) {

                    context.setResponseBody(MessageHelper::to_json(SMSTemplateResult->getResults()));
                    MessageHandler::putToSendQueue(context.createSimpleResponse());
                    return true;
                }
                else {
                    return false;
                }
            },
            context);

        query->setQueryListener(std::move(listener));
        DBServiceAPI::GetQuery(ownerServicePtr, db::Interface::Name::SMSTemplate, std::move(query));
    }
    else // get messages templates
    {
        auto query = std::make_unique<db::query::SMSTemplateGet>(context.getBody()[json::messages::offset].int_value(),
                                                                 context.getBody()[json::messages::count].int_value());
        auto listener = std::make_unique<db::EndpointListener>(
            [=](db::QueryResult *result, Context context) {
                if (auto SMSTemplateResult = dynamic_cast<db::query::SMSTemplateGetResult *>(result)) {

                    json11::Json::array SMSTemplateArray;
                    for (auto record : SMSTemplateResult->getResults()) {
                        SMSTemplateArray.emplace_back(to_json(record));
                    }

                    context.setResponseBody(SMSTemplateArray);
                    MessageHandler::putToSendQueue(context.createSimpleResponse());
                    return true;
                }
                else {
                    return false;
                }
            },
            context);

        query->setQueryListener(std::move(listener));
        DBServiceAPI::GetQuery(ownerServicePtr, db::Interface::Name::SMSTemplate, std::move(query));
    }

    return sys::ReturnCodes::Success;
}

auto MessageHelper::createDBEntry(Context &context) -> sys::ReturnCodes
{

    if (context.getBody()[json::messages::msgTemplate].bool_value() == true) {
        return createTemplate(context);
    }
    else {
        return createSMS(context);
    }
}

auto MessageHelper::createTemplate(Context &context) -> sys::ReturnCodes
{
    SMSTemplateRecord record = from_json(context.getBody());

    auto query    = std::make_unique<db::query::SMSTemplateAdd>(record);
    auto listener = std::make_unique<db::EndpointListener>(
        [=](db::QueryResult *result, Context context) {
            if (auto SMSTemplateResult = dynamic_cast<db::query::SMSTemplateAddResult *>(result)) {

                context.setResponseStatus(SMSTemplateResult->getResult() ? http::Code::OK
                                                                         : http::Code::InternalServerError);
                MessageHandler::putToSendQueue(context.createSimpleResponse());
                return true;
            }
            else {
                return false;
            }
        },
        context);

    query->setQueryListener(std::move(listener));
    DBServiceAPI::GetQuery(ownerServicePtr, db::Interface::Name::SMSTemplate, std::move(query));

    return sys::ReturnCodes::Success;
}

auto MessageHelper::createSMS(Context &context) -> sys::ReturnCodes
{
    return sys::ReturnCodes::Success;
}

auto MessageHelper::deleteDBEntry(Context &context) -> sys::ReturnCodes
{

    if (context.getBody()[json::messages::msgTemplate].bool_value() == true) {
        return deleteTemplate(context);
    }
    else {
        return deleteSMS(context);
    }
}

auto MessageHelper::deleteSMS(Context &context) -> sys::ReturnCodes
{
    auto id       = context.getBody()[json::messages::id].int_value();
    auto query    = std::make_unique<db::query::SMSRemove>(id);
    auto listener = std::make_unique<db::EndpointListener>(
        [=](db::QueryResult *result, Context context) {
            if (auto SMSTemplateResult = dynamic_cast<db::query::SMSRemoveResult *>(result)) {

                context.setResponseStatus(SMSTemplateResult->getResults() ? http::Code::OK
                                                                          : http::Code::InternalServerError);
                MessageHandler::putToSendQueue(context.createSimpleResponse());
                return true;
            }
            else {
                return false;
            }
        },
        context);

    query->setQueryListener(std::move(listener));
    DBServiceAPI::GetQuery(ownerServicePtr, db::Interface::Name::SMS, std::move(query));

    return sys::ReturnCodes::Success;
}

auto MessageHelper::deleteTemplate(Context &context) -> sys::ReturnCodes
{
    auto id       = context.getBody()[json::messages::id].int_value();
    auto query    = std::make_unique<db::query::SMSTemplateRemove>(id);
    auto listener = std::make_unique<db::EndpointListener>(
        [=](db::QueryResult *result, Context context) {
            if (auto SMSTemplateResult = dynamic_cast<db::query::SMSTemplateRemoveResult *>(result)) {

                context.setResponseStatus(SMSTemplateResult->getResults() ? http::Code::OK
                                                                          : http::Code::InternalServerError);
                MessageHandler::putToSendQueue(context.createSimpleResponse());
                return true;
            }
            else {
                return false;
            }
        },
        context);

    query->setQueryListener(std::move(listener));
    DBServiceAPI::GetQuery(ownerServicePtr, db::Interface::Name::SMSTemplate, std::move(query));
    return sys::ReturnCodes::Success;
}

auto MessageHelper::updateDBEntry(Context &context) -> sys::ReturnCodes
{

    if (context.getBody()[json::messages::msgTemplate].bool_value() == true) {
        return updateTemplate(context);
    }
    else {
        return updateSMS(context);
    }
}

auto MessageHelper::updateSMS(Context &context) -> sys::ReturnCodes
{
    using namespace db::query;

    auto query = std::make_unique<db::query::MarkAsRead>(
        context.getBody()[json::messages::threadID].int_value(),
        (context.getBody()[json::messages::isUnread].bool_value() ? MarkAsRead::Read::False : MarkAsRead::Read::True));

    auto listener = std::make_unique<db::EndpointListener>(
        [=](db::QueryResult *result, Context context) {
            if (auto SMSResult = dynamic_cast<db::query::MarkAsReadResult *>(result)) {

                context.setResponseStatus(SMSResult->getResult() ? http::Code::OK : http::Code::InternalServerError);
                MessageHandler::putToSendQueue(context.createSimpleResponse());

                return true;
            }
            else {
                return false;
            }
        },
        context);

    query->setQueryListener(std::move(listener));

    DBServiceAPI::GetQuery(ownerServicePtr, db::Interface::Name::SMSThread, std::move(query));
    return sys::ReturnCodes::Success;
}

auto MessageHelper::updateTemplate(Context &context) -> sys::ReturnCodes
{
    SMSTemplateRecord record;
    record.ID   = context.getBody()[json::messages::id].int_value();
    record.text = context.getBody()[json::messages::templateText].string_value();

    auto query    = std::make_unique<db::query::SMSTemplateUpdate>(record);
    auto listener = std::make_unique<db::EndpointListener>(
        [=](db::QueryResult *result, Context context) {
            if (auto SMSTemplateResult = dynamic_cast<db::query::SMSTemplateUpdateResult *>(result)) {

                context.setResponseStatus(SMSTemplateResult->getResult() ? http::Code::OK
                                                                         : http::Code::InternalServerError);
                MessageHandler::putToSendQueue(context.createSimpleResponse());
                return true;
            }
            else {
                return false;
            }
        },
        context);

    query->setQueryListener(std::move(listener));
    DBServiceAPI::GetQuery(ownerServicePtr, db::Interface::Name::SMSTemplate, std::move(query));

    return sys::ReturnCodes::Success;
}
