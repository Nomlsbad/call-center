#ifndef CALLCENTERCONFIG_H
#define CALLCENTERCONFIG_H

#include <nlohmann/json.hpp>

class CallCenterConfig
{
public:

    CallCenterConfig();

    [[nodiscard]] size_t getQueueSize() const;
    [[nodiscard]] size_t getOperators() const;

private:

    std::string configPath;

    size_t queueSize;
    size_t operators;

public:

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(CallCenterConfig, queueSize, operators);

};

#endif // CALLCENTERCONFIG_H
