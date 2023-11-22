#include <fstream>

#include "CallCenter.h"


CallCenter::CallCenter(size_t queueSize, size_t operatorsSize)
    : queueSize(queueSize), activeCalls(0), operators(operatorsSize)
{}

void CallCenter::registerCall(const std::string& phone)
{
    auto callDetail = std::make_unique<CallDetail>(phone);
    callDetail->recordReceiption();

    if (!isQueueFull())
    {
        calls.push(std::move(callDetail));
    }
    else
    {
        callDetail->recordEnding(CallEndingStatus::OVERLOAD);
        makeRecord(*callDetail);
    }
}

void CallCenter::endCall(IdType callId, CallEndingStatus callEndingStatus)
{
    auto isIdEquals = [id = callId](const auto& callDetail) { return callDetail->getId() == id; };
    const auto callDetailIter = std::ranges::find_if(activeCalls, isIdEquals);
    const auto callDetail = std::move(*callDetailIter);

    callDetail->recordEnding(callEndingStatus);
    makeRecord(*callDetail);

    activeCalls.erase(callDetailIter);
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
    return calls.size() == queueSize;
}
