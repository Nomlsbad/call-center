#ifndef LISTENER_H
#define LISTENER_H

#include "Types.h"

#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <log4cplus/log4cplus.h>
#include <memory>

namespace Log = log4cplus;

namespace beast = boost::beast;
namespace net = boost::asio;

class Listener : public std::enable_shared_from_this<Listener>
{
public:

    Listener(net::io_context& ioContext, tcp::endpoint endpoint);

    void run();

private:

    void doAccept();
    void onAccept(beast::error_code ec, tcp::socket socket);

private:

    net::io_context& ioContext;
    tcp::acceptor acceptor;

    Log::Logger serverLogger;
};

#endif // LISTENER_H