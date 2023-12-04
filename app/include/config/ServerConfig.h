#ifndef SERVERCONFIG_H
#define SERVERCONFIG_H

#include <nlohmann/json.hpp>

class ServerConfig
{
public:

    ServerConfig();

    [[nodiscard]] std::string getHost() const;
    [[nodiscard]] unsigned short getPort() const;
    [[nodiscard]] int getThreads() const;

private:

    std::string configPath;

    std::string host;
    unsigned short port;
    int threads;

public:

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(ServerConfig, host, port, threads);
};

#endif // SERVERCONFIG_H
