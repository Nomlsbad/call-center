#ifndef USERCONTROLLER_H
#define USERCONTROLLER_H

#include "controller/IController.h"

#include <log4cplus/logger.h>

namespace Log = log4cplus;

class CallCenter;

class UserController : public IController
{
public:

    using RequestHandler = std::function<ResponceType(RequestType&&)>;

public:

    explicit UserController(std::weak_ptr<CallCenter> callCenter);
    ~UserController() override = default;

    ResponceType handleRequest(RequestType&& req) const override;

    ResponceType registerCall(RequestType&& req) const;
    ResponceType endCall(RequestType&& req) const;

private:

    std::weak_ptr<CallCenter> callCenter;

    Log::Logger controllerLogger;

    const std::unordered_map<std::string, const RequestHandler> enpoindsMap;
};

#endif // USERCONTROLLER_H
