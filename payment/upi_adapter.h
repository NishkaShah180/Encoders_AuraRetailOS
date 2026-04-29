#pragma once
#include <iostream>
#include <string>
#include "payment_interface.h"

// Adaptee — existing UPI system (unchanged)
class RealUPISystem {
public:
    void sendViaUPI(const std::string& upiId, double amount) {
        std::cout << "[UPI System] Transferring Rs." << amount
                  << " via UPI ID: " << upiId << "\n";
    }
};

// Adapter — wraps RealUPISystem to implement IPayment [Adapter Pattern]
class UPIAdapter : public IPayment {
    RealUPISystem upiSystem;
    std::string   savedUpiId;
    bool          initialized = false;   // credentials stored once per session

public:
    // Called ONCE when the customer session starts
    void setupUPI(const std::string& upiId) {
        savedUpiId  = upiId;
        initialized = true;
    }

    // pay() never prompts — uses saved session credentials
    bool pay(double amount) override {
        if (!initialized) {
            std::cout << "[UPI Payment] No UPI ID configured. Payment failed.\n";
            return false;
        }
        std::cout << "[UPIAdapter] Converting to UPI interface...\n";
        upiSystem.sendViaUPI(savedUpiId, amount);
        std::cout << "Paid Rs." << amount << " using UPI ID: " << savedUpiId << "\n";
        return true;
    }

    std::string getProviderName() { return "UPI"; }
};