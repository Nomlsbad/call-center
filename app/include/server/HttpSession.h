#ifndef HTTPSESSION_H
#define HTTPSESSION_H

#include "Types.h"

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <memory>

namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;

class HttpSession : public std::enable_shared_from_this<HttpSession>
{
public:

    explicit HttpSession(tcp::socket&& socket);

    void run();

private:

    void doRead();
    void onRead(beast::error_code errorCode, size_t bytes_transfered);

private:

    beast::tcp_stream stream;
    beast::flat_buffer buffer;

    static constexpr size_t queueSize = 8;
    std::vector<http::message_generator> responceQueue;

    std::optional<http::request_parser<http::string_body>> parser;
};

#endif // HTTPSESSION_H
