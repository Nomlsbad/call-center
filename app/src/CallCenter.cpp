#include <fstream>

#include "CallCenter.h"


CallCenter::CallCenter(size_t queueSize, size_t operatorsSize)
    : queueSize(queueSize), operators(operatorsSize)
{}

void CallCenter::registerCall(const std::string& phone)
{
    CallDetail callDetail(phone);
    callDetail.recordReceiption();

    if (!isQueueFull())
    {
        awaitingCalls.push_back(std::move(callDetail));
    }
    else
    {
        callDetail.recordEnding(CallEndingStatus::OVERLOAD);
        makeRecord(callDetail);
    }
}

void CallCenter::endCall(IdType callId, CallEndingStatus callEndingStatus)
{
    auto isIdEquals = [id = callId](const auto& callDetail) { return callDetail.getId() == id; };

    std::vector<CallDetail>& callHolder = getCDHolderByEndingStatus(callEndingStatus);
    const auto callDetail = std::ranges::find_if(callHolder, isIdEquals);
    if (callDetail == callHolder.end()) return;

    callDetail->recordEnding(callEndingStatus);
    callHolder.erase(callDetail);
    makeRecord(*callDetail);
}

void CallCenter::makeRecord(const CallDetail& callDetail) const
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

std::vector<CallDetail>& CallCenter::getCDHolderByEndingStatus(CallEndingStatus callEndingStatus)
{
    switch (callEndingStatus)
    {
    case CallEndingStatus::OK: return activeCalls;
    case CallEndingStatus::TIMEOUT: return awaitingCalls;
    default: throw;
    }
}
