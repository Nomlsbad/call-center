#include <fstream>

#include "CallCenter.h"
#include "utils/Exceptions.h"

#include <config/CallCenterConfig.h>

CallCenter::CallCenter(const CallCenterConfig& config)
    : queueSize(config.getQueueSize()),
      freeOperatorId(1),
      callCenterLogger(Log::Logger::getInstance(LOG4CPLUS_TEXT("CallHandlingLogger"))),
      CDRLogger(Log::Logger::getInstance(LOG4CPLUS_TEXT("CDRLogger")))
{
    for (size_t i = 0; i < config.getOperators(); ++i)
    {
        connectOperator();
    }
}

void CallCenter::registerCall(IdType& callId, const std::string& phone, Date date)
{
    std::lock_guard callCenterLock(callCenterMutex);

    const bool alreadyInQueue =
        std::ranges::any_of(calls, [&phone](const auto& pair) { return pair.second.getPhone() == phone; });
    if (alreadyInQueue)
    {
        LOG4CPLUS_WARN(callCenterLogger, "Call Center: call was rejected. You're already in queue!");
        throw CCenter::AlreadyInQueue("Call was rejected. You're already in queue!");
    }

    CallDetail callDetail(phone);

    if (isQueueFull())
    {
        LOG4CPLUS_WARN(callCenterLogger, "Call Center: call was rejected. Queue is full");
        callDetail.recordEnding(CallEndingStatus::OVERLOAD, date);
        makeCallDetailRecord(callDetail);

        throw CCenter::Overload("Call was rejected. Queue is full");
    }

    callDetail.recordReceiption(date);
    callId = callDetail.getId();
    LOG4CPLUS_INFO(callCenterLogger, "Call Center: Call[" << callId << "]: call was accepted for registration");

    onRegisterCallSignature(callId, phone);

    awaitingCalls.push_back(callId);
    calls.emplace(callId, std::move(callDetail));
    LOG4CPLUS_INFO(callCenterLogger, "Call Center: Call[" << callId << "]: call was added to queue");

    tryToAcceptCall();
}

void CallCenter::responseCall(IdType callId, IdType operatorId, Date date)
{
    LOG4CPLUS_INFO(callCenterLogger,
                   "Call Center: Call[" << callId << "]: call was accepted by Operator[" << operatorId << "]");
    std::lock_guard callCenterLock(callCenterMutex);

    if (!calls.contains(callId))
    {
        LOG4CPLUS_ERROR(callCenterLogger, "Call Center: Call[" << callId << "]: call with this id doesn't exist!");
        return;
    }

    CallDetail& callDetail = calls.at(callId);
    callDetail.recordResponse(operatorId, date);

    onResponseCallSignature(callId);
}

void CallCenter::endCall(IdType callId, CallEndingStatus callEndingStatus, Date date)
{
    LOG4CPLUS_INFO(callCenterLogger, "Call Center: Call[" << callId << "]: call was added for ending");
    std::lock_guard callCenterLock(callCenterMutex);

    if (!calls.contains(callId))
    {
        LOG4CPLUS_ERROR(callCenterLogger, "Call Center: Call[" << callId << "]: call with this id doesn't exist!");
        return;
    }

    CallDetail& callDetail = calls.at(callId);
    callDetail.recordEnding(callEndingStatus, date);
    makeCallDetailRecord(callDetail);

    availableOperators.push_back(callDetail.getOperatorId());
    calls.erase(callId);
    LOG4CPLUS_INFO(callCenterLogger, "Call Center: Call[" << callId << "]: call was ended");

    onEndCallSignature(callId);
    tryToAcceptCall();
}

void CallCenter::tryToAcceptCall()
{
    LOG4CPLUS_INFO(callCenterLogger, "Call Center: Trying to accept call...");
    if (availableOperators.empty() || awaitingCalls.empty())
    {
        LOG4CPLUS_INFO(callCenterLogger, "Call Center: Accepting call failed. Awaiting calls: "
                                             << awaitingCalls.size()
                                             << ", available operators: " << availableOperators.size());
        return;
    }

    const IdType operatorId = availableOperators.front();
    const IdType callId = awaitingCalls.front();

    Operator& availableOperator = operators.at(operatorId);
    availableOperator.acceptCall(callId);
    LOG4CPLUS_INFO(callCenterLogger,
                   "Call Center: Call[" << callId << "]: call was accepted by operator[" << operatorId << "]");

    availableOperators.pop_front();
    awaitingCalls.pop_front();
}

void CallCenter::connectOperator()
{
    Operator newOperator;
    const IdType operatorId = freeOperatorId++;

    newOperator.connect(weak_from_this(), operatorId);

    std::lock_guard lock(callCenterMutex);
    operators.emplace(operatorId, std::move(newOperator));
    availableOperators.push_back(operatorId);
}

void CallCenter::makeCallDetailRecord(const CallDetail& callDetail) const
{
    LOG4CPLUS_INFO(CDRLogger, callDetail.toString());
}

bool CallCenter::isQueueFull() const
{
    return awaitingCalls.size() == queueSize;
}