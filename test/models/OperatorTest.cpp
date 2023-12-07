#include "callcenter/CallCenter.h"
#include "config/Configuration.h"
#include "utils/Exceptions.h"

#include <boost/date_time/posix_time/time_parsers.hpp>
#include <gtest/gtest.h>

using boost::posix_time::microsec_clock;

TEST(OperatorTest, CanConnect)
{
    const auto callCenter = std::make_shared<CallCenter>();
    const auto mobileOperator = std::make_shared<Operator>();

    mobileOperator->connect(callCenter);

    EXPECT_TRUE(mobileOperator->isConnected());
}

TEST(OperatorTest, TowDifferentOperatorsHaveDifferentId)
{
    const auto callCenter = std::make_shared<CallCenter>();
    const auto mobileOperatorOne = std::make_shared<Operator>();
    const auto mobileOperatorTwo = std::make_shared<Operator>();

    mobileOperatorOne->connect(callCenter);
    mobileOperatorTwo->connect(callCenter);

    EXPECT_NE(mobileOperatorOne->getId(), mobileOperatorTwo->getId());
}

TEST(OperatorTest, CanAcceptCallInQueue)
{
    IdType callId = 0;
    const auto callCenter = std::make_shared<CallCenter>();
    const auto mobileOperator = std::make_shared<Operator>();

    callCenter->registerCall(callId, "+71234567890", microsec_clock::local_time());
    mobileOperator->connect(callCenter);

    EXPECT_NO_THROW(mobileOperator->acceptCall(callId));
    EXPECT_TRUE(mobileOperator->wasBusy());
    EXPECT_EQ(mobileOperator->getLastCallId(), callId);
}

TEST(OperatorTest, CantAcceptCallAfterEnding)
{
    IdType callId = 0;
    const auto callCenter = std::make_shared<CallCenter>();
    const auto mobileOperator = std::make_shared<Operator>();

    callCenter->registerCall(callId, "+71234567890", microsec_clock::local_time());
    callCenter->endCall(callId, CallEndingStatus::TIMEOUT, microsec_clock::local_time());
    mobileOperator->connect(callCenter);
    mobileOperator->acceptCall(callId);

    EXPECT_FALSE(mobileOperator->wasBusy());
    EXPECT_EQ(mobileOperator->getLastCallId(), 0);
}

TEST(OperatorTest, CantAcceptCallAfterResponse)
{
    IdType callId = 0;
    const auto callCenter = std::make_shared<CallCenter>();
    const auto mobileOperatorOne = std::make_shared<Operator>();
    const auto mobileOperatorTwo = std::make_shared<Operator>();

    callCenter->registerCall(callId, "+71234567890", microsec_clock::local_time());
    mobileOperatorOne->connect(callCenter);
    mobileOperatorTwo->connect(callCenter);

    mobileOperatorOne->acceptCall(callId);

    EXPECT_THROW(mobileOperatorTwo->acceptCall(callId), CCenter::DoubleResponse);
    EXPECT_FALSE(mobileOperatorTwo->wasBusy());
    EXPECT_EQ(mobileOperatorTwo->getLastCallId(), 0);
}