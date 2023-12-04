#include "config/ServerConfig.h"

#include <fstream>

#include <nlohmann/json.hpp>

using json = nlohmann::json;

ServerConfig::ServerConfig()
    : ServerConfig("../../config.json")
{
}

ServerConfig::ServerConfig(const std::string& path)
{
    try
    {
        readFromJson(path);
    }
    catch (const json::exception& e)
    {
        host = "127.0.0.1";
        port = 37904;
        threads = 1;
    }
}

void ServerConfig::readFromJson(const std::string& path)
{
    std::ifstream configFile(path);
    json config = json::parse(configFile);

    host = config["server"]["host"];
    port = config["server"]["port"];
    threads = config["server"]["threads"];
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