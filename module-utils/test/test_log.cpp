// Copyright (c) 2017-2020, Mudita Sp. z.o.o. All rights reserved.
// For licensing, see https://github.com/mudita/MuditaOS/LICENSE.md

/*
 * unittest_log.cpp
 *
 *  Created on: 29 kwi 2019
 *      Author: robert
 */

#include "log/log.hpp"

#include <iostream>
#include <string>

#include "test_log2.h"

using namespace std;

int main()
{

    cout << "RUNNING UNIT TEST FOR LOG" << endl;

    LOG_TRACE("This should be blue");
    LOG_DEBUG("This should be navy blue");
    LOG_INFO("This should be green");
    LOG_WARN("This should be yellow");
    LOG_ERROR("This should be red");
    LOG_FATAL("This should be purple");

    log_test_function();

    return 0;
}
