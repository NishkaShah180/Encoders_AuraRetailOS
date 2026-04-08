#pragma once
#include <iostream>
#include <string>
#include "payment_interface.h"

class RealUPISystem {
public:
    void sendViaUPI(const std::string& upiId, double amount) {
        std::cout << "[UPI System] Transferring Rs." << amount << " via UPI ID: " << upiId << "\n";
    }
};

class UPIAdapter : public IPayment {
    std::string upiId;
    RealUPISystem upiSystem;
public:
    UPIAdapter(const std::string& id) : upiId(id) {}

    void pay(double amount) override {
        std::cout << "[UPIAdapter] Converting to UPI interface...\n";
        upiSystem.sendViaUPI(upiId, amount);
        std::cout << "Paid Rs." << amount << " using UPI ID: " << upiId << "\n";
    }
};