#include "CallCenter.h"

#include "models/Abonent.h"
#include "utils/AbonentSimulation.h"

AbonentSimulation::AbonentSimulation(std::shared_ptr<CallCenter> center)
    : callCenter(std::move(center))
{
    callCenter->onRegisterCallSignature = [this](IdType callId, std::string phone){ this->onRegisterCall(callId, std::move(phone)); };
    callCenter->onResponseCallSignature = [this](IdType callId){ this->onResponse(callId); };
    callCenter->onEndCallSignature = [this](IdType callId){ this->onEndCall(callId); };
}

void AbonentSimulation::onRegisterCall(IdType callId, std::string phone)
{
    const auto abonent = abonents.emplace(callId, Abonent(callId, std::move(phone), callCenter));
    abonent.first->second.wait();
}

void AbonentSimulation::onResponse(IdType callId)
{
    Abonent& abonent = abonents.at(callId);
    abonent.response();
}

void AbonentSimulation::onEndCall(IdType callId)
{
    abonents.erase(callId);
}