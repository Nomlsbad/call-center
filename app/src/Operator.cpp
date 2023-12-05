#include "CallCenter.h"

Operator::Operator()
    : operatorLogger(Log::Logger::getInstance(LOG4CPLUS_TEXT("OperatorLogger")))
{
}

void Operator::connect(std::weak_ptr<CallCenter> center, IdType operatorId)
{
    callCenter = std::move(center);
    id = operatorId;
}

void Operator::acceptCall(IdType callId)
{
    if (!callCenter.expired())
    {
        LOG4CPLUS_WARN(operatorLogger,
                       "Operator: Operator[" << id << "]: this operator doesn't connect to call center");
        return;
    }

    callCenter.lock()->responseCall(callId, id, boost::posix_time::microsec_clock::local_time());
    LOG4CPLUS_INFO(operatorLogger, "Operator: Operator[" << id << "]: operator was accepted call");
}

IdType Operator::getId() const
{
    return id;
}