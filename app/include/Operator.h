#ifndef OPERATOR_H
#define OPERATOR_H

#include <thread>

#include <log4cplus/log4cplus.h>

#include "Types.h"

namespace Log = log4cplus;

class Operator
{
public:

    Operator();

    [[nodiscard]] IdType getId() const { return id; }

    [[nodiscard]] bool isConnected() const { return callCenter != nullptr; }

    void connectTo(class CallCenter* center) { callCenter = center; }

    void acceptCall(IdType callId);

private:

    void talk(IdType callId) const;

private:

    IdType id = 0;

    static IdType nextId;

    class CallCenter* callCenter = nullptr;

    std::thread callThread;

    Log::Logger operatorLogger;
};

#endif // OPERATOR_H
