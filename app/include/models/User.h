#ifndef USER_H
#define USER_H

#include "utils/Types.h"

#include <log4cplus/log4cplus.h>

enum class CallEndingStatus : uint8_t;

class CallCenter;

namespace Log = log4cplus;

class User
{
public:

    User(IdType callId, std::string phone, std::weak_ptr<CallCenter> callCenter);

    [[nodiscard]] IdType getCallId() const;
    [[nodiscard]] std::string getPhone() const;

public:

    void response();

    void wait(const TimeDuration& waitingTime);
    void talk(const TimeDuration& talkingTime);

private:

    IdType callId;
    std::string phone;

    std::weak_ptr<CallCenter> callCenter;

    Log::Logger userLogger;

    enum Status
    {
        WAITING = 0,
        TALKING = 1,

        NONE = 2
    };
    Status status = NONE;
    bool wasResponded = false;
};

#endif // USER_H
