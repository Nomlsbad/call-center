#ifndef LISTENER_H
#define LISTENER_H

#include "utils/Types.h"

#include <log4cplus/logger.h>
#include <memory>

namespace Log = log4cplus;

class IController;

class Listener : public std::enable_shared_from_this<Listener>
{
public:

    Listener(net::io_context& ioContext, tcp::endpoint endpoint, std::weak_ptr<IController> controller);

    void run();

    ~Listener() = default;

private:

    void doAccept();
    void onAccept(beast::error_code ec, tcp::socket socket);

private:

    net::io_context& ioContext;
    tcp::acceptor acceptor;

    std::weak_ptr<IController> controller;

    Log::Logger serverLogger;
};

#endif // LISTENER_H