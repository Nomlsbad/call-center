#include "config/CallCenterConfig.h"

#include <fstream>

using json = nlohmann::json;

CallCenterConfig::CallCenterConfig()
    : configPath("../../config.json")
{
    std::ifstream configFile(configPath);

    try
    {
        json config = json::parse(configFile);
        *this = config["callCenter"];
    }
    catch (const json::exception& e)
    {
        queueSize = 1;
        operators = 1;
    }
}

size_t CallCenterConfig::getQueueSize() const
{
    return queueSize;
}

size_t CallCenterConfig::getOperators() const
{
    return operators;
}