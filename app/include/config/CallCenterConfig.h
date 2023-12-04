#ifndef CALLCENTERCONFIG_H
#define CALLCENTERCONFIG_H

#include <string>

class CallCenterConfig
{
public:

    CallCenterConfig();
    explicit CallCenterConfig(const std::string& path);

    void readFromJson(const std::string& path);

    [[nodiscard]] size_t getQueueSize() const;
    [[nodiscard]] size_t getOperators() const;

private:

    size_t queueSize;
    size_t operators;

};

#endif // CALLCENTERCONFIG_H
