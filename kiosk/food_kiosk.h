#pragma once
#include <iostream>
#include <memory>
#include "kiosk_interface.h"

class FoodKiosk : public IKiosk {
public:
    FoodKiosk(const std::string& location, std::shared_ptr<IDispenser> disp)
        : IKiosk(location, "FoodKiosk_User", disp) {
        std::cout << "[FoodKiosk] Initialized at " << location << "\n";
        std::cout << "[FoodKiosk] Active hardware: " << dispenser->getDispenserType() << "\n";
    }

    void purchaseItem(const std::string& itemName, double amount, IPayment& payment) override {
        std::cout << "\n[FoodKiosk] Customer requests: " << itemName << "\n";
        if (!inventory.checkStock(itemName)) {
            std::cout << "[FoodKiosk] Item out of stock.\n";
            return;
        }
        payment.pay(amount);
        dispenser->dispense(itemName);
        inventory.updateStock(itemName, -1);
        std::cout << "Purchase of " << itemName << " successful at FoodKiosk.\n";
    }

    void runDiagnostics() override {
        std::cout << "[FoodKiosk] Diagnostics OK | Hardware: "
                  << dispenser->getDispenserType()
                  << " | Location: " << location << "\n";
    }
};