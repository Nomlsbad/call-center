#include "CallCenter.h"
#include "models/User.h"

#include <thread>

User::User(IdType callId, std::string phone, std::weak_ptr<CallCenter> callCenter)
    : callId(callId),
      phone(std::move(phone)),
      callCenter(std::move(callCenter))
{
    const TimeDuration minWaitingTime(0, 0, 1);
    const TimeDuration maxWaitingTime(0, 0, 5);
    waitingTime = getRandomDuration(minWaitingTime, maxWaitingTime);

    const TimeDuration minTalkingTime(0, 0, 3);
    const TimeDuration maxTalkingTime(0, 0, 10);
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