#include "callcenter/CallDetail.h"
#include "utils/Exceptions.h"

#include <boost/date_time/posix_time/time_parsers.hpp>
#include <gtest/gtest.h>

using boost::posix_time::microsec_clock;

TEST(CallDetailRecord, CDRContains8Segments)
{
    const CallDetail callDetail("+71234567890");

    std::string record = callDetail.toString();
    const size_t segments = std::ranges::count(record, ';');

    ASSERT_EQ(segments, 8);
}

TEST(CallDetailRecord, CDRContains8SegmentsAfterRecordReceiption)
{
    CallDetail callDetail("+71234567890");

    callDetail.recordReceiption(microsec_clock::local_time());
    std::string record = callDetail.toString();
    const size_t segments = std::ranges::count(record, ';');

    ASSERT_EQ(segments, 8);
}

TEST(CallDetailRecord, CDRContains8SegmentsAfterRecordResponse)
{
    CallDetail callDetail("+71234567890");

    callDetail.recordReceiption(microsec_clock::local_time());
    callDetail.recordResponse(1, microsec_clock::local_time());
    std::string record = callDetail.toString();
    const size_t segments = std::ranges::count(record, ';');

    ASSERT_EQ(segments, 8);
}

TEST(CallDetailRecord, CDRContains8SegmentsAfterRecordEnding)
{
    CallDetail callDetail("+71234567890");

    callDetail.recordReceiption(microsec_clock::local_time());
    callDetail.recordEnding(CallEndingStatus::OK, microsec_clock::local_time());
    std::string record = callDetail.toString();
    const size_t segments = std::ranges::count(record, ';');

    ASSERT_EQ(segments, 8);
}

TEST(CallDetailRecord, CDThrowsExceptionWithoutReceiption)
{
    CallDetail callDetailOne("+71234567890");
    CallDetail callDetailTwo("+71234567890");

    auto response = [&callDetailOne] {callDetailOne.recordResponse(1, microsec_clock::local_time());};
    auto endCall = [&callDetailTwo] {callDetailTwo.recordEnding(CallEndingStatus::OK, microsec_clock::local_time());};

    EXPECT_THROW(response(), CCenter::WrongRocordingOrder);
    EXPECT_THROW(endCall(), CCenter::WrongRocordingOrder);
}

TEST(CallDetailRecord, CDRDosntHaveLast3FieldsWithoutResposeWithAnyEndingStatus)
{
    CallDetail callDetail("+71234567890");

    const std::string emptyRecord = callDetail.toString();

    callDetail.recordReceiption(microsec_clock::local_time());
    const std::string receiptionRecord = callDetail.toString();

    callDetail.recordEnding(CallEndingStatus::OK, microsec_clock::local_time());
    const std::string endingRecordOk = callDetail.toString();

    callDetail.recordEnding(CallEndingStatus::TIMEOUT, microsec_clock::local_time());
    const std::string endingRecordTimeout = callDetail.toString();

    callDetail.recordEnding(CallEndingStatus::OVERLOAD, microsec_clock::local_time());
    const std::string endingRecordOverload = callDetail.toString();

    EXPECT_TRUE(emptyRecord.ends_with(";;;")) << emptyRecord;
    EXPECT_TRUE(receiptionRecord.ends_with(";;;")) << receiptionRecord;
    EXPECT_TRUE(endingRecordOk.ends_with(";;;")) << endingRecordOk;
    EXPECT_TRUE(endingRecordTimeout.ends_with(";;;")) << endingRecordTimeout;
    EXPECT_TRUE(endingRecordOverload.ends_with(";;;")) << endingRecordOverload;
}