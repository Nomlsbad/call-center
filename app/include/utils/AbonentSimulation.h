#ifndef ABONENTSIMULATION_H
#define ABONENTSIMULATION_H

#include "Types.h"

class Abonent;
class CallCenter;

class AbonentSimulation
{
public:

    explicit AbonentSimulation(std::shared_ptr<CallCenter> callCenter);

    void onRegisterCall(IdType callId, std::string phone);
    void onResponse(IdType callId);
    void onEndCall(IdType callId);

private:

    std::map<IdType, Abonent> abonents;

    std::shared_ptr<CallCenter> callCenter;
};

#endif // ABONENTSIMULATION_H
