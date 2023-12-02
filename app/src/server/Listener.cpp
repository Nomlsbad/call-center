#include "server/Listener.h"
#include "server/CallController.h"
#include "server/HttpSession.h"

#include <log4cplus/loggingmacros.h>

Listener::Listener(net::io_context& ioContext, tcp::endpoint endpoint, std::weak_ptr<CallController> controller)
    : ioContext(ioContext),
      acceptor(net::make_strand(ioContext)),
      controller(std::move(controller)),
      serverLogger(Log::Logger::getInstance(LOG4CPLUS_TEXT("ServerLogger")))
{
    try
    {
        acceptor.open(endpoint.protocol());
        acceptor.set_option(net::socket_base::reuse_address(true));
        acceptor.bind(endpoint);
        acceptor.listen(net::socket_base::max_listen_connections);
    }
    catch (const boost::system::system_error& e)
    {
        LOG4CPLUS_FATAL(serverLogger, "Listener: " << e.what());
        return;
    }
}

void Listener::run()
{
    LOG4CPLUS_INFO(serverLogger, "Listener: listner was run");
    net::dispatch(acceptor.get_executor(), beast::bind_front_handler(&Listener::doAccept, shared_from_this()));
}

void Listener::doAccept()
{
    acceptor.async_accept(net::make_strand(ioContext),
                          beast::bind_front_handler(&Listener::onAccept, shared_from_this()));
}

void Listener::onAccept(beast::error_code ec, tcp::socket socket)
{
    if (ec)
    {
        LOG4CPLUS_ERROR(serverLogger, "Listener: " << ec.message());
        return;
    }
    else
    {
        LOG4CPLUS_INFO(serverLogger, "Listener: HttpSession was created");
        std::make_shared<HttpSession>(std::move(socket), controller)->run();
    }

    doAccept();
}