#pragma once
#include <string>
#include <vector>
#include <memory>
#include "smtp_client.h"

struct CartItemInfo {
    std::string itemName;
    int quantity;
    double subtotal;
};

struct PurchaseInfo {
    std::vector<CartItemInfo> items;
    double totalAmount;
    std::string paymentMethod;
    std::string location;
};

class EmailService {
public:
    EmailService();
    bool validateEmail(const std::string& email);
    
    // Sync sending
    bool sendOTP(const std::string& to, const std::string& name, const std::string& otp);
    bool sendReceipt(const std::string& to, const std::string& name, const PurchaseInfo& purchase);

    // Async sending wrapper
    void sendOTPAsync(const std::string& to, const std::string& name, const std::string& otp);
    void sendReceiptAsync(const std::string& to, const std::string& name, const PurchaseInfo& purchase);

private:
    std::unique_ptr<SMTPClient> smtpClient;
    void loadConfig();
    SMTPClient::Config config;
    bool configLoaded = false;
};
