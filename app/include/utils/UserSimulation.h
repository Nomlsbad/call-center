#ifndef USERSIMULATION_H
#define USERSIMULATION_H

#include "Types.h"

class User;
class CallCenter;

class UserSimulation
{
public:

    explicit UserSimulation(std::shared_ptr<CallCenter> callCenter);

    void onRegisterCall(IdType callId, std::string phone);
    void onResponse(IdType callId);
    void onEndCall(IdType callId);

private:

    std::map<IdType, User> users;

    std::shared_ptr<CallCenter> callCenter;
};

#endif // USERSIMULATION_H
