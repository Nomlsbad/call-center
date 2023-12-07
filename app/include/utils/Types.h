#ifndef TYPES_H
#define TYPES_H

#include <boost/beast.hpp>
#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <nlohmann/json.hpp>

#include <nlohmann/json_fwd.hpp>

using IdType = unsigned long long;


//------------Boost network-------------//
using tcp = boost::asio::ip::tcp;

namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
//------------Boost network-------------//


//--------------Boost time--------------//
using Date = boost::posix_time::ptime;
using TimeDuration = boost::posix_time::time_duration;
//--------------Boost time--------------//


//------------Nlohmann JSON------------//
using basic_json = nlohmann::basic_json<>;
using json = nlohmann::json;
using json_ptr = json::json_pointer;
//------------Nlohmann JSON------------//

#endif //TYPES_H
