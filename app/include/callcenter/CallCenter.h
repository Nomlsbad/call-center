#ifndef CALLCENTER_H
#define CALLCENTER_H

#include <deque>
#include <map>

#include <log4cplus/log4cplus.h>

#include "CallDetail.h"
#include "models/Operator.h"

class UserSimulation;

namespace Log = log4cplus;


class CallCenter : public std::enable_shared_from_this<CallCenter>
{
public:

    CallCenter();

    CallCenter(const CallCenter& callCenter) = delete;
    CallCenter(CallCenter&& callCenter) = delete;

    CallCenter& operator=(const CallCenter& callCenter) = delete;
    CallCenter& operator=(CallCenter&& callCenter) = delete;

public:

    void run();

    void registerCall(IdType& callId, const std::string& phone, Date date);
    void responseCall(IdType callId, IdType operatorId, Date date);
    void endCall(IdType callId, CallEndingStatus callEndingStatus, Date date);

    void connectOperator();

private:

    [[nodiscard]] bool isQueueFull() const;
    [[nodiscard]] bool IsRegistred(const std::string& phone) const;

    void tryToAcceptCall();

    void makeCallDetailRecord(const CallDetail& callDetail) const;

private:

    IdType freeCallId;
    size_t queueSize;
    std::deque<IdType> awaitingCalls;
    std::map<IdType, CallDetail> calls;

    IdType freeOperatorId;
    std::deque<IdType> availableOperators;
    std::map<IdType, Operator> operators;

    std::mutex callCenterMutex;

    Log::Logger callCenterLogger;
    Log::Logger CDRLogger;

    std::shared_ptr<UserSimulation> simulation;
};

#endif // CALLCENTER_H
