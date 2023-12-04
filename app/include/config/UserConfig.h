#ifndef USERCONFIG_H
#define USERCONFIG_H

#include <nlohmann/json.hpp>

class UserConfig
{
public:

    UserConfig();
    explicit UserConfig(const std::string& path);

    void readFromJson(const std::string& path);

    [[nodiscard]] std::string getMinWaitingTime() const;
    [[nodiscard]] std::string getMaxWaitingTime() const;
    [[nodiscard]] std::string getMinTalkingTime() const;
    [[nodiscard]] std::string getMaxTalkingTime() const;

private:

    std::string minWaitingTime;
    std::string maxWaitingTime;
    std::string minTalkingTime;
    std::string maxTalkingTime;

};

#endif // USERCONFIG_H
