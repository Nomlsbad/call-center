#ifndef ABONENTSIMULATION_H
#define ABONENTSIMULATION_H

#include "Types.h"

class AbonentController;
class Abonent;

class AbonentSimulation
{
public:

    explicit AbonentSimulation(std::weak_ptr<const AbonentController> controller);

    void addAbonent(std::string phone, IdType callId);
    void removeAbonent(IdType callId);

private:

    std::map<IdType, Abonent> abonents;

    std::weak_ptr<const AbonentController> controller;

    mutable std::mutex mutex;
};

#endif // ABONENTSIMULATION_H
