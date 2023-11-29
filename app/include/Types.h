#ifndef TYPES_H
#define TYPES_H

#include <boost/asio/ip/tcp.hpp>
#include <boost/date_time/posix_time/posix_time_types.hpp>


using IdType = unsigned long long;
using Date = boost::posix_time::ptime;
using TimeDuration = boost::posix_time::time_duration;

using tcp = boost::asio::ip::tcp;


#endif //TYPES_H
