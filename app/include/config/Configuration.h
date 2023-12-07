#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include "config/Configs.h"

#include <concepts>

class Configuration
{
public:

    static void initialize(std::string path)
    {
        configPath = std::move(path);
    }

    template <std::derived_from<JsonConfig> Config>
    static basic_json get(const std::string& param)
    {
        static Config config;
        config.update(configPath, true);
        return config[param];
    }

private:

    static inline std::string configPath = "../../config.json";
};

#endif // CONFIGURATION_H
