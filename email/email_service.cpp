#include "email_service.h"
#include <iostream>
#include <fstream>
#include <regex>
#include <ctime>
#include <iomanip>
#include <sstream>

EmailService::EmailService() {
    loadConfig();
    smtpClient = std::make_unique<SMTPClient>(config);
}

void EmailService::loadConfig() {
    // Default values
    config.host = "smtp.gmail.com";
    config.port = 587;
    config.username = "";
    config.password = "";
    config.useTLS = true;

    std::ifstream file("smtp_config.json");
    if (!file.is_open()) {
        std::cerr << "\n[ERROR] smtp_config.json not found! Real SMTP will likely fail.\n";
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.find("\"smtp_server\":") != std::string::npos) {
            size_t start = line.find(": \"") + 3;
            size_t end = line.find("\"", start);
            config.host = line.substr(start, end - start);
        } else if (line.find("\"smtp_port\":") != std::string::npos) {
            size_t start = line.find(": ") + 2;
            size_t end = line.find_first_of(", \r\n}", start);
            config.port = std::stoi(line.substr(start, end - start));
        } else if (line.find("\"smtp_email\":") != std::string::npos) {
            size_t start = line.find(": \"") + 3;
            size_t end = line.find("\"", start);
            config.username = line.substr(start, end - start);
        } else if (line.find("\"smtp_password\":") != std::string::npos) {
            size_t start = line.find(": \"") + 3;
            size_t end = line.find("\"", start);
            config.password = line.substr(start, end - start);
        }
    }
    file.close();

    if (config.username.empty() || config.password.empty()) {
        std::cerr << "[WARNING] SMTP credentials missing in config file.\n";
    } else {
        configLoaded = true;
    }
}

bool EmailService::validateEmail(const std::string& email) {
    if (email.empty()) return false;
    const std::regex pattern(R"((\w+)(\.|_)?(\w*)@(\w+)(\.(\w+))+)");
    return std::regex_match(email, pattern);
}

bool EmailService::sendOTP(const std::string& to, const std::string& name, const std::string& otp) {
    if (!configLoaded) return false;
    std::string subject = "Aura Retail OS Verification Code";
    std::string body = "Hello " + name + ",\n\n"
                       "Your OTP for Aura Retail OS registration is: " + otp + "\n\n"
                       "This code will expire in 5 minutes.\n\n"
                       "Thank you!";
    return smtpClient->sendEmail(to, subject, body);
}

bool EmailService::sendReceipt(const std::string& to, const std::string& name, const PurchaseInfo& purchase) {
    if (!configLoaded) return false;
    std::time_t t = std::time(nullptr);
    std::tm* now = std::localtime(&t);
    char dateStr[20];
    std::strftime(dateStr, sizeof(dateStr), "%Y-%m-%d %H:%M:%S", now);

    std::string subject = "Aura Retail OS - Transaction Receipt";
    std::stringstream body;
    body << "Thank you for using Aura Retail OS.\n\n"
         << "CUSTOMER RECEIPT\n"
         << "===================================\n"
         << "Customer:  " << name << "\n"
         << "Location:  " << purchase.location << "\n"
         << "Date:      " << dateStr << "\n"
         << "-----------------------------------\n"
         << "Items Purchased:\n";
    
    for (const auto& item : purchase.items) {
        body << "- " << item.itemName << " (x" << item.quantity << ") : Rs." << (int)item.subtotal << "\n";
    }

    body << "-----------------------------------\n"
         << "TOTAL PAID: Rs." << (int)purchase.totalAmount << "\n"
         << "Payment Method: " << purchase.paymentMethod << "\n"
         << "Status: Successful\n"
         << "===================================\n"
         << "We hope to see you again soon!\n";
    
    return smtpClient->sendEmail(to, subject, body.str());
}

// ─── WRAPPERS ────────────────────────────────────────────────────────

void EmailService::sendOTPAsync(const std::string& to, const std::string& name, const std::string& otp) {
    // Fallback to sync for older MinGW
    this->sendOTP(to, name, otp);
}

void EmailService::sendReceiptAsync(const std::string& to, const std::string& name, const PurchaseInfo& purchase) {
    // Fallback to sync for older MinGW
    if (!this->sendReceipt(to, name, purchase)) {
        std::cerr << "\n[ERROR] Failed to send receipt email to " << to << ".\n";
    }
}
