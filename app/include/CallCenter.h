#ifndef CALLCENTER_H
#define CALLCENTER_H

#include <queue>
#include <vector>
#include <memory>

#include "CallDetail.h"
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
    void endCall(IdType callId, CallEndingStatus callEndingStatus);

private:

    void makeRecord(const CallDetail& callDetail) const;

    [[nodiscard]] bool isQueueFull() const;

private:

    std::string journalPath = "../../resurses/journal.txt";

    size_t queueSize = 0;

    std::queue<std::unique_ptr<CallDetail>> calls;

    std::vector<std::unique_ptr<CallDetail>> activeCalls;

    std::queue<Operator*> freeOperators;

    std::vector<Operator> operators;
};


#endif //CALLCENTER_H