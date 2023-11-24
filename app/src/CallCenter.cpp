#include <fstream>

#include "CallCenter.h"


CallCenter::CallCenter(size_t queueSize, size_t operatorsSize)
    : queueSize(queueSize), operators(operatorsSize)
{}

void CallCenter::registerCall(const std::string& phone, Date date)
{
    CallDetail callDetail(phone);
    callDetail.recordReceiption(date);

    if (isQueueFull())
    {
        callDetail.recordEnding(CallEndingStatus::OVERLOAD, date);
        makeCallDetailRecord(callDetail);
        return;
    }

    awaitingCalls.push_back(callDetail.getId());
    calls.emplace(callDetail.getId(), std::move(callDetail));
}

void CallCenter::endCall(IdType callId, CallEndingStatus callEndingStatus, Date date)
{
    CallDetail& callDetail = calls[callId];

    callDetail.recordEnding(callEndingStatus, date);
    makeCallDetailRecord(callDetail);

    freeOperators.push_back(callDetail.getOperatorId());
    calls.erase(callId);
}

void CallCenter::makeCallDetailRecord(const CallDetail& callDetail) const
{
    std::ofstream out;
    out.open(journalPath);
    out << callDetail.toString();
    out.close();
}

bool CallCenter::isQueueFull() const
{
    return awaitingCalls.size() == queueSize;
}