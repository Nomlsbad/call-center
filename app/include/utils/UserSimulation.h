#ifndef USERSIMULATION_H
#define USERSIMULATION_H

#include "Types.h"
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

    std::map<IdType, User> users;

    std::weak_ptr<CallCenter> callCenter;

    Log::Logger userLogger;
};

#endif // USERSIMULATION_H
