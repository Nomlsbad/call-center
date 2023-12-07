#include "callcenter/CallCenter.h"
#include "utils/Exceptions.h"

#include <sstream>

#include <boost/date_time/posix_time/posix_time.hpp>


IdType CallDetail::nextId = 1;

CallDetail::CallDetail(std::string phone)
    : phone(std::move(phone)){};

std::string CallDetail::toString() const
{
    std::stringstream strStream;

    strStream << to_simple_string(receiptDate) << ";";
    strStream << id << ";";
    strStream << phone << ";";
    strStream << to_simple_string(endingDate) << ";";
    strStream << getEndingStatusAsString() << ";";

    if (!responseDate.is_not_a_date_time())
    {
        strStream << to_simple_string(responseDate) << ";";
        strStream << operatorId << ";";
        strStream << duration.seconds() << ";";
    }
    else
    {
        strStream << ";;;";
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

void CallDetail::recordReceiption(Date date)
{
    id = std::max<IdType>(1, nextId++);
    receiptDate = date;
}

void CallDetail::recordResponse(IdType acceptedOperatotId, Date date)
{
    if (id == 0) throw CCenter::WrongRocordingOrder("Not allowed to record responce befor receiption");
    responseDate = date;
    operatorId = acceptedOperatotId;
}

void CallDetail::recordEnding(CallEndingStatus status, Date date)
{
    if (id == 0) throw CCenter::WrongRocordingOrder("Not allowed to record ending befor receiption");
    endingDate = date;
    endingStatus = status;

    if (responseDate.is_not_a_date_time()) return;
    duration = endingDate - responseDate;
}

IdType CallDetail::getId() const
{
    return id;
}

IdType CallDetail::getOperatorId() const
{
    return operatorId;
}

std::string CallDetail::getPhone() const
{
    return phone;
}