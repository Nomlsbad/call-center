#include "callcenter/CallCenter.h"

Operator::Operator()
    : operatorLogger(Log::Logger::getInstance(LOG4CPLUS_TEXT("OperatorLogger")))
{
}

void Operator::connect(std::weak_ptr<CallCenter> center, IdType operatorId)
{
    callCenter = std::move(center);
    id = operatorId;
    LOG4CPLUS_INFO(operatorLogger, "Operator: operator[" << id << "]: connected to call center");
}

void Operator::acceptCall(IdType callId)
{
    const std::shared_ptr<CallCenter> center = callCenter.lock();
    if (!center)
    {
        LOG4CPLUS_WARN(operatorLogger, "Operator: operator[" << id << "] doesn't connect to call center");
        return;
    }

    acceptedCallId = callId;
    isBusy = false;

    try
    {
        center->responseCall(callId, id, boost::posix_time::microsec_clock::local_time());
    }
    catch (const std::out_of_range& e)
    {
        LOG4CPLUS_WARN(operatorLogger, "Operator: Call[" << callId << "] has already droped");
    }

    LOG4CPLUS_INFO(operatorLogger, "Operator: operator[" << id << "] accepted the call[" << callId << "]");
}

void Operator::onEndCall()
{
    isBusy = true;
}

IdType Operator::getId() const
{
    return id;
}

bool Operator::isConnected() const
{
    return !callCenter.expired();
}

bool Operator::wasBusy() const
{
    return isBusy;
}

IdType Operator::getLastCallId() const
{
    return acceptedCallId;
}