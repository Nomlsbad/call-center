#ifndef ICONTROLLER_H
#define ICONTROLLER_H

#include "Types.h"

class IController
{
public:

    using RequestType = http::request<http::string_body>;
    using ResponceType = http::message_generator;

public:

    IController() = default;
    virtual ~IController() = default;

public:

    virtual ResponceType handleRequest(RequestType&& req) const = 0;

};

#endif // ICONTROLLER_H
