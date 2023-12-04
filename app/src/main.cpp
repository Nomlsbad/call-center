#include <iostream>

#include "CallCenter.h"

#include <config/ServerConfig.h>
#include <server/Serever.h>

int main()
{
    Log::Initializer initializer;
    Log::PropertyConfigurator::doConfigure(LOG4CPLUS_TEXT("../../log4cplus.properties"));

    Serever serever{ServerConfig()};
    serever.run();

    return 0;
}