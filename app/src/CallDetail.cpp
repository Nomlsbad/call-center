#include <sstream>

#include <boost/date_time/posix_time/posix_time.hpp>

#include "CallDetail.h"
#include "utils/Exceptions.h"

IdType CallDetail::nextId = 1;

CallDetail::CallDetail(std::string phone)
    : phone(std::move(phone)){};

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
    case CallEndingStatus::OK:
        return "OK";
    case CallEndingStatus::TIMEOUT:
        return "TIMEOUT";
    case CallEndingStatus::OVERLOAD:
        return "OVERLOAD";
    default:
        return "STATUS_NONE";
    }
}

void CallDetail::recordReceiption(Date date)
{
    if (recordingStatus != CREATED)
    {
        throw CCenter::CallDetailRecordError("recordReceiption must be called first");
    }

    id = nextId++;
    receiptDate = date;
    recordingStatus = ACCEPTED;
}

void CallDetail::recordResponse(IdType acceptedOperatotId, Date date)
{
    if (recordingStatus != ACCEPTED)
    {
        throw CCenter::CallDetailRecordError("recordResponce must be called after receiption and befor ending");
    }

    responseDate = date;
    operatorId = acceptedOperatotId;
    recordingStatus = RESPONDED;
}

void CallDetail::recordEnding(CallEndingStatus status, Date date)
{
    if (recordingStatus != ACCEPTED && recordingStatus != RESPONDED)
    {
        throw CCenter::CallDetailRecordError("recordEnding must be called after receiption or response");
    }

    endingDate = date;
    endingStatus = status;

    if (endingStatus == CallEndingStatus::OK)
    {
        duration = endingDate - responseDate;
    }
    recordingStatus = ENDED;
}

IdType CallDetail::getId() const
{
    return id;
}

IdType CallDetail::getOperatorId() const
{
    return operatorId;
}