#include "server/Serever.h"
#include "controller/AbonentController.h"
#include "server/Listener.h"

#include <log4cplus/loggingmacros.h>

Serever::Serever()
    : controller(std::make_shared<AbonentController>()),
      ioContext(threads),
      listener(std::make_shared<Listener>(ioContext, tcp::endpoint{address, port}, controller)),
      serverLogger(Log::Logger::getInstance(LOG4CPLUS_TEXT("ServerLogger")))
{
}

void Serever::run()
{
    LOG4CPLUS_INFO(serverLogger, "Server: server was run");
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