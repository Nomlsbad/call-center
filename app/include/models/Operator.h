#ifndef OPERATOR_H
#define OPERATOR_H

#include <log4cplus/log4cplus.h>

#include "utils/Types.h"

namespace Log = log4cplus;

class CallCenter;

class Operator : public std::enable_shared_from_this<Operator>
{
public:

    Operator();

    void connect(const std::weak_ptr<CallCenter>& center);
    void acceptCall(IdType callId);
    void onEndCall();

    [[nodiscard]] IdType getId() const;
    [[nodiscard]] bool isConnected() const;
    [[nodiscard]] bool wasBusy() const;
    [[nodiscard]] IdType getLastCallId() const;

private:

    IdType id = 0;
    IdType acceptedCallId = 0;
    bool isBusy = false;
    std::weak_ptr<CallCenter> callCenter;

    Log::Logger operatorLogger;
};

#endif // OPERATOR_H
