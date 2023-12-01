#ifndef HTTPSESSION_H
#define HTTPSESSION_H

#include "Types.h"

#include <deque>
#include <memory>

class CallController;

class HttpSession : public std::enable_shared_from_this<HttpSession>
{
public:

    explicit HttpSession(tcp::socket&& socket, std::weak_ptr<CallController> controller);

    void run();

private:

    void read();
    void onRead(beast::error_code errorCode, size_t bytes_transfered);

    bool write();
    void onWrite(bool keepAlive, beast::error_code errorCode, size_t bytes_transfered);

    void addResponseToQueue(http::message_generator response);

    void close();

private:

    beast::tcp_stream stream;
    beast::flat_buffer buffer;

    std::weak_ptr<CallController> controller;

    static constexpr size_t queueSize = 8;
    std::deque<http::message_generator> responceQueue;

    std::optional<http::request_parser<http::string_body>> parser;
};

#endif // HTTPSESSION_H
