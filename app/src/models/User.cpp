#include "CallCenter.h"
#include "models/User.h"
#include "config/Configuration.h"

#include <boost/date_time/posix_time/time_parsers.hpp>
#include <thread>

User::User(IdType callId, std::string phone, std::weak_ptr<CallCenter> callCenter)
    : callId(callId),
      phone(std::move(phone)),
      callCenter(std::move(callCenter))
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
    std::thread(
        [this]()
        {
            const std::chrono::milliseconds msWaitingTime(waitingTime.total_milliseconds());
            std::this_thread::sleep_for(msWaitingTime);

            if (wasResponded) return;
            callCenter.lock()->endCall(callId, CallEndingStatus::TIMEOUT, boost::posix_time::microsec_clock::local_time());
        }).detach();
}

void User::talk() const
{
    std::thread(
        [this]()
        {
            const std::chrono::milliseconds millisecondsTalkingTime(talkingTime.total_milliseconds());
            std::this_thread::sleep_for(millisecondsTalkingTime);

            callCenter.lock()->endCall(callId, CallEndingStatus::OK, boost::posix_time::microsec_clock::local_time());
        }).detach();
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

void User::response()
{
    wasResponded = true;
}