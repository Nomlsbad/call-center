#include "controller/AbonentController.h"
#include "CallCenter.h"

#include <log4cplus/loggingmacros.h>

AbonentController::AbonentController()
    : callCenter(std::make_shared<CallCenter>(10, 5)),
      controllerLogger(Log::Logger::getInstance(LOG4CPLUS_TEXT("ServerLogger"))),
      enpoindsMap(
          {
          {"/register-call", [this](RequestType&& req) { return this->registerCall(std::move(req)); }},
          {"/end-call", [this](RequestType&& req) { return this->endCall(std::move(req)); }},
          }
      )
{
}

AbonentController::ResponceType AbonentController::handleRequest(RequestType&& req) const
{
    const std::string& endpoint = req.target();
    LOG4CPLUS_DEBUG(controllerLogger, "Handling request was begun");
    try
    {
        auto& handler = enpoindsMap.at(endpoint);
        return handler(std::move(req));
    }
    catch (const std::out_of_range& e)
    {
        http::response<http::string_body> res{http::status::bad_request, req.version()};
        res.prepare_payload();
        return res;
    }
}

AbonentController::ResponceType AbonentController::registerCall(RequestType&& req) const
{
    LOG4CPLUS_DEBUG(controllerLogger, "/register-call");
    http::response<http::string_body> res{http::status::ok, req.version()};
    // TODO: smth like that:
    // callCenter->registerCall(req.body(), boost::posix_time::microsec_clock::local_time());
    res.prepare_payload();
    return res;
}

AbonentController::ResponceType AbonentController::endCall(RequestType&& req) const
{
    LOG4CPLUS_DEBUG(controllerLogger, "/end-call");
    http::response<http::string_body> res{http::status::ok, req.version()};
    // TODO: smth like that:
    // callCenter->endCall(callId, callEndingStatus,
    //                     boost::posix_time::microsec_clock::local_time());
    res.prepare_payload();
    return res;
}
