#ifndef USERSIMULATION_H
#define USERSIMULATION_H

#include "utils/Types.h"
#include "models/User.h"

namespace Log = log4cplus;

class CallCenter;

class UserSimulation
{
public:

    explicit UserSimulation(std::weak_ptr<CallCenter> callCenter);

    void onRegisterCall(IdType callId, std::string phone);
    void onResponse(IdType callId);
    void onEndCall(IdType callId);

private:

    static TimeDuration getRandomDuration(const TimeDuration& min, const TimeDuration& max);
    TimeDuration getWaitingDuration() const;
    TimeDuration getTalkingDuration() const;

    std::map<IdType, User> users;

    TimeDuration minWaitingTime;
    TimeDuration maxWaitingTime;
    TimeDuration minTalkingTime;
    TimeDuration maxTalkingTime;

    std::weak_ptr<CallCenter> callCenter;

    Log::Logger userLogger;
};

#endif // USERSIMULATION_H
