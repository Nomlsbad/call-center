#ifndef USERCONFIG_H
#define USERCONFIG_H

#include <nlohmann/json.hpp>

class UserConfig
{
public:

    UserConfig();

    [[nodiscard]] std::string getMinWaitingTime() const;
    [[nodiscard]] std::string getMaxWaitingTime() const;
    [[nodiscard]] std::string getMinTalkingTime() const;
    [[nodiscard]] std::string getMaxTalkingTime() const;

private:

    std::string configPath;

    std::string minWaitingTime;
    std::string maxWaitingTime;
    std::string minTalkingTime;
    std::string maxTalkingTime;

public:

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(UserConfig, minWaitingTime, minWaitingTime, minTalkingTime, maxTalkingTime);

};

#endif // USERCONFIG_H
