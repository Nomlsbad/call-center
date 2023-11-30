#include "server/CallController.h"

CallController::CallController()
    : enpoindsMap(
        {
            {"/register-call", std::bind(&CallController::registerCall, std::placeholders::_1)}
           })
{
}

CallController::ResponceType CallController::registerCall(RequestType&& req)
{
    boost::ignore_unused(req);
    return http::response<http::string_body>();
}
