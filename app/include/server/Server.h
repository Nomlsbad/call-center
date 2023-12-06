#ifndef SERVER_H
#define SERVER_H

#include "Types.h"

#include <boost/asio/ip/address.hpp>
#include <log4cplus/logger.h>

namespace Log = log4cplus;

class CallCenter;
class IController;
class Listener;
class ServerConfig;

class Server
{
public:

    explicit Server();
    ~Server() = default;

    void run();

private:

    net::ip::address address;
    unsigned short port;
    int threads;

    std::shared_ptr<CallCenter> callCenter;
    std::shared_ptr<IController> controller;

    net::io_context ioContext;
    std::shared_ptr<Listener> listener;

    Log::Logger serverLogger;
};

#endif // SERVER_H
