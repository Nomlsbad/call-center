#include "CallCenter.h"
#include "models/Abonent.h"

#include <thread>

Abonent::Abonent(IdType callId, std::string phone, std::weak_ptr<CallCenter> callCenter)
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

void Abonent::wait() const
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

void Abonent::talk() const
{
    std::thread(
        [this]()
        {
            const std::chrono::milliseconds millisecondsTalkingTime(talkingTime.total_milliseconds());
            std::this_thread::sleep_for(millisecondsTalkingTime);

            callCenter.lock()->endCall(callId, CallEndingStatus::OK, boost::posix_time::microsec_clock::local_time());
        }).detach();
}

TimeDuration Abonent::getRandomDuration(const TimeDuration& min, const TimeDuration& max)
{
    const long minDuration = min.total_milliseconds();
    const long maxDuration = max.total_milliseconds();

    const long randomDuration = rand() % (maxDuration - minDuration) + minDuration;
    return boost::posix_time::milliseconds(randomDuration);
}

IdType Abonent::getCallId() const
{
    return callId;
}

std::string Abonent::getPhone() const
{
    return phone;
}

void Abonent::response()
{
    wasResponded = true;
}