#ifndef OPERATOR_H
#define OPERATOR_H

#include <log4cplus/log4cplus.h>

#include "utils/Types.h"

namespace Log = log4cplus;

class CallCenter;

class Operator
{
public:

    Operator();

    [[nodiscard]] IdType getId() const;

    void connect(std::weak_ptr<CallCenter> center, IdType operatorId);
    void acceptCall(IdType callId);

private:

    IdType id = 0;
    std::weak_ptr<CallCenter> callCenter;

    Log::Logger operatorLogger;
};

#endif // OPERATOR_H
