#include "config/UserConfig.h"

#include <fstream>

using json = nlohmann::json;

UserConfig::UserConfig()
    : UserConfig("../../config.json")
{
}

UserConfig::UserConfig(const std::string& path)
{
    try
    {
        readFromJson(path);
    }
    catch (const json::exception& e)
    {
        minWaitingTime = "00:00:00";
        maxWaitingTime = "00:00:00";
        minTalkingTime = "00:00:00";
        maxTalkingTime = "00:00:00";
    }
}

void UserConfig::readFromJson(const std::string& path)
{
    std::ifstream configFile(path);
    json config = json::parse(configFile);

    minWaitingTime = config["user"]["minWaitingTime"];
    maxWaitingTime = config["user"]["maxWaitingTime"];
    minTalkingTime = config["user"]["minTalkingTime"];
    maxTalkingTime = config["user"]["maxTalkingTime"];
}

std::string UserConfig::getMinWaitingTime() const
{
    return minWaitingTime;
}

std::string UserConfig::getMaxWaitingTime() const
{
    return maxWaitingTime;
}

std::string UserConfig::getMinTalkingTime() const
{
    return minTalkingTime;
}

std::string UserConfig::getMaxTalkingTime() const
{
    return maxTalkingTime;
}