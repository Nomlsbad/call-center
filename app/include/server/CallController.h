#ifndef CALLCONTROLLER_H
#define CALLCONTROLLER_H

#include "Types.h"

#include <functional>
#include <string>
#include <unordered_map>

#include <log4cplus/logger.h>

namespace Log = log4cplus;

class CallCenter;

class CallController
{
public:

    using RequestType = http::request<http::string_body>;
    using ResponceType = http::message_generator;
    using HandleRequest = std::function<ResponceType(RequestType&&)>;

public:

    CallController();

    ResponceType handleRequest(RequestType&& req) const;

    ResponceType registerCall(RequestType&& req) const;
    ResponceType endCall(RequestType&& req) const;

private:

    std::shared_ptr<CallCenter> callCenter;

    Log::Logger controllerLogger;

    const std::unordered_map<std::string, const HandleRequest> enpoindsMap;

};

#endif // CALLCONTROLLER_H
