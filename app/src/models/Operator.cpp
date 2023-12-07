#include "callcenter/CallCenter.h"

Operator::Operator()
    : operatorLogger(Log::Logger::getInstance(LOG4CPLUS_TEXT("OperatorLogger")))
{
}

void Operator::connect(const std::weak_ptr<CallCenter>& center)
{
    const auto sharedCenter = center.lock();
    if (!sharedCenter) return;

    id = sharedCenter->applyConnection(shared_from_this());
    if (id == 0) return;

    callCenter = center;
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

    try
    {
        center->responseCall(callId, id, boost::posix_time::microsec_clock::local_time());
        acceptedCallId = callId;
        isBusy = true;
    }
    catch (const std::out_of_range& e)
    {
        LOG4CPLUS_WARN(operatorLogger, "Operator: Call[" << callId << "] has already droped");
    }

    LOG4CPLUS_INFO(operatorLogger, "Operator: operator[" << id << "] accepted the call[" << callId << "]");
}

void Operator::onEndCall()
{
    isBusy = false;
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