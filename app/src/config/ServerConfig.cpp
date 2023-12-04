#include "config/ServerConfig.h"

#include <fstream>

using json = nlohmann::json;

ServerConfig::ServerConfig()
    : configPath("../../config.json")
{
    std::ifstream configFile(configPath);

    try
    {
        json config = json::parse(configFile);
        *this = config["server"];
    }
    catch (const json::exception& e)
    {
        host = "127.0.0.1";
        port = 37904;
        threads = 1;
    }
}

std::string ServerConfig::getHost() const
{
    return host;
}

unsigned short ServerConfig::getPort() const
{
    return port;
}

int ServerConfig::getThreads() const
{
    return threads;
}