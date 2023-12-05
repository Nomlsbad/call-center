#ifndef SERVERCONFIG_H
#define SERVERCONFIG_H

#include <string>

class ServerConfig
{
public:

    ServerConfig();
    explicit ServerConfig(const std::string& path);

    void readFromJson(const std::string& path);

    [[nodiscard]] std::string getHost() const;
    [[nodiscard]] unsigned short getPort() const;
    [[nodiscard]] int getThreads() const;

private:

    std::string host;
    unsigned short port;
    int threads;
};

#endif // SERVERCONFIG_H
