#include "server/CallController.h"
#include "CallCenter.h"

CallController::CallController()
    : enpoindsMap({{"/register-call", [this](RequestType&& req) { return this->registerCall(std::move(req)); }},
                   {"/end-call", [this](RequestType&& req) { return this->endCall(std::move(req)); }}})
{
    callCenter = std::make_shared<CallCenter>(10, 5);
}

CallController::ResponceType CallController::handleRequest(RequestType&& req) const
{
    const std::string& endpoint = req.target();
    try
    {
        auto& handler = enpoindsMap.at(endpoint);
        return handler(std::move(req));
    }
    catch (const std::out_of_range& e)
    {
        //TODO: return error
        return http::response<http::string_body>();
    }
}

CallController::ResponceType CallController::registerCall(RequestType&& req) const
{
    boost::ignore_unused(req);
    //TODO: smth like that:
    //callCenter->registerCall(req.body(), boost::posix_time::microsec_clock::local_time());
    return http::response<http::string_body>();
}

CallController::ResponceType CallController::endCall(RequestType&& req) const
{
    boost::ignore_unused(req);
    //TODO: smth like that:
    //callCenter->endCall(callId, callEndingStatus,
    //                    boost::posix_time::microsec_clock::local_time());
    return http::response<http::string_body>();
}