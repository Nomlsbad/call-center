#include "config/JsonConfig.h"

#include <fstream>

JsonConfig::JsonConfig()
    : JsonConfig("/"_json_pointer)
{
}

JsonConfig::JsonConfig(json_ptr jsonPtr)
    : jsonPtr(std::move(jsonPtr))
{
}

void JsonConfig::update(const std::string& path, bool checkOnlyPath)
{
    if (checkOnlyPath && path == configPath) return;
    readFromFile(path);
}

void JsonConfig::readFromFile(const std::string& path)
{
    std::ifstream file(path);
    json configFile = json::parse(file);
    configFile = configFile.at(jsonPtr);

    for (auto& [key, value]: config.items())
    {
        value = configFile.at(key);
    }

    configPath = path;
}

basic_json JsonConfig::operator[] (const std::string& param)
{
    return config.at(param);
}
