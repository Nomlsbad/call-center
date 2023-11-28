#ifndef OPERATOR_H
#define OPERATOR_H

#include <thread>

#include "Types.h"

class Operator
{
public:
    Operator(): id(nextId++) {};

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
};


#endif //OPERATOR_H
