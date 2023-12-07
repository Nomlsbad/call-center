#include "callcenter/CallCenter.h"
#include "config/Configuration.h"
#include "utils/Exceptions.h"

#include <boost/date_time/posix_time/time_parsers.hpp>
#include <gtest/gtest.h>

using boost::posix_time::microsec_clock;

TEST(CallCenterTest, CanRegister2DifferentCallsIfQueueSizeIs2AndNoOperators)
{
    Configuration::get<CallCenterConfig>("queueSize") = 2;
    auto callCenter = std::make_shared<CallCenter>();
    IdType firstCallId = 0;
    IdType secondCallId = 0;

    auto registerCall = [&callCenter](IdType& callId, const std::string& phone)
    { callCenter->registerCall(callId, phone, microsec_clock::local_time()); };

    callCenter->run();

    EXPECT_NO_THROW(registerCall(firstCallId, "+71234567890"));
    EXPECT_NO_THROW(registerCall(secondCallId, "+71234567891"));
    EXPECT_TRUE(firstCallId != secondCallId);
}

TEST(CallCenterTest, CantRegister2SameCalls)
{
    Configuration::get<CallCenterConfig>("queueSize") = 2;
    auto callCenter = std::make_shared<CallCenter>();
    IdType firstCallId = 0;
    IdType secondCallId = 0;

    auto registerCall = [&callCenter](IdType& callId, const std::string& phone)
    { callCenter->registerCall(callId, phone, microsec_clock::local_time()); };

    callCenter->run();

    EXPECT_NO_THROW(registerCall(firstCallId, "+71234567890"));
    EXPECT_THROW(registerCall(secondCallId, "+71234567890"), CCenter::AlreadyInQueue);
}

TEST(CallCenterTest, CantRegister2DifferentCallsIfQueueSizeIs1AndNoOperators)
{
    Configuration::get<CallCenterConfig>("queueSize") = 1;
    auto callCenter = std::make_shared<CallCenter>();
    IdType firstCallId = 0;
    IdType secondCallId = 0;

    auto registerCall = [&callCenter](IdType& callId, const std::string& phone)
    { callCenter->registerCall(callId, phone, microsec_clock::local_time()); };

    callCenter->run();

    EXPECT_NO_THROW(registerCall(firstCallId, "+71234567890"));
    EXPECT_THROW(registerCall(secondCallId, "+71234567890"), CCenter::Overload);
}

TEST(CallCenterTest, CanRegister2DifferentCallsIfQueueSizeAndOperatorsAre1)
{
    Configuration::get<CallCenterConfig>("queueSize") = 1;
    auto callCenter = std::make_shared<CallCenter>();
    const auto mobileOperator = std::make_shared<Operator>();
    IdType firstCallId = 0;
    IdType secondCallId = 0;

    auto registerCall = [&callCenter](IdType& callId, const std::string& phone)
    { callCenter->registerCall(callId, phone, microsec_clock::local_time()); };

    mobileOperator->connect(callCenter);
    callCenter->run();

    EXPECT_NO_THROW(registerCall(firstCallId, "+71234567890"));
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    EXPECT_NO_THROW(registerCall(secondCallId, "+71234567891"));
    EXPECT_TRUE(firstCallId != secondCallId);
}

TEST(CallCenterTest, CantRegisterCallIfQueueSizeIsZeroButOperatorsIsntZero)
{
    Configuration::get<CallCenterConfig>("queueSize") = 0;
    auto callCenter = std::make_shared<CallCenter>();
    const auto mobileOperator = std::make_shared<Operator>();
    IdType callId = 0;

    auto registerCall = [&callCenter](IdType& callId, const std::string& phone)
    { callCenter->registerCall(callId, phone, microsec_clock::local_time()); };

    mobileOperator->connect(callCenter);
    callCenter->run();

    EXPECT_THROW(registerCall(callId, "+71234567890"), CCenter::Overload);
}

TEST(CallCenterTest, CantRegister2SameCallsWith1Operator)
{
    Configuration::get<CallCenterConfig>("queueSize") = 2;
    auto callCenter = std::make_shared<CallCenter>();
    const auto mobileOperator = std::make_shared<Operator>();
    IdType firstCallId = 0;
    IdType secondCallId = 0;

    auto registerCall = [&callCenter](IdType& callId, const std::string& phone)
    { callCenter->registerCall(callId, phone, microsec_clock::local_time()); };

    mobileOperator->connect(callCenter);
    callCenter->run();

    EXPECT_NO_THROW(registerCall(firstCallId, "+71234567890"));
    EXPECT_THROW(registerCall(secondCallId, "+71234567890"), CCenter::AlreadyInQueue);
}

TEST(CallCenterTest, CantResponseOnRandomCall)
{
    Configuration::get<CallCenterConfig>("queueSize") = 1;
    auto callCenter = std::make_shared<CallCenter>();
    IdType callId = 0;

    auto responseCall = [&callCenter](IdType callId)
    { callCenter->responseCall(callId, 1, microsec_clock::local_time()); };

    callCenter->run();
    callCenter->registerCall(callId, "+71234567890", microsec_clock::local_time());

    EXPECT_NO_THROW(responseCall(callId));
    EXPECT_THROW(responseCall(6847), std::out_of_range);
    EXPECT_THROW(responseCall(2), std::out_of_range);
    EXPECT_THROW(responseCall(-424), std::out_of_range);
}

TEST(CallCenterTest, CantResponseTwoTimes)
{
    Configuration::get<CallCenterConfig>("queueSize") = 1;
    auto callCenter = std::make_shared<CallCenter>();
    IdType callId = 0;

    auto responseCall = [&callCenter](IdType callId)
    { callCenter->responseCall(callId, 1, microsec_clock::local_time()); };

    callCenter->run();
    callCenter->registerCall(callId, "+71234567890", microsec_clock::local_time());

    EXPECT_NO_THROW(responseCall(callId));
    EXPECT_THROW(responseCall(callId), CCenter::DoubleResponse);
}

TEST(CallCenterTest, CanEndCallInQueue)
{
    Configuration::get<CallCenterConfig>("queueSize") = 1;
    auto callCenter = std::make_shared<CallCenter>();
    IdType callId = 0;

    auto endCall = [&callCenter](IdType callId)
    { callCenter->endCall(callId, CallEndingStatus::TIMEOUT, microsec_clock::local_time()); };

    callCenter->run();
    callCenter->registerCall(callId, "+71234567890", microsec_clock::local_time());

    EXPECT_NO_THROW(endCall(callId));
}

TEST(CallCenterTest, CanEndCallInTalking)
{
    Configuration::get<CallCenterConfig>("queueSize") = 1;
    auto callCenter = std::make_shared<CallCenter>();
    const auto mobileOperator = std::make_shared<Operator>();
    IdType callId = 0;

    auto endCall = [&callCenter](IdType callId)
    { callCenter->endCall(callId, CallEndingStatus::OK, microsec_clock::local_time()); };

    mobileOperator->connect(callCenter);
    callCenter->run();
    callCenter->registerCall(callId, "+71234567890", microsec_clock::local_time());

    EXPECT_NO_THROW(endCall(callId));
}

TEST(CallCenterTest, CantEndRandomCall)
{
    Configuration::get<CallCenterConfig>("queueSize") = 1;
    auto callCenter = std::make_shared<CallCenter>();
    IdType callId = 0;

    auto endCall = [&callCenter](IdType callId)
    { callCenter->endCall(callId, CallEndingStatus::OK, microsec_clock::local_time()); };

    callCenter->run();
    callCenter->registerCall(callId, "+71234567890", microsec_clock::local_time());

    EXPECT_THROW(endCall(6847), std::out_of_range) << 1;
    EXPECT_THROW(endCall(2), std::out_of_range) << 2;
    EXPECT_THROW(endCall(-424), std::out_of_range) << 3;
    EXPECT_NO_THROW(endCall(callId));
}