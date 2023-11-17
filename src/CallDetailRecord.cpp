#include <sstream>

#include "CallDetailRecord.h"

CallDetailRecord::CallDetailRecord(String  phone)
    : id(nextId), phone(std::move(phone))
{
    ++nextId;
}

CallDetailRecord::String CallDetailRecord::makeCallReport() const
{
    std::stringstream strStream;

    strStream << "CallId:" << id << "|";
    strStream << "Phone:" << phone << "|";
    strStream << "CallReceiptDate:" << to_simple_string(receiptDate) << "|";
    strStream << "CallEndingDate:" << to_simple_string(endingDate) << "|";
    strStream << "CallEndingStatus:" << getEndingStatusAsString() << "|";
    strStream << "CallEndingDate:" << to_simple_string(endingDate) << "|";

    if (hasEndingStatus(CallEndingStatus::OK))
    {
        strStream << "CallEndingDate:" << to_simple_string(answerDate) << "|";
        strStream << "OperatorId:" << operatorId << "|";
        strStream << "CallEndingDate:" << duration.seconds() << "|";
    }
    else
    {
        strStream << "CallEndingDate:|OperatorId:|CallEndingDate:|";
    }

    String result;
    strStream >> result;

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
