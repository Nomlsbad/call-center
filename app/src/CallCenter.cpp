#include <fstream>

#include "CallCenter.h"

CallCenter::CallCenter(size_t queueSize, size_t operatorsSize)
    : queueSize(queueSize),
      availableOperators(operatorsSize)
{
    for (size_t i = 0; i < operatorsSize; ++i)
    {
        Operator newOperator;
        const IdType operatorId = newOperator.getId();

        newOperator.connectTo(this);

        availableOperators[i] = operatorId;
        operators.emplace(operatorId, std::move(newOperator));
    }
}

void CallCenter::registerCall(const std::string& phone, Date date)
{
    CallDetail callDetail(phone);
    callDetail.recordReceiption(date);
    std::lock_guard callCenterLock(callCenterMutex);

    if (isQueueFull())
    {
        callDetail.recordEnding(CallEndingStatus::OVERLOAD, date);
        makeCallDetailRecord(callDetail);
        return;
    }

    awaitingCalls.push_back(callDetail.getId());
    calls.emplace(callDetail.getId(), std::move(callDetail));
    tryToAcceptCall();
}

void CallCenter::endCall(IdType callId, CallEndingStatus callEndingStatus, Date date)
{
    std::lock_guard callCenterLock(callCenterMutex);

    if (!calls.contains(callId)) return;
    CallDetail& callDetail = calls.at(callId);

    callDetail.recordEnding(callEndingStatus, date);
    makeCallDetailRecord(callDetail);

    availableOperators.push_back(callDetail.getOperatorId());
    calls.erase(callId);
    tryToAcceptCall();
}

void CallCenter::tryToAcceptCall()
{
    if (availableOperators.empty() || awaitingCalls.empty()) return;

    const IdType operatorId = availableOperators.front();
    const IdType callId = awaitingCalls.front();

    Operator& availableOperator = operators.at(operatorId);
    CallDetail& callDetail = calls.at(callId);

    availableOperator.acceptCall(callDetail);

    availableOperators.pop_front();
    awaitingCalls.pop_front();
}

void CallCenter::makeCallDetailRecord(const CallDetail& callDetail) const
{
    std::ofstream out;
    out.open(journalPath, std::ios::app);
    out << callDetail.toString() << "\n";
    out.close();
}

bool CallCenter::isQueueFull() const
{
    return awaitingCalls.size() == queueSize;
}