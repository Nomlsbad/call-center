#include "controller/UserController.h"
#include "CallCenter.h"

#include <utils/Exceptions.h>
#include <log4cplus/loggingmacros.h>
#include <nlohmann/json.hpp>

UserController::UserController(std::weak_ptr<CallCenter> callCenter)
    : callCenter(std::move(callCenter)),
      controllerLogger(Log::Logger::getInstance(LOG4CPLUS_TEXT("ServerLogger"))),
      enpoindsMap({
          {"/register-call", [this](RequestType&& req) { return this->registerCall(std::move(req)); }},
          {"/end-call", [this](RequestType&& req) { return this->endCall(std::move(req)); }},
      })
{
}

UserController::ResponceType UserController::handleRequest(RequestType&& req) const
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
        return badRequest(std::move(req), e.what());
    }
}

UserController::ResponceType UserController::registerCall(RequestType&& req) const
{
    LOG4CPLUS_DEBUG(controllerLogger, "/register-call");

    try
    {
        IdType callId = 0;
        json requestBody = json::parse(req.body());
        const std::string phone = requestBody.at("phone");

        callCenter.lock()->registerCall(callId, phone, boost::posix_time::microsec_clock::local_time());

        http::response<http::string_body> res(http::status::ok, req.version(), std::to_string(callId));
        res.set(http::field::content_type, "text/plain");
        res.prepare_payload();
        return res;
    }
    catch (const json::exception& e)
    {
        LOG4CPLUS_ERROR(controllerLogger, "UserController: " << e.what());
        return badRequest(std::move(req), e.what());
    }
    catch (const CCenter::AlreadyInQueue& e)
    {
        LOG4CPLUS_ERROR(controllerLogger, "UserController: " << e.what());
        return badRequest(std::move(req), e.what());
    }
    catch (const CCenter::Overload& e)
    {
        LOG4CPLUS_ERROR(controllerLogger, "UserController: " << e.what());
        return serverError(std::move(req), e.what());
    }
}

UserController::ResponceType UserController::endCall(RequestType&& req) const
{
    LOG4CPLUS_DEBUG(controllerLogger, "/end-call");

    try
    {
        json requestBody = json::parse(req.body());
        const IdType callId = requestBody.at("callId");
        const CallEndingStatus callEndingStatus = requestBody.at("status");

        callCenter.lock()->endCall(callId, callEndingStatus, boost::posix_time::microsec_clock::local_time());

        http::response<http::string_body> res(http::status::ok, req.version(), "Call was ended");
        res.set(http::field::content_type, "text/plain");
        res.prepare_payload();
        return res;
    }
    catch (const json::exception& e)
    {
        LOG4CPLUS_ERROR(controllerLogger, "UserController: " << e.what());
        return badRequest(std::move(req), e.what());
    }
}