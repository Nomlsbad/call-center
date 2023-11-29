#ifndef HTTPSESSION_H
#define HTTPSESSION_H

#include "Types.h"

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <memory>

namespace beast = boost::beast;
namespace http = beast::http;

class HttpSession : public std::enable_shared_from_this<HttpSession>
{
public:

    explicit HttpSession(tcp::socket&& socket);

    void run(){};

private:

    beast::tcp_stream stream;

    static constexpr size_t queueSize = 8;
    std::vector<http::message_generator> responceQueue;
};

#endif // HTTPSESSION_H
