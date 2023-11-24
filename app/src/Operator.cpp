#include "Operator.h"
#include "CallDetail.h"

Operator::IdType Operator::nextId = 1;

void Operator::acceptCall(CallDetail& callDetail)
{
    isBusy = true;
    callDetail.recordResponse(id, boost::posix_time::microsec_clock::local_time());

    callThread = std::thread(&Operator::talk, this);
    callThread.detach();
}

void Operator::talk()
{
    std::this_thread::sleep_for(std::chrono::seconds(5));

    isBusy = false;
}