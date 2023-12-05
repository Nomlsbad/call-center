#ifndef USER_H
#define USER_H

#include "Types.h"

enum class CallEndingStatus : uint8_t;

class CallCenter;

class User
{
public:

    explicit User(IdType callId, std::string phone, std::weak_ptr<CallCenter> callCenter);

    [[nodiscard]] IdType getCallId() const;
    [[nodiscard]] std::string getPhone() const;

private:

    IdType callId;
    std::string phone;

/* Section for simulation end call after random time. */
public:

    void response();

    void wait() const;
    void talk() const;

    bool wasResponded = false;

private:

    static TimeDuration getRandomDuration(const TimeDuration& min, const TimeDuration& max);

    std::weak_ptr<CallCenter> callCenter;
    TimeDuration waitingTime;
    TimeDuration talkingTime;
};

#endif // USER_H
