#include "CallCenter.h"

IdType Operator::nextId = 1;

Operator::Operator()
    : id(nextId++),
      operatorLogger(Log::Logger::getInstance(LOG4CPLUS_TEXT("OperatorLogger"))){};

void Operator::acceptCall(IdType callId)
{
    if (!isConnected())
    {
        LOG4CPLUS_WARN(operatorLogger,
                       "Operator: Operator[" << id << "]: this operator doesn't connect to call center");
        return;
    }

    LOG4CPLUS_WARN(operatorLogger, "Operator: Operator[" << id << "]: operator was accepted call");
    callThread = std::thread(&Operator::talk, this, callId);
    callThread.detach();
}

void Operator::talk(IdType callId) const
{
    if (!isConnected()) return;
    LOG4CPLUS_WARN(operatorLogger, "Operator: Operator[" << id << "]: talking was began");

    callCenter->responseCall(callId, id, boost::posix_time::microsec_clock::local_time());
    std::this_thread::sleep_for(std::chrono::seconds(5));
    callCenter->endCall(callId, CallEndingStatus::OK, boost::posix_time::microsec_clock::local_time());


    LOG4CPLUS_WARN(operatorLogger, "Operator: Operator[" << id << "]: talking was ended");
}