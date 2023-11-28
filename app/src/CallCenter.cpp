#include <fstream>

#include "CallCenter.h"

CallCenter::CallCenter(size_t queueSize, size_t operatorsSize)
    : queueSize(queueSize),
      availableOperators(operatorsSize),
      callCenterLogger(Log::Logger::getInstance(LOG4CPLUS_TEXT("CallHandlingLogger")))
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
    LOG4CPLUS_INFO(callCenterLogger,
                   "Call Center: Call[" << callDetail.getId() << "]: call was accepted for registration");

    std::lock_guard callCenterLock(callCenterMutex);

    if (isQueueFull())
    {
        LOG4CPLUS_INFO(callCenterLogger,
                       "Call Center: Call[" << callDetail.getId() << "]: call was rejected. Queue is full");
        callDetail.recordEnding(CallEndingStatus::OVERLOAD, date);
        makeCallDetailRecord(callDetail);
        return;
    }

    awaitingCalls.push_back(callDetail.getId());
    calls.emplace(callDetail.getId(), std::move(callDetail));
    LOG4CPLUS_INFO(callCenterLogger, "Call Center: Call[" << callDetail.getId() << "]: call was added to queue");

    tryToAcceptCall();
}

void CallCenter::responseCall(IdType callId, IdType operatorId, Date date)
{
    LOG4CPLUS_INFO(callCenterLogger,
                   "Call Center: Call[" << callId << "]: call was accepted by Operator[" << operatorId << "]");
    std::lock_guard callCenterLock(callCenterMutex);

    if (!calls.contains(callId))
    {
        LOG4CPLUS_WARN(callCenterLogger, "Call Center: Call[" << callId << "]: call with this id doesn't exist!");
        return;
    }

    CallDetail& callDetail = calls.at(callId);
    callDetail.recordResponse(operatorId, date);
}

void CallCenter::endCall(IdType callId, CallEndingStatus callEndingStatus, Date date)
{
    LOG4CPLUS_INFO(callCenterLogger, "Call Center: Call[" << callId << "]: call was added for ending");
    std::lock_guard callCenterLock(callCenterMutex);

    if (!calls.contains(callId))
    {
        LOG4CPLUS_WARN(callCenterLogger, "Call Center: Call[" << callId << "]: call with this id doesn't exist!");
        return;
    }

    CallDetail& callDetail = calls.at(callId);
    callDetail.recordEnding(callEndingStatus, date);
    makeCallDetailRecord(callDetail);

    availableOperators.push_back(callDetail.getOperatorId());
    calls.erase(callId);
    LOG4CPLUS_INFO(callCenterLogger, "Call Center: Call[" << callId << "]: call was ended");

    tryToAcceptCall();
}

void CallCenter::tryToAcceptCall()
{
    if (availableOperators.empty() || awaitingCalls.empty()) return;

    const IdType operatorId = availableOperators.front();
    const IdType callId = awaitingCalls.front();

    Operator& availableOperator = operators.at(operatorId);
    availableOperator.acceptCall(callId);

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