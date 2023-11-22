#ifndef CALLCENTER_H
#define CALLCENTER_H

#include <queue>
#include <vector>
#include <memory>

#include "CallDetailRecord.h"
#include "Operator.h"

class CallCenter
{
public:
    CallCenter(size_t queueSize, size_t operatorsSize);

    CallCenter(const CallCenter& callCenter) = delete;
    CallCenter(CallCenter&& callCenter) = delete;

    CallCenter& operator= (const CallCenter& callCenter) = delete;
    CallCenter& operator= (CallCenter&& callCenter) = delete;

public:

    void registerCall(const std::string& phone);
    void endCall(CallDetailRecord::IdType callId, CallEndingStatus callEndingStatus);

private:

    void makeRecord(const CallDetailRecord& callDetail) const;

    [[nodiscard]] bool isQueueFull() const;

private:

    std::string journalPath = "../../resurses/journal.txt";

    size_t queueSize = 0;

    std::queue<std::unique_ptr<CallDetailRecord>> calls;

    std::vector<std::unique_ptr<CallDetailRecord>> activeCalls;

    std::queue<Operator*> freeOperators;

    std::vector<Operator> operators;
};


#endif //CALLCENTER_H
