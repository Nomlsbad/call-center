#include "Operator.h"
#include "CallDetailRecord.h"

Operator::IdType Operator::nextId = 1;

void Operator::acceptCall(CallDetailRecord& callDetail)
{
    isBusy = true;
    callDetail.recordCallResponse(id);

    callThread = std::thread(&Operator::talk, this);
    callThread.detach();
}

void Operator::talk()
{
    std::this_thread::sleep_for(std::chrono::seconds(5));

    isBusy = false;
}