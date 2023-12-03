#include "models/Abonent.h"

#include <CallDetail.h>
#include <controller/AbonentController.h>
#include <thread>

Abonent::Abonent(std::string phone, std::weak_ptr<const AbonentController> controller)
    : callId(0),
      phone(std::move(phone)),
      controller(std::move(controller))
{
    const TimeDuration minWaitingTime(0, 0, 1);
    const TimeDuration maxWaitingTime(0, 0, 5);
    waitingTime = getRandomDuration(minWaitingTime, maxWaitingTime);

    const TimeDuration minTalkingTime(0, 0, 3);
    const TimeDuration maxTalkingTime(0, 0, 10);
    talkingTime = getRandomDuration(minTalkingTime, maxTalkingTime);
}

void Abonent::wait()
{
    std::thread(
        [this]()
        {
            const std::chrono::milliseconds millisecondsWaitingTime(waitingTime.total_milliseconds());
            std::this_thread::sleep_for(millisecondsWaitingTime);

            if (wasResponded) return;
            controller.lock()->endCall(endCallRequest(CallEndingStatus::TIMEOUT));
        }).detach();
}

void Abonent::talk()
{
    std::thread(
        [this]()
        {
            const std::chrono::milliseconds millisecondsTalkingTime(talkingTime.total_milliseconds());
            std::this_thread::sleep_for(millisecondsTalkingTime);

            controller.lock()->endCall(endCallRequest(CallEndingStatus::OK));
        }).detach();
}

TimeDuration Abonent::getRandomDuration(const TimeDuration& min, const TimeDuration& max)
{
    const long minDuration = min.total_milliseconds();
    const long maxDuration = max.total_milliseconds();

    const long randomDuration = rand() % (maxDuration - minDuration) + minDuration;
    return boost::posix_time::milliseconds(randomDuration);
}

http::request<http::string_body> Abonent::endCallRequest(CallEndingStatus callEndingStatus)
{
    http::request<http::string_body> req{http::verb::post, "/end-call", 11};
    const int staus = static_cast<int>(callEndingStatus);

    req.body() = R"('{"callId":)" + std::to_string(123) + R"(,"status":)" + std::to_string(staus) + "}\'";
    req.prepare_payload();
    return req;
}

IdType Abonent::getCallId() const
{
    return callId;
}

std::string Abonent::getPhone() const
{
    return phone;
}

void Abonent::setCallId(IdType id)
{
    callId = id;
}

void Abonent::response()
{
    wasResponded = true;
}