#pragma once
#include <iostream>
#include <memory>
#include "kiosk_decorator.h"

class NetworkDecorator : public KioskDecorator {
    int signalStrength;

public:
    NetworkDecorator(std::shared_ptr<IKiosk> kiosk, int signal = 94)
        : KioskDecorator(kiosk), signalStrength(signal) {
        std::cout << "[NetworkDecorator] Module attached. "
                  << "Signal strength: " << signalStrength << "%\n";
    }

    void purchaseItem(const std::string& itemName,
                      double amount, IPayment& payment) override {
        std::cout << "[NetworkDecorator] Network check... "
                  << signalStrength << "% signal OK\n";
        wrappedKiosk->purchaseItem(itemName, amount, payment);
    }

    void runDiagnostics() override {
        wrappedKiosk->runDiagnostics();
        std::cout << "[NetworkDecorator] Network: Connected | "
                  << "Signal: " << signalStrength << "%\n";
    }
};