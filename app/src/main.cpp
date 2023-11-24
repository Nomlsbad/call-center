#include <iostream>

#include "CallCenter.h"

int main()
{
    CallCenter callCenter(0, 10);
    callCenter.registerCall("+71234567890", boost::posix_time::microsec_clock::local_time());

    return 0;
}