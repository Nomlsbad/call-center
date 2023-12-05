#include "server/Serever.h"

#include <log4cplus/configurator.h>
#include <log4cplus/initializer.h>

int main()
{
    Log::Initializer initializer;
    Log::PropertyConfigurator::doConfigure(LOG4CPLUS_TEXT("../../log4cplus.properties"));

    Serever serever{};
    serever.run();

    return 0;
}