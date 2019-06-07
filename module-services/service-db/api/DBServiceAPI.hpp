
/*
 * @file DBServiceAPI.hpp
 * @author Mateusz Piesta (mateusz.piesta@mudita.com)
 * @date 06.06.19
 * @brief
 * @copyright Copyright (C) 2019 mudita.com
 * @details
 */


#ifndef PUREPHONE_DBSERVICEAPI_HPP
#define PUREPHONE_DBSERVICEAPI_HPP

#include "Service/Bus.hpp"
#include "Interface/SettingsRecord.hpp"
#include "Interface/SMSRecord.hpp"

class DBServiceAPI {

public:
    static SettingsRecord SettingsGet(sys::Service* serv);
    static bool SettingsUpdate(sys::Service* serv,const SettingsRecord& rec);

    static bool SMSAdd(sys::Service* serv,const SMSRecord& rec);
    static bool SMSRemove(sys::Service* serv,uint32_t id);
    static std::unique_ptr<std::vector<SMSRecord>> SMSGetLimitOffset(uint32_t offset,uint32_t limit);

};


#endif //PUREPHONE_DBSERVICEAPI_HPP
