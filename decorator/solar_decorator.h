#pragma once
#include <iostream>
#include <memory>
#include "kiosk_decorator.h"

class SolarDecorator : public KioskDecorator {
    int batteryLevel;

public:
    SolarDecorator(std::shared_ptr<IKiosk> kiosk, int battery = 87)
        : KioskDecorator(kiosk), batteryLevel(battery) {
        std::cout << "[SolarDecorator] Module attached. "
                  << "Battery level: " << batteryLevel << "%\n";
    }

    void purchaseItem(const std::string& itemName,
                      double amount, IPayment& payment) override {
        std::cout << "[SolarDecorator] Solar power check... "
                  << batteryLevel << "% battery OK\n";
        wrappedKiosk->purchaseItem(itemName, amount, payment);
    }

    void runDiagnostics() override {
        wrappedKiosk->runDiagnostics();
        std::cout << "[SolarDecorator] Solar Power: Active | "
                  << "Battery: " << batteryLevel << "%\n";
    }
};