#ifndef CONFIGS_H
#define CONFIGS_H

#include "config/JsonConfig.h"
#include "utils/Macros.h"

JSON_CONFIG(CallCenterConfig, "/callCenter")
{
    config["queueSize"] = 1;
    config["operators"] = 1;
}

JSON_CONFIG(ServerConfig, "/server")
{
    config["address"] = "127.0.0.1";
    config["port"] = 29147;
    config["threads"] = 1;
}

JSON_CONFIG(UserConfig, "/user")
{
    config["minWaitingTime"] = "00:00:01";
    config["maxWaitingTime"] = "00:00:01";
    config["minTalkingTime"] = "00:00:01";
    config["maxTalkingTime"] = "00:00:01";
}

#endif // CONFIGS_H