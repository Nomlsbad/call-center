#include "utils/UserSimulation.h"
#include "CallCenter.h"
#include "config/Configuration.h"
#include "models/User.h"

#include <boost/date_time/posix_time/time_parsers.hpp>

using boost::posix_time::duration_from_string;

UserSimulation::UserSimulation(std::weak_ptr<CallCenter> callCenter)
    : minWaitingTime(duration_from_string(Configuration::get<UserConfig>("minWaitingTime"))),
      maxWaitingTime(duration_from_string(Configuration::get<UserConfig>("maxWaitingTime"))),
      minTalkingTime(duration_from_string(Configuration::get<UserConfig>("minTalkingTime"))),
      maxTalkingTime(duration_from_string(Configuration::get<UserConfig>("maxTalkingTime"))),
      callCenter(std::move(callCenter)),
      userLogger(Log::Logger::getInstance("CallHandlingLogger"))
{
}

void UserSimulation::onRegisterCall(IdType callId, std::string phone)
{
    LOG4CPLUS_INFO(userLogger, "UserSimulation: user's call[" << callId << "] was registred");
    const auto user = users.emplace(callId, User(callId, std::move(phone), callCenter));
    user.first->second.wait(getWaitingDuration());
}

void UserSimulation::onResponse(IdType callId)
{
    LOG4CPLUS_INFO(userLogger, "UserSimulation: user's call[" << callId << "] was responded");
    User& user = users.at(callId);
    user.response();
    user.talk(getTalkingDuration());
}

void UserSimulation::onEndCall(IdType callId)
{
    LOG4CPLUS_INFO(userLogger, "UserSimulation: user's call [" << callId << "]was ended");
    users.erase(callId);
}

TimeDuration UserSimulation::getRandomDuration(const TimeDuration& min, const TimeDuration& max)
{
    const long minDuration = min.total_milliseconds();
    const long maxDuration = max.total_milliseconds();

    const long mod = std::max<long>(1, maxDuration - minDuration);
    const long randomDuration = rand() % mod + minDuration;
    return boost::posix_time::milliseconds(randomDuration);
}

TimeDuration UserSimulation::getWaitingDuration() const
{
    return getRandomDuration(minWaitingTime, maxWaitingTime);
}

TimeDuration UserSimulation::getTalkingDuration() const
{
    return getRandomDuration(minTalkingTime, maxTalkingTime);
}