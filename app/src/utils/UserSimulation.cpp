#include "CallCenter.h"

#include "models/User.h"
#include "utils/UserSimulation.h"

UserSimulation::UserSimulation(std::weak_ptr<CallCenter> callCenter)
    : callCenter(std::move(callCenter)),
      userLogger(Log::Logger::getInstance("CallHandlingLogger"))
{
}

void UserSimulation::onRegisterCall(IdType callId, std::string phone)
{
    LOG4CPLUS_INFO(userLogger, "UserSimulation: user's call[" << callId << "] was registred");
    const auto user = users.emplace(callId, User(callId, std::move(phone), callCenter));
    user.first->second.wait();
}

void UserSimulation::onResponse(IdType callId)
{
    LOG4CPLUS_INFO(userLogger, "UserSimulation: user's call[" << callId << "] was responded");
    User& user = users.at(callId);
    user.response();
}

void UserSimulation::onEndCall(IdType callId)
{
    LOG4CPLUS_INFO(userLogger, "UserSimulation: user's call [" << callId << "]was ended");
    users.erase(callId);
}