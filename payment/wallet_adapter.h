#pragma once
#include <iostream>
#include <string>
#include "payment_interface.h"

class RealWalletSystem {
public:
    void deductWallet(const std::string& walletName, double amount) {
        std::cout << "[Wallet System] Deducting Rs." << amount << " from " << walletName << "\n";
    }
};

class WalletAdapter : public IPayment {
    std::string walletName;
    RealWalletSystem walletSystem;
public:
    WalletAdapter(const std::string& name) : walletName(name) {}

    void pay(double amount) override {
        std::cout << "[WalletAdapter] Converting to Wallet interface...\n";
        walletSystem.deductWallet(walletName, amount);
        std::cout << "Paid Rs." << amount << " using Wallet: " << walletName << "\n";
    }
};