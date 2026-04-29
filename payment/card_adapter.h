#pragma once
#include <iostream>
#include <string>
#include "payment_interface.h"

// Adaptee — existing card system (unchanged)
class RealCardSystem {
public:
    void swipeCard(const std::string& cardNumber, double amount) {
        std::string last4 = cardNumber.size() >= 4
                          ? cardNumber.substr(cardNumber.size() - 4) : cardNumber;
        std::cout << "[Card System] Charging Rs." << amount
                  << " to card ending in " << last4 << "\n";
    }
};

// Adapter — wraps RealCardSystem to implement IPayment [Adapter Pattern]
class CardAdapter : public IPayment {
    RealCardSystem cardSystem;
    std::string    savedCardNumber;
    std::string    savedExpiry;
    bool           initialized = false;  // credentials stored once per session

public:
    // Called ONCE when the customer session starts
    void setupCard(const std::string& cardNumber, const std::string& expiry) {
        savedCardNumber = cardNumber;
        while (savedCardNumber.size() < 4) savedCardNumber = "0" + savedCardNumber;
        savedExpiry = expiry;
        initialized = true;
    }

    // pay() never prompts — uses saved session credentials
    bool pay(double amount) override {
        if (!initialized) {
            std::cout << "[Card Payment] No card configured. Payment failed.\n";
            return false;
        }
        std::string last4 = savedCardNumber.substr(savedCardNumber.size() - 4);
        std::cout << "[CardAdapter] Card **** **** **** " << last4 << " accepted.\n";
        cardSystem.swipeCard(savedCardNumber, amount);
        std::cout << "Paid Rs." << amount << " via Card ending " << last4 << "\n";
        return true;
    }

    std::string getProviderName() { return "Card"; }
};