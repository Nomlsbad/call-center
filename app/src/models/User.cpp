#include "models/User.h"
#include "CallCenter.h"
#include "config/Configuration.h"

#include <boost/date_time/posix_time/time_formatters.hpp>
#include <boost/date_time/posix_time/time_parsers.hpp>
#include <thread>

using boost::posix_time::to_simple_string;
using boost::posix_time::microsec_clock;

User::User(IdType callId, std::string phone, std::weak_ptr<CallCenter> callCenter)
    : callId(callId),
      phone(std::move(phone)),
      callCenter(std::move(callCenter)),
      userLogger(Log::Logger::getInstance("CallHandlingLogger"))
{
    using boost::posix_time::duration_from_string;

    const TimeDuration minWaitingTime = duration_from_string(Configuration::get<UserConfig>().getMinWaitingTime());
    const TimeDuration maxWaitingTime = duration_from_string(Configuration::get<UserConfig>().getMaxWaitingTime());
    waitingTime = getRandomDuration(minWaitingTime, maxWaitingTime);

    const TimeDuration minTalkingTime = duration_from_string(Configuration::get<UserConfig>().getMinTalkingTime());
    const TimeDuration maxTalkingTime = duration_from_string(Configuration::get<UserConfig>().getMaxTalkingTime());
    talkingTime = getRandomDuration(minTalkingTime, maxTalkingTime);
}

void User::wait() const
{
    LOG4CPLUS_INFO(userLogger, "User: user[" << callId << "] will waiting " << to_simple_string(waitingTime) << "ms.");

    auto self = this;
    std::thread waiting(
        [self]
        {
            Log::Logger waitingLogger(Log::Logger::getInstance("CallHandlingLogger"));
            if (!self)
            {
                LOG4CPLUS_ERROR(waitingLogger, "User: user was destroyed");
                return;
            }

            const std::shared_ptr callCenter = self->callCenter.lock();
            const IdType callId = self->getCallId();
            const std::chrono::milliseconds waitingTime(self->waitingTime.total_milliseconds());

            std::this_thread::sleep_for(waitingTime);
            if (!self || !callCenter || self->wasResponded)
            {
                if (!self)
                {
                    LOG4CPLUS_ERROR(waitingLogger, "User: user was destroyed");
                }
                if (!callCenter)
                {
                    LOG4CPLUS_ERROR(waitingLogger, "User: call center was destroyed");
                }
                return;
            }

            callCenter->endCall(callId, CallEndingStatus::TIMEOUT, microsec_clock::local_time());
        });
    waiting.detach();
}

void User::talk() const
{
    LOG4CPLUS_INFO(userLogger, "User: user[" << callId << "] will talking " << to_simple_string(waitingTime) << "ms.");
    auto self = this;
    std::thread talking(
        [self]
        {
            Log::Logger talkingLogger(Log::Logger::getInstance("CallHandlingLogger"));

            if (!self)
            {
                LOG4CPLUS_ERROR(talkingLogger, "User: user was destroyed");
                return;
            }
            const std::shared_ptr callCenter = self->callCenter.lock();
            const IdType callId = self->getCallId();
            const std::chrono::milliseconds talkingTime(self->talkingTime.total_milliseconds());

            std::this_thread::sleep_for(talkingTime);
            if (!callCenter)
            {
                LOG4CPLUS_ERROR(talkingLogger, "User: call center was destroyed");
                return;
            }

            callCenter->endCall(callId, CallEndingStatus::OK, microsec_clock::local_time());
        });
    talking.detach();
}

void User::response()
{
    LOG4CPLUS_INFO(userLogger, "User: user[" << callId << "] get response");
    wasResponded = true;
    talk();
}

TimeDuration User::getRandomDuration(const TimeDuration& min, const TimeDuration& max)
{
    const long minDuration = min.total_milliseconds();
    const long maxDuration = max.total_milliseconds();

    const long randomDuration = rand() % (maxDuration - minDuration) + minDuration;
    return boost::posix_time::milliseconds(randomDuration);
}

IdType User::getCallId() const
{
    return callId;
}

std::string User::getPhone() const
{
    return phone;
}