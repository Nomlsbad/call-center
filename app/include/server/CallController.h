#ifndef CALLCONTROLLER_H
#define CALLCONTROLLER_H

#include <boost/beast/http.hpp>
#include <functional>
#include <string>
#include <unordered_map>

namespace beast = boost::beast;
namespace http = beast::http;

class CallController
{
public:

    using RequestType = http::request<http::string_body>;
    using ResponceType = http::message_generator;
    using HandleRequest = std::function<ResponceType(RequestType&&)>;

public:

    CallController();

    static ResponceType registerCall(RequestType&& req);

private:

    const std::unordered_map<std::string, const HandleRequest> enpoindsMap;
};

#endif // CALLCONTROLLER_H
