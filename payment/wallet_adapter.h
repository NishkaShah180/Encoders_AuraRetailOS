#pragma once
#include <iostream>
#include <string>
#include "payment_interface.h"

// Adaptee — existing wallet system (unchanged)
class RealWalletSystem {
public:
    void deductWallet(const std::string& walletName, double amount) {
        std::cout << "[Wallet System] Deducting Rs." << amount
                  << " from " << walletName << "\n";
    }
};

// Adapter — wraps RealWalletSystem to implement IPayment [Adapter Pattern]
class WalletAdapter : public IPayment {
    RealWalletSystem walletSystem;
    std::string      savedProvider;
    bool             initialized = false;  // credentials stored once per session

public:
    // Called ONCE when the customer session starts
    void setupWallet(const std::string& provider) {
        savedProvider = provider;
        initialized   = true;
    }

    // pay() never prompts — uses saved session credentials
    bool pay(double amount) override {
        if (!initialized) {
            std::cout << "[Wallet Payment] No wallet configured. Payment failed.\n";
            return false;
        }
        std::cout << "[WalletAdapter] Converting to Wallet interface...\n";
        walletSystem.deductWallet(savedProvider, amount);
        std::cout << "Paid Rs." << amount << " using Wallet: " << savedProvider << "\n";
        return true;
    }

    std::string getProviderName() { return "Wallet (" + savedProvider + ")"; }
};