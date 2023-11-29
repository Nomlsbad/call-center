#include <iostream>

#include "CallCenter.h"

int main()
{
    Log::Initializer initializer;
    Log::PropertyConfigurator::doConfigure(LOG4CPLUS_TEXT("../../log4cplus.properties"));

    CallCenter callCenter(5, 10);

    for (size_t i = 0; i < 20; ++i)
    {
        callCenter.registerCall("+71234567890", boost::posix_time::microsec_clock::local_time());
    }

    while (true) {};

    return 0;
}