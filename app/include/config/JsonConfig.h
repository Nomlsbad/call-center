#ifndef JSONCONFIG_H
#define JSONCONFIG_H

#include "utils/Types.h"
#include "config/IConfig.h"

#include <nlohmann/json.hpp>

class JsonConfig : public IConfig
{
public:

    JsonConfig();
    explicit JsonConfig(json_ptr jsonPtr);

    void update(const std::string& path, bool checkOnlyPath = false);
    void readFromFile(const std::string& path) override;

    basic_json operator[] (const std::string& param);

protected:

    json config;
    json_ptr jsonPtr;

    std::string configPath;
};

#endif // JSONCONFIG_H
