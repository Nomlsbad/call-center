#include "CallCenter.h"

IdType Operator::nextId = 1;

void Operator::acceptCall(IdType callId)
{
    if (!isConnected()) return;;

    callThread = std::thread(&Operator::talk, this, callId);
    callThread.detach();
}

void Operator::talk(IdType callId) const
{
    if (!isConnected()) return;

    callCenter->responseCall(callId, id, boost::posix_time::microsec_clock::local_time());
    std::this_thread::sleep_for(std::chrono::seconds(5));
    callCenter->endCall(callId, CallEndingStatus::OK, boost::posix_time::microsec_clock::local_time());
}