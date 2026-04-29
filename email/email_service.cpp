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
    // 1. Default values
    config.host = "smtp.gmail.com";
    config.port = 587;
    config.useTLS = true;

    // 2. Try Environment Variables (Most Secure)
    const char* envEmail = std::getenv("SMTP_EMAIL");
    const char* envPass  = std::getenv("SMTP_PASSWORD");

    if (envEmail && envPass) {
        config.username = envEmail;
        config.password = envPass;
        configLoaded = true;
        return;
    }

    // 3. Fallback to smtp_config.json (for local dev)
    std::ifstream file("smtp_config.json");
    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            if (line.find("\"smtp_host\":") != std::string::npos) {
                size_t start = line.find(": \"") + 3;
                size_t end = line.find("\"", start);
                if (start != std::string::npos && end != std::string::npos)
                    config.host = line.substr(start, end - start);
            } else if (line.find("\"email\":") != std::string::npos) {
                size_t start = line.find(": \"") + 3;
                size_t end = line.find("\"", start);
                if (start != std::string::npos && end != std::string::npos)
                    config.username = line.substr(start, end - start);
            } else if (line.find("\"app_password\":") != std::string::npos) {
                size_t start = line.find(": \"") + 3;
                size_t end = line.find("\"", start);
                if (start != std::string::npos && end != std::string::npos)
                    config.password = line.substr(start, end - start);
            }
        }
        file.close();
    }

    if (!config.username.empty() && !config.password.empty()) {
        configLoaded = true;
    } else {
        std::cerr << "[WARNING] SMTP credentials not found in environment or config file.\n";
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
