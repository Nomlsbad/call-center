#include "server/HttpSession.h"
#include "controller/IController.h"

#include <boost/asio/dispatch.hpp>
#include <log4cplus/loggingmacros.h>

HttpSession::HttpSession(tcp::socket&& socket, std::weak_ptr<IController> controller)
    : stream(std::move(socket)),
      controller(std::move(controller)),
      sessionLogger(Log::Logger::getInstance(LOG4CPLUS_TEXT("ServerLogger")))
{
}

void HttpSession::run()
{
    LOG4CPLUS_INFO(sessionLogger, "HttpSession: session was run");
    net::dispatch(stream.get_executor(), beast::bind_front_handler(&HttpSession::read, shared_from_this()));
}

void HttpSession::read()
{
    parser.emplace();
    stream.expires_after(std::chrono::seconds(30));

    http::async_read(stream, buffer, *parser, beast::bind_front_handler(&HttpSession::onRead, shared_from_this()));
}

void HttpSession::onRead(beast::error_code errorCode, size_t bytes_transfered)
{
    boost::ignore_unused(bytes_transfered);
    LOG4CPLUS_INFO(sessionLogger, "HttpsSession: read new request");

    if (errorCode == http::error::end_of_stream)
    {
        close();
        return;
    }
    if (errorCode)
    {
        LOG4CPLUS_ERROR(sessionLogger, "HttpsSession: " << errorCode.message());
        return;
    }

    const std::shared_ptr<IController> userController = controller.lock();
    if (!userController)
    {
        LOG4CPLUS_ERROR(sessionLogger, "HttpsSession: controlles isn't available");
        close();
        return;
    }
    addResponseToQueue(userController->handleRequest(parser->release()));

    if (responceQueue.size() == queueSize)
    {
        LOG4CPLUS_INFO(sessionLogger, "HttpsSession: response queue is full");
        return;
    }

    read();
}

bool HttpSession::write()
{
    const bool wasFull = responceQueue.size() == queueSize;
    if (responceQueue.empty())
    {
        LOG4CPLUS_INFO(sessionLogger, "HttpsSession: nothing to write");
        return wasFull;
    }

    http::message_generator msg = std::move(responceQueue.front());
    const bool keepAlive = msg.keep_alive();
    responceQueue.pop_front();

    beast::async_write(stream, std::move(msg),
                       beast::bind_front_handler(&HttpSession::onWrite, shared_from_this(), keepAlive));

    return wasFull;
}

void HttpSession::onWrite(bool keepAlive, beast::error_code errorCode, size_t bytes_transfered)
{
    boost::ignore_unused(bytes_transfered);
    LOG4CPLUS_INFO(sessionLogger, "HttpsSession: response was written");

    if (errorCode)
    {
        LOG4CPLUS_ERROR(sessionLogger, "HttpsSession: " << errorCode.message());
        return;
    }

    if (!keepAlive)
    {
        close();
        return;
    }

    if (write())
    {
        read();
    }
}

void HttpSession::addResponseToQueue(http::message_generator response)
{
    responceQueue.push_back(std::move(response));
    LOG4CPLUS_INFO(sessionLogger, "HttpsSession: response was added to queue");
    if (responceQueue.size() != 1) return;

    write();
}

void HttpSession::close()
{
    LOG4CPLUS_INFO(sessionLogger, "HttpSession: session was closed");
    stream.socket().shutdown(tcp::socket::shutdown_send);
}
