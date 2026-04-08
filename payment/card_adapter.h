#pragma once
#include <iostream>
#include <string>
#include "payment_interface.h"

class RealCardSystem {
public:
    void swipeCard(const std::string& cardNumber, double amount) {
        std::cout << "[Card System] Charging Rs." << amount
                  << " to card ending in " << cardNumber.substr(cardNumber.size() - 4) << "\n";
    }
};

class CardAdapter : public IPayment {
    std::string cardNumber;
    RealCardSystem cardSystem;
public:
    CardAdapter(const std::string& number) : cardNumber(number) {}

    void pay(double amount) override {
        std::cout << "[CardAdapter] Converting to Card interface...\n";
        cardSystem.swipeCard(cardNumber, amount);
        std::cout << "Paid Rs." << amount << " using Card ending in: "
                  << cardNumber.substr(cardNumber.size() - 4) << "\n";
    }
};