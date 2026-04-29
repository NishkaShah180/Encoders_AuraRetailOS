#pragma once
#include <string>
#include <vector>

class SMTPClient {
public:
    struct Config {
        std::string host;
        int port;
        std::string username;
        std::string password;
        bool useTLS;
    };

    SMTPClient(const Config& config);
    bool sendEmail(const std::string& to, const std::string& subject, const std::string& body);

private:
    Config config;
};
