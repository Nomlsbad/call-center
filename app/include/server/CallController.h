#ifndef CALLCONTROLLER_H
#define CALLCONTROLLER_H

#include "Types.h"

#include <functional>
#include <string>
#include <unordered_map>

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

    const std::unordered_map<std::string, const HandleRequest> enpoindsMap;

    std::shared_ptr<class CallCenter> callCenter;
};

#endif // CALLCONTROLLER_H
