#ifndef SEREVER_H
#define SEREVER_H

#include "Types.h"

#include <boost/asio/ip/address.hpp>
#include <log4cplus/logger.h>

namespace Log = log4cplus;

class CallController;
class Listener;

class Serever
{
public:

    Serever();
    ~Serever() = default;

    void run();

private:

    net::ip::address address = net::ip::make_address("127.0.0.1");
    unsigned short port = 1234;
    int threads = 2;

    std::shared_ptr<CallController> controller;

    net::io_context ioContext;
    std::shared_ptr<Listener> listener;

    Log::Logger serverLogger;
};

#endif // SEREVER_H
