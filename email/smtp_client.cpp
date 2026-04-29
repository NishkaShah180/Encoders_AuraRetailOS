#include "smtp_client.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <cstdio>
#include <vector>

SMTPClient::SMTPClient(const Config& config) : config(config) {}

bool SMTPClient::sendEmail(const std::string& to, const std::string& subject, const std::string& body) {
    // Create a temporary file for the email content
    std::string tempFile = "temp_mail.txt";
    std::ofstream out(tempFile);
    if (!out.is_open()) return false;

    out << "To: " << to << "\r\n";
    out << "From: " << config.username << "\r\n";
    out << "Subject: " << subject << "\r\n";
    out << "\r\n";
    out << body << "\r\n";
    out.close();

    // Construct the curl command
    // Format: curl --url "smtp://smtp.gmail.com:587" --ssl-reqd --mail-from "sender@gmail.com" 
    //              --mail-rcpt "receiver@gmail.com" --user "user:pass" -T "file.txt"
    
    std::stringstream cmd;
    cmd << "curl.exe --url \"smtp://" << config.host << ":" << config.port << "\" ";
    if (config.useTLS) {
        cmd << "--ssl-reqd ";
    }
    cmd << "--mail-from \"" << config.username << "\" ";
    cmd << "--mail-rcpt \"" << to << "\" ";
    cmd << "--user \"" << config.username << ":" << config.password << "\" ";
    cmd << "-T \"" << tempFile << "\" ";
    cmd << "--silent --show-error"; // Don't show progress bar, but show errors

    // Run the command
    int result = std::system(cmd.str().c_str());

    // Clean up
    std::remove(tempFile.c_str());

    if (result == 0) {
        std::cout << "[SMTP Success] Email sent successfully to " << to << " via curl.exe\n";
        return true;
    } else {
        std::cerr << "[SMTP Error] curl.exe failed with exit code " << result << "\n";
        return false;
    }
}
