#include "config/UserConfig.h"

#include <fstream>

using json = nlohmann::json;

UserConfig::UserConfig()
    :configPath("../../config.json")
{
    std::ifstream configFile(configPath);

    try
    {
        json config = json::parse(configFile);
        *this = config["user"];
    }
    catch (const json::exception& e)
    {
        minWaitingTime = "00:00:00";
        maxWaitingTime = "00:00:00";
        minTalkingTime = "00:00:00";
        maxTalkingTime = "00:00:00";
    }
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