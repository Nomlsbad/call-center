#ifndef TYPES_H
#define TYPES_H

#include <boost/beast.hpp>
#include <boost/date_time/posix_time/posix_time_types.hpp>

using IdType = unsigned long long;
using Date = boost::posix_time::ptime;
using TimeDuration = boost::posix_time::time_duration;

using tcp = boost::asio::ip::tcp;

namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;

#endif //TYPES_H
