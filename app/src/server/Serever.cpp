#include "server/Serever.h"

#include "CallCenter.h"
#include "config/Configuration.h"
#include "controller/UserController.h"
#include "server/Listener.h"

#include <log4cplus/loggingmacros.h>

Serever::Serever()
    : address(net::ip::make_address(Configuration::get<ServerConfig>().getHost())),
      port(Configuration::get<ServerConfig>().getPort()),
      threads(Configuration::get<ServerConfig>().getThreads()),
      callCenter(std::make_shared<CallCenter>()),
      controller(std::make_shared<UserController>(callCenter)),
      ioContext(threads),
      listener(std::make_shared<Listener>(ioContext, tcp::endpoint{address, port}, controller)),
      serverLogger(Log::Logger::getInstance(LOG4CPLUS_TEXT("ServerLogger")))
{
}

void Serever::run()
{
    LOG4CPLUS_INFO(serverLogger, "Server: server was run");

    const size_t operators = Configuration::get<CallCenterConfig>().getOperators();
    for (size_t i = 0; i < operators; ++i)
    {
        callCenter->connectOperator();
    }
    callCenter->run();

    listener->run();

    net::io_context& ioContextRef = ioContext;

    net::signal_set signals(ioContextRef, SIGINT, SIGTERM);
    signals.async_wait([&ioContextRef](beast::error_code const&, int) { ioContextRef.stop(); });

    std::vector<std::thread> ioThreads;
    ioThreads.reserve(static_cast<size_t>(threads - 1));
    for (int i = 0; i < threads - 1; ++i)
    {
        ioThreads.emplace_back([&ioContextRef] { ioContextRef.run(); });
    }
    ioContextRef.run();

    LOG4CPLUS_INFO(serverLogger, "Server: server was stoped by signal");
    for (auto& thread : ioThreads)
    {
        thread.join();
    }
}