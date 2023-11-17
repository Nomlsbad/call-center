#include <boost/date_time/posix_time/time_parsers.hpp>
#include <gtest/gtest.h>

#include "CallDetailRecord.h"

struct StringSection
{
    StringSection() = default;
    StringSection(size_t begin, size_t end) : begin(begin), end(end) {};

    StringSection(const StringSection& section) = default;
    StringSection& operator= (const StringSection& section) = default;

    [[nodiscard]] size_t length() const { return end - begin; }

    size_t begin = 0;
    size_t end = 0;

};

StringSection getCDRSection(const std::string& cdr, const std::string& strSection)
{
    const size_t shift = cdr.find(strSection);
    const size_t begin = cdr.find(':', shift) + 1;
    const size_t end = cdr.find('|', shift);

    return {begin, end};
}

TEST(CallDetailRecord, AssignId)
{
    const CallDetailRecord cdr1("");
    const CallDetailRecord cdr2("");
    const CallDetailRecord cdr3("");

    for (int i = 0; i < 30; ++i)
    {
        CallDetailRecord tempCdr("");
    }
    CallDetailRecord lastCdr("");

    EXPECT_EQ(cdr1.getId(), 1);
    EXPECT_EQ(cdr2.getId(), 2);
    EXPECT_EQ(cdr3.getId(), 3);
    EXPECT_EQ(lastCdr.getId(), 34);
}

TEST(CallDetailRecord, emptyCDRFormat)
{
    const CallDetailRecord emptyCDR("");

    std::string emptyCallDetail = emptyCDR.makeCallReport();

    EXPECT_NE(emptyCallDetail.find("CallId:"), std::string::npos);
    EXPECT_NE(emptyCallDetail.find("Phone:"), std::string::npos);
    EXPECT_NE(emptyCallDetail.find("CallReceiptDate:"), std::string::npos);
    EXPECT_NE(emptyCallDetail.find("CallEndingDate:"), std::string::npos);
    EXPECT_NE(emptyCallDetail.find("CallEndingStatus:"), std::string::npos);
    EXPECT_NE(emptyCallDetail.find("CallResponseDate:"), std::string::npos);
    EXPECT_NE(emptyCallDetail.find("OperatorId:"), std::string::npos);
    EXPECT_NE(emptyCallDetail.find("CallDuration:"), std::string::npos);
}

TEST(CallDetailRecord, fillCDRFormat)
{
    CallDetailRecord fillCDR("");

    fillCDR.recordCallReceiption();
    fillCDR.recordCallResponse(13);
    fillCDR.recordCallEnding(CallEndingStatus::OK);
    std::string emptyCallDetail = fillCDR.makeCallReport();

    EXPECT_NE(emptyCallDetail.find("CallId:"), std::string::npos);
    EXPECT_NE(emptyCallDetail.find("Phone:"), std::string::npos);
    EXPECT_NE(emptyCallDetail.find("CallReceiptDate:"), std::string::npos);
    EXPECT_NE(emptyCallDetail.find("CallEndingDate:"), std::string::npos);
    EXPECT_NE(emptyCallDetail.find("CallEndingStatus:"), std::string::npos);
    EXPECT_NE(emptyCallDetail.find("CallResponseDate:"), std::string::npos);
    EXPECT_NE(emptyCallDetail.find("OperatorId:"), std::string::npos);
    EXPECT_NE(emptyCallDetail.find("CallDuration:"), std::string::npos);
}

TEST(CallDetailRecord, recordCalls)
{
    CallDetailRecord cdr("+79966971812");

    cdr.recordCallReceiption();
    cdr.recordCallResponse(13);
    cdr.recordCallEnding(CallEndingStatus::OK);

    const std::string callDetail = cdr.makeCallReport();

    const StringSection callIdSection = getCDRSection(callDetail, "CallId");
    const StringSection phoneSection = getCDRSection(callDetail, "Phone");
    const StringSection callReceiptSection = getCDRSection(callDetail, "CallReceiptDate");
    const StringSection callEndingSection = getCDRSection(callDetail, "CallEndingDate");
    const StringSection callEndingStatusSection = getCDRSection(callDetail, "CallEndingStatus");
    const StringSection callResponseSection = getCDRSection(callDetail, "CallResponseDate");
    const StringSection operatorIdSection = getCDRSection(callDetail, "OperatorId");
    const StringSection callDurationSection = getCDRSection(callDetail, "CallDuration");

    const std::string callId = callDetail.substr(callIdSection.begin, callIdSection.length());
    const std::string phone = callDetail.substr(phoneSection.begin, phoneSection.length());
    const std::string receiptDate = callDetail.substr(callReceiptSection.begin, callReceiptSection.length());
    const std::string endingDate = callDetail.substr(callEndingSection.begin, callEndingSection.length());
    const std::string endingStatus = callDetail.substr(callEndingStatusSection.begin, callEndingStatusSection.length());
    const std::string responseDate = callDetail.substr(callResponseSection.begin, callResponseSection.length());
    const std::string operatorId = callDetail.substr(operatorIdSection.begin, operatorIdSection.length());
    const std::string duration = callDetail.substr(callDurationSection.begin, callDurationSection.length());

    EXPECT_EQ(callId, "1");
    EXPECT_EQ(phone, "+79966971812");
    EXPECT_NE(receiptDate, "not-a-date-time");
    EXPECT_NO_THROW(boost::posix_time::time_from_string(receiptDate));
    EXPECT_NE(endingDate, "not-a-date-time");
    EXPECT_NO_THROW(boost::posix_time::time_from_string(endingDate));
    EXPECT_EQ(endingStatus, "OK");
    EXPECT_NE(responseDate, "not-a-date-time");
    EXPECT_NE(responseDate, "");
    EXPECT_NO_THROW(boost::posix_time::time_from_string(responseDate));
    EXPECT_EQ(operatorId, "13");
    EXPECT_NE(duration, "");
    EXPECT_NO_THROW(boost::posix_time::duration_from_string(duration));

}