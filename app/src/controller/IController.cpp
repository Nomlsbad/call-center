#include "controller/IController.h"

IController::ResponceType IController::badRequest(RequestType&& req, std::string what)
{
    return errorResponce(std::move(req), std::move(what), http::status::bad_request);
}

IController::ResponceType IController::notFound(RequestType&& req, std::string what)
{
    return errorResponce(std::move(req), std::move(what), http::status::not_found);;
}

IController::ResponceType IController::serverError(RequestType&& req, std::string what)
{
    return errorResponce(std::move(req), std::move(what), http::status::internal_server_error);
}

IController::ResponceType IController::errorResponce(RequestType&& req, std::string what, http::status status)
{
    http::response<http::string_body> res{status, req.version()};
    res.set(http::field::content_type, "text/plain");
    res.keep_alive(req.keep_alive());
    res.body() = std::move(what);
    res.prepare_payload();
    return res;
}
