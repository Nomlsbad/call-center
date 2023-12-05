#ifndef CALLCENTER_H
#define CALLCENTER_H

#include <deque>
#include <map>

#include <log4cplus/log4cplus.h>

#include "CallDetail.h"
#include "models/Operator.h"

class CallCenterConfig;

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

    void registerCall(IdType& callId, const std::string& phone, Date date);
    void responseCall(IdType callId, IdType operatorId, Date date);
    void endCall(IdType callId, CallEndingStatus callEndingStatus, Date date);

    void connectOperator();

public:

    std::function<void(IdType, std::string)> onRegisterCallSignature;
    std::function<void(IdType)> onResponseCallSignature;
    std::function<void(IdType)> onEndCallSignature;

private:

    [[nodiscard]] bool isQueueFull() const;
    [[nodiscard]] bool IsRegistred(const std::string& phone) const;
    [[nodiscard]] bool IsRegistred(IdType callId) const;
    [[nodiscard]] CallDetail& getCallDetail(IdType callId);

    void addCallToQueue(CallDetail&& callDetail);
    void releaseOperator(IdType operatorId, IdType callId);

    void tryToAcceptCall();

    void makeCallDetailRecord(const CallDetail& callDetail) const;

private:

    size_t queueSize;
    std::deque<IdType> awaitingCalls;
    std::map<IdType, CallDetail> calls;

    IdType freeOperatorId;
    std::deque<IdType> availableOperators;
    std::map<IdType, Operator> operators;

    mutable std::mutex callCenterMutex;

    Log::Logger callCenterLogger;
    Log::Logger CDRLogger;
};

#endif // CALLCENTER_H
