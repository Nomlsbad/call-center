#include <sstream>

#include <boost/date_time/posix_time/posix_time.hpp>

#include "CallDetail.h"

IdType CallDetail::nextId = 1;

std::string CallDetail::toString() const
{
    std::stringstream strStream;

    strStream << "CallId:" << id << "|";
    strStream << "Phone:" << phone << "|";
    strStream << "CallReceiptDate:" << to_simple_string(receiptDate) << "|";
    strStream << "CallEndingDate:" << to_simple_string(endingDate) << "|";
    strStream << "CallEndingStatus:" << getEndingStatusAsString() << "|";

    if (endingStatus == CallEndingStatus::OK)
    {
        strStream << "CallResponseDate:" << to_simple_string(responseDate) << "|";
        strStream << "OperatorId:" << operatorId << "|";
        strStream << "CallDuration:" << duration.seconds() << "|";
    }
    else
    {
        strStream << "CallResponseDate:|OperatorId:|CallDuration:|";
    }

    std::string result = strStream.str();

    return result;
}

std::string CallDetail::getEndingStatusAsString() const
{
    switch (endingStatus)
    {
    case CallEndingStatus::OK: return "OK";
    case CallEndingStatus::TIMEOUT: return "TIMEOUT";
    case CallEndingStatus::OVERLOAD: return "OVERLOAD";
    default: return "STATUS_NONE";
    }
}

void CallDetail::recordReceiption()
{
    id = nextId++;
    receiptDate = boost::posix_time::microsec_clock::local_time();
}

void CallDetail::recordResponse(IdType acceptedOperatotId)
{
    responseDate = boost::posix_time::microsec_clock::local_time();
    operatorId = acceptedOperatotId;
}

void CallDetail::recordEnding(CallEndingStatus status)
{
    endingDate = boost::posix_time::microsec_clock::local_time();
    endingStatus = status;

    if (endingStatus == CallEndingStatus::OK)
    {
        duration = endingDate - responseDate;
    }
}
