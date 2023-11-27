#ifndef CALLCENTER_H
#define CALLCENTER_H

#include <deque>
#include <map>

#include <log4cplus/log4cplus.h>

#include "CallDetail.h"
#include "Operator.h"


namespace Log = log4cplus;


class CallCenter
{
public:

    CallCenter(size_t queueSize, size_t operatorsSize);

    CallCenter(const CallCenter& callCenter) = delete;
    CallCenter(CallCenter&& callCenter) = delete;

    CallCenter& operator=(const CallCenter& callCenter) = delete;
    CallCenter& operator=(CallCenter&& callCenter) = delete;

public:

    void registerCall(const std::string& phone, Date date);
    void endCall(IdType callId, CallEndingStatus callEndingStatus, Date date);

private:

    [[nodiscard]] bool isQueueFull() const;

    void tryToAcceptCall();

    void makeCallDetailRecord(const CallDetail& callDetail) const;

private:

    std::string journalPath = "../../resurses/journal.txt";

    size_t queueSize = 0;

    std::deque<IdType> awaitingCalls;
    std::map<IdType, CallDetail> calls;

    std::deque<IdType> availableOperators;
    std::map<IdType, Operator> operators;

    mutable std::mutex callCenterMutex;

    Log::Logger callCenterLogger;
};

#endif // CALLCENTER_H
