#ifndef ABONENT_H
#define ABONENT_H

#include "Types.h"

enum class CallEndingStatus : uint8_t;
class AbonentController;

class Abonent
{
public:

    explicit Abonent(std::string phone, std::weak_ptr<const AbonentController> controller);

    [[nodiscard]] IdType getCallId() const;
    [[nodiscard]] std::string getPhone() const;

    void setCallId(IdType id);

private:

    IdType callId;
    std::string phone;

/* Section for simulation end call after random time. */
public:

    void response();

    void wait();
    void talk();

    bool wasResponded = false;

private:

    static TimeDuration getRandomDuration(const TimeDuration& min, const TimeDuration& max);

    static http::request<http::string_body> endCallRequest(CallEndingStatus callEndingStatus);

    std::weak_ptr<const AbonentController> controller;
    TimeDuration waitingTime;
    TimeDuration talkingTime;
};

#endif // ABONENT_H
