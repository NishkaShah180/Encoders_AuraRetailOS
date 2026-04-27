#pragma once
#include <iostream>
#include <string>
#include "payment_interface.h"

class RealWalletSystem {
public:
    void deductWallet(const std::string& walletName, double amount) {
        std::cout << "[Wallet System] Deducting Rs." << amount
                  << " from " << walletName << "\n";
    }
};

class WalletAdapter : public IPayment {
    RealWalletSystem walletSystem;
     std::string walletName;
public:
   bool pay(double amount) override {
        std::string pin;
        std::cout << "[Wallet Payment] Enter Wallet Name: ";
        std::cin >> walletName;
        std::cout << "[Wallet Payment] Enter Wallet PIN: ";
        std::cin >> pin;

        if (pin.length() < 4) {
            std::cout << "[Wallet Payment] Invalid PIN. Payment failed.\n";
            return false;
        }
        std::cout << "[WalletAdapter] Converting to Wallet interface...\n";
        walletSystem.deductWallet(walletName, amount);
        std::cout << "Paid Rs." << amount << " using Wallet: " << walletName << "\n";
        return true;
    }

    std::string getProviderName() { return "Wallet"; }
};