#include "callcenter/CallCenter.h"
#include "callcenter/UserSimulation.h"
#include "config/Configuration.h"
#include "utils/Exceptions.h"

#include <boost/date_time/posix_time/time_parsers.hpp>
#include <gtest/gtest.h>

using boost::posix_time::microsec_clock;

TEST(UserSimulationTest, AfterOnRegisterCallUserWillWait)
{
    Configuration::get<UserConfig>("minWaitingTime") = "00:00:00";
    Configuration::get<UserConfig>("maxWaitingTime") = "00:00:00";
    IdType callId = 0;
    auto callCenter = std::make_shared<CallCenter>();
    auto userSimulation = std::make_shared<UserSimulation>(callCenter);

    auto endCall = [&callCenter](IdType callId)
    { callCenter->endCall(callId, CallEndingStatus::TIMEOUT, microsec_clock::local_time()); };

    callCenter->registerCall(callId, "+71234567890", microsec_clock::local_time());
    userSimulation->onRegisterCall(callId, "+71234567890");

    std::this_thread::sleep_for(std::chrono::milliseconds(50));

    EXPECT_THROW(endCall(callId), std::out_of_range);
}

TEST(UserSimulationTest, AfterRegisterCallWillOnRegisterCall)
{
    Configuration::get<UserConfig>("minWaitingTime") = "00:00:00";
    Configuration::get<UserConfig>("maxWaitingTime") = "00:00:00";
    IdType callId = 0;
    auto callCenter = std::make_shared<CallCenter>();

    auto endCall = [&callCenter](IdType callId)
    { callCenter->endCall(callId, CallEndingStatus::TIMEOUT, microsec_clock::local_time()); };

    callCenter->run(std::make_shared<UserSimulation>(callCenter));
    callCenter->registerCall(callId, "+71234567890", microsec_clock::local_time());

    std::this_thread::sleep_for(std::chrono::milliseconds(50));

    EXPECT_THROW(endCall(callId), std::out_of_range);
}

TEST(UserSimulationTest, AfterOnRegisterCallUserWillTalk)
{
    Configuration::get<UserConfig>("minWaitingTime") = "00:00:05";
    Configuration::get<UserConfig>("maxWaitingTime") = "00:00:05";
    Configuration::get<UserConfig>("minTalkingTime") = "00:00:00";
    Configuration::get<UserConfig>("maxTalkingTime") = "00:00:00";
    IdType callId = 0;
    auto callCenter = std::make_shared<CallCenter>();
    auto userSimulation = std::make_shared<UserSimulation>(callCenter);

    auto endCall = [&callCenter](IdType callId)
    { callCenter->endCall(callId, CallEndingStatus::TIMEOUT, microsec_clock::local_time()); };

    callCenter->registerCall(callId, "+71234567890", microsec_clock::local_time());
    userSimulation->onRegisterCall(callId, "+71234567890");
    userSimulation->onResponse(callId);

    std::this_thread::sleep_for(std::chrono::milliseconds(50));

    EXPECT_THROW(endCall(callId), std::out_of_range);
}

TEST(UserSimulationTest, AfterRegisterCallWillOnResponceIfFreeOperatorExist)
{
    Configuration::get<UserConfig>("minWaitingTime") = "00:00:00.01";
    Configuration::get<UserConfig>("maxWaitingTime") = "00:00:00.01";
    IdType callId = 0;
    auto callCenter = std::make_shared<CallCenter>();
    auto mobileOperator = std::make_shared<Operator>();

    auto endCall = [&callCenter](IdType callId)
    { callCenter->endCall(callId, CallEndingStatus::TIMEOUT, microsec_clock::local_time()); };

    mobileOperator->connect(callCenter);
    callCenter->run(std::make_shared<UserSimulation>(callCenter));
    callCenter->registerCall(callId, "+71234567890", microsec_clock::local_time());

    std::this_thread::sleep_for(std::chrono::milliseconds(50));

    EXPECT_THROW(endCall(callId), std::out_of_range);
}