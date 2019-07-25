/*
 *  @file Playback_tests.cpp
 *  @author Mateusz Piesta (mateusz.piesta@mudita.com)
 *  @date 23.07.19
 *  @brief  
 *  @copyright Copyright (C) 2019 mudita.com
 *  @details
 */

#include <iostream>
#include <memory>
#include <cstring>
#include <unistd.h>


#include "catch.hpp"

#include "vfs.hpp"

#include "Audio/Operation/Operation.hpp"
#include "Audio/Profiles/ProfilePlaybackLoudspeaker.hpp"
#include "Audio/Profiles/ProfilePlaybackHeadphones.hpp"


TEST_CASE( "Playback tests" ) {

    auto cwd = vfs.getcurrdir();
    cwd = cwd.substr(0,cwd.find_last_of("/\\"));
    cwd = cwd.append("/module-audio/tests/samples");

    SECTION("Sample1.wav 16bit 44100Hz stereo")
    {
        SECTION("Full playback"){
            auto playbackRet = Operation::Create(Operation::Type ::Playback,(cwd + "/sample1.wav").c_str());
            REQUIRE(playbackRet);
            playbackRet.value()->Start([](uint32_t)->int32_t{
                std::cout<<"End of file reached!\n";
                return 0;
            });
            sleep(6);
            REQUIRE(playbackRet.value()->GetState() == Operation::State::Idle);
        }

        SECTION("Switch profile during playback"){
            auto playbackRet =  Operation::Create(Operation::Type ::Playback,(cwd + "/sample1.wav").c_str());
            REQUIRE(playbackRet);
            playbackRet.value()->Start([](uint32_t)->int32_t{
                std::cout<<"End of file reached!\n";
                return 0;
            });
            sleep(1);
            playbackRet.value()->SwitchProfile(Profile::Type::PlaybackHeadphones);
            REQUIRE(playbackRet.value()->GetProfile()->GetName() == "Playback Headphones");
            sleep(5);
            REQUIRE(playbackRet.value()->GetState() == Operation::State::Idle);
        }

        SECTION("Destroy playback object before end of file"){
            {
                auto playbackRet =  Operation::Create(Operation::Type ::Playback,(cwd + "/sample1.wav").c_str());
                REQUIRE(playbackRet);
                    playbackRet.value()->Start([](uint32_t)->int32_t{
                    std::cout<<"End of file reached!\n";
                    return 0;
                });
                sleep(1);
            }
        }

        SECTION("Pause/Resume/Stop sequence"){
            auto playbackRet =  Operation::Create(Operation::Type ::Playback,(cwd + "/sample1.wav").c_str());
            REQUIRE(playbackRet);
            playbackRet.value()->Start([](uint32_t)->int32_t{
                std::cout<<"End of file reached!\n";
                return 0;
            });
            sleep(1);
            playbackRet.value()->Pause();
            REQUIRE(playbackRet.value()->GetState() == Operation::State::Paused);
            sleep(1);
            playbackRet.value()->Resume();
            REQUIRE(playbackRet.value()->GetState() == Operation::State::Active);
            sleep(1);
            playbackRet.value()->Stop();
            REQUIRE(playbackRet.value()->GetState() == Operation::State::Idle);
        }

        SECTION("Pause/Resume/Stop sequence with profile switching"){
            auto playbackRet =  Operation::Create(Operation::Type ::Playback,(cwd + "/sample1.wav").c_str());
            REQUIRE(playbackRet);
            playbackRet.value()->Start([](uint32_t)->int32_t{
                std::cout<<"End of file reached!\n";
                return 0;
            });
            sleep(1);
            playbackRet.value()->Pause();
            playbackRet.value()->SwitchProfile(Profile::Type::PlaybackHeadphones);
            REQUIRE(playbackRet.value()->GetState() == Operation::State::Paused);
            REQUIRE(playbackRet.value()->GetProfile()->GetName() == "Playback Headphones");
            sleep(1);
            playbackRet.value()->Resume();
            REQUIRE(playbackRet.value()->GetState() == Operation::State::Active);
            sleep(1);
            playbackRet.value()->Stop();
            REQUIRE(playbackRet.value()->GetState() == Operation::State::Idle);
        }

    }

}


