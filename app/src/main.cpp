#include "server/Server.h"

#include <config/Configuration.h>
#include <log4cplus/configurator.h>
#include <log4cplus/initializer.h>

int main()
{
    const std::string sourceDir = std::string(PROJECT_SOURCE_DIR);

    const std::string configPath = sourceDir + "/config.json";
    Configuration::initialize(configPath);

    const std::string loggerPropertiesPath = sourceDir + "/log4cplus.properties";
    Log::Initializer initializer;
    Log::PropertyConfigurator::doConfigure(LOG4CPLUS_TEXT(loggerPropertiesPath));

    Server server{};
    server.run();

    return 0;
}