#pragma once
#include <iostream>
#include <memory>
#include "kiosk_interface.h"

class EmergencyKiosk : public IKiosk {
public:
    EmergencyKiosk(const std::string& location, std::shared_ptr<IDispenser> disp)
        : IKiosk(location, "EmergencyKiosk_User", disp) {
        std::cout << "[EmergencyKiosk] Initialized at " << location << "\n";
        std::cout << "[EmergencyKiosk] Active hardware: " << dispenser->getDispenserType() << "\n";
    }

    void purchaseItem(const std::string& itemName, double amount, IPayment& payment) override {
        std::cout << "\n[EmergencyKiosk] Distributing: " << itemName << "\n";
        if (!inventory.checkStock(itemName)) {
            std::cout << "[EmergencyKiosk] Item out of stock.\n";
            return;
        }
        payment.pay(amount);
        dispenser->dispense(itemName);
        inventory.updateStock(itemName, -1);
        std::cout << "Emergency item " << itemName << " distributed successfully.\n";
    }

    void runDiagnostics() override {
        std::cout << "[EmergencyKiosk] Diagnostics OK | Hardware: "
                  << dispenser->getDispenserType()
                  << " | Location: " << location << "\n";
    }
};