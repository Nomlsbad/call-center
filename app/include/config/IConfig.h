#ifndef ICONFIG_H
#define ICONFIG_H

#include <string>

class IConfig
{
public:
    IConfig() = default;
    virtual ~IConfig() = default;

    virtual void readFromFile(const std::string& path) = 0;

};

#endif // ICONFIG_H
