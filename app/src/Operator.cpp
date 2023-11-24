#include "CallCenter.h"

IdType Operator::nextId = 1;

void Operator::acceptCall(CallDetail& callDetail)
{
    if (!isConnected()) throw;

    callDetail.recordResponse(id, boost::posix_time::microsec_clock::local_time());

    callThread = std::thread(&Operator::talk, this, callDetail.getId());
    callThread.detach();
}

void Operator::talk(IdType callId) const
{
    if (!isConnected()) return;

    std::this_thread::sleep_for(std::chrono::seconds(5));
    callCenter->endCall(callId, CallEndingStatus::OK, boost::posix_time::microsec_clock::local_time());
}