#include "server/HttpSession.h"
#include "server/CallController.h"

#include <boost/asio/dispatch.hpp>

HttpSession::HttpSession(tcp::socket&& socket, std::weak_ptr<CallController> controller)
    : stream(std::move(socket)),
      controller(std::move(controller))
{
}

void HttpSession::run()
{
    net::dispatch(stream.get_executor(), beast::bind_front_handler(&HttpSession::read, shared_from_this()));
}

void HttpSession::read()
{
    parser.emplace();
    parser->body_limit(20);

    stream.expires_after(std::chrono::seconds(30));

    http::async_read(stream, buffer, *parser, beast::bind_front_handler(&HttpSession::onRead, shared_from_this()));
}

void HttpSession::onRead(beast::error_code errorCode, size_t bytes_transfered)
{
    boost::ignore_unused(bytes_transfered);

    if (errorCode == http::error::end_of_stream)
    {
        close();
        return;
    }
    if (errorCode)
    {
        // Log
        return;
    }

    addResponseToQueue(controller.lock()->handleRequest(parser->release()));
    if (responceQueue.size() == queueSize) return;

    read();
}

bool HttpSession::write()
{
    const bool wasFull = responceQueue.size() == queueSize;
    if (responceQueue.empty()) return wasFull;

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

    if (errorCode)
    {
        // log
        return;
    }

    if (!keepAlive)
    {
        return close();
    }

    if (write())
    {
        read();
    }
}

void HttpSession::addResponseToQueue(http::message_generator response)
{
    responceQueue.push_back(std::move(response));
    if (responceQueue.size() != 1) return;

    write();
}

void HttpSession::close()
{
    beast::error_code errorCode;
    stream.socket().shutdown(tcp::socket::shutdown_send, errorCode);
}
