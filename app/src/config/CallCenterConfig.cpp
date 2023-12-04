#include "config/CallCenterConfig.h"

#include <fstream>

#include <nlohmann/json.hpp>

using json = nlohmann::json;

CallCenterConfig::CallCenterConfig()
    : CallCenterConfig("../../config.json")
{
}

CallCenterConfig::CallCenterConfig(const std::string& path)
{
    try
    {
        readFromJson(path);
    }
    catch (const json::exception& e)
    {
        queueSize = 1;
        operators = 1;
    }
}

void CallCenterConfig::readFromJson(const std::string& path)
{
    std::ifstream configFile(path);
    json config = json::parse(configFile);

    queueSize = config["callCenter"]["queueSize"];
    operators = config["callCenter"]["operators"];
}

size_t CallCenterConfig::getQueueSize() const
{
    return queueSize;
}

size_t CallCenterConfig::getOperators() const
{
    return operators;
}