#include "utils/AbonentSimulation.h"
#include "models/Abonent.h"

AbonentSimulation::AbonentSimulation(std::weak_ptr<const AbonentController> controller)
    : controller(std::move(controller))
{
}

void AbonentSimulation::addAbonent(std::string phone, IdType callId)
{
    std::unique_lock lock(mutex);
    const auto abonent = abonents.emplace(callId, Abonent(std::move(phone), controller));
    lock.unlock();

    abonent.first->second.setCallId(callId);
    abonent.first->second.wait();
}

void AbonentSimulation::removeAbonent(IdType callId)
{
    abonents.erase(callId);
}