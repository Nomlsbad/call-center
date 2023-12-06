#include "controller/UserController.h"
#include "CallCenter.h"

#include <log4cplus/loggingmacros.h>
#include <nlohmann/json.hpp>
#include <utils/Exceptions.h>

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
    try
    {
        LOG4CPLUS_INFO(controllerLogger, "UserController: accept request");
        const std::string& endpoint = req.target();
        auto& handler = enpoindsMap.at(endpoint);
        return handler(std::move(req));
    }
    catch (const std::out_of_range& e)
    {
        LOG4CPLUS_INFO(controllerLogger, "UserController: wrong endpoint (" << e.what() << ")");
        return badRequest(std::move(req), "Wrong endpoint");
    }
}

UserController::ResponceType UserController::registerCall(RequestType&& req) const
{
    try
    {
        LOG4CPLUS_INFO(controllerLogger, "UserController: request for register call was accepted."
                                              << "Request body: " << req.body());
        const std::shared_ptr<CallCenter> center = callCenter.lock();
        if (!center)
        {
            LOG4CPLUS_ERROR(controllerLogger, "UserController: call center isn't available");
            return serverError(std::move(req), "Server error");
        }

        IdType callId = 0;
        json requestBody = json::parse(req.body());
        const std::string phone = requestBody.at("phone");

        center->registerCall(callId, phone, boost::posix_time::microsec_clock::local_time());

        http::response<http::string_body> res(http::status::ok, req.version(), std::to_string(callId) + "\n");
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
        return badRequest(std::move(req), "You're already in the queue");
    }
    catch (const CCenter::Overload& e)
    {
        LOG4CPLUS_WARN(controllerLogger, "UserController: " << e.what());
        return serverError(std::move(req), "Sorry, queue is full");
    }
}

UserController::ResponceType UserController::endCall(RequestType&& req) const
{
    try
    {
        LOG4CPLUS_INFO(controllerLogger, "UserController: request for end call was accepted."
                                              << "Request body: " << req.body());
        const std::shared_ptr<CallCenter> center = callCenter.lock();
        if (!center)
        {
            LOG4CPLUS_ERROR(controllerLogger, "UserController: call center isn't available");
            return serverError(std::move(req), "Server error");
        }

        json requestBody = json::parse(req.body());
        const IdType callId = requestBody.at("callId");
        const CallEndingStatus callEndingStatus = requestBody.at("status");

        center->endCall(callId, callEndingStatus, boost::posix_time::microsec_clock::local_time());

        http::response<http::string_body> res(http::status::ok, req.version(), "Call was ended\n");
        res.set(http::field::content_type, "text/plain");
        res.prepare_payload();
        return res;
    }
    catch (const json::exception& e)
    {
        LOG4CPLUS_ERROR(controllerLogger, "UserController: " << e.what());
        return badRequest(std::move(req), e.what());
    }
    catch (const std::out_of_range& e)
    {
        LOG4CPLUS_ERROR(controllerLogger, "UserController: " << e.what());
        return notFound(std::move(req), "Call with this CallId doesn't exist");
    }
}