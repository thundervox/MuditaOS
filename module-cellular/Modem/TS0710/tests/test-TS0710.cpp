#include "catch.hpp"
#include "TS0710.h"
#include "DLC_channel.h"
TEST_CASE("test-TS0710") {
    TS0710 *_class = new TS0710();

    //get channel by name
    DLC_channel *channel = _class->GetChannel("Control");
    REQUIRE(channel->getName() == "Control");
    channel = _class->GetChannel("xxx");
    REQUIRE(channel->getName() == "none");

    delete _class;
}
