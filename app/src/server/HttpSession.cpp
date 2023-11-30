#include "server/HttpSession.h"

#include <boost/asio/dispatch.hpp>

HttpSession::HttpSession(tcp::socket&& socket)
    : stream(std::move(socket))
{
    responceQueue.reserve(queueSize);
}

void HttpSession::run()
{
    net::dispatch(stream.get_executor(), beast::bind_front_handler(&HttpSession::doRead, shared_from_this()));
}

void HttpSession::doRead()
{
    parser.emplace();
    parser->body_limit(20);

    stream.expires_after(std::chrono::seconds(30));

    http::async_read(stream, buffer, *parser, beast::bind_front_handler(&HttpSession::onRead, shared_from_this()));
}

void HttpSession::onRead(beast::error_code errorCode, size_t bytes_transfered)
{
    boost::ignore_unused(bytes_transfered);
    if (errorCode)
    {
        return;
    }
}
