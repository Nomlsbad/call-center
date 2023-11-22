#include <boost/date_time/posix_time/time_parsers.hpp>
#include <gtest/gtest.h>

#include "CallDetail.h"

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

TEST(CallDetailRecord, CallDetailReportFormat)
{
    CallDetail callDetail("");

    callDetail.recordReceiption();
    callDetail.recordResponse(13);
    callDetail.recordEnding(CallEndingStatus::OK);
    std::string emptyCallDetail = callDetail.toString();

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
    CallDetail callDetail("+79966971812");

    callDetail.recordReceiption();
    callDetail.recordResponse(13);
    callDetail.recordEnding(CallEndingStatus::OK);

    const std::string callReport = callDetail.toString();

    const StringSection phoneSection = getCDRSection(callReport, "Phone");
    const StringSection callReceiptSection = getCDRSection(callReport, "CallReceiptDate");
    const StringSection callEndingSection = getCDRSection(callReport, "CallEndingDate");
    const StringSection callEndingStatusSection = getCDRSection(callReport, "CallEndingStatus");
    const StringSection callResponseSection = getCDRSection(callReport, "CallResponseDate");
    const StringSection operatorIdSection = getCDRSection(callReport, "OperatorId");
    const StringSection callDurationSection = getCDRSection(callReport, "CallDuration");

    const std::string phone = callReport.substr(phoneSection.begin, phoneSection.length());
    const std::string receiptDate = callReport.substr(callReceiptSection.begin, callReceiptSection.length());
    const std::string endingDate = callReport.substr(callEndingSection.begin, callEndingSection.length());
    const std::string endingStatus = callReport.substr(callEndingStatusSection.begin, callEndingStatusSection.length());
    const std::string responseDate = callReport.substr(callResponseSection.begin, callResponseSection.length());
    const std::string operatorId = callReport.substr(operatorIdSection.begin, operatorIdSection.length());
    const std::string duration = callReport.substr(callDurationSection.begin, callDurationSection.length());

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