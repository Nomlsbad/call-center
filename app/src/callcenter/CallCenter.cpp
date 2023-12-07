#include "callcenter/CallCenter.h"
#include "callcenter/UserSimulation.h"
#include "config/Configuration.h"
#include "utils/Exceptions.h"

CallCenter::CallCenter()
    : freeCallId(1),
      queueSize(Configuration::get<CallCenterConfig>("queueSize")),
      freeOperatorId(1),
      callCenterLogger(Log::Logger::getInstance(LOG4CPLUS_TEXT("CallHandlingLogger"))),
      CDRLogger(Log::Logger::getInstance(LOG4CPLUS_TEXT("CDRLogger")))
{
}

void CallCenter::run(std::shared_ptr<UserSimulation> userSimulation)
{
    simulation = std::move(userSimulation);
    std::weak_ptr weakCenter = shared_from_this();

    std::thread queueObserver(
        [weakCenter]()
        {
            while (auto center = weakCenter.lock())
            {
                center->tryToAcceptCall();
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }
        });
    queueObserver.detach();
    LOG4CPLUS_INFO(callCenterLogger, "Call Center: call center was run");
}

void CallCenter::registerCall(IdType& callId, const std::string& phone, Date date)
{
    CallDetail callDetail(phone);
    callId = std::max<IdType>(1, freeCallId++);
    callDetail.recordReceiption(callId, date);
    std::lock_guard callCenterLock(callCenterMutex);

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

    callId = callDetail.getId();
    awaitingCalls.push_back(callId);
    calls.emplace(callId, std::move(callDetail));
    LOG4CPLUS_INFO(callCenterLogger, "Call Center: Call[" << callId << "]: call was added to queue");

    if (!simulation) return;
    simulation->onRegisterCall(callId, phone);
}

void CallCenter::responseCall(IdType callId, IdType operatorId, Date date)
{
    LOG4CPLUS_INFO(callCenterLogger,
                   "Call Center: Call[" << callId << "]: call was accepted by Operator[" << operatorId << "]");
    std::lock_guard callCenterLock(callCenterMutex);

    CallDetail& callDetail = calls.at(callId);
    if (callDetail.getOperatorId() != 0) throw CCenter::DoubleResponse("Double response");
    callDetail.recordResponse(operatorId, date);

    if (!simulation) return;
    simulation->onResponse(callId);
}

void CallCenter::endCall(IdType callId, CallEndingStatus callEndingStatus, Date date)
{
    LOG4CPLUS_INFO(callCenterLogger, "Call Center: Call[" << callId << "]: call was added for ending");
    std::lock_guard callCenterLock(callCenterMutex);

    CallDetail& callDetail = calls.at(callId);
    callDetail.recordEnding(callEndingStatus, date);
    makeCallDetailRecord(callDetail);

    const IdType operatorId = callDetail.getOperatorId();
    if (operatorId != 0)
    {
        Operator& phoneOperator = operators.at(operatorId);
        phoneOperator.onEndCall();

        availableOperators.push_back(operatorId);
    }

    calls.erase(callId);
    LOG4CPLUS_INFO(callCenterLogger, "Call Center: Call[" << callId << "]: call was ended");

    if (!simulation) return;
    simulation->onEndCall(callId);
}

void CallCenter::tryToAcceptCall()
{
    std::unique_lock callCenterLock(callCenterMutex);

    if (availableOperators.empty() || awaitingCalls.empty()) return;

    const IdType operatorId = availableOperators.front();
    const IdType callId = awaitingCalls.front();

    availableOperators.pop_front();
    awaitingCalls.pop_front();

    Operator& availableOperator = operators.at(operatorId);
    callCenterLock.unlock();

    availableOperator.acceptCall(callId);
}

void CallCenter::connectOperator()
{
    Operator newOperator;
    const IdType operatorId = std::max<IdType>(1, freeOperatorId++);

    newOperator.connect(weak_from_this(), operatorId);
    LOG4CPLUS_INFO(callCenterLogger, "Call Center: Operator[" << operatorId << "]: connected");

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

bool CallCenter::IsRegistred(const std::string& phone) const
{
    return std::ranges::any_of(calls, [&phone](const auto& pair) { return pair.second.getPhone() == phone; });
}