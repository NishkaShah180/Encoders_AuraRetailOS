#pragma once
#include <iostream>
#include <string>
#include "payment_interface.h"

class RealCardSystem {
public:
    void swipeCard(const std::string& cardNumber, double amount) {
        std::cout << "[Card System] Charging Rs." << amount
                  << " to card ending in "
                  << cardNumber.substr(cardNumber.size() - 4) << "\n";
    }
};

class CardAdapter : public IPayment {
    RealCardSystem cardSystem;
    std::string cardNumber;
public:
    bool pay(double amount) override {
        std::string cvv;
        std::cout << "[Card Payment] Enter Card Number (any digits): ";
        std::cin >> cardNumber;

        // pad to at least 4 chars for display safety
        while (cardNumber.size() < 4) cardNumber = "0" + cardNumber;

        std::cout << "[Card Payment] Enter CVV: ";
        std::cin >> cvv;

        std::cout << "[CardAdapter] Card **** **** **** "
                << cardNumber.substr(cardNumber.size() - 4) << " accepted.\n";
        cardSystem.swipeCard(cardNumber, amount);
        std::cout << "Paid Rs." << amount << " via Card.\n";
        return true;
    }
    std::string getProviderName() { return "Card"; }
};