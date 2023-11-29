#include "server/HttpSession.h"

HttpSession::HttpSession(tcp::socket&& socket)
    : stream(std::move(socket))
{
    responceQueue.reserve(queueSize);
}