#ifndef ABONENTCONTROLLER_H
#define ABONENTCONTROLLER_H

#include "controller/IController.h"

#include <log4cplus/logger.h>

namespace Log = log4cplus;

class CallCenter;

class AbonentController : public IController
{
public:

    using RequestHandler = std::function<ResponceType(RequestType&&)>;

public:

    AbonentController();
    ~AbonentController() override = default;

    ResponceType handleRequest(RequestType&& req) const override;

    ResponceType registerCall(RequestType&& req) const;
    ResponceType endCall(RequestType&& req) const;

private:

    std::shared_ptr<CallCenter> callCenter;

    Log::Logger controllerLogger;

    const std::unordered_map<std::string, const RequestHandler> enpoindsMap;
};

#endif // ABONENTCONTROLLER_H
