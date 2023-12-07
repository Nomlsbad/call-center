#include "callcenter/CallCenter.h"
#include "config/Configuration.h"
#include "models/User.h"
#include "utils/Exceptions.h"

#include <boost/date_time/posix_time/time_parsers.hpp>
#include <gtest/gtest.h>

using boost::posix_time::microsec_clock;

TEST(UserTest, AfterWaitingCallWillEnd)
{
    auto callCenter = std::make_shared<CallCenter>();
    IdType callId = 0;

    // without slepping there will be segfault, becose callCenter doesn't exist enough
    auto wait = [](User& user)
    {
        user.wait(TimeDuration(0, 0, 0));
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    };

    auto endCall = [&callCenter](IdType callId)
    { callCenter->endCall(callId, CallEndingStatus::TIMEOUT, microsec_clock::local_time()); };

    callCenter->registerCall(callId, "+71234567890", microsec_clock::local_time());

    User user(callId, "+71234567890", callCenter);
    wait(user);

    EXPECT_THROW(endCall(callId), std::out_of_range);
}

TEST(UserTest, CantDoubleWait)
{
    const auto callCenter = std::make_shared<CallCenter>();
    IdType callId = 0;

    auto wait = [](User& user)
    {
        user.wait(TimeDuration(0, 0, 0));
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    };

    callCenter->registerCall(callId, "+71234567890", microsec_clock::local_time());
    User user(callId, "+71234567890", callCenter);

    EXPECT_NO_THROW(wait(user));
    EXPECT_NO_THROW(wait(user));
}

TEST(UserTest, AfterTalkingCallWillEnd)
{
    auto callCenter = std::make_shared<CallCenter>();
    IdType callId = 0;

    auto talk = [](User& user)
    {
        user.talk(TimeDuration(0, 0, 0));
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    };

    auto endCall = [&callCenter](IdType callId)
    { callCenter->endCall(callId, CallEndingStatus::TIMEOUT, microsec_clock::local_time()); };

    callCenter->registerCall(callId, "+71234567890", microsec_clock::local_time());

    User user(callId, "+71234567890", callCenter);
    user.wait(TimeDuration(0, 0, 10));
    user.response();

    talk(user);

    EXPECT_THROW(endCall(callId), std::out_of_range);
}

TEST(UserTest, CantTalkingBeforWaiting)
{
    const auto callCenter = std::make_shared<CallCenter>();
    IdType callId = 0;

    auto talk = [](User& user)
    {
        user.talk(TimeDuration(0, 0, 0));
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    };

    auto endCall = [&callCenter](IdType callId)
    { callCenter->endCall(callId, CallEndingStatus::TIMEOUT, microsec_clock::local_time()); };

    callCenter->registerCall(callId, "+71234567890", microsec_clock::local_time());
    User user(callId, "+71234567890", callCenter);

    EXPECT_NO_THROW(talk(user));
    EXPECT_NO_THROW(endCall(callId));
}

TEST(UserTest, IfResponceWasCalledCallWontEnd)
{
    auto callCenter = std::make_shared<CallCenter>();
    IdType callId = 0;

    auto endCall = [&callCenter](IdType callId)
    { callCenter->endCall(callId, CallEndingStatus::TIMEOUT, microsec_clock::local_time()); };

    callCenter->registerCall(callId, "+71234567890", microsec_clock::local_time());

    User user(callId, "+71234567890", callCenter);
    user.wait(TimeDuration(boost::posix_time::milliseconds(100)));
    user.response();

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    EXPECT_NO_THROW(endCall(callId));
}