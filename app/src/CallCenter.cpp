#include "CallCenter.h"
#include "config/Configuration.h"
#include "utils/Exceptions.h"

CallCenter::CallCenter()
    : queueSize(Configuration::get<CallCenterConfig>().getQueueSize()),
      freeOperatorId(1),
      callCenterLogger(Log::Logger::getInstance(LOG4CPLUS_TEXT("CallHandlingLogger"))),
      CDRLogger(Log::Logger::getInstance(LOG4CPLUS_TEXT("CDRLogger")))
{
}

void CallCenter::registerCall(IdType& callId, const std::string& phone, Date date)
{
    CallDetail callDetail(phone);

    if (isQueueFull())
    {
        LOG4CPLUS_WARN(callCenterLogger, "Call Center: call was rejected. Queue is full");
        callDetail.recordEnding(CallEndingStatus::OVERLOAD, date);
        makeCallDetailRecord(callDetail);

        throw CCenter::Overload("Call was rejected. Queue is full");
    }

    if (IsRegistred(phone))
    {
        LOG4CPLUS_WARN(callCenterLogger, "Call Center: call was rejected. You're already registered!");
        throw CCenter::AlreadyInQueue("Call was rejected. You're already registered!");
    }

    LOG4CPLUS_INFO(callCenterLogger, "Call Center: new call was accepted for registration");

    callDetail.recordReceiption(date);
    callId = callDetail.getId();
    addCallToQueue(std::move(callDetail));
    LOG4CPLUS_INFO(callCenterLogger, "Call Center: Call[" << callId << "]: call was added to queue");

    tryToAcceptCall();
}

void CallCenter::responseCall(IdType callId, IdType operatorId, Date date)
{
    LOG4CPLUS_INFO(callCenterLogger,
                   "Call Center: Call[" << callId << "]: call was accepted by Operator[" << operatorId << "]");

    if (!IsRegistred(callId))
    {
        LOG4CPLUS_ERROR(callCenterLogger, "Call Center: Call[" << callId << "]: call with this id doesn't exist!");
        return;
    }

    CallDetail& callDetail = getCallDetail(callId);
    callDetail.recordResponse(operatorId, date);

    //onResponseCallSignature(callId);
}

void CallCenter::endCall(IdType callId, CallEndingStatus callEndingStatus, Date date)
{
    LOG4CPLUS_INFO(callCenterLogger, "Call Center: Call[" << callId << "]: call was added for ending");

    if (!IsRegistred(callId))
    {
        LOG4CPLUS_ERROR(callCenterLogger, "Call Center: Call[" << callId << "]: call with this id doesn't exist!");
        return;
    }

    CallDetail& callDetail = getCallDetail(callId);
    callDetail.recordEnding(callEndingStatus, date);
    makeCallDetailRecord(callDetail);

    releaseOperator(callDetail.getOperatorId(), callId);
    LOG4CPLUS_INFO(callCenterLogger, "Call Center: Call[" << callId << "]: call was ended");

    tryToAcceptCall();
}

void CallCenter::tryToAcceptCall()
{
    LOG4CPLUS_INFO(callCenterLogger, "Call Center: Trying to accept call...");
    std::unique_lock callCenterLock(callCenterMutex);

    if (availableOperators.empty() || awaitingCalls.empty())
    {
        LOG4CPLUS_INFO(callCenterLogger, "Call Center: Accepting call failed. Awaiting calls: "
                                             << awaitingCalls.size()
                                             << ", available operators: " << availableOperators.size());
        return;
    }

    const IdType operatorId = availableOperators.front();
    const IdType callId = awaitingCalls.front();

    availableOperators.pop_front();
    awaitingCalls.pop_front();

    Operator& availableOperator = operators.at(operatorId);
    callCenterLock.unlock();

    availableOperator.acceptCall(callId);
    LOG4CPLUS_INFO(callCenterLogger,
                   "Call Center: Call[" << callId << "]: call was accepted by operator[" << operatorId << "]");
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

void CallCenter::addCallToQueue(CallDetail&& callDetail)
{
    const IdType callId = callDetail.getId();
    std::lock_guard callCenterLock(callCenterMutex);

    awaitingCalls.push_back(callId);
    calls.emplace(callId, std::move(callDetail));

    // onRegisterCallSignature(callId, phone);
}

void CallCenter::releaseOperator(IdType operatorId, IdType callId)
{
    std::lock_guard callCenterLock(callCenterMutex);

    availableOperators.push_back(operatorId);
    calls.erase(callId);

    //onEndCallSignature(callId);
}

void CallCenter::makeCallDetailRecord(const CallDetail& callDetail) const
{
    LOG4CPLUS_INFO(CDRLogger, callDetail.toString());
}

bool CallCenter::isQueueFull() const
{
    std::lock_guard callCenterLock(callCenterMutex);
    return awaitingCalls.size() == queueSize;
}

bool CallCenter::IsRegistred(const std::string& phone) const
{
    std::lock_guard callCenterLock(callCenterMutex);
    return std::ranges::any_of(calls, [&phone](const auto& pair) { return pair.second.getPhone() == phone; });
}

bool CallCenter::IsRegistred(IdType callId) const
{
    std::lock_guard callCenterLock(callCenterMutex);
    return calls.contains(callId);
}

CallDetail& CallCenter::getCallDetail(IdType callId)
{
    return calls.at(callId);
}