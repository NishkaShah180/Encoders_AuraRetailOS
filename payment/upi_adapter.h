#pragma once
#include <iostream>
#include <string>
#include "payment_interface.h"

class RealUPISystem {
public:
    void sendViaUPI(const std::string& upiId, double amount) {
        std::cout << "[UPI System] Transferring Rs." << amount
                  << " via UPI ID: " << upiId << "\n";
    }
};

class UPIAdapter : public IPayment {
    RealUPISystem upiSystem;
    std::string upiId;
public:
    bool pay(double amount) override {
        std::cout << "[UPI Payment] Enter your UPI ID: ";
        std::cin >> upiId;

        if (upiId.find('@') == std::string::npos) {
            std::cout << "[UPI Payment] Invalid UPI ID. Payment failed.\n";
            return false;
        }
        std::cout << "[UPIAdapter] Converting to UPI interface...\n";
        upiSystem.sendViaUPI(upiId, amount);
        std::cout << "Paid Rs." << amount << " using UPI ID: " << upiId << "\n";
        return true;
    }
    std::string getProviderName() { return "UPI"; }
};