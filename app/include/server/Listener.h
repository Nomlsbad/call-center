#ifndef LISTENER_H
#define LISTENER_H

#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <memory>

#include "Types.h"

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

};

#endif // LISTENER_H