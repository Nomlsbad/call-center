#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include "config/CallCenterConfig.h"
#include "config/ServerConfig.h"
#include "config/UserConfig.h"

#include "utils/Exceptions.h"
#include "utils/Macros.h"

class Configuration
{
public:

    template <typename T>
    static T& get() { throw CCenter::UnkownConfiguration("This unknown configuration class"); }

    GET_CONFIG(CallCenterConfig);
    GET_CONFIG(ServerConfig);
    GET_CONFIG(UserConfig);
};

#endif // CONFIGURATION_H
