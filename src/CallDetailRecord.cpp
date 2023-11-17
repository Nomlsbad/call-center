#include <sstream>

#include <boost/date_time/posix_time/posix_time.hpp>

#include "CallDetailRecord.h"


CallDetailRecord::String CallDetailRecord::makeCallReport() const
{
    std::stringstream strStream;

    strStream << "CallId:" << id << "|";
    strStream << "Phone:" << phone << "|";
    strStream << "CallReceiptDate:" << to_simple_string(receiptDate) << "|";
    strStream << "CallEndingDate:" << to_simple_string(endingDate) << "|";
    strStream << "CallEndingStatus:" << getEndingStatusAsString() << "|";
    strStream << "CallEndingDate:" << to_simple_string(endingDate) << "|";

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

    String result = strStream.str();

    return result;
}

CallDetailRecord::String CallDetailRecord::getEndingStatusAsString() const
{
    switch (endingStatus)
    {
    case CallEndingStatus::OK: return "OK";
    case CallEndingStatus::TIMEOUT: return "TIMEOUT";
    case CallEndingStatus::OVERLOAD: return "OVERLOAD";
    default: return "STATUS_NONE";
    }
}

void CallDetailRecord::recordCallReceiption()
{
    receiptDate = boost::posix_time::microsec_clock::local_time();
}

void CallDetailRecord::recordCallResponse(IdType acceptedOperatotId)
{
    responseDate = boost::posix_time::microsec_clock::local_time();
    operatorId = acceptedOperatotId;
}

void CallDetailRecord::recordCallEnding(CallEndingStatus status)
{
    endingDate = boost::posix_time::microsec_clock::local_time();
    endingStatus = status;

    if (endingStatus == CallEndingStatus::OK)
    {
        duration = endingDate - responseDate;
    }
}
