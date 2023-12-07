#include "models/User.h"
#include "callcenter/CallCenter.h"
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
}

void User::wait(const TimeDuration& waitingTime)
{
    if (status != NONE) return;

    LOG4CPLUS_INFO(userLogger, "User: user[" << callId << "] will waiting " << to_simple_string(waitingTime) << "ms.");
    auto self = this;

    std::thread waiting(
        [self, waitingTime]
        {
            Log::Logger waitingLogger(Log::Logger::getInstance("CallHandlingLogger"));
            if (!self)
            {
                LOG4CPLUS_ERROR(waitingLogger, "User: user was destroyed");
                return;
            }

            const std::shared_ptr callCenter = self->callCenter.lock();
            const IdType callId = self->getCallId();

            std::this_thread::sleep_for(std::chrono::milliseconds(waitingTime.total_milliseconds()));
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
    status = WAITING;
}

void User::talk(const TimeDuration& talkingTime)
{
    if (status != WAITING) return;

    LOG4CPLUS_INFO(userLogger, "User: user[" << callId << "] will talking " << to_simple_string(talkingTime) << "ms.");
    auto self = this;

    std::thread talking(
        [self, talkingTime]
        {
            Log::Logger talkingLogger(Log::Logger::getInstance("CallHandlingLogger"));

            if (!self)
            {
                LOG4CPLUS_ERROR(talkingLogger, "User: user was destroyed");
                return;
            }
            const std::shared_ptr callCenter = self->callCenter.lock();
            const IdType callId = self->getCallId();

            std::this_thread::sleep_for(std::chrono::milliseconds(talkingTime.total_milliseconds()));
            if (!callCenter)
            {
                LOG4CPLUS_ERROR(talkingLogger, "User: call center was destroyed");
                return;
            }

            callCenter->endCall(callId, CallEndingStatus::OK, microsec_clock::local_time());
        });
    talking.detach();
    status = TALKING;
}

void User::response()
{
    if (status != WAITING) return;

    LOG4CPLUS_INFO(userLogger, "User: user[" << callId << "] get response");
    wasResponded = true;
}

IdType User::getCallId() const
{
    return callId;
}

std::string User::getPhone() const
{
    return phone;
}